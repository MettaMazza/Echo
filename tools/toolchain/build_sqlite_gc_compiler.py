"""Build the SQLite-safe Ernos compiler from its canonical runtime sources.
Usage: python3 build_sqlite_gc_compiler.py COMPILER_ROOT [BOOTSTRAP_BINARY]
Produces epc_latency_gc; does not replace a running compiler or service.
"""
from pathlib import Path
import hashlib,json,subprocess,sys
root=Path(sys.argv[1]).resolve();here=Path(__file__).resolve().parent
bootstrap=Path(sys.argv[2]).resolve() if len(sys.argv)>2 else root/'epc_gc_literal'
metadata=json.loads((here/'sqlite-gc-safepoints.json').read_text())
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
runtime=root/'runtime/ep_runtime.c'
if digest(runtime)==metadata['before']:
    subprocess.run(['patch','--batch','--forward','-p1','-i',str(here/'sqlite-gc-safepoints.patch')],cwd=root,check=True)
assert digest(runtime)==metadata['after'], 'Runtime differs: review and port the patch explicitly.'
subprocess.run([str(bootstrap),'tools/gen_runtime_ep.ep'],cwd=root,check=True)
subprocess.run([str(root/'gen_runtime_ep')],cwd=root,check=True)
(root/'epc_latency_gc.ep').write_bytes((root/'epc_gc_literal.ep').read_bytes())
subprocess.run([str(bootstrap),'epc_latency_gc.ep'],cwd=root,check=True)
print(json.dumps({'compiler':str(root/'epc_latency_gc'),'sha256':digest(root/'epc_latency_gc'),'runtime_sha256':digest(runtime)},indent=2))
