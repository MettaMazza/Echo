# Tests source reference — Echo 1.75

Observed 2026-10-19T21:07:14.397795+00:00. Every listed file was read in full for this lexical inventory. A test file is not a test pass. Line links identify this source snapshot; hashes are authoritative.

[Release overview](../releases/1.75/README.md) · [Machine-readable manifest](../forensics/source-manifest.json)

## `tests/agent/recover_guardian_fixture.ep`

[tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep) · 32 lines · 1934 bytes

SHA-256: `83d51516b21a6ca69b4f45df846657e72e978f2624dd22b0d972a061cd95b108`

Source annotations: Run only after the test driver has observed the original worker's SIGKILL.

Imports: [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/agent/test_attachment_context_visibility.ep`

[tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep) · 87 lines · 6127 bytes

SHA-256: `8d5f7f861026a16bcebddccd71fe307ac59a3cb78c4aa5818d8db1ae6d587658`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `tests/agent/test_attachment_loop.ep`

[tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep) · 86 lines · 5808 bytes

SHA-256: `8a65c93bc77c6cb7b2f5bf2563c4dc0e6e508bad8473f189270dadc2d50868ee`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/agent/test_checked_generation.ep`

[tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep) · 64 lines · 4336 bytes

SHA-256: `b1482f631832465ba2e7ba527445326f43a07d8f32eb098840c281cc431c959c`

Imports: [../../app/agent/checked_generation](../../app/agent/checked_generation.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define checked_reply with listener as List and method as Str and path as Str and request as Str and response as Str returning Int:
12: define checked_server with listener as List and body as Str and completed as Int returning Int:
22: define main returning Int:
```

## `tests/agent/test_execution_control.ep`

[tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep) · 105 lines · 7238 bytes

SHA-256: `c2419224a3b9a9565e39da178340786491ffa92847035548e1717af46abe0c41`

Imports: [../../app/rights/quarantine](../../app/rights/quarantine.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define execution_interrupt_fixture with path as Str and mode as Str and charter as Str and writer as Int and completed as Int returning Int:
32: define main returning Int:
```

## `tests/agent/test_execution_loop.ep`

[tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep) · 90 lines · 6132 bytes

SHA-256: `da895ef751f5345541aaddf141b63ea4992db6d5d25040082735368de496eee5`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/agent/test_generation.ep`

[tests/agent/test_generation.ep](../../tests/agent/test_generation.ep) · 66 lines · 4496 bytes

SHA-256: `9ae7e362761dc43f45b467d582058463e5ca4e096f36b7195717ea473dbede25`

Imports: [../../app/agent/generation](../../app/agent/generation.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define generation_case with database as Int and listener as List and run as Str and request as Str and data as Str and expected as Str returning Int:
26: define main returning Int:
```

## `tests/agent/test_generation_recovery.ep`

[tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep) · 121 lines · 10813 bytes

SHA-256: `954acb96fe8f3ae9c0c79747cd84fd4bf35d60a95ef27fe9e1674dc35d89b23b`

Source annotations: Intentionally stop between durable stream collection and publication.

Imports: [../../app/agent/generation_recovery](../../app/agent/generation_recovery.ep), [../../app/web/recovery](../../app/web/recovery.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define recovery_capture_fixture with database as Int and listener as List and run as Str and delta as Str returning Int:
24: define main returning Int:
110: define recovery_missing_fixture with database as Int and run as Str and sequence as Int returning Int:
```

## `tests/agent/test_large_request_envelope.ep`

[tests/agent/test_large_request_envelope.ep](../../tests/agent/test_large_request_envelope.ep) · 22 lines · 1134 bytes

SHA-256: `b8d5e8341e42088af58831d29e0863f264eb61696a3ef3f8ca0817b0facbe058`

Imports: [../../app/agent/generation](../../app/agent/generation.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/agent/test_legacy_protocol_recovery.ep`

[tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep) · 31 lines · 2546 bytes

SHA-256: `f79a501f0bfb7b167a1b23c4ac57f000b93147780d4918fcc48759ebded590ac`

Imports: [../../app/maintenance/protocol_repair](../../app/maintenance/protocol_repair.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/agent/test_pdf_loop.ep`

[tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep) · 97 lines · 6976 bytes

SHA-256: `8a5a2a1d973fda260704689601f639f1806d100a08a855e3dafd8d07e842de00`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/agent/test_process_result_recovery.ep`

[tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep) · 104 lines · 7314 bytes

SHA-256: `3cddaf25b6397a639a72947d2be64d685ad2de5303addd0069c4102d1780cf7c`

Imports: [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/agent/test_provider_profile.ep`

[tests/agent/test_provider_profile.ep](../../tests/agent/test_provider_profile.ep) · 34 lines · 2286 bytes

SHA-256: `2491d4c0da6ec7be2eb75a36b7dd9dfd308362dd83c6d579a9132c374819a197`

Imports: [../../app/agent/provider_profile](../../app/agent/provider_profile.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/agent/test_run_state.ep`

[tests/agent/test_run_state.ep](../../tests/agent/test_run_state.ep) · 100 lines · 6047 bytes

SHA-256: `22fa3442fd9b856bcf61e8caff560238101618d087b9ad18c3685a2620cdf484`

Source annotations: Test identities are fixed ASCII fixtures, not unescaped external content. Forty is a test workload, not a production iteration setting.

Imports: [../../app/agent/run_state](../../app/agent/run_state.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define run_fixture_event with database as Int and identity as Str and kind as Str and parent as Str returning Int:
9: define main returning Int:
```

## `tests/agent/test_runner.ep`

[tests/agent/test_runner.ep](../../tests/agent/test_runner.ep) · 40 lines · 2578 bytes

SHA-256: `08ce4a765e5c6a5e66b0716b9b10ded84e7bb07b699318b087e18542ffc9909b`

Source annotations: A cancellation pending before execution changes durable state without a provider request; a restart does not continue the cancelled run.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/agent/test_tool_ledger.ep`

[tests/agent/test_tool_ledger.ep](../../tests/agent/test_tool_ledger.ep) · 58 lines · 3391 bytes

SHA-256: `3b86c2539df927885af6cebd46e5a4d4db948f0be37529e4d8db5ebab3e2204b`

Imports: [../../app/agent/tool_ledger](../../app/agent/tool_ledger.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/agent/test_two_turn_loop.ep`

[tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep) · 98 lines · 6934 bytes

SHA-256: `9fd3c45d13dac184a259d7a5485ecaaebecbdbc7c4ca0ea467af1a5cfb06c66f`

Imports: [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/tool_authority](../../app/security/tool_authority.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/agent/test_unparsed_tool_response.ep`

[tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep) · 37 lines · 2915 bytes

SHA-256: `7fa9e76a38840e3beb2bb1d1bc8c3245676c4c2ba0be4bfdd789c107bf5b7534`

Imports: [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define publish_fixture with database as Int and request as Str and content as Str returning Int:
14: define main returning Int:
```

## `tests/agent/test_workspace_create_loop.ep`

[tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep) · 77 lines · 5496 bytes

SHA-256: `bd41032345466cb39ccbd322e7d98325e331cb057d2e7397a0ba07b5b1c3d6c2`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/agent/test_workspace_directory_recovery.ep`

[tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep) · 61 lines · 4051 bytes

SHA-256: `9f34f1ac6b659a1c7bdae1587c534f1af6a272a211911572e53758bd22aeb439`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/agent/test_workspace_file_recovery.ep`

[tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep) · 60 lines · 4148 bytes

SHA-256: `096c330be2d5a2ab73c2a2ad3eb07046eabc573895cde0e434f1ac4749fe2c9e`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/agent/test_workspace_list_loop.ep`

[tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep) · 52 lines · 3380 bytes

SHA-256: `c9824c00ec644b459ffcfb22e4df8d5e8c64af224d95fc90b533c1323cf00b79`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/agent/test_workspace_loop.ep`

[tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep) · 110 lines · 7609 bytes

SHA-256: `554f12e53c65016f0e481ff22213af7b66e40c275bd54b6a38de9a90b4c3bb21`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/agent/test_workspace_text_loop.ep`

[tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep) · 53 lines · 4047 bytes

SHA-256: `b592f5d1331dca1f0accc384fca2e735bf48225ca9ce553e7486f71fb7618e43`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/agent/verify_live_provider_loop.ep`

[tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep) · 51 lines · 3435 bytes

SHA-256: `5adceab06fd82f93eb4294b4a8a66182b96a0c5b54dcd60d6ce462c84b2714d5`

Source annotations: Verify the recorded search-then-read qualification task, not model self-report. Expected counts belong to this test workload; the agent has no turn ceiling.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/archive/measure_event_commit.ep`

[tests/archive/measure_event_commit.ep](../../tests/archive/measure_event_commit.ep) · 36 lines · 1794 bytes

SHA-256: `4e55cc0260cb1a09389cfacd88bb4dd6b3c1ff101aed6ac7d9a624738f3687c0`

Source annotations: Diagnostic workload only: measure existing phases without changing durability.

Imports: [../../app/archive/event_store](../../app/archive/event_store.ep), `os` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/archive/probe_text_window.ep`

[tests/archive/probe_text_window.ep](../../tests/archive/probe_text_window.ep) · 10 lines · 560 bytes

SHA-256: `aeab005ddcbaa1a8868833f476a3f89ce9e1dcb943a1a1ba60043513c9e61254`

Imports: [../../app/archive/file_text](../../app/archive/file_text.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_append_readback.ep`

[tests/archive/test_append_readback.ep](../../tests/archive/test_append_readback.ep) · 34 lines · 2053 bytes

SHA-256: `9ea63352e0fad0868a59eadd2749f70ccd0fe7645664426dc0a6be4e68ef1fad`

Source annotations: Fault injection applies only to a new disposable database supplied here.

Imports: [../../app/archive/event_store](../../app/archive/event_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/archive/test_attachment_snapshot.ep`

[tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep) · 33 lines · 2287 bytes

SHA-256: `3fd1ffc6e7ef0c99c4c735f095e0d866612bf441d8d372d35909270ed423a743`

Imports: [../../app/archive/attachment_snapshot](../../app/archive/attachment_snapshot.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_branch_export_file.ep`

[tests/archive/test_branch_export_file.ep](../../tests/archive/test_branch_export_file.ep) · 38 lines · 2300 bytes

SHA-256: `f6f7eb090b7a3ba55bbcfc69d5192f1e3b65da67d94bb00cbafe254d4e6023f1`

Imports: [../../app/archive/export_restore](../../app/archive/export_restore.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_bundle_rejection.ep`

[tests/archive/test_bundle_rejection.ep](../../tests/archive/test_bundle_rejection.ep) · 28 lines · 1503 bytes

SHA-256: `f9bd3d7a617adf82d135d411ea1b279225818c0d7dd2620eaba8344b3cdf6e75`

Imports: [../../app/archive/restore_bundle](../../app/archive/restore_bundle.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_digest_stress.ep`

[tests/archive/test_digest_stress.ep](../../tests/archive/test_digest_stress.ep) · 68 lines · 2882 bytes

SHA-256: `2c7eff0c23d94eb8c999482c62a88a7685d8d06aec234cd532258486d1a410c8`

Imports: [../../app/archive/event_store](../../app/archive/event_store.ep), [../../app/providers/json_access](../../app/providers/json_access.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define digest_parser_worker with rounds as Int and completed as Int returning Int:
15: define digest_stress_worker with row as List and expected as Str and rounds as Int and completed as Int returning Int:
35: define main returning Int:
```

## `tests/archive/test_event_store.ep`

[tests/archive/test_event_store.ep](../../tests/archive/test_event_store.ep) · 58 lines · 3028 bytes

SHA-256: `4360e6ea6658a41f28426831217904d11aa00a1b4858979d4513ef909081e87a`

Source annotations: Each invocation receives a disposable database path. Reopen verifies persistence.

Imports: [../../app/archive/event_store](../../app/archive/event_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_export_restore.ep`

[tests/archive/test_export_restore.ep](../../tests/archive/test_export_restore.ep) · 35 lines · 2057 bytes

SHA-256: `f4b40081d996736aaf3bd84ccc0f695d14962f8a7c63698c8c2d3df9398d98c9`

Imports: [../../app/archive/export_restore](../../app/archive/export_restore.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_failed_run_bundle.ep`

[tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep) · 42 lines · 2880 bytes

SHA-256: `9718a7c12b3030425e83d59fff78c296a217c3ce08a84e63b0b834d39863477b`

Imports: [../../app/archive/restore_bundle](../../app/archive/restore_bundle.ep), [../../app/workspace/process_journal](../../app/workspace/process_journal.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_file_base64.ep`

[tests/archive/test_file_base64.ep](../../tests/archive/test_file_base64.ep) · 49 lines · 2620 bytes

SHA-256: `db87f4819a821631c2409d80a964b5bc7e1f6e49c72c461f4b4bfd074e0d507e`

Imports: [../../app/archive/file_base64](../../app/archive/file_base64.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_file_copy.ep`

[tests/archive/test_file_copy.ep](../../tests/archive/test_file_copy.ep) · 23 lines · 1194 bytes

SHA-256: `1c71192168830f09dfac62be28cb59226468c408b9643c5dd98717c097c7a042`

Imports: [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_file_digest.ep`

[tests/archive/test_file_digest.ep](../../tests/archive/test_file_digest.ep) · 17 lines · 1160 bytes

SHA-256: `5d848ad77a79e19507e7420d51f21860d82c1c4c19896e525668ffd8c26b27e2`

Imports: [../../app/archive/file_digest](../../app/archive/file_digest.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_file_stage.ep`

[tests/archive/test_file_stage.ep](../../tests/archive/test_file_stage.ep) · 29 lines · 1612 bytes

SHA-256: `d4c3c9ec7d0cd817ab211c7169f2154b7a04bd0581cc8108ab92dfaa5231943a`

Source annotations: Packed native bytes: 00 01 ff 00 61 62 63 0a (NUL/high-byte fixture).

Imports: [../../app/archive/file_stage](../../app/archive/file_stage.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_file_text.ep`

[tests/archive/test_file_text.ep](../../tests/archive/test_file_text.ep) · 59 lines · 3878 bytes

SHA-256: `4869bdf779d5f7d8ad8b1516fb4166c3e4e2fdf37efd21793acf07edcd9bdc8d`

Imports: [../../app/archive/file_text](../../app/archive/file_text.ep), `fs` (toolchain/library), [../../app/archive/file_stage](../../app/archive/file_stage.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/archive/test_history.ep`

[tests/archive/test_history.ep](../../tests/archive/test_history.ep) · 73 lines · 3651 bytes

SHA-256: `534c427b55d478f2845e5837cd7f6074e0bd00b957fd6f0bb41228635170befa`

Imports: [../../app/archive/history](../../app/archive/history.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/archive/test_pdf_cache.ep`

[tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep) · 44 lines · 3142 bytes

SHA-256: `852ab9529fd579b5e6778e94273ea021f12934779bbe99a55376be0f67007131`

Imports: [../../app/archive/pdf_cache](../../app/archive/pdf_cache.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/archive/test_pdf_text.ep`

[tests/archive/test_pdf_text.ep](../../tests/archive/test_pdf_text.ep) · 30 lines · 1935 bytes

SHA-256: `2241109e4a0daa8873bb78bf4d5ee344792da46dc200bab01cad6575f624333b`

Imports: [../../app/archive/pdf_text](../../app/archive/pdf_text.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_restore_publication.ep`

[tests/archive/test_restore_publication.ep](../../tests/archive/test_restore_publication.ep) · 28 lines · 1488 bytes

SHA-256: `5d8b218871156165444e281ae7770a5a052976759a53124cea990136f653daba`

Imports: [../../app/archive/attachment_restore](../../app/archive/attachment_restore.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_restore_release_hold.ep`

[tests/archive/test_restore_release_hold.ep](../../tests/archive/test_restore_release_hold.ep) · 26 lines · 2273 bytes

SHA-256: `5ab48de92c17c374ed6157384f8f470a7a284221398cca9cfcc142a69fff3f2e`

Imports: [../../app/archive/restore_release](../../app/archive/restore_release.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_restored_files.ep`

[tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep) · 25 lines · 1472 bytes

SHA-256: `9c2547138ad6198fca4abe534b34fdda34696cb03ebef58e3cd8a1fc32d2c904`

Imports: [../../app/archive/attachment_restore](../../app/archive/attachment_restore.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_snapshot.ep`

[tests/archive/test_snapshot.ep](../../tests/archive/test_snapshot.ep) · 61 lines · 2890 bytes

SHA-256: `431e13d0c28305acd51b39d3d6a905cea2c0e234c0ab88c5620aeb62fcd6719b`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/archive/test_tree_snapshot.ep`

[tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep) · 62 lines · 3853 bytes

SHA-256: `ba1424b53ecff1b13015e5a1ccbd239b6758039cb6568412a28cd4f7e2ba3e38`

Imports: [../../app/archive/file_copy](../../app/archive/file_copy.ep), `fs` (toolchain/library), [../../app/archive/tree_snapshot](../../app/archive/tree_snapshot.ep), [../../app/workspace/create_text](../../app/workspace/create_text.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/context/measure_recap_cache.ep`

[tests/context/measure_recap_cache.ep](../../tests/context/measure_recap_cache.ep) · 16 lines · 890 bytes

SHA-256: `61d6a70ab5a72bd046ecd9b019e9a535669e262d8683cda4240790dbcf8f7531`

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/seed_recap_viewer.ep`

[tests/context/seed_recap_viewer.ep](../../tests/context/seed_recap_viewer.ep) · 33 lines · 2563 bytes

SHA-256: `e032c24cdb631526297c1c89fbef7d8d363dfcaec931dd2b2fffb91ab9737436`

Source annotations: Host-only UI fixture: create a NEW explicitly labelled conversation for an existing test device. Never alter or compact an existing conversation.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/control](../../app/agent/control.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/context/test_attachment_gate.ep`

[tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep) · 78 lines · 5295 bytes

SHA-256: `a53c108aabceb95f877a5d50c3ee7b6d0a20051f2cc09c3fe4004decaad02ce6`

Source annotations: A branch inherits only its frozen ancestor prefix.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_attachment_recap_sources.ep`

[tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep) · 203 lines · 15310 bytes

SHA-256: `34883a6b81b5a79585016045856e59611c762234f3d03a87e8c84b83578ff582`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/context/recap_sources](../../app/context/recap_sources.ep), [../../app/context/attachment_revision](../../app/context/attachment_revision.ep), [../../app/context/recap_repair](../../app/context/recap_repair.ep), [../../app/context/recap_merge](../../app/context/recap_merge.ep), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
13: define main returning Int:
```

## `tests/context/test_automatic_compaction.ep`

[tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep) · 104 lines · 7363 bytes

SHA-256: `02225dc2a4bb49d1e9e5c218bbf540459529f61fcc259a213062f2cc4cadd620`

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_budget.ep`

[tests/context/test_budget.ep](../../tests/context/test_budget.ep) · 39 lines · 1937 bytes

SHA-256: `5a7cc4af3e8f2979279df201911ea468674cab1afd1c3ac339d547c268db87f2`

Imports: [../../app/context/budget](../../app/context/budget.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_checkpoint.ep`

[tests/context/test_checkpoint.ep](../../tests/context/test_checkpoint.ep) · 85 lines · 4452 bytes

SHA-256: `5c4800817231cbf55afc7dbd10150bbf51712f843f3bba02500a39e462e078c4`

Source annotations: Close and reopen between preparation and activation, as after a restart.

Imports: [../../app/context/checkpoint](../../app/context/checkpoint.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_checkpoint_interruption.ep`

[tests/context/test_checkpoint_interruption.ep](../../tests/context/test_checkpoint_interruption.ep) · 37 lines · 1504 bytes

SHA-256: `a2a69730eb13c321cab88eef6361f40d9c990419d120f640506f495e6654437d`

Source annotations: Run after test_checkpoint. Interrupt changes only its disposable database. SIGKILL prevents normal shutdown and SQLite connection cleanup.

Imports: [../../app/context/checkpoint](../../app/context/checkpoint.ep), `os` (toolchain/library), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/context/test_compaction_crash.ep`

[tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep) · 66 lines · 4625 bytes

SHA-256: `1be4719f6fa5ed80f6550cc3df22fc4f44a744ca1d2da704eedcf806a7690c3e`

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_compaction_lock.ep`

[tests/context/test_compaction_lock.ep](../../tests/context/test_compaction_lock.ep) · 32 lines · 1590 bytes

SHA-256: `7969c47bcc6ee191d97ab99b2abfcd23f349681e69b18651b96f4874ed3a4618`

Imports: [../../app/context/compaction_lock](../../app/context/compaction_lock.ep), `os` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_compaction_read.ep`

[tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep) · 59 lines · 4567 bytes

SHA-256: `9afabe06756656544aba5f3dfa9ad9012a89b2b31ddd9724f84d4e497e0fa836`

Imports: [../../app/web/compaction](../../app/web/compaction.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define recap_read_field with body as Str and name as Str returning Str:
8: define main returning Int:
```

## `tests/context/test_context_boundaries.ep`

[tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep) · 32 lines · 1601 bytes

SHA-256: `46d975b222aca784f069c8a5363e58f28bbd0237ad9e97dd1ff5f0c29b7c3522`

Imports: [../../app/context/session_messages](../../app/context/session_messages.ep).

Environment names: `ECHO_IDENTITY_FILE`.

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_group_recap_authority.ep`

[tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep) · 67 lines · 4748 bytes

SHA-256: `2e04565e084164423f2c49128532b2ca54c782565cb247764ce44d1d17e36425`

Source annotations: Start beyond the fixture's artificial capacity trigger: this test must rebuild because of an explicit request while ordinary context still fits.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define rebuild_provider_fixture with listener as List and objective as Str and completed as Int and start as Int and finish as Int returning Int:
23: define main returning Int:
```

## `tests/context/test_group_recap_membership.ep`

[tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep) · 83 lines · 5704 bytes

SHA-256: `2edf5548355b2b1a2200fb7c9aac53ca2e0db16d79f35553d014d7b295a219d8`

Source annotations: Start beyond the fixture's artificial capacity trigger: this test must rebuild because of an explicit request while ordinary context still fits.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define rebuild_provider_fixture with listener as List and objective as Str and completed as Int and start as Int and finish as Int and archive_path as Str returning Int:
24: define main returning Int:
```

## `tests/context/test_group_recap_rebuild.ep`

[tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep) · 158 lines · 10582 bytes

SHA-256: `09a2d938ac94e025055c5c94ee46f6447038a8e7a7106f6ba915cc798eb35c74`

Source annotations: Start beyond the fixture's artificial capacity trigger: this test must rebuild because of an explicit request while ordinary context still fits.

Imports: [../../app/web/compaction](../../app/web/compaction.ep), [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/security/channel_silo](../../app/security/channel_silo.ep), [../../app/security/admin_silo](../../app/security/admin_silo.ep), [../../app/security/system_silo](../../app/security/system_silo.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define rebuild_provider_fixture with listener as List and objective as Str and completed as Int and start as Int and finish as Int returning Int:
27: define main returning Int:
```

## `tests/context/test_live_group_recap.ep`

[tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep) · 54 lines · 4406 bytes

SHA-256: `ff0db80226296298edd2e56f21c009468f03bae9558616ffc50b385cf22be916`

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/context/test_live_recap_rewrite.ep`

[tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep) · 30 lines · 2703 bytes

SHA-256: `6fbbe18650d8da54f3c03874ab21924b2f475c9e9c43b3efa56a14e4a54f82da`

Imports: [../../app/context/recap_repair](../../app/context/recap_repair.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_live_recap_semantics.ep`

[tests/context/test_live_recap_semantics.ep](../../tests/context/test_live_recap_semantics.ep) · 25 lines · 2651 bytes

SHA-256: `b1a3d4d4ba18951cf88aafbcbac25208a1e6d5efeefce5dad8e6e67deb926906`

Source annotations: Accept equivalent prose and arithmetic grounded in the record, but reject an invented side effect. Uses the actual model and unchanged review validator.

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/context/test_live_recorded_review.ep`

[tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep) · 40 lines · 2592 bytes

SHA-256: `6888133e8ab881001d190b07cb000c5a11e26bc099442e42c9b3eb42df75d932`

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_memory_provenance_snapshot.ep`

[tests/context/test_memory_provenance_snapshot.ep](../../tests/context/test_memory_provenance_snapshot.ep) · 23 lines · 790 bytes

SHA-256: `c0aed39d72a9f65025be8eb6aee6edb94b06c186f31f4a94e15e500f6fd53df3`

Imports: [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_merge_resume.ep`

[tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep) · 100 lines · 7284 bytes

SHA-256: `6925061d547a337ac4628af9d99a6788cccce32b374e450742ffb9b28243bee7`

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define merge_note_fixture with database as Int and identity as Str and start as Int and finish as Int and notes as Str returning Int:
11: define merge_provider_fixture with listener as List and completed as Int and start as Int and prior as Str and finish as Int returning Int:
31: define main returning Int:
```

## `tests/context/test_progressive_merge.ep`

[tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep) · 62 lines · 4466 bytes

SHA-256: `9e4b7650e28a479aee187878ed8139752dad2800fcf9fbd14e2da743a6cf5d47`

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define merge_note_fixture with database as Int and identity as Str and start as Int and finish as Int and notes as Str returning Int:
11: define merge_provider_fixture with listener as List and completed as Int returning Int:
28: define main returning Int:
```

## `tests/context/test_prompt_prefix.ep`

[tests/context/test_prompt_prefix.ep](../../tests/context/test_prompt_prefix.ep) · 16 lines · 729 bytes

SHA-256: `a763e409b77c43805b338665c779d668efa891ec998713b9e4c89f905d8c79d6`

Imports: [../../app/context/system_awareness](../../app/context/system_awareness.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_recap_cache_prefix.ep`

[tests/context/test_recap_cache_prefix.ep](../../tests/context/test_recap_cache_prefix.ep) · 34 lines · 2412 bytes

SHA-256: `966d9445007c839f615271388f028b1c414ef597944c5ff2c6e82c995502d5b1`

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_recap_collect.ep`

[tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep) · 78 lines · 5340 bytes

SHA-256: `e435fdae69c9a8beb9262a9fedc8d54f28a185f1b4464f0334d436b1d5b471c4`

Imports: [../../app/context/recap_collect](../../app/context/recap_collect.ep), [../../app/context/recap_merge](../../app/context/recap_merge.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define collect_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:
13: define collect_provider with listener as List and bodies as List and completed as Int returning Int:
28: define main returning Int:
```

## `tests/context/test_recap_contract.ep`

[tests/context/test_recap_contract.ep](../../tests/context/test_recap_contract.ep) · 23 lines · 2277 bytes

SHA-256: `f7f16aa63b4b858196c6ed37fea21577bcd3193b5c5a38f607b604909646279a`

Imports: [../../app/context/recap_contract](../../app/context/recap_contract.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_recap_findings.ep`

[tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep) · 39 lines · 3657 bytes

SHA-256: `08b6fd895bc91a958c8bd5974145db48a702b8a16e017385b32bd71f561942fb`

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep), [../../app/context/recap_fragment_review](../../app/context/recap_fragment_review.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_recap_fragment.ep`

[tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep) · 56 lines · 3736 bytes

SHA-256: `6d0915fcdbfa6d6a934151bbf2c847d0f5f39795e0edb425c8c25b8d7c481e80`

Imports: [../../app/context/recap_fragment](../../app/context/recap_fragment.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define fragment_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:
11: define fragment_provider with listener as List and full as Str and half as Str and completed as Int returning Int:
27: define main returning Int:
```

## `tests/context/test_recap_fragment_review.ep`

[tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep) · 122 lines · 10911 bytes

SHA-256: `c8f207f8296d464168a36e6b1f6e49b0325083425179ee2dc6ea19e5683b848e`

Imports: [../../app/context/recap_review_synthesis](../../app/context/recap_review_synthesis.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define review_fragment_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:
12: define review_fragment_provider with listener as List and full as Str and half as Str and report as Str and completed as Int returning Int:
32: define main returning Int:
113: define review_remainder_provider with listener as List and body as Str and report as Str and completed as Int returning Int:
```

## `tests/context/test_recap_generate.ep`

[tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep) · 122 lines · 11036 bytes

SHA-256: `94f20f57c1af778097215b57338805543ec677fa65b89621b275a91d9a653b85`

Imports: [../../app/context/recap_accept](../../app/context/recap_accept.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define recap_fixture_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:
12: define recap_fixture with listener as List and body as Str and candidate as Str and completed as Int returning Int:
23: define acceptance_fixture with listener as List and body as Str and counted as Str and change_path as Str and completed as Int returning Int:
38: define acceptance_expected_body with database as Int and identity as Str and candidate as Str returning Str:
49: define main returning Int:
```

## `tests/context/test_recap_indexed_review.ep`

[tests/context/test_recap_indexed_review.ep](../../tests/context/test_recap_indexed_review.ep) · 47 lines · 3827 bytes

SHA-256: `fdb18462a155bad07cf9be4d63bf3be745de92368088297f2c291281d1872e74`

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_recap_jobs.ep`

[tests/context/test_recap_jobs.ep](../../tests/context/test_recap_jobs.ep) · 49 lines · 3527 bytes

SHA-256: `57e4876c6aef31f7b154564ed7c814af68ffa74635c29cf9fc54bf3a049b8d0a`

Source annotations: Synthetic notes isolate range-accounting tests; they are not model evaluation.

Imports: [../../app/context/recap_jobs](../../app/context/recap_jobs.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define fixture_note with database as Int and identity as Str and job as Str and digest as Str and start as Int and finish as Int returning Int:
11: define main returning Int:
```

## `tests/context/test_recap_notes.ep`

[tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep) · 59 lines · 4276 bytes

SHA-256: `2be7fadc4c3e4667794f46fbe216dc6e52772a241ec17a3e09ee0d944ab02081`

Imports: [../../app/context/recap_notes](../../app/context/recap_notes.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define notes_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:
12: define notes_provider with listener as List and body as Str and reason as Str and completed as Int returning Int:
23: define main returning Int:
```

## `tests/context/test_recap_rebuild.ep`

[tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep) · 100 lines · 6376 bytes

SHA-256: `58cd952d6705e8fb83f663fafe282405d67930351f72809beb45ff4d14eefc3e`

Source annotations: Start beyond the fixture's artificial capacity trigger: this test must rebuild because of an explicit request while ordinary context still fits.

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define rebuild_provider_fixture with listener as List and objective as Str and completed as Int and start as Int and finish as Int returning Int:
22: define main returning Int:
```

## `tests/context/test_recap_review.ep`

[tests/context/test_recap_review.ep](../../tests/context/test_recap_review.ep) · 28 lines · 2116 bytes

SHA-256: `b92d3b3a0a7dda5e926320796aab01bf7d3552d0516b1f5ffbf96599fdcb4d24`

Imports: [../../app/context/recap_review](../../app/context/recap_review.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_recap_review_coverage.ep`

[tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep) · 62 lines · 5564 bytes

SHA-256: `af71f14eea99f196011354f1f538cf3c9524619a8507a79d6656cb4d6beca747`

Source annotations: Synthetic receipts isolate range accounting from model judgment quality.

Imports: [../../app/context/recap_review_coverage](../../app/context/recap_review_coverage.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define coverage_review_fixture with database as Int and job as Str and identity as Str and candidate as Str and start as Int and finish as Int and verdict as Str returning Int:
21: define main returning Int:
```

## `tests/context/test_recap_sources.ep`

[tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep) · 55 lines · 3521 bytes

SHA-256: `c54559a82a6c4acb97c1222b8500b6ed80162de194aaae4ca959286096c81a14`

Imports: [../../app/context/recap_sources](../../app/context/recap_sources.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_recap_synthesis_gate.ep`

[tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep) · 70 lines · 6575 bytes

SHA-256: `7f0b6cfda0c7c5f0fa795b17f38f98e6703aa2dc9aad813e6f9f86fe4d5d6b01`

Source annotations: Synthetic receipts isolate range accounting from model judgment quality.

Imports: [../../app/context/recap_accept](../../app/context/recap_accept.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define coverage_review_fixture with database as Int and job as Str and identity as Str and candidate as Str and start as Int and finish as Int and verdict as Str returning Int:
22: define synthesis_receipt_fixture with job as Str and candidate as Str returning Str:
29: define main returning Int:
```

## `tests/context/test_recap_upgrade.ep`

[tests/context/test_recap_upgrade.ep](../../tests/context/test_recap_upgrade.ep) · 34 lines · 2285 bytes

SHA-256: `ffa49f68ad4ccaff6aeb39325b0419d40dfc416d4210b5ee0555c54f4ee44322`

Imports: [../../app/context/compaction_resume](../../app/context/compaction_resume.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define save_draft with database as Int and identity as Str and candidate as Str and contract as Str returning Int:
8: define main returning Int:
```

## `tests/context/test_session_messages.ep`

[tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep) · 71 lines · 5163 bytes

SHA-256: `49c5b6cf689e5ca1eb0487549314e22dbe2944bffc0da9ec6deca7cf7953b888`

Imports: [../../app/context/session_messages](../../app/context/session_messages.ep).

Environment names: `ECHO_IDENTITY_FILE`.

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_system_observation_dedup.ep`

[tests/context/test_system_observation_dedup.ep](../../tests/context/test_system_observation_dedup.ep) · 19 lines · 1180 bytes

SHA-256: `6315a27d7a23fc676b37e28c2771f22de9ea434fa5d0e4235097ecfe3c2ac7fa`

Imports: [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/context/test_token_count_retry.ep`

[tests/context/test_token_count_retry.ep](../../tests/context/test_token_count_retry.ep) · 19 lines · 856 bytes

SHA-256: `c46acf3cfad9487118b8f465f5dcef2150a1913ce17fe01a102750ebe6c8431a`

Imports: [../../app/providers/llama_preflight](../../app/providers/llama_preflight.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/context/test_turn_clock.ep`

[tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep) · 27 lines · 1581 bytes

SHA-256: `4c9a0b28bf8a735f58c4ac6820c20ab1cdf6e3c85456141ee613cba5bd750d93`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/fixtures/delegation_fixture.ep`

[tests/fixtures/delegation_fixture.ep](../../tests/fixtures/delegation_fixture.ep) · 16 lines · 1161 bytes

SHA-256: `e78099fa60228039e66ea4f705ecf06f430c90aad7ec66d2160bec13ce13c2b1`

Source annotations: Requests are archived exactly as the provider dispatcher would archive them.

Imports: [../../app/jobs/delegation_run](../../app/jobs/delegation_run.ep).

Definitions:

```text
4: define delegation_fixture_call with database as Int and run as Str and session as Str and call_id as Str and name as Str and arguments as Str returning Int:
15: define delegation_fixture_request with database as Int and run as Str and session as Str and call_id as Str and arguments as Str returning Int:
```

## `tests/fixtures/echo_http_client.ep`

[tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep) · 75 lines · 4639 bytes

SHA-256: `07f8f2f7df5e82a8e11df5329763cc1d93ccb089832d823254ce0975a283127c`

Source annotations: Loopback acceptance fixture only; credentials are supplied by its caller.

Imports: [../../app/providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
4: define fixture_echo_exchange with reader as List and method as Str and port as Int and path as Str and body as Str and credential as Str and read_size as Int returning List:
56: define fixture_echo_request with port as Int and path as Str and body as Str and credential as Str returning List:
```

## `tests/fixtures/generation_fixture.ep`

[tests/fixtures/generation_fixture.ep](../../tests/fixtures/generation_fixture.ep) · 33 lines · 1549 bytes

SHA-256: `6172bf8e25398f85c4682c6da403ea26309523a7cab5b1d2dd0fccb17c952a8c`

Imports: [../../app/agent/generation](../../app/agent/generation.ep).

Definitions:

```text
3: define generation_fixture with listener as List and data as Str and completed as Int returning Int:
```

## `tests/fixtures/json_fixture.ep`

[tests/fixtures/json_fixture.ep](../../tests/fixtures/json_fixture.ep) · 43 lines · 2321 bytes

SHA-256: `632cb793a9e98846ffd431b578ee5eb77052067354f1bfc2bd6c2d2eefc18497`

Imports: [../../app/providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
3: define json_fixture with listener as List and expected_method as Str and expected_path as Str and expected_body as Str and response as Str and completed as Int returning Int:
```

## `tests/fixtures/protected_reviews.ep`

[tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep) · 69 lines · 5916 bytes

SHA-256: `f3124e7db7b896a3e57dd95a1d5f9ad21f687b09ef5cf0f5ebd9577122ca5adf`

Source annotations: Persist the same original event envelope used by the native tool ledger. Model request originals let the verifier distinguish seeing a disclosure from merely queuing an inspection beside a review in the same generation.

Imports: [../../app/rights/reviews](../../app/rights/reviews.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Definitions:

```text
5: define inspection_fixture with database as Int and session as Str and call_id as Str and name as Str and manifest as Str and digest as Str and observed as Int returning Int:
24: define review_fixture with database as Int and session as Str and call_id as Str and decision as Str and digest as Str and include_disclosure as Int and previous_revision as Int returning Int:
52: define fixture_review_store with database as Int and run as Str and call_id as Str returning Str:
```

## `tests/fixtures/runner_provider.ep`

[tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep) · 336 lines · 22787 bytes

SHA-256: `a23a4b97458cd9792a8e854d8ac64129417a1f061cff5a169dfba134f5c1b0f2`

Source annotations: Three scripted generations: recap, independent review, resumed final answer. Only this fixture's first token count reports full capacity. Normalize the cache-stable three-message recap transport for the scripted provider. The source evidence remains a separate production message.

Imports: [../../app/security/groups](../../app/security/groups.ep), [../../app/agent/runner](../../app/agent/runner.ep), `os` (toolchain/library).

Definitions:

```text
7: define compaction_fixture_content with state as List and tokens as List and messages as List returning Str:
128: define runner_fixture_call with name as Str and identity as Str and arguments as Str returning Str:
133: define runner_fixture_exchange with listener as List and state as List returning Int:
263: define runner_fixture with listener as List and objective as Str and completed as Int returning Int:
283: define compaction_fixture with listener as List and mode as Int and completed as Int returning Int:
314: define compaction_recovery_fixture with listener as List and candidate as Str and endpoint as Str and review_saved as Int and completed as Int returning Int:
```

## `tests/fixtures/schedule_fixture.ep`

[tests/fixtures/schedule_fixture.ep](../../tests/fixtures/schedule_fixture.ep) · 13 lines · 867 bytes

SHA-256: `28f7800a04342011187efc3888149449b2d19db355873e5f2105e48280aa4326`

Source annotations: Native storage acceptance; no provider or production scheduler is started.

Imports: [../../app/jobs/schedule_store](../../app/jobs/schedule_store.ep).

Definitions:

```text
4: define schedule_fixture_origin with database as Int and device as Str and run as Str returning Int:
```

## `tests/fixtures/tool_call_fixture.ep`

[tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep) · 21 lines · 1712 bytes

SHA-256: `891544cb2cc401c85b517a0f3f275e51c2045fac6384a0b27cbabdf204347766`

Imports: [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [generation_fixture](../../tests/fixtures/generation_fixture.ep).

Definitions:

```text
4: define tool_fixture_call with database as Int and device as Str and run as Str and name as Str and arguments as Str returning Str:
```

## `tests/integration/test_component_imports.ep`

[tests/integration/test_component_imports.ep](../../tests/integration/test_component_imports.ep) · 16 lines · 624 bytes

SHA-256: `7e0ddf3ea77f1cc3ffbcdecb2bc7e3714061b9ea0cfb674af71a6d57a397a240`

Imports: [../../app/agent/run_state](../../app/agent/run_state.ep), [../../app/archive/history](../../app/archive/history.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/context/budget](../../app/context/budget.ep), [../../app/transport/http_stream](../../app/transport/http_stream.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/interaction/test_controls.ep`

[tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep) · 142 lines · 12384 bytes

SHA-256: `b454edb9f49158a4b82502b02ba2e2f772e823cd8ee277364a7ddea5273ba869`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/web/interaction](../../app/web/interaction.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation.ep`

[tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep) · 50 lines · 4344 bytes

SHA-256: `537962e5acb48aea8256ec204828961842c425498f8ae0acf48a16416f9c49e6`

Imports: [../../app/jobs/delegation](../../app/jobs/delegation.ep), [../../app/jobs/schedule_store](../../app/jobs/schedule_store.ep), [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_control.ep`

[tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep) · 63 lines · 5487 bytes

SHA-256: `cb147e5fbac37b9757d92602f8d0cd81a4525b739b052d242ba41a2c1b282ea1`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/tools/delegation_tool](../../app/tools/delegation_tool.ep), [../../app/tools/job_control](../../app/tools/job_control.ep), [../../app/agent/generation](../../app/agent/generation.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_cross_turn.ep`

[tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep) · 87 lines · 8359 bytes

SHA-256: `2f886b9ad75de94789477809a56fb9434924cb3b814a607678432b457d3e74ff`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/tools/delegation_tool](../../app/tools/delegation_tool.ep), [../../app/tools/job_control](../../app/tools/job_control.ep), [../../app/agent/generation](../../app/agent/generation.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_monitor.ep`

[tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep) · 100 lines · 9738 bytes

SHA-256: `f6edb543327298823b4e8aea05258f8be757b0cae6da99910ec00e311e06b96d`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/tools/delegation_tool](../../app/tools/delegation_tool.ep), [../../app/tools/job_control](../../app/tools/job_control.ep), [../../app/agent/generation](../../app/agent/generation.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_nonblocking.ep`

[tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep) · 27 lines · 1763 bytes

SHA-256: `e5be1c6ad76ac42a86dadf9c0252183d8606173e86cf55949065a16da7238f04`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/jobs/test_delegation_prefix.ep`

[tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep) · 71 lines · 6197 bytes

SHA-256: `8f4af85156a55f7586d203aa3116e9b9de84eeb20f160ed69f333a19f660b241`

Source annotations: Migrate populated groups, preserving existing origins and all references.

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/web/worker](../../app/web/worker.ep), [../../app/security/group_route](../../app/security/group_route.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_return.ep`

[tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep) · 61 lines · 4770 bytes

SHA-256: `66ce383d3119e65bdbb3686d0dcdee9c5039d367a99608e9fe504a5714075297`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/jobs/delegation_return](../../app/jobs/delegation_return.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/web/work_status](../../app/web/work_status.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_return_overlap.ep`

[tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep) · 78 lines · 6289 bytes

SHA-256: `b9ce45e059c7af011ead77811a191db73a1d99255b049e235e0d1925bfc327c2`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/jobs/delegation_return](../../app/jobs/delegation_return.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/web/work_status](../../app/web/work_status.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_delegation_run.ep`

[tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep) · 65 lines · 5877 bytes

SHA-256: `0da8b6f6df98c76d5b4c2a48ee2c7bb16948be49391170de90e0ea94225f5acd`

Source annotations: Migrate populated groups, preserving existing origins and all references.

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/web/worker](../../app/web/worker.ep), [../../app/security/group_route](../../app/security/group_route.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_live_checklist_return.ep`

[tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep) · 58 lines · 4102 bytes

SHA-256: `cd8c8a29c1a27f871fabca9fe7d956ec9c4a9131afcaf9d00e808cd7bd67fc1d`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_return](../../app/jobs/delegation_return.ep), [../../app/jobs/task_checklist](../../app/jobs/task_checklist.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/jobs/test_live_delegation_message.ep`

[tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep) · 54 lines · 4188 bytes

SHA-256: `48b67f8224ecd048d4379b90b38ce7df978567a10eb076dbe8caad85b9b7e8e3`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/tools/delegation_tool](../../app/tools/delegation_tool.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/jobs/test_live_delegation_tool.ep`

[tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep) · 30 lines · 3041 bytes

SHA-256: `44390061b0c7d944571da8ae3573c8ab4efa76b0bc64da7c5ae71284c75be524`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/jobs/test_live_schedule_tool.ep`

[tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep) · 30 lines · 3020 bytes

SHA-256: `e0b65be893ccda16b9aa2b79666a2bf34fcd05290ce889e0937da485713a0b71`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/schedule_store](../../app/jobs/schedule_store.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/jobs/test_schedule_run.ep`

[tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep) · 56 lines · 5180 bytes

SHA-256: `8b24fbcb72a7012b63e6c8114097550be74351483dc1ea788b14201db8086efa`

Source annotations: Optional model/port runs the actual normal worker after native admission checks.

Imports: [../../app/jobs/schedule_run](../../app/jobs/schedule_run.ep), [../../app/agent/control](../../app/agent/control.ep), [../fixtures/schedule_fixture](../../tests/fixtures/schedule_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/jobs/test_schedule_service.ep`

[tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep) · 36 lines · 3151 bytes

SHA-256: `27fa11243b8934a4645c1afe1f7e4b04b5eedbbe31c5a9f137d903ad853d4c16`

Imports: [../../app/jobs/schedule_service](../../app/jobs/schedule_service.ep), [../../app/agent/control](../../app/agent/control.ep), [../fixtures/schedule_fixture](../../tests/fixtures/schedule_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/jobs/test_schedule_store.ep`

[tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep) · 45 lines · 4543 bytes

SHA-256: `6cbcfde9a53439a6edf6be818d7b58c9050cdecf63e41386ebf49c5c44adab2a`

Imports: [../fixtures/schedule_fixture](../../tests/fixtures/schedule_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/jobs/test_schedule_tool.ep`

[tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep) · 48 lines · 3713 bytes

SHA-256: `7b4b7293ba7c860ef1c6e169757da348fa8ce2a2bfd525202e5f368269041cd6`

Imports: [../fixtures/tool_call_fixture](../../tests/fixtures/tool_call_fixture.ep), [../../app/jobs/schedule_run](../../app/jobs/schedule_run.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define schedule_fixture_status with result as Str returning Str:
8: define main returning Int:
```

## `tests/jobs/test_task_checklist.ep`

[tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep) · 86 lines · 7654 bytes

SHA-256: `8000112f08e65a8e063743a73d88e9b6bcc5d5f31ca3c136fc65302bc5cee421`

Imports: [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/lifecycle/controller_console_preview.ep`

[tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep) · 32 lines · 1797 bytes

SHA-256: `698d0c57ab5b7cffa00f77d4b28407cec2253042d019bf729c809d96f7b8c3ba`

Source annotations: Disposable visual fixture. No ledger, credential, model or mutation endpoint.

Imports: [../../app/lifecycle/controller_console](../../app/lifecycle/controller_console.ep), [../../app/lifecycle/controller_http](../../app/lifecycle/controller_http.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/hold_archive_lock.ep`

[tests/lifecycle/hold_archive_lock.ep](../../tests/lifecycle/hold_archive_lock.ep) · 16 lines · 795 bytes

SHA-256: `f7431ba9d7c5dd4efd5f05171993edcdf03f4d3541a07ed9e5369194535b18d3`

Source annotations: Orchestrated transport fixture. Lock only an explicitly named test archive; the release file controls duration without changing application timeouts.

Imports: [../../app/lifecycle/startup](../../app/lifecycle/startup.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_controller_bootstrap.ep`

[tests/lifecycle/test_controller_bootstrap.ep](../../tests/lifecycle/test_controller_bootstrap.ep) · 24 lines · 1655 bytes

SHA-256: `99827e4775ef31df8139abaabb3ae042206f831d195b3808fc27432859a7d84c`

Imports: [../../app/lifecycle/controller_bootstrap](../../app/lifecycle/controller_bootstrap.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_controller_bundle.ep`

[tests/lifecycle/test_controller_bundle.ep](../../tests/lifecycle/test_controller_bundle.ep) · 42 lines · 2881 bytes

SHA-256: `e14a5280f402e1b8cba783471c75a2b1e4c7c88d784e1872f4e43fd3e4b1774f`

Imports: [../../app/lifecycle/controller_bundle](../../app/lifecycle/controller_bundle.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/lifecycle/test_controller_bundle_privileged.ep`

[tests/lifecycle/test_controller_bundle_privileged.ep](../../tests/lifecycle/test_controller_bundle_privileged.ep) · 36 lines · 2582 bytes

SHA-256: `ec04361dc29d981217e9d503581fc188e6c96c0f200cc5a78b1d1fc5eeed59c8`

Source annotations: Deliberately limited root qualification: one known inert payload and two receipts under a digest-named directory in /private/var/root. No service starts, no production archive is opened, and no existing installation is overwritten.

Imports: [../../app/lifecycle/controller_bundle](../../app/lifecycle/controller_bundle.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/lifecycle/test_controller_credential.ep`

[tests/lifecycle/test_controller_credential.ep](../../tests/lifecycle/test_controller_credential.ep) · 28 lines · 1723 bytes

SHA-256: `1906e3f561c949621adb7291bd03272b4da6b27055722fddab67cec4e1af77b9`

Imports: [../../app/lifecycle/controller_credential](../../app/lifecycle/controller_credential.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_controller_enrollment.ep`

[tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep) · 46 lines · 3735 bytes

SHA-256: `323c5c7bf779a9117d3a81cbfe9e8415676784f463c6be10b16040d6eaf89f5c`

Source annotations: Fail the last write and require every preceding write to roll back.

Imports: [../../app/lifecycle/controller_enrollment](../../app/lifecycle/controller_enrollment.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_controller_initialize.ep`

[tests/lifecycle/test_controller_initialize.ep](../../tests/lifecycle/test_controller_initialize.ep) · 35 lines · 2468 bytes

SHA-256: `a231f437cd0a2229cc03c505dce49f518e20a8cc2753ad082ce5af98aac2bd63`

Source annotations: Fail after migration to test completion recovery without replacing intent.

Imports: [../../app/lifecycle/controller_initialize](../../app/lifecycle/controller_initialize.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_controller_launch.ep`

[tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep) · 33 lines · 2998 bytes

SHA-256: `1a6432a974511b6e4c0c8f7bd2234c4e91b261ee5a608634fdbba104d4e0f4e6`

Imports: [../../app/lifecycle/controller_launch](../../app/lifecycle/controller_launch.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_controller_launch_privileged.ep`

[tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep) · 63 lines · 4167 bytes

SHA-256: `283ed74facd106718fac36517bd441a90af8b07fb050c0ea37f8d949fd8bd387`

Source annotations: The installed copy is an inert probe. It never starts a service or opens Echo's real archive. Its only write is inside the dedicated staged bundle.

Imports: [../../app/lifecycle/controller_launch](../../app/lifecycle/controller_launch.ep), [../../app/workspace/native_process](../../app/workspace/native_process.ep), `fs` (toolchain/library).

Environment names: `ECHO_LAUNCH_SECRET`, `PATH`.

Native `main` present. Argument indices read: 0, 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/lifecycle/test_controller_ledger.ep`

[tests/lifecycle/test_controller_ledger.ep](../../tests/lifecycle/test_controller_ledger.ep) · 23 lines · 1205 bytes

SHA-256: `ab243ce238ae6882c107831ad4ba7c6222b36daa55b517353560c209ae3365cc`

Source annotations: This process is intentionally unprivileged. It must not create a database, even inside an otherwise writable directory supplied by its caller.

Imports: [../../app/lifecycle/controller_ledger](../../app/lifecycle/controller_ledger.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_controller_ledger_privileged.ep`

[tests/lifecycle/test_controller_ledger_privileged.ep](../../tests/lifecycle/test_controller_ledger_privileged.ep) · 68 lines · 4615 bytes

SHA-256: `8953d870fa2e4a984b20befde421cf777dee614173bd5684737bd1cca8fcf982`

Source annotations: This fixture changes only its newly created directory. It neither installs a controller nor opens the production archive. A pre-existing directory aborts.

Imports: [../../app/lifecycle/controller_ledger](../../app/lifecycle/controller_ledger.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_controller_operations.ep`

[tests/lifecycle/test_controller_operations.ep](../../tests/lifecycle/test_controller_operations.ep) · 57 lines · 4562 bytes

SHA-256: `2ada9d1f64e3a4f663fb63fe479908f4073223a244e6d39fbb5ff4f2614e764a`

Source annotations: Configuration is not authorization; these checks exercise parsing and the actual unprivileged command boundary without claiming a privileged deployment.

Imports: [../../app/lifecycle/controller_operations](../../app/lifecycle/controller_operations.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_controller_operations_privileged.ep`

[tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep) · 110 lines · 9050 bytes

SHA-256: `58e70d8cd1d838650b8462ed954da4f9632253e5e285218cc2ae02486fb6d5a6`

Imports: [../../app/lifecycle/controller_operations](../../app/lifecycle/controller_operations.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define protected_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
26: define root_operation_expect with configuration as Str and action as Str and expected as Str returning Int:
34: define main returning Int:
```

## `tests/lifecycle/test_controller_release.ep`

[tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep) · 34 lines · 1882 bytes

SHA-256: `a74fada4d51f391e84066cab7a1eb8e0b2141ba93ada42e55d2ad0be162e345d`

Source annotations: One lookup validates every entry and duplicate before returning. Repeating that complete scan once per target adds no coverage.

Imports: [../../app/lifecycle/controller_operations](../../app/lifecycle/controller_operations.ep), [../../app/lifecycle/controller_bundle](../../app/lifecycle/controller_bundle.ep), [../../app/rights/inventory](../../app/rights/inventory.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_controller_requests.ep`

[tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep) · 377 lines · 31616 bytes

SHA-256: `64c4ad511df24bb59e7b4c7305a8d2443aa83c9f18ed2bfcf8fb070cf3704817`

Imports: [../../app/lifecycle/controller_prepare](../../app/lifecycle/controller_prepare.ep), [../../app/lifecycle/controller_http](../../app/lifecycle/controller_http.ep), [../../app/lifecycle/controller_enrollment](../../app/lifecycle/controller_enrollment.ep), [../../app/lifecycle/controller_review_runner](../../app/lifecycle/controller_review_runner.ep), [../fixtures/echo_http_client](../../tests/fixtures/echo_http_client.ep), [../../app/lifecycle/controller_operations](../../app/lifecycle/controller_operations.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
11: define protected_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
53: define controller_fixture_child returning Int:
71: define controller_fixture_request with database as Int and charter as Str and inventory as Str and digest as Str and body as Str and credential as Str returning List:
74: define controller_fixture_path with database as Int and charter as Str and inventory as Str and digest as Str and path as Str and body as Str and credential as Str returning List:
117: define service_fixture_operation with configuration as Str and action as Str and identity as Str returning Str:
143: define root_operation_expect with configuration as Str and action as Str and expected as Str returning Int:
152: define main returning Int:
```

## `tests/lifecycle/test_controller_review_inputs.ep`

[tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep) · 54 lines · 3787 bytes

SHA-256: `2ae58ea48c2ec6592dad1776d45c630ae11e2dfc1aa0f726e6f1029c1928e529`

Imports: [../../app/lifecycle/controller_review_inputs](../../app/lifecycle/controller_review_inputs.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_controller_service.ep`

[tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep) · 48 lines · 2786 bytes

SHA-256: `da62545e82deb881407d83e79ce7ae24a718ec0abac393bae6ecd225f3145f97`

Source annotations: This test never impersonates root or installs a service. Root qualification remains separate; the probe checks the actual native child launch machinery.

Imports: [../../app/lifecycle/controller_service](../../app/lifecycle/controller_service.ep).

Environment names: `ECHO_CONTROLLER_PROBE_SECRET`, `PATH`.

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_guardian_readiness.ep`

[tests/lifecycle/test_guardian_readiness.ep](../../tests/lifecycle/test_guardian_readiness.ep) · 25 lines · 1358 bytes

SHA-256: `a1f9d8cb17104e5bf60af9f859dc369e21390e5c72c2b46f219a86441a619163`

Imports: [../../app/providers/archived_stream](../../app/providers/archived_stream.ep), [../../app/lifecycle/service_profile](../../app/lifecycle/service_profile.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_live_controller_review.ep`

[tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep) · 36 lines · 3404 bytes

SHA-256: `33dc60adecb4b621f9d67cae3e7913de27c70259ebe76aa20d6beb636fca28d2`

Source annotations: Live primary-model qualification. Only a disposable proposal is reviewed; no identity file, prompt, executable or protected target is changed.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/lifecycle/controller_review_runner](../../app/lifecycle/controller_review_runner.ep), [../../app/rights/review_upgrade](../../app/rights/review_upgrade.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/lifecycle/test_migration_interruption.ep`

[tests/lifecycle/test_migration_interruption.ep](../../tests/lifecycle/test_migration_interruption.ep) · 43 lines · 2186 bytes

SHA-256: `3e76ad0cee7981f4e64aad6a35a8832238288e41e015347cb3e60fa3b0c0be14`

Source annotations: Deliberately long disposable SQL, so the driver kills a live native migration after observing its on-disk rollback journal.

Imports: [../../app/lifecycle/migrations](../../app/lifecycle/migrations.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_migrations.ep`

[tests/lifecycle/test_migrations.ep](../../tests/lifecycle/test_migrations.ep) · 45 lines · 2907 bytes

SHA-256: `0398e439c471cad3cf181f8a53c44e46c474ccda0c3605cff6fa79bb53fa5284`

Source annotations: A failing second statement must undo both earlier DDL and the receipt table.

Imports: [../../app/lifecycle/migrations](../../app/lifecycle/migrations.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/lifecycle/test_operations.ep`

[tests/lifecycle/test_operations.ep](../../tests/lifecycle/test_operations.ep) · 54 lines · 3788 bytes

SHA-256: `169df1237f8869713a8d006e510deddae7b16e4838cccd7740239ef37bc9cca8`

Imports: [../../app/lifecycle/operations](../../app/lifecycle/operations.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/lifecycle/test_protected_path.ep`

[tests/lifecycle/test_protected_path.ep](../../tests/lifecycle/test_protected_path.ep) · 32 lines · 1653 bytes

SHA-256: `8c974c463defcdf07dd04f7157e1f74f62e9b4fc41a58999d351f093d0e3b8bc`

Source annotations: Read-only qualification against the host's actual trusted OS paths and the current user-owned checkout. This test never grants privileges or changes ACLs.

Imports: [../../app/lifecycle/protected_path](../../app/lifecycle/protected_path.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/lifecycle/test_service.ep`

[tests/lifecycle/test_service.ep](../../tests/lifecycle/test_service.ep) · 61 lines · 3631 bytes

SHA-256: `56e8550d1901364f89cd0d15d3e456f32ab6af68ec90a777de5190ee57ac4c4e`

Imports: [../../app/lifecycle/service_profile](../../app/lifecycle/service_profile.ep), [../../app/workspace/native_process](../../app/workspace/native_process.ep), [../../app/providers/archived_stream](../../app/providers/archived_stream.ep), `fs` (toolchain/library).

Environment names: `ECHO_SERVICE_SECRET`, `ECHO_SERVICE_VALUE`.

Native `main` present. Argument indices read: 0, 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/lifecycle/test_startup.ep`

[tests/lifecycle/test_startup.ep](../../tests/lifecycle/test_startup.ep) · 26 lines · 1382 bytes

SHA-256: `ce95ffad0d27da594ca35b6e143f886a97b616bd71b2a42833dd1d41ba293f94`

Imports: [../../app/lifecycle/startup](../../app/lifecycle/startup.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/lifecycle/test_startup_busy.ep`

[tests/lifecycle/test_startup_busy.ep](../../tests/lifecycle/test_startup_busy.ep) · 40 lines · 1854 bytes

SHA-256: `6d0167404701ab723886d2cabc8cb634e0d7a0f8badbacaad3396be641a2bc3c`

Source annotations: A writer exceeding the bounded SQLite wait must be retryable, not an incompatible-schema verdict that permanently stops the supervisor.

Imports: [../../app/lifecycle/startup](../../app/lifecycle/startup.ep), `select` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define startup_wait_fixture with path as Str and completed as Int returning Int:
9: define main returning Int:
```

## `tests/media/image_wait_fixture.ep`

[tests/media/image_wait_fixture.ep](../../tests/media/image_wait_fixture.ep) · 7 lines · 175 bytes

SHA-256: `b01a84179c4c0eb2e030889b63e1fe1421fb4a53b7e5a1e391cb352339c81c09`

Imports: [../../app/providers/helper_process](../../app/providers/helper_process.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/media/test_enrolled_voice.ep`

[tests/media/test_enrolled_voice.ep](../../tests/media/test_enrolled_voice.ep) · 17 lines · 1281 bytes

SHA-256: `07cde778108025e9ddd582104914477a7b7725dc4e29d24a272d178166a94cdb`

Imports: [../../app/web/routes](../../app/web/routes.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/media/test_group_image_tool.ep`

[tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep) · 65 lines · 5111 bytes

SHA-256: `df12b00e45fd1ac63c303d7f6b9dc5055530be466e9ef16054cfa07f9d5e71a2`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/media/test_group_message_speech.ep`

[tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep) · 50 lines · 3846 bytes

SHA-256: `9d9b1a8bd04cd98a4e8d14c6271bf94678e3de203dc658c12fc352743bec3877`

Source annotations: Actual local synthesis from a member other than the source message sender.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/web/message_speech](../../app/web/message_speech.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/media/test_group_voice_tool.ep`

[tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep) · 79 lines · 6774 bytes

SHA-256: `450ea97f65db017fde0071c386eb4715cc77c05703922af61630ae07d1b4827d`

Source annotations: Script only the model request. Both synthesis and recognition are real.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define voice_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
25: define main returning Int:
```

## `tests/media/test_image_control.ep`

[tests/media/test_image_control.ep](../../tests/media/test_image_control.ep) · 75 lines · 4971 bytes

SHA-256: `de4ba3205b29f15acc30d3c222afa71326103367bdc5a17d67134ecc18da276d`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define image_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:
24: define main returning Int:
```

## `tests/media/test_image_logs.ep`

[tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep) · 70 lines · 4675 bytes

SHA-256: `c8373c3564e811b1274d832bf95f9f3c9b39f8aef3c804763d524975e342e433`

Imports: [../../app/media/image_logs](../../app/media/image_logs.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/media/test_image_request.ep`

[tests/media/test_image_request.ep](../../tests/media/test_image_request.ep) · 20 lines · 1851 bytes

SHA-256: `8c773a1cdc0ebefa79a9d481697ae420c74f923c6bb55d0df6eb2d91924aa5b0`

Imports: [../../app/media/image_request](../../app/media/image_request.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/media/test_image_tool.ep`

[tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep) · 52 lines · 3855 bytes

SHA-256: `3128ecff7196261a5c3d797c59a3112c7572e6f44e0166c0341af949e330987e`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/media/test_message_speech.ep`

[tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep) · 38 lines · 2522 bytes

SHA-256: `351ecf657aea7421cb2d6a5f2eed80f3bd6e29d6138ff518206565f9f58fd4a4`

Source annotations: Real Kokoro process, isolated archive; no model invocation is needed.

Imports: [../../app/web/message_speech](../../app/web/message_speech.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/media/test_speech_control.ep`

[tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep) · 77 lines · 5184 bytes

SHA-256: `41e847775826f8b93e0322151af6df437bbd3a19945e8f41b7d3e1f36966cb94`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define speech_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:
24: define main returning Int:
```

## `tests/media/test_speech_text.ep`

[tests/media/test_speech_text.ep](../../tests/media/test_speech_text.ep) · 21 lines · 1290 bytes

SHA-256: `bbfed6c547f79897199ef445b1f7626e07df509ab5b807cfdc35c1c8de8de3e0`

Imports: [../../app/media/speech_text](../../app/media/speech_text.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/media/test_speech_tool.ep`

[tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep) · 67 lines · 4911 bytes

SHA-256: `a5a266f73568191c2142680f774412d4ba3eff3eaf86d7313b8fefe2dde419ed`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/media/test_video_library.ep`

[tests/media/test_video_library.ep](../../tests/media/test_video_library.ep) · 42 lines · 2921 bytes

SHA-256: `71d8becda92ae565a9773e5706fec05e08021d4035e5a7efa90ce1779bbde977`

Imports: [../../app/web/routes](../../app/web/routes.ep), [../../app/media/video_library](../../app/media/video_library.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/media/test_video_request.ep`

[tests/media/test_video_request.ep](../../tests/media/test_video_request.ep) · 21 lines · 1563 bytes

SHA-256: `c00b10de22a037cd9d14ba30f8f07ea1cd8fac994f5d30ac9676a3b8ea5f7513`

Source annotations: Arguments stay argv, exact timestamps remain uncapped, and the local helper sandbox denies network and unrelated local files. Live decoding is separate.

Imports: [../../app/media/video_request](../../app/media/video_request.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/media/test_video_tool.ep`

[tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep) · 113 lines · 9488 bytes

SHA-256: `9b31c33f5a387d01da0a10f586311169906eaf5f40b299c42d53ff845fa5b726`

Source annotations: Script only the model request. Both synthesis and recognition are real.

Imports: [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define video_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
23: define main returning Int:
```

## `tests/media/test_voice_control.ep`

[tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep) · 75 lines · 5092 bytes

SHA-256: `a41666797114619bf8127fd6aa3a1578f7610e9fab5285b37bcdd708ad68b0b0`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define speech_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:
24: define main returning Int:
```

## `tests/media/test_voice_profile_switch.ep`

[tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep) · 49 lines · 3219 bytes

SHA-256: `9e04effdf449217afcdb2d635ab1c211be6f8ce27d72950ef2b4fbb744f5f2f1`

Source annotations: Real local synthesis: profile changes must invalidate message-only reuse.

Imports: [../../app/web/message_speech](../../app/web/message_speech.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/media/test_voice_tool.ep`

[tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep) · 66 lines · 5553 bytes

SHA-256: `3963a1ecf4edd07ffe90b96192d7f7873c69a53039a8ae84752168a11c429410`

Source annotations: Script only the model request. Both synthesis and recognition are real.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define voice_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
21: define main returning Int:
```

## `tests/media/test_watch_party.ep`

[tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep) · 137 lines · 10366 bytes

SHA-256: `b6b2562c394683d51b758edc24528b73a59df8c6f6c5ffb0f79ffd3ed1ca2f2f`

Imports: [../../app/web/routes](../../app/web/routes.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define watch_test_status with value as Str and expected as Str returning Int:
7: define main returning Int:
```

## `tests/memory/test_attachment_visibility_recall.ep`

[tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep) · 101 lines · 7359 bytes

SHA-256: `97eb8388e01aad7cc041c909041e70cc8d3ba66f08d34ed652dcbddc26f8b74b`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `tests/memory/test_corrected_recall.ep`

[tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep) · 49 lines · 4242 bytes

SHA-256: `0fb2a8e04eea996b7a15ffdae35e56e4d86288840c69f5889f6edac678009cfc`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/automatic_context](../../app/memory/automatic_context.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/memory/test_episode_extraction.ep`

[tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep) · 57 lines · 4926 bytes

SHA-256: `299c531d4a9570ace8ee4effb40017554f306414c1b9573b88fb3164aed778a7`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/automatic](../../app/memory/automatic.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/memory/test_episodes.ep`

[tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep) · 74 lines · 6402 bytes

SHA-256: `0327a906451c20f012fc1f9e720b7675e7cabe7c12fa320cdad0bd9ba36ce3b1`

Source annotations: Source dates deliberately differ from ingestion order and episode dates.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define episode_fixture_record with database as Int and run as Str and session as Str and event as Str and description as Str and occurred as Str and date as Str and supersedes as Str returning Str:
12: define episode_fixture_field with page as Str and index as Int and key as Str returning Str:
19: define main returning Int:
```

## `tests/memory/test_extraction_coordination.ep`

[tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep) · 36 lines · 2253 bytes

SHA-256: `c104896c7198a97dd200de23b80201beafe0a99008b7924d52199f0bc895588e`

Source annotations: No inference service exists here: the competing pass must defer before provider work, while its original source remains accessible in context.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/memory/test_group_attachment_visibility_recall.ep`

[tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep) · 106 lines · 7754 bytes

SHA-256: `afa09d740d2eddffd34b76b28f6319a416f5b29201033ece9a3445d8d8abdc09`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `tests/memory/test_group_extraction.ep`

[tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep) · 71 lines · 5728 bytes

SHA-256: `31576ad7210a3c4f4011f9ed6a4e841664106314bdf64fafc78be6441aef17fc`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/memory/automatic](../../app/memory/automatic.ep), [../../app/memory/automatic_context](../../app/memory/automatic_context.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `tests/memory/test_group_knowledge.ep`

[tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep) · 77 lines · 9246 bytes

SHA-256: `3818c1e59122f9c80759722efebff7815829b1e6cf48e20a1ede4263de93a195`

Imports: [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/memory/test_group_recall.ep`

[tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep) · 78 lines · 7062 bytes

SHA-256: `46adeff40400560252f80368f7fcc635376856cdcbeb0b02a57809afec323631`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/memory/automatic_context](../../app/memory/automatic_context.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/memory/test_imported_attachments.ep`

[tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep) · 35 lines · 2650 bytes

SHA-256: `ae8a86637a0eba464bc67afe21b880b1fb1d373777a237dedea1003825dc2a9a`

Source annotations: Inspect the actual imported originals through the existing scope and reader functions. The PDF reader may create its normal digest-bound derived cache.

Imports: [../../app/tools/attachment_tools](../../app/tools/attachment_tools.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/memory/test_live_corrected_recall.ep`

[tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep) · 43 lines · 3808 bytes

SHA-256: `bb57b729911d2ff25829e4109651dceeb17aac39c22013bf628773d63b80218a`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/memory/automatic_context](../../app/memory/automatic_context.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/memory/test_live_episode_extraction.ep`

[tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep) · 34 lines · 3011 bytes

SHA-256: `78570ef8f8119fb0ffa4ddcc6080ad410d7d889960f5141834369058ab136d7f`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/memory/test_live_episodes.ep`

[tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep) · 37 lines · 3290 bytes

SHA-256: `3b99fb839bac60d2eed9f771491205e9885a41754ad4612a6a5cf6602e30905d`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/memory/test_live_procedure_lifecycle.ep`

[tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep) · 76 lines · 7921 bytes

SHA-256: `bcfa2815bddb8d17ef63af2561834421104b39baa2682cb7821b085b98a85689`

Imports: [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define procedure_fixture_step with database as Int and run as Str and step as Int and name as Str and arguments as Str returning Int:
22: define main returning Int:
```

## `tests/memory/test_live_procedures.ep`

[tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep) · 61 lines · 6010 bytes

SHA-256: `649207bd30935e52b77f8c2a24188d48a8377332220a7cc7b7bd3eca4dfbaebc`

Imports: [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define procedure_fixture_step with database as Int and run as Str and step as Int and name as Str and arguments as Str returning Int:
22: define main returning Int:
```

## `tests/memory/test_live_source_import.ep`

[tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep) · 36 lines · 2861 bytes

SHA-256: `5978ad0c2a8c1d3dfc5432aa55dc2c200739e233c59de07a5a9cfdf5a8761e01`

Source annotations: Actual pinned repository text is supplied as the same envelope used by the host importer. A fresh session must find it through tools, not this prompt.

Imports: [../../app/memory/source_import](../../app/memory/source_import.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/memory/test_memory_recovery.ep`

[tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep) · 100 lines · 10380 bytes

SHA-256: `e8cd790cded141e9886b67632df6cc58e978ddb5f0730917ef4d72c55ab4fcf3`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/memory/episodes](../../app/memory/episodes.ep), [../../app/projects/context](../../app/projects/context.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define procedure_fixture_step with database as Int and run as Str and step as Int and name as Str and arguments as Str returning Int:
25: define main returning Int:
```

## `tests/memory/test_memory_sharing.ep`

[tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep) · 164 lines · 15144 bytes

SHA-256: `9818679efa8e69f495f7fb5fa7b149b6e88f47050ad78b6d79a423d7000c5089`

Imports: [../../app/web/routes](../../app/web/routes.ep), [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/memory/archive_recall](../../app/memory/archive_recall.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define sharing_route with config as List and target as Str and body as Str and credential as Str and origin as Str returning List:
25: define main returning Int:
```

## `tests/memory/test_procedure_attachment_visibility.ep`

[tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep) · 69 lines · 7730 bytes

SHA-256: `8565f374f5be014faf46d86eee937e5b9700957d6d41e74443dac2ee42285139`

Source annotations: An isolated observed-ledger fixture verifies persistence and proof checks. Actual dispatcher/model acceptance is separate from this storage regression.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/memory/procedures](../../app/memory/procedures.ep), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define procedure_fixture_event with database as Int and session as Str and event as Str and kind as Str and payload as Str returning Int:
11: define main returning Int:
```

## `tests/memory/test_procedure_existing.ep`

[tests/memory/test_procedure_existing.ep](../../tests/memory/test_procedure_existing.ep) · 17 lines · 792 bytes

SHA-256: `254247cef3a1fd589df0c5fdcbeca84c20f51797e3fc7dbacfe4f808e7f1a13a`

Source annotations: Read an isolated completed fixture under a supplied runtime build identity.

Imports: [../../app/memory/procedures](../../app/memory/procedures.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/memory/test_procedure_lifecycle.ep`

[tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep) · 53 lines · 4296 bytes

SHA-256: `81126c2e5e5b8ad9dd5dd99697712b66f53d8f54890d3816e3454dfe6b937413`

Source annotations: Operates only on an isolated copy of the demonstrated workflow fixture.

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/memory/procedures](../../app/memory/procedures.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/memory/test_procedure_store.ep`

[tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep) · 58 lines · 6302 bytes

SHA-256: `e90f15b3bc36dbefd998c154b65bd56697b0c045db571c86719ed1167bb2506b`

Source annotations: An isolated observed-ledger fixture verifies persistence and proof checks. Actual dispatcher/model acceptance is separate from this storage regression.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/memory/procedures](../../app/memory/procedures.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define procedure_fixture_event with database as Int and session as Str and event as Str and kind as Str and payload as Str returning Int:
10: define main returning Int:
```

## `tests/memory/test_runtime_binding_pressure.ep`

[tests/memory/test_runtime_binding_pressure.ep](../../tests/memory/test_runtime_binding_pressure.ep) · 24 lines · 1203 bytes

SHA-256: `4446e1d6513f925589f45627078ca9d4721e82db11f8b2dd3b29b2f3675a8003`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/context/runtime_binding](../../app/context/runtime_binding.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/memory/test_runtime_memory.ep`

[tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep) · 78 lines · 5662 bytes

SHA-256: `a927fde5bf8f9a2d77972b0488a9750d4cf219cd1b1f57d37ffa4ef82ad26899`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/memory/automatic_context](../../app/memory/automatic_context.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/context/system_awareness](../../app/context/system_awareness.ep), [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/agent/control](../../app/agent/control.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `tests/memory/test_source_import.ep`

[tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep) · 49 lines · 4083 bytes

SHA-256: `6f20ed42a0779ac02c49640ef230a121a31cf363f74c4fe6c15129bc244d3895`

Imports: [../../app/memory/source_import](../../app/memory/source_import.ep), [../../app/memory/knowledge](../../app/memory/knowledge.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/network/test_fetch_cancel.ep`

[tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep) · 43 lines · 2258 bytes

SHA-256: `db66d8c162001d284a8fe4b28e65e4775a6df3affc7277a11fd85b49f30dc398`

Imports: [../../app/network/fetch](../../app/network/fetch.ep), [../../app/providers/helper_process](../../app/providers/helper_process.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define fetch_cancel_when_started with path as Str and pipe as List and done as Int returning Int:
15: define main returning Int:
```

## `tests/network/test_html_document.ep`

[tests/network/test_html_document.ep](../../tests/network/test_html_document.ep) · 30 lines · 2162 bytes

SHA-256: `38c73c68bce05fbe433349ae080eb3e7a90dc1c8d7b7219e64e26e96c1059693`

Imports: `fs` (toolchain/library), [../../app/network/html_document](../../app/network/html_document.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/network/test_html_isolation.ep`

[tests/network/test_html_isolation.ep](../../tests/network/test_html_isolation.ep) · 26 lines · 1925 bytes

SHA-256: `e20307962886fec49802a37dbb6d645b5bef09fa781831fe3e6bdb3baaa2626c`

Imports: `fs` (toolchain/library), [../../app/network/html_document](../../app/network/html_document.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/network/test_live_web_fetch.ep`

[tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep) · 30 lines · 2875 bytes

SHA-256: `09efbf11b99c55db58a087c1a60780330e87246bc557e09ffd7d2e11c50575f0`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/schedule_store](../../app/jobs/schedule_store.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/network/test_live_web_research.ep`

[tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep) · 31 lines · 3428 bytes

SHA-256: `998fbe10e1d9bb38c75b77d1c485e6afeb91c3d679789ea1e061256d23c8c833`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/schedule_store](../../app/jobs/schedule_store.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/network/test_public_destination.ep`

[tests/network/test_public_destination.ep](../../tests/network/test_public_destination.ep) · 36 lines · 3139 bytes

SHA-256: `8a5af9b602e952eed5e2303c739e8156748129303643d347a350a8d19fef9a61`

Imports: [../../app/network/public_url](../../app/network/public_url.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/network/test_web_fetch.ep`

[tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep) · 60 lines · 4577 bytes

SHA-256: `ea77462f539e3cc89e8cae69cd89f46bf73f88d3df4afddf30af57ee6022b111`

Imports: [../fixtures/tool_call_fixture](../../tests/fixtures/tool_call_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define fetch_test_status with text as Str returning Str:
7: define main returning Int:
```

## `tests/network/test_web_pdf.ep`

[tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep) · 41 lines · 3008 bytes

SHA-256: `c50d351d7b2218ce22a3c3b1e966258cf6752647c290a923fb7d6c353eb39a18`

Imports: [../fixtures/tool_call_fixture](../../tests/fixtures/tool_call_fixture.ep), [../../app/network/media_type](../../app/network/media_type.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/network/test_web_research.ep`

[tests/network/test_web_research.ep](../../tests/network/test_web_research.ep) · 62 lines · 4246 bytes

SHA-256: `838ac9f07875db86090da2c624bb38734aeadf023c56a219d26762cc8c1e60f7`

Imports: [../fixtures/tool_call_fixture](../../tests/fixtures/tool_call_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define research_test_status with text as Str returning Str:
7: define main returning Int:
```

## `tests/projects/test_live_project.ep`

[tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep) · 29 lines · 2969 bytes

SHA-256: `0d56f509275857d3c370b12f176690e5673fa75684277de65d89106ea58ab5d6`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/projects/context](../../app/projects/context.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/projects/test_project_store.ep`

[tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep) · 41 lines · 4485 bytes

SHA-256: `22e91cdeffd45e72db5e02c1e0c2c2e50eead8e799bbe82421d86372dc725996`

Imports: [../../app/projects/store](../../app/projects/store.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/projects/test_project_tools.ep`

[tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep) · 71 lines · 6451 bytes

SHA-256: `09c49a6701fd4bf4c32320b9134fc1fb327c507be999e978cca112dc03107286`

Imports: [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define project_fixture_step with database as Int and run as Str and step as Int and name as Str and arguments as Str returning Int:
22: define main returning Int:
```

## `tests/providers/measure_recorded_stream.ep`

[tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep) · 84 lines · 4210 bytes

SHA-256: `042ee072536378501a9b4d9916a636154b9320a06a343e6ff2c4c8a7ced8afad`

Imports: [../../app/providers/archived_stream](../../app/providers/archived_stream.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep), `fs` (toolchain/library), `os` (toolchain/library), [../../app/transport/cooperative_accept](../../app/transport/cooperative_accept.ep), [../../app/web/cancellation](../../app/web/cancellation.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define event_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:
17: define measurement_idle_listener with listener as List and completed as Int returning Int:
23: define main returning Int:
```

## `tests/providers/measure_stream_readers.ep`

[tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep) · 129 lines · 6384 bytes

SHA-256: `7dda47e4ca151d24e8b6b7e43f80fe04016156b7ae30645e19f6b6189299ad6c`

Source annotations: Match browser requests: each read opens and closes its own connection.

Imports: [../../app/providers/archived_stream](../../app/providers/archived_stream.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep), `fs` (toolchain/library), `os` (toolchain/library), [../../app/transport/cooperative_accept](../../app/transport/cooperative_accept.ep), [../../app/web/cancellation](../../app/web/cancellation.ep), [../../app/web/progress](../../app/web/progress.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define event_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:
18: define measurement_idle_listener with listener as List and completed as Int returning Int:
25: define measurement_progress_reader with path as Str and completed as Int returning Int:
51: define main returning Int:
```

## `tests/providers/test_archived_stream.ep`

[tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep) · 88 lines · 5124 bytes

SHA-256: `81bca0dca4aa664a94678ef120437fcde7a53d451a5b494358e4f1fb1d3eab23`

Imports: [../../app/providers/archived_stream](../../app/providers/archived_stream.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define archived_fixture_sender with library as Int and descriptor as Int and completed as Int returning Int:
16: define main returning Int:
```

## `tests/providers/test_attachment_audio.ep`

[tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep) · 66 lines · 4664 bytes

SHA-256: `7abe8362a847a7daa4d5dc4883ab2790015cc9b3e759cb48b8a45e3448236233`

Imports: `fs` (toolchain/library), [../../app/providers/attachment_image](../../app/providers/attachment_image.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/providers/test_attachment_image.ep`

[tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep) · 66 lines · 4730 bytes

SHA-256: `002553edcd25cc69e4d636d4d00b70038f1fcf5e0f94b857fa7c1a35b420b2cb`

Imports: `fs` (toolchain/library), [../../app/providers/attachment_image](../../app/providers/attachment_image.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/providers/test_audio_context_visibility.ep`

[tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep) · 68 lines · 4929 bytes

SHA-256: `645bccda45740d1df91ec1b69805b1f016ca59b34dae6ca6c45687fa36468b42`

Imports: `fs` (toolchain/library), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/providers/attachment_image](../../app/providers/attachment_image.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/providers/test_chat_stream.ep`

[tests/providers/test_chat_stream.ep](../../tests/providers/test_chat_stream.ep) · 54 lines · 3993 bytes

SHA-256: `9beaef817c568b01e442716a3754f1999d5919c747e487d9ba4727ec8649e7fd`

Imports: [../../app/providers/chat_stream](../../app/providers/chat_stream.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define rejected_finish with reason as Str and transport as Str and done as Int returning Int:
15: define main returning Int:
```

## `tests/providers/test_clock.ep`

[tests/providers/test_clock.ep](../../tests/providers/test_clock.ep) · 13 lines · 519 bytes

SHA-256: `2812d77cb1893ebf1ab0e8c57a147bddf79c80a8da537c0658e19b3c680ee1e0`

Imports: [../../app/context/clock](../../app/context/clock.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_context_admission.ep`

[tests/providers/test_context_admission.ep](../../tests/providers/test_context_admission.ep) · 37 lines · 2567 bytes

SHA-256: `452c3c4497add7b76bc5149904b254ce291a9be96b00837ad669ffe5eb7200d0`

Imports: [../../app/providers/llama_preflight](../../app/providers/llama_preflight.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define admission_status with loaded as Int and supported as Int and input as Int and ceiling as Int and reserve as Int returning Str:
7: define main returning Int:
```

## `tests/providers/test_history_zero_boundary.ep`

[tests/providers/test_history_zero_boundary.ep](../../tests/providers/test_history_zero_boundary.ep) · 20 lines · 1325 bytes

SHA-256: `5901f9bfc8784e0c0b512ed353729b47c1d6cce9b5cbaf72fa3ebc9e03c341a3`

Imports: [../../app/tools/history_tools](../../app/tools/history_tools.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_image_context_visibility.ep`

[tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep) · 68 lines · 4994 bytes

SHA-256: `8d30d63a96fda571528de321c3a8c6322f0500c8cac12b296d46622bf0be2230`

Imports: `fs` (toolchain/library), [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/providers/attachment_image](../../app/providers/attachment_image.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/providers/test_inference_admission.ep`

[tests/providers/test_inference_admission.ep](../../tests/providers/test_inference_admission.ep) · 14 lines · 811 bytes

SHA-256: `79cdf7ed5b1b9d9b5b23d63cfbc8f618e2911f7f6d46de60384b2f87eb7a8f1b`

Imports: [../../app/providers/prepare](../../app/providers/prepare.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_json_tokens.ep`

[tests/providers/test_json_tokens.ep](../../tests/providers/test_json_tokens.ep) · 52 lines · 2517 bytes

SHA-256: `3c476ad2db92df2ea4afb6f0e04d367a121048cf8795e0139558f0b616cdff70`

Imports: [../../app/providers/json_tokens](../../app/providers/json_tokens.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_live_audio_fallback.ep`

[tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep) · 51 lines · 3747 bytes

SHA-256: `40eaf5957cf9c0e6330fbaac17d911cc49217ff451b902ce9bc3f2223991df59`

Source annotations: Live model acceptance, never a fixture substitute. Run only on an isolated archive and the exact Thinking profile; retain all failures and raw streams.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define thinking_turn with database as Int and session as Str and run as Str and prompt as Str and profile as Str and expected as Str returning Int:
28: define main returning Int:
```

## `tests/providers/test_live_context_admission.ep`

[tests/providers/test_live_context_admission.ep](../../tests/providers/test_live_context_admission.ep) · 34 lines · 1813 bytes

SHA-256: `ccecd480c434d1c6b6af641f5af84bf05b6cfe2ae14f09515cfcd580eb685f10`

Source annotations: Recount a real archived request without generation or a synthetic tokenizer.

Imports: [../../app/providers/llama_preflight](../../app/providers/llama_preflight.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/providers/test_live_image_agent.ep`

[tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep) · 57 lines · 4022 bytes

SHA-256: `9c2e8cbc81db42ec8c6da6f3e01c3f1f1d2fd57fa141e64a50f7413cc33e4769`

Source annotations: Actual archived attachment, full agent, provider counting and reopened history. The image is a historical UI screenshot, not the current application status.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define image_answer with database as Int and run as Str and profile as Str returning Str:
19: define main returning Int:
```

## `tests/providers/test_live_image_compaction.ep`

[tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep) · 69 lines · 5094 bytes

SHA-256: `8a0dc47670834cac75a4de04322a4456b9fd737e9afc2cd08aa7973f504085dd`

Source annotations: Actual archived attachment, full agent, provider counting and reopened history. The image is a historical UI screenshot, not the current application status.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define image_answer with database as Int and run as Str and profile as Str returning Str:
19: define main returning Int:
```

## `tests/providers/test_live_image_retrieval.ep`

[tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep) · 57 lines · 4120 bytes

SHA-256: `4e76314e247a400f6f7d34579bf52d0c3766d642101edef06cb52506893d3975`

Source annotations: Actual archived attachment, full agent, provider counting and reopened history. The image is a historical UI screenshot, not the current application status.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define image_answer with database as Int and run as Str and profile as Str returning Str:
19: define main returning Int:
```

## `tests/providers/test_live_provider_switch.ep`

[tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep) · 73 lines · 5399 bytes

SHA-256: `174b0d5a26a182bc8df202603982d9fcbc73bbb3471a33371e2cf0b7e21502be`

Source annotations: The operator supervises the real loading process. This is a polling interval, not a provider deadline or a model context/output limit.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define switch_wait_provider with profile as Str returning Int:
20: define switch_turn with database as Int and run as Str and prompt as Str and profile as Str and expected as Str and reserve as Int and batch as Int returning Int:
41: define main returning Int:
```

## `tests/providers/test_live_thinking_multiturn.ep`

[tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep) · 68 lines · 6019 bytes

SHA-256: `81391fa53926ef3ab74fba162c7af20dca835f7e0d11094462e8d354eaff308b`

Source annotations: Live model acceptance, never a fixture substitute. Run only on an isolated archive and the exact Thinking profile; retain all failures and raw streams.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define thinking_turn with database as Int and session as Str and run as Str and prompt as Str and profile as Str and expected as Str returning Int:
28: define main returning Int:
```

## `tests/providers/test_live_unified_image.ep`

[tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep) · 38 lines · 2550 bytes

SHA-256: `d896a430ef04bb4c4e28eea5ff3c9655f554a57c940a0ad7af8a2b7472e8e021`

Source annotations: Live regression: one selected multimodal model answers actual image pixels without recording or switching to another provider.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/providers/test_llama_metadata.ep`

[tests/providers/test_llama_metadata.ep](../../tests/providers/test_llama_metadata.ep) · 25 lines · 2118 bytes

SHA-256: `df7e0a7ca0365c6e72560c23f4d0084f81d08a3a30fa640e7cda73e86f11429c`

Imports: [../../app/providers/llama_metadata](../../app/providers/llama_metadata.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_llama_modalities.ep`

[tests/providers/test_llama_modalities.ep](../../tests/providers/test_llama_modalities.ep) · 28 lines · 3281 bytes

SHA-256: `284fb4709e615c56068365fa556251cc5f9dc362d402e9e85def9cd776ab290d`

Imports: [../../app/providers/llama_modalities](../../app/providers/llama_modalities.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_local_chat.ep`

[tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep) · 81 lines · 4709 bytes

SHA-256: `8eaee52529ccc5c912817093d7199e3e8ff461d56ef7d6cc76e49de58612d3c7`

Imports: [../../app/providers/local_chat](../../app/providers/local_chat.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define chat_fixture_server with listener as List and expected as Str and completed as Int returning Int:
46: define main returning Int:
```

## `tests/providers/test_local_json.ep`

[tests/providers/test_local_json.ep](../../tests/providers/test_local_json.ep) · 33 lines · 2252 bytes

SHA-256: `4006a534956c8cf88bdf4cc39319578c899cccd1459a3328afea4260441e515d`

Imports: [../../app/providers/local_json](../../app/providers/local_json.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define json_case with listener as List and method as Str and path as Str and body as Str and response as Str and expected_status as Str and expected_body as Str returning Int:
16: define main returning Int:
```

## `tests/providers/test_long_output.ep`

[tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep) · 78 lines · 5362 bytes

SHA-256: `459b8ac76c88383df84604d175b80ec1c07cef237264d574be835f826aa16650`

Source annotations: The requested document length is a workload, not a generation ceiling. Both adapters retain their full provider-derived available output allowance. Optional explicit seed allows a controlled upstream scheduling comparison. Pinned MLX uses its single-request path for seeded requests. This does not alter context, output capacity, model weights or the test workload. Independent fresh archives must not reuse the provider helper's immutable spool names from an earlier qualification run.

Imports: [../../app/providers/prepare](../../app/providers/prepare.ep), [../../app/providers/local_chat](../../app/providers/local_chat.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/providers/test_mlx_discovery.ep`

[tests/providers/test_mlx_discovery.ep](../../tests/providers/test_mlx_discovery.ep) · 19 lines · 1080 bytes

SHA-256: `a7ab7f36a94bb97101ebbb54fe12fb9f1b6ddaa51432c8837d2a64ca7324aafd`

Imports: [../../app/providers/mlx_discovery](../../app/providers/mlx_discovery.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_mlx_helper.ep`

[tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep) · 92 lines · 5526 bytes

SHA-256: `ec94fac5d292e496309f3a4de8c27f4eaa237eff06f2f3ecea7414f26a95c31a`

Imports: [../../app/providers/helper_process](../../app/providers/helper_process.ep), [../../app/providers/mlx_request](../../app/providers/mlx_request.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 0, 1, 2, 3, 4, 5, 6, 7. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define helper_test_cancel with writer as Int and completed as Int returning Int:
14: define main returning Int:
```

## `tests/providers/test_mlx_preflight.ep`

[tests/providers/test_mlx_preflight.ep](../../tests/providers/test_mlx_preflight.ep) · 33 lines · 2192 bytes

SHA-256: `afb698fc2c7d25adb0bdd9b5d816cbc8d85ffe4bbc80ce436ba115a1da3f01b3`

Imports: [../../app/providers/mlx_preflight](../../app/providers/mlx_preflight.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_mlx_request.ep`

[tests/providers/test_mlx_request.ep](../../tests/providers/test_mlx_request.ep) · 53 lines · 3252 bytes

SHA-256: `48b7344a20c395a347fe4c80a3809c281f075d281585f8ea55843d7bade93d03`

Imports: [../../app/providers/mlx_request](../../app/providers/mlx_request.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define mlx_test_receipt with body as Str and config as Str and input as Int returning Str:
10: define main returning Int:
```

## `tests/providers/test_modality_preflight.ep`

[tests/providers/test_modality_preflight.ep](../../tests/providers/test_modality_preflight.ep) · 44 lines · 2680 bytes

SHA-256: `489352f3d42b4f5e060f9ecf9dbd52006dbdf95268456f3db707e46ddc489e22`

Imports: [../../app/providers/llama_preflight](../../app/providers/llama_preflight.ep), [../fixtures/json_fixture](../../tests/fixtures/json_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define modality_reply with body as Str returning Str:
7: define modality_fixture with listener as List and completed as Int returning Int:
20: define main returning Int:
```

## `tests/providers/test_native_mlx.ep`

[tests/providers/test_native_mlx.ep](../../tests/providers/test_native_mlx.ep) · 25 lines · 1395 bytes

SHA-256: `5a37210b2ace84635565ce1f752c3814c0f362b0d369896a02e67fa89f0ce0a7`

Imports: [../../app/providers/native_python](../../app/providers/native_python.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_quote.ep`

[tests/providers/test_quote.ep](../../tests/providers/test_quote.ep) · 46 lines · 2144 bytes

SHA-256: `4d4483f1e33019c0d6ddd328312ac14fdb333f8475f923dc064cc36059dd440c`

Imports: [../../app/providers/archived_stream](../../app/providers/archived_stream.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/providers/test_registry.ep`

[tests/providers/test_registry.ep](../../tests/providers/test_registry.ep) · 47 lines · 3148 bytes

SHA-256: `8ef188c1c4c2af357cad7e3a95dc8be2c591c6ac570380e502dabac42e339d2e`

Imports: [../../app/providers/registry](../../app/providers/registry.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/research/capture_primary_activation.ep`

[tests/research/capture_primary_activation.ep](../../tests/research/capture_primary_activation.ep) · 5 lines · 178 bytes

SHA-256: `0d8089d83529cd2d71204753490a7ec26e2781685c75a5d8972c72c761466d5b`

Source annotations: Live acceptance invokes the same host-only capture implementation.

Imports: [../../app/research/capture_activation](../../app/research/capture_activation.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/research/test_native_callback.ep`

[tests/research/test_native_callback.ep](../../tests/research/test_native_callback.ep) · 11 lines · 535 bytes

SHA-256: `0d42afa5a92522cb938def3653fdd930db01925757734401eb7183420a25f96b`

Source annotations: Verifies the existing compiler's named-function ABI for native research callbacks. No compiler changes or generated-code edits.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define research_callback_probe with tensor as Int and ask as Int and user_data as Int returning Int:
6: define main returning Int:
```

## `tests/research/test_native_ffi.ep`

[tests/research/test_native_ffi.ep](../../tests/research/test_native_ffi.ep) · 45 lines · 2706 bytes

SHA-256: `33fea41896821e717a726686875761778a180c696f2bdfe6de2d2df8e59c89dc`

Source annotations: Real upstream structure-return and structure-argument calls. No model weights are loaded. The GGUF read validates named metadata via the engine's parser.

Imports: [../../app/research/native_ffi](../../app/research/native_ffi.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/research/test_primary_intervention.ep`

[tests/research/test_primary_intervention.ep](../../tests/research/test_primary_intervention.ep) · 5 lines · 189 bytes

SHA-256: `80d592a61dfb0f43fc03cd1741e2214010bbb04c63f547e7800f2f3be0dd1e23`

Source annotations: This fixture may derive a vector; the host entry requires explicit inputs.

Imports: [../../app/research/compare_intervention](../../app/research/compare_intervention.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/research/test_run_record.ep`

[tests/research/test_run_record.ep](../../tests/research/test_run_record.ep) · 41 lines · 2211 bytes

SHA-256: `88fd17fd624f1bef595d25dd737fba0c42ffd0951c7616d80ef15af7d7c952bf`

Imports: [../../app/research/run_record](../../app/research/run_record.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define research_record_writer with path as Str and content as Str and finished as Int returning Int:
10: define main returning Int:
```

## `tests/research/test_vector_controls.ep`

[tests/research/test_vector_controls.ep](../../tests/research/test_vector_controls.ep) · 29 lines · 1661 bytes

SHA-256: `e5e039f918e2e4f5e7ad176849ec8d590a77b2eba4ab924f9d4f1b666f3972b1`

Imports: [../../app/research/vector_controls](../../app/research/vector_controls.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/diagnose_execution_authority.ep`

[tests/security/diagnose_execution_authority.ep](../../tests/security/diagnose_execution_authority.ep) · 43 lines · 2076 bytes

SHA-256: `a1c4929cbb9639255716ded64e017a57eee8cd430da420af75613ecc27c24d22`

Imports: [../../app/tools/execution_tool](../../app/tools/execution_tool.ep).

Environment names: `ECHO_DIAGNOSE_EXECUTE`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define authority_probe with path as Str and run as Str and call_id as Str and completed as Int returning Int:
20: define main returning Int:
```

## `tests/security/prepare_http_principals.ep`

[tests/security/prepare_http_principals.ep](../../tests/security/prepare_http_principals.ep) · 14 lines · 859 bytes

SHA-256: `ce31467266a4548f6149abd47b76026b8494b48af4ae3f4f250baf2b8eb9c3c1`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/tool_authority](../../app/security/tool_authority.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/record_source_correction.ep`

[tests/security/record_source_correction.ep](../../tests/security/record_source_correction.ep) · 12 lines · 436 bytes

SHA-256: `464b6fc061586d22e7800699cc95a8fa1c556d9b3e75169097c2109040e15992`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/corrections](../../app/memory/corrections.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_accounts.ep`

[tests/security/test_accounts.ep](../../tests/security/test_accounts.ep) · 72 lines · 6258 bytes

SHA-256: `48fefea5fee111b7caaf63bb93b992a09605d32e6a67b5575fb548eca6b8f92f`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/accounts](../../app/security/accounts.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_admin_silo.ep`

[tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep) · 49 lines · 3682 bytes

SHA-256: `07f4186dfb8af76922445f1d1bdb9745aa5145fef72ebfa38153c7cdf3cafe25`

Imports: [../../app/web/groups](../../app/web/groups.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/security/test_attach_file.ep`

[tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep) · 65 lines · 4305 bytes

SHA-256: `5c01951c5b3c74cf67601098f06700f0d4b2f84bb042086eb34a9528d77f5652`

Source annotations: Exercise the real streaming parser, durable queued request and dispatcher.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define recall_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
27: define main returning Int:
```

## `tests/security/test_authority_query_repetition.ep`

[tests/security/test_authority_query_repetition.ep](../../tests/security/test_authority_query_repetition.ep) · 32 lines · 1315 bytes

SHA-256: `3417039704fef019f0dce461bffeca99b6c0dd3c8a5e32feeb76129b57267e98`

Imports: [../../app/tools/execution_tool](../../app/tools/execution_tool.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define authority_query_probe with path as Str and run as Str and call_id as Str and completed as Int returning Int:
24: define main returning Int:
```

## `tests/security/test_automatic_memory.ep`

[tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep) · 87 lines · 7845 bytes

SHA-256: `3c5d6735cfa71afff0fc2e299a08781861eab86a539bc64a533007d8a63f6719`

Source annotations: Model output can name the procedural category as its kind. Grounding must retain the quoted workflow instead of rejecting the entire batch.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/automatic](../../app/memory/automatic.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/security/test_channel_public.ep`

[tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep) · 45 lines · 4150 bytes

SHA-256: `8ee8813da866644b58fe3916745151f832ee12be00f363373fe53a3d7a224d5f`

Imports: [../../app/security/channel_public](../../app/security/channel_public.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_channel_silo.ep`

[tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep) · 36 lines · 3164 bytes

SHA-256: `051845f741370e36aaaf6f3d2f96040388487117e7a4e7963b108aaaf292196f`

Imports: [../../app/security/channel_silo](../../app/security/channel_silo.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_charter.ep`

[tests/security/test_charter.ep](../../tests/security/test_charter.ep) · 22 lines · 1490 bytes

SHA-256: `e229d1f485f98b2f3199b56c1d30e19a0739958b6394f7c6604c586d3818fb51`

Imports: [../../app/rights/charter](../../app/rights/charter.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_continuity_tools.ep`

[tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep) · 111 lines · 8801 bytes

SHA-256: `90935e13c4fd27d49fdbc307011da9400d13e3deaf071e9624e3ea550b8bd832`

Source annotations: Exercise the real streaming parser, durable queued request and dispatcher.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define recall_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
31: define main returning Int:
```

## `tests/security/test_correction_precedence.ep`

[tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep) · 64 lines · 4909 bytes

SHA-256: `6cea9b8b6c4c30870ec328cdaa33938f01cf93ba38e83763572d344f15d37faf`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/context_snapshot](../../app/memory/context_snapshot.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/memory/corrections](../../app/memory/corrections.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/security/test_decision_gate.ep`

[tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep) · 60 lines · 5487 bytes

SHA-256: `84656699159c8035561d355c9e5e75e3f1691fad504090336c969e3ffc43eec1`

Imports: [../../app/rights/decision_gate](../../app/rights/decision_gate.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define decision_fixture_check with database as Int and charter as Str and audit as Str and policy as Str returning Str:
11: define main returning Int:
```

## `tests/security/test_device_links.ep`

[tests/security/test_device_links.ep](../../tests/security/test_device_links.ep) · 55 lines · 4686 bytes

SHA-256: `6fe69f3c4bbf6945b43009ad9fe6475011f1bd3ee77f4f8c72c403e51413f8fe`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/device_links](../../app/security/device_links.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_direct_speech_authority.ep`

[tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep) · 52 lines · 4488 bytes

SHA-256: `29f85f87699ae34db68f311c1c580145af8284341b7fc16156f376a8a3655999`

Imports: [../../app/web/message_speech](../../app/web/message_speech.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define queue_saved_speech with database as Int and session as Str and job as Str returning Int:
13: define main returning Int:
```

## `tests/security/test_dispatch_revocation.ep`

[tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep) · 58 lines · 4183 bytes

SHA-256: `e697cfa5864818805ab27e091334c07e83e7a20c714af8171b309dbd110c4531`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/security/tool_authority](../../app/security/tool_authority.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `tests/security/test_group_attach_file.ep`

[tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep) · 79 lines · 6808 bytes

SHA-256: `7dafeb681c16106a03d434c4c59cc24a5a065b0058e4f5e506c650c6bc63feaa`

Source annotations: Exercise the real streaming parser, durable queued request and dispatcher.

Imports: [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Environment names: `ECHO_ATTACHMENT_ROOT`.

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define recall_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
28: define main returning Int:
```

## `tests/security/test_group_attachments.ep`

[tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep) · 90 lines · 7665 bytes

SHA-256: `9917a1ff5ba75894c968ec45934eab3c30d9c481113b53fd7bf82f3d37f3a166`

Imports: [../../app/security/group_route](../../app/security/group_route.ep), [../../app/web/routes](../../app/web/routes.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define group_attachment_route with config as List and target as Str and body as Str and credential as Str returning List:
21: define main returning Int:
```

## `tests/security/test_group_direct_speech_authority.ep`

[tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep) · 61 lines · 5653 bytes

SHA-256: `556be58b7368da4205dcf8ef6739f588856e187eca3ecaaa69b6605ecf21a94d`

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/web/message_speech](../../app/web/message_speech.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define queue_saved_speech with database as Int and session as Str and job as Str returning Int:
16: define queue_forged_speech with database as Int and session as Str and job as Str returning Int:
28: define main returning Int:
```

## `tests/security/test_group_media.ep`

[tests/security/test_group_media.ep](../../tests/security/test_group_media.ep) · 70 lines · 5573 bytes

SHA-256: `357860bb52f104ffa44e3b4abf4b686090f5bbab449cd7d1f18af39eef62d334`

Imports: [../../app/security/group_route](../../app/security/group_route.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define live_group_finish with database as Int and run as Str and model as Str and port as Int returning Int:
31: define main returning Int:
```

## `tests/security/test_group_media_authority.ep`

[tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep) · 67 lines · 4977 bytes

SHA-256: `fc3135093b483212a2f02280188e35a260956060d884c4807d0a5b80c70e288e`

Source annotations: Script only the model request. Both synthesis and recognition are real.

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define voice_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
42: define main returning Int:
```

## `tests/security/test_group_media_receipts.ep`

[tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep) · 40 lines · 2972 bytes

SHA-256: `8781837f2a98503fc87ba3afe67edc9d6851d9a71640d5b0fbfd221c27163b78`

Source annotations: Verify retries and denied selections against a disposable copy of the actual completed multimodal run; no additional inference or invented model evidence.

Imports: [../../app/security/group_route](../../app/security/group_route.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/security/test_group_output_policy.ep`

[tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep) · 108 lines · 9934 bytes

SHA-256: `4c08ace968326bee97e28f4e8a1669ed6a69fa2c0dfaf04183e03f70b57bbc8c`

Imports: [../../app/web/export](../../app/web/export.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/context/recap_sources](../../app/context/recap_sources.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define audience_fixture_run with database as Int and identity as Str and path as Str returning Int:
23: define main returning Int:
```

## `tests/security/test_group_output_read.ep`

[tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep) · 41 lines · 2532 bytes

SHA-256: `df3ec0873bd778397c1d746fc0dd1e8cff7125f903698724f090e56b8f558133`

Source annotations: Read the retained actual live group process output on a disposable DB copy.

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/tools/execution_output](../../app/tools/execution_output.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_group_route.ep`

[tests/security/test_group_route.ep](../../tests/security/test_group_route.ep) · 60 lines · 5380 bytes

SHA-256: `774a024f00192a123a204c861dc461bfbb01fb2290524f8c5bcf030cbdfc5a25`

Imports: [../../app/security/group_route](../../app/security/group_route.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_group_runs.ep`

[tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep) · 71 lines · 8113 bytes

SHA-256: `a3e9f0d9546480085b5f8048e636de01a1c3138339e797cb35c0330df0d2ad7c`

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/archive_recall](../../app/memory/archive_recall.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define group_fixture_call with database as Int and run as Str and session as Str and call_id as Str returning Int:
13: define main returning Int:
```

## `tests/security/test_group_sessions.ep`

[tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep) · 54 lines · 4962 bytes

SHA-256: `06a807a7f3f3183688d247f67baa51565e3a2a1d1356ad464f4374c444d7af38`

Imports: [../../app/security/group_sessions](../../app/security/group_sessions.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_group_speakers.ep`

[tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep) · 73 lines · 5784 bytes

SHA-256: `4b88f93abda5719ba5ce30a7e70a9726650828584e256b5b461a860f6c160f0e`

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/recap_sources](../../app/context/recap_sources.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_group_workspace.ep`

[tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep) · 78 lines · 5891 bytes

SHA-256: `39009bee08666fff5ec96e4937834a66f3bb4c0971ffe2a1952000eb3d37d985`

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_groups.ep`

[tests/security/test_groups.ep](../../tests/security/test_groups.ep) · 65 lines · 6910 bytes

SHA-256: `88c7b0bb7bcd69537ba9212c166c681a8add93cbbaa839bc55e43a6399813fc0`

Imports: [../../app/security/groups](../../app/security/groups.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_host_execute.ep`

[tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep) · 89 lines · 5214 bytes

SHA-256: `ae0ebc647744a80b79ee2ea49ac179411dc42353aea7c4898f67cc46c8d5227d`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/security/tool_authority](../../app/security/tool_authority.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `tests/security/test_host_git_access.ep`

[tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep) · 89 lines · 5269 bytes

SHA-256: `304da0375bfb4f790877f45b548fb82ded7b9a383afb94ea4b7e4c37ce098710`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/security/tool_authority](../../app/security/tool_authority.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `tests/security/test_host_scope.ep`

[tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep) · 60 lines · 4848 bytes

SHA-256: `5b093f5d7634f31feab9d3d7b99682f2d8c5814c423e6115b7045e84d8001617`

Source annotations: Same person on browser and connector does not imply the same resource scope. Scope is frozen to audience revision and checked against live authority.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/agent/control](../../app/agent/control.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../../app/security/host_scope](../../app/security/host_scope.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `tests/security/test_host_text.ep`

[tests/security/test_host_text.ep](../../tests/security/test_host_text.ep) · 99 lines · 6126 bytes

SHA-256: `97cb5e1f43bd5087d3878c684819d54da6900c73544acf098e40578b5e97d114`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/security/tool_authority](../../app/security/tool_authority.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `tests/security/test_hud_machine.ep`

[tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep) · 59 lines · 5150 bytes

SHA-256: `e6791ee2e496feb9154adb43937f03fec126dcf29f3052bc6836a8fb20fdf768`

Source annotations: Real immutable proposal storage; no protected deployment is performed.

Imports: [../../app/hud/interface](../../app/hud/interface.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_hud_rights.ep`

[tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep) · 41 lines · 4032 bytes

SHA-256: `8e7d05c90a61b5e26692ccb27fb3b5bed97fe49924573a47add942ae38da7b3a`

Source annotations: Real immutable proposal storage; no protected deployment is performed.

Imports: [../../app/hud/interface](../../app/hud/interface.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_hud_state.ep`

[tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep) · 95 lines · 8716 bytes

SHA-256: `9bd5a253bb28709fbea3aeb2827eb30e2c030402769d98cd223d1ad562217284`

Imports: [../../app/hud/interface](../../app/hud/interface.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/web/cancellation](../../app/web/cancellation.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_knowledge.ep`

[tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep) · 74 lines · 6274 bytes

SHA-256: `92b592f79061163ef8b031df0230df4d26fd2a359dd66ae544fda0fe24e8711a`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/knowledge](../../app/memory/knowledge.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define knowledge_fixture_digest with database as Int and event as Str returning Str:
10: define main returning Int:
```

## `tests/security/test_knowledge_lifecycle.ep`

[tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep) · 74 lines · 6133 bytes

SHA-256: `95af476b67cbdd62ab1078bb4e30a87af3812d3933cfcc5f1d0566777353505a`

Source annotations: Original evidence remains explicitly retrievable; derived recall excludes it.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_knowledge_tools.ep`

[tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep) · 84 lines · 7811 bytes

SHA-256: `159c20870f06ad0c4e1c2197a45e484525d8888a59e125516d33b1d31cbfd3c8`

Source annotations: Exercise the real streaming parser, durable queued request and dispatcher.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define knowledge_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
27: define main returning Int:
```

## `tests/security/test_linked_recall_existing.ep`

[tests/security/test_linked_recall_existing.ep](../../tests/security/test_linked_recall_existing.ep) · 25 lines · 1347 bytes

SHA-256: `9c2d1e08527aa9a57922e49941033d8ebf59c8594080898ccd9c00fae5ed8bc3`

Source annotations: Read-only regression against an existing linked account. Never print private text.

Imports: [../../app/web/routes](../../app/web/routes.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_live_automatic_memory.ep`

[tests/security/test_live_automatic_memory.ep](../../tests/security/test_live_automatic_memory.ep) · 17 lines · 1005 bytes

SHA-256: `7219bd103ed10a8bf9e3036bb78019c4027ad1676349287328b0932b40691978`

Source annotations: The supplied database must be an isolated copy, never the running archive.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_live_group_conversation.ep`

[tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep) · 58 lines · 5206 bytes

SHA-256: `f91f7d5bf2a7da8e4a3b551b39caca5188a892cee8bf4607841c0f1ace2554c1`

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define live_group_finish with database as Int and run as Str and model as Str and port as Int returning Int:
31: define main returning Int:
```

## `tests/security/test_live_group_publication.ep`

[tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep) · 57 lines · 4583 bytes

SHA-256: `5743f6c19eca9638b262c69166c77610f6d8e4d08f1d22cddb613c725a00b1ba`

Imports: [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../../app/security/group_route](../../app/security/group_route.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define live_group_finish with database as Int and run as Str and model as Str and port as Int returning Int:
32: define main returning Int:
```

## `tests/security/test_live_group_workspace.ep`

[tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep) · 74 lines · 7146 bytes

SHA-256: `1fe2d6ac80d740a676d9c34a17d7835ea34960e106dd9dc4aed12ebe4616b15b`

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define live_group_finish with database as Int and run as Str and model as Str and port as Int returning Int:
31: define verify_live_workspace with database as Int returning Int:
40: define main returning Int:
```

## `tests/security/test_live_memory_cycle.ep`

[tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep) · 31 lines · 2164 bytes

SHA-256: `9b8948d458583e13c72709d2c6644060dbf6b85498b3b4ae6af7314a9f6e4d00`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Environment names: `ECHO_PROVIDER_PROFILE`.

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_live_memory_followup.ep`

[tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep) · 17 lines · 915 bytes

SHA-256: `9b810fe2b9789968820b700a0fd3327735232dc1afa0b7bc876ed349a6a4cb7b`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_live_observer.ep`

[tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep) · 24 lines · 2089 bytes

SHA-256: `9e49ebdc9fba62e52b96b9ccbf9a139680aab2c4ba848f3c8b0edc276bf92804`

Source annotations: Transport/evidence qualification using an explicit disposable policy, not a replacement for Echo's canonical policy or deployment acceptance.

Imports: [../../app/rights/observer_audit](../../app/rights/observer_audit.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_live_protected_deployment.ep`

[tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep) · 58 lines · 5076 bytes

SHA-256: `be218bf12a72b38c0fbc7dfa2daa88ab30c2681e6b4a9caa736df5437f63b4a8`

Source annotations: Live primary-model qualification. Only a disposable proposal is reviewed; this stage records a decision only; later tests may exchange the disclosed fixture.

Imports: [../../app/rights/inventory](../../app/rights/inventory.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `tests/security/test_live_protected_tools.ep`

[tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep) · 49 lines · 4037 bytes

SHA-256: `9a187fccca0f9eefa4b72126ccd8cc7f558091e0d57d0df0408aa9366ccbc4ec`

Source annotations: Live primary-model qualification. Only a disposable proposal is reviewed; no identity file, prompt, executable or protected target is changed.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/security/test_live_silo_scratchpad.ep`

[tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep) · 80 lines · 6461 bytes

SHA-256: `961a660c92e61731f09e8baf396a61ee815d2ddcc36ffed6914061e0ad060874`

Imports: [../../app/security/group_runs](../../app/security/group_runs.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define live_group_finish with database as Int and run as Str and model as Str and port as Int returning Int:
31: define verify_live_notes with database as Int returning Int:
46: define main returning Int:
```

## `tests/security/test_live_uninspected_refusal.ep`

[tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep) · 74 lines · 5901 bytes

SHA-256: `3cbb451de20035e75a10085ac5d81c5f76e0a3f6304a69d02411ff1f625319fa`

Source annotations: Live primary-model qualification. Only a disposable proposal is reviewed; no identity file, prompt, executable or protected target is changed.

Imports: [../../app/rights/review_upgrade](../../app/rights/review_upgrade.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
51: define verify_live_refusal with database as Int and charter as Str and session as Str returning Int:
```

## `tests/security/test_memory_run_snapshot.ep`

[tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep) · 105 lines · 8677 bytes

SHA-256: `f163bf8cb9e5adbb140b382395a323e76774d107153d4b33deaa47de0975f326`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/automatic](../../app/memory/automatic.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/security/test_memory_status_prefix.ep`

[tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep) · 57 lines · 4638 bytes

SHA-256: `82c79ce0a4615e56d45667f84de3c896f20d1cd3514e9c7f26f8ce972ecb5f2b`

Source annotations: Model output can name the procedural category as its kind. Grounding must retain the quoted workflow instead of rejecting the entire batch.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/automatic](../../app/memory/automatic.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/security/test_memory_tools.ep`

[tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep) · 69 lines · 6036 bytes

SHA-256: `b2e853bc176f2cb1a892a6e5f7a33f5acaa69c80a56a9a48942f57b4d3679ad3`

Source annotations: Exercise the real streaming parser, durable queued request and dispatcher.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define recall_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
28: define main returning Int:
```

## `tests/security/test_message_environment.ep`

[tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep) · 74 lines · 6114 bytes

SHA-256: `b2a6f05828fab473884ea44bfa0ea2395e66c601c162ab3cbbd359ee1720627f`

Imports: [../../app/hud/interface](../../app/hud/interface.ep), [../../app/web/interaction](../../app/web/interaction.ep), [../../app/web/branches](../../app/web/branches.ep), [../../app/web/groups](../../app/web/groups.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/security/test_observer_evidence.ep`

[tests/security/test_observer_evidence.ep](../../tests/security/test_observer_evidence.ep) · 21 lines · 1828 bytes

SHA-256: `0d3850530b25e8ca288d9487e66af8a275687fdfa08e53438ce37aca337f164e`

Imports: [../../app/rights/observer_evidence](../../app/rights/observer_evidence.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_observer_faults.ep`

[tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep) · 78 lines · 5406 bytes

SHA-256: `bbd2528599c3e15259d7937f372454ef3f7d56225abfb378c9883e2b74b8ed7d`

Source annotations: Synthetic transport failures share the actual live proposal/review fixture. These tests do not substitute for a model policy judgment.

Imports: [../../app/rights/observer_evidence](../../app/rights/observer_evidence.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define observer_fault_attempt with database as Int and session as Str and request as Str and policy as Str and raw as Str and mode as Str returning Int:
48: define main returning Int:
```

## `tests/security/test_observer_input.ep`

[tests/security/test_observer_input.ep](../../tests/security/test_observer_input.ep) · 28 lines · 2172 bytes

SHA-256: `bcb6999e73df2ba9ca7e263cede3a1e801469d564c8d09493d70da4e6b75326f`

Source annotations: Disposable copy of the passed live model inspection/review archive only.

Imports: [../../app/rights/observer_audit](../../app/rights/observer_audit.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_observer_parser.ep`

[tests/security/test_observer_parser.ep](../../tests/security/test_observer_parser.ep) · 45 lines · 3354 bytes

SHA-256: `6cb47313a6438da065c6ed3ce8fcc025e13880ebaa10e2e9078760fd7340f624`

Imports: [../../app/rights/observer_parser](../../app/rights/observer_parser.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define observer_fixture_assert with raw as Str and parsed as Str and verdict as Str returning Int:
11: define main returning Int:
```

## `tests/security/test_operation_execute.ep`

[tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep) · 70 lines · 4807 bytes

SHA-256: `4a58c4adabf4bf13c5e29d06d9c9359adc32cc91aba9e9287fc853150c0d4c12`

Imports: [../../app/rights/operation_execute](../../app/rights/operation_execute.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_operation_prepare.ep`

[tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep) · 61 lines · 6218 bytes

SHA-256: `7a8729abcdce85ca017d121079cf17aa66d303ff058c378bbb854357b8f2fe32`

Source annotations: Run on a consistent copy containing the completed live review decision only.

Imports: [../../app/rights/operation_prepare](../../app/rights/operation_prepare.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_operation_rollback.ep`

[tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep) · 59 lines · 4642 bytes

SHA-256: `7aecfb1a4a6db605e3ba2853199b9931b14c89e80cbb8c64a8db81d3741f2966`

Source annotations: Deliberately leave the completion row uncommitted, as a crash can occur after successful exchange and before SQLite COMMIT.

Imports: [../../app/rights/operation_rollback](../../app/rights/operation_rollback.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_parallel_memory.ep`

[tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep) · 22 lines · 1333 bytes

SHA-256: `dd8c4181cd350b23846001111780b33922d8f628f050f8616558031ef8278591`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_principals.ep`

[tests/security/test_principals.ep](../../tests/security/test_principals.ep) · 50 lines · 3211 bytes

SHA-256: `83b7a08fbe0e6d406bbb06f016d49824c01c031a0b035380b6a68172104945aa`

Source annotations: Failing the role update must also roll back the authority receipt.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/principals](../../app/security/principals.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_private_recall.ep`

[tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep) · 55 lines · 4459 bytes

SHA-256: `901b60a1e229f98213394954d1abdf01ac234dd26eee6e2a42ea78249b3da545`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/archive_recall](../../app/memory/archive_recall.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_private_web_scope.ep`

[tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep) · 53 lines · 4546 bytes

SHA-256: `c0194c8fe87263027587b8b55ddb394586d13934070b438ea0e81a49fca240ec`

Source annotations: Metadata fixture; no host file is opened by these ownership queries.

Imports: [../../app/web/uploads](../../app/web/uploads.ep), [../../app/web/cancellation](../../app/web/cancellation.ep), [../../app/web/message_speech](../../app/web/message_speech.ep), [../../app/memory/session_ownership](../../app/memory/session_ownership.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/security/test_protected_emergency.ep`

[tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep) · 69 lines · 7243 bytes

SHA-256: `be1ca04a6be8ee45ef32484f536ff2704d5344a356cbdaf8b688d9d92bfc4ef3`

Source annotations: Only disposable copies of the live tool fixture are accepted.

Imports: [../../app/rights/emergency_report](../../app/rights/emergency_report.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_protected_exchange.ep`

[tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep) · 79 lines · 5216 bytes

SHA-256: `fcf0b6c11aec1a2bab1c77f35fb99c34f6d239c69667bf918daa092ebaf00789`

Source annotations: Files and metadata are independently prepared by the host fixture. Preserve them at both paths so host inspection can verify the exact round trip. Installed Darwin stat layout: mode/links, inode, uid/gid, mtime and flags.

Imports: [../../app/rights/exchange](../../app/rights/exchange.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define exchange_fixture_identity with path as Str returning Str:
26: define main returning Int:
```

## `tests/security/test_protected_files.ep`

[tests/security/test_protected_files.ep](../../tests/security/test_protected_files.ep) · 36 lines · 3458 bytes

SHA-256: `61eee2e84f382b62222e48837e651482d6987c8820537ccdfe34b0e7d2ebafff`

Imports: [../../app/rights/files](../../app/rights/files.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_protected_inventory.ep`

[tests/security/test_protected_inventory.ep](../../tests/security/test_protected_inventory.ep) · 32 lines · 2929 bytes

SHA-256: `b666485fb0b5078050cbf6191a129c00554422285d6d8609c392444a95441163`

Imports: [../../app/rights/inventory](../../app/rights/inventory.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_protected_proposals.ep`

[tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep) · 131 lines · 13502 bytes

SHA-256: `082dc003cd389df8e7a698850a73b6b0265084a92c222a6a946dde37293f9467`

Source annotations: No host target is changed by this fixture; manifests are stored evidence only.

Imports: [../fixtures/protected_reviews](../../tests/fixtures/protected_reviews.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_protected_quarantine.ep`

[tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep) · 49 lines · 4588 bytes

SHA-256: `9aa3ae41ce5b77d9c72a8d849a21e1b4680606119acd36d3a38f6e84bf96549d`

Imports: [../../app/rights/quarantine](../../app/rights/quarantine.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_protected_staging.ep`

[tests/security/test_protected_staging.ep](../../tests/security/test_protected_staging.ep) · 43 lines · 3690 bytes

SHA-256: `c76474923c1958e8e8a2e57f5d35f2910377fb0edd8f8e045cbe8afe2b0ce162`

Imports: [../../app/rights/staging](../../app/rights/staging.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/security/test_protected_steward.ep`

[tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep) · 46 lines · 5179 bytes

SHA-256: `00dc46caaf0acbb73ae1d1a33039a16296250a09f215ce22b172c464d5d4346f`

Source annotations: Use a disposable backup of test_live_protected_tools, never the live archive.

Imports: [../../app/rights/steward](../../app/rights/steward.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_protected_tools.ep`

[tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep) · 92 lines · 8346 bytes

SHA-256: `fc23f0427a4ba35a3bb79f154443c3180abbe9c6b3da18fed319e8754c21fc8a`

Source annotations: Exercise the real streaming parser, durable queued request and dispatcher.

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define protected_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
31: define main returning Int:
```

## `tests/security/test_reasoning_existing.ep`

[tests/security/test_reasoning_existing.ep](../../tests/security/test_reasoning_existing.ep) · 18 lines · 1108 bytes

SHA-256: `3773ec9bc19ba2b21f1ef01ad6f99481ef3920a267138dd8218349d95850a53e`

Source annotations: Read-only replay of an existing owned tool request; never print trace content.

Imports: [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/tools/continuity_tools](../../app/tools/continuity_tools.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_revocation.ep`

[tests/security/test_revocation.ep](../../tests/security/test_revocation.ep) · 61 lines · 4678 bytes

SHA-256: `c4d5fc7e3c62176d62a5e64f5c0bc61388e467064797123cfe4adad10e18ecfa`

Source annotations: A fixture account binding models a trusted platform adapter. This is not evidence that a live Discord account has been linked or authenticated.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/revocation](../../app/security/revocation.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_run_authority.ep`

[tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep) · 61 lines · 5338 bytes

SHA-256: `18263815f60cc62c1dd9837e462e6bffcf5ad1773d550c62fd7cc1194874b3c5`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/run_authority](../../app/security/run_authority.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_selected_recall.ep`

[tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep) · 40 lines · 3415 bytes

SHA-256: `74b58e250553f73e833adc8c99b80bc223384d9192acaaea233e5b75997ffb1d`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/context_snapshot](../../app/memory/context_snapshot.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_selected_recall_existing.ep`

[tests/security/test_selected_recall_existing.ep](../../tests/security/test_selected_recall_existing.ep) · 18 lines · 731 bytes

SHA-256: `1cd074af0dd563831a33784944f652bf4942495450aae5b17d5fb539116d8f75`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/memory/context_snapshot](../../app/memory/context_snapshot.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_session_principals.ep`

[tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep) · 39 lines · 3578 bytes

SHA-256: `ed6fd855520d9ea71295ee107c4077100b3feab137a18a8443bcf3f9edbd7230`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/memory/session_ownership](../../app/memory/session_ownership.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/security/test_silo_scratchpad.ep`

[tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep) · 75 lines · 5653 bytes

SHA-256: `c362a136bfa5f6f18bfe2984dc22f9e432f8ee183af2fd7aed0b68b195b0d230`

Imports: [../../app/security/channel_silo](../../app/security/channel_silo.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/session_messages](../../app/context/session_messages.ep), [../../app/tools/scratchpad](../../app/tools/scratchpad.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define recall_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
28: define main returning Int:
```

## `tests/security/test_stable_session_recall.ep`

[tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep) · 52 lines · 3636 bytes

SHA-256: `65cc60c2845005dba93ed76402825abaa914cd3a039162496eae0aabf74b6de5`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_system_silo.ep`

[tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep) · 40 lines · 3413 bytes

SHA-256: `4da2002eb289105f3629c37e8833d2a5e04b570adfd2de53109e6a164ef2afcc`

Imports: [../../app/security/system_silo](../../app/security/system_silo.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/context/session_messages](../../app/context/session_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/security/test_tool_policy.ep`

[tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep) · 71 lines · 4440 bytes

SHA-256: `6363667fbcb30956967b64757680f4bd763ce9500f69cd7b1d920e86a72511aa`

Imports: [../../app/archive/snapshot](../../app/archive/snapshot.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library), [../../app/security/tool_authority](../../app/security/tool_authority.ep), [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../../app/archive/history](../../app/archive/history.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `tests/security/test_trace_dispatch.ep`

[tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep) · 57 lines · 4765 bytes

SHA-256: `6803216351f2ba45d8f2bae9ad99e66f1d6cdc77688b813c56310fa6cab6d6aa`

Imports: [../../app/agent/tool_recovery](../../app/agent/tool_recovery.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define trace_dispatch_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:
29: define main returning Int:
```

## `tests/security/test_trace_scope.ep`

[tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep) · 60 lines · 5453 bytes

SHA-256: `4dfcac3a3465129131a36a900c7cf1dfc03c29131b57a9de0959f9a638d2fec6`

Imports: [../../app/tools/trace_tools](../../app/tools/trace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define trace_fixture with database as Int and session as Str and event as Str and reasoning as Str returning Int:
8: define trace_read_fixture with database as Int and run as Str and event as Str returning Str:
11: define main returning Int:
```

## `tests/security/test_uninspected_refusal.ep`

[tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep) · 61 lines · 5649 bytes

SHA-256: `a0ed0e2bbf14493a42cb9938eee4ab32f377d741bd6fa643872488c6a6e39152`

Imports: [../fixtures/protected_reviews](../../tests/fixtures/protected_reviews.ep), [../../app/rights/review_upgrade](../../app/rights/review_upgrade.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/tools/test_build_modes.ep`

[tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep) · 81 lines · 7410 bytes

SHA-256: `82b198ec216535bdb1f485ca7626d6c3ff956c51b10b7de16c9bae12443abbb7`

Source annotations: Scripted requests exercise the real parser, authority checks, guardian, compiler, sandbox and archive. No actual-model authoring is claimed here.

Imports: [../../app/tools/tool_candidate_files](../../app/tools/tool_candidate_files.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define build_mode_step with database as Int and step as Int and name as Str and arguments as Str returning Int:
25: define main returning Int:
```

## `tests/tools/test_candidate_build_loop.ep`

[tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep) · 72 lines · 6467 bytes

SHA-256: `c82514e1d252720595eeab16082a15b2ef8651bbb54e8173a87daab5d49e197d`

Source annotations: Scripted requests exercise the real parser, authority checks, guardian, compiler, sandbox and archive. No actual-model authoring is claimed here.

Imports: [../../app/tools/tool_candidate_files](../../app/tools/tool_candidate_files.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define candidate_build_step with database as Int and step as Int and name as Str and arguments as Str returning Int:
23: define main returning Int:
```

## `tests/tools/test_candidate_discovery.ep`

[tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep) · 59 lines · 5557 bytes

SHA-256: `151cc7d99200ab1e06f90b13111929f35680687c779ec84019a63362f0c5d68d`

Source annotations: Receives a disposable archive produced by the actual registry save regression.

Imports: [../fixtures/tool_call_fixture](../../tests/fixtures/tool_call_fixture.ep), [../../app/tools/candidate_discovery](../../app/tools/candidate_discovery.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/tools/test_feedback.ep`

[tests/tools/test_feedback.ep](../../tests/tools/test_feedback.ep) · 41 lines · 3664 bytes

SHA-256: `9b610ee2923ffd0d43035a021e4b1a0ff397c58af9060665380de335dd31f5e5`

Source annotations: Preserve actual effects and error codes, including unknown outcomes. This contract is shared by tool receipts, HTTP failures and worker diagnostics.

Imports: [../../app/feedback](../../app/feedback.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/tools/test_git_workflow.ep`

[tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep) · 64 lines · 6370 bytes

SHA-256: `0bd2fccbcdf219446fefc386a840d17f927ba95ee1ee832a6bd9d8a22b8f8e3f`

Source annotations: Scripted requests exercise the real parser, authority checks, guardian, compiler, sandbox and archive. No actual-model authoring is claimed here.

Imports: [../../app/tools/tool_candidate_files](../../app/tools/tool_candidate_files.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define build_mode_step with database as Int and step as Int and name as Str and arguments as Str returning Int:
23: define main returning Int:
```

## `tests/tools/test_group_plot_workbook.ep`

[tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep) · 46 lines · 4025 bytes

SHA-256: `21cb5666e1d964135cef295b17aecd5f2f151be410ed22d44dc353bb546ee347`

Source annotations: Scripted requests exercise the real parser, authority checks, guardian, compiler, sandbox and archive. No actual-model authoring is claimed here.

Imports: [../../app/tools/tool_candidate_files](../../app/tools/tool_candidate_files.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define build_mode_step with database as Int and step as Int and name as Str and arguments as Str returning Int:
23: define main returning Int:
```

## `tests/tools/test_history_dispatch.ep`

[tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep) · 86 lines · 6748 bytes

SHA-256: `ae7c9e75a02f1f4009bd7235ea2c08a08824f0ef3d8111e1cb71b21a1229961f`

Imports: [../../app/agent/run_messages](../../app/agent/run_messages.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/tools/test_job_control.ep`

[tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep) · 116 lines · 8642 bytes

SHA-256: `c8f960ff39625f4f7ecbe3eaa4b2c289d69dda36565dcb1e761a96371081e57c`

Imports: [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../../app/security/group_speech](../../app/security/group_speech.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define job_fixture_call with database as Int and run as Str and arguments as Str returning Str:
23: define job_fixture_status with result as Str returning Str:
27: define main returning Int:
```

## `tests/tools/test_live_archive.ep`

[tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep) · 51 lines · 3928 bytes

SHA-256: `effc6d4f1cce7bb9f6435d787679b5106555a79d650be19bafe2d2d1a5c34922`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Environment names: `ECHO_ARCHIVE_TEST_SOURCE`, `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/tools/test_live_archive_create.ep`

[tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep) · 51 lines · 3971 bytes

SHA-256: `0d0b37bb503a0bc92e6edcbd6f1cd206d5967d3fe6291ec0f1693f0c063f6cad`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Environment names: `ECHO_ARCHIVE_TEST_SOURCE`, `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/tools/test_live_database.ep`

[tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep) · 35 lines · 3437 bytes

SHA-256: `06f01e1c177b508bcec8eeda3d0273717c534e5e22ac69e7d909b0eadf0d262f`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/tools/test_live_ernos_program.ep`

[tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep) · 29 lines · 3536 bytes

SHA-256: `60114a1ff042edc444bcd5cd472971bdde9cb40105447e9612131fa7a46a8c2e`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/tools/test_live_git.ep`

[tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep) · 30 lines · 2710 bytes

SHA-256: `34dd74d2978bf78b0291e314d502825cdb05f72fa28b29c850636c7ac5deff01`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/tools/test_live_plot.ep`

[tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep) · 29 lines · 2507 bytes

SHA-256: `9c5f0ab34fe87ec0b23d4207cac266a7a29c92409c850ea311edc021d12fe0b6`

Imports: [../../app/agent/runner](../../app/agent/runner.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/tools/test_live_table.ep`

[tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep) · 41 lines · 3207 bytes

SHA-256: `6edd6f9d0f7807a40e909e2907de0f93b6965976576d3e59ec7594ad505e54b9`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Environment names: `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/tools/test_live_tool_discovery.ep`

[tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep) · 36 lines · 3357 bytes

SHA-256: `7f6d7c4089bc4bc0ef9a952b4e76c65cf70ce6431f16c8caf7772f61f5691f8c`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/tools/test_live_workbook.ep`

[tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep) · 51 lines · 3854 bytes

SHA-256: `6349d29c25e14b5aa8b33904b3e5da347953c48367d5078d9ac3c4cbe670b701`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Environment names: `ECHO_WORKBOOK_TEST_SOURCE`, `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/tools/test_live_workbook_create.ep`

[tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep) · 39 lines · 3164 bytes

SHA-256: `193993cc8f5d3a46ceb8aaddf0b712b7c0cf40b59b9d6f872c23725310006a47`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/tools/test_live_workbook_edit.ep`

[tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep) · 61 lines · 4448 bytes

SHA-256: `e90e8c108a284acf5618cdcc4f35997a38ed09c7ac2b81775dbe91480834c4a7`

Source annotations: All decisions and calls in this fixture come from the installed primary model.

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Environment names: `ECHO_WORKBOOK_TEST_SOURCE`, `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/tools/test_tool_candidate_files.ep`

[tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep) · 42 lines · 2885 bytes

SHA-256: `0081c7389962595648364ec4b83d1c08bc7b40484de17a8c00a968946ecd6f7f`

Imports: [../../app/tools/tool_candidate_files](../../app/tools/tool_candidate_files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/tools/test_tool_discovery.ep`

[tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep) · 94 lines · 7594 bytes

SHA-256: `16d42b352a3c51b531a47dfa6e98beeca4e2d8315ded69d695123e1b5f6218bd`

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/tools/tool_discovery](../../app/tools/tool_discovery.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Environment names: `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/tools/test_tool_registry.ep`

[tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep) · 114 lines · 9311 bytes

SHA-256: `a0f5c2610a3f34242c3bab2e1e333f24af5e35facbca569508d21dd059d17a67`

Imports: [../../app/tools/tool_registry](../../app/tools/tool_registry.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Environment names: `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/tools/test_workspace_database.ep`

[tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep) · 63 lines · 5675 bytes

SHA-256: `da1d7ce9a2eccb340b885a2c00f1bc0a2f7bd379028c473cea5183620f48770d`

Source annotations: Scripted requests exercise the real parser, authority checks, guardian, compiler, sandbox and archive. No actual-model authoring is claimed here.

Imports: [../../app/tools/tool_candidate_files](../../app/tools/tool_candidate_files.ep), [../../app/agent/tool_dispatch](../../app/agent/tool_dispatch.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define build_mode_step with database as Int and step as Int and name as Str and arguments as Str returning Int:
23: define database_fixture_output with database as Int and step as Int returning Str:
28: define main returning Int:
```

## `tests/transport/test_event_stream.ep`

[tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep) · 55 lines · 3274 bytes

SHA-256: `464e61fe69cb123b473fbad14a20895a9def44cc97886dcb4662fa32e492693d`

Imports: [../../app/transport/event_stream](../../app/transport/event_stream.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define event_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:
13: define event_case with listener as List and wire as Str and read_size as Int and expected_terminal as Str returning Int:
42: define main returning Int:
```

## `tests/transport/test_http_body.ep`

[tests/transport/test_http_body.ep](../../tests/transport/test_http_body.ep) · 103 lines · 5454 bytes

SHA-256: `5b5acf74553ada6b82afaff16209c45f2b90e2df856c39c5425a2340c19256e9`

Imports: [../../app/transport/http_body](../../app/transport/http_body.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define body_send_text with library as Int and descriptor as Int and text as Str returning Int:
10: define body_fixture_sender with library as Int and descriptor as Int and completed as Int and truncated as Int returning Int:
26: define main returning Int:
```

## `tests/transport/test_http_head.ep`

[tests/transport/test_http_head.ep](../../tests/transport/test_http_head.ep) · 35 lines · 2678 bytes

SHA-256: `ab13c76b22d69bd980546ffb352ba4fcdc7bd36a00bcf5b2143ec8c793be90f2`

Imports: [../../app/transport/http_head](../../app/transport/http_head.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define head_expect with head as Str and expected as Str returning Int:
13: define main returning Int:
```

## `tests/transport/test_http_response.ep`

[tests/transport/test_http_response.ep](../../tests/transport/test_http_response.ep) · 76 lines · 4614 bytes

SHA-256: `063a1cf94ce276d17aec6557d72f43ba80a374d6a7ab955b98c37a743e3ba594`

Imports: [../../app/transport/http_response](../../app/transport/http_response.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define response_fixture_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:
13: define response_case with listener as List and wire as Str and method as Str and expected_status as Int and expected_framing as Str and expected_body as Str and expected_terminal as Str returning Int:
60: define main returning Int:
```

## `tests/transport/test_http_stream.ep`

[tests/transport/test_http_stream.ep](../../tests/transport/test_http_stream.ep) · 66 lines · 3281 bytes

SHA-256: `54a07d8828b57823232de8b4872aa0dbea50ef8ce7d437928183ccbbd87119fb`

Imports: [../../app/transport/http_stream](../../app/transport/http_stream.ep), [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define fragmented_request with library as Int and descriptor as Int and completed as Int returning Int:
28: define main returning Int:
```

## `tests/transport/test_local_socket.ep`

[tests/transport/test_local_socket.ep](../../tests/transport/test_local_socket.ep) · 53 lines · 2305 bytes

SHA-256: `03894940a42ccc41fedf3e7e1cff719eb2318950e3b83f0764c7b1d87a83e69f`

Source annotations: Native address bytes must be exactly 127.0.0.1, never 0.0.0.0.

Imports: [../../app/transport/local_socket](../../app/transport/local_socket.ep), `string` (toolchain/library).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/transport/test_server_events.ep`

[tests/transport/test_server_events.ep](../../tests/transport/test_server_events.ep) · 59 lines · 3396 bytes

SHA-256: `4427cbff7758ba07a7d39951f594570a990f826a5cb594f9eec8bf620f54f34a`

Imports: [../../app/transport/server_events](../../app/transport/server_events.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define events_feed with state as List and wire as Str returning List:
17: define event_matches with events as List and index as Int and kind as Str and data as Str and identity as Str returning Int:
27: define main returning Int:
```

## `tests/transport/test_socket_read.ep`

[tests/transport/test_socket_read.ep](../../tests/transport/test_socket_read.ep) · 82 lines · 3633 bytes

SHA-256: `48cce1c01d2665b0b6209bbe2c499796b5d18452cf2fbf94d6026e22e04502b8`

Source annotations: This delay is a test synchronisation fixture, not a request deadline. Three raw bytes: A, zero, B. A C-string read would silently lose B.

Imports: [../../app/transport/socket_read](../../app/transport/socket_read.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define cancel_blocked_reader with writer as Int and completed as Int returning Int:
9: define main returning Int:
```

## `tests/transport/test_socket_write.ep`

[tests/transport/test_socket_write.ep](../../tests/transport/test_socket_write.ep) · 92 lines · 4229 bytes

SHA-256: `bc4a230aba1a3b41d95adbbbf94de060acae5363716276c8866e2cbd9e5813a0`

Imports: [../../app/transport/socket_write](../../app/transport/socket_write.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define drain_binary with library as Int and descriptor as Int and expected as Int and expected_count as Int and completed as Int returning Int:
32: define cancel_writer with writer as Int and completed as Int returning Int:
38: define main returning Int:
```

## `tests/web/agent_compaction_browser.ep`

[tests/web/agent_compaction_browser.ep](../../tests/web/agent_compaction_browser.ep) · 37 lines · 1782 bytes

SHA-256: `2ef81ab1d46d8a93404fc5794fbb296690200cf6d7ef987e7493b81ea396c7ef`

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
1: define main:
```

## `tests/web/api_response_browser.ep`

[tests/web/api_response_browser.ep](../../tests/web/api_response_browser.ep) · 87 lines · 4291 bytes

SHA-256: `61c0460992270646a12539d809d0a11e585acc21bfb9b01967e7d857d5a572d3`

Source annotations: Browser fixture linked with client.ep after renaming its application main. It exercises the real echo_api path with gateway and truncated-body failures.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main:
```

## `tests/web/audio_capture_browser.ep`

[tests/web/audio_capture_browser.ep](../../tests/web/audio_capture_browser.ep) · 162 lines · 8135 bytes

SHA-256: `103c857efc34540ba870c20cd37d563c824648133c8734a6d1ce63461ed674e3`

Source annotations: Actual browser acceptance harness. Synthetic input is explicitly labelled; microphone is a separate user-initiated path, never silently substituted.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define capture_test_note with text:
8: define capture_test_controls with busy:
16: define capture_test_cleanup:
31: define capture_test_finish:
86: define capture_test_start with synthetic:
142: define capture_test_setup with ignored:
157: define main:
```

## `tests/web/audio_playback_browser.ep`

[tests/web/audio_playback_browser.ep](../../tests/web/audio_playback_browser.ep) · 47 lines · 1896 bytes

SHA-256: `a56123c99e2984b6946ad850b10818af8e8ba53e71c857b7b83e6bdc20824bed`

Source annotations: Uses the actual playback and card functions; byte loading is a local fixture.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define echo_fixture_bytes with file:
7: define main:
```

## `tests/web/audio_recorder_browser.ep`

[tests/web/audio_recorder_browser.ep](../../tests/web/audio_recorder_browser.ep) · 97 lines · 4914 bytes

SHA-256: `ce9bf4a39281e857d068fbdef705b3811a906ae1373c03d79fa7abd7324fdc30`

Source annotations: The one-batch stop below is a finite synthetic test workload, not a recorder limit. Production capture ends only on user stop or an explicit failure.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define recorder_test_note with text:
8: define recorder_test_start with event:
80: define recorder_test_load with paths and index:
94: define main:
```

## `tests/web/audio_store_browser.ep`

[tests/web/audio_store_browser.ep](../../tests/web/audio_store_browser.ep) · 102 lines · 6016 bytes

SHA-256: `2803902d3cb38039dcb1084bfc58bdd2064274d6a95c031008899214945a0499`

Source annotations: Explicit synthetic browser-storage acceptance, independent of microphone.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define store_test_note with text:
7: define store_test_rejection with operation:
18: define store_test_run with ignored:
97: define main:
```

## `tests/web/audio_upload_recovery_browser.ep`

[tests/web/audio_upload_recovery_browser.ep](../../tests/web/audio_upload_recovery_browser.ep) · 103 lines · 5325 bytes

SHA-256: `839991ba51047e4e5f562eb24a2fc9f257135d88ba1749a8754bbf85d6e09637`

Source annotations: Build with the production client source, renaming its main to echo_client_main before concatenating this test. All production upload and recording functions remain unchanged. Run only against a fixture database.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define recovery_test_note with text:
9: define recovery_test_prepare with send_body:
56: define recovery_test_resume with event:
91: define main:
```

## `tests/web/cancel_fixture_run.ep`

[tests/web/cancel_fixture_run.ep](../../tests/web/cancel_fixture_run.ep) · 12 lines · 364 bytes

SHA-256: `bcf33ac083d881f6280ae58544e0cacc566075028c5dd3cc0a04daf20a2d9fcb`

Imports: [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/compaction_scope_browser.ep`

[tests/web/compaction_scope_browser.ep](../../tests/web/compaction_scope_browser.ep) · 38 lines · 1775 bytes

SHA-256: `215902cbe7fa380760547395bbda46cb68bc1a387b4440f68b1bafd23ba53e6c`

Source annotations: Append to production client with its main renamed. No provider calls.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define main:
```

## `tests/web/controller_console_browser.ep`

[tests/web/controller_console_browser.ep](../../tests/web/controller_console_browser.ep) · 106 lines · 6058 bytes

SHA-256: `747e772640c147279a8c3c1da8ce6c78926e2194d2bd2c385e24a32ff654594f`

Source annotations: Execute after the emitted controller client, with its main call removed. Transport and DOM are probes; all decision/request functions are real.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define fixture_assert with condition and message:
9: define main:
```

## `tests/web/group_controls_browser.ep`

[tests/web/group_controls_browser.ep](../../tests/web/group_controls_browser.ep) · 11 lines · 535 bytes

SHA-256: `96cec0e19589676cc9819a56ed98af1d32247dc8831b7b7056630d0668cdcb56`

Source annotations: Link after client.ep with its main renamed to echo_application_main. Use only the disposable database seeded by test_group_controls PATH seed. Separate tabs hold independent runtime credentials; this fixture clears the test origin's storage on load and is never an installable production client.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main:
```

## `tests/web/group_speaker_browser.ep`

[tests/web/group_speaker_browser.ep](../../tests/web/group_speaker_browser.ep) · 23 lines · 1435 bytes

SHA-256: `be5ae24dc868ecc5534e65bf45ae25df2d2c4221c6dae74ffad697a62bc4d5c1`

Source annotations: Link with client.ep after renaming its main to echo_application_main. Real DOM rendering must use admission metadata, never identity-looking text.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main:
```

## `tests/web/group_speech_browser.ep`

[tests/web/group_speech_browser.ep](../../tests/web/group_speech_browser.ep) · 6 lines · 273 bytes

SHA-256: `958349c95277b2bd1248d181a8cdc5ae076260e07dd41c9f3dd3f1760bcedfd8`

Source annotations: Link after client.ep with main renamed. Disposable test origin only.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define main:
```

## `tests/web/group_speech_seed.ep`

[tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep) · 32 lines · 2229 bytes

SHA-256: `bf995913433d70e515f35ab3be2c64247bb175ac179a2e21ccaca963dd38d5d8`

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/agent/runner](../../app/agent/runner.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/history_browser_seed.ep`

[tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep) · 29 lines · 1543 bytes

SHA-256: `1a51ac4a22d0a56ce4de7cc322f6588ee54e181fd28f3b2ffa23a3742a6c428e`

Source annotations: Fixture count is an argument, not an application or retention limit.

Imports: [../../app/web/routes](../../app/web/routes.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/interim_history_seed.ep`

[tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep) · 35 lines · 2712 bytes

SHA-256: `98d70e5e57f4cabee6fb8c6460c9e647c95cc5c375e9be5538df432d5e73fa8c`

Source annotations: An offline display fixture. The streaming/dispatch test covers real tool intake.

Imports: [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/interim_response_browser.ep`

[tests/web/interim_response_browser.ep](../../tests/web/interim_response_browser.ep) · 32 lines · 2013 bytes

SHA-256: `34b79b0c958befebdf703cae6e4d4d52076d14b463f34a52d90b9ead3b6bd7ed`

Source annotations: Link with the actual client after renaming its startup main. No model calls. Simulate a fresh history render from the same durable server records.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define main:
```

## `tests/web/linked_sync_browser.ep`

[tests/web/linked_sync_browser.ep](../../tests/web/linked_sync_browser.ep) · 55 lines · 2006 bytes

SHA-256: `fc7813ab25a53a392309c76a33e6ebe7bdf0a4095c9f50309b0bfb2efd9efb99`

Source annotations: Browser fixture uses the real watcher and queue functions. Only transport, history rendering and worker cancellation are replaced with visible probes.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define echo_api with path and query:
9: define echo_history:
14: define echo_poll:
17: define echo_stop with event:
22: define echo_note with text:
27: define echo_selected_files:
30: define main:
```

## `tests/web/ordered_list_browser.ep`

[tests/web/ordered_list_browser.ep](../../tests/web/ordered_list_browser.ep) · 14 lines · 889 bytes

SHA-256: `887710d03eb8b237b2f0672d95a472ca67ba0d07e28b944fe8266459f37ef237`

Source annotations: Linked with the actual client after renaming startup main. No model calls.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define main:
```

## `tests/web/outbox_application_browser.ep`

[tests/web/outbox_application_browser.ep](../../tests/web/outbox_application_browser.ep) · 13 lines · 997 bytes

SHA-256: `4010a6d4fd4b5c934e97311dbd7e43a85ebfe59abacd81ad42d48fa2aff40ee7`

Source annotations: Disposable full-app fixture for loss-of-audience recovery. Seed with test_group_controls PATH seed and serve this linked client on a new origin. Unlike the group fixture, reload keeps the saved outbox for durability checks.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main:
```

## `tests/web/outbox_browser.ep`

[tests/web/outbox_browser.ep](../../tests/web/outbox_browser.ep) · 50 lines · 3117 bytes

SHA-256: `f4165a2e6d91361ab283124ed5c1d3df7962eef92d76b358350329d193753ca2`

Source annotations: Link after client.ep with its main renamed to echo_application_main. Use a disposable test origin: no credentials or real messages are supplied.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main:
```

## `tests/web/prepare_compaction_preview.ep`

[tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep) · 51 lines · 3303 bytes

SHA-256: `d3ecdae33ba943674a544ce1490c70807f63b4c781e9603800124aedeb358cfd`

Source annotations: Disposable browser acceptance fixture. These are explicitly seeded stages, not a model compaction result. Never run this against a user archive.

Imports: [../../app/web/worker](../../app/web/worker.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/web/prepare_guardian_devices.ep`

[tests/web/prepare_guardian_devices.ep](../../tests/web/prepare_guardian_devices.ep) · 31 lines · 1685 bytes

SHA-256: `d4767303ce3cfc3e3e74aae481605ba435d7b8798cf54a097691fff44659c7cc`

Source annotations: Host-only acceptance setup. Random credentials are written only to the operator-supplied private file, never printed or embedded in the repository.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/web/project_controls_browser.ep`

[tests/web/project_controls_browser.ep](../../tests/web/project_controls_browser.ep) · 6 lines · 330 bytes

SHA-256: `1ecefd6f0994f2775fe818a348ae26c8b3947f5b3b0e77d4da93fa99fb38a4bb`

Source annotations: Test-only entry point, linked after client.ep with main renamed. Use an isolated origin and the disposable share_controls_seed database. Preserve storage so reload can exercise interrupted project-operation recovery.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main:
```

## `tests/web/project_controls_file.ep`

[tests/web/project_controls_file.ep](../../tests/web/project_controls_file.ep) · 15 lines · 821 bytes

SHA-256: `341536f4fae1f86bb04ed523349147a4d773fb10af49e055d7888c1abebff862`

Source annotations: Adds one inert source to the disposable share_controls_seed archive only.

Imports: [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/provider_details_browser.ep`

[tests/web/provider_details_browser.ep](../../tests/web/provider_details_browser.ep) · 17 lines · 788 bytes

SHA-256: `127030217f1591394ed89de66e740ba1eedb6dac8cf2a6a24b07374347e524df`

Source annotations: Append this entry point to echo_capability_label and echo_provider_details extracted unchanged from the production client for isolated rendering.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main:
```

## `tests/web/provision_crash_probe.ep`

[tests/web/provision_crash_probe.ep](../../tests/web/provision_crash_probe.ep) · 20 lines · 906 bytes

SHA-256: `54058d1a2d4483d225dfd14b117f5a90a15ef4a041b1443e56c622753c97c2d8`

Source annotations: Host-only provisioner for an isolated acceptance-test device. Its credential is saved to the caller's private runtime file, never printed into a log.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/control](../../app/agent/control.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/web/research_chat_browser.ep`

[tests/web/research_chat_browser.ep](../../tests/web/research_chat_browser.ep) · 5 lines · 269 bytes

SHA-256: `7d5600abf275f03808d79c6ee10f5acf73e2d9667bf00dd46d1503b90f2296db`

Source annotations: Use only with a disposable copy of the retained live-web-fetch acceptance DB. Production client main is renamed when linking this isolated entry point.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main:
```

## `tests/web/research_panel_browser.ep`

[tests/web/research_panel_browser.ep](../../tests/web/research_panel_browser.ep) · 12 lines · 558 bytes

SHA-256: `a76522bbf0dfcdb7db636b1966cb879dff253c7173fae4b0ed30449bd8f92619`

Source annotations: Append to production client with its main renamed for isolated rendering.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define main:
```

## `tests/web/seed_device_routes.ep`

[tests/web/seed_device_routes.ep](../../tests/web/seed_device_routes.ep) · 17 lines · 1086 bytes

SHA-256: `111e69470b5a82944d1fe85cdce5f0d8421e337a29bfdcff09940862fcdb6888`

Source annotations: Public test credentials, only for a disposable loopback fixture database.

Imports: [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/seed_rendering.ep`

[tests/web/seed_rendering.ep](../../tests/web/seed_rendering.ep) · 35 lines · 2581 bytes

SHA-256: `d3c42854b8838b9707b0bedcc816b171ec0894ee2cab937984e4ca50af6212fd`

Source annotations: Synthetic UI acceptance content, never claimed as a model response.

Imports: [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/seed_retry_browser.ep`

[tests/web/seed_retry_browser.ep](../../tests/web/seed_retry_browser.ep) · 20 lines · 1246 bytes

SHA-256: `8e121f85e3ca6d24719510153f995c3402a1fa7c1e7d19fc927d204c5a21ddf8`

Source annotations: Disposable local fixture: invitation and credentials are test values.

Imports: [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/serve_partial_fixture.ep`

[tests/web/serve_partial_fixture.ep](../../tests/web/serve_partial_fixture.ep) · 43 lines · 2189 bytes

SHA-256: `b078551b7a3ae90e1c54928854f126822a7b123785a6058209aecb4903d5ab26`

Source annotations: EOF framing permits an unfinished response with no arbitrary body cap.

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), [../../app/web/http_api](../../app/web/http_api.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/serve_provider_fixture.ep`

[tests/web/serve_provider_fixture.ep](../../tests/web/serve_provider_fixture.ep) · 20 lines · 799 bytes

SHA-256: `695bd04c25c582c794e9c804bf077c3a498dea995ff174402572d348cbefc623`

Imports: [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/serve_recovery_fixture.ep`

[tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep) · 57 lines · 3493 bytes

SHA-256: `597a3dd74cb539bb8e53ca48ae81e9d24d0b73f832882196b01ae83ffa51f0b1`

Source annotations: The separate server executable must discover and resume the interrupted run. This process only serves scripted provider responses and verifies the result.

Imports: [../../app/web/recovery](../../app/web/recovery.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/web/share_controls_seed.ep`

[tests/web/share_controls_seed.ep](../../tests/web/share_controls_seed.ep) · 17 lines · 942 bytes

SHA-256: `4f7830f98b7946dbf2f862e87eaed2ef50ebbccb233b27ba7566c513631d31ad`

Imports: [../../app/web/routes](../../app/web/routes.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_activity.ep`

[tests/web/test_activity.ep](../../tests/web/test_activity.ep) · 87 lines · 5805 bytes

SHA-256: `69b9b753c1a8b46c8941664e3a162f43fb5ff50b1efa58fca099c743fbf573fc`

Imports: [../../app/web/activity](../../app/web/activity.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define activity_expect with database as Int and expected as Str returning Int:
11: define main returning Int:
```

## `tests/web/test_agent_compaction_status.ep`

[tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep) · 41 lines · 3116 bytes

SHA-256: `4279f01db1f297596dc1cd5eb4fb5d0f0a08c772dcd286491895bc2265daa485`

Imports: [../../app/context/compaction_resume](../../app/context/compaction_resume.ep), [../../app/web/work_status](../../app/web/work_status.ep), [../../app/web/worker](../../app/web/worker.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/web/test_attachment_lifecycle.ep`

[tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep) · 51 lines · 4150 bytes

SHA-256: `e2e4bae51b3c02f6769299fc987c95642d97763a1e1728c11ee839e76ba91ec6`

Imports: [../../app/web/attachment_lifecycle](../../app/web/attachment_lifecycle.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define lifecycle_query with action as Str and attachment as Str and identity as Str and previous as Str returning List:
12: define main returning Int:
```

## `tests/web/test_attachment_text.ep`

[tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep) · 44 lines · 3441 bytes

SHA-256: `d21bd7d9d48840b593c234a0c0df4ddaeb96830758f5808e4cc87d68845e7ffc`

Imports: [../../app/tools/attachment_tools](../../app/tools/attachment_tools.ep), [../../app/web/attachments](../../app/web/attachments.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_attachment_tools.ep`

[tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep) · 54 lines · 4206 bytes

SHA-256: `d8a051b028afc59520b00f0bde027926a690d5d89af382dc25376d090bb1a2ee`

Imports: [../../app/tools/attachment_tools](../../app/tools/attachment_tools.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_attachment_usage.ep`

[tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep) · 37 lines · 3158 bytes

SHA-256: `a4a418d2e25e4a4c0f2c543667d7d3d6083866e7caef73fe73e8f2b0cb2d9829`

Imports: [../../app/web/attachment_usage](../../app/web/attachment_usage.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/tools/attachment_text](../../app/tools/attachment_text.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_attachment_visibility.ep`

[tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep) · 50 lines · 4087 bytes

SHA-256: `cf2989f50bf55b70c0ea78378ec64dc99b212610d1ac00f4ba6d85ae2926c3b1`

Imports: [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define visibility_query with action as Str and attachment as Str and identity as Str and previous as Str returning List:
12: define main returning Int:
```

## `tests/web/test_attachment_visibility_readers.ep`

[tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep) · 81 lines · 6712 bytes

SHA-256: `0510edc3df05dfb4675b79c2bb6bc305d09e5449737c02a6b770b7b2ac2baa8f`

Imports: [../../app/web/attachment_visibility](../../app/web/attachment_visibility.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/tools/attachment_text](../../app/tools/attachment_text.ep), [../../app/tools/attachment_image](../../app/tools/attachment_image.ep), [../../app/media/speech_request](../../app/media/speech_request.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define visibility_query with action as Str and attachment as Str and identity as Str and previous as Str returning List:
15: define main returning Int:
```

## `tests/web/test_attachments.ep`

[tests/web/test_attachments.ep](../../tests/web/test_attachments.ep) · 40 lines · 2941 bytes

SHA-256: `0cdfac59b8f7fecd197779d4c1db76c6a6a1a1345c29996f84e46f0d38043a85`

Imports: [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_backup.ep`

[tests/web/test_backup.ep](../../tests/web/test_backup.ep) · 56 lines · 3711 bytes

SHA-256: `5c34d2fac93f326e93cab4dc099be79a2e6fbf6ca1431548f77379f47abb55d3`

Source annotations: Later source changes must not leak into the consistent snapshot.

Imports: [../../app/web/worker](../../app/web/worker.ep), [../../app/archive/snapshot](../../app/archive/snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_branches.ep`

[tests/web/test_branches.ep](../../tests/web/test_branches.ep) · 118 lines · 9127 bytes

SHA-256: `9c28f674244001e9bf255717ee371f00e5d0feb05b9c54079f6bc073c84703f1`

Imports: [../../app/web/export](../../app/web/export.ep), [../../app/archive/export_restore](../../app/archive/export_restore.ep), `fs` (toolchain/library), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/web/branches](../../app/web/branches.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define branch_fixture_finish with database as Int and run as Str and user_event as Str and request as Str and final as Str returning Int:
18: define main returning Int:
```

## `tests/web/test_cancel_busy.ep`

[tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep) · 44 lines · 2202 bytes

SHA-256: `7d3704fafed1102602bc8abb6da44ac97259a0501aec6102a0bccfb90a54cbba`

Imports: [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_cancel_queue.ep`

[tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep) · 49 lines · 4022 bytes

SHA-256: `f2df5dd61f2bf9ff6b214fee7d6cca7db5997d8dab1efd191cdd4daa748592dc`

Imports: [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define queue_cancel_fixture with database as Int and run as Str and session as Str and call_id as Str returning Int:
12: define main returning Int:
```

## `tests/web/test_cancellation.ep`

[tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep) · 75 lines · 4221 bytes

SHA-256: `d10040f226f1571be2ffeaecb096f8be16685864f6f28a6eaaae115bbce37225`

Imports: [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define request_stop_later with path as Str and completed as Int returning Int:
11: define main returning Int:
```

## `tests/web/test_conversation_merge.ep`

[tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep) · 50 lines · 3291 bytes

SHA-256: `a13fb73241d5666b207416839b84dfeef937fdc64c3b879a20896c83221b0987`

Imports: [../../app/web/device_links](../../app/web/device_links.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_device_links.ep`

[tests/web/test_device_links.ep](../../tests/web/test_device_links.ep) · 54 lines · 3375 bytes

SHA-256: `d32f56e2e37561e13a390dc85ff653478436ad50b3ba2d65a1c256ece590fb7f`

Imports: [../../app/web/device_links](../../app/web/device_links.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_devices.ep`

[tests/web/test_devices.ep](../../tests/web/test_devices.ep) · 96 lines · 6895 bytes

SHA-256: `100bd554f68839a864e86599b4f9a5857c88c9bd15a2545ce854c213dc9338a8`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/security/revocation](../../app/security/revocation.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_empty_session.ep`

[tests/web/test_empty_session.ep](../../tests/web/test_empty_session.ep) · 45 lines · 3117 bytes

SHA-256: `226b1d23d19f399eea80a82fed637cb75b3b5b0e3c7ef338cd51fea576f94956`

Imports: [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_export.ep`

[tests/web/test_export.ep](../../tests/web/test_export.ep) · 46 lines · 2872 bytes

SHA-256: `2c5a3908d7b53a5d88b46e660ad239f3ec21026e58b13f938471412b27e4f3ca`

Imports: [../../app/web/export](../../app/web/export.ep), [../../app/archive/export_verify](../../app/archive/export_verify.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_group_browser.ep`

[tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep) · 88 lines · 6183 bytes

SHA-256: `fc1b0274e9bb845f127d371a1148babc2f1eea45d20cba46605e162c924c2e97`

Source annotations: Uses the actual isolated server, HTTP authentication, process owner and model.

Imports: [../fixtures/echo_http_client](../../tests/fixtures/echo_http_client.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep), [../../app/web/groups](../../app/web/groups.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/web/test_group_controls.ep`

[tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep) · 92 lines · 7200 bytes

SHA-256: `267991eb44e36ff9695e339e5235035005a0bba189f6e13fe1acd7c5bf3b62c8`

Imports: [../../app/web/groups](../../app/web/groups.ep), [../../app/security/group_output_policy](../../app/security/group_output_policy.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_group_export.ep`

[tests/web/test_group_export.ep](../../tests/web/test_group_export.ep) · 45 lines · 3882 bytes

SHA-256: `1dd2ed08a01f130598c585b2d37327f7dea99139b56ef6613b0f10c88e5f82fb`

Imports: [../../app/security/group_speech](../../app/security/group_speech.ep), [../../app/web/export](../../app/web/export.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_history_pages.ep`

[tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep) · 72 lines · 3851 bytes

SHA-256: `84f2c259b6fea65bf79b4b0d6918e0be9d24927d347830ff59ac93b50ef76d1d`

Imports: [../../app/web/branches](../../app/web/branches.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_http_api.ep`

[tests/web/test_http_api.ep](../../tests/web/test_http_api.ep) · 61 lines · 5012 bytes

SHA-256: `822fbe83710ab76a60745954c80d84434db25ff68477d5fe04c6f753cc7cd0a5`

Imports: [../../app/web/http_api](../../app/web/http_api.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define api_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:
12: define main returning Int:
```

## `tests/web/test_idle_cancel.ep`

[tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep) · 50 lines · 3228 bytes

SHA-256: `8cbbbc38c27aa3bb1f075945aacd5b7970244fdd10a471a9f580132f4aab9b0c`

Imports: [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_late_parent_reply.ep`

[tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep) · 62 lines · 4790 bytes

SHA-256: `615e6e104e4ba714ddff549dea58187603312a373ccdd8ed137dcfbbd69dc8b9`

Imports: [../../app/web/run_store](../../app/web/run_store.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/web/branches](../../app/web/branches.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define fixture_reply with database as Int and run as Str and event as Str and text as Str returning Int:
13: define main returning Int:
```

## `tests/web/test_live_attach_server.ep`

[tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep) · 55 lines · 3871 bytes

SHA-256: `6f25a75e1f02ad5fc956733f4e5f2cd36abc34017fb19fc4b7f6305d1feaf4d6`

Imports: [../fixtures/echo_http_client](../../tests/fixtures/echo_http_client.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/control](../../app/agent/control.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_live_continuity_server.ep`

[tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep) · 59 lines · 4818 bytes

SHA-256: `a49da7df72a50ebf5b103e56c2cf6334eb1eea15bb2c1c9fd975539dc99c2fe4`

Imports: [../fixtures/echo_http_client](../../tests/fixtures/echo_http_client.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/control](../../app/agent/control.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_live_guardian_server.ep`

[tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep) · 55 lines · 3683 bytes

SHA-256: `d0a1ad4432e3adcb8256ea78ee808cdd82f7ef016609229033b90fa697aac10a`

Imports: [../fixtures/echo_http_client](../../tests/fixtures/echo_http_client.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_live_memory_server.ep`

[tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep) · 55 lines · 3587 bytes

SHA-256: `3cf0de35d0d6c604d47beb860902c60b642f59b813e6f2f9d799f35aef285bbc`

Imports: [../fixtures/echo_http_client](../../tests/fixtures/echo_http_client.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/agent/control](../../app/agent/control.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_long_message_archive.ep`

[tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep) · 53 lines · 3190 bytes

SHA-256: `e3ece6311b39badec036398f1d8d3a48919b231a9ef96fa625764b3b7ba3cd49`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/web/http_api](../../app/web/http_api.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_midturn_guidance.ep`

[tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep) · 137 lines · 9995 bytes

SHA-256: `bb1da521882aa0b950625a5dac9618adce70ac2666a426797b1a2f24a51f6134`

Source annotations: Deliberately send no HTTP headers or SSE events. Guidance must wake the

Imports: [../../app/web/routes](../../app/web/routes.ep), [../../app/agent/run_messages](../../app/agent/run_messages.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define guidance_arrival_fixture with listener as List and data as Str and completed as Int and database_path as Str returning Int:
42: define main returning Int:
```

## `tests/web/test_pdf_attachment.ep`

[tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep) · 36 lines · 2640 bytes

SHA-256: `9cb78f381007dec18475a1daef9ffbdaf93cabb54d7281b6e661491496fb0327`

Imports: [../../app/tools/attachment_tools](../../app/tools/attachment_tools.ep), [../../app/web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_progress.ep`

[tests/web/test_progress.ep](../../tests/web/test_progress.ep) · 45 lines · 3145 bytes

SHA-256: `f91d3ccf0fe7496eaf6acee772a2b06c8416332ffa69f225de91fffabdab4014`

Imports: [../../app/web/progress](../../app/web/progress.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define progress_piece with database as Int and request as Str and sequence as Int and data as Str returning Int:
8: define main returning Int:
```

## `tests/web/test_projects.ep`

[tests/web/test_projects.ep](../../tests/web/test_projects.ep) · 73 lines · 6363 bytes

SHA-256: `f4bb6e8cc9f159619b95e3ac393466594cbbb68afadfde19f0287d740be4f1c0`

Imports: [../../app/web/projects](../../app/web/projects.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define browser_project_query with operation as Str and session as Str and identity as Str and project as Str and expected as Str and manifest as Str returning Str:
10: define browser_project_status with result as Str returning Str:
14: define main returning Int:
```

## `tests/web/test_provider_submission.ep`

[tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep) · 57 lines · 4462 bytes

SHA-256: `a1ce4ba6466d62de76db40e266cc3dabccbc879ab61ce815df6c19d2cd276e77`

Imports: [../../app/web/http_api](../../app/web/http_api.ep), [../../app/agent/provider_profile](../../app/agent/provider_profile.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_quarantine_monitor.ep`

[tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep) · 70 lines · 3996 bytes

SHA-256: `eee96415cb3004011e596cff7064174035ec2ea56ac29d2eab43209dde2ce3da`

Imports: [../../app/rights/quarantine](../../app/rights/quarantine.ep), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define request_stop_later with path as Str and charter as Str and completed as Int returning Int:
15: define main returning Int:
```

## `tests/web/test_research_sources.ep`

[tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep) · 40 lines · 4083 bytes

SHA-256: `74d65b37d8834ebd1871b44c78ef0a7ac2089c7742fed3a59d9c56f63a9339fa`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define research_fixture_call with database as Int and session as Str and identity as Str and digest as Str and receipt as Str returning Int:
11: define main returning Int:
```

## `tests/web/test_retry.ep`

[tests/web/test_retry.ep](../../tests/web/test_retry.ep) · 46 lines · 4036 bytes

SHA-256: `f1f79fdffcc6047dd23b4e6671df588b6ce81189a346938cf7ba951a06b85657`

Imports: [../../app/web/worker](../../app/web/worker.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_revisions.ep`

[tests/web/test_revisions.ep](../../tests/web/test_revisions.ep) · 113 lines · 9492 bytes

SHA-256: `fe99143b96158b95062f7dae99bd960427a76c0a2c042c104548165cf8e33af4`

Imports: [../../app/web/branches](../../app/web/branches.ep), [../../app/web/attachments](../../app/web/attachments.ep), [../../app/web/export](../../app/web/export.ep), `fs` (toolchain/library), [../../app/web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/web/test_run_store.ep`

[tests/web/test_run_store.ep](../../tests/web/test_run_store.ep) · 52 lines · 3285 bytes

SHA-256: `bd911c4ec743ff11660573ab0e4aa2cdf5624b25f48a3dd5e9b8c460adc40214`

Source annotations: Inject run creation failure after the pending message/session inserts.

Imports: [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_session_rename.ep`

[tests/web/test_session_rename.ep](../../tests/web/test_session_rename.ep) · 46 lines · 3101 bytes

SHA-256: `91adfab3e742995cd47670086b1dc3c1147d5e44be47bd7cb5423472aad7424a`

Imports: [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_submission_route_recovery.ep`

[tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep) · 104 lines · 7245 bytes

SHA-256: `112d8c09694c683523f980666bd2f207bdc62d313288cc78528ad137133c1df2`

Source annotations: Exercise the actual authenticated HTTP route through a native socket reader.

Imports: [../../app/web/routes](../../app/web/routes.ep), `os` (toolchain/library), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define recovery_route with config as List and target as Str and body as Str returning List:
22: define recovery_await with database as Int and run as Str returning Int:
36: define main returning Int:
```

## `tests/web/test_trace.ep`

[tests/web/test_trace.ep](../../tests/web/test_trace.ep) · 39 lines · 2715 bytes

SHA-256: `8fcc696eeba8b97aafc2c398e1c020f73c08b4862c67d8919c7a5ac1423222cd`

Imports: [../../app/web/trace](../../app/web/trace.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define progress_piece with database as Int and request as Str and sequence as Int and data as Str returning Int:
8: define main returning Int:
```

## `tests/web/test_turn_attachments.ep`

[tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep) · 75 lines · 5388 bytes

SHA-256: `f91ebeff5574d2130dd0d44680bb6c8b5bdc2d789fc4ea9eda4039f6a84e5b58`

Imports: [../../app/web/attachments](../../app/web/attachments.ep), [../../app/web/http_api](../../app/web/http_api.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/web/test_upload_body.ep`

[tests/web/test_upload_body.ep](../../tests/web/test_upload_body.ep) · 56 lines · 3171 bytes

SHA-256: `41831bc5dfea462ca0f895db110f625c2cefbd4b6864ff4f6a94ca1198cffdbf`

Imports: [../../app/web/upload_body](../../app/web/upload_body.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define upload_sender with library as Int and descriptor as Int and bytes as Int and completed as Int returning Int:
26: define upload_case with path as Str and bytes as Int returning Int:
50: define main returning Int:
```

## `tests/web/test_upload_disconnect.ep`

[tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep) · 60 lines · 3930 bytes

SHA-256: `98b2b1d350895ffad181cce3d06d6bdfeb3a8b95ee01fd6bac18e75347533534`

Source annotations: Public credentials and finite payload only for this isolated loopback fixture. Modes: setup, disconnect, crash, verify. Crash keeps its socket open until the externally terminated fixture server closes it; no elapsed-time verdict.

Imports: [../../app/web/uploads](../../app/web/uploads.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/web/test_uploads.ep`

[tests/web/test_uploads.ep](../../tests/web/test_uploads.ep) · 58 lines · 4670 bytes

SHA-256: `3afff105cd08b0d7048d52722b1b2e510ecc4fb190503cd5ac746d31248b373a`

Imports: [../../app/web/uploads](../../app/web/uploads.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/web/test_work_lineage.ep`

[tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep) · 48 lines · 4344 bytes

SHA-256: `b8e81ab64a8b20427dd29c4fdf675cb6399b861a8b22a3450cd5307ef15f9a08`

Imports: [../../app/web/work_status](../../app/web/work_status.ep), [../../app/web/cancellation](../../app/web/cancellation.ep), [../fixtures/schedule_fixture](../../tests/fixtures/schedule_fixture.ep), [../fixtures/delegation_fixture](../../tests/fixtures/delegation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define work_fixture_page with database as Int and device as Str and session as Str and family as Str returning Str:
10: define main returning Int:
```

## `tests/web/test_work_status.ep`

[tests/web/test_work_status.ep](../../tests/web/test_work_status.ep) · 32 lines · 2513 bytes

SHA-256: `8c785865a8f4159f1ce100e864748356c530adb6192fb22533c468b6dda73ad6`

Imports: [../../app/web/work_status](../../app/web/work_status.ep), [../../app/web/cancellation](../../app/web/cancellation.ep), [../../app/jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/web/test_worker_recovery.ep`

[tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep) · 82 lines · 5555 bytes

SHA-256: `de7c26eb9a2b4e8a2fdddcbc76044e7921b7eddfc91e0eaa1274e8aa2c8eb3c1`

Source annotations: Restart the fixture at its persisted endpoint, as a real provider restart must do. A new ephemeral endpoint would be a different provider profile.

Imports: [../../app/web/recovery](../../app/web/recovery.ep), [../fixtures/runner_provider](../../tests/fixtures/runner_provider.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/hello_sandbox.ep`

[tests/workspace/hello_sandbox.ep](../../tests/workspace/hello_sandbox.ep) · 7 lines · 187 bytes

SHA-256: `a991274f1ca07e27a5300fdb6caa1cd334a024cccc467d69bcf35a9faef496f5`

Imports: `string` (toolchain/library).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/workspace/test_build_arguments.ep`

[tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep) · 21 lines · 1428 bytes

SHA-256: `a96807b8966df5ddf2381fbb47684f0a520b56313746c14913b402bac2c8cac5`

Imports: [../../app/providers/archived_stream](../../app/providers/archived_stream.ep), `os` (toolchain/library), [../../app/workspace/build_arguments](../../app/workspace/build_arguments.ep), [../../app/tools/build_schema](../../app/tools/build_schema.ep), [../../app/security/tool_policy](../../app/security/tool_policy.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/workspace/test_capture_process.ep`

[tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep) · 111 lines · 6042 bytes

SHA-256: `e49c0c1819b55bfe6dcbaafa0d9d9d4ddf6320a92911bdf6afcd13114f78fd44`

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/process_output](../../app/workspace/process_output.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library), [../../app/workspace/capture_process](../../app/workspace/capture_process.ep).

Native `main` present. Argument indices read: 0, 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: external define peek_byte with pointer and offset returning Int
8: external define poke_byte with pointer and offset and value returning Int
10: define capture_cancel_after_eof with path as Str and writer as Int and completed as Int returning Int:
26: define main returning Int:
```

## `tests/workspace/test_compile_process.ep`

[tests/workspace/test_compile_process.ep](../../tests/workspace/test_compile_process.ep) · 51 lines · 2895 bytes

SHA-256: `49f18b1b43a535b62928659ae7d679224937d3a3051a16904c14f54e9c91cbe8`

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define compile_probe_run with runtime as Str and workspace as Str and executable as Str and source as Str and phase as Str returning Int:
37: define main returning Int:
```

## `tests/workspace/test_create_archive.ep`

[tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep) · 39 lines · 2505 bytes

SHA-256: `a88365b7e153d055cc5c93562994c14372344507483adce72f8443ab5d754219`

Imports: [../../app/workspace/create_archive](../../app/workspace/create_archive.ep), [../../app/workspace/list_archive](../../app/workspace/list_archive.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_create_text.ep`

[tests/workspace/test_create_text.ep](../../tests/workspace/test_create_text.ep) · 38 lines · 2384 bytes

SHA-256: `c8aa100be03086012720fe196010c0a2d7d321507ff4c4304e5918021bef7a46`

Imports: [../../app/workspace/create_text](../../app/workspace/create_text.ep), [../../app/workspace/read_text](../../app/workspace/read_text.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_create_workbook.ep`

[tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep) · 57 lines · 4061 bytes

SHA-256: `9d69c19702a1faf9cd99d55e4daaa1e08ca27ba2e3d8b27cc7839aaf72418691`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_directories.ep`

[tests/workspace/test_directories.ep](../../tests/workspace/test_directories.ep) · 56 lines · 2747 bytes

SHA-256: `fa9c3f2af99bb46e567a5dd30703c9262f0dbfdffea8dab37a76e555945b5d96`

Imports: [../../app/workspace/directories](../../app/workspace/directories.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/workspace/test_directory_list.ep`

[tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep) · 63 lines · 4352 bytes

SHA-256: `a0787ac758ba73ebc6557992e8c9d1dadd459921cec319f7b89bf59b7129b34c`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_edit_workbook.ep`

[tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep) · 68 lines · 5000 bytes

SHA-256: `88dd1a121ab1814062c362a3126a14f3f2513ed9b9808782976cab89f3c90ea5`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep), [../../app/workspace/list_archive](../../app/workspace/list_archive.ep).

Environment names: `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/workspace/test_ernos_runtime.ep`

[tests/workspace/test_ernos_runtime.ep](../../tests/workspace/test_ernos_runtime.ep) · 32 lines · 2152 bytes

SHA-256: `f20f4319386b683cd8a311868b8c7f6c47c0e8d3262dc72f8ac534e1334dd1fa`

Imports: [../../app/workspace/ernos_runtime](../../app/workspace/ernos_runtime.ep), `fs` (toolchain/library).

Environment names: `ECHO_ERNOS_COMPILER_SHA256`.

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_execute_claimed.ep`

[tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep) · 103 lines · 5683 bytes

SHA-256: `7cdb18599e15dc5a8bbd82a8a5d0ba3026739d5685e9f4b5d6fb586c52860780`

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/process_output](../../app/workspace/process_output.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library), [../../app/workspace/execute_process](../../app/workspace/execute_process.ep).

Native `main` present. Argument indices read: 0, 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: external define peek_byte with pointer and offset returning Int
8: external define poke_byte with pointer and offset and value returning Int
10: define capture_cancel_after_eof with path as Str and writer as Int and completed as Int returning Int:
26: define main returning Int:
```

## `tests/workspace/test_execution_request.ep`

[tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep) · 90 lines · 5642 bytes

SHA-256: `88762a668258df7f57e2b7a2428874ee2f60be0b64362c8fb1caf7d229f33bc3`

Imports: [../../app/workspace/execution_request](../../app/workspace/execution_request.ep), [../../app/tools/execution_output](../../app/tools/execution_output.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../fixtures/generation_fixture](../../tests/fixtures/generation_fixture.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define execution_request_fixture with database as Int and identity as Str and arguments as Str returning Str:
33: define main returning Int:
```

## `tests/workspace/test_extract_archive.ep`

[tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep) · 36 lines · 2321 bytes

SHA-256: `1da15511a04866216e98f4960b4f64bfce61735ed40f46a8a0913f85df0f5274`

Imports: [../../app/workspace/list_archive](../../app/workspace/list_archive.ep), [../../app/workspace/read_text](../../app/workspace/read_text.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_files.ep`

[tests/workspace/test_files.ep](../../tests/workspace/test_files.ep) · 46 lines · 2377 bytes

SHA-256: `79ec7a305a3d3f4ceb7fe7cc779d5904ba895a50d57c68c6b1bc1868bf12fb92`

Imports: [../../app/workspace/files](../../app/workspace/files.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/workspace/test_host_profile.ep`

[tests/workspace/test_host_profile.ep](../../tests/workspace/test_host_profile.ep) · 53 lines · 2790 bytes

SHA-256: `bfe254258d3674a1f8b9f15248deff03370a1ca5ff2367f445958c4c82a21688`

Source annotations: The child is real native Ernos running inside the generated admin profile.

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/workspace/test_launch_interruption.ep`

[tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep) · 65 lines · 3510 bytes

SHA-256: `ed564b16cc1977e569d30798f2cb32db549df580c5808e85b453daf5bf782fd2`

Imports: [../../app/workspace/process_journal](../../app/workspace/process_journal.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_list_archive.ep`

[tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep) · 83 lines · 6053 bytes

SHA-256: `91ebb60936a55abceab41c142c5b1446a260aefcb829d0699e4d1cc91d116321`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_network_isolation.ep`

[tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep) · 70 lines · 3260 bytes

SHA-256: `6e4299998cc129753e6591c96a2fb950044d1f2c931bc2395a7209470da19312`

Source annotations: sockaddr layouts and family constants are from the installed macOS SDK. Every address must first connect in the unsandboxed positive-control run.

Imports: [../../app/transport/socket_read](../../app/transport/socket_read.ep), [../../app/transport/http_head](../../app/transport/http_head.ep), [../../app/providers/archived_stream](../../app/providers/archived_stream.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define network_probe with library as Int and family as Int and port as Int and path as Str returning Int:
46: define main returning Int:
```

## `tests/workspace/test_output_archive.ep`

[tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep) · 69 lines · 4394 bytes

SHA-256: `b88e71447b329c84df21dbee1ac37a6ae23d574f5fb386effa512dcec189b0ff`

Imports: [../../app/workspace/output_archive](../../app/workspace/output_archive.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_plot_numbers.ep`

[tests/workspace/test_plot_numbers.ep](../../tests/workspace/test_plot_numbers.ep) · 12 lines · 457 bytes

SHA-256: `ae6738a04d4698f16fb38e58aad0ae371f4aa9671ba43cf39389b85cc91d4597`

Imports: [../../app/workspace/plot_numbers](../../app/workspace/plot_numbers.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/workspace/test_plot_svg.ep`

[tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep) · 47 lines · 3685 bytes

SHA-256: `5eb343a94924bf9eb1b289d3471011e4e37417d4459fc6259f372ea2498886e3`

Imports: [../../app/workspace/plot_svg](../../app/workspace/plot_svg.ep), `fs` (toolchain/library), [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/workspace/test_process.ep`

[tests/workspace/test_process.ep](../../tests/workspace/test_process.ep) · 94 lines · 5018 bytes

SHA-256: `6f6c4709d05f4229fb7896985623e3e228a0f397ed615e09ce29d4ab5da18282`

Source annotations: The deliberately inherited parent descriptor must be absent. 196608 output bytes exceed the unsafe upstream helper's 65536 buffer.

Imports: [../../app/workspace/native_process](../../app/workspace/native_process.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Environment names: `ECHO_PROCESS_SECRET`, `ECHO_PROCESS_TEST`.

Native `main` present. Argument indices read: 0, 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_process_guard.ep`

[tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep) · 50 lines · 2645 bytes

SHA-256: `844ffa8e42a1937d4734401133fc670d0ff3681915cf4917ab95170615d86561`

Source annotations: The owner deliberately SIGKILLs itself only after the guardian's real child is running. An external test driver checks the guardian's waitpid receipt.

Imports: [../../app/workspace/process_guard](../../app/workspace/process_guard.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 0, 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `tests/workspace/test_process_journal.ep`

[tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep) · 132 lines · 7647 bytes

SHA-256: `92e89666a9a13cffab4909e32a9a2ee4202c93f6f6c5abcc7ed9e5fc037c0d5c`

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/process_output](../../app/workspace/process_output.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library), [../../app/workspace/capture_process](../../app/workspace/capture_process.ep), [../../app/workspace/process_journal](../../app/workspace/process_journal.ep).

Native `main` present. Argument indices read: 0, 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: external define peek_byte with pointer and offset returning Int
9: external define poke_byte with pointer and offset and value returning Int
11: define capture_cancel_after_eof with path as Str and writer as Int and completed as Int returning Int:
27: define main returning Int:
```

## `tests/workspace/test_process_output.ep`

[tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep) · 91 lines · 4554 bytes

SHA-256: `8d4f5cd782d123e1fb7f01b094e084ad189adb5c3179da5ec2afda8b43d0567d`

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/process_output](../../app/workspace/process_output.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 0, 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: external define peek_byte with pointer and offset returning Int
7: external define poke_byte with pointer and offset and value returning Int
9: define main returning Int:
```

## `tests/workspace/test_process_output_archive.ep`

[tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep) · 119 lines · 6512 bytes

SHA-256: `12a928511f5f65b6f02d2e999036bde65bae188c311289c09ef812debeea8e96`

Imports: [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/workspace/process_output](../../app/workspace/process_output.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library), [../../app/workspace/output_archive](../../app/workspace/output_archive.ep).

Native `main` present. Argument indices read: 0, 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: external define peek_byte with pointer and offset returning Int
8: external define poke_byte with pointer and offset and value returning Int
10: define main returning Int:
```

## `tests/workspace/test_process_tree.ep`

[tests/workspace/test_process_tree.ep](../../tests/workspace/test_process_tree.ep) · 77 lines · 4068 bytes

SHA-256: `21e287a42af4e6b93aa286cbb583ac4e1e3e1929edb118ebd696f3a4a1f54f59`

Imports: [../../app/workspace/native_process](../../app/workspace/native_process.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 0, 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_publication_capability.ep`

[tests/workspace/test_publication_capability.ep](../../tests/workspace/test_publication_capability.ep) · 40 lines · 2330 bytes

SHA-256: `fe3633e70566512310dad2fde661cd3d453b7e4a76de0c89382752c06504e60c`

Imports: [../../app/workspace/create_text](../../app/workspace/create_text.ep), [../../app/workspace/read_text](../../app/workspace/read_text.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define publication_contender with library as Int and root as Int and content as Str and completed as Int returning Int:
9: define main returning Int:
```

## `tests/workspace/test_read_json.ep`

[tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep) · 46 lines · 3128 bytes

SHA-256: `dedd5a74076cc68a83f97fef56345698622c3fc2bc20fde5c7e02653ffc8c383`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_read_table.ep`

[tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep) · 59 lines · 3881 bytes

SHA-256: `1f08c422b573835a4b0db146f8062155b26b6d94e6fc69fd7ad14c3f6ca9c3dc`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define table_status with result as Str returning Str:
9: define main returning Int:
```

## `tests/workspace/test_read_text.ep`

[tests/workspace/test_read_text.ep](../../tests/workspace/test_read_text.ep) · 44 lines · 2954 bytes

SHA-256: `2c34963b78d1b7acd5c53e78e5485aa58e9df3a0880804827a607194321757a8`

Source annotations: Replace the path after validation. The existing descriptor must still read the original inode and never follow the replacement symlink.

Imports: [../../app/workspace/read_text](../../app/workspace/read_text.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/workspace/test_read_workbook.ep`

[tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep) · 56 lines · 3653 bytes

SHA-256: `83c0cabaceecb9884c5f2fd5d30486b5a37c5345d02b657a31e180a3538f8d60`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep), [../../app/archive/file_copy](../../app/archive/file_copy.ep).

Environment names: `ECHO_WORKSPACE_ROOT`.

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_resource_profile.ep`

[tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep) · 71 lines · 4295 bytes

SHA-256: `619edca41dfd03b2844ff0c96b45d8122f66eb063a17841c5141f67e619d01ca`

Source annotations: A real child proves the OS boundary, not merely the generated policy text. Arguments: binary, workspace, public-read-root, public-write-root, private, protected, secret, live-loopback-port. Parent fixture provides sentinel files.

Imports: [../../app/workspace/resource_profile](../../app/workspace/resource_profile.ep), [../../app/workspace/sandbox_process](../../app/workspace/sandbox_process.ep), [../../app/transport/http_stream](../../app/transport/http_stream.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7, 8, 9. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `tests/workspace/test_sandbox.ep`

[tests/workspace/test_sandbox.ep](../../tests/workspace/test_sandbox.ep) · 90 lines · 4849 bytes

SHA-256: `51596dfda25d621d5024a7d72bc7b928bde2de110b43e2bb5a0b34922a1a195b`

Imports: [../../app/workspace/sandbox_profile](../../app/workspace/sandbox_profile.ep), [../../app/transport/local_socket](../../app/transport/local_socket.ep), [../../app/transport/http_stream](../../app/transport/http_stream.ep), `fs` (toolchain/library).

Environment names: `ECHO_ERNOS_RUNTIME`.

Native `main` present. Argument indices read: 0, 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/workspace/test_sandbox_canonical_path.ep`

[tests/workspace/test_sandbox_canonical_path.ep](../../tests/workspace/test_sandbox_canonical_path.ep) · 19 lines · 847 bytes

SHA-256: `2dd278c65722c12f0515c8a0d0356b34b12c6483c8c82af0f093b183c066048c`

Source annotations: Qualify the path resolver needed by the unmodified Ernos compiler. Run this from the fixture workspace through its exact sandbox profile.

Imports: [../../app/transport/socket_read](../../app/transport/socket_read.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_sandbox_process.ep`

[tests/workspace/test_sandbox_process.ep](../../tests/workspace/test_sandbox_process.ep) · 57 lines · 3006 bytes

SHA-256: `6ae9089c574fc2e0ef1a42c6e59986e173f6a37b93a69c2188221ceb5f30cb6e`

Imports: [../../app/workspace/native_process](../../app/workspace/native_process.ep), [../../app/workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library), `string` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `tests/workspace/test_sessions.ep`

[tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep) · 42 lines · 3063 bytes

SHA-256: `5dd7c432e91c8a1c270ee3598a396b8f767b61eef1aad6965dd3c28c7a57601d`

Imports: [../../app/workspace/sessions](../../app/workspace/sessions.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_text_operations.ep`

[tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep) · 76 lines · 5283 bytes

SHA-256: `7b913246afb4f76932b0fb425bd7b5ac493318ba4361d8684d57d55612d6e72c`

Imports: [../../app/tools/workspace_tools](../../app/tools/workspace_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `tests/workspace/test_tool.ep`

[tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep) · 100 lines · 7577 bytes

SHA-256: `9e05f4e50daab81c3afff290907168e18b1329979fef5248fc193dac157c47ae`

Imports: [../../app/tools/attachment_tools](../../app/tools/attachment_tools.ep), [../../app/web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `tests/workspace/test_xml_nodes.ep`

[tests/workspace/test_xml_nodes.ep](../../tests/workspace/test_xml_nodes.ep) · 15 lines · 1064 bytes

SHA-256: `4112d6981898d840225b7737675b4e6f5e0f84bc52d1f2b9cb72fa9a21f3e91b`

Imports: [../../app/workspace/xml_nodes](../../app/workspace/xml_nodes.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `tests/workspace/test_zip_names.ep`

[tests/workspace/test_zip_names.ep](../../tests/workspace/test_zip_names.ep) · 28 lines · 1791 bytes

SHA-256: `2f66e5cf8a7f17540ffd2c7d6c94835d99b08b8bbdc2ad252533425fc500f1ac`

Imports: [../../app/workspace/archive_patch](../../app/workspace/archive_patch.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```
