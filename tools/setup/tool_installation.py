"""Prepare the protected tool worker from pinned inputs; never rewrite prompts.

Installation tooling only. Echo runtime and orchestration are native Ernos.
"""
from pathlib import Path
import hashlib
import importlib.util
import json
import os
import plistlib
import subprocess


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def source_workspace_bindings(pipeline, profiles):
    """Use only root-selected archive/principal and explicit service workspace pairs."""
    mapped = {}
    for name, profile in profiles.items():
        if name not in ('web', 'agent'):
            continue
        environment = dict(item.split('=', 1) for item in profile['environment'])
        key = 'ECHO_WORKSPACE_ROOT' if name == 'web' else 'ECHO_DISCORD_WORKSPACE_ROOT'
        workspace = environment.get(key, '')
        arguments = profile['arguments']
        if len(arguments) < 4 or not Path(workspace).is_absolute() or not Path(arguments[3]).is_absolute():
            raise RuntimeError('Installed archive/workspace binding is missing for ' + name)
        archive = arguments[3]
        if archive in mapped and mapped[archive] != workspace:
            raise RuntimeError('Installed services disagree on workspace root')
        mapped[archive] = workspace
    sources = []
    for source in pipeline['sources']:
        path = source['path']
        principals = source['principals']
        enrolled = json.loads(principals)
        if path not in mapped or not isinstance(enrolled, list) or not enrolled or not all(isinstance(item, str) and item for item in enrolled):
            raise RuntimeError('An enrolled source has no explicit workspace/principal binding')
        sources.append({'path':path, 'principals':principals, 'workspace_root':mapped[path]})
    if not sources or len({item['path'] for item in sources}) != len(sources):
        raise RuntimeError('Missing or duplicated installed archive enrollment')
    return sources


def prepare_tool_installation(stage, spec, root, active):
    if os.geteuid() != 0:
        raise RuntimeError('Protected installation requires macOS administrator authentication')
    settings = spec['tool_installation']
    package = stage / 'tool-fixture'
    installer = package / 'qualify-tool-registration.py'
    manifest = package / 'package.json'
    if sha(installer) != settings['fixture_installer_sha256'] or sha(manifest) != settings['fixture_manifest_sha256']:
        raise RuntimeError('Pinned isolated fixture changed')
    loader = importlib.util.spec_from_file_location('echo_tool_fixture_installation', installer)
    q = importlib.util.module_from_spec(loader)
    loader.loader.exec_module(q)
    # Exercise real isolated review/publication/invocation before exposing the
    # production worker. No consent or success receipt is fabricated here.
    subprocess.run(['/usr/bin/python3', str(installer), str(manifest), '--sha256', settings['fixture_manifest_sha256']], check=True, env={'PATH':'/usr/bin:/bin:/usr/sbin:/sbin'})
    fixture = json.loads(manifest.read_bytes())
    contents = {}
    for name in ['tool_registration_controller', 'tool_registry_check']:
        data = (package/name).read_bytes()
        if hashlib.sha256(data).hexdigest() != fixture['files'][name]['sha256']:
            raise RuntimeError('Protected worker input changed')
        contents[name] = data
    pipeline = q.read_installed(root/'config/pipeline.json')
    original = q.read_installed(Path(pipeline['controller_configuration']))
    profiles = {}
    for entry in active['services']:
        profile_path = Path(entry['profile'])
        profiles[entry['name']] = q.read_installed(profile_path)
        if sha(profile_path) != entry['profile_sha256']:
            raise RuntimeError('Installed service profile changed')
    sources = source_workspace_bindings(pipeline, profiles)
    build = q.read_installed(Path(pipeline['build_plan']))
    if sha(pipeline['build_plan']) != pipeline['build_plan_sha256']:
        raise RuntimeError('Installed build plan changed')
    base = root/'tools'
    q.new_directory(base, 0o755)
    for name, mode in {'registry':0o755,'versions':0o755,'work':0o700,'releases':0o755}.items():
        q.new_directory(base/name, mode)
    version = base/'releases'/settings['fixture_manifest_sha256']
    q.new_directory(version, 0o755)
    for name, mode in {'bin':0o755,'config':0o700,'ledger':0o700,'logs':0o700,'runtime':0o755}.items():
        q.new_directory(version/name, mode)
    for name, data in contents.items():
        q.retain(version/'bin'/name, data, 0o755)
    for name in ['execute_guardian','workspace_database']:
        helper = stage/'files'/name
        if sha(helper) != spec['files'][name]:
            raise RuntimeError('Execution helper changed')
        q.retain(version/'runtime'/name, helper.read_bytes(), 0o755)
    registry = base/'registry/registry.json'
    if not registry.exists():
        q.retain(registry, q.encoded({'format':1,'revision':0,'entries':[]}), 0o644)
    else:
        q.protected(registry)
    inventory = version/'config/inventory.json'
    q.retain(inventory, q.encoded({'version':1,'targets':[{'path':str(registry),'category':'agency'}]}))
    checker = version/'bin/tool_registry_check'
    check_plan = version/'config/qualification.json'
    q.retain(check_plan, q.encoded({'version':1,'checks':[{'name':'protected-registry-integrity','executable':str(checker),'sha256':sha(checker),'arguments':[],'read_trees':[str(base/'registry'),str(base/'versions')],'timeout_ms':60000}]}))
    control = {key:original[key] for key in ['charter','policy','policy_sha256','review_profile','review_profile_sha256','review_prompt','review_prompt_sha256']}
    q.protected(Path(control['charter']))
    for field in ['policy','review_profile','review_prompt']:
        q.protected(Path(control[field]))
        if sha(control[field]) != control[field+'_sha256']:
            raise RuntimeError('Installed review input changed')
    control.update(ledger_directory=str(version/'ledger'),ledger_name='controller.sqlite',inventory=str(inventory),inventory_sha256=sha(inventory),qualification_plan=str(check_plan),qualification_plan_sha256=sha(check_plan),health_plan=str(check_plan),health_plan_sha256=sha(check_plan))
    controller = version/'config/controller.json'
    q.retain(controller, q.encoded(control))
    build_plan = version/'config/build.json'
    compiler_plan = {key:build[key] for key in ['runtime','compiler','compiler_sha256']}
    compiler_plan['entries'] = ['tool.ep']
    q.retain(build_plan, q.encoded(compiler_plan))
    archive = Path('/Volumes/One Touch/Echo-private/generated-tools')
    if archive.is_symlink() or not archive.parent.is_dir() or archive.parent.is_symlink():
        raise RuntimeError('Canonical archival destination is unavailable')
    archive.mkdir(mode=0o700, exist_ok=True)
    if archive.resolve() != archive:
        raise RuntimeError('Canonical archival destination is indirect')
    worker = version/'config/worker.json'
    q.retain(worker, q.encoded({'controller_configuration':str(controller),'registry_directory':str(base/'registry'),'registry_name':'registry.json','tool_directory':str(base/'versions'),'work_directory':str(base/'work'),'archive_directory':str(archive),'runtime':build['runtime'],'build_plan':str(build_plan),'build_plan_sha256':sha(build_plan),'case_timeout_ms':10000,'review_device':'tool-registration-review','sources':sources}))
    initializer = root/'bin/rsi_initialize'
    q.protected(initializer)
    q.run(initializer, controller, 'tool-registration-review')
    q.run(checker, registry)
    job = {'Label':'local.echo.tool-registry','ProgramArguments':[str(version/'bin/tool_registration_controller'),str(worker)],'RunAtLoad':True,'KeepAlive':True,'ProcessType':'Background','ThrottleInterval':10,'WorkingDirectory':str(version),'EnvironmentVariables':{'PATH':'/usr/bin:/bin:/usr/sbin:/sbin'},'StandardOutPath':str(version/'logs/worker.log'),'StandardErrorPath':str(version/'logs/worker-error.log')}
    plist = version/'local.echo.tool-registry.plist'
    q.retain(plist, plistlib.dumps(job), 0o644)
    return {'registry':str(registry),'plist':str(plist),'worker_config':str(worker),'fixture_qualified':True,'execution_runtime':str(version/'runtime')}
