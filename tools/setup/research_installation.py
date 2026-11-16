"""Install pinned historical research records without changing model state."""
from pathlib import Path
import hashlib
import json
import os
import sqlite3


def validate_catalog(raw, allowed):
    catalog = json.loads(raw)
    if catalog.get('format') != 1 or not isinstance(catalog.get('records'), list):
        raise ValueError('Invalid research catalogue')
    seen = set()
    for record in catalog['records']:
        owner, source = record['principal'], record['source']
        original = record['original_record']
        if owner not in allowed or not Path(source).is_absolute() or (owner, source) in seen:
            raise ValueError('Unenrolled, duplicated or invalid research source')
        if not isinstance(original, str) or not isinstance(json.loads(original), dict):
            raise ValueError('Research source is not an original JSON object')
        if hashlib.sha256(original.encode()).hexdigest() != record['sha256']:
            raise ValueError('Research record hash mismatch')
        seen.add((owner, source))
    return catalog


def prepare_research_installation(stage, spec, root, active):
    if os.geteuid() != 0:
        raise RuntimeError('Protected installation requires administrator authentication')
    expected = spec['research_catalog']['sha256']
    raw = (stage/'research-catalog.json').read_bytes()
    if hashlib.sha256(raw).hexdigest() != expected:
        raise ValueError('Pinned research catalogue changed')
    allowed = set()
    for entry in active['services']:
        if entry['name'] not in ('web', 'agent'):
            continue
        profile_raw = Path(entry['profile']).read_bytes()
        if hashlib.sha256(profile_raw).hexdigest() != entry['profile_sha256']:
            raise ValueError('Installed profile changed')
        profile = json.loads(profile_raw)
        archive = Path(profile['arguments'][3])
        with sqlite3.connect(archive.as_uri()+'?mode=ro', uri=True) as database:
            allowed.update(row[0] for row in database.execute("SELECT identity FROM echo_principals WHERE role IN ('admin','steward') AND revoked=0"))
    validate_catalog(raw, allowed)
    launcher = json.loads((root/'config/launcher.json').read_bytes())
    uid, gid = int(launcher['uid']), int(launcher['gid'])
    parent = root/'research'
    if parent.is_symlink():
        raise ValueError('Research directory is linked')
    parent.mkdir(mode=0o755, exist_ok=True)
    info = parent.stat()
    if info.st_uid != 0 or info.st_mode & 0o022:
        raise ValueError('Research installation directory is not protected')
    destination = parent/(expected+'.json')
    if destination.exists() or destination.is_symlink():
        info = destination.lstat()
        if destination.is_symlink() or not destination.is_file() or info.st_nlink != 1 or info.st_uid != uid or info.st_mode & 0o077 or destination.read_bytes() != raw:
            raise ValueError('Different research catalogue already installed')
    else:
        with destination.open('xb') as output:
            os.fchmod(output.fileno(), 0o400)
            os.fchown(output.fileno(), uid, gid)
            output.write(raw)
            output.flush()
            os.fsync(output.fileno())
    return {'ECHO_RESEARCH_CATALOG': str(destination), 'ECHO_RESEARCH_CATALOG_SHA256': expected}
