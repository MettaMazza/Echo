# Operations

## Storage and service configuration

The canonical project and large stored assets live on One Touch. Active compiler/runtime files, the SQLite archive, helper spool and session workspaces run from the Mac's internal filesystem. The examples in `config/examples/` contain placeholders and must be populated with the installation's actual paths. Live profiles are deliberately excluded from Git.

`app/start_service.ep` reads a profile with executable, directory, argument array and explicit environment. It uses direct execution without shell interpretation. Run `start_service PROFILE check` to validate the executable/directory, then omit `check` to start in the foreground. The check does not establish provider health, port availability, model-file completeness or full acceptance. Detached launch is used on the current host; a reboot supervisor is not established.

The Echo server command takes: port, compiled browser JavaScript, port-record path, database path, model, provider port, workload reserve, I/O batch and optional public HTTPS origin. The workload reserve participates in admission; it is not a fixed output length. I/O batch size is transfer granularity, not total history capacity. Inspect `app/server.ep` for validation. Provider registry selection in the database remains authoritative for subsequent runs.

## Local network access

On the Mac use its loopback URL; on a phone use the Mac's LAN address and configured gateway port. `127.0.0.1` on a phone means the phone, not the Mac. Caddy terminates local HTTPS and proxies to the loopback-only Echo listener. Model servers remain loopback-only. This installation does not configure router forwarding or public internet access.

Pair each browser with a one-use invitation from the host `device_admin` command. The returned credential belongs to that browser profile. A shared browser profile shares access. Use `device_admin DATABASE list` and `revoke DEVICE` when required. Keep invitations, credentials and local certificate keys out of logs and Git. A local certificate may require browser trust; microphone access additionally depends on secure-context and OS/browser permissions.

## Lost responses and pending submissions

The client records its submission before sending it. The server accepts an idempotent replay of exactly the same request. The browser now reconciles pending work before accepting different text and preserves the newer draft. If the old run is still active, wait for completion or cancel through the normal UI. HTTP 502/503/504 now identify service unavailability; malformed JSON is reported as an incomplete response. Neither condition proves the server rejected the original submission.

If the service is down, inspect the private service log and process/listener state, then restart using the existing profile. Startup recovery claims abandoned lock-aware work. Verify a single original submission and terminal run state afterward. Do not manually reset ledger states, remove worker locks or replay uncertain external effects to force progress.

## Backup and restoration

Root commands include `backup`, `backup_files`, `backup_tree`, `backup_bundle`, `restore_export`, `restore_files`, `restore_tree`, `restore_bundle`, `verify_export` and `release_restored_archive`. Their required arguments are indexed in the source reference; use disposable destinations for qualification. Bundle restoration verifies manifests and digests, and retains an execution hold until historical actions are reconciled. A backup of the database alone is not a backup of every workspace, model, attachment or environment.

The prior bundle snapshot and independent restore have verified evidence. They are not a continuously updated backup of the current installation. External ExFAT and internal APFS have different publication guarantees: active workspace creation requires the tested atomic exclusive behavior; restore publication has its own reservation protocol. Cooperating-publisher exclusivity does not protect against an unrelated process modifying the same destination.

## Upgrade discipline

Keep originals and backup snapshots. Build into the internal runtime, confirm compiler success, test changed behavior, wait for active runs to finish and deploy one coherent binary/bundle/configuration set. Record source and executable digests. Preserve private identity files byte for byte and keep technical architecture updates separate. A source commit, a successful build, a running process and a passing live test are separate facts.

## Crash records and recovery

Start the native server with its normal arguments followed by `--supervise` for a separate Ernos service owner. Abnormal listener exits produce a durable exit record and restart after a one-second scheduling delay; normal exit and SIGTERM stop supervision. Request child exits are reaped and recorded. Model workers have a separate owner which records exit status and returns interrupted generation to a saved, retryable state when the existing recovery protocol proves that safe. Uncertain tool effects require reconciliation rather than automatic redispatch.

`ECHO_DIAGNOSTIC_LOG` selects a private JSON-lines incident file; the default is `../data/echo-incidents.jsonl` relative to the compiler working directory. Records contain timestamp, role/event, PID, operation identity and raw wait status, without prompt bodies or credentials. Each record is appended and synced. Failed writes also go to stderr. Preserve that file, stderr and macOS DiagnosticReports when investigating a fault. Native fatal-signal handlers use macOS reporting. Raw wait status 9 means SIGKILL; 11 means SIGSEGV; a normal exit code is stored shifted by eight bits.

If the listener is unavailable, the service owner restarts it and startup reconciles saved claims. If a chat shows interrupted-retry-available, Retry continues its saved run. If it shows reconciliation-required, inspect the run's archived tool dispatch/results and the matching incident identity before any retry. Never clear the database or create a replacement submission to hide an interrupted one.

No software can promise a durable record when the machine loses power before writing or its storage fails. An unpaired start record is evidence of an interrupted operation, not proof of success. Reboot startup is not currently qualified: an attempted LaunchAgent start stalled before the listener became available and was disabled; normal host-context startup works. The local Ernos service owner supplies runtime restart independently of that unqualified LaunchAgent.

Offline bundle backups permit saved failed runs only when all tool calls are observed and process launches have finished. They preserve failure state and diagnostics; they do not retry or clear failures. Ready/running/waiting work still requires reconciliation, and every restored bundle remains held before execution. The Phase Two baseline bundle was independently restored with current registered attachments and configured source/state files; model weights and full dependency runtimes remain outside that explicitly listed snapshot.

The host `principal_admin` command supports `DATABASE revoke-principal PRINCIPAL UNIQUE_CHANGE_ID REASON` and `DATABASE restore-principal PRINCIPAL UNIQUE_CHANGE_ID REASON` after explicit migration. These are local operator controls, not model tools or HTTP endpoints. A restore assigns ordinary-user role; privileged authority requires a separate recorded grant. Independently revoked device/account credentials stay revoked. Existing production binaries do not yet enforce the Phase Two principal layer; deploy the complete capability integration before relying on these controls for live requests.

Host account association uses `principal_admin DATABASE link-account PLATFORM SUBJECT PRINCIPAL UNIQUE_CHANGE_ID REASON` or `unlink-account` with the same arguments, after explicit migration. Confirm the platform subject through the authenticated provider before linking. Unlink the current association before moving it to another principal. These commands do not import or merge conversations, and account authentication remains the connector’s responsibility.
