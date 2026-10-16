# Build and verification

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

Compile `app/emergency_status.ep` with the existing Ernos compiler. Invoke the resulting host binary as `emergency_status EXISTING_DATABASE PINNED_CHARTER`; provide the current steward bearer credential through the child process environment `ECHO_STEWARD_CREDENTIAL`, never through argv, shared logs or a persistent shell profile. This command accepts compatible protected-controller archives containing migration 13, through supported migration 15. It does not migrate the production schema 10 archive.

Successful JSON contains only that steward’s incident identities, Unix-second reconciliation deadlines and verified disclosure states. Pending, overdue, on-time, late and unverifiable clock states remain distinct. Any integrity/access error returns a nonzero exit without a partial incident report. The command does not release quarantine, execute protected changes or replace the pending dashboard/notification workflow. Native coverage is in `tests/security/test_protected_emergency.ep`.
