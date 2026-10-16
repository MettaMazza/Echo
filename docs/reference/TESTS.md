# Tests source reference

Generated 2026-10-16T01:50:38.957004+00:00. Exact hashes and lexical signatures; not a claim that every path was executed.

## tests/agent/test_attachment_loop.ep

[Source](../../tests/agent/test_attachment_loop.ep) · 86 lines · 5808 bytes

SHA-256: `8a65c93bc77c6cb7b2f5bf2563c4dc0e6e508bad8473f189270dadc2d50868ee`

> Restart the database connection before constructing the next request.

Imports: ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 7: `define main returning Int:`

## tests/agent/test_checked_generation.ep

[Source](../../tests/agent/test_checked_generation.ep) · 64 lines · 4336 bytes

SHA-256: `b1482f631832465ba2e7ba527445326f43a07d8f32eb098840c281cc431c959c`


Imports: ../../app/agent/checked_generation → app/agent/checked_generation.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define checked_reply with listener as List and method as Str and path as Str and request as Str and response as Str returning Int:`
- Line 12: `define checked_server with listener as List and body as Str and completed as Int returning Int:`
- Line 22: `define main returning Int:`

## tests/agent/test_execution_control.ep

[Source](../../tests/agent/test_execution_control.ep) · 71 lines · 4432 bytes

SHA-256: `e10f1ca807f3e78e3c9b26c2ee73793d587c168c08be5ef4200b7487b2f228d1`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define execution_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:`
- Line 24: `define main returning Int:`

## tests/agent/test_execution_loop.ep

[Source](../../tests/agent/test_execution_loop.ep) · 90 lines · 6132 bytes

SHA-256: `da895ef751f5345541aaddf141b63ea4992db6d5d25040082735368de496eee5`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define main returning Int:`

## tests/agent/test_generation.ep

[Source](../../tests/agent/test_generation.ep) · 54 lines · 3506 bytes

SHA-256: `d0af98356ef7898172af6257978d10393cdf25d73f31234fbe6a4164e7f068a3`


Imports: ../../app/agent/generation → app/agent/generation.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 4: `define generation_case with database as Int and listener as List and run as Str and request as Str and data as Str and expected as Str returning Int:`
- Line 26: `define main returning Int:`

## tests/agent/test_generation_recovery.ep

[Source](../../tests/agent/test_generation_recovery.ep) · 121 lines · 10813 bytes

SHA-256: `954acb96fe8f3ae9c0c79747cd84fd4bf35d60a95ef27fe9e1674dc35d89b23b`

> Intentionally stop between durable stream collection and publication.
> Repeated recovery must not republish or queue a second action.
> Synthetic crash prefixes: one lacks terminal evidence, one has a gap.

Imports: ../../app/agent/generation_recovery → app/agent/generation_recovery.ep, ../../app/web/recovery → app/web/recovery.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 6: `define recovery_capture_fixture with database as Int and listener as List and run as Str and delta as Str returning Int:`
- Line 24: `define main returning Int:`
- Line 110: `define recovery_missing_fixture with database as Int and run as Str and sequence as Int returning Int:`

## tests/agent/test_large_request_envelope.ep

[Source](../../tests/agent/test_large_request_envelope.ep) · 22 lines · 1134 bytes

SHA-256: `b8d5e8341e42088af58831d29e0863f264eb61696a3ef3f8ca0817b0facbe058`


Imports: ../../app/agent/generation → app/agent/generation.ep

- Line 3: `define main returning Int:`

## tests/agent/test_pdf_loop.ep

[Source](../../tests/agent/test_pdf_loop.ep) · 97 lines · 6976 bytes

SHA-256: `8a5a2a1d973fda260704689601f639f1806d100a08a855e3dafd8d07e842de00`

> Restart the database connection before constructing the next request.

Imports: ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 7: `define main returning Int:`

## tests/agent/test_provider_profile.ep

[Source](../../tests/agent/test_provider_profile.ep) · 34 lines · 2286 bytes

SHA-256: `2491d4c0da6ec7be2eb75a36b7dd9dfd308362dd83c6d579a9132c374819a197`


Imports: ../../app/agent/provider_profile → app/agent/provider_profile.ep

- Line 3: `define main returning Int:`

## tests/agent/test_run_state.ep

[Source](../../tests/agent/test_run_state.ep) · 100 lines · 6047 bytes

SHA-256: `22fa3442fd9b856bcf61e8caff560238101618d087b9ad18c3685a2620cdf484`

> Test identities are fixed ASCII fixtures, not unescaped external content.
> Forty is a test workload, not a production iteration setting.

Imports: ../../app/agent/run_state → app/agent/run_state.ep, fs → standard library or compiler dependency

- Line 5: `define run_fixture_event with database as Int and identity as Str and kind as Str and parent as Str returning Int:`
- Line 9: `define main returning Int:`

## tests/agent/test_runner.ep

[Source](../../tests/agent/test_runner.ep) · 40 lines · 2578 bytes

SHA-256: `08ce4a765e5c6a5e66b0716b9b10ded84e7bb07b699318b087e18542ffc9909b`

> A cancellation pending before execution changes durable state without a
> provider request; a restart does not continue the cancelled run.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 4: `define main returning Int:`

## tests/agent/test_tool_ledger.ep

[Source](../../tests/agent/test_tool_ledger.ep) · 58 lines · 3391 bytes

SHA-256: `3b86c2539df927885af6cebd46e5a4d4db948f0be37529e4d8db5ebab3e2204b`


Imports: ../../app/agent/tool_ledger → app/agent/tool_ledger.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/agent/test_two_turn_loop.ep

[Source](../../tests/agent/test_two_turn_loop.ep) · 98 lines · 6934 bytes

SHA-256: `9fd3c45d13dac184a259d7a5485ecaaebecbdbc7c4ca0ea467af1a5cfb06c66f`

> Restart the database connection before constructing the next request.

Imports: ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/web/run_store → app/web/run_store.ep, ../../app/security/tool_authority → app/security/tool_authority.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 7: `define main returning Int:`

## tests/agent/test_workspace_create_loop.ep

[Source](../../tests/agent/test_workspace_create_loop.ep) · 77 lines · 5496 bytes

SHA-256: `bd41032345466cb39ccbd322e7d98325e331cb057d2e7397a0ba07b5b1c3d6c2`

> Every subsequent request is constructed after reopening the archive.

Imports: ../../app/web/attachments → app/web/attachments.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 6: `define main returning Int:`

## tests/agent/test_workspace_directory_recovery.ep

[Source](../../tests/agent/test_workspace_directory_recovery.ep) · 61 lines · 4051 bytes

SHA-256: `9f34f1ac6b659a1c7bdae1587c534f1af6a272a211911572e53758bd22aeb439`

> This isolated fixture has no worker; recovery owns the sole connection.

Imports: ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 8: `define main returning Int:`

## tests/agent/test_workspace_file_recovery.ep

[Source](../../tests/agent/test_workspace_file_recovery.ep) · 60 lines · 4148 bytes

SHA-256: `096c330be2d5a2ab73c2a2ad3eb07046eabc573895cde0e434f1ac4749fe2c9e`

> This isolated fixture has no worker; recovery owns the sole connection.

Imports: ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 8: `define main returning Int:`

## tests/agent/test_workspace_loop.ep

[Source](../../tests/agent/test_workspace_loop.ep) · 110 lines · 7609 bytes

SHA-256: `554f12e53c65016f0e481ff22213af7b66e40c275bd54b6a38de9a90b4c3bb21`

> Change the source after observation. Reconstructed messages must use the
> archived observation, not silently reread today's workspace contents.
> Restart the database connection before constructing the next request.

Imports: ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 7: `define main returning Int:`

## tests/agent/verify_live_provider_loop.ep

[Source](../../tests/agent/verify_live_provider_loop.ep) · 51 lines · 3435 bytes

SHA-256: `5adceab06fd82f93eb4294b4a8a66182b96a0c5b54dcd60d6ce462c84b2714d5`

> Verify the recorded search-then-read qualification task, not model self-report.
> Expected counts belong to this test workload; the agent has no turn ceiling.

Imports: ../../app/agent/runner → app/agent/runner.ep, fs → standard library or compiler dependency

- Line 6: `define main returning Int:`

## tests/archive/measure_event_commit.ep

[Source](../../tests/archive/measure_event_commit.ep) · 36 lines · 1794 bytes

SHA-256: `4e55cc0260cb1a09389cfacd88bb4dd6b3c1ff101aed6ac7d9a624738f3687c0`

> Diagnostic workload only: measure existing phases without changing durability.

Imports: ../../app/archive/event_store → app/archive/event_store.ep, os → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/archive/probe_text_window.ep

[Source](../../tests/archive/probe_text_window.ep) · 10 lines · 560 bytes

SHA-256: `aeab005ddcbaa1a8868833f476a3f89ce9e1dcb943a1a1ba60043513c9e61254`


Imports: ../../app/archive/file_text → app/archive/file_text.ep

- Line 3: `define main returning Int:`

## tests/archive/test_append_readback.ep

[Source](../../tests/archive/test_append_readback.ep) · 34 lines · 2053 bytes

SHA-256: `9ea63352e0fad0868a59eadd2749f70ccd0fe7645664426dc0a6be4e68ef1fad`

> Fault injection applies only to a new disposable database supplied here.

Imports: ../../app/archive/event_store → app/archive/event_store.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/archive/test_attachment_snapshot.ep

[Source](../../tests/archive/test_attachment_snapshot.ep) · 33 lines · 2287 bytes

SHA-256: `3fd1ffc6e7ef0c99c4c735f095e0d866612bf441d8d372d35909270ed423a743`


Imports: ../../app/archive/attachment_snapshot → app/archive/attachment_snapshot.ep, ../../app/web/attachments → app/web/attachments.ep

- Line 4: `define main returning Int:`

## tests/archive/test_branch_export_file.ep

[Source](../../tests/archive/test_branch_export_file.ep) · 38 lines · 2300 bytes

SHA-256: `f6f7eb090b7a3ba55bbcfc69d5192f1e3b65da67d94bb00cbafe254d4e6023f1`


Imports: ../../app/archive/export_restore → app/archive/export_restore.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/archive/test_bundle_rejection.ep

[Source](../../tests/archive/test_bundle_rejection.ep) · 28 lines · 1503 bytes

SHA-256: `f9bd3d7a617adf82d135d411ea1b279225818c0d7dd2620eaba8344b3cdf6e75`


Imports: ../../app/archive/restore_bundle → app/archive/restore_bundle.ep

- Line 3: `define main returning Int:`

## tests/archive/test_digest_stress.ep

[Source](../../tests/archive/test_digest_stress.ep) · 68 lines · 2882 bytes

SHA-256: `2c7eff0c23d94eb8c999482c62a88a7685d8d06aec234cd532258486d1a410c8`

> Read-only diagnostic: compare repeated native hashes to an independently
> calculated expected value, including overlapping native worker allocations.

Imports: ../../app/archive/event_store → app/archive/event_store.ep, ../../app/providers/json_access → app/providers/json_access.ep

- Line 4: `define digest_parser_worker with rounds as Int and completed as Int returning Int:`
- Line 15: `define digest_stress_worker with row as List and expected as Str and rounds as Int and completed as Int returning Int:`
- Line 35: `define main returning Int:`

## tests/archive/test_event_store.ep

[Source](../../tests/archive/test_event_store.ep) · 58 lines · 3028 bytes

SHA-256: `4360e6ea6658a41f28426831217904d11aa00a1b4858979d4513ef909081e87a`

> Each invocation receives a disposable database path. Reopen verifies persistence.
> Corrupt only this disposable database to exercise the independent verifier.

Imports: ../../app/archive/event_store → app/archive/event_store.ep

- Line 4: `define main returning Int:`

## tests/archive/test_export_restore.ep

[Source](../../tests/archive/test_export_restore.ep) · 35 lines · 2057 bytes

SHA-256: `f4b40081d996736aaf3bd84ccc0f695d14962f8a7c63698c8c2d3df9398d98c9`


Imports: ../../app/archive/export_restore → app/archive/export_restore.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/archive/test_failed_run_bundle.ep

[Source](../../tests/archive/test_failed_run_bundle.ep) · 42 lines · 2880 bytes

SHA-256: `9718a7c12b3030425e83d59fff78c296a217c3ce08a84e63b0b834d39863477b`


Imports: ../../app/archive/restore_bundle → app/archive/restore_bundle.ep, ../../app/workspace/process_journal → app/workspace/process_journal.ep

- Line 4: `define main returning Int:`

## tests/archive/test_file_base64.ep

[Source](../../tests/archive/test_file_base64.ep) · 49 lines · 2620 bytes

SHA-256: `db87f4819a821631c2409d80a964b5bc7e1f6e49c72c461f4b4bfd074e0d507e`


Imports: ../../app/archive/file_base64 → app/archive/file_base64.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/archive/test_file_copy.ep

[Source](../../tests/archive/test_file_copy.ep) · 23 lines · 1194 bytes

SHA-256: `1c71192168830f09dfac62be28cb59226468c408b9643c5dd98717c097c7a042`


Imports: ../../app/archive/file_copy → app/archive/file_copy.ep

- Line 3: `define main returning Int:`

## tests/archive/test_file_digest.ep

[Source](../../tests/archive/test_file_digest.ep) · 17 lines · 1160 bytes

SHA-256: `5d848ad77a79e19507e7420d51f21860d82c1c4c19896e525668ffd8c26b27e2`


Imports: ../../app/archive/file_digest → app/archive/file_digest.ep

- Line 3: `define main returning Int:`

## tests/archive/test_file_stage.ep

[Source](../../tests/archive/test_file_stage.ep) · 29 lines · 1612 bytes

SHA-256: `d4c3c9ec7d0cd817ab211c7169f2154b7a04bd0581cc8108ab92dfaa5231943a`

> Packed native bytes: 00 01 ff 00 61 62 63 0a (NUL/high-byte fixture).

Imports: ../../app/archive/file_stage → app/archive/file_stage.ep

- Line 3: `define main returning Int:`

## tests/archive/test_file_text.ep

[Source](../../tests/archive/test_file_text.ep) · 59 lines · 3878 bytes

SHA-256: `4869bdf779d5f7d8ad8b1516fb4166c3e4e2fdf37efd21793acf07edcd9bdc8d`


Imports: ../../app/archive/file_text → app/archive/file_text.ep, fs → standard library or compiler dependency, ../../app/archive/file_stage → app/archive/file_stage.ep

- Line 5: `define main returning Int:`

## tests/archive/test_history.ep

[Source](../../tests/archive/test_history.ep) · 73 lines · 3651 bytes

SHA-256: `534c427b55d478f2845e5837cd7f6074e0bd00b957fd6f0bb41228635170befa`

> Three characters is an adversarial test fixture, never a runtime cap.

Imports: ../../app/archive/history → app/archive/history.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/archive/test_pdf_cache.ep

[Source](../../tests/archive/test_pdf_cache.ep) · 44 lines · 3142 bytes

SHA-256: `852ab9529fd579b5e6778e94273ea021f12934779bbe99a55376be0f67007131`


Imports: ../../app/archive/pdf_cache → app/archive/pdf_cache.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/archive/test_pdf_text.ep

[Source](../../tests/archive/test_pdf_text.ep) · 30 lines · 1935 bytes

SHA-256: `2241109e4a0daa8873bb78bf4d5ee344792da46dc200bab01cad6575f624333b`


Imports: ../../app/archive/pdf_text → app/archive/pdf_text.ep

- Line 3: `define main returning Int:`

## tests/archive/test_restore_publication.ep

[Source](../../tests/archive/test_restore_publication.ep) · 28 lines · 1488 bytes

SHA-256: `5d8b218871156165444e281ae7770a5a052976759a53124cea990136f653daba`


Imports: ../../app/archive/attachment_restore → app/archive/attachment_restore.ep

- Line 3: `define main returning Int:`

## tests/archive/test_restore_release_hold.ep

[Source](../../tests/archive/test_restore_release_hold.ep) · 26 lines · 2273 bytes

SHA-256: `5ab48de92c17c374ed6157384f8f470a7a284221398cca9cfcc142a69fff3f2e`


Imports: ../../app/archive/restore_release → app/archive/restore_release.ep

- Line 3: `define main returning Int:`

## tests/archive/test_restored_files.ep

[Source](../../tests/archive/test_restored_files.ep) · 25 lines · 1472 bytes

SHA-256: `9c2547138ad6198fca4abe534b34fdda34696cb03ebef58e3cd8a1fc32d2c904`


Imports: ../../app/archive/attachment_restore → app/archive/attachment_restore.ep

- Line 3: `define main returning Int:`

## tests/archive/test_snapshot.ep

[Source](../../tests/archive/test_snapshot.ep) · 61 lines · 2890 bytes

SHA-256: `431e13d0c28305acd51b39d3d6a905cea2c0e234c0ab88c5620aeb62fcd6719b`

> Deliberately damage only the disposable test snapshot.

Imports: ../../app/archive/snapshot → app/archive/snapshot.ep

- Line 3: `define main returning Int:`

## tests/archive/test_tree_snapshot.ep

[Source](../../tests/archive/test_tree_snapshot.ep) · 62 lines · 3853 bytes

SHA-256: `ba1424b53ecff1b13015e5a1ccbd239b6758039cb6568412a28cd4f7e2ba3e38`


Imports: ../../app/archive/file_copy → app/archive/file_copy.ep, fs → standard library or compiler dependency, ../../app/archive/tree_snapshot → app/archive/tree_snapshot.ep, ../../app/workspace/create_text → app/workspace/create_text.ep

- Line 6: `define main returning Int:`

## tests/context/seed_recap_viewer.ep

[Source](../../tests/context/seed_recap_viewer.ep) · 33 lines · 2563 bytes

SHA-256: `e032c24cdb631526297c1c89fbef7d8d363dfcaec931dd2b2fffb91ab9737436`

> Host-only UI fixture: create a NEW explicitly labelled conversation for an
> existing test device. Never alter or compact an existing conversation.

Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/control → app/agent/control.ep

- Line 6: `define main returning Int:`

## tests/context/test_attachment_gate.ep

[Source](../../tests/context/test_attachment_gate.ep) · 78 lines · 5295 bytes

SHA-256: `a53c108aabceb95f877a5d50c3ee7b6d0a20051f2cc09c3fe4004decaad02ce6`

> A branch inherits only its frozen ancestor prefix.
> Repairing the original removes the failure without deleting history.
> A prefix before the attachment is unaffected by the later missing file.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../../app/web/attachments → app/web/attachments.ep

- Line 4: `define main returning Int:`

## tests/context/test_automatic_compaction.ep

[Source](../../tests/context/test_automatic_compaction.ep) · 104 lines · 7363 bytes

SHA-256: `02225dc2a4bb49d1e9e5c218bbf540459529f61fcc259a213062f2cc4cadd620`

> Reopen application storage while the same provider endpoint remains available.
> A retry may not silently change its persisted provider endpoint.

Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 3: `define main returning Int:`

## tests/context/test_budget.ep

[Source](../../tests/context/test_budget.ep) · 39 lines · 1937 bytes

SHA-256: `5a7cc4af3e8f2979279df201911ea468674cab1afd1c3ac339d547c268db87f2`


Imports: ../../app/context/budget → app/context/budget.ep

- Line 3: `define main returning Int:`

## tests/context/test_checkpoint.ep

[Source](../../tests/context/test_checkpoint.ep) · 85 lines · 4452 bytes

SHA-256: `5c4800817231cbf55afc7dbd10150bbf51712f843f3bba02500a39e462e078c4`

> Close and reopen between preparation and activation, as after a restart.

Imports: ../../app/context/checkpoint → app/context/checkpoint.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/context/test_checkpoint_interruption.ep

[Source](../../tests/context/test_checkpoint_interruption.ep) · 37 lines · 1504 bytes

SHA-256: `a2a69730eb13c321cab88eef6361f40d9c990419d120f640506f495e6654437d`

> Run after test_checkpoint. Interrupt changes only its disposable database.
> SIGKILL prevents normal shutdown and SQLite connection cleanup.

Imports: ../../app/context/checkpoint → app/context/checkpoint.ep, os → standard library or compiler dependency, fs → standard library or compiler dependency

- Line 6: `define main returning Int:`

## tests/context/test_compaction_crash.ep

[Source](../../tests/context/test_compaction_crash.ep) · 66 lines · 4625 bytes

SHA-256: `1be4719f6fa5ed80f6550cc3df22fc4f44a744ca1d2da704eedcf806a7690c3e`


Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 3: `define main returning Int:`

## tests/context/test_compaction_lock.ep

[Source](../../tests/context/test_compaction_lock.ep) · 32 lines · 1590 bytes

SHA-256: `7969c47bcc6ee191d97ab99b2abfcd23f349681e69b18651b96f4874ed3a4618`


Imports: ../../app/context/compaction_lock → app/context/compaction_lock.ep, os → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/context/test_compaction_read.ep

[Source](../../tests/context/test_compaction_read.ep) · 59 lines · 4567 bytes

SHA-256: `9afabe06756656544aba5f3dfa9ad9012a89b2b31ddd9724f84d4e497e0fa836`


Imports: ../../app/web/compaction → app/web/compaction.ep, ../../app/web/run_store → app/web/run_store.ep

- Line 4: `define recap_read_field with body as Str and name as Str returning Str:`
- Line 8: `define main returning Int:`

## tests/context/test_merge_resume.ep

[Source](../../tests/context/test_merge_resume.ep) · 100 lines · 7284 bytes

SHA-256: `6925061d547a337ac4628af9d99a6788cccce32b374e450742ffb9b28243bee7`

> This stray note is not a manifest member and must never be processed.
> A repeated first checkpoint cannot masquerade as additional coverage.

Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 3: `define merge_note_fixture with database as Int and identity as Str and start as Int and finish as Int and notes as Str returning Int:`
- Line 11: `define merge_provider_fixture with listener as List and completed as Int and start as Int and prior as Str and finish as Int returning Int:`
- Line 31: `define main returning Int:`

## tests/context/test_progressive_merge.ep

[Source](../../tests/context/test_progressive_merge.ep) · 62 lines · 4466 bytes

SHA-256: `9e4b7650e28a479aee187878ed8139752dad2800fcf9fbd14e2da743a6cf5d47`

> This stray note is not a manifest member and must never be processed.

Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 3: `define merge_note_fixture with database as Int and identity as Str and start as Int and finish as Int and notes as Str returning Int:`
- Line 11: `define merge_provider_fixture with listener as List and completed as Int returning Int:`
- Line 28: `define main returning Int:`

## tests/context/test_prompt_prefix.ep

[Source](../../tests/context/test_prompt_prefix.ep) · 16 lines · 729 bytes

SHA-256: `a763e409b77c43805b338665c779d668efa891ec998713b9e4c89f905d8c79d6`


Imports: ../../app/context/system_awareness → app/context/system_awareness.ep

- Line 3: `define main returning Int:`

## tests/context/test_recap_collect.ep

[Source](../../tests/context/test_recap_collect.ep) · 78 lines · 5340 bytes

SHA-256: `e435fdae69c9a8beb9262a9fedc8d54f28a185f1b4464f0334d436b1d5b471c4`

> The scripted answer is plain notes, not the recap contract. It must be
> archived as invalid and must not replace active memory.
> No provider is available. A covered job must return without another call.

Imports: ../../app/context/recap_collect → app/context/recap_collect.ep, ../../app/context/recap_merge → app/context/recap_merge.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 6: `define collect_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:`
- Line 13: `define collect_provider with listener as List and bodies as List and completed as Int returning Int:`
- Line 28: `define main returning Int:`

## tests/context/test_recap_contract.ep

[Source](../../tests/context/test_recap_contract.ep) · 23 lines · 2277 bytes

SHA-256: `f7f16aa63b4b858196c6ed37fea21577bcd3193b5c5a38f607b604909646279a`


Imports: ../../app/context/recap_contract → app/context/recap_contract.ep

- Line 3: `define main returning Int:`

## tests/context/test_recap_fragment.ep

[Source](../../tests/context/test_recap_fragment.ep) · 56 lines · 3736 bytes

SHA-256: `6d0915fcdbfa6d6a934151bbf2c847d0f5f39795e0edb425c8c25b8d7c481e80`


Imports: ../../app/context/recap_fragment → app/context/recap_fragment.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep

- Line 4: `define fragment_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:`
- Line 11: `define fragment_provider with listener as List and full as Str and half as Str and completed as Int returning Int:`
- Line 27: `define main returning Int:`

## tests/context/test_recap_fragment_review.ep

[Source](../../tests/context/test_recap_fragment_review.ep) · 122 lines · 10911 bytes

SHA-256: `c8f207f8296d464168a36e6b1f6e49b0325083425179ee2dc6ea19e5683b848e`


Imports: ../../app/context/recap_review_synthesis → app/context/recap_review_synthesis.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define review_fragment_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:`
- Line 12: `define review_fragment_provider with listener as List and full as Str and half as Str and report as Str and completed as Int returning Int:`
- Line 32: `define main returning Int:`
- Line 113: `define review_remainder_provider with listener as List and body as Str and report as Str and completed as Int returning Int:`

## tests/context/test_recap_generate.ep

[Source](../../tests/context/test_recap_generate.ep) · 122 lines · 10832 bytes

SHA-256: `bd0b5c84060e57ad77f3019fdf08c0cc89a4e01e42506b0ccc4a459839b0a27a`


Imports: ../../app/context/recap_accept → app/context/recap_accept.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define recap_fixture_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:`
- Line 12: `define recap_fixture with listener as List and body as Str and candidate as Str and completed as Int returning Int:`
- Line 23: `define acceptance_fixture with listener as List and body as Str and counted as Str and change_path as Str and completed as Int returning Int:`
- Line 38: `define acceptance_expected_body with database as Int and identity as Str and candidate as Str returning Str:`
- Line 49: `define main returning Int:`

## tests/context/test_recap_jobs.ep

[Source](../../tests/context/test_recap_jobs.ep) · 49 lines · 3527 bytes

SHA-256: `57e4876c6aef31f7b154564ed7c814af68ffa74635c29cf9fc54bf3a049b8d0a`

> Synthetic notes isolate range-accounting tests; they are not model evaluation.

Imports: ../../app/context/recap_jobs → app/context/recap_jobs.ep

- Line 4: `define fixture_note with database as Int and identity as Str and job as Str and digest as Str and start as Int and finish as Int returning Int:`
- Line 11: `define main returning Int:`

## tests/context/test_recap_notes.ep

[Source](../../tests/context/test_recap_notes.ep) · 59 lines · 4276 bytes

SHA-256: `2be7fadc4c3e4667794f46fbe216dc6e52772a241ec17a3e09ee0d944ab02081`


Imports: ../../app/context/recap_notes → app/context/recap_notes.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define notes_reply with listener as List and method as Str and path as Str and body as Str and response as Str returning Int:`
- Line 12: `define notes_provider with listener as List and body as Str and reason as Str and completed as Int returning Int:`
- Line 23: `define main returning Int:`

## tests/context/test_recap_rebuild.ep

[Source](../../tests/context/test_recap_rebuild.ep) · 100 lines · 6376 bytes

SHA-256: `58cd952d6705e8fb83f663fafe282405d67930351f72809beb45ff4d14eefc3e`

> Start beyond the fixture's artificial capacity trigger: this test must
> rebuild because of an explicit request while ordinary context still fits.

Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep, ../../app/web/run_store → app/web/run_store.ep

- Line 5: `define rebuild_provider_fixture with listener as List and objective as Str and completed as Int and start as Int and finish as Int returning Int:`
- Line 22: `define main returning Int:`

## tests/context/test_recap_review.ep

[Source](../../tests/context/test_recap_review.ep) · 28 lines · 2116 bytes

SHA-256: `b92d3b3a0a7dda5e926320796aab01bf7d3552d0516b1f5ffbf96599fdcb4d24`


Imports: ../../app/context/recap_review → app/context/recap_review.ep

- Line 3: `define main returning Int:`

## tests/context/test_recap_review_coverage.ep

[Source](../../tests/context/test_recap_review_coverage.ep) · 62 lines · 5564 bytes

SHA-256: `af71f14eea99f196011354f1f538cf3c9524619a8507a79d6656cb4d6beca747`

> Synthetic receipts isolate range accounting from model judgment quality.

Imports: ../../app/context/recap_review_coverage → app/context/recap_review_coverage.ep

- Line 4: `define coverage_review_fixture with database as Int and job as Str and identity as Str and candidate as Str and start as Int and finish as Int and verdict as Str returning Int:`
- Line 21: `define main returning Int:`

## tests/context/test_recap_sources.ep

[Source](../../tests/context/test_recap_sources.ep) · 53 lines · 3433 bytes

SHA-256: `03561b933e2a5f0b221ddbe8e0e52886b85be12073c03f62116b9c6c3f22f920`


Imports: ../../app/context/recap_sources → app/context/recap_sources.ep, ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 4: `define main returning Int:`

## tests/context/test_recap_synthesis_gate.ep

[Source](../../tests/context/test_recap_synthesis_gate.ep) · 70 lines · 6575 bytes

SHA-256: `7f0b6cfda0c7c5f0fa795b17f38f98e6703aa2dc9aad813e6f9f86fe4d5d6b01`

> Synthetic receipts isolate range accounting from model judgment quality.

Imports: ../../app/context/recap_accept → app/context/recap_accept.ep

- Line 4: `define coverage_review_fixture with database as Int and job as Str and identity as Str and candidate as Str and start as Int and finish as Int and verdict as Str returning Int:`
- Line 22: `define synthesis_receipt_fixture with job as Str and candidate as Str returning Str:`
- Line 29: `define main returning Int:`

## tests/context/test_session_messages.ep

[Source](../../tests/context/test_session_messages.ep) · 71 lines · 5163 bytes

SHA-256: `49c5b6cf689e5ca1eb0487549314e22dbe2944bffc0da9ec6deca7cf7953b888`


Imports: ../../app/context/session_messages → app/context/session_messages.ep

- Line 3: `define main returning Int:`

## tests/fixtures/generation_fixture.ep

[Source](../../tests/fixtures/generation_fixture.ep) · 33 lines · 1549 bytes

SHA-256: `6172bf8e25398f85c4682c6da403ea26309523a7cab5b1d2dd0fccb17c952a8c`


Imports: ../../app/agent/generation → app/agent/generation.ep

- Line 3: `define generation_fixture with listener as List and data as Str and completed as Int returning Int:`

## tests/fixtures/json_fixture.ep

[Source](../../tests/fixtures/json_fixture.ep) · 38 lines · 2029 bytes

SHA-256: `b863ae11eba2182b535f50e81b667f0cb0865f3bd2ce1c2a41c871a7fcb4b02f`


Imports: ../../app/providers/local_json → app/providers/local_json.ep

- Line 3: `define json_fixture with listener as List and expected_method as Str and expected_path as Str and expected_body as Str and response as Str and completed as Int returning Int:`

## tests/fixtures/runner_provider.ep

[Source](../../tests/fixtures/runner_provider.ep) · 301 lines · 20096 bytes

SHA-256: `0ea744f4fd87e9dedbba0c237e80f06068cdd558889275ca971d48a2ce88fde5`

> Three scripted generations: recap, independent review, resumed final answer.
> Only this fixture's first token count reports full capacity.
> Kill while activation is asking for metadata, after candidate and
> passing review have committed and while compaction owns its lock.
> This fixture ends after its scripted final answer; the application runner
> itself decides when to make requests and contains no turn-count limit.
> Start at review: the candidate must come from the durable archive.

Imports: ../../app/agent/runner → app/agent/runner.ep, os → standard library or compiler dependency

- Line 6: `define compaction_fixture_content with state as List and tokens as List and messages as List returning Str:`
- Line 104: `define runner_fixture_call with name as Str and identity as Str and arguments as Str returning Str:`
- Line 109: `define runner_fixture_exchange with listener as List and state as List returning Int:`
- Line 228: `define runner_fixture with listener as List and objective as Str and completed as Int returning Int:`
- Line 248: `define compaction_fixture with listener as List and mode as Int and completed as Int returning Int:`
- Line 279: `define compaction_recovery_fixture with listener as List and candidate as Str and endpoint as Str and review_saved as Int and completed as Int returning Int:`

## tests/integration/test_component_imports.ep

[Source](../../tests/integration/test_component_imports.ep) · 16 lines · 624 bytes

SHA-256: `7e0ddf3ea77f1cc3ffbcdecb2bc7e3714061b9ea0cfb674af71a6d57a397a240`


Imports: ../../app/agent/run_state → app/agent/run_state.ep, ../../app/archive/history → app/archive/history.ep, ../../app/archive/snapshot → app/archive/snapshot.ep, ../../app/context/budget → app/context/budget.ep, ../../app/transport/http_stream → app/transport/http_stream.ep, ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 8: `define main returning Int:`

## tests/lifecycle/test_migration_interruption.ep

[Source](../../tests/lifecycle/test_migration_interruption.ep) · 43 lines · 2186 bytes

SHA-256: `3e76ad0cee7981f4e64aad6a35a8832238288e41e015347cb3e60fa3b0c0be14`

> Deliberately long disposable SQL, so the driver kills a live native
> migration after observing its on-disk rollback journal.

Imports: ../../app/lifecycle/migrations → app/lifecycle/migrations.ep, ../../app/archive/snapshot → app/archive/snapshot.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/lifecycle/test_migrations.ep

[Source](../../tests/lifecycle/test_migrations.ep) · 45 lines · 2907 bytes

SHA-256: `0398e439c471cad3cf181f8a53c44e46c474ccda0c3605cff6fa79bb53fa5284`

> A failing second statement must undo both earlier DDL and the receipt table.

Imports: ../../app/lifecycle/migrations → app/lifecycle/migrations.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/lifecycle/test_operations.ep

[Source](../../tests/lifecycle/test_operations.ep) · 54 lines · 3788 bytes

SHA-256: `169df1237f8869713a8d006e510deddae7b16e4838cccd7740239ef37bc9cca8`

> A failure to persist an outcome cannot consume the current revision.

Imports: ../../app/lifecycle/operations → app/lifecycle/operations.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/lifecycle/test_service.ep

[Source](../../tests/lifecycle/test_service.ep) · 61 lines · 3631 bytes

SHA-256: `56e8550d1901364f89cd0d15d3e456f32ab6af68ec90a777de5190ee57ac4c4e`


Imports: ../../app/lifecycle/service_profile → app/lifecycle/service_profile.ep, ../../app/workspace/native_process → app/workspace/native_process.ep, ../../app/providers/archived_stream → app/providers/archived_stream.ep, fs → standard library or compiler dependency

- Line 6: `define main returning Int:`

## tests/lifecycle/test_startup.ep

[Source](../../tests/lifecycle/test_startup.ep) · 26 lines · 1382 bytes

SHA-256: `ce95ffad0d27da594ca35b6e143f886a97b616bd71b2a42833dd1d41ba293f94`


Imports: ../../app/lifecycle/startup → app/lifecycle/startup.ep

- Line 3: `define main returning Int:`

## tests/lifecycle/test_startup_busy.ep

[Source](../../tests/lifecycle/test_startup_busy.ep) · 32 lines · 1321 bytes

SHA-256: `7e579bad744b65497e57b5563c0c50849221241f40800905fd40b36bd33ab1f5`


Imports: ../../app/lifecycle/startup → app/lifecycle/startup.ep, select → standard library or compiler dependency

- Line 4: `define startup_wait_fixture with path as Str and completed as Int returning Int:`
- Line 9: `define main returning Int:`

## tests/media/image_wait_fixture.ep

[Source](../../tests/media/image_wait_fixture.ep) · 7 lines · 175 bytes

SHA-256: `b01a84179c4c0eb2e030889b63e1fe1421fb4a53b7e5a1e391cb352339c81c09`


Imports: ../../app/providers/helper_process → app/providers/helper_process.ep

- Line 3: `define main returning Int:`

## tests/media/test_image_control.ep

[Source](../../tests/media/test_image_control.ep) · 75 lines · 4971 bytes

SHA-256: `de4ba3205b29f15acc30d3c222afa71326103367bdc5a17d67134ecc18da276d`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define image_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:`
- Line 24: `define main returning Int:`

## tests/media/test_image_logs.ep

[Source](../../tests/media/test_image_logs.ep) · 70 lines · 4675 bytes

SHA-256: `c8373c3564e811b1274d832bf95f9f3c9b39f8aef3c804763d524975e342e433`

> The completed stream cannot silently be duplicated.
> Reject the second page: retain the first page, never advertise EOF.

Imports: ../../app/media/image_logs → app/media/image_logs.ep, ../../app/archive/snapshot → app/archive/snapshot.ep

- Line 4: `define main returning Int:`

## tests/media/test_image_request.ep

[Source](../../tests/media/test_image_request.ep) · 20 lines · 1851 bytes

SHA-256: `8c773a1cdc0ebefa79a9d481697ae420c74f923c6bb55d0df6eb2d91924aa5b0`


Imports: ../../app/media/image_request → app/media/image_request.ep

- Line 3: `define main returning Int:`

## tests/media/test_image_tool.ep

[Source](../../tests/media/test_image_tool.ep) · 52 lines · 3855 bytes

SHA-256: `3128ecff7196261a5c3d797c59a3112c7572e6f44e0166c0341af949e330987e`


Imports: ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 4: `define main returning Int:`

## tests/media/test_message_speech.ep

[Source](../../tests/media/test_message_speech.ep) · 38 lines · 2522 bytes

SHA-256: `351ecf657aea7421cb2d6a5f2eed80f3bd6e29d6138ff518206565f9f58fd4a4`

> Real Kokoro process, isolated archive; no model invocation is needed.

Imports: ../../app/web/message_speech → app/web/message_speech.ep

- Line 4: `define main returning Int:`

## tests/media/test_speech_control.ep

[Source](../../tests/media/test_speech_control.ep) · 77 lines · 5184 bytes

SHA-256: `41e847775826f8b93e0322151af6df437bbd3a19945e8f41b7d3e1f36966cb94`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define speech_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:`
- Line 24: `define main returning Int:`

## tests/media/test_speech_text.ep

[Source](../../tests/media/test_speech_text.ep) · 21 lines · 1290 bytes

SHA-256: `bbfed6c547f79897199ef445b1f7626e07df509ab5b807cfdc35c1c8de8de3e0`


Imports: ../../app/media/speech_text → app/media/speech_text.ep

- Line 3: `define main returning Int:`

## tests/media/test_speech_tool.ep

[Source](../../tests/media/test_speech_tool.ep) · 67 lines · 4911 bytes

SHA-256: `a5a266f73568191c2142680f774412d4ba3eff3eaf86d7313b8fefe2dde419ed`


Imports: ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 4: `define main returning Int:`

## tests/media/test_voice_control.ep

[Source](../../tests/media/test_voice_control.ep) · 75 lines · 5092 bytes

SHA-256: `a41666797114619bf8127fd6aa3a1578f7610e9fab5285b37bcdd708ad68b0b0`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define speech_interrupt_fixture with path as Str and mode as Str and writer as Int and completed as Int returning Int:`
- Line 24: `define main returning Int:`

## tests/media/test_voice_tool.ep

[Source](../../tests/media/test_voice_tool.ep) · 66 lines · 5553 bytes

SHA-256: `3963a1ecf4edd07ffe90b96192d7f7873c69a53039a8ae84752168a11c429410`

> Script only the model request. Both synthesis and recognition are real.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 5: `define voice_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:`
- Line 21: `define main returning Int:`

## tests/providers/measure_recorded_stream.ep

[Source](../../tests/providers/measure_recorded_stream.ep) · 84 lines · 4210 bytes

SHA-256: `042ee072536378501a9b4d9916a636154b9320a06a343e6ff2c4c8a7ced8afad`


Imports: ../../app/providers/archived_stream → app/providers/archived_stream.ep, ../../app/transport/socket_write → app/transport/socket_write.ep, fs → standard library or compiler dependency, os → standard library or compiler dependency, ../../app/transport/cooperative_accept → app/transport/cooperative_accept.ep, ../../app/web/cancellation → app/web/cancellation.ep

- Line 8: `define event_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:`
- Line 17: `define measurement_idle_listener with listener as List and completed as Int returning Int:`
- Line 23: `define main returning Int:`

## tests/providers/measure_stream_readers.ep

[Source](../../tests/providers/measure_stream_readers.ep) · 129 lines · 6384 bytes

SHA-256: `7dda47e4ca151d24e8b6b7e43f80fe04016156b7ae30645e19f6b6189299ad6c`

> Match browser requests: each read opens and closes its own connection.

Imports: ../../app/providers/archived_stream → app/providers/archived_stream.ep, ../../app/transport/socket_write → app/transport/socket_write.ep, fs → standard library or compiler dependency, os → standard library or compiler dependency, ../../app/transport/cooperative_accept → app/transport/cooperative_accept.ep, ../../app/web/cancellation → app/web/cancellation.ep, ../../app/web/progress → app/web/progress.ep

- Line 9: `define event_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:`
- Line 18: `define measurement_idle_listener with listener as List and completed as Int returning Int:`
- Line 25: `define measurement_progress_reader with path as Str and completed as Int returning Int:`
- Line 51: `define main returning Int:`

## tests/providers/test_archived_stream.ep

[Source](../../tests/providers/test_archived_stream.ep) · 88 lines · 5124 bytes

SHA-256: `81bca0dca4aa664a94678ef120437fcde7a53d451a5b494358e4f1fb1d3eab23`

> Reusing an attempt ID must fail at the first archive commit, before its
> repeated content can enter a fresh assembler or become duplicate work.

Imports: ../../app/providers/archived_stream → app/providers/archived_stream.ep, ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 4: `define archived_fixture_sender with library as Int and descriptor as Int and completed as Int returning Int:`
- Line 16: `define main returning Int:`

## tests/providers/test_attachment_image.ep

[Source](../../tests/providers/test_attachment_image.ep) · 66 lines · 4730 bytes

SHA-256: `002553edcd25cc69e4d636d4d00b70038f1fcf5e0f94b857fa7c1a35b420b2cb`


Imports: fs → standard library or compiler dependency, ../../app/providers/attachment_image → app/providers/attachment_image.ep, ../../app/web/attachments → app/web/attachments.ep

- Line 5: `define main returning Int:`

## tests/providers/test_chat_stream.ep

[Source](../../tests/providers/test_chat_stream.ep) · 54 lines · 3993 bytes

SHA-256: `9beaef817c568b01e442716a3754f1999d5919c747e487d9ba4727ec8649e7fd`


Imports: ../../app/providers/chat_stream → app/providers/chat_stream.ep

- Line 3: `define rejected_finish with reason as Str and transport as Str and done as Int returning Int:`
- Line 15: `define main returning Int:`

## tests/providers/test_context_admission.ep

[Source](../../tests/providers/test_context_admission.ep) · 37 lines · 2567 bytes

SHA-256: `452c3c4497add7b76bc5149904b254ce291a9be96b00837ad669ffe5eb7200d0`


Imports: ../../app/providers/llama_preflight → app/providers/llama_preflight.ep

- Line 3: `define admission_status with loaded as Int and supported as Int and input as Int and ceiling as Int and reserve as Int returning Str:`
- Line 7: `define main returning Int:`

## tests/providers/test_json_tokens.ep

[Source](../../tests/providers/test_json_tokens.ep) · 52 lines · 2517 bytes

SHA-256: `3c476ad2db92df2ea4afb6f0e04d367a121048cf8795e0139558f0b616cdff70`


Imports: ../../app/providers/json_tokens → app/providers/json_tokens.ep

- Line 3: `define main returning Int:`

## tests/providers/test_live_context_admission.ep

[Source](../../tests/providers/test_live_context_admission.ep) · 34 lines · 1813 bytes

SHA-256: `ccecd480c434d1c6b6af641f5af84bf05b6cfe2ae14f09515cfcd580eb685f10`

> Recount a real archived request without generation or a synthetic tokenizer.

Imports: ../../app/providers/llama_preflight → app/providers/llama_preflight.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/providers/test_live_image_agent.ep

[Source](../../tests/providers/test_live_image_agent.ep) · 57 lines · 4022 bytes

SHA-256: `9c2e8cbc81db42ec8c6da6f3e01c3f1f1d2fd57fa141e64a50f7413cc33e4769`

> Actual archived attachment, full agent, provider counting and reopened history.
> The image is a historical UI screenshot, not the current application status.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency

- Line 5: `define image_answer with database as Int and run as Str and profile as Str returning Str:`
- Line 19: `define main returning Int:`

## tests/providers/test_live_image_compaction.ep

[Source](../../tests/providers/test_live_image_compaction.ep) · 69 lines · 5094 bytes

SHA-256: `8a0dc47670834cac75a4de04322a4456b9fd737e9afc2cd08aa7973f504085dd`

> Actual archived attachment, full agent, provider counting and reopened history.
> The image is a historical UI screenshot, not the current application status.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency

- Line 5: `define image_answer with database as Int and run as Str and profile as Str returning Str:`
- Line 19: `define main returning Int:`

## tests/providers/test_live_image_retrieval.ep

[Source](../../tests/providers/test_live_image_retrieval.ep) · 57 lines · 4120 bytes

SHA-256: `4e76314e247a400f6f7d34579bf52d0c3766d642101edef06cb52506893d3975`

> Actual archived attachment, full agent, provider counting and reopened history.
> The image is a historical UI screenshot, not the current application status.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency

- Line 5: `define image_answer with database as Int and run as Str and profile as Str returning Str:`
- Line 19: `define main returning Int:`

## tests/providers/test_live_provider_switch.ep

[Source](../../tests/providers/test_live_provider_switch.ep) · 73 lines · 5399 bytes

SHA-256: `174b0d5a26a182bc8df202603982d9fcbc73bbb3471a33371e2cf0b7e21502be`

> The operator supervises the real loading process. This is a polling
> interval, not a provider deadline or a model context/output limit.
> Two genuinely different local runtimes, three turns in one archive, storage
> reopened between turns. Models must apply a correction after switching back.

Imports: ../../app/agent/runner → app/agent/runner.ep, fs → standard library or compiler dependency

- Line 4: `define switch_wait_provider with profile as Str returning Int:`
- Line 20: `define switch_turn with database as Int and run as Str and prompt as Str and profile as Str and expected as Str and reserve as Int and batch as Int returning Int:`
- Line 41: `define main returning Int:`

## tests/providers/test_live_unified_image.ep

[Source](../../tests/providers/test_live_unified_image.ep) · 38 lines · 2550 bytes

SHA-256: `d896a430ef04bb4c4e28eea5ff3c9655f554a57c940a0ad7af8a2b7472e8e021`

> Live regression: one selected multimodal model answers actual image pixels
> without recording or switching to another provider.

Imports: ../../app/agent/runner → app/agent/runner.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency

- Line 7: `define main returning Int:`

## tests/providers/test_llama_metadata.ep

[Source](../../tests/providers/test_llama_metadata.ep) · 25 lines · 2118 bytes

SHA-256: `df7e0a7ca0365c6e72560c23f4d0084f81d08a3a30fa640e7cda73e86f11429c`


Imports: ../../app/providers/llama_metadata → app/providers/llama_metadata.ep

- Line 3: `define main returning Int:`

## tests/providers/test_llama_modalities.ep

[Source](../../tests/providers/test_llama_modalities.ep) · 19 lines · 1855 bytes

SHA-256: `2fce4f8d49bc9f8480d027ff090f85f304895c3f1b1c47468690d6e76c323d16`


Imports: ../../app/providers/llama_modalities → app/providers/llama_modalities.ep

- Line 3: `define main returning Int:`

## tests/providers/test_local_chat.ep

[Source](../../tests/providers/test_local_chat.ep) · 81 lines · 4709 bytes

SHA-256: `8eaee52529ccc5c912817093d7199e3e8ff461d56ef7d6cc76e49de58612d3c7`


Imports: ../../app/providers/local_chat → app/providers/local_chat.ep

- Line 3: `define chat_fixture_server with listener as List and expected as Str and completed as Int returning Int:`
- Line 46: `define main returning Int:`

## tests/providers/test_local_json.ep

[Source](../../tests/providers/test_local_json.ep) · 33 lines · 2252 bytes

SHA-256: `4006a534956c8cf88bdf4cc39319578c899cccd1459a3328afea4260441e515d`


Imports: ../../app/providers/local_json → app/providers/local_json.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep

- Line 4: `define json_case with listener as List and method as Str and path as Str and body as Str and response as Str and expected_status as Str and expected_body as Str returning Int:`
- Line 16: `define main returning Int:`

## tests/providers/test_long_output.ep

[Source](../../tests/providers/test_long_output.ep) · 78 lines · 5362 bytes

SHA-256: `459b8ac76c88383df84604d175b80ec1c07cef237264d574be835f826aa16650`

> The requested document length is a workload, not a generation ceiling.
> Both adapters retain their full provider-derived available output allowance.
> Optional explicit seed allows a controlled upstream scheduling comparison.
> Pinned MLX uses its single-request path for seeded requests. This does not
> alter context, output capacity, model weights or the test workload.
> Independent fresh archives must not reuse the provider helper's immutable
> spool names from an earlier qualification run.

Imports: ../../app/providers/prepare → app/providers/prepare.ep, ../../app/providers/local_chat → app/providers/local_chat.ep, fs → standard library or compiler dependency

- Line 7: `define main returning Int:`

## tests/providers/test_mlx_discovery.ep

[Source](../../tests/providers/test_mlx_discovery.ep) · 19 lines · 1080 bytes

SHA-256: `a7ab7f36a94bb97101ebbb54fe12fb9f1b6ddaa51432c8837d2a64ca7324aafd`


Imports: ../../app/providers/mlx_discovery → app/providers/mlx_discovery.ep

- Line 3: `define main returning Int:`

## tests/providers/test_mlx_helper.ep

[Source](../../tests/providers/test_mlx_helper.ep) · 92 lines · 5526 bytes

SHA-256: `ec94fac5d292e496309f3a4de8c27f4eaa237eff06f2f3ecea7414f26a95c31a`

> Another waiter consumed the exit status. Do not infer success, signal a
> recycled PID, or leak the retained library handle on the ECHILD path.

Imports: ../../app/providers/helper_process → app/providers/helper_process.ep, ../../app/providers/mlx_request → app/providers/mlx_request.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 6: `define helper_test_cancel with writer as Int and completed as Int returning Int:`
- Line 14: `define main returning Int:`

## tests/providers/test_mlx_preflight.ep

[Source](../../tests/providers/test_mlx_preflight.ep) · 33 lines · 2192 bytes

SHA-256: `afb698fc2c7d25adb0bdd9b5d816cbc8d85ffe4bbc80ce436ba115a1da3f01b3`


Imports: ../../app/providers/mlx_preflight → app/providers/mlx_preflight.ep

- Line 3: `define main returning Int:`

## tests/providers/test_mlx_request.ep

[Source](../../tests/providers/test_mlx_request.ep) · 53 lines · 3252 bytes

SHA-256: `48b7344a20c395a347fe4c80a3809c281f075d281585f8ea55843d7bade93d03`


Imports: ../../app/providers/mlx_request → app/providers/mlx_request.ep, fs → standard library or compiler dependency

- Line 4: `define mlx_test_receipt with body as Str and config as Str and input as Int returning Str:`
- Line 10: `define main returning Int:`

## tests/providers/test_modality_preflight.ep

[Source](../../tests/providers/test_modality_preflight.ep) · 44 lines · 2680 bytes

SHA-256: `489352f3d42b4f5e060f9ecf9dbd52006dbdf95268456f3db707e46ddc489e22`


Imports: ../../app/providers/llama_preflight → app/providers/llama_preflight.ep, ../fixtures/json_fixture → tests/fixtures/json_fixture.ep

- Line 4: `define modality_reply with body as Str returning Str:`
- Line 7: `define modality_fixture with listener as List and completed as Int returning Int:`
- Line 20: `define main returning Int:`

## tests/providers/test_native_mlx.ep

[Source](../../tests/providers/test_native_mlx.ep) · 25 lines · 1395 bytes

SHA-256: `5a37210b2ace84635565ce1f752c3814c0f362b0d369896a02e67fa89f0ce0a7`


Imports: ../../app/providers/native_python → app/providers/native_python.ep

- Line 3: `define main returning Int:`

## tests/providers/test_quote.ep

[Source](../../tests/providers/test_quote.ep) · 46 lines · 2144 bytes

SHA-256: `4d4483f1e33019c0d6ddd328312ac14fdb333f8475f923dc064cc36059dd440c`


Imports: ../../app/providers/archived_stream → app/providers/archived_stream.ep

- Line 3: `define main returning Int:`

## tests/providers/test_registry.ep

[Source](../../tests/providers/test_registry.ep) · 35 lines · 2248 bytes

SHA-256: `51532eb19ab4d8e30777732e0ab2ac80a4012d025be4db63697de143cbddfe93`


Imports: ../../app/providers/registry → app/providers/registry.ep

- Line 3: `define main returning Int:`

## tests/research/capture_primary_activation.ep

[Source](../../tests/research/capture_primary_activation.ep) · 5 lines · 178 bytes

SHA-256: `0d8089d83529cd2d71204753490a7ec26e2781685c75a5d8972c72c761466d5b`

> Live acceptance invokes the same host-only capture implementation.

Imports: ../../app/research/capture_activation → app/research/capture_activation.ep

- Line 4: `define main returning Int:`

## tests/research/test_native_callback.ep

[Source](../../tests/research/test_native_callback.ep) · 11 lines · 535 bytes

SHA-256: `0d42afa5a92522cb938def3653fdd930db01925757734401eb7183420a25f96b`

> Verifies the existing compiler's named-function ABI for native research
> callbacks. No compiler changes or generated-code edits.

Imports: 

- Line 3: `define research_callback_probe with tensor as Int and ask as Int and user_data as Int returning Int:`
- Line 6: `define main returning Int:`

## tests/research/test_native_ffi.ep

[Source](../../tests/research/test_native_ffi.ep) · 45 lines · 2706 bytes

SHA-256: `33fea41896821e717a726686875761778a180c696f2bdfe6de2d2df8e59c89dc`

> Real upstream structure-return and structure-argument calls. No model weights
> are loaded. The GGUF read validates named metadata via the engine's parser.

Imports: ../../app/research/native_ffi → app/research/native_ffi.ep

- Line 5: `define main returning Int:`

## tests/research/test_primary_intervention.ep

[Source](../../tests/research/test_primary_intervention.ep) · 5 lines · 189 bytes

SHA-256: `80d592a61dfb0f43fc03cd1741e2214010bbb04c63f547e7800f2f3be0dd1e23`

> This fixture may derive a vector; the host entry requires explicit inputs.

Imports: ../../app/research/compare_intervention → app/research/compare_intervention.ep

- Line 4: `define main returning Int:`

## tests/research/test_run_record.ep

[Source](../../tests/research/test_run_record.ep) · 41 lines · 2211 bytes

SHA-256: `88fd17fd624f1bef595d25dd737fba0c42ffd0951c7616d80ef15af7d7c952bf`


Imports: ../../app/research/run_record → app/research/run_record.ep

- Line 3: `define research_record_writer with path as Str and content as Str and finished as Int returning Int:`
- Line 10: `define main returning Int:`

## tests/research/test_vector_controls.ep

[Source](../../tests/research/test_vector_controls.ep) · 29 lines · 1661 bytes

SHA-256: `e5e039f918e2e4f5e7ad176849ec8d590a77b2eba4ab924f9d4f1b666f3972b1`


Imports: ../../app/research/vector_controls → app/research/vector_controls.ep

- Line 3: `define main returning Int:`

## tests/security/prepare_http_principals.ep

[Source](../../tests/security/prepare_http_principals.ep) · 14 lines · 859 bytes

SHA-256: `ce31467266a4548f6149abd47b76026b8494b48af4ae3f4f250baf2b8eb9c3c1`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/tool_authority → app/security/tool_authority.ep

- Line 4: `define main returning Int:`

## tests/security/test_accounts.ep

[Source](../../tests/security/test_accounts.ep) · 72 lines · 6258 bytes

SHA-256: `48fefea5fee111b7caaf63bb93b992a09605d32e6a67b5575fb548eca6b8f92f`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/accounts → app/security/accounts.ep

- Line 4: `define main returning Int:`

## tests/security/test_device_links.ep

[Source](../../tests/security/test_device_links.ep) · 55 lines · 4686 bytes

SHA-256: `6fe69f3c4bbf6945b43009ad9fe6475011f1bd3ee77f4f8c72c403e51413f8fe`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/device_links → app/security/device_links.ep

- Line 4: `define main returning Int:`

## tests/security/test_direct_speech_authority.ep

[Source](../../tests/security/test_direct_speech_authority.ep) · 52 lines · 4488 bytes

SHA-256: `29f85f87699ae34db68f311c1c580145af8284341b7fc16156f376a8a3655999`


Imports: ../../app/web/message_speech → app/web/message_speech.ep

- Line 3: `define queue_saved_speech with database as Int and session as Str and job as Str returning Int:`
- Line 13: `define main returning Int:`

## tests/security/test_dispatch_revocation.ep

[Source](../../tests/security/test_dispatch_revocation.ep) · 58 lines · 4183 bytes

SHA-256: `e697cfa5864818805ab27e091334c07e83e7a20c714af8171b309dbd110c4531`


Imports: ../../app/archive/snapshot → app/archive/snapshot.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/security/tool_authority → app/security/tool_authority.ep, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 10: `define main returning Int:`

## tests/security/test_host_text.ep

[Source](../../tests/security/test_host_text.ep) · 90 lines · 5326 bytes

SHA-256: `a062ff19f57bec3d86e3dcda3e626739d8534a4a536bc41a238a9b6dac56ad9b`


Imports: ../../app/archive/snapshot → app/archive/snapshot.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/security/tool_authority → app/security/tool_authority.ep, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 10: `define main returning Int:`

## tests/security/test_knowledge.ep

[Source](../../tests/security/test_knowledge.ep) · 74 lines · 6274 bytes

SHA-256: `92b592f79061163ef8b031df0230df4d26fd2a359dd66ae544fda0fe24e8711a`

> A separate contradictory claim is retained; it is not silently merged.

Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/memory/knowledge → app/memory/knowledge.ep

- Line 4: `define knowledge_fixture_digest with database as Int and event as Str returning Str:`
- Line 10: `define main returning Int:`

## tests/security/test_knowledge_lifecycle.ep

[Source](../../tests/security/test_knowledge_lifecycle.ep) · 74 lines · 6133 bytes

SHA-256: `95af476b67cbdd62ab1078bb4e30a87af3812d3933cfcc5f1d0566777353505a`

> Original evidence remains explicitly retrievable; derived recall excludes it.

Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/memory/knowledge_lifecycle → app/memory/knowledge_lifecycle.ep, ../../app/archive/snapshot → app/archive/snapshot.ep

- Line 5: `define main returning Int:`

## tests/security/test_knowledge_tools.ep

[Source](../../tests/security/test_knowledge_tools.ep) · 84 lines · 7811 bytes

SHA-256: `159c20870f06ad0c4e1c2197a45e484525d8888a59e125516d33b1d31cbfd3c8`

> Exercise the real streaming parser, durable queued request and dispatcher.

Imports: ../../app/web/attachments → app/web/attachments.ep, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 7: `define knowledge_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:`
- Line 27: `define main returning Int:`

## tests/security/test_memory_tools.ep

[Source](../../tests/security/test_memory_tools.ep) · 57 lines · 4612 bytes

SHA-256: `2d74b555a34a218dd02c6954330422f828ba3a81979144088d815d95c59d9f55`

> Exercise the real streaming parser, durable queued request and dispatcher.

Imports: ../../app/web/attachments → app/web/attachments.ep, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 7: `define recall_fixture_call with database as Int and run as Str and request as Str and call_id as Str and name as Str and arguments as Str returning Int:`
- Line 27: `define main returning Int:`

## tests/security/test_principals.ep

[Source](../../tests/security/test_principals.ep) · 50 lines · 3211 bytes

SHA-256: `83b7a08fbe0e6d406bbb06f016d49824c01c031a0b035380b6a68172104945aa`

> Failing the role update must also roll back the authority receipt.

Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/principals → app/security/principals.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/security/test_private_recall.ep

[Source](../../tests/security/test_private_recall.ep) · 55 lines · 4459 bytes

SHA-256: `901b60a1e229f98213394954d1abdf01ac234dd26eee6e2a42ea78249b3da545`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/memory/archive_recall → app/memory/archive_recall.ep

- Line 4: `define main returning Int:`

## tests/security/test_private_web_scope.ep

[Source](../../tests/security/test_private_web_scope.ep) · 53 lines · 4546 bytes

SHA-256: `c0194c8fe87263027587b8b55ddb394586d13934070b438ea0e81a49fca240ec`

> Metadata fixture; no host file is opened by these ownership queries.

Imports: ../../app/web/uploads → app/web/uploads.ep, ../../app/web/cancellation → app/web/cancellation.ep, ../../app/web/message_speech → app/web/message_speech.ep, ../../app/memory/session_ownership → app/memory/session_ownership.ep

- Line 6: `define main returning Int:`

## tests/security/test_revocation.ep

[Source](../../tests/security/test_revocation.ep) · 61 lines · 4678 bytes

SHA-256: `c4d5fc7e3c62176d62a5e64f5c0bc61388e467064797123cfe4adad10e18ecfa`

> A fixture account binding models a trusted platform adapter. This is not
> evidence that a live Discord account has been linked or authenticated.
> A delayed replay of an old revoke is only a receipt lookup, never a new
> state change that could undo a newer explicit restoration.

Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/revocation → app/security/revocation.ep

- Line 4: `define main returning Int:`

## tests/security/test_run_authority.ep

[Source](../../tests/security/test_run_authority.ep) · 61 lines · 5338 bytes

SHA-256: `18263815f60cc62c1dd9837e462e6bffcf5ad1773d550c62fd7cc1194874b3c5`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/run_authority → app/security/run_authority.ep

- Line 4: `define main returning Int:`

## tests/security/test_session_principals.ep

[Source](../../tests/security/test_session_principals.ep) · 39 lines · 3578 bytes

SHA-256: `ed6fd855520d9ea71295ee107c4077100b3feab137a18a8443bcf3f9edbd7230`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/memory/session_ownership → app/memory/session_ownership.ep

- Line 4: `define main returning Int:`

## tests/security/test_tool_policy.ep

[Source](../../tests/security/test_tool_policy.ep) · 71 lines · 4440 bytes

SHA-256: `6363667fbcb30956967b64757680f4bd763ce9500f69cd7b1d920e86a72511aa`


Imports: ../../app/archive/snapshot → app/archive/snapshot.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency, ../../app/security/tool_authority → app/security/tool_authority.ep, ../../app/agent/tool_recovery → app/agent/tool_recovery.ep, ../../app/agent/run_messages → app/agent/run_messages.ep, ../../app/archive/history → app/archive/history.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 10: `define main returning Int:`

## tests/tools/test_history_dispatch.ep

[Source](../../tests/tools/test_history_dispatch.ep) · 86 lines · 6748 bytes

SHA-256: `ae7c9e75a02f1f4009bd7235ea2c08a08824f0ef3d8111e1cb71b21a1229961f`


Imports: ../../app/agent/run_messages → app/agent/run_messages.ep

- Line 3: `define main returning Int:`

## tests/transport/test_event_stream.ep

[Source](../../tests/transport/test_event_stream.ep) · 55 lines · 3274 bytes

SHA-256: `464e61fe69cb123b473fbad14a20895a9def44cc97886dcb4662fa32e492693d`


Imports: ../../app/transport/event_stream → app/transport/event_stream.ep, ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 4: `define event_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:`
- Line 13: `define event_case with listener as List and wire as Str and read_size as Int and expected_terminal as Str returning Int:`
- Line 42: `define main returning Int:`

## tests/transport/test_http_body.ep

[Source](../../tests/transport/test_http_body.ep) · 103 lines · 5454 bytes

SHA-256: `5b5acf74553ada6b82afaff16209c45f2b90e2df856c39c5425a2340c19256e9`


Imports: ../../app/transport/http_body → app/transport/http_body.ep, ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 4: `define body_send_text with library as Int and descriptor as Int and text as Str returning Int:`
- Line 10: `define body_fixture_sender with library as Int and descriptor as Int and completed as Int and truncated as Int returning Int:`
- Line 26: `define main returning Int:`

## tests/transport/test_http_head.ep

[Source](../../tests/transport/test_http_head.ep) · 35 lines · 2678 bytes

SHA-256: `ab13c76b22d69bd980546ffb352ba4fcdc7bd36a00bcf5b2143ec8c793be90f2`


Imports: ../../app/transport/http_head → app/transport/http_head.ep

- Line 3: `define head_expect with head as Str and expected as Str returning Int:`
- Line 13: `define main returning Int:`

## tests/transport/test_http_response.ep

[Source](../../tests/transport/test_http_response.ep) · 76 lines · 4614 bytes

SHA-256: `063a1cf94ce276d17aec6557d72f43ba80a374d6a7ab955b98c37a743e3ba594`


Imports: ../../app/transport/http_response → app/transport/http_response.ep, ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 4: `define response_fixture_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:`
- Line 13: `define response_case with listener as List and wire as Str and method as Str and expected_status as Int and expected_framing as Str and expected_body as Str and expected_terminal as Str returning Int:`
- Line 60: `define main returning Int:`

## tests/transport/test_http_stream.ep

[Source](../../tests/transport/test_http_stream.ep) · 66 lines · 3281 bytes

SHA-256: `54a07d8828b57823232de8b4872aa0dbea50ef8ce7d437928183ccbbd87119fb`


Imports: ../../app/transport/http_stream → app/transport/http_stream.ep, ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 4: `define fragmented_request with library as Int and descriptor as Int and completed as Int returning Int:`
- Line 28: `define main returning Int:`

## tests/transport/test_local_socket.ep

[Source](../../tests/transport/test_local_socket.ep) · 53 lines · 2305 bytes

SHA-256: `03894940a42ccc41fedf3e7e1cff719eb2318950e3b83f0764c7b1d87a83e69f`

> Native address bytes must be exactly 127.0.0.1, never 0.0.0.0.

Imports: ../../app/transport/local_socket → app/transport/local_socket.ep, string → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/transport/test_server_events.ep

[Source](../../tests/transport/test_server_events.ep) · 59 lines · 3396 bytes

SHA-256: `4427cbff7758ba07a7d39951f594570a990f826a5cb594f9eec8bf620f54f34a`


Imports: ../../app/transport/server_events → app/transport/server_events.ep

- Line 3: `define events_feed with state as List and wire as Str returning List:`
- Line 17: `define event_matches with events as List and index as Int and kind as Str and data as Str and identity as Str returning Int:`
- Line 27: `define main returning Int:`

## tests/transport/test_socket_read.ep

[Source](../../tests/transport/test_socket_read.ep) · 82 lines · 3633 bytes

SHA-256: `48cce1c01d2665b0b6209bbe2c499796b5d18452cf2fbf94d6026e22e04502b8`

> This delay is a test synchronisation fixture, not a request deadline.
> Three raw bytes: A, zero, B. A C-string read would silently lose B.

Imports: ../../app/transport/socket_read → app/transport/socket_read.ep

- Line 2: `define cancel_blocked_reader with writer as Int and completed as Int returning Int:`
- Line 9: `define main returning Int:`

## tests/transport/test_socket_write.ep

[Source](../../tests/transport/test_socket_write.ep) · 92 lines · 4229 bytes

SHA-256: `bc4a230aba1a3b41d95adbbbf94de060acae5363716276c8866e2cbd9e5813a0`

> An 8 MiB all-zero fixture exceeds socket buffers and exercises binary data.
> A connected peer deliberately does not read. Cancellation must interrupt
> backpressure without requiring that peer to close or consume any bytes.

Imports: ../../app/transport/socket_write → app/transport/socket_write.ep

- Line 3: `define drain_binary with library as Int and descriptor as Int and expected as Int and expected_count as Int and completed as Int returning Int:`
- Line 32: `define cancel_writer with writer as Int and completed as Int returning Int:`
- Line 38: `define main returning Int:`

## tests/web/api_response_browser.ep

[Source](../../tests/web/api_response_browser.ep) · 87 lines · 4291 bytes

SHA-256: `61c0460992270646a12539d809d0a11e585acc21bfb9b01967e7d857d5a572d3`

> Browser fixture linked with client.ep after renaming its application main.
> It exercises the real echo_api path with gateway and truncated-body failures.

Imports: 

- Line 3: `define main:`

## tests/web/audio_capture_browser.ep

[Source](../../tests/web/audio_capture_browser.ep) · 162 lines · 8135 bytes

SHA-256: `103c857efc34540ba870c20cd37d563c824648133c8734a6d1ce63461ed674e3`

> Actual browser acceptance harness. Synthetic input is explicitly labelled;
> microphone is a separate user-initiated path, never silently substituted.

Imports: 

- Line 3: `define capture_test_note with text:`
- Line 8: `define capture_test_controls with busy:`
- Line 16: `define capture_test_cleanup:`
- Line 31: `define capture_test_finish:`
- Line 86: `define capture_test_start with synthetic:`
- Line 142: `define capture_test_setup with ignored:`
- Line 157: `define main:`

## tests/web/audio_recorder_browser.ep

[Source](../../tests/web/audio_recorder_browser.ep) · 97 lines · 4914 bytes

SHA-256: `ce9bf4a39281e857d068fbdef705b3811a906ae1373c03d79fa7abd7324fdc30`

> The one-batch stop below is a finite synthetic test workload, not a recorder
> limit. Production capture ends only on user stop or an explicit failure.

Imports: 

- Line 3: `define recorder_test_note with text:`
- Line 8: `define recorder_test_start with event:`
- Line 80: `define recorder_test_load with paths and index:`
- Line 94: `define main:`

## tests/web/audio_store_browser.ep

[Source](../../tests/web/audio_store_browser.ep) · 102 lines · 6016 bytes

SHA-256: `2803902d3cb38039dcb1084bfc58bdd2064274d6a95c031008899214945a0499`

> Explicit synthetic browser-storage acceptance, independent of microphone.

Imports: 

- Line 2: `define store_test_note with text:`
- Line 7: `define store_test_rejection with operation:`
- Line 18: `define store_test_run with ignored:`
- Line 97: `define main:`

## tests/web/audio_upload_recovery_browser.ep

[Source](../../tests/web/audio_upload_recovery_browser.ep) · 103 lines · 5325 bytes

SHA-256: `839991ba51047e4e5f562eb24a2fc9f257135d88ba1749a8754bbf85d6e09637`

> Build with the production client source, renaming its main to
> echo_client_main before concatenating this test. All production upload and
> recording functions remain unchanged. Run only against a fixture database.

Imports: 

- Line 4: `define recovery_test_note with text:`
- Line 9: `define recovery_test_prepare with send_body:`
- Line 56: `define recovery_test_resume with event:`
- Line 91: `define main:`

## tests/web/cancel_fixture_run.ep

[Source](../../tests/web/cancel_fixture_run.ep) · 12 lines · 364 bytes

SHA-256: `bcf33ac083d881f6280ae58544e0cacc566075028c5dd3cc0a04daf20a2d9fcb`


Imports: ../../app/web/run_store → app/web/run_store.ep

- Line 3: `define main returning Int:`

## tests/web/provision_crash_probe.ep

[Source](../../tests/web/provision_crash_probe.ep) · 20 lines · 906 bytes

SHA-256: `54058d1a2d4483d225dfd14b117f5a90a15ef4a041b1443e56c622753c97c2d8`

> Host-only provisioner for an isolated acceptance-test device. Its credential
> is saved to the caller's private runtime file, never printed into a log.

Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/agent/control → app/agent/control.ep, fs → standard library or compiler dependency

- Line 7: `define main returning Int:`

## tests/web/seed_device_routes.ep

[Source](../../tests/web/seed_device_routes.ep) · 17 lines · 1086 bytes

SHA-256: `111e69470b5a82944d1fe85cdce5f0d8421e337a29bfdcff09940862fcdb6888`

> Public test credentials, only for a disposable loopback fixture database.

Imports: ../../app/web/run_store → app/web/run_store.ep

- Line 4: `define main returning Int:`

## tests/web/seed_rendering.ep

[Source](../../tests/web/seed_rendering.ep) · 35 lines · 2581 bytes

SHA-256: `d3c42854b8838b9707b0bedcc816b171ec0894ee2cab937984e4ca50af6212fd`

> Synthetic UI acceptance content, never claimed as a model response.

Imports: ../../app/web/run_store → app/web/run_store.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/web/seed_retry_browser.ep

[Source](../../tests/web/seed_retry_browser.ep) · 20 lines · 1246 bytes

SHA-256: `8e121f85e3ca6d24719510153f995c3402a1fa7c1e7d19fc927d204c5a21ddf8`

> Disposable local fixture: invitation and credentials are test values.

Imports: ../../app/web/worker → app/web/worker.ep

- Line 4: `define main returning Int:`

## tests/web/serve_partial_fixture.ep

[Source](../../tests/web/serve_partial_fixture.ep) · 43 lines · 2189 bytes

SHA-256: `b078551b7a3ae90e1c54928854f126822a7b123785a6058209aecb4903d5ab26`

> EOF framing permits an unfinished response with no arbitrary body cap.
> The fixture deliberately withholds DONE and natural completion.

Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep, ../../app/web/http_api → app/web/http_api.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/web/serve_provider_fixture.ep

[Source](../../tests/web/serve_provider_fixture.ep) · 20 lines · 799 bytes

SHA-256: `695bd04c25c582c794e9c804bf077c3a498dea995ff174402572d348cbefc623`


Imports: ../fixtures/runner_provider → tests/fixtures/runner_provider.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/web/serve_recovery_fixture.ep

[Source](../../tests/web/serve_recovery_fixture.ep) · 57 lines · 3493 bytes

SHA-256: `597a3dd74cb539bb8e53ca48ae81e9d24d0b73f832882196b01ae83ffa51f0b1`

> The separate server executable must discover and resume the interrupted run.
> This process only serves scripted provider responses and verifies the result.

Imports: ../../app/web/recovery → app/web/recovery.ep, ../fixtures/runner_provider → tests/fixtures/runner_provider.ep, fs → standard library or compiler dependency

- Line 7: `define main returning Int:`

## tests/web/test_activity.ep

[Source](../../tests/web/test_activity.ep) · 61 lines · 4036 bytes

SHA-256: `c928beba05f8914b53ac0b2a6521617096004f5678700e23d99e309d2c91354a`

> A later unrelated job must not replace the current run's activity.
> Even a matching job prefix in another session cannot supply a stage.
> A committed binding for an old head must not describe a later run step.

Imports: ../../app/web/activity → app/web/activity.ep, fs → standard library or compiler dependency

- Line 4: `define activity_expect with database as Int and expected as Str returning Int:`
- Line 11: `define main returning Int:`

## tests/web/test_attachment_text.ep

[Source](../../tests/web/test_attachment_text.ep) · 30 lines · 2057 bytes

SHA-256: `90c623c461d43ce363405c4547a3105cf125fe679017668c318853df730c3ab4`


Imports: ../../app/tools/attachment_tools → app/tools/attachment_tools.ep, ../../app/web/attachments → app/web/attachments.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/web/test_attachment_tools.ep

[Source](../../tests/web/test_attachment_tools.ep) · 54 lines · 4206 bytes

SHA-256: `d8a051b028afc59520b00f0bde027926a690d5d89af382dc25376d090bb1a2ee`


Imports: ../../app/tools/attachment_tools → app/tools/attachment_tools.ep, ../../app/web/attachments → app/web/attachments.ep

- Line 4: `define main returning Int:`

## tests/web/test_attachments.ep

[Source](../../tests/web/test_attachments.ep) · 40 lines · 2941 bytes

SHA-256: `0cdfac59b8f7fecd197779d4c1db76c6a6a1a1345c29996f84e46f0d38043a85`


Imports: ../../app/web/attachments → app/web/attachments.ep

- Line 3: `define main returning Int:`

## tests/web/test_backup.ep

[Source](../../tests/web/test_backup.ep) · 56 lines · 3711 bytes

SHA-256: `5c34d2fac93f326e93cab4dc099be79a2e6fbf6ca1431548f77379f47abb55d3`

> Later source changes must not leak into the consistent snapshot.
> Disposable corruption: integrity_check alone does not catch broken FKs.

Imports: ../../app/web/worker → app/web/worker.ep, ../../app/archive/snapshot → app/archive/snapshot.ep

- Line 4: `define main returning Int:`

## tests/web/test_branches.ep

[Source](../../tests/web/test_branches.ep) · 118 lines · 9127 bytes

SHA-256: `9c28f674244001e9bf255717ee371f00e5d0feb05b9c54079f6bc073c84703f1`


Imports: ../../app/web/export → app/web/export.ep, ../../app/archive/export_restore → app/archive/export_restore.ep, fs → standard library or compiler dependency, ../../app/web/attachments → app/web/attachments.ep, ../../app/web/branches → app/web/branches.ep

- Line 7: `define branch_fixture_finish with database as Int and run as Str and user_event as Str and request as Str and final as Str returning Int:`
- Line 18: `define main returning Int:`

## tests/web/test_cancellation.ep

[Source](../../tests/web/test_cancellation.ep) · 75 lines · 4221 bytes

SHA-256: `d10040f226f1571be2ffeaecb096f8be16685864f6f28a6eaaae115bbce37225`

> The connected peer sends nothing. Only cancellation can wake this read.
> A saved cancellation survives reopening and reaches terminal run state.
> No server listens on this fixture port. A normal early return must stop
> and join its monitor without inventing a user cancellation or hanging.

Imports: ../../app/web/worker → app/web/worker.ep

- Line 3: `define request_stop_later with path as Str and completed as Int returning Int:`
- Line 11: `define main returning Int:`

## tests/web/test_devices.ep

[Source](../../tests/web/test_devices.ep) · 96 lines · 6895 bytes

SHA-256: `100bd554f68839a864e86599b4f9a5857c88c9bd15a2545ce854c213dc9338a8`


Imports: ../../app/web/run_store → app/web/run_store.ep, ../../app/security/revocation → app/security/revocation.ep

- Line 4: `define main returning Int:`

## tests/web/test_empty_session.ep

[Source](../../tests/web/test_empty_session.ep) · 45 lines · 3117 bytes

SHA-256: `226b1d23d19f399eea80a82fed637cb75b3b5b0e3c7ef338cd51fea576f94956`


Imports: ../../app/web/attachments → app/web/attachments.ep

- Line 3: `define main returning Int:`

## tests/web/test_export.ep

[Source](../../tests/web/test_export.ep) · 46 lines · 2872 bytes

SHA-256: `2c5a3908d7b53a5d88b46e660ad239f3ec21026e58b13f938471412b27e4f3ca`


Imports: ../../app/web/export → app/web/export.ep, ../../app/archive/export_verify → app/archive/export_verify.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/web/test_history_pages.ep

[Source](../../tests/web/test_history_pages.ep) · 72 lines · 3851 bytes

SHA-256: `84f2c259b6fea65bf79b4b0d6918e0be9d24927d347830ff59ac93b50ef76d1d`


Imports: ../../app/web/branches → app/web/branches.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/web/test_http_api.ep

[Source](../../tests/web/test_http_api.ep) · 61 lines · 5012 bytes

SHA-256: `822fbe83710ab76a60745954c80d84434db25ff68477d5fe04c6f753cc7cd0a5`


Imports: ../../app/web/http_api → app/web/http_api.ep

- Line 3: `define api_sender with library as Int and descriptor as Int and wire as Str and completed as Int returning Int:`
- Line 12: `define main returning Int:`

## tests/web/test_idle_cancel.ep

[Source](../../tests/web/test_idle_cancel.ep) · 38 lines · 2310 bytes

SHA-256: `1963298ce1e1715e1756db62075838e851878680be8ff12569e51a0dbf4a74b2`


Imports: ../../app/web/worker → app/web/worker.ep

- Line 3: `define main returning Int:`

## tests/web/test_pdf_attachment.ep

[Source](../../tests/web/test_pdf_attachment.ep) · 36 lines · 2640 bytes

SHA-256: `9cb78f381007dec18475a1daef9ffbdaf93cabb54d7281b6e661491496fb0327`


Imports: ../../app/tools/attachment_tools → app/tools/attachment_tools.ep, ../../app/web/attachments → app/web/attachments.ep

- Line 4: `define main returning Int:`

## tests/web/test_progress.ep

[Source](../../tests/web/test_progress.ep) · 45 lines · 3145 bytes

SHA-256: `f91d3ccf0fe7496eaf6acee772a2b06c8416332ffa69f225de91fffabdab4014`


Imports: ../../app/web/progress → app/web/progress.ep

- Line 3: `define progress_piece with database as Int and request as Str and sequence as Int and data as Str returning Int:`
- Line 8: `define main returning Int:`

## tests/web/test_provider_submission.ep

[Source](../../tests/web/test_provider_submission.ep) · 57 lines · 4462 bytes

SHA-256: `a1ce4ba6466d62de76db40e266cc3dabccbc879ab61ce815df6c19d2cd276e77`


Imports: ../../app/web/http_api → app/web/http_api.ep, ../../app/agent/provider_profile → app/agent/provider_profile.ep

- Line 4: `define main returning Int:`

## tests/web/test_retry.ep

[Source](../../tests/web/test_retry.ep) · 46 lines · 4036 bytes

SHA-256: `f1f79fdffcc6047dd23b4e6671df588b6ce81189a346938cf7ba951a06b85657`


Imports: ../../app/web/worker → app/web/worker.ep, ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 4: `define main returning Int:`

## tests/web/test_revisions.ep

[Source](../../tests/web/test_revisions.ep) · 113 lines · 9492 bytes

SHA-256: `fe99143b96158b95062f7dae99bd960427a76c0a2c042c104548165cf8e33af4`

> The branch boundary already freezes selection before a run exists.
> Simulate disconnect after branch commit but before submission.
> Simulate another disconnect after submission commit but before worker claim.

Imports: ../../app/web/branches → app/web/branches.ep, ../../app/web/attachments → app/web/attachments.ep, ../../app/web/export → app/web/export.ep, fs → standard library or compiler dependency, ../../app/web/worker → app/web/worker.ep

- Line 7: `define main returning Int:`

## tests/web/test_run_store.ep

[Source](../../tests/web/test_run_store.ep) · 52 lines · 3285 bytes

SHA-256: `bd911c4ec743ff11660573ab0e4aa2cdf5624b25f48a3dd5e9b8c460adc40214`

> Inject run creation failure after the pending message/session inserts.

Imports: ../../app/web/run_store → app/web/run_store.ep

- Line 3: `define main returning Int:`

## tests/web/test_session_rename.ep

[Source](../../tests/web/test_session_rename.ep) · 46 lines · 3101 bytes

SHA-256: `91adfab3e742995cd47670086b1dc3c1147d5e44be47bd7cb5423472aad7424a`


Imports: ../../app/web/run_store → app/web/run_store.ep

- Line 3: `define main returning Int:`

## tests/web/test_submission_route_recovery.ep

[Source](../../tests/web/test_submission_route_recovery.ep) · 104 lines · 7245 bytes

SHA-256: `112d8c09694c683523f980666bd2f207bdc62d313288cc78528ad137133c1df2`

> Exercise the actual authenticated HTTP route through a native socket reader.
> Durable original submission with deliberately no worker claim.

Imports: ../../app/web/routes → app/web/routes.ep, os → standard library or compiler dependency, fs → standard library or compiler dependency

- Line 6: `define recovery_route with config as List and target as Str and body as Str returning List:`
- Line 22: `define recovery_await with database as Int and run as Str returning Int:`
- Line 36: `define main returning Int:`

## tests/web/test_trace.ep

[Source](../../tests/web/test_trace.ep) · 39 lines · 2715 bytes

SHA-256: `8fcc696eeba8b97aafc2c398e1c020f73c08b4862c67d8919c7a5ac1423222cd`


Imports: ../../app/web/trace → app/web/trace.ep

- Line 3: `define progress_piece with database as Int and request as Str and sequence as Int and data as Str returning Int:`
- Line 8: `define main returning Int:`

## tests/web/test_turn_attachments.ep

[Source](../../tests/web/test_turn_attachments.ep) · 75 lines · 5388 bytes

SHA-256: `f91ebeff5574d2130dd0d44680bb6c8b5bdc2d789fc4ea9eda4039f6a84e5b58`


Imports: ../../app/web/attachments → app/web/attachments.ep, ../../app/web/http_api → app/web/http_api.ep

- Line 4: `define main returning Int:`

## tests/web/test_upload_body.ep

[Source](../../tests/web/test_upload_body.ep) · 56 lines · 3171 bytes

SHA-256: `41831bc5dfea462ca0f895db110f625c2cefbd4b6864ff4f6a94ca1198cffdbf`


Imports: ../../app/web/upload_body → app/web/upload_body.ep

- Line 3: `define upload_sender with library as Int and descriptor as Int and bytes as Int and completed as Int returning Int:`
- Line 26: `define upload_case with path as Str and bytes as Int returning Int:`
- Line 50: `define main returning Int:`

## tests/web/test_upload_disconnect.ep

[Source](../../tests/web/test_upload_disconnect.ep) · 60 lines · 3930 bytes

SHA-256: `98b2b1d350895ffad181cce3d06d6bdfeb3a8b95ee01fd6bac18e75347533534`

> Public credentials and finite payload only for this isolated loopback fixture.
> Modes: setup, disconnect, crash, verify. Crash keeps its socket open until
> the externally terminated fixture server closes it; no elapsed-time verdict.
> Wait for authoritative peer closure, not a timeout or a guessed delay.

Imports: ../../app/web/uploads → app/web/uploads.ep

- Line 6: `define main returning Int:`

## tests/web/test_uploads.ep

[Source](../../tests/web/test_uploads.ep) · 58 lines · 4670 bytes

SHA-256: `3afff105cd08b0d7048d52722b1b2e510ecc4fb190503cd5ac746d31248b373a`


Imports: ../../app/web/uploads → app/web/uploads.ep

- Line 3: `define main returning Int:`

## tests/web/test_worker_recovery.ep

[Source](../../tests/web/test_worker_recovery.ep) · 82 lines · 5555 bytes

SHA-256: `de7c26eb9a2b4e8a2fdddcbc76044e7921b7eddfc91e0eaa1274e8aa2c8eb3c1`

> Restart the fixture at its persisted endpoint, as a real provider restart
> must do. A new ephemeral endpoint would be a different provider profile.
> A claim from an older executable has no lock protocol proof.
> A changed host default must not replace this interrupted run's selection.

Imports: ../../app/web/recovery → app/web/recovery.ep, ../fixtures/runner_provider → tests/fixtures/runner_provider.ep

- Line 4: `define main returning Int:`

## tests/workspace/hello_sandbox.ep

[Source](../../tests/workspace/hello_sandbox.ep) · 7 lines · 187 bytes

SHA-256: `a991274f1ca07e27a5300fdb6caa1cd334a024cccc467d69bcf35a9faef496f5`


Imports: string → standard library or compiler dependency

- Line 3: `define main returning Int:`

## tests/workspace/test_capture_process.ep

[Source](../../tests/workspace/test_capture_process.ep) · 111 lines · 6042 bytes

SHA-256: `e49c0c1819b55bfe6dcbaafa0d9d9d4ddf6320a92911bdf6afcd13114f78fd44`


Imports: ../../app/workspace/sandbox_process → app/workspace/sandbox_process.ep, ../../app/workspace/process_output → app/workspace/process_output.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency, ../../app/workspace/capture_process → app/workspace/capture_process.ep

- Line 7: `external define peek_byte with pointer and offset returning Int`
- Line 8: `external define poke_byte with pointer and offset and value returning Int`
- Line 10: `define capture_cancel_after_eof with path as Str and writer as Int and completed as Int returning Int:`
- Line 26: `define main returning Int:`

## tests/workspace/test_compile_process.ep

[Source](../../tests/workspace/test_compile_process.ep) · 51 lines · 2895 bytes

SHA-256: `49f18b1b43a535b62928659ae7d679224937d3a3051a16904c14f54e9c91cbe8`


Imports: ../../app/workspace/sandbox_process → app/workspace/sandbox_process.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 5: `define compile_probe_run with runtime as Str and workspace as Str and executable as Str and source as Str and phase as Str returning Int:`
- Line 37: `define main returning Int:`

## tests/workspace/test_create_text.ep

[Source](../../tests/workspace/test_create_text.ep) · 38 lines · 2384 bytes

SHA-256: `c8aa100be03086012720fe196010c0a2d7d321507ff4c4304e5918021bef7a46`


Imports: ../../app/workspace/create_text → app/workspace/create_text.ep, ../../app/workspace/read_text → app/workspace/read_text.ep

- Line 4: `define main returning Int:`

## tests/workspace/test_directories.ep

[Source](../../tests/workspace/test_directories.ep) · 56 lines · 2747 bytes

SHA-256: `fa9c3f2af99bb46e567a5dd30703c9262f0dbfdffea8dab37a76e555945b5d96`


Imports: ../../app/workspace/directories → app/workspace/directories.ep

- Line 3: `define main returning Int:`

## tests/workspace/test_execute_claimed.ep

[Source](../../tests/workspace/test_execute_claimed.ep) · 103 lines · 5683 bytes

SHA-256: `7cdb18599e15dc5a8bbd82a8a5d0ba3026739d5685e9f4b5d6fb586c52860780`


Imports: ../../app/workspace/sandbox_process → app/workspace/sandbox_process.ep, ../../app/workspace/process_output → app/workspace/process_output.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency, ../../app/workspace/execute_process → app/workspace/execute_process.ep

- Line 7: `external define peek_byte with pointer and offset returning Int`
- Line 8: `external define poke_byte with pointer and offset and value returning Int`
- Line 10: `define capture_cancel_after_eof with path as Str and writer as Int and completed as Int returning Int:`
- Line 26: `define main returning Int:`

## tests/workspace/test_execution_request.ep

[Source](../../tests/workspace/test_execution_request.ep) · 90 lines · 5642 bytes

SHA-256: `88762a668258df7f57e2b7a2428874ee2f60be0b64362c8fb1caf7d229f33bc3`


Imports: ../../app/workspace/execution_request → app/workspace/execution_request.ep, ../../app/tools/execution_output → app/tools/execution_output.ep, ../../app/web/run_store → app/web/run_store.ep, ../fixtures/generation_fixture → tests/fixtures/generation_fixture.ep

- Line 6: `define execution_request_fixture with database as Int and identity as Str and arguments as Str returning Str:`
- Line 33: `define main returning Int:`

## tests/workspace/test_files.ep

[Source](../../tests/workspace/test_files.ep) · 46 lines · 2377 bytes

SHA-256: `79ec7a305a3d3f4ceb7fe7cc779d5904ba895a50d57c68c6b1bc1868bf12fb92`


Imports: ../../app/workspace/files → app/workspace/files.ep

- Line 3: `define main returning Int:`

## tests/workspace/test_launch_interruption.ep

[Source](../../tests/workspace/test_launch_interruption.ep) · 65 lines · 3510 bytes

SHA-256: `ed564b16cc1977e569d30798f2cb32db549df580c5808e85b453daf5bf782fd2`

> Rejected terminal-to-start transition, even with a forged test-only handle.

Imports: ../../app/workspace/process_journal → app/workspace/process_journal.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/workspace/test_output_archive.ep

[Source](../../tests/workspace/test_output_archive.ep) · 69 lines · 4394 bytes

SHA-256: `b88e71447b329c84df21dbee1ac37a6ae23d574f5fb386effa512dcec189b0ff`


Imports: ../../app/workspace/output_archive → app/workspace/output_archive.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## tests/workspace/test_process.ep

[Source](../../tests/workspace/test_process.ep) · 94 lines · 5018 bytes

SHA-256: `6f6c4709d05f4229fb7896985623e3e228a0f397ed615e09ce29d4ab5da18282`

> The deliberately inherited parent descriptor must be absent.
> 196608 output bytes exceed the unsafe upstream helper's 65536 buffer.
> No CLOEXEC flag here: the adapter itself must prevent this descriptor leak.

Imports: ../../app/workspace/native_process → app/workspace/native_process.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/workspace/test_process_journal.ep

[Source](../../tests/workspace/test_process_journal.ep) · 132 lines · 7647 bytes

SHA-256: `92e89666a9a13cffab4909e32a9a2ee4202c93f6f6c5abcc7ed9e5fc037c0d5c`


Imports: ../../app/workspace/sandbox_process → app/workspace/sandbox_process.ep, ../../app/workspace/process_output → app/workspace/process_output.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency, ../../app/workspace/capture_process → app/workspace/capture_process.ep, ../../app/workspace/process_journal → app/workspace/process_journal.ep

- Line 8: `external define peek_byte with pointer and offset returning Int`
- Line 9: `external define poke_byte with pointer and offset and value returning Int`
- Line 11: `define capture_cancel_after_eof with path as Str and writer as Int and completed as Int returning Int:`
- Line 27: `define main returning Int:`

## tests/workspace/test_process_output.ep

[Source](../../tests/workspace/test_process_output.ep) · 91 lines · 4554 bytes

SHA-256: `8d4f5cd782d123e1fb7f01b094e084ad189adb5c3179da5ec2afda8b43d0567d`

> A closed cancellation pipe wakes even when output pipes have no data.

Imports: ../../app/workspace/sandbox_process → app/workspace/sandbox_process.ep, ../../app/workspace/process_output → app/workspace/process_output.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 6: `external define peek_byte with pointer and offset returning Int`
- Line 7: `external define poke_byte with pointer and offset and value returning Int`
- Line 9: `define main returning Int:`

## tests/workspace/test_process_output_archive.ep

[Source](../../tests/workspace/test_process_output_archive.ep) · 119 lines · 6512 bytes

SHA-256: `12a928511f5f65b6f02d2e999036bde65bae188c311289c09ef812debeea8e96`

> A closed cancellation pipe wakes even when output pipes have no data.

Imports: ../../app/workspace/sandbox_process → app/workspace/sandbox_process.ep, ../../app/workspace/process_output → app/workspace/process_output.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency, ../../app/workspace/output_archive → app/workspace/output_archive.ep

- Line 7: `external define peek_byte with pointer and offset returning Int`
- Line 8: `external define poke_byte with pointer and offset and value returning Int`
- Line 10: `define main returning Int:`

## tests/workspace/test_process_tree.ep

[Source](../../tests/workspace/test_process_tree.ep) · 77 lines · 4068 bytes

SHA-256: `21e287a42af4e6b93aa286cbb583ac4e1e3e1929edb118ebd696f3a4a1f54f59`

> Fixture readiness timeout only; the production adapter has no lifetime cap.

Imports: ../../app/workspace/native_process → app/workspace/native_process.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 5: `define main returning Int:`

## tests/workspace/test_publication_capability.ep

[Source](../../tests/workspace/test_publication_capability.ep) · 40 lines · 2330 bytes

SHA-256: `fe3633e70566512310dad2fde661cd3d453b7e4a76de0c89382752c06504e60c`


Imports: ../../app/workspace/create_text → app/workspace/create_text.ep, ../../app/workspace/read_text → app/workspace/read_text.ep

- Line 4: `define publication_contender with library as Int and root as Int and content as Str and completed as Int returning Int:`
- Line 9: `define main returning Int:`

## tests/workspace/test_read_text.ep

[Source](../../tests/workspace/test_read_text.ep) · 44 lines · 2954 bytes

SHA-256: `2c34963b78d1b7acd5c53e78e5485aa58e9df3a0880804827a607194321757a8`

> Replace the path after validation. The existing descriptor must still
> read the original inode and never follow the replacement symlink.

Imports: ../../app/workspace/read_text → app/workspace/read_text.ep

- Line 3: `define main returning Int:`

## tests/workspace/test_sandbox.ep

[Source](../../tests/workspace/test_sandbox.ep) · 74 lines · 3857 bytes

SHA-256: `c1cd2146cf8863d3f6b201447472f4e163e2d8d662d4e1ac55601e231630c79d`


Imports: ../../app/workspace/sandbox_profile → app/workspace/sandbox_profile.ep, ../../app/transport/local_socket → app/transport/local_socket.ep, ../../app/transport/http_stream → app/transport/http_stream.ep, fs → standard library or compiler dependency

- Line 6: `define main returning Int:`

## tests/workspace/test_sandbox_process.ep

[Source](../../tests/workspace/test_sandbox_process.ep) · 54 lines · 2817 bytes

SHA-256: `d40fe86af39971a13edb487d0488d30b4b0f8c35a086e94d781df0182c073e2e`

> A missing executable must fail before returning a live process handle.

Imports: ../../app/workspace/native_process → app/workspace/native_process.ep, ../../app/workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency, string → standard library or compiler dependency

- Line 6: `define main returning Int:`

## tests/workspace/test_sessions.ep

[Source](../../tests/workspace/test_sessions.ep) · 42 lines · 3063 bytes

SHA-256: `5dd7c432e91c8a1c270ee3598a396b8f767b61eef1aad6965dd3c28c7a57601d`


Imports: ../../app/workspace/sessions → app/workspace/sessions.ep, ../../app/web/run_store → app/web/run_store.ep

- Line 4: `define main returning Int:`

## tests/workspace/test_tool.ep

[Source](../../tests/workspace/test_tool.ep) · 100 lines · 7577 bytes

SHA-256: `9e05f4e50daab81c3afff290907168e18b1329979fef5248fc193dac157c47ae`


Imports: ../../app/tools/attachment_tools → app/tools/attachment_tools.ep, ../../app/web/run_store → app/web/run_store.ep

- Line 4: `define main returning Int:`
