"""Round-trip the compiled inference encoder, including missing/null/type cases."""
import json, pathlib, subprocess, sys, tempfile

def decode(encoded):
    if encoded == []:
        return []
    result = []
    for entry in encoded['records']:
        record = {**encoded['shared_fields'], **entry}
        for key, cell in record.items():
            if key in encoded['reference_fields'] and cell is not None:
                record[key] = encoded['references'][cell] if isinstance(cell, str) else cell['literal']
        result.append(record)
    return result

def check(binary, records, directory):
    source = directory / 'records.json'
    source.write_text(json.dumps(records, ensure_ascii=False))
    run = subprocess.run([binary, str(source)], capture_output=True, text=True, check=True)
    encoded = json.loads(run.stdout)
    assert decode(encoded) == records, (records, encoded)
    return encoded

if __name__ == '__main__':
    binary = str(pathlib.Path(sys.argv[1]).resolve())
    with tempfile.TemporaryDirectory(prefix='echo-recall-codec-') as tmp:
        directory = pathlib.Path(tmp)
        fixtures = [[], [{}, {}], [
            {'id': 'claim:' + 'ab12' * 16, 'identity': 'claim:' + 'ab12' * 16,
             'source_session': 'session:' + 'cd34' * 16, 'source_event': 'source:42',
             'source_digest': 'ef56' * 16, 'quotation': 'Exact café 🌈 quote: "hello"\nnext line',
             'source_sequence': 9007199254740993, 'event_time': '', 'valid_until': None,
             'extra': {'$missing': 1, 'ref': 2}, 'literal_bool': True},
            {'id': 'claim:' + 'ab12' * 16, 'identity': 'claim:' + 'ab12' * 16,
             'source_session': 'session:' + 'cd34' * 16, 'source_event': None,
             'source_digest': 'ef56' * 16, 'quotation': 'Unchanged second quotation.',
             'source_sequence': 3, 'extra': [1, 'value', False], 'literal_bool': False},
            {'source_event': {'literal': 'object'}, 'source_session': 7,
             'source_digest': True, 'quotation': 'Absent is different from null.'}
        ]]
        for fixture in fixtures:
            check(binary, fixture, directory)
        print('PASS: compiled encoder round-trips every record, Unicode quotation, identifier, integer, null, absent field and nested value exactly.')
