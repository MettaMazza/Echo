#!/usr/bin/env python3
"""Prepare an inactive per-user launchd schedule for the native Gmail runner.

This setup utility writes artifacts only: it does not bootstrap a service,
access Gmail, install credentials, or modify an existing Echo profile.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import plistlib


def absolute(value):
    path = Path(value)
    if not path.is_absolute():
        raise ValueError('Service paths must be absolute')
    return path


def prepare(binary, archive, account, mailbox, interval, secret_roots, output):
    binary, archive, mailbox, output = map(absolute, (binary, archive, mailbox, output))
    if not binary.is_file() or not os.access(binary, os.X_OK):
        raise ValueError('Native Gmail sync executable is unavailable')
    if not archive.is_file() or not mailbox.is_dir():
        raise ValueError('Existing Echo archive and private mailbox directory are required')
    if not account or '\x00' in account or interval < 1:
        raise ValueError('Account and a positive scheduling interval are required')
    if not isinstance(secret_roots, list) or any(not isinstance(p, str) or not Path(p).is_absolute() for p in secret_roots):
        raise ValueError('Secret exclusions must be a JSON array of absolute paths')
    roots = list(dict.fromkeys([*secret_roots, str(mailbox)]))
    identifier = hashlib.sha256(account.encode()).hexdigest()
    label = 'com.echo.gmail-sync.' + identifier[:16]
    stop = mailbox / 'sync.stop'
    profile = {
        'Label': label,
        'ProgramArguments': [str(binary), str(archive), account, str(stop)],
        'EnvironmentVariables': {
            'ECHO_MAIL_ROOT': str(mailbox),
            'ECHO_HOST_SECRET_ROOTS': json.dumps(roots, separators=(',', ':')),
        },
        'WorkingDirectory': str(mailbox),
        'StartInterval': interval,
        'RunAtLoad': False,
        'KeepAlive': False,
        'ProcessType': 'Background',
        'StandardOutPath': str(mailbox / 'sync.log'),
        'StandardErrorPath': str(mailbox / 'sync-errors.log'),
        'Umask': 0o077,
    }
    # Exclusive creation preserves earlier prepared/installed profiles.
    output.mkdir(mode=0o700)
    data = plistlib.dumps(profile, sort_keys=True)
    target = output / (label + '.plist')
    with target.open('xb') as stream:
        stream.write(data)
    target.chmod(0o600)
    receipt = {
        'format': 1,
        'label': label,
        'plist': str(target),
        'plist_sha256': hashlib.sha256(data).hexdigest(),
        'binary': str(binary),
        'binary_sha256': hashlib.sha256(binary.read_bytes()).hexdigest(),
        'stop_file': str(stop),
        'activated': False,
    }
    with (output / 'schedule.json').open('x') as stream:
        json.dump(receipt, stream, indent=2)
        stream.write('\n')
    (output / 'schedule.json').chmod(0o600)
    return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('binary', 'archive', 'account', 'mailbox', 'output'):
        parser.add_argument('--' + name, required=True)
    parser.add_argument('--interval-seconds', type=int, default=300)
    parser.add_argument('--secret-roots', required=True, help='Existing host secret-root exclusions as a JSON array')
    args = parser.parse_args()
    receipt = prepare(args.binary, args.archive, args.account, args.mailbox,
                      args.interval_seconds, json.loads(args.secret_roots), args.output)
    print(json.dumps(receipt, indent=2))
    print('Schedule prepared only. Production installation and activation remain separate.')


if __name__ == '__main__':
    main()
