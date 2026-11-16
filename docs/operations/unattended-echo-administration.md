# Unattended Echo administration

Installed 29 October 2026 for local owner `dolltrading`.

Normal rebuild/deploy operations use `tools/setup/install-prepared-release.py` from the prepared package. It calls the root-owned `/Library/PrivilegedHelperTools/local.echo.admin` through `sudo -n`; it never falls back to a password dialog. Rebuild compilation runs as the normal user.

```sh
sudo -n /Library/PrivilegedHelperTools/local.echo.admin status
sudo -n /Library/PrivilegedHelperTools/local.echo.admin check-package /absolute/package/directory
sudo -n /Library/PrivilegedHelperTools/local.echo.admin restart agent
```

Restart accepts only Echo service names: web, agent, activity, queue_controls, gateway, actions, interaction_ack, rsi-controller, tool-registry, self-monitor, primary, embedding, omni, speech and proxy. A restart is immediate; avoid interrupting active requests. Model and speech services have separate profiles; target only the services required by the change.

Deploy through the coordinator, which checks idle state and acknowledges the protected installer's checkpoints. The helper snapshots hash-pinned inputs into root-owned storage and uses enrolled installer code. Package-provided scripts are not executed as installers. Protected acceptance, integrity checks, rollback and intake maintenance remain in place. Ordinary application updates preserve the installed web asset path unless they include client.js. Transient Discord delivery receives up to 45 seconds to drain.

The narrowly scoped sudo rule lives in `/private/etc/sudoers.d/echo-admin`. It grants the enrolled local owner this helper only; it does not grant a general root shell or change Discord-user permissions. Protected configuration lives in `/Library/Application Support/EchoRSI/administration`.

For a fresh machine, prepare the enrollment with `tools/setup/prepare-echo-admin.py DESTINATION --runtime RUNTIME`, then run its bootstrap.py once as administrator. This is setup, not part of each rebuild. An operating-system permission reset or removal of the helper would require enrollment again.

Verification: noninteractive status and pinned-package validation succeeded with cached sudo credentials explicitly ignored. Unknown services and shell operations were rejected; general sudo still required authentication. Echo health remained ready. The maintained setup reproduced the installed payload exactly.
