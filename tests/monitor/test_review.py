import importlib.util
import json
from pathlib import Path
import sqlite3
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT/'tools/monitor'))
import review
import worker


def report(evidence=None):
    return {'findings': [dict(title='Delayed tool continuation', severity='high',
        observation='Recorded dispatch follows the tool result by 40 seconds.',
        hypothesis='An unnecessary serial wait may precede dispatch.',
        evidence=evidence or ['e2'], recommendation='Inspect the recorded dispatch wait.',
        acceptance_test='Compare tool-result-to-next-dispatch time on the same task.',
        confidence='medium')]}


class MonitorTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name)/'source.sqlite'
        with sqlite3.connect(self.path) as db:
            db.executescript('''CREATE TABLE echo_events(sequence INTEGER PRIMARY KEY,
                session TEXT,event TEXT,kind TEXT,payload TEXT,occurred TEXT);
                CREATE TABLE echo_runs(run TEXT,session TEXT,objective_event TEXT,last_event TEXT,state TEXT);
                CREATE TABLE echo_tool_calls(state TEXT);''')
            db.executemany('INSERT INTO echo_events VALUES(?,?,?,?,?,?)', [
                (1,'private','e1','user','{"content":"task"}','2026-11-16T00:00:00Z'),
                (2,'private','e2','assistant_final','{"content":"done"}','2026-11-16T00:00:42Z'),
                (3,'public','public-e','user','{"content":"public"}','2026-11-16T00:00:43Z')])
            db.execute('INSERT INTO echo_runs VALUES(?,?,?,?,?)', ('r','private','e1','e2','completed'))

    def test_archive_reader_closes_its_descriptor(self):
        with review.readonly(self.path) as db:
            self.assertEqual(db.execute('SELECT COUNT(*) FROM echo_events').fetchone()[0],3)
        with self.assertRaises(sqlite3.ProgrammingError):
            db.execute('SELECT 1')

    def test_evidence_is_exact_session_and_run_interval(self):
        page = review.evidence_page(self.path, 'private', 'r')
        self.assertEqual([e['event'] for e in page['events']], ['e1','e2'])
        self.assertTrue(page['complete'])
        with self.assertRaises(ValueError):
            review.evidence_page(self.path, 'public', 'r')

    def test_only_settled_runs(self):
        with sqlite3.connect(self.path) as db:
            db.execute("UPDATE echo_runs SET state='generating'")
        with self.assertRaises(ValueError):
            review.evidence_page(self.path, 'private', 'r')

    def test_no_monitor_self_loop(self):
        with sqlite3.connect(self.path) as db:
            db.execute("UPDATE echo_events SET kind='scratchpad_revision' WHERE event='e1'")
        self.assertEqual([e['event'] for e in review.evidence_page(self.path,'private','r')['events']], ['e2'])

    def test_invalid_or_unseen_citations_are_rejected(self):
        for evidence in (['another-user-event'], [], ['e2',42]):
            data=report(); data['findings'][0]['evidence']=evidence
            with self.assertRaises(ValueError):
                review.validate_report(json.dumps(data), {'e1','e2'})

    def test_findings_only_no_actions_or_extra_fields(self):
        data=report(); data['execute']='deploy'
        with self.assertRaises(ValueError):
            review.validate_report(json.dumps(data), {'e2'})
        data=report(); data['findings'][0]['severity']='critical'
        with self.assertRaises(ValueError):
            review.validate_report(json.dumps(data), {'e2'})
        self.assertEqual(review.validate_report('{"findings":[]}',set()), {'findings':[]})

    def test_duplicate_json_keys_rejected(self):
        with self.assertRaises(ValueError):
            review.validate_report('{"findings":[],"findings":[]}',set())

    def test_credentials_redacted_after_json_decoding(self):
        secret='test-secret-credential'
        with sqlite3.connect(self.path) as db:
            db.execute('UPDATE echo_events SET payload=? WHERE event=?',
                       (json.dumps({'password': secret,'nested':{'content':secret}}),'e1'))
        page = review.evidence_page(self.path,'private','r')
        self.assertNotIn(secret,json.dumps(review.redact(page,[secret])))

    def test_bounded_pages_are_explicit(self):
        with sqlite3.connect(self.path) as db:
            db.execute('UPDATE echo_events SET payload=? WHERE event=?',('x'*20000,'e1'))
        page=review.evidence_page(self.path,'private','r')
        self.assertTrue(page['events'][0]['payload_truncated'])
        self.assertTrue(page['bounded_excerpt'])

    def test_first_start_does_not_replay_history(self):
        state=worker.initialise(Path(self.tmp.name)/'state.sqlite')
        worker.discover(state,self.path)
        self.assertEqual(state.execute('SELECT count(*) FROM pending').fetchone()[0],0)
        with sqlite3.connect(self.path) as db:
            db.execute("INSERT INTO echo_events VALUES(4,'private','e4','assistant_final','{}','now')")
            db.execute("INSERT INTO echo_runs VALUES('r2','private','e1','e4','completed')")
        worker.discover(state,self.path)
        self.assertEqual(state.execute('SELECT run FROM pending').fetchall(),[('r2',)])
        state.close()
        state=worker.initialise(Path(self.tmp.name)/'state.sqlite')
        worker.discover(state,self.path)
        self.assertEqual(state.execute('SELECT run FROM pending').fetchall(),[('r2',)])
        state.close()

    def test_stream_deltas_are_assembled_with_exact_provenance(self):
        with sqlite3.connect(self.path) as db:
            db.execute("DELETE FROM echo_events WHERE event='e2'")
            for index in range(500):
                payload=json.dumps({'request_event':'request-1','data':json.dumps(
                    {'choices':[{'delta':{'reasoning_content':'word '},'finish_reason':None}]})})
                db.execute('INSERT INTO echo_events VALUES(?,?,?,?,?,?)',
                    (10+index,'private','chunk-'+str(index),'provider_event',payload,'now'))
            db.execute("INSERT INTO echo_events VALUES(600,'private','end','assistant_final','{}','now')")
            db.execute("UPDATE echo_runs SET last_event='end'")
        page=review.evidence_page(self.path,'private','r')
        self.assertTrue(page['complete'])
        assembled=[e for e in page['events'] if e['kind']=='assembled_provider_text']
        self.assertEqual(len(assembled),1)
        self.assertEqual(assembled[0]['payload']['reasoning_content'],'word '*500)
        self.assertEqual(assembled[0]['source_event_count'],500)
        self.assertEqual((assembled[0]['event'],assembled[0]['last_event']),('chunk-0','chunk-499'))

    def test_daily_storage_budget_blocks_more_reviews(self):
        import time
        state=worker.initialise(Path(self.tmp.name)/'state.sqlite')
        state.execute('INSERT INTO budget VALUES(?,16)',(time.strftime('%Y-%m-%d',time.gmtime()),));state.commit()
        with patch.object(worker,'idle',return_value=True),patch.object(worker,'review') as call:
            self.assertEqual(worker.tick(state,{'enabled':True,'archives':[]},self.tmp.name),'daily-budget')
            call.assert_not_called()
        self.assertEqual(state.execute('PRAGMA max_page_count').fetchone()[0],2048)
        state.close()

    def test_maintenance_defers_background_work(self):
        marker=Path(self.tmp.name)/'maintenance';marker.touch()
        self.assertFalse(worker.idle({'maintenance':str(marker)}))

    def test_busy_foreground_does_not_start_review(self):
        state=worker.initialise(Path(self.tmp.name)/'state.sqlite')
        with patch.object(worker,'idle',return_value=False), patch.object(worker,'review') as call:
            self.assertEqual(worker.tick(state,{'enabled':True},self.tmp.name),'deferred')
            call.assert_not_called()
        state.close()


if __name__ == '__main__':
    unittest.main()
