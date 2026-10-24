> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Admin host execution

`host_execute` is the Step 05 admin worker. It accepts `program` and a list of `arguments`, taken from the immutable queued tool request. It is advertised only to a currently authorised admin in an eligible audience and only when the host has configured its protected and secret roots. Guest `workspace_execute` retains its original restricted profile.

The existing execution guardian owns the child process, records launch and exit evidence, captures complete stdout/stderr, and monitors the initiating authority and cancellation. Revocation, Stop, or owner loss terminates and reaps the owned process. An uncertain execution is reconciled from its saved records, never silently repeated. Commands receive an explicit environment and begin in the conversation workspace. They execute as the Mac service account, not automatically as root.

The host supplies JSON path arrays in `ECHO_HOST_PROTECTED_ROOTS` and `ECHO_HOST_SECRET_ROOTS`. Both must be nonempty and valid before this tool is advertised or launched. The admin sandbox permits ordinary host file operations, processes and outbound network access, while denying writes to protected installation/state/model roots and reads/writes in connector-secret roots. The profile is passed directly to sandbox-exec, not loaded from a model-writable file. The ordinary host text reader also excludes configured connector-secret paths. Protected installation changes remain the charter controller’s responsibility in Step 07.

Tests use disposable files and principals: an admin executes a real command; guests, revoked admins and authority promoted after submission cannot execute it; Stop and revocation after launch reap the process; the native child cannot change protected files or create a hardlink escape, and cannot inspect the secret fixture. These checks extend the existing execution tests and do not add plan steps.

Implementation: `app/workspace/host_profile.ep`, `execution_request.ep`, `sandbox_process.ep`, `execute_process.ep`, `app/tools/execution_guardian.ep`, and the shared tool policy/schema/dispatch functions. No database migration. Operations retain the sandbox digest in the original process reservation.

The deployed host-worker dependencies were reconciled into a logical source commit on 16 October before adding Code/Git tools. Fresh admin, guest, revoked and late-promotion fixtures passed against the current deployed guardian; evidence is in `evidence/phase2/host-dependency-20261016/`. Earlier protected-file, hardlink and cancellation evidence remains in `evidence/phase2/host-worker-20261016/`. This commit records existing deployed behavior and requires no service restart.
