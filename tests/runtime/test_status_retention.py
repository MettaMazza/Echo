"""Deletion boundaries: background receipts expire without touching user data."""
import importlib.util,tempfile
from pathlib import Path
path=Path(__file__).resolve().parents[2]/'tools/setup/prune-discord-status-receipts.py'
spec=importlib.util.spec_from_file_location('status_retention',path)
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
with tempfile.TemporaryDirectory() as directory:
    base=Path(directory)/'receipts';base.mkdir()
    rotation=base/'rest-rotation-test'
    for service in ['activity','queue_controls','agent','actions']:
        for day in [10,13,20]:
            target=rotation/service/'receipts'/str(day);target.mkdir(parents=True)
            (target/'receipt').write_text('fixture')
    outside=Path(directory)/'unrelated';outside.mkdir()
    (outside/'preserve').write_text('fixture')
    (rotation/'activity/receipts/1').symlink_to(outside,target_is_directory=True)
    (base/'agent.sqlite').write_text('preserve')
    result=module.prune(base,now=20*86400)
    assert len(result['removed'])==2
    for service in ['activity','queue_controls','agent','actions']:
        for day in [13,20]:assert (rotation/service/'receipts'/str(day)/'receipt').exists()
    for service in ['agent','actions']:assert (rotation/service/'receipts/10/receipt').exists()
    assert (outside/'preserve').exists() and (base/'agent.sqlite').exists()
    assert not module.prune(base,now=20*86400)['removed']
print('PASS: expired background receipts only; boundary/current days, other services, databases, symlink targets and repeated cleanup preserved')
