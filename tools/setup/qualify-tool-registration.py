#!/usr/bin/env python3
"""Install and exercise an isolated registration fixture; never alter live Echo."""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import sqlite3
import stat
import subprocess


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def encoded(value) -> bytes:
    return (json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(',', ':')) + '\n').encode()


def protected(path: Path) -> None:
    for part in [*reversed(path.parents), path]:
        info = part.lstat()
        if stat.S_ISLNK(info.st_mode) or info.st_uid != 0 or info.st_mode & 0o022:
            raise RuntimeError(f'Unprotected installed input: {part}')
    if path.is_file() and path.stat().st_nlink != 1:
        raise RuntimeError(f'Linked installed input: {path}')


def read_installed(path: Path):
    protected(path)
    return json.loads(path.read_bytes())


def new_directory(path: Path, mode: int) -> None:
    if path.exists():
        protected(path)
        if not path.is_dir() or stat.S_IMODE(path.stat().st_mode) != mode:
            raise RuntimeError(f'Existing directory has a different mode: {path}')
        return
    protected(path.parent)
    path.mkdir(mode=mode)
    os.chmod(path, mode)


def retain(path: Path, data: bytes, mode: int = 0o600) -> None:
    if path.exists():
        protected(path)
        if path.read_bytes() != data or stat.S_IMODE(path.stat().st_mode) != mode:
            raise RuntimeError(f'Existing file differs; preserved: {path}')
        return
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, mode)
    with os.fdopen(fd, 'wb') as handle:
        handle.write(data)
        handle.flush()
        os.fchmod(handle.fileno(), mode)
        os.fsync(handle.fileno())
    fd = os.open(path.parent, os.O_RDONLY | os.O_DIRECTORY)
    try:
        os.fsync(fd)
    finally:
        os.close(fd)


def run(*args: object) -> None:
    subprocess.run([str(arg) for arg in args], check=True, env={"PATH":"/usr/bin:/bin:/usr/sbin:/sbin"})


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('package', type=Path)
    parser.add_argument('--sha256', required=True)
    parser.add_argument('--check-only', action='store_true')
    parser.add_argument('--pipeline', type=Path, default=Path('/Library/Application Support/EchoRSI/config/pipeline.json'))
    args = parser.parse_args()
    manifest_bytes = args.package.read_bytes()
    if sha(manifest_bytes) != args.sha256:
        raise RuntimeError('Package manifest differs from the explicitly pinned bytes')
    package = json.loads(manifest_bytes)
    required = {'tool_registration_controller', 'tool_registry_check', 'test_registered_workflow', 'execute_guardian', 'tool_behavior_probe', 'tool_behavior_probe.ep'}
    files = package['files']
    if set(files) != required:
        raise RuntimeError('Package must contain exactly the fixture worker/checker/guardian and source inputs')
    contents = {}
    for name, record in files.items():
        path = args.package.parent / name
        if path.is_symlink() or not path.is_file():
            raise RuntimeError(f'Package file is missing or linked: {name}')
        contents[name] = path.read_bytes()
        if sha(contents[name]) != record['sha256']:
            raise RuntimeError(f'Package file changed: {name}')
    if args.check_only:
        print('PASS: complete isolated fixture package matches all pinned hashes; no installation performed.')
        return
    if os.geteuid() != 0:
        raise RuntimeError('macOS administrator authentication is required to install this isolated fixture')
    pipeline = read_installed(args.pipeline)
    original = read_installed(Path(pipeline['controller_configuration']))
    build = read_installed(Path(pipeline['build_plan']))
    if sha(Path(pipeline['build_plan']).read_bytes()) != pipeline['build_plan_sha256']:
        raise RuntimeError('Installed compiler plan changed')
    charter = read_installed(Path(original['charter']))
    if 'agency' not in charter['rights']['informed_integrity_and_consent']['consent_required']:
        raise RuntimeError('Installed charter does not classify agency as consent-required')
    parent = Path('/Library/Application Support/EchoToolQualification')
    new_directory(parent, 0o755)
    root = parent / args.sha256
    new_directory(root, 0o755)
    for name, mode in {'bin':0o755, 'config':0o700, 'registry':0o755, 'versions':0o755, 'ledger':0o700, 'work':0o700, 'workspaces':0o700, 'source':0o700, 'runtime':0o755}.items():
        new_directory(root / name, mode)
    for name, content in contents.items():
        retain(root / 'bin' / name, content, 0o644 if name.endswith('.ep') else 0o755)
    retain(root / 'runtime' / 'execute_guardian', contents['execute_guardian'], 0o755)
    retain(root / 'package.json', manifest_bytes)
    marker = root / 'qualified.json'
    if marker.exists():
        previous = read_installed(marker)
        if previous.get('package_sha256') != args.sha256:
            raise RuntimeError('Existing qualification receipt differs')
        print(f'Already qualified; retained receipt: {marker}')
        return
    registry = root / 'registry' / 'registry.json'
    if not registry.exists():
        retain(registry, encoded({'format':1,'revision':0,'entries':[]}), 0o644)
    else:
        protected(registry)
    inventory = root / 'config' / 'inventory.json'
    retain(inventory, encoded({'version':1,'targets':[{'path':str(registry),'category':'agency'}]}))
    checker = root / 'bin' / 'tool_registry_check'
    check_plan = root / 'config' / 'qualification.json'
    retain(check_plan, encoded({'version':1,'checks':[{'name':'protected-registry-integrity','executable':str(checker),'sha256':sha(contents['tool_registry_check']),'arguments':[],'read_trees':[str(root/'registry'),str(root/'versions')],'timeout_ms':60000}]}))
    control = {key:original[key] for key in ['charter','policy','policy_sha256','review_profile','review_profile_sha256','review_prompt','review_prompt_sha256']}
    for field in ['policy','review_profile','review_prompt']:
        protected(Path(control[field]))
        if sha(Path(control[field]).read_bytes()) != control[field+'_sha256']:
            raise RuntimeError(f'Installed {field} changed; no replacement prompt or profile is substituted')
    control.update(ledger_directory=str(root/'ledger'),ledger_name='controller.sqlite',inventory=str(inventory),inventory_sha256=sha(inventory.read_bytes()),qualification_plan=str(check_plan),qualification_plan_sha256=sha(check_plan.read_bytes()),health_plan=str(check_plan),health_plan_sha256=sha(check_plan.read_bytes()))
    controller_config = root / 'config' / 'controller.json'
    retain(controller_config, encoded(control))
    build_plan = root / 'config' / 'build.json'
    compiler_plan = {key:build[key] for key in ['runtime','compiler','compiler_sha256']}
    compiler_plan['entries'] = ['tool.ep']
    retain(build_plan, encoded(compiler_plan))
    archive_parent = Path('/Volumes/One Touch/Echo-private')
    # External copies are archival data, not protected executable authority.
    if not archive_parent.is_dir() or archive_parent.is_symlink():
        raise RuntimeError('One Touch archival parent unavailable')
    archive = archive_parent / ('tool-registration-qualified-' + args.sha256)
    if not archive.exists():
        archive.mkdir(mode=0o700)
    if archive.is_symlink() or not archive.is_dir():
        raise RuntimeError('Archival destination is not a real directory')
    database = root / 'source' / 'agent.sqlite'
    worker_config = root / 'config' / 'worker.json'
    retain(worker_config, encoded({'controller_configuration':str(controller_config),'registry_directory':str(root/'registry'),'registry_name':'registry.json','tool_directory':str(root/'versions'),'work_directory':str(root/'work'),'archive_directory':str(archive),'runtime':build['runtime'],'build_plan':str(build_plan),'build_plan_sha256':sha(build_plan.read_bytes()),'case_timeout_ms':10000,'review_device':'tool-registration-review','sources':[{'path':str(database),'principals':json.dumps(['device:fixture']),'workspace_root':str(root/'workspaces')}]}))
    source = contents['tool_behavior_probe.ep'].decode()
    cases = []
    for name, value in [('simple',{'value':1}),('unicode',{'value':'café 🪞'})]:
        output = json.dumps(value, ensure_ascii=False, separators=(',', ':')) + '\n'
        cases.append(json.dumps({'name':name,'input':value,'stdout':output,'exit':0},ensure_ascii=False,separators=(',', ':')))
    candidate = {'name':'fixture_tool','version':1,'purpose':'Isolated registration workflow fixture; JSON input/output probe','source':'tool.ep','source_sha256':sha(contents['tool_behavior_probe.ep']),'artifact':'tool','artifact_sha256':sha(contents['tool_behavior_probe']),'input_schema':{'type':'object'},'output_schema':{'type':'object'},'capabilities':['workspace.execute-isolated'],'dependencies':[],'tests':cases,'side_effects':[],'discovery_receipts':['BIND_REAL_DISCOVERY_RECEIPTS'],'failure_policy':'Fail without publication; preserve accepted registry.','gap':'Exercise the protected registration mechanism; no claim of a missing production semantic capability.','composition_decision':'Explicit fixture for registration acceptance, not a production reuse decision.'}
    fixture = root / 'config' / 'fixture.json'
    retain(fixture, encoded({'database':str(database),'workspace_root':str(root/'workspaces'),'execution_runtime':str(root/'runtime'),'registry':str(registry),'source_text':source,'manifest':json.dumps(candidate,ensure_ascii=False,separators=(',', ':'))}))
    initializer = Path('/Library/Application Support/EchoRSI/bin/rsi_initialize')
    protected(initializer)
    run(initializer, controller_config, 'tool-registration-review')
    if not database.exists():
        run(root/'bin'/'test_registered_workflow', 'seed', fixture)
    run(root/'bin'/'tool_registration_controller', worker_config, '--once')
    ledger = sqlite3.connect(f'file:{root / "ledger" / "controller.sqlite"}?mode=ro', uri=True)
    terminals = [row[0] for row in ledger.execute("SELECT payload FROM echo_events WHERE kind='tool_registration_terminal'")]
    ledger.close()
    if terminals != ['activated-and-verified']:
        raise RuntimeError(f'Protected worker did not confirm publication: {terminals!r}. All state preserved; no production changes.')
    run(root/'bin'/'test_registered_workflow', 'invoke', fixture)
    for action in ['quarantined', 'active', 'retired']:
        run(root/'bin'/'test_registered_workflow', 'request-' + action, fixture)
        run(root/'bin'/'tool_registration_controller', worker_config, '--once')
        ledger = sqlite3.connect(f'file:{root / "ledger" / "controller.sqlite"}?mode=ro', uri=True)
        lifecycle = [row[0] for row in ledger.execute("SELECT payload FROM echo_events WHERE kind='tool_version_terminal' ORDER BY sequence")]
        ledger.close()
        if len(lifecycle) != ['quarantined', 'active', 'retired'].index(action) + 1 or any(value != 'activated-and-verified' for value in lifecycle):
            raise RuntimeError(f'Lifecycle publication failed: {lifecycle!r}; retained all state.')
        run(root/'bin'/'test_registered_workflow', 'verify-' + action, fixture)
    run(checker, registry)
    retain(marker, encoded({'package_sha256':args.sha256,'status':'qualified-isolated-fixture','registry_sha256':sha(registry.read_bytes()),'worker_result':terminals[0],'lifecycle':['quarantined','active','retired'],'production_changed':False}))
    print(f'PASS: isolated protected registration and same-run/reopen invocation. Receipt: {marker}')


if __name__ == '__main__':
    main()
