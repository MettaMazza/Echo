#!/usr/bin/env python3
"""Prepare an exact, owner-scoped research evidence catalogue for installation.

This developer/setup utility never runs a model, grants consent, activates
weights or changes a service. Install the printed path/hash together in the
protected service configuration. Rebuild the catalogue to add new records;
the running service refuses bytes different from its configured pin.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat


def read_record(path: Path) -> str:
    path = path.absolute()
    for component in (path, *path.parents):
        if component.is_symlink():
            raise ValueError(f"Symlink is not a retained source: {path}")
    with path.open('rb') as source:
        before = os.fstat(source.fileno())
        if not stat.S_ISREG(before.st_mode) or before.st_nlink != 1:
            raise ValueError(f"Source must be a single-link regular file: {path}")
        raw = source.read()
        after = os.fstat(source.fileno())
        if (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns):
            raise ValueError(f"Source changed while reading: {path}")
    text = raw.decode('utf-8')
    if '\0' in text or not isinstance(json.loads(text), dict):
        raise ValueError(f"Expected a JSON experiment object: {path}")
    return text


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--principal', action='append', required=True, help='Existing owner principal; repeat only for verified aliases belonging to that owner.')
    parser.add_argument('--record', type=Path, action='append', required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if len(set(args.principal)) != len(args.principal) or any(not owner.strip() or any(ord(char) < 32 for char in owner) for owner in args.principal):
        raise ValueError('A nonempty canonical principal is required')
    records = []
    seen = set()
    for source in args.record:
        source = source.absolute()
        if source in seen:
            raise ValueError(f'Duplicate source: {source}')
        seen.add(source)
        original = read_record(source)
        for owner in args.principal:
            records.append({'principal': owner, 'source': str(source),
                            'sha256': hashlib.sha256(original.encode()).hexdigest(),
                            'original_record': original})
    payload = (json.dumps({'format': 1, 'records': records}, ensure_ascii=False, indent=2) + '\n').encode()
    output = args.output.absolute()
    with output.open('xb') as target:
        os.fchmod(target.fileno(), 0o600)
        target.write(payload)
        target.flush()
        os.fsync(target.fileno())
    print(json.dumps({'catalog': str(output), 'sha256': hashlib.sha256(payload).hexdigest(),
                      'records': len(records), 'principal': args.principal,
                      'environment_keys': ['ECHO_RESEARCH_CATALOG', 'ECHO_RESEARCH_CATALOG_SHA256'],
                      'installed': False}, indent=2))


if __name__ == '__main__':
    main()
