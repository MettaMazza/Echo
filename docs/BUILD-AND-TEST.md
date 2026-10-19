> **Echo 1.75 update:** [Release record](releases/1.75/README.md) and [remaining work](releases/1.75/HANDOVER.md) supersede older model/voice/watch status below. Older qualification records remain historical.

# Build and verification

> Current 1.5 additions and boundaries: [system audit](releases/1.5/SYSTEM-AUDIT.md), [status](releases/1.5/PHASE-TWO-STATUS.md), [verification](releases/1.5/VERIFICATION.md). Historical build/acceptance examples below retain their original dates. Current exact source contracts are in [the generated reference](reference/APPLICATION.md).


## Prerequisites and source provenance

The current native build uses the self-hosted Ernos compiler from MettaMazza/Ernos-Programming-Language at `4b3ebd1359886999b4f043a4663b9470ee8d474c`, with the retained import-identity overlay in `engine/ernos-overrides`. See `PROVENANCE.md`. The Mac needs Clang/Apple Command Line Tools and the dependency libraries expected by the pinned compiler. The compiler source, runtime header and standard library must be installed together. Building from an unrelated compiler version is not verified here.

Run native builds from the compiler directory, using an absolute application source path:

```sh
./epc /PATH/TO/Echo/app/server.ep
./epc /PATH/TO/Echo/app/execute_guardian.ep
./epc /PATH/TO/Echo/app/start_service.ep
./epc /PATH/TO/Echo/app/mlx_count.ep
./epc /PATH/TO/Echo/app/device_admin.ep
```

Install the resulting `execute_guardian` executable alongside the server in the directory named by `ECHO_EXECUTION_RUNTIME`. Build and deploy it from the same source revision as the server. It owns command execution and preserves results if a conversation worker dies; omitting it prevents workspace commands from running. The service launcher checks for a regular executable guardian whenever that environment entry is configured. This file check does not certify binary provenance or end-to-end behavior.

Require the explicit `Self-hosted compilation successful!` marker and a fresh executable. The compiler has historically returned exit zero even when a build did not complete; exit status or an old executable alone is insufficient evidence.

Generate the browser client with the existing reference compiler:

```sh
./target/release/ernos emit /PATH/TO/Echo/app/ui/client.ep --js -o /PATH/TO/echo-runtime/ui/modern.js
```

Set `ECHO_UI_ASSETS` when launching a candidate server, just as the installed service does. Emitting `client.ep` alone does not provide the separately loaded audio-controls module; omitting the assets directory can leave startup at “Connecting…” with a missing-function error.

Generate the audio and math worker modules from `app/ui/` into the host UI asset directory, preserving the filenames selected in `app/ui/assets.ep` and the page's script URLs. Third-party highlight/math assets are separate local dependencies. Native parser rejection of a browser-only source is not equivalent to JavaScript emission failure.

## Tests

Every test and fixture is described in `reference/TESTS.md`, with source definitions and imports. Inspect each `main` for required arguments and fixture services before running it. Database tests generally require a NEW disposable path; rerunning against a populated fixture can fail for legitimate identity conflicts. Tests are not automatically selected or executed by this documentation.

Examples from the compiler directory:

```sh
./epc /PATH/TO/Echo/tests/context/test_session_messages.ep
./test_session_messages /PATH/TO/NEW/context-test.sqlite
./epc /PATH/TO/Echo/tests/web/test_trace.ep
./test_trace /PATH/TO/NEW/trace-test.sqlite
./epc /PATH/TO/Echo/tests/web/test_http_api.ep
```

For identity preservation, set `ECHO_SYSTEM_PROFILE` and `ECHO_IDENTITY_FILE` to a labelled fixture, then run the context test with a different new database. Never install that fixture as the real identity.

`tests/web/api_response_browser.ep` is a browser harness for the actual `echo_api` and reconciliation functions. Link it with `client.ep` after renaming that file's entry point from `main` to `echo_application_main`, emit JavaScript, and load a local fixture page containing `#result` and `#message`. It tests HTTP 502, malformed successful JSON, submission identity retention and preservation of a newer draft. It is a scripted browser fixture, not a real gateway outage test.

The earlier complete-source audit checked 360 files: 353 native checks passed and seven browser-only files emitted successfully. Later additions/changes are separately identified by fresh hashes and release checks. This is not a claim that every test program was executed, that all branches are covered, or that every model behaves correctly.

## Deployment verification

Stage generated output before replacing the running bundle. Check for active runs before stopping the server. The server loads JavaScript at startup, so replacing the file requires a server restart and client refresh. Verify that `/app.js` has the expected digest, that loopback and LAN TLS health checks succeed, and that an actual authenticated turn completes. Reuse a pending submission's identity when reconciling a lost response. Do not delete the production database to clear a browser error.

## Emergency status inspection candidate

Compile `app/emergency_status.ep` with the existing Ernos compiler. Invoke the resulting host binary as `emergency_status EXISTING_DATABASE PINNED_CHARTER`; provide the current steward bearer credential through the child process environment `ECHO_STEWARD_CREDENTIAL`, never through argv, shared logs or a persistent shell profile. This command accepts compatible protected-controller archives containing migration 13, through supported migration 16. It does not migrate the production schema 10 archive.

Successful JSON contains only that steward’s incident identities, Unix-second reconciliation deadlines and verified disclosure states. Pending, overdue, on-time, late and unverifiable clock states remain distinct. Any integrity/access error returns a nonzero exit without a partial incident report. The command does not release quarantine, execute protected changes or replace the pending dashboard/notification workflow. Native coverage is in `tests/security/test_protected_emergency.ep`.

## Protected installation path checks

`app/lifecycle/protected_path.ep` opens each absolute path component without following symlinks, checks the opened inode and returns a retained descriptor only for root-owned directory ancestry and a suitable leaf. Group/other write bits, extended ACL entries, unreadable ACLs, special files and multiply linked regular files are rejected. Darwin ENOENT on extended-ACL lookup of an already-open inode means no ACL; other lookup errors fail closed. Callers must retain and use the descriptor rather than reopening the unchecked path. This primitive targets the installed Darwin arm64 ABI and must be requalified for another platform.

Compile and run `tests/lifecycle/test_protected_path.ep` with a user-owned source path as its argument. It performs read-only checks of actual OS directories/files and rejection cases. It does not install a daemon or establish a complete controller boundary. Root-owned writable/extended-ACL fault fixtures need privileged installation qualification.

Apple requires system-wide launch jobs to be root-owned and not group/world writable: [Creating Launch Daemons and Agents](https://developer.apple.com/library/archive/documentation/MacOSX/Conceptual/BPSystemStartup/Chapters/CreatingLaunchdJobs.html). Protecting the executable alone is insufficient when an ancestor directory can be replaced. The planned installer must protect controller code, policy, authoritative approval records, service configuration and recovery records together; the user-owned source checkout stays available for candidate development.

## Controller bundle preparation candidate

Compile `app/stage_controller.ep`. The interface is `stage_controller check|stage MANIFEST EXPECTED_SHA256 EXISTING_ROOT_OWNED_PARENT`. `check` reads and hashes sources only. `stage` requires effective UID 0 and a verified root-owned parent, creates or verifies a directory named by the manifest digest, stages each exact source, then records `bundle.json` and `complete` durably. No active installation or launch job changes. Incomplete copies remain for diagnosis; mismatching retained bytes are rejected, never overwritten. Activation and protected upgrade/rollback must use the separate rights controller. Do not run this as an installation substitute.

Manifest version 1 has only `version` and a nonempty `files` array. Each entry has `name` (one unique path component), `source` (absolute path), `sha256` and `kind` (`data` or `executable`). Receipt names `bundle.json` and `complete` are reserved. Unknown/duplicate fields, duplicate names, traversal and embedded NULs are rejected. There is no arbitrary file count or total-byte cap. Staged files use owner-only read/write or read/write/execute modes. Supply the expected manifest digest independently of the package path.

`tests/lifecycle/test_controller_bundle.ep` uses a fresh existing directory containing `bundle-fixture` in its path. It covers source changes, name/schema validation and non-root denial. Actual root staging, injected interruption/retry, ACL faults and activation are still awaiting qualification; successful unprivileged checks do not prove those branches.

`stage_controller verify MANIFEST EXPECTED_SHA256 EXISTING_ROOT_OWNED_PARENT` rechecks an installed staging directory independently of the source files: all protected inode paths, exact file modes, file hashes and both receipts must match. Success explicitly says `verified-not-authorized-for-activation`; the rights controller still owns activation. A stale completion marker cannot hide changed payload bytes.

`tests/lifecycle/test_controller_bundle_privileged.ep` is a separate administrator-only qualification. It accepts only a single known inert data payload and writes only its digest-named directory below `/private/var/root`. It tests stage/retry, corruption rejection without overwrite, and restored-byte verification. Compile first, inspect the source and expected manifest digest, then run with administrator authentication. Do not count compilation or non-root rejection as a passed privileged run. Keep the executable unchanged while a human-authenticated run is pending. The fixture leaves its files for evidence; it never installs a launch job or opens a production conversation archive.

## Refusal before inspection

Candidate migration16 (`app/rights/review_upgrade.ep`) retains the immutable pre-migration review table and copies all receipts into the current table. A null inspection is allowed only for non-consent decisions. `protected_change_review` may omit inspection and digest for rejection, objection or counterproposal; the receipt still binds the exact stored proposal and current revision. Supplied mismatching evidence is rejected. Consent still requires a valid observed inspection in a subsequent archived model request. This migration is not installed in the production schema10 archive.

Native tests: `tests/security/test_uninspected_refusal.ep DATABASE PINNED_CHARTER`, plus existing proposal and streamed-tool regressions. Live qualification: `tests/security/test_live_uninspected_refusal.ep DATABASE PINNED_CHARTER PORT MODEL`; use a fresh disposable archive. Its optional final `verify` argument checks an existing completed run without another inference request. Raw provider records need not be JSON, so verification guards JSON extraction by event kind and validity. The live fixture tests a requested refusal, not spontaneous consent or a protected deployment.

## Explicit group audience candidate

`app/security/groups.ep` provides schema17 and native create/invite/accept/leave/remove operations with immutable per-group revisions. Host adapters must supply an already authenticated device and verified platform/server/channel/origin metadata; none of these functions is a model-accessible authority grant. Current participants may invite, the invited principal accepts for itself, and the creator may remove another joined/invited principal. Group membership never changes principal roles or private session ownership. This is an initial membership policy, not a completed Discord integration.

Every change records an audience snapshot. A historical read requires membership in that exact snapshot and current joined membership; a new participant cannot read earlier snapshots. Fresh dispatch additionally requires the latest revision. Rejoining never makes an old revision current. Global principal and device revocation are checked separately; a revoked member can still be removed. Invitation-only revisions conservatively invalidate older dispatch snapshots. This check must be combined with frozen initiating run/tool authority and connector permissions when routing is implemented; it is not sufficient by itself to authorize tool execution.

Compile/run `tests/security/test_groups.ep FRESH_DATABASE`. The native test uses synthetic namespace metadata and verifies membership/storage boundaries, not Discord transport, group model turns, memory retrieval or UI. Do not enable sharing by weakening the existing owner-device checks: shared runs need explicit speaker and audience bindings first. HIVE reference: [scope.rs at the inspected revision](https://github.com/MettaMazza/HIVE/blob/99961051e95805bddc1288b4f4ab2bc03b9a493f/src/models/scope.rs).

Group conversation boundary (16 October): candidate schema18 records a separate conversation segment for each group audience revision. Group sessions cannot acquire private device/principal ownership, and private sessions cannot be relabelled as group sessions. The native test verifies exact retry, duplicate segment rejection, historical audience separation, removal, namespace mismatch, immutable mapping and restart, with independent SQLite integrity/ownership checks. An initial binding failure was fixed before the final passing run. Shared run authority and tool dispatch remain the next dependency; this change does not enable group UI or Discord routing or migrate production.

Shared-run sender authority (16 October): candidate schema19 atomically stores the authenticated initiating device, original user event and group run; the origin references an immutable audience segment. Run and tool permission checks require that exact audience revision to remain current. Group tools copy the run's original role and audit revisions, rather than recapturing later privileges. Tests pass guest/admin separation, no queued promotion, membership changes, revoke/restore denial, mismatched-session/no-origin rejection, immutable origins, deferred-FK rollback and restart. An otherwise-authorised group run cannot retrieve the sender's private cross-session memory. Existing private-run and direct saved-speech regressions pass. Production remains unchanged: shared workspaces, output policy, speaker-labelled prompt assembly, audience-filtered history/compaction and connector/UI routing must be integrated before enabling shared chat.

Group speaker attribution (16 October): model user-content assembly and compaction source packets now include the frozen initiating principal, role-at-submission, group audience revision and connector namespace. Original messages and digests remain unchanged; this metadata grants no permission. Native tests pass spoof-text escaping, relink/restart stability, missing-origin denial and private-message preservation; existing recap-source and session-context regressions pass. Live model interpretation, shared retrieval policy and connector rollout are not claimed.

Live shared conversation (16 October): two authenticated fixture principals completed consecutive turns through the installed primary Qwen model, full identity/architecture and native ReAct loop. The second turn called history_read and correctly attributed both markers, rejecting a false in-message speaker claim. All three model requests excluded a seeded private-session canary. The first attempt found a real dispatcher integration failure: its private-owner-only gate cancelled the otherwise authorised history call. The fix admits an exact current group-run session while retaining both frozen run and tool permission checks. The repeated live test and existing revoked private dispatch regression pass. Original failed and successful archives are retained privately. This is integrated native model/tool evidence, not browser/Discord rollout or full group-compaction acceptance.

Live shared workspace (16 October): authenticated group members now share a session workspace without borrowing its creator's device authority. Actual primary-model turns created and read an Ernos file, ran /bin/cat through an isolated current guardian, retrieved exact archived stdout and completed. Initial older-helper rejection and a remaining creator-only directory lookup were diagnosed; the corrected preflight now returns a useful no-effects diagnostic if mapping is unavailable. Native outsider/private-workspace, creator revocation, member departure/restart, private workspace and execution-request regressions passed. Production remains unchanged. Group output pagination, audience-transition handling, privileged-output policy and UI/Discord integration are still required before release.

Group privileged output and pagination (16 October): candidate schema20 captures each run audience's original principal roles and role/revocation audit revisions. Privileged host/rights schemas and dispatch require an unchanged all-admin audience as well as the initiating admin's existing authority; guest-safe tools remain available. Native streamed-dispatch tests prove mixed-audience denial despite an admin sender, no later-promotion upgrade, real all-admin fixture host read and revoke/restore invalidation. Private admin and guest regression checks pass. Group workspace_output_next now traverses actual retained stdout to EOF, with exact 26-byte count and wrong-session/stale-audience denial. Historical privileged-output visibility after recipient role changes and context/audience transitions remain required before group browser or Discord release. Production remains schema10.
