#!/usr/bin/env python3
"""Human-run Google Desktop OAuth enrollment. Runtime mailbox code is Ernos.
Writes a private grant for native installation; never sends mail or edits services.
Reference: https://developers.google.com/identity/protocols/oauth2/native-app
"""
import argparse, base64, ctypes, hashlib, hmac, http.server, json, os
from pathlib import Path
import secrets, sqlite3, ssl, stat, time, urllib.error, urllib.parse, urllib.request, webbrowser

AUTH = 'https://accounts.google.com/o/oauth2/v2/auth'
TOKEN = 'https://oauth2.googleapis.com/token'
PROFILE = 'https://gmail.googleapis.com/gmail/v1/users/me/profile'
MAILBOX = 'echo.ernos.smith@gmail.com'
SCOPES = ['https://www.googleapis.com/auth/gmail.modify', 'https://www.googleapis.com/auth/gmail.settings.basic']


def callback_result(target, host, expected_host, state):
    p = urllib.parse.urlsplit(target)
    if p.scheme or p.netloc or p.path != '/callback' or host != expected_host:
        return None
    q = urllib.parse.parse_qs(p.query, keep_blank_values=True)
    def one(key):
        values = q.get(key, [])
        return values[0] if len(values) == 1 and values[0] else None
    returned = one('state')
    if returned is None or not hmac.compare_digest(returned, state):
        return None
    if one('error'):
        return ('denied', None)
    code = one('code')
    return ('authorized', code) if code else None


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, *args, **kwargs):
        raise RuntimeError('Google API redirect refused')


def google_json(url, form=None, token=None):
    if url not in (TOKEN, PROFILE):
        raise ValueError('Unapproved endpoint')
    headers = {'Accept': 'application/json'}
    data = None
    if form is not None:
        data = urllib.parse.urlencode(form).encode('ascii')
        headers['Content-Type'] = 'application/x-www-form-urlencoded'
    if token:
        if not isinstance(token, str) or any(ord(c) < 33 or ord(c) > 126 for c in token):
            raise ValueError('Invalid access-token format')
        headers['Authorization'] = 'Bearer ' + token
    opener = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect(),
        urllib.request.HTTPSHandler(context=ssl.create_default_context()))
    try:
        with opener.open(urllib.request.Request(url, data=data, headers=headers), timeout=60) as response:
            return json.load(response)
    except (urllib.error.URLError, json.JSONDecodeError, TimeoutError):
        raise RuntimeError('Google authorization/profile request failed; no grant installed') from None


def enrolled_admin(archive, principal):
    with sqlite3.connect(Path(archive).absolute().as_uri() + '?mode=ro', uri=True) as db:
        row = db.execute("SELECT 1 FROM echo_principals WHERE identity=? AND role IN ('admin','steward') AND revoked=0", (principal,)).fetchone()
    if row is None:
        raise ValueError('Mailbox owner must be an enrolled, active Echo admin')


def no_acl(fd):
    lib = ctypes.CDLL('/usr/lib/libSystem.B.dylib', use_errno=True)
    lib.acl_get_fd_np.argtypes = (ctypes.c_int, ctypes.c_int)
    lib.acl_get_fd_np.restype = ctypes.c_void_p
    lib.acl_get_entry.argtypes = (ctypes.c_void_p, ctypes.c_int, ctypes.POINTER(ctypes.c_void_p))
    lib.acl_free.argtypes = (ctypes.c_void_p,)
    acl = lib.acl_get_fd_np(fd, 256)
    if not acl:
        return ctypes.get_errno() == 2
    try:
        return lib.acl_get_entry(acl, 0, ctypes.byref(ctypes.c_void_p())) == -1
    finally:
        lib.acl_free(acl)


def private_directory(path):
    path = Path(path).absolute()
    fd = os.open('/', os.O_RDONLY | os.O_DIRECTORY)
    try:
        for component in path.parts[1:]:
            if component in ('.', '..'):
                raise ValueError('Ambiguous private directory')
            child = os.open(component, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=fd)
            os.close(fd)
            fd = child
        info = os.fstat(fd)
        if info.st_uid != os.geteuid() or stat.S_IMODE(info.st_mode) != 0o700 or not no_acl(fd):
            raise ValueError('Grant directory requires your ownership, mode 0700 and no ACL grants')
        return fd
    except BaseException:
        os.close(fd)
        raise


def write_grant(directory, name, record):
    if Path(name).name != name or name in ('', '.', '..'):
        raise ValueError('Invalid grant filename')
    fd = os.open(name, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600, dir_fd=directory)
    try:
        if os.fstat(fd).st_nlink != 1 or not no_acl(fd):
            raise ValueError('Grant file requires private permissions and a single link')
        raw = (json.dumps(record, separators=(',', ':')) + '\n').encode()
        with os.fdopen(fd, 'wb', closefd=False) as stream:
            stream.write(raw)
            stream.flush()
            os.fsync(fd)
        os.fsync(directory)
    finally:
        os.close(fd)


def authorize(client, scopes):
    state, verifier = secrets.token_urlsafe(32), secrets.token_urlsafe(64)
    challenge = base64.urlsafe_b64encode(hashlib.sha256(verifier.encode()).digest()).rstrip(b'=').decode()
    result = []
    class Callback(http.server.BaseHTTPRequestHandler):
        def log_message(self, *args):
            pass  # Callback URLs contain authorization codes.
        def do_GET(self):
            value = callback_result(self.path, self.headers.get('Host'), expected_host, state)
            if value is None:
                self.send_error(400, 'Invalid authorization callback')
                return
            if result:
                self.send_error(409, 'Authorization already received')
                return
            result.append(value)
            self.send_response(200)
            for key, value in [('Content-Type','text/plain; charset=utf-8'),('Cache-Control','no-store'),('Referrer-Policy','no-referrer')]:
                self.send_header(key, value)
            self.end_headers()
            self.wfile.write(b'Authorization received. Return to Echo setup. No email sent.')
    class LoopbackServer(http.server.HTTPServer):
        def get_request(self):
            connection, address = super().get_request()
            connection.settimeout(5)
            return connection, address
    with LoopbackServer(('127.0.0.1', 0), Callback) as server:
        server.timeout = 1
        expected_host = f'127.0.0.1:{server.server_port}'
        redirect = f'http://{expected_host}/callback'
        url = AUTH + '?' + urllib.parse.urlencode({'client_id':client, 'redirect_uri':redirect,
            'response_type':'code', 'scope':' '.join(scopes), 'state':state,
            'code_challenge':challenge, 'code_challenge_method':'S256', 'access_type':'offline',
            'prompt':'consent', 'login_hint':MAILBOX})
        if not webbrowser.open(url):
            raise RuntimeError('System browser did not open')
        print('Complete Google authorization in your browser. Waiting up to five minutes.')
        end = time.monotonic() + 300
        while not result and time.monotonic() < end:
            server.handle_request()
    if not result or result[0][0] != 'authorized':
        raise RuntimeError('Authorization denied or timed out; no grant installed')
    return {'client_id':client, 'code':result[0][1], 'code_verifier':verifier,
            'redirect_uri':redirect, 'grant_type':'authorization_code'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--client-json', required=True, type=Path)
    p.add_argument('--archive', required=True, type=Path)
    p.add_argument('--principal', required=True)
    p.add_argument('--output', required=True, type=Path, help='New file under an existing private 0700 directory')
    p.add_argument('--permanent-delete', action='store_true', help='Explicit full-mail scope request')
    p.add_argument('--contacts', action='store_true', help='Separately authorize read-only Contacts')
    args = p.parse_args()
    enrolled_admin(args.archive, args.principal)
    directory = private_directory(args.output.parent)
    try:
        installed = json.loads(args.client_json.read_text()).get('installed', {})
        client = installed.get('client_id')
        if not isinstance(client, str) or not client.endswith('.apps.googleusercontent.com'):
            raise ValueError('Google Desktop app OAuth client required')
        scopes = list(SCOPES)
        if args.permanent_delete:
            scopes.append('https://mail.google.com/')
        if args.contacts:
            scopes.append('https://www.googleapis.com/auth/contacts.readonly')
        form = authorize(client, scopes)
        if installed.get('client_secret'):
            form['client_secret'] = installed['client_secret']
        token = google_json(TOKEN, form=form)
        if not token.get('access_token') or not token.get('refresh_token') or token.get('token_type', '').lower() != 'bearer':
            raise RuntimeError('Google did not provide a renewable offline grant')
        granted = token.get('scope', '').split()
        if not set(SCOPES).issubset(granted):
            raise RuntimeError('Required mailbox scopes were not granted')
        profile = google_json(PROFILE, token=token['access_token'])
        if profile.get('emailAddress', '').lower() != MAILBOX:
            raise RuntimeError('Authorized mailbox does not match Echo; no grant installed')
        enrolled_admin(args.archive, args.principal)
        expires = token.get('expires_in')
        if type(expires) is not int or expires <= 0:
            raise RuntimeError('Invalid Google token expiry')
        credential = {'client_id':client, 'client_secret':installed.get('client_secret', ''),
            'access_token':token['access_token'], 'refresh_token':token['refresh_token'],
            'scope':' '.join(granted), 'expires_at':int(time.time()) + expires}
        write_grant(directory, args.output.name, {'format':1, 'principal':args.principal,
            'email':MAILBOX, 'credential':credential})
        print('Private grant saved for native installation. No email sent; services unchanged.')
    finally:
        os.close(directory)


if __name__ == '__main__':
    try:
        main()
    except (ValueError, RuntimeError, OSError, sqlite3.Error) as error:
        print(str(error) if isinstance(error, (ValueError, RuntimeError)) else 'Local enrollment failed; no successful installation claimed.')
        raise SystemExit(1)
