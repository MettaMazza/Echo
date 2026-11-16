"""Exercise the actual generated runtime against a contending SQLite writer.
Usage: python3 tests/runtime/test_sqlite_gc.py path/to/discord_agent_compiled.c
"""
from pathlib import Path
import json, subprocess, sys, tempfile
source = Path(sys.argv[1]).read_text().replace('int main(int argc, char** argv) {', 'int original_main(int argc, char** argv) {', 1)
source += Path(__file__).with_name('sqlite_gc_fixture.c').read_text()
with tempfile.TemporaryDirectory(prefix='echo-sqlite-gc-') as directory:
    root = Path(directory)
    (root/'test.c').write_text(source)
    with (root/'compile.log').open('w') as log:
        result = subprocess.run(['clang', str(root/'test.c'), '-o', str(root/'test'), '-lpthread', '-DEP_HAS_SQLITE', '-lsqlite3'], stdout=log, stderr=log)
    if result.returncode:
        raise RuntimeError((root/'compile.log').read_text())
    for mode in ['0', '1']:
        result = subprocess.run([str(root/'test'), str(root/(mode+'.sqlite')), mode], capture_output=True, text=True, timeout=30, check=True)
        report = json.loads(result.stdout)
        assert report['milliseconds'] < 100, report
        assert report['writer_and_roots_ok'] == 1 and report['parked_after'] == 0, report
        assert report['callback_result'] == 0 and report['callback_rows'] == 1000, report
        print('PASS', 'exec' if mode == '0' else 'step', json.dumps(report))
