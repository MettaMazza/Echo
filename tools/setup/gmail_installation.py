"""Prepare private Gmail storage/profile bindings without OAuth or consent.

Installation tooling only. Runtime mailbox work remains native Ernos.
"""
from pathlib import Path
import json
import os
import stat


def gmail_profile_environment(environment, mailbox_root, discord=False):
    result = dict(environment)
    path = Path(mailbox_root)
    if not path.is_absolute() or '..' in path.parts:
        raise ValueError('Mailbox storage path must be absolute and canonical')
    roots = json.loads(result.get('ECHO_HOST_SECRET_ROOTS', '[]'))
    if not isinstance(roots, list) or any(not isinstance(p, str) or not Path(p).is_absolute() for p in roots):
        raise ValueError('Existing host secret exclusions are invalid')
    if str(path) not in roots:
        roots.append(str(path))
    result['ECHO_HOST_SECRET_ROOTS'] = json.dumps(roots, separators=(',', ':'))
    result['ECHO_MAIL_ROOT'] = str(path)
    if discord:
        result['ECHO_DISCORD_MAIL_ROOT'] = str(path)
        for suffix in ('POLICY_PATH', 'POLICY_SHA256'):
            generic = 'ECHO_MAIL_' + suffix
            if generic in result:
                result['ECHO_DISCORD_MAIL_' + suffix] = result[generic]
    return result


def prepare_gmail_installation(spec, root):
    if os.geteuid() != 0:
        raise RuntimeError('Protected installation requires macOS administrator authentication')
    mailbox = Path(spec['mail_installation']['mail_root'])
    # Validate the path before creating anything; do not adopt someone else's
    # directory, follow a link, or reset permissions on pre-existing mail.
    gmail_profile_environment({}, str(mailbox))
    if mailbox.resolve() != mailbox:
        raise ValueError('Mailbox path traverses a link')
    launcher = json.loads((Path(root) / 'config/launcher.json').read_bytes())
    uid, gid = int(launcher['uid']), int(launcher['gid'])
    for directory in (mailbox, mailbox / 'credentials', mailbox / 'spool', mailbox / 'attachments'):
        if directory.exists() or directory.is_symlink():
            info = directory.lstat()
            if not stat.S_ISDIR(info.st_mode) or directory.is_symlink() or info.st_uid != uid or info.st_mode & 0o077:
                raise ValueError('Existing mailbox directory is not private to the installed service owner')
        else:
            directory.mkdir(mode=0o700)
            os.chown(directory, uid, gid)
            directory.chmod(0o700)
    # Account/database creation belongs to the OAuth enrollment helper. No
    # account, credential, policy, correspondence or schedule is fabricated.
    return str(mailbox)
