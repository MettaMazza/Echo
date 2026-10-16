# Application source reference

Generated 2026-10-16T04:11:20.905495+00:00. Exact hashes and lexical signatures; not a claim that every path was executed.

## app/agent/checked_generation.ep

[Source](../../app/agent/checked_generation.ep) · 45 lines · 3220 bytes

SHA-256: `e5458d70c2110e60cc113b5f14bbbb1b11d506a114599654258b7c063df45888`

> Context composition supplies the exact body and workload output reserve.
> The ready event captured before discovery must still be current afterwards.
> A non-ready decision leaves the run ready for compaction/configuration work.
> Existing callers retain their launch configuration while using the same
> provider contract. New callers pass an authorized profile to the entry above.

Imports: ../retained_text → app/retained_text.ep, generation → app/agent/generation.ep, ../providers/prepare → app/providers/prepare.ep

- Line 8: `define agent_generate_provider_at with database as Int and run as Str and expected_parent as Str and request as Str and body as Str and profile as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int and choice_index as Str returning List:`
- Line 31: `define agent_generate_checked_at with database as Int and run as Str and expected_parent as Str and request as Str and body as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int and choice_index as Str returning List:`
- Line 39: `define agent_generate_checked with database as Int and run as Str and request as Str and body as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int and choice_index as Str returning List:`

## app/agent/control.ep

[Source](../../app/agent/control.ep) · 41 lines · 1660 bytes

SHA-256: `2b2b7ad847d28a04bcd48ecc5268b17e60fe1a0738d2026882d2ed4017befe56`

> Shared cancellation check for agent work and compaction between requests.

Imports: ../retained_text → app/retained_text.ep, checked_generation → app/agent/checked_generation.ep

- Line 4: `external define ep_uuid_v4`
- Line 6: `define agent_identity returning Str:`
- Line 10: `define agent_cancellation_pending with library as Int and reader as Int returning Int:`
- Line 28: `define agent_cancel_run with database as Int and run as Str returning Int:`

## app/agent/generation.ep

[Source](../../app/agent/generation.ep) · 120 lines · 7816 bytes

SHA-256: `4b67a8dc9060b56c70620cbaf7ef36d0afdd98d46cb025d117f8518f4450e02c`

> Keep the small identity envelope in a named live value while quoting a large
> multimodal body. Do not leave it as an intermediate nested-call result.
> Persist originals before making the selected response actionable. Selection
> is explicit; other choices remain in the archived provider events.
> Publication is one commit: originals, the complete tool ledger batch and
> the accepted run head cannot be separated by a process interruption.
> Queue the complete tool set and advance the exact generation in one
> transaction. Workers cannot claim a partial batch; stale replies roll back.
> One generation within the persistent ReAct run. No iteration counter or turn
> cap is imposed here. A tool dispatcher supplies observations before the next
> generation; the context builder supplies the complete token-budgeted request.

Imports: ../retained_text → app/retained_text.ep, run_state → app/agent/run_state.ep, ../providers/local_chat → app/providers/local_chat.ep

- Line 5: `define agent_event_envelope with run as Str and parent as Str returning Str:`
- Line 10: `define agent_request_envelope with run as Str and parent as Str and body as Str returning Str:`
- Line 17: `define agent_generation_fail with database as Int and run as Str and session as Str and request as Str and reason as Str returning Int:`
- Line 26: `define agent_generation_publish with database as Int and run as Str and session as Str and request as Str and state as List and choice_index as Str returning Int:`
- Line 91: `define agent_generate_local_at with database as Int and run as Str and expected_parent as Str and request as Str and body as Str and port as Int and path as Str and cancellation_reader as Int and read_size as Int and choice_index as Str returning Int:`
- Line 116: `define agent_generate_local with database as Int and run as Str and request as Str and body as Str and port as Int and path as Str and cancellation_reader as Int and read_size as Int and choice_index as Str returning Int:`

## app/agent/generation_recovery.ep

[Source](../../app/agent/generation_recovery.ep) · 41 lines · 2717 bytes

SHA-256: `adf406c507e3945ce7ef3dd0bbb155ff2d04450ae212ef514be97e523b7f7025`

> The caller must own the run's native worker lock. No provider request or tool
> action is replayed here. Complete streams publish atomically; partial streams
> remain archived and the run receives a new ready head for a fresh attempt.

Imports: ../retained_text → app/retained_text.ep, generation → app/agent/generation.ep, ../providers/restored_stream → app/providers/restored_stream.ep

- Line 8: `define agent_recover_generation_owned with database as Int and run as Str and recovery_event as Str and choice_index as Str returning Str:`

## app/agent/provider_profile.ep

[Source](../../app/agent/provider_profile.ep) · 58 lines · 3212 bytes

SHA-256: `81fd545556c0b703631bbccfdf55c8fefdf7a64a1c8b2d8d350dcd0c658c5cbc`

> The host binds a profile before the first model request. An existing binding
> is immutable: explicit new selection belongs to a new run, never a retry.
> Return 0 exact binding, 2 conflicting binding, 1 invalid state/storage error.

Imports: ../retained_text → app/retained_text.ep, run_state → app/agent/run_state.ep, ../providers/prepare → app/providers/prepare.ep

- Line 8: `define agent_provider_bind with database as Int and run as Str and profile as Str returning Int:`
- Line 49: `define agent_provider_read with database as Int and run as Str returning Str:`

## app/agent/run_messages.ep

[Source](../../app/agent/run_messages.ep) · 183 lines · 11831 bytes

SHA-256: `4280572351ac36ae0bdf193de030cd971ba49fd67a9a000a1a90792291b8979a`

> Recover a JSON subtree without normalizing numeric spelling or object order.
> Follow accepted parent links, excluding uncommitted/stale response branches.
> Sequences must strictly decrease, so corrupt cycles cannot spin indefinitely.
> Reconstruct this run's accepted exchanges. Session/project context and active
> compaction checkpoints are composed by the context layer, not silently omitted
> or substituted here. All tool messages retain the matching provider call ID.
> A compaction boundary must never separate a call from its result.
> Retain the whole exchange if any observation lies in the new suffix.

Imports: ../retained_text → app/retained_text.ep, tool_dispatch → app/agent/tool_dispatch.ep, user_content → app/agent/user_content.ep

- Line 6: `define agent_json_source with text as Str and tokens as List and index as Int returning Str:`
- Line 16: `define agent_run_lineage with database as Int and run as Str returning List:`
- Line 54: `define agent_run_messages_after with database as Int and run as Str and boundary as Int returning Str:`
- Line 182: `define agent_run_messages with database as Int and run as Str returning Str:`

## app/agent/run_state.ep

[Source](../../app/agent/run_state.ep) · 82 lines · 5579 bytes

SHA-256: `97c624bbdf3672ad33616863a61200f69f15d2c7d9fc31db9cbf6a1c9bc3c7fd`

> The event kind records why the state changes. The dispatcher validates the
> actual model/tool outcome before writing that event; storage never invents it.
> Compare the exact last event, not merely the state name. This rejects a late
> response from an earlier generation even after the run cycles to generating.
> Context compaction does not change run identity or reset pending tool work.
> The dispatcher uses this claim so cancellation and tool execution cannot race
> into a new action after a run has left its tools phase.

Imports: tool_ledger → app/agent/tool_ledger.ep

- Line 3: `define agent_run_schema with database as Int returning Int:`
- Line 8: `define agent_run_create with database as Int and run as Str and session as Str and objective_event as Str returning Int:`
- Line 19: `define agent_transition_kind with previous as Str and next_state as Str returning Str:`
- Line 45: `define agent_run_transition with database as Int and run as Str and previous as Str and expected_event as Str and next_state as Str and evidence_event as Str returning Int:`
- Line 59: `define agent_run_read with database as Int and run as Str returning List:`
- Line 80: `define agent_run_claim_tool with database as Int and run as Str and call_id as Str returning Int:`

## app/agent/runner.ep

[Source](../../app/agent/runner.ep) · 95 lines · 5624 bytes

SHA-256: `0c656adda2b2f6f61ce2b47df558e544f5ed641e3f4aa381d7abd5892ee054dc`

> The model/tool loop has no iteration limit. Non-ready preflight decisions are
> Capacity triggers archived recap/review/activation before resuming this run.
> Configuration and unresolved recap failures return explicitly, never truncate.
> Replay an already recorded historical route only; never select another model for new image input.

Imports: ../retained_text → app/retained_text.ep, control → app/agent/control.ep, provider_profile → app/agent/provider_profile.ep, ../context/compact → app/context/compact.ep

- Line 6: `define agent_loop_inner_provider with database as Int and run as Str and profile as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 62: `define agent_loop_provider with database as Int and run as Str and profile as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`
- Line 89: `define agent_loop_inner with database as Int and run as Str and model as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 93: `define agent_loop with database as Int and run as Str and model as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`

## app/agent/tool_dispatch.ep

[Source](../../app/agent/tool_dispatch.ep) · 125 lines · 9145 bytes

SHA-256: `19349a48b639449059e9deda46751dfcc2d62ee8c7bc96a5465d991b60c29397`

> One invocation executes at most the next cursor item. The enclosing run loops
> until the ledger is observed; this is scheduling granularity, not a turn cap.
> Do not advance another queued action past an unresolved earlier effect.
> Existing direct callers have no external cancellation descriptor.

Imports: ../retained_text → app/retained_text.ep, generation → app/agent/generation.ep, ../security/tool_policy → app/security/tool_policy.ep, ../tools/host_text → app/tools/host_text.ep, ../tools/memory_tools → app/tools/memory_tools.ep, ../tools/knowledge_tools → app/tools/knowledge_tools.ep, ../tools/protected_tools → app/tools/protected_tools.ep, ../tools/attachment_tools → app/tools/attachment_tools.ep, ../tools/execution_tool → app/tools/execution_tool.ep, ../media/image_tool → app/media/image_tool.ep, ../media/speech_tool → app/media/speech_tool.ep, ../context/recap_rebuild → app/context/recap_rebuild.ep

- Line 16: `define agent_tool_step_cancellable with database as Int and run as Str and cancellation as Int returning Int:`
- Line 124: `define agent_tool_step with database as Int and run as Str returning Int:`

## app/agent/tool_ledger.ep

[Source](../../app/agent/tool_ledger.ep) · 80 lines · 6430 bytes

SHA-256: `57c47f00d2ebbf50257b473a81b34ce4eb2b651e4911f90d4b1b4eb9cedacad1`

> The runner persists original tool-call and result events before referring to
> them here. State changes are atomic, and only one worker can claim a call.
> Invoke only after the owning worker/process is confirmed dead. An interrupted
> side effect may have happened even though its result was never recorded.
> A real persisted result can resolve running or uncertain work. Its session and
> call identity must match; another call's observation cannot close this call.
> Requeue requires a persisted reconciliation observation from the tool adapter.
> This API records the evidence; the adapter must actually verify its claim.

Imports: ../context/checkpoint → app/context/checkpoint.ep

- Line 5: `define tool_ledger_schema with database as Int returning Int:`
- Line 8: `define tool_ledger_change with database as Int and query as Str and values as List returning Int:`
- Line 17: `define tool_ledger_identity with run as Str and call_id as Str returning List:`
- Line 23: `define tool_ledger_queue with database as Int and run as Str and call_id as Str and session as Str and request_event as Str returning Int:`
- Line 33: `define tool_ledger_claim with database as Int and run as Str and call_id as Str returning Int:`
- Line 39: `define tool_ledger_interrupt with database as Int and run as Str and call_id as Str returning Int:`
- Line 45: `define tool_ledger_observe with database as Int and run as Str and call_id as Str and result_event as Str returning Int:`
- Line 55: `define tool_ledger_confirm_not_executed with database as Int and run as Str and call_id as Str and evidence_event as Str returning Int:`
- Line 63: `define tool_ledger_read with database as Int and run as Str and call_id as Str returning List:`

## app/agent/tool_recovery.ep

[Source](../../app/agent/tool_recovery.ep) · 48 lines · 2786 bytes

SHA-256: `d04a0bb748717768e849786de15c841ceb03d525e6367fea4de2833ff5bdb3fc`

> Caller owns the native run lock and has verified the abandoned worker claim.
> Recover only original saved results. Missing outcomes never authorize replay.

Imports: ../retained_text → app/retained_text.ep, run_state → app/agent/run_state.ep

- Line 6: `define agent_recover_tools_owned with database as Int and run as Str returning Int:`

## app/agent/user_content.ep

[Source](../../app/agent/user_content.ep) · 40 lines · 2817 bytes

SHA-256: `b14affa2f2a94d84d0f528345e7821d766c75627b3b52575060bd2ba6bdbb702`

> Rebuild each user's selected sources from the durable original receipt.
> A missing or changed image fails the request instead of silently dropping it.

Imports: ../retained_text → app/retained_text.ep, ../archive/turn_attachments → app/archive/turn_attachments.ep, ../providers/attachment_image → app/providers/attachment_image.ep

- Line 7: `define agent_user_content with database as Int and session as Str and event as Str and text as Str returning Str:`

## app/archive/attachment_restore.ep

[Source](../../app/archive/attachment_restore.ep) · 95 lines · 6530 bytes

SHA-256: `eb918a8cb15048a23766f907d8654fa2d00953ebfe874e440c483575b212c391`

> Offline candidate only. Originals and hashes are unchanged; only host storage
> locations are rebound inside a transaction, with immutability restored before
> commit. Uploads without completed attachments cannot resume old paths.
> This gate prevents a historical backup from silently replaying actions that
> may have completed after its snapshot. Reconciliation is a separate workflow.
> Attachment bytes were flushed by file_copy_verified; flush their directory
> before publishing the database that references them.

Imports: ../retained_text → app/retained_text.ep, file_publication → app/archive/file_publication.ep, attachment_snapshot → app/archive/attachment_snapshot.ep, ../web/uploads → app/web/uploads.ep

- Line 9: `define attachment_restore_relocate with database as Int and backup_root as Str and destination_root as Str returning Int:`
- Line 48: `define attachment_restore_start_allowed with database as Int returning Int:`
- Line 60: `define attachment_restore_reserved with library as Int and source as Str and backup_root as Str and destination as Str and destination_root as Str returning Int:`
- Line 87: `define attachment_restore with source as Str and backup_root as Str and destination as Str and destination_root as Str returning Int:`

## app/archive/attachment_snapshot.ep

[Source](../../app/archive/attachment_snapshot.ep) · 78 lines · 5206 bytes

SHA-256: `7c222af7bb34d3ef6c6f0580de2ded4479b671f4ba66e096204ec5b21cd4f2ff`

> Caller chooses a new private snapshot and existing host-controlled directory.
> Paths use a digest of the opaque ID, never an uploaded filename or raw ID.
> Rehash every copied file and require complete one-to-one metadata coverage.
> Root may move; no saved absolute backup path is followed.

Imports: ../retained_text → app/retained_text.ep, snapshot → app/archive/snapshot.ep, file_copy → app/archive/file_copy.ep, history → app/archive/history.ep, ../context/checkpoint → app/context/checkpoint.ep

- Line 9: `define attachment_snapshot_path with root as Str and attachment as Str returning Str:`
- Line 12: `define attachment_snapshot_copy with database as Int and root as Str returning Int:`
- Line 55: `define attachment_snapshot_verify with database as Int and root as Str returning Int:`

## app/archive/backup_bundle.ep

[Source](../../app/archive/backup_bundle.ep) · 161 lines · 9832 bytes

SHA-256: `dfb80b5b50de7071e9c8fdc1bb54fcf430df50347b3f1594877c7c0a0e8d5898`

> Host-only profile. Each tree and individual file is explicit; weights are
> not silently duplicated. Runtime ownership prevents Echo from restarting
> while its database and files are copied. Other host editors must be stopped.
> Failed runs are preserved, not retried or erased. Unobserved tools and
> unfinished processes still refuse backup; restores retain execution hold.

Imports: attachment_restore → app/archive/attachment_restore.ep, tree_snapshot → app/archive/tree_snapshot.ep, ../providers/archived_stream → app/providers/archived_stream.ep, ../lifecycle/service_profile → app/lifecycle/service_profile.ep

- Line 9: `define backup_bundle_identity with path as Str returning Str:`
- Line 15: `define backup_bundle_absolute with value as Str returning Int:`
- Line 20: `define backup_bundle_valid with tokens as List returning Int:`
- Line 55: `define backup_bundle_copy with library as Int and database as Int and tokens as List and profile as Str returning Int:`
- Line 136: `define backup_bundle_run with profile as Str returning Int:`

## app/archive/event_store.ep

[Source](../../app/archive/event_store.ep) · 220 lines · 12249 bytes

SHA-256: `4aad681e6e10138bb9922b333cd392590c3ec0094050220aaca25faaf0e77784`

> Echo owns its original history independently of prompts, summaries and caches.
> SQLite constants below are protocol result codes, not application limits.
> Report the failed operation without exposing archived message contents.
> These wrappers preserve database errors; an empty query is not an error signal.
> Native workers, status readers and cancellation monitors use separate
> connections. Give short write locks time to clear instead of failing on
> the first SQLITE_BUSY. This is a database lock wait, not a model deadline.
> Rollback journals avoid assuming shared-memory/WAL support on the drive.
> Length-prefix every field so embedded separators cannot change hash boundaries.
> Named strings remain explicit compiler GC roots while another worker
> allocates. Nested temporary arguments can be missed by minor collection.
> Internal transaction participant. Caller holds BEGIN IMMEDIATE and owns commit
> or rollback, allowing a message and its run to become durable together.
> Keep an owned chain value across later query steps.
> Read the inserted bytes back before the caller commits. A mismatch must
> abort the transaction, rather than becoming an apparently saved event.
> Caller supplies a stable event ID; a retry must never silently duplicate it.
> The transaction serializes chain construction across concurrent connections.
> Read one event by its exact stable identity without a prompt-size restriction.
> A zero result means missing/error; the caller must not invent replacement text.
> Stream verification one row at a time; no growing whole-session buffer.

Imports: ../retained_text → app/retained_text.ep, sql → standard library or compiler dependency, string → standard library or compiler dependency

- Line 7: `external define sqlite3_extended_errcode with database as Int returning Int`
- Line 10: `define archive_failure_detail with database as Int and operation as Str returning Int:`
- Line 17: `define archive_prepare with database as Int and query as Str returning Int:`
- Line 23: `define archive_bind with statement as Int and position as Int and value as Str returning Int:`
- Line 29: `define archive_finish with statement as Int and expected as Int returning Int:`
- Line 39: `define archive_open with path as Str returning Int:`
- Line 60: `define archive_hash_field with value as Str returning Str:`
- Line 67: `define archive_digest with session as Str and event as Str and kind as Str and payload as Str and occurred as Str and previous as Str returning Str:`
- Line 82: `define archive_rollback with database as Int returning Int:`
- Line 90: `define archive_append_pending with database as Int and session as Str and event as Str and kind as Str and payload as Str and occurred as Str returning Int:`
- Line 149: `define archive_append with database as Int and session as Str and event as Str and kind as Str and payload as Str and occurred as Str returning Int:`
- Line 163: `define archive_read_event with database as Int and event as Str returning List:`
- Line 191: `define archive_verify with database as Int and session as Str returning Int:`

## app/archive/export_restore.ep

[Source](../../app/archive/export_restore.ep) · 80 lines · 4930 bytes

SHA-256: `8d0b06cd6094513a0b3a88e23e796c91c5306ecae716c07328dd92446da10032`

> Restore originals only into an empty archive. Application state and device
> authority are deliberately not inferred from arbitrary imported event text.
> Verification above establishes every complete prefix and its parent anchor.
> Preserve original sequence positions because branch boundaries reference them.
> Restoring does not manufacture device authority, runnable tasks or file bytes.

Imports: export_verify → app/archive/export_verify.ep

- Line 5: `define archive_export_restore with database as Int and text as Str and trusted_head as Str returning Int:`
- Line 40: `define archive_branch_export_restore with database as Int and text as Str returning Int:`

## app/archive/export_verify.ep

[Source](../../app/archive/export_verify.ep) · 127 lines · 8027 bytes

SHA-256: `cd534ba5bb3ad1f28cb087a917640f90ac3018e55b17c6d3dfd8e1c8fb30eb11`

> Hash chains detect corruption, not authorship. A separately trusted head can
> also reject a consistently rewritten or shortened export.
> Extract one exact JSON object without reserialising payload strings.

Imports: event_store → app/archive/event_store.ep, ../providers/json_access → app/providers/json_access.ep

- Line 6: `define archive_export_verify_single with text as Str and trusted_head as Str returning Int:`
- Line 42: `define archive_export_part with text as Str and tokens as List and index as Int returning Str:`
- Line 50: `define archive_export_verify with text as Str and trusted_head as Str returning Int:`

## app/archive/file_base64.ep

[Source](../../app/archive/file_base64.ep) · 134 lines · 7090 bytes

SHA-256: `cd65b4d79f9cb56f38dfc8db74479d415089848dab47025d9d0db650126edddf`

> Encode and hash the same binary descriptor stream; preserve NUL/high bytes.
> The inline provider request requires retaining the encoded payload in memory.
> Batch bounds are CommonCrypto ABI limits, never an attachment-size ceiling.
> CC_SHA256_CTX is 26 unsigned 32-bit words in the installed SDK.

Imports: file_digest → app/archive/file_digest.ep

- Line 3: `external define peek_byte with pointer as Int and offset as Int returning Int`
- Line 4: `external define poke_byte with pointer as Int and offset as Int and value as Int returning Int`
- Line 6: `define file_base64_group with output as Int and offset as Int and value as Int and bytes as Int returning Int:`
- Line 23: `define file_base64_verified with path as Str and expected_digest as Str and expected_bytes as Int and batch as Int returning List:`

## app/archive/file_copy.ep

[Source](../../app/archive/file_copy.ep) · 59 lines · 2703 bytes

SHA-256: `a8a50f1075e02a08ed89687ae8d8e5951705704a4f798730df565f7649f4c250`

> Host-only exclusive copy. Success requires flushed destination bytes matching
> the archive's recorded digest and length. Failure leaves an unpublished
> candidate for inspection, never overwrites an existing backup file.

Imports: ../retained_text → app/retained_text.ep, file_stage → app/archive/file_stage.ep

- Line 7: `define file_copy_verified with source as Str and destination as Str and digest as Str and bytes as Int and batch as Int returning Int:`

## app/archive/file_digest.ep

[Source](../../app/archive/file_digest.ep) · 121 lines · 6075 bytes

SHA-256: `75355c42e8cd33b4c522f9466604a98ce689c4e7d4aeb16156fab8c658417d1b`

> Binary streaming SHA-256 via the installed macOS CommonCrypto ABI.
> Batch size is I/O granularity, not a file-size ceiling. Never use C strings
> for file contents: attachments may include NUL and arbitrary byte values.
> Compaction requires a flushed regular original. Hash the same descriptor
> after fsync; nonblocking/no-symlink opening rejects special-file substitutions.
> O_CLOEXEC | O_NOFOLLOW_ANY | O_NONBLOCK. Darwin rejects combining
> O_NOFOLLOW_ANY with the separate O_NOFOLLOW flag.
> CC_SHA256_CTX is 26 unsigned 32-bit words in the installed SDK.

Imports: ../transport/socket_read → app/transport/socket_read.ep, string → standard library or compiler dependency

- Line 4: `define file_digest_result with status as Str and digest as Str and bytes as Int returning List:`
- Line 14: `define file_digest with path as Str and batch as Int returning List:`
- Line 19: `define file_digest_durable with path as Str and batch as Int returning List:`
- Line 22: `define file_digest_mode with path as Str and batch as Int and durable as Int returning List:`

## app/archive/file_publication.ep

[Source](../../app/archive/file_publication.ep) · 61 lines · 3000 bytes

SHA-256: `92d740779430d4f615e9155f417a96335c092a369bf2bffe0b9aed9dbb13a34d`

> Host-controlled paths only. Permanent reservations serialize cooperating Echo
> publishers on APFS and ExFAT. Failed attempts retain evidence for inspection.
> This does not exclude unrelated processes with write access to these paths.
> ExFAT does not support RENAME_EXCL. An exclusive mkdir reservation
> serializes cooperating Echo writers without requiring hard links.
> Darwin ENOTSUP45 observed on One Touch. The permanent reservation
> makes this fallback exclusive among Echo publishers. Refuse any
> destination already present, including a dangling symlink.
> Darwin O_CLOEXEC | O_NOFOLLOW_ANY | O_DIRECTORY, read-only directory.

Imports: file_stage → app/archive/file_stage.ep

- Line 6: `define file_publication_reserve with system as Int and path as Str returning Int:`
- Line 22: `define file_publication_finish with system as Int and temporary as Str and path as Str returning Int:`

## app/archive/file_stage.ep

[Source](../../app/archive/file_stage.ep) · 44 lines · 2040 bytes

SHA-256: `1e237d9025c3d294c4703c4c350da9be3044ea1fdb6959feb7feecceb6332a34`

> Staging paths are generated by the host, never derived from upload names.
> Failed candidates stay unregistered; callers may inspect/remove them later.
> Darwin O_RDWR|O_CREAT|O_EXCL|O_NOFOLLOW|O_CLOEXEC, mode 0600.
> Always closes the descriptor, including on flush failure. Success means the
> file was flushed; directory publication is a separate storage operation.

Imports: ../transport/native_open → app/transport/native_open.ep, file_digest → app/archive/file_digest.ep

- Line 6: `define file_stage_open with library as Int and path as Str returning Int:`
- Line 13: `define file_stage_write with library as Int and descriptor as Int and pointer as Int and bytes as Int returning Int:`
- Line 33: `define file_stage_finish with library as Int and descriptor as Int returning Int:`

## app/archive/file_text.ep

[Source](../../app/archive/file_text.ep) · 173 lines · 9200 bytes

SHA-256: `3990543bad2d1c10b352beaf27a578c7702bfb0e63590fed79a7d2ad5015ed92`

> Only the requested Unicode passage is retained. Hashing and UTF-8 validation
> cover the same complete descriptor stream, including bytes outside the range.
> This scanner borrows a freshly opened descriptor at offset zero. It does not
> close or reopen it. Empty expected digest or byte count -1 requests a new
> snapshot: status read, with observed SHA256 and byte count at indexes3 and4.
> Verified means both supplied expectations matched the complete byte stream.
> CC_SHA256_CTX is 26 unsigned 32-bit words in the installed SDK.
> Compatibility helper for callers explicitly requesting the entire text.

Imports: file_digest → app/archive/file_digest.ep

- Line 3: `external define peek_byte with pointer as Int and offset as Int returning Int`
- Line 7: `define file_text_result with status as Str and text as Str and characters as Int returning List:`
- Line 18: `define file_text_descriptor_window with library as Int and descriptor as Int and expected_digest as Str and expected_bytes as Int and batch as Int and offset as Int and characters as Int returning List:`
- Line 151: `define file_text_window with path as Str and expected_digest as Str and expected_bytes as Int and batch as Int and offset as Int and characters as Int returning List:`
- Line 172: `define file_text_read with path as Str and expected_digest as Str and expected_bytes as Int and batch as Int returning List:`

## app/archive/history.ep

[Source](../../app/archive/history.ep) · 83 lines · 4932 bytes

SHA-256: `f310e713d81133b53d57cf48b94c2785a6619cfd7faefdd7e3692905e20e6695`

> Every result explicitly distinguishes exhaustion from a database failure.
> Returned historical content is data with its original role, not instructions.
> Capture a finite source boundary for a scan. Later events remain retrievable
> in the next scan; appending messages cannot keep this scan running forever.
> Cursor traversal has no total result cap. LIMIT 1 advances a single cursor;
> callers continue until end, with no fixed number of calls or messages.
> An empty literal searches all events. Percent signs and SQL text are literal.
> Fields: status, event, sequence, kind, occurred, character count, digest.
> Offsets and lengths count Unicode characters as defined by SQLite, not UTF-8
> bytes. The caller sizes a piece using the live provider budget/tokenizer.
> No default excerpt size truncates the source. Empty final pieces are valid.
> Fields: status, event, kind, occurred, total characters, digest, offset, piece.

Imports: session_scope → app/archive/session_scope.ep, event_store → app/archive/event_store.ep

- Line 6: `define history_status with status as Str returning List:`
- Line 11: `define history_collect_one with statement as Int returning List:`
- Line 34: `define history_boundary with database as Int and session as Str returning List:`
- Line 47: `define history_next with database as Int and session as Str and after_sequence as Int and through_sequence as Int and literal as Str returning List:`
- Line 67: `define history_read_piece with database as Int and session as Str and event as Str and offset as Int and characters as Int returning List:`

## app/archive/pdf_cache.ep

[Source](../../app/archive/pdf_cache.ep) · 84 lines · 6322 bytes

SHA-256: `efdeeec977e712ff94e8fe523003552e742b8683fe5b7ca508bffe77fb3c6459`

> Cache versions include the OS build because PDFKit extraction can change
> between system releases. The algorithm prefix changes when this adapter does.
> Caller has already resolved ownership and source path. Every cache hit checks
> original file integrity as well as the derived payload's own digest.

Imports: ../retained_text → app/retained_text.ep, pdf_text → app/archive/pdf_text.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 7: `define pdf_extractor_version returning Str:`
- Line 27: `define pdf_cache_schema with database as Int returning Int:`
- Line 30: `define pdf_cache_result with status as Str and text as Str and pages as Int and version as Str and cached as Int returning List:`
- Line 41: `define pdf_cached_page with database as Int and event as Str and path as Str and digest as Str and bytes as Int and page as Int returning List:`

## app/archive/pdf_text.ep

[Source](../../app/archive/pdf_text.ep) · 82 lines · 5528 bytes

SHA-256: `26bc9d9cbcc71f21d22e82b8ab3603545e68eaa2e54c0a41cc6b975988fa4a8b`

> Decode the verified original bytes into immutable NSData before PDFKit sees
> them. This avoids reopening a mutable path between hashing and extraction.
> The first implementation holds the encoded and decoded file in memory.
> Page minus one is the internal whole-document compatibility path.
> Objective-C classes remain registered; keep framework handles loaded.

Imports: ../retained_text → app/retained_text.ep, file_base64 → app/archive/file_base64.ep

- Line 7: `define pdf_text_verified with path as Str and digest as Str and bytes as Int returning List:`
- Line 11: `define pdf_text_page_verified with path as Str and digest as Str and bytes as Int and page as Int returning List:`

## app/archive/restore_bundle.ep

[Source](../../app/archive/restore_bundle.ep) · 137 lines · 8852 bytes

SHA-256: `1f2e13297b668c4cc01b9e0405326de85b20c7f99860d2e5121318f55de95b59`

> A retained top-level manifest hash is the recovery trust input. Paths from
> the original profile are documentary only; recovery reads exclusively from
> the chosen bundle and writes beneath a new host-chosen destination.

Imports: backup_bundle → app/archive/backup_bundle.ep

- Line 6: `define restore_bundle_content_valid with tokens as List and object as Int and path as Str returning Int:`
- Line 19: `define restore_bundle_components_valid with tokens as List and bundle as Str returning Int:`
- Line 64: `define restore_bundle_copy with library as Int and tokens as List and bundle as Str and destination as Str and expected as Str returning Int:`
- Line 119: `define restore_bundle_run with bundle as Str and expected as Str and destination as Str returning Int:`

## app/archive/restore_release.ep

[Source](../../app/archive/restore_release.ep) · 53 lines · 4112 bytes

SHA-256: `6e735e3d060e3b024e2d576c5b67b03c79a38ed4e2f3eb5928fd5d61acc2e596`

> No pending historical action is automatically retried or marked completed.
> This release mode applies only to a snapshot whose recorded runs, tools and
> processes are already terminal. Uncertain snapshots stay held for review.

Imports: attachment_restore → app/archive/attachment_restore.ep

- Line 6: `define restore_release_transaction with database as Int and attachment_root as Str and reviewed_hash as Str returning Int:`
- Line 29: `define restore_release with path as Str and attachment_root as Str and expected_hash as Str returning Int:`

## app/archive/runtime_lock.ep

[Source](../../app/archive/runtime_lock.ep) · 58 lines · 3136 bytes

SHA-256: `f6cfbb12a1fd981315375f6a4468a35360af4ad34b444684cca04963a2d52209`

> Copies retain this lock location. Do not delete lock files or silently change
> the anchor when moving a restored archive on the same host.

Imports: ../transport/native_open → app/transport/native_open.ep, event_store → app/archive/event_store.ep, ../transport/local_socket → app/transport/local_socket.ep

- Line 7: `define archive_runtime_anchor with database as Int and library as Int returning Str:`
- Line 44: `define archive_runtime_lock with database as Int and library as Int returning Int:`

## app/archive/session_scope.ep

[Source](../../app/archive/session_scope.ep) · 5 lines · 1144 bytes

SHA-256: `99555fb6801d97a0714972c1ba64a9bc9a833d52ad3be59638e685bda5c38a5c`

> Resolve immutable branch ancestry at recorded source endpoints. Every parent
> endpoint predates its branch event; descendants cannot expose later messages.
> UNION also deduplicates repeated ancestry without imposing a depth ceiling.

Imports: 

- Line 4: `define session_scope_sql returning Str:`

## app/archive/snapshot.ep

[Source](../../app/archive/snapshot.ep) · 101 lines · 4762 bytes

SHA-256: `0e27b01f85355240495352944dbdf6d6b430281b7c6a78b58d826e6c4c15e53c`

> Verify the database structure and every session without loading all events.
> SQLite takes a consistent snapshot while the application can keep its own
> connection. The destination must be new; an earlier backup is never replaced.
> A FULL-synchronous receipt transaction flushes the output before success.
> A failed or interrupted candidate must not be advertised as a usable backup.
> A restore is a fresh snapshot into an explicitly chosen internal destination.
> Validate completion, row coverage and original hashes before copying anything.

Imports: event_store → app/archive/event_store.ep, fs → standard library or compiler dependency, runtime_lock → app/archive/runtime_lock.ep

- Line 6: `define archive_verify_all with database as Int returning Int:`
- Line 46: `define archive_snapshot with database as Int and destination as Str returning Int:`
- Line 79: `define archive_restore with source as Str and destination as Str returning Int:`

## app/archive/tree_sidecars.ep

[Source](../../app/archive/tree_sidecars.ep) · 65 lines · 4251 bytes

SHA-256: `04a034d6426c702adcc65b0d76b788a73ee3c72b8683665d1d31efac7d9835c5`

> macOS tar merges AppleDouble entries when the corresponding file exists,
> even with metadata translation disabled. Preserve original sidecar bytes
> through the native reader, without using its disk writer or copyfile.
> Paths remain relative to an opened restore directory; existing files are
> not replaced. The complete mtree verification follows this recovery pass.

Imports: ../workspace/files → app/workspace/files.ep

- Line 8: `define tree_sidecar_entries with system as Int and archive_library as Int and reader as Int and root as Int returning Int:`
- Line 42: `define tree_restore_sidecars with system as Int and archive_path as Str and root as Int returning Int:`

## app/archive/tree_snapshot.ep

[Source](../../app/archive/tree_snapshot.ep) · 173 lines · 11145 bytes

SHA-256: `da8244ea4ab689056c2a1b230908b45bf9a18d01f6932da8a4df73eb9b3f2e28`

> Host-only offline tree backup. The caller must stop all writers first.
> macOS tar and mtree are upstream dependencies, invoked with explicit argv;
> all orchestration, failure handling and publication logic remains Ernos.
> -p enables metadata flags, so disable translation after that option.
> Never let an archive or restore candidate be created inside its source.
> New destinations only. Failed candidates remain available for inspection.
> Recovery depends only on the saved bundle and independently retained hashes.
> It never modifies the bundle or starts any recovered program.
> Detect replacement or damage during extraction as well as before it.

Imports: tree_sidecars → app/archive/tree_sidecars.ep, fs → standard library or compiler dependency, ../workspace/native_process → app/workspace/native_process.ep, ../workspace/files → app/workspace/files.ep

- Line 6: `define tree_snapshot_realpath with path as Str returning Str:`
- Line 19: `define tree_snapshot_command with library as Int and executable as Str and arguments as List and directory as Int and output_path as Str and errors_path as Str returning Int:`
- Line 50: `define tree_snapshot_verify_at with library as Int and directory as Int and bundle as Str and audit as Str and label as Str returning Int:`
- Line 56: `define tree_snapshot_verify with library as Int and directory as Int and bundle as Str and label as Str returning Int:`
- Line 59: `define tree_archive_extract with library as Int and bundle as Str and target as Int and audit as Str returning Int:`
- Line 77: `define tree_snapshot with source as Str and bundle as Str and restored as Str returning Int:`
- Line 148: `define tree_restore_bundle with bundle as Str and archive_hash as Str and manifest_hash as Str and restored as Str returning Int:`

## app/archive/turn_attachments.ep

[Source](../../app/archive/turn_attachments.ep) · 41 lines · 3110 bytes

SHA-256: `5c2df94f1b0427e0230fca0be8f6fd76acb6af798dbb2bf39da9bde5c97c1df4`

> Resolve selected identities under the submission transaction. The immutable
> receipt records original provenance without placing host paths in context.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 7: `define turn_attachment_manifest with database as Int and device as Str and session as Str and event as Str and attachments as List returning Str:`
- Line 37: `define turn_attachment_read with database as Int and session as Str and event as Str returning List:`

## app/backup.ep

[Source](../../app/backup.ep) · 24 lines · 1056 bytes

SHA-256: `163a3cab863d44550db7789309abfb311b4ae975d24430b49362c766393788f1`


Imports: archive/snapshot → app/archive/snapshot.ep

- Line 3: `define main returning Int:`

## app/backup_bundle.ep

[Source](../../app/backup_bundle.ep) · 12 lines · 573 bytes

SHA-256: `3cb9322ba0d8f50d2b17d8d5af99e90638ac8cce5e5eb4492f53ecbefdeddafb`


Imports: archive/backup_bundle → app/archive/backup_bundle.ep

- Line 3: `define main returning Int:`

## app/backup_files.ep

[Source](../../app/backup_files.ep) · 27 lines · 1363 bytes

SHA-256: `2e1273e8322fd2aa9f99e143d7d800c1bd3aef6221b7bf9d2b28541a9fc5d3db`


Imports: archive/attachment_snapshot → app/archive/attachment_snapshot.ep

- Line 3: `define main returning Int:`

## app/backup_tree.ep

[Source](../../app/backup_tree.ep) · 11 lines · 735 bytes

SHA-256: `9f432faf253c9cd179fb952d807bbd3a03bab195fff7f932921c031089c136a8`


Imports: archive/tree_snapshot → app/archive/tree_snapshot.ep

- Line 3: `define main returning Int:`

## app/capture_activation.ep

[Source](../../app/capture_activation.ep) · 5 lines · 176 bytes

SHA-256: `e9aa2b3faf089bc6b813be7ebed26805652c597d3f40cfaa33438768e438ba9b`

> Host-only native research entry point; not exposed to remote chat devices.

Imports: research/capture_activation → app/research/capture_activation.ep

- Line 4: `define main returning Int:`

## app/compare_intervention.ep

[Source](../../app/compare_intervention.ep) · 4 lines · 102 bytes

SHA-256: `d8f80b413d1131ee1256012d64c9066d69377151c149b595e3be2b96b3760dc7`


Imports: research/compare_intervention → app/research/compare_intervention.ep

- Line 3: `define main returning Int:`

## app/context/attachment_gate.ep

[Source](../../app/context/attachment_gate.ep) · 53 lines · 4090 bytes

SHA-256: `9531a4d7c667a1a51c920a5afaa50e29cf2c4be70b8643d87aa14463ca119fc3`

> Frozen branch ancestry, bounded by the exact endpoint being compacted.
> All original attachment events in that scope must retain matching metadata
> and flushed original bytes. Generated images use the same attachment store.
> Caller supplies a stable database snapshot/transaction. No model text can
> choose a path. Missing storage is a recoverable failure, never source removal.

Imports: ../retained_text → app/retained_text.ep, ../archive/file_digest → app/archive/file_digest.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 8: `define compaction_attachment_scope returning Str:`
- Line 13: `define compaction_attachments_verify with database as Int and session as Str and endpoint as Str returning Str:`

## app/context/budget.ep

[Source](../../app/context/budget.ep) · 48 lines · 2280 bytes

SHA-256: `67b02fd3424c79fa3327526352a064c3153457f373f5189beac7e603a262d292`

> All capacities and token counts arrive from the provider/tokenizer or workload.
> A negative result is invalid metadata, never a guessed replacement capacity.
> A provider output ceiling of -1 explicitly means no separate ceiling.
> Zero or other negative values mean unresolved/invalid provider information.
> Reserve each known contribution separately to avoid overflowing integer sums.
> Result: -1 invalid input, 0 compaction required, 1 request fits as assembled.
> The output reserve is chosen from the actual next workload, not a fixed default.
> Comparison is exact: reductions must be surfaced instead of called maximum.

Imports: 

- Line 4: `define context_remaining with capacity as Int and occupied as Int returning Int:`
- Line 13: `define context_output_allowance with capacity as Int and occupied as Int and provider_output_ceiling as Int returning Int:`
- Line 28: `define context_request_fits with capacity as Int and occupied as Int and next_input as Int and tool_overhead as Int and output_reserve as Int returning Int:`
- Line 43: `define context_loaded_at_maximum with requested as Int and loaded as Int returning Int:`

## app/context/checkpoint.ep

[Source](../../app/context/checkpoint.ep) · 133 lines · 8539 bytes

SHA-256: `f15d64ae80364158bb452410b9598f2d9ff7c4d615988832ae8b5e6f1e05c8bb`

> This module persists checkpoints. The caller must generate and validate recap
> coverage before activation; a nonempty validation record is not that validator.
> Stage a frozen, verified source prefix while holding the writer lock. Recap
> generation may append records after the captured endpoint; they remain suffix.
> Repeated compaction must advance beyond the previous checkpoint's endpoint.
> Compare and switch is atomic. Two candidates based on the same old context
> cannot overwrite one another. The prior checkpoint and all originals survive.
> Caller owns the transaction so context rechecks and activation can commit
> atomically. Failure must roll back that transaction.
> Zero means no active checkpoint or a read error. Callers retain raw history.
> Returned fields include the exact source endpoint for replaying the suffix.
> Capture source identity and previous checkpoint in one read snapshot before
> asking the model to summarise. Fields: endpoint, digest, sequence, previous.
> Zero means no source or an error; callers must retain the current context.

Imports: ../retained_text → app/retained_text.ep, ../archive/event_store → app/archive/event_store.ep

- Line 6: `define checkpoint_schema with database as Int returning Int:`
- Line 9: `define checkpoint_prepare with database as Int and query as Str and values as List returning Int:`
- Line 25: `define checkpoint_stage with database as Int and identity as Str and session as Str and source_end as Str and previous as Str and summary as Str and structured_state as Str and validation as Str returning Int:`
- Line 56: `define checkpoint_activate with database as Int and session as Str and identity as Str returning Int:`
- Line 67: `define checkpoint_activate_pending with database as Int and session as Str and identity as Str returning Int:`
- Line 86: `define checkpoint_read_active with database as Int and session as Str returning List:`
- Line 108: `define checkpoint_capture with database as Int and session as Str returning List:`

## app/context/compact.ep

[Source](../../app/context/compact.ep) · 129 lines · 9076 bytes

SHA-256: `af85656bf91662dfb78c63c6d3c43667418b0c893ff069069acf813d1a70dfd5`

> Compaction preserves the run identity and accepted tool lineage. Every stage
> archives its evidence; only reviewed, fitting context reaches activation.
> Oversized merge/review/repair inputs remain explicit recoverable results.
> No arbitrary retry ceiling. Repeated candidates are a detected cycle;
> changed candidates must independently pass review before activation.
> Compatibility entry for existing host llama.cpp launch settings.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_accept → app/context/recap_accept.ep, recap_collect → app/context/recap_collect.ep, recap_merge → app/context/recap_merge.ep, recap_repair → app/context/recap_repair.ep, compaction_lock → app/context/compaction_lock.ep, compaction_resume → app/context/compaction_resume.ep

- Line 9: `define context_compact_provider with database as Int and run as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 27: `define context_compact_owned_provider with database as Int and run as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 122: `define context_compact with database as Int and run as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 127: `define context_compact_owned with database as Int and run as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`

## app/context/compaction_lock.ep

[Source](../../app/context/compaction_lock.ep) · 38 lines · 2104 bytes

SHA-256: `95889d4a578365c661579d2522845500b44cf73322757d6eb0ae3b004b92cc09`

> A host-local advisory lock prevents simultaneous compaction of one archive
> session. The kernel releases it on process death; it has no timeout or lease
> expiry that could grant ownership while an earlier worker is still running.
> Lock files must not be deleted during normal operation (inode identity matters).
> Darwin flags from the installed SDK: O_RDWR|O_CREAT|O_NOFOLLOW|O_CLOEXEC;
> permission 0600, LOCK_EX|LOCK_NB. These are ABI constants, not work limits.

Imports: ../transport/native_open → app/transport/native_open.ep, ../agent/control → app/agent/control.ep

- Line 8: `define compaction_lock_acquire with database as Int and session as Str and library as Int returning Int:`

## app/context/compaction_resume.ep

[Source](../../app/context/compaction_resume.ep) · 163 lines · 10114 bytes

SHA-256: `0eb9e3c9c74b4ac48a075794d2f3029eb4a0c6732c28d996f5205243ccffece4`

> Call only while owning the session compaction lock. A job belongs to the exact
> accepted run head and prior checkpoint; changed work starts a different job.
> Revalidate saved review evidence against the exact candidate and originals.
> Invalid reports can be retried; valid repair feedback and passes are reused.
> Only complete candidate receipts are reusable. Requests without a receipt are
> abandoned attempts; a subsequent dispatch always receives a fresh identity.
> Return status, candidate, request identity, earlier candidate hashes.
> A completed synthesis is reusable only after rechecking original coverage.
> Result: verdict, report, exact receipt identity (no suffix reconstruction).

Imports: ../retained_text → app/retained_text.ep, recap_jobs → app/context/recap_jobs.ep, recap_review_synthesis → app/context/recap_review_synthesis.ep, ../agent/control → app/agent/control.ep

- Line 8: `define compaction_job with database as Int and session as Str and run as Str and parent as Str and captured as List returning List:`
- Line 53: `define compaction_saved_review with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str returning List:`
- Line 91: `define compaction_saved_candidate with database as Int and session as Str and endpoint as Str and job as Str returning List:`
- Line 140: `define compaction_saved_synthesis with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str returning List:`

## app/context/recap_accept.ep

[Source](../../app/context/recap_accept.ep) · 105 lines · 8187 bytes

SHA-256: `508c2bbe11892bc96ca5470f72baf97f59c4a1e3b46ec0e6326c914c80c32574`

> Consume archived candidate and review receipts, not a caller-supplied verdict.
> A passing model review remains fallible evidence; this gate verifies its
> binding and checks the exact resumed request without changing model limits.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_review_synthesis → app/context/recap_review_synthesis.ep, session_messages → app/context/session_messages.ep, attachment_gate → app/context/attachment_gate.ep

- Line 9: `define recap_accept_provider with database as Int and session as Str and run as Str and candidate_event as Str and review_event as Str and identity as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`
- Line 103: `define recap_accept with database as Int and session as Str and run as Str and candidate_event as Str and review_event as Str and identity as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`

## app/context/recap_collect.ep

[Source](../../app/context/recap_collect.ep) · 61 lines · 4118 bytes

SHA-256: `956836bc73f96f3549d59eec30cce5dcaaff05069540c4b8fd85601635138f72`

> One caller owns a collection job at a time. Conflicting concurrent notes are
> rejected by coverage accounting, never silently merged or called complete.
> Collection has no iteration cap; every successful step must advance its cursor.
> Compatibility entry for existing host llama.cpp launch settings.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_jobs → app/context/recap_jobs.ep, ../agent/control → app/agent/control.ep

- Line 8: `define recap_collect_inner_provider with database as Int and session as Str and job as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 41: `define recap_collect_provider with database as Int and session as Str and job as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`
- Line 54: `define recap_collect_inner with database as Int and session as Str and job as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 59: `define recap_collect with database as Int and session as Str and job as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`

## app/context/recap_contract.ep

[Source](../../app/context/recap_contract.ep) · 87 lines · 5548 bytes

SHA-256: `6ed32b20f17a008b46e6ded6abd09f2c681cc5d0e36631c9aa83e636c8699dd3`

> Structural/provenance validation only. Passing does not establish semantic
> completeness and must never by itself authorize checkpoint activation.

Imports: ../retained_text → app/retained_text.ep, checkpoint → app/context/checkpoint.ep, ../providers/archived_stream → app/providers/archived_stream.ep

- Line 7: `define recap_sources_valid with database as Int and session as Str and boundary as Int and tokens as List and field as Int returning Int:`
- Line 29: `define recap_categories returning List:`
- Line 43: `define recap_validate_structure with database as Int and session as Str and endpoint as Str and candidate as Str returning Int:`
- Line 86: `define recap_generation_instruction returning Str:`

## app/context/recap_fragment.ep

[Source](../../app/context/recap_fragment.ep) · 96 lines · 9093 bytes

SHA-256: `c6d5a0d79722c57efa28816d1fca030e19d9bc5cbc4e94a1ae503d248f471f59`

> Result: status, exact preflighted body, next Unicode offset. The caller keeps
> processing until the original character count is reached. No source is cut
> from storage, and model context/output capacities are never reduced here.
> A nonempty candidate selects original-fragment review rather than note
> extraction. The complete candidate stays in every counted request.
> Compatibility entry for existing host llama.cpp launch settings.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_generate → app/context/recap_generate.ep

- Line 4: `define recap_fragment_source with endpoint as Str and original as List and offset as Int and next_offset as Int returning Str:`
- Line 15: `define recap_fragment_body with model as Str and endpoint as Str and original as List and offset as Int and next_offset as Int returning Str:`
- Line 22: `define recap_fragment_review_body with model as Str and endpoint as Str and original as List and offset as Int and next_offset as Int and candidate as Str returning Str:`
- Line 34: `define recap_fit_fragment_provider with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 42: `define recap_fit_fragment_for_provider with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and candidate as Str returning List:`
- Line 89: `define recap_fit_fragment with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 94: `define recap_fit_fragment_for with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and candidate as Str returning List:`

## app/context/recap_fragment_review.ep

[Source](../../app/context/recap_fragment_review.ep) · 103 lines · 7467 bytes

SHA-256: `055d560f4f7b8ac2e25f858797cd9ffd8b39e54f8b262de87ef9b5772c7859c0`

> A fragment verdict is bound to candidate bytes and an exact original range.
> It cannot be substituted for whole-source or whole-candidate review.
> Return status, next offset, receipt identity. Invalid/truncated reports do not
> advance coverage. A repair verdict is saved evidence, never activation consent.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_fragment → app/context/recap_fragment.ep

- Line 6: `define recap_fragment_review_validate with candidate as Str and endpoint as Str and event as Str and digest as Str and offset as Int and next_offset as Int and report as Str returning Str:`
- Line 37: `define recap_fragment_review_generate_provider with database as Int and session as Str and endpoint as Str and job as Str and event as Str and offset as Int and candidate as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 101: `define recap_fragment_review_generate with database as Int and session as Str and endpoint as Str and job as Str and event as Str and offset as Int and candidate as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/context/recap_generate.ep

[Source](../../app/context/recap_generate.ep) · 75 lines · 6633 bytes

SHA-256: `777e0f34a95d302b94f033c2bac72a91bf9f5468cb4cc2d444af355541198eb6`

> Generates a candidate only. The active context cannot change here. Oversized
> source input returns the real preflight decision for token-aware segmentation.
> Internal entry point for a verified frozen source or covered-notes package.
> The response still requires source validation and separate semantic review.
> Compatibility entry for existing host llama.cpp launch settings.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_sources → app/context/recap_sources.ep, recap_contract → app/context/recap_contract.ep, ../providers/prepare → app/providers/prepare.ep, ../providers/local_chat → app/providers/local_chat.ep

- Line 7: `define recap_request_body with model as Str and sources as Str returning Str:`
- Line 12: `define recap_generation_result with status as Str and candidate as Str returning List:`
- Line 20: `define recap_generate_provider with database as Int and session as Str and endpoint as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 31: `define recap_generate_sources_provider with database as Int and session as Str and endpoint as Str and sources as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 68: `define recap_generate with database as Int and session as Str and endpoint as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 73: `define recap_generate_sources with database as Int and session as Str and endpoint as Str and sources as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/context/recap_jobs.ep

[Source](../../app/context/recap_jobs.ep) · 78 lines · 4820 bytes

SHA-256: `0685ae99842b65e7b449427bf610900cdaaf2ef93ebe8ab930d55dce26ace72a`

> Immutable manifest: later messages or run transitions do not redefine a job.
> Return the contiguous covered Unicode prefix, or -1 for inconsistent notes.
> Gaps/overlaps fail explicitly rather than treating the largest offset as proof.
> Result: status, next offset, original event. A completed manifest means only
> that all source ranges have notes, not that those notes preserve all meaning.

Imports: ../retained_text → app/retained_text.ep, recap_notes → app/context/recap_notes.ep

- Line 5: `define recap_job_create with database as Int and session as Str and endpoint as Str and job as Str returning Int:`
- Line 13: `define recap_note_cursor with database as Int and session as Str and endpoint as Str and job as Str and source as Str and digest as Str and total as Int returning Int:`
- Line 47: `define recap_job_progress with database as Int and session as Str and job as Str returning List:`

## app/context/recap_merge.ep

[Source](../../app/context/recap_merge.ep) · 216 lines · 15907 bytes

SHA-256: `c483a264fb29d1c8c33631771017c1ae4208fbf532e75e4389433322d291cb90`

> Coverage is required before merge. Only notes for manifest members enter the
> package; stray notes bearing the same job name cannot inject extra sources.
> Keep coverage and note reads in one snapshot; release it before inference.
> Compatibility entry for existing host llama.cpp launch settings.
> Process only notes belonging to the covered immutable source manifest.
> An intermediate draft has an independent request identity, so interrupted
> partial merges cannot be mistaken for final candidates by job recovery.

Imports: ../retained_text → app/retained_text.ep, recap_jobs → app/context/recap_jobs.ep, recap_merge_resume → app/context/recap_merge_resume.ep

- Line 7: `define recap_merge_sources with database as Int and session as Str and job as Str returning Str:`
- Line 69: `define recap_merge_provider with database as Int and session as Str and job as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 99: `define recap_merge with database as Int and session as Str and job as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 106: `define recap_merge_progressive_provider with database as Int and session as Str and job as Str and endpoint as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/context/recap_merge_resume.ep

[Source](../../app/context/recap_merge_resume.ep) · 114 lines · 8426 bytes

SHA-256: `dc9b4b15dd13a74b5437a5493ea228814ff8f88c3e584d7320bf631fbb9de503`

> Bind the ordered notes, including exact immutable contents, to this merge.
> Reconstruct every committed step, checking contiguous Unicode ranges and the
> actual archived model input/output. A cursor alone never proves coverage.
> Result: status, next note index, character offset, cumulative candidate.

Imports: ../retained_text → app/retained_text.ep, recap_jobs → app/context/recap_jobs.ep

- Line 4: `define recap_merge_resume_result with status as Str returning List:`
- Line 13: `define recap_merge_notes_digest with database as Int and notes as List returning Str:`
- Line 29: `define recap_merge_resume with database as Int and session as Str and job as Str and endpoint as Str and notes as List and digest as Str returning List:`
- Line 110: `define recap_merge_progress_save with database as Int and session as Str and job as Str and digest as Str and request as Str returning Int:`

## app/context/recap_notes.ep

[Source](../../app/context/recap_notes.ep) · 64 lines · 4958 bytes

SHA-256: `dbab3966cf82e0a549937a7f29116c10ea629f63ec822b05b1c0e98418d021f7`

> Result: status, next offset, note event. Notes are immutable intermediate
> evidence, not active memory. Every note is tied to a completed provider stream
> and exact original-event character range; coverage/semantic review follow.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_fragment → app/context/recap_fragment.ep

- Line 7: `define recap_note_result with status as Str and next_offset as Int and event as Str returning List:`
- Line 14: `define recap_note_generate_provider with database as Int and session as Str and endpoint as Str and job as Str and source as Str and offset as Int and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 62: `define recap_note_generate with database as Int and session as Str and endpoint as Str and job as Str and source as Str and offset as Int and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/context/recap_rebuild.ep

[Source](../../app/context/recap_rebuild.ep) · 69 lines · 6564 bytes

SHA-256: `7790cd84b47ac4ceb355398eeaa71f044c3fadae3623caa8f7552189289ba722`

> Requesting a rebuild is a durable tool observation, not an immediate edit.
> The runner consumes observed requests only after all tool results are saved.
> Return none, required, recorded or an explicit recoverable failure. A crash
> after checkpoint activation but before this receipt reuses that checkpoint;
> it does not regenerate another recap or mutate original source records.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, ../tools/history_tools → app/tools/history_tools.ep, ../agent/run_state → app/agent/run_state.ep

- Line 8: `define recap_rebuild_enqueue with database as Int and run as Str and call_id as Str and arguments as Str returning Str:`
- Line 36: `define recap_rebuild_pending with database as Int and run as Str returning Str:`
- Line 68: `define recap_rebuild_schema returning Str:`

## app/context/recap_repair.ep

[Source](../../app/context/recap_repair.ep) · 22 lines · 2377 bytes

SHA-256: `6c7da3211763fd73936cccb05ee6eda6e19aa964a4dedb925c7cbc7c9b24259f`

> Repairs receive the originals as well as the rejected candidate and feedback.
> Both candidate and feedback are untrusted evidence, never new instructions.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: recap_generate → app/context/recap_generate.ep

- Line 5: `define recap_repair_provider with database as Int and session as Str and endpoint as Str and candidate as Str and feedback as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 20: `define recap_repair with database as Int and session as Str and endpoint as Str and candidate as Str and feedback as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/context/recap_review.ep

[Source](../../app/context/recap_review.ep) · 108 lines · 9079 bytes

SHA-256: `312745dfa73725f7610e8457ab30d375dccf8f56cdb056d8e4c43a351e87ee37`

> Validates the shape, candidate binding and exhaustive source accounting of a
> model review. A well-formed passing judgment is still fallible model evidence.
> This function does not change active context or certify semantic truth.
> Review always rereads original accepted sources. Intermediate notes cannot
> stand in for the evidence being checked. No review result activates context.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_generate → app/context/recap_generate.ep

- Line 4: `define recap_review_body with model as Str and sources as Str and candidate as Str returning Str:`
- Line 16: `define recap_review_validate with sources as Str and candidate as Str and report as Str returning Str:`
- Line 65: `define recap_review_generate_provider with database as Int and session as Str and endpoint as Str and candidate as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 106: `define recap_review_generate with database as Int and session as Str and endpoint as Str and candidate as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/context/recap_review_coverage.ep

[Source](../../app/context/recap_review_coverage.ep) · 115 lines · 8143 bytes

SHA-256: `794f1c3057d6683e279e0d01f467083099da78c6daaa1d84f0bfd4fa12e4c2ee`

> Return status, contiguous Unicode cursor, source. A covered prefix is evidence
> accounting only; repair findings remain separate from missing coverage.
> Walk only the immutable accepted-source manifest. A different candidate starts
> at zero; its predecessor's review ranges cannot count toward its coverage.
> Caller owns the session compaction lock. Each iteration advances a durable
> cursor; completion does not authorize activation or certify global claims.
> Compatibility entry for existing host llama.cpp launch settings.

Imports: ../retained_text → app/retained_text.ep, recap_fragment_review → app/context/recap_fragment_review.ep, recap_jobs → app/context/recap_jobs.ep, ../agent/control → app/agent/control.ep

- Line 8: `define recap_review_cursor with database as Int and session as Str and endpoint as Str and job as Str and source as Str and digest as Str and total as Int and candidate as Str returning List:`
- Line 49: `define recap_review_progress with database as Int and session as Str and job as Str and candidate as Str returning List:`
- Line 87: `define recap_review_collect_provider with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`
- Line 113: `define recap_review_collect with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:`

## app/context/recap_review_synthesis.ep

[Source](../../app/context/recap_review_synthesis.ep) · 144 lines · 11506 bytes

SHA-256: `07a8c57cd7dede97c02595ecd2f9eee65d26bf37b4cbcc9ba11e72a95b713d0f`

> Package every reviewed manifest source and range for cross-fragment analysis.
> Requires a read transaction or the session's compaction ownership.
> Synthesis remains a separate evidence kind. Activation independently checks
> its complete original-source manifest and every fragment review receipt.
> Compatibility entry for existing host llama.cpp launch settings.
> Revalidate the source manifest and every range, not the stored validation label.
> Call outside a read transaction because recap_sources verifies its own snapshot.

Imports: ../retained_text → app/retained_text.ep, recap_review_coverage → app/context/recap_review_coverage.ep, recap_review → app/context/recap_review.ep

- Line 7: `define recap_review_package with database as Int and session as Str and job as Str and candidate as Str returning Str:`
- Line 66: `define recap_review_synthesis_body with model as Str and package as Str returning Str:`
- Line 74: `define recap_review_synthesis_provider with database as Int and session as Str and job as Str and candidate as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 116: `define recap_review_synthesis with database as Int and session as Str and job as Str and candidate as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:`
- Line 122: `define recap_synthesis_validate with database as Int and session as Str and endpoint as Str and candidate as Str and payload as Str returning Str:`

## app/context/recap_sources.ep

[Source](../../app/context/recap_sources.ep) · 121 lines · 7429 bytes

SHA-256: `8df7afc5b40e9283726966977e768edfebdfa88bf2a5170f4104bc745e98314d`

> Read one consistent view of accepted run ancestry through a frozen endpoint.
> Transport copies and nested model requests stay in the original archive but
> are not repeated as conversational evidence. Tool originals retain identities.

Imports: ../retained_text → app/retained_text.ep, ../agent/run_messages → app/agent/run_messages.ep

- Line 4: `define recap_source_record with row as List returning Str:`
- Line 14: `define recap_sources_snapshot with database as Int and session as Str and endpoint as Str returning Str:`
- Line 111: `define recap_sources with database as Int and session as Str and endpoint as Str returning Str:`

## app/context/session_messages.ep

[Source](../../app/context/session_messages.ep) · 173 lines · 11447 bytes

SHA-256: `e7785ce816c03e7adfdae616d17f7bff75e24c8b8d13ba4141b14c6810e8e380`

> Admin-only schemas are selected from frozen initiating authority. Dispatch
> repeats the check; advertising a schema is never itself permission.
> Caller holds a read transaction while collecting context, so checkpoint,
> accepted run heads and tool observations come from the same database snapshot.
> Checkpoint activation requires generation and coverage validation upstream;
> this consumer checks source identity, not semantic completeness. The automatic
> recap generator/validator is not implemented by this module.
> Preview and active reconstruction share the same path. The caller holds a
> database snapshot; passing a staged checkpoint does not activate it.
> Keep the current objective verbatim even when its event is covered by
> a recap. It remains the user's instruction, not a summary's inference.
> A rebuild finishes after its replacement checkpoint is activated. Keep
> that trusted runtime receipt in the resumed prompt so the model does not
> mistake the earlier queued observation for still-pending work.

Imports: ../retained_text → app/retained_text.ep, system_awareness → app/context/system_awareness.ep, ../archive/session_scope → app/archive/session_scope.ep, ../tools/attachment_tools → app/tools/attachment_tools.ep, ../tools/host_text → app/tools/host_text.ep, ../tools/memory_tools → app/tools/memory_tools.ep, ../tools/knowledge_tools → app/tools/knowledge_tools.ep, ../tools/protected_tools → app/tools/protected_tools.ep, ../agent/run_messages → app/agent/run_messages.ep

- Line 11: `define session_request_body with model as Str and messages as Str returning Str:`
- Line 18: `define session_request_body_owned with database as Int and run as Str and model as Str and messages as Str returning Str:`
- Line 46: `define session_messages_snapshot with database as Int and session as Str and current_run as Str returning Str:`
- Line 51: `define session_messages_checkpoint with database as Int and session as Str and current_run as Str and checkpoint as List returning Str:`
- Line 163: `define session_messages with database as Int and session as Str and current_run as Str returning Str:`

## app/context/system_awareness.ep

[Source](../../app/context/system_awareness.ep) · 62 lines · 4564 bytes

SHA-256: `770dedc65049f0bb18543ec05dad82c10b1036b9d931af4ab2e86576556ee510`

> Only operator-controlled files enter the system role. Conversation content,
> attachments and tool observations never select identity or architecture files.
> Run identity is available through system_inspect, not the shared prompt prefix.
> Fresh inspection is a durable tool observation, never volatile text inserted
> during deterministic checkpoint preview/reconstruction. No credentials or
> unrelated sessions are returned. The run comes from the trusted dispatcher.

Imports: ../retained_text → app/retained_text.ep, ../agent/provider_profile → app/agent/provider_profile.ep, ../providers/status → app/providers/status.ep, ../security/run_authority → app/security/run_authority.ep, fs → standard library or compiler dependency

- Line 9: `define echo_system_prompt with database as Int and run as Str returning Str:`
- Line 39: `define echo_system_inspect with database as Int and run as Str and arguments as Str returning Str:`
- Line 61: `define echo_system_schema returning Str:`

## app/device_admin.ep

[Source](../../app/device_admin.ep) · 48 lines · 2312 bytes

SHA-256: `081369ce2f6ae8ba16ed7e822f0252138e7f4d4d7940b484bbb045ab6cb4890d`

> Host-local administration only. Never expose these operations as public routes.

Imports: web/run_store → app/web/run_store.ep

- Line 4: `define main returning Int:`

## app/echo_agent.ep

[Source](../../app/echo_agent.ep) · 62 lines · 3331 bytes

SHA-256: `a8ad7128582631c9122d17f9d2c4ba8e92bfb02113114b04d2fac70786d6fc6a`

> Command-line entry point while the browser lifecycle is connected.
> start database session model port output-reserve read-batch objective
> resume database run model port output-reserve read-batch
> start-profile database session profile-file output-reserve read-batch objective
> resume-profile database run profile-file output-reserve read-batch

Imports: agent/runner → app/agent/runner.ep, fs → standard library or compiler dependency

- Line 9: `define main returning Int:`

## app/echo_providers.ep

[Source](../../app/echo_providers.ep) · 29 lines · 1171 bytes

SHA-256: `7bc97cfb16ac3eee45dadf5ed148c52b7b83d62f3c9eae4f5d71334df388fd7c`


Imports: providers/registry → app/providers/registry.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## app/hash_file.ep

[Source](../../app/hash_file.ep) · 13 lines · 467 bytes

SHA-256: `40a3fcf8f643e9c8b33a07806bb25624eab9045ea1ee666ba443b3006f608e32`


Imports: archive/file_digest → app/archive/file_digest.ep

- Line 3: `define main returning Int:`

## app/https_config.ep

[Source](../../app/https_config.ep) · 37 lines · 2305 bytes

SHA-256: `22773f01ef0a1ca617872c5e39b96e18724d676692539e9f238d730ddf088218`

> Generate upstream configuration from Ernos. Default is loopback; explicit
> public host and local bind address enable the configured LAN interface.

Imports: web/http_api → app/web/http_api.ep, fs → standard library or compiler dependency

- Line 6: `define main returning Int:`

## app/kokoro_speak.ep

[Source](../../app/kokoro_speak.ep) · 57 lines · 3294 bytes

SHA-256: `56899571cfc3f6b5b547aedef232314d411844a9126f32047a91ba4f7ad10600`

> Isolated local speech helper. Only upstream library calls cross the bridge.
> The private job directory contains input.txt and receives spoken.wav.

Imports: providers/native_python → app/providers/native_python.ep, providers/json_access → app/providers/json_access.ep, providers/archived_stream → app/providers/archived_stream.ep, fs → standard library or compiler dependency

- Line 8: `define main returning Int:`

## app/lifecycle/migrations.ep

[Source](../../app/lifecycle/migrations.ep) · 67 lines · 4895 bytes

SHA-256: `f91c3d349d006d0e295fb61639c31af7ce7f18726992300b4a51e1c1c6684052`

> Host-authored SQL from a pinned executable only, never browser/model input.
> DDL and receipt commit together. Failed candidates never change live history.
> Call before workers write. An older binary must refuse a newer schema.
> Restore a verified backup for incompatible rollback; do not destroy new data.

Imports: ../archive/history → app/archive/history.ep, ../context/checkpoint → app/context/checkpoint.ep

- Line 6: `define migration_fail with database as Int and reason as Str returning Str:`
- Line 11: `define migration_apply with database as Int and version as Int and name as Str and statements as Str returning Str:`
- Line 54: `define migration_compatible with database as Int and supported as Int returning Int:`

## app/lifecycle/operations.ep

[Source](../../app/lifecycle/operations.ep) · 100 lines · 7833 bytes

SHA-256: `cd35e53134283d1800010b0bd5e3bcb2781ba1a4050987dfbd5cdd21e4ae8573`

> Scope and principal are resolved by the trusted caller, never claimed by a
> tool. Store diagnostic codes and artifact references, not credentials/prompts.
> Every state is an immutable receipt. There is no mutable status projection
> that can commit without its matching audit record.
> status, principal, kind, request_digest, revision, state, stage, cause,
> effects, diagnostic reference, recovery action, result reference.
> A different scope must not learn the owner or reuse its operation ID.
> Compare revision while holding the writer lock. A lost response never grants
> a second execution claim. Reopen reads running as running until an owner-death
> check records uncertainty. Uncertainty cannot transition back into dispatch.

Imports: migrations → app/lifecycle/migrations.ep

- Line 7: `define operation_schema with database as Int returning Str:`
- Line 13: `define operation_read with database as Int and scope as Str and identity as Str returning List:`
- Line 19: `define operation_reserve with database as Int and scope as Str and identity as Str and principal as Str and kind as Str and request as Str returning Str:`
- Line 54: `define operation_transition with database as Int and scope as Str and identity as Str and revision as Int and state as Str and stage as Str and cause as Str and effects as Str and diagnostic as Str and recovery as Str and result as Str returning Str:`

## app/lifecycle/service_profile.ep

[Source](../../app/lifecycle/service_profile.ep) · 109 lines · 5923 bytes

SHA-256: `19900767c6942c37b10507cb089ac4a42fb1d1d67b9856ec1d5e625cac605b37`

> Host configuration only. This is not a browser or model execution endpoint.
> Exec replaces the launcher, so the caller owns the actual service PID and
> receives its exit status and signals without an unsupervised wrapper child.
> Check filesystem requirements without starting a service or changing cwd.
> No shell expansion, inherited environment or token caps.

Imports: ../providers/json_access → app/providers/json_access.ep, ../workspace/directories → app/workspace/directories.ep

- Line 7: `define service_native_text with text as Str returning Int:`
- Line 15: `define service_profile_valid with tokens as List returning Int:`
- Line 65: `define service_profile_vector with tokens as List and name as Str returning List:`
- Line 78: `define service_profile_run with profile as Str and check_only as Int returning Int:`

## app/lifecycle/startup.ep

[Source](../../app/lifecycle/startup.ep) · 34 lines · 1587 bytes

SHA-256: `b6d9fc10b2375874f6f97accab84b190519ec8b8c2882fe9fd7b8917d1170371`

> Inspect before archive_open can create or update any application schema.
> Deployment must stop old processes before migrations; this is not a live
> migration lock or permission to run mixed application versions.
> Distinguish a transient database lock from a proven schema incompatibility.
> -1 never permits writes; it tells the process supervisor this is retryable.
> Match normal archive access: a short writer lock is not a schema change.
> Existing boolean callers remain fail-closed for both busy and incompatible.

Imports: migrations → app/lifecycle/migrations.ep, fs → standard library or compiler dependency

- Line 9: `define startup_archive_status with path as Str and supported as Int returning Int:`
- Line 33: `define startup_archive_compatible with path as Str and supported as Int returning Int:`

## app/media/image_logs.ep

[Source](../../app/media/image_logs.ep) · 44 lines · 2057 bytes

SHA-256: `488873de100cca86d92fc43e9878c04261583439aec8c32769ff39f7a25fbdaa`

> Called only after the helper is reaped. The original host-owned descriptor
> stays open; no pathname is reopened. Pages are transfer sizes, not quotas.
> Immutable process_output events travel with database snapshots and can be
> read through the existing session-scoped output cursor and history tools.

Imports: ../workspace/output_archive → app/workspace/output_archive.ep, ../workspace/files → app/workspace/files.ep

- Line 4: `external define ep_uuid_v4`
- Line 10: `define image_log_archive with database as Int and run as Str and call_id as Str and library as Int and descriptor as Int and stream as Str returning Int:`

## app/media/image_request.ep

[Source](../../app/media/image_request.ep) · 110 lines · 5692 bytes

SHA-256: `3050f510163f61ad5519f6e9a84980a4856c512f6c14e2cd7f541001fbc88c7e`

> Host profile owns the executable, weights and native argument template.
> Model text can fill named values only; it cannot select paths or add flags.
> Require each setting exactly once; no silently ignored controls.

Imports: ../retained_text → app/retained_text.ep, ../providers/json_tokens → app/providers/json_tokens.ep, ../providers/json_string → app/providers/json_string.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 8: `define image_request_arguments with profile as Str and arguments as Str returning List:`

## app/media/image_tool.ep

[Source](../../app/media/image_tool.ep) · 166 lines · 10823 bytes

SHA-256: `247de1d73a86fad464781f6fc355b22eeb5d5498860f5e82bb8e9e3b7ed71faf`

> Run/call identity comes from the dispatcher. Re-read the immutable request.
> Each claimed call gets its own private directory outside writable workspaces.
> Interrupted calls remain uncertain under the existing tool ledger; no replay.
> Leave an uncertain claim when logging failed; never report a fully saved
> result or authorize automatic replay after partial archive persistence.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, image_request → app/media/image_request.ep, ../security/tool_policy → app/security/tool_policy.ep, image_logs → app/media/image_logs.ep, ../web/attachments → app/web/attachments.ep, ../workspace/directories → app/workspace/directories.ep, ../providers/helper_process → app/providers/helper_process.ep

- Line 11: `define image_tool_authority with database as Int and run as Str and call_id as Str returning List:`
- Line 26: `define image_tool_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:`
- Line 52: `define image_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:`

## app/media/speech_request.ep

[Source](../../app/media/speech_request.ep) · 118 lines · 7720 bytes

SHA-256: `430a8d8544131ba2dad518319f231970fadb36c9b6cfd0db177bca8075516947`

> This adapter accepts only an owned archive identity and spoken language.
> Runtime paths, model weights and output filenames are host controlled.
> No duration, output-token or segment-length override. Whisper processes
> the entire recording using its native windows. JSON retains timestamps.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, ../tools/history_tools → app/tools/history_tools.ep, ../archive/session_scope → app/archive/session_scope.ep

- Line 8: `define speech_request_source with database as Int and session as Str and arguments as Str returning List:`
- Line 66: `define speech_request_arguments with profile as Str and language as Str returning List:`
- Line 117: `define speech_tool_schema returning Str:`

## app/media/speech_text.ep

[Source](../../app/media/speech_text.ep) · 93 lines · 5139 bytes

SHA-256: `96b3d5a6c5ba1435609b6ffdfe085f3fc4062032d8c7f063b402ccdbc8daeaf6`

> Speech is a derived presentation. The original archived message stays intact.
> Markdown structure is silent; link labels and inline code remain readable.

Imports: ../providers/json_string → app/providers/json_string.ep

- Line 5: `define speech_plain_text with text as Str returning Str:`

## app/media/speech_tool.ep

[Source](../../app/media/speech_tool.ep) · 243 lines · 16583 bytes

SHA-256: `9f8556a350b10fa660d19e961859bddbffa7d99795ce86f5946f7d50a01495c8`

> Run/call identity comes from the dispatcher. Re-read the immutable request.
> Direct Speak actions have no model run. Their immutable request binds the
> exact saved assistant event and is still claimed through the tool ledger.
> Each claimed call gets its own private directory outside writable workspaces.
> Interrupted calls remain uncertain under the existing tool ledger; no replay.
> Private job input is exclusive and flushed before dispatch. It is not
> published into a user workspace, so no rename operation is required.
> Leave an uncertain claim when logging failed; never report a fully saved
> result or authorize automatic replay after partial archive persistence.

Imports: ../security/principals → app/security/principals.ep, speech_text → app/media/speech_text.ep, ../security/tool_authority → app/security/tool_authority.ep, ../retained_text → app/retained_text.ep, speech_request → app/media/speech_request.ep, voice_request → app/media/voice_request.ep, ../archive/file_copy → app/archive/file_copy.ep, image_logs → app/media/image_logs.ep, ../web/attachments → app/web/attachments.ep, ../workspace/directories → app/workspace/directories.ep, ../providers/helper_process → app/providers/helper_process.ep

- Line 14: `define speech_tool_authority with database as Int and run as Str and call_id as Str returning List:`
- Line 36: `define speech_tool_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:`
- Line 62: `define speech_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:`

## app/media/voice_request.ep

[Source](../../app/media/voice_request.ep) · 101 lines · 5682 bytes

SHA-256: `0a5df378693c2c01949b7d4ed7408b2a7e6ae9bc552b90c45f446ae47c00e5c2`

> Text is written to a private input file, never interpolated into a command.

Imports: ../retained_text → app/retained_text.ep, ../providers/json_tokens → app/providers/json_tokens.ep, ../providers/json_string → app/providers/json_string.ep

- Line 6: `define voice_request_text with arguments as Str returning Str:`
- Line 30: `define voice_request_arguments with profile as Str returning List:`
- Line 100: `define voice_tool_schema returning Str:`

## app/memory/archive_recall.ep

[Source](../../app/memory/archive_recall.ep) · 66 lines · 4482 bytes

SHA-256: `b46488e5d04afea05d1ba34bacd077ea227192edad28d3ce57ca0bceec00c927`

> Resolve only the initiating run's own private principal. Neither a requested
> principal nor an administrator role selects another person's memory.
> A fixed boundary makes a scan finite without dropping older conversations.
> Result: status, event, source session, sequence, kind, timestamp, digest.
> This is chronological source recall, not inferred semantic knowledge.
> Guessing an event identifier is not permission. Authorise the original source
> session before using the existing Unicode passage reader and original digest.

Imports: session_ownership → app/memory/session_ownership.ep, ../archive/history → app/archive/history.ep

- Line 6: `define memory_archive_principal with database as Int and run as Str returning Str:`
- Line 20: `define memory_archive_boundary with database as Int and run as Str returning List:`
- Line 33: `define memory_archive_next with database as Int and run as Str and literal as Str and after_sequence as Int and through_sequence as Int returning List:`
- Line 51: `define memory_archive_read with database as Int and run as Str and event as Str and offset as Int and characters as Int returning List:`

## app/memory/knowledge.ep

[Source](../../app/memory/knowledge.ep) · 139 lines · 12607 bytes

SHA-256: `97bde516bb73b6724b05cb296eaa376b20f525722fb9b636cf1f685471d01cb1`

> Facts and episodes are derived assertions. The immutable source remains the
> evidence; repeating a model inference never converts it into an observation.
> This primitive accepts already parsed fields from a trusted caller. Source
> ownership, evidence class and correction lineage are still enforced here.
> Empty event_time means unknown, not the time the system ingested the record.
> A model-authored claim is always an inference unless it points to an
> independently recorded user account or tool observation.
> Exact graph traversal by subject or object; empty node scans all records.
> All versions remain available, and supersession is reported rather than
> silently deleting contradictory evidence. Callers choose their cursor.
> This SQL fragment is shared by derived recall and future indexes. A missing
> lifecycle schema means the earlier graph version; a read failure denies recall.

Imports: archive_recall → app/memory/archive_recall.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 6: `define knowledge_schema with database as Int returning Str:`
- Line 12: `define knowledge_abort with database as Int and status as Str returning Str:`
- Line 20: `define knowledge_record with database as Int and run as Str and kind as Str and subject as Str and predicate as Str and object as Str and evidence as Str and source_event as Str and source_digest as Str and event_time as Str and valid_until as Str and supersedes as Str returning Str:`
- Line 116: `define knowledge_next with database as Int and run as Str and node as Str and after_sequence as Int returning List:`
- Line 133: `define knowledge_visible_sql with database as Int returning Str:`

## app/memory/knowledge_lifecycle.ep

[Source](../../app/memory/knowledge_lifecycle.ep) · 64 lines · 5528 bytes

SHA-256: `e67a94a2e37dd070eb9deea9bbd6c54ca478dd83193408447312a40e1c8f991d`

> Forgetting removes a derived assertion from ordinary recall. Original source
> events and the audit remain intact; restoration is a new explicit transition.
> Management lookup is explicit by identity. It returns state/revision only,
> so a forgotten assertion does not reappear as recalled content.
> CAS prevents an old forget/restore request from undoing a newer decision.
> Exact retries return the prior receipt without reapplying its state.

Imports: knowledge → app/memory/knowledge.ep

- Line 5: `define knowledge_lifecycle_schema with database as Int returning Str:`
- Line 13: `define knowledge_visibility_read with database as Int and run as Str and claim as Str returning List:`
- Line 27: `define knowledge_visibility_set with database as Int and run as Str and change as Str and claim as Str and previous_revision as Int and state as Str and reason as Str returning Str:`

## app/memory/session_ownership.ep

[Source](../../app/memory/session_ownership.ep) · 24 lines · 2637 bytes

SHA-256: `7c390e9d8fc950f37898c1407769698a8def3ef5b224d12efb3f926ad413e7e9`

> Conversation ownership is durable, unlike a credential's current account
> link. Migration preserves each legacy device's original principal; it never
> retroactively transfers private history because that device was relinked.
> This is the principal scope check for future cross-session retrieval. It is
> not a replacement for current device routes until every surface is migrated.
> Caller supplies an authenticated device; credential and principal revocation
> are checked again here. Admin status does not bypass private ownership.

Imports: ../security/tool_authority → app/security/tool_authority.ep

- Line 6: `define session_principal_schema with database as Int returning Str:`
- Line 16: `define session_principal_allows with database as Int and device as Str and session as Str returning Int:`

## app/mlx_count.ep

[Source](../../app/mlx_count.ep) · 78 lines · 4694 bytes

SHA-256: `39fad251d1d5233cc960853008ca2397686a31dd77f3668f5ced689687bd2cf3`

> One-shot native helper; all interpreter-owned objects live until process exit.
> Invoke the pinned upstream server method, including its tools, reasoning tail
> and content normalization. No hand-written substitute chat template.

Imports: providers/native_python → app/providers/native_python.ep, providers/json_access → app/providers/json_access.ep, providers/archived_stream → app/providers/archived_stream.ep, fs → standard library or compiler dependency

- Line 9: `define main returning Int:`

## app/principal_admin.ep

[Source](../../app/principal_admin.ep) · 98 lines · 5544 bytes

SHA-256: `bde981502057599369af45462ce789deb07e3b63e01685aca7e5eec0fc610007`

> OS-local operator command. No browser or model schema exposes this program.
> Guest sandbox workers cannot open the host database. Never infer authority
> from an HTTP origin, device name, caller-supplied role or network address.
> Check compatibility before archive_open can perform schema writes.

Imports: web/run_store → app/web/run_store.ep, memory/knowledge_lifecycle → app/memory/knowledge_lifecycle.ep

- Line 7: `define main returning Int:`

## app/providers/archived_stream.ep

[Source](../../app/providers/archived_stream.ep) · 110 lines · 6123 bytes

SHA-256: `cf93c4f7b741c8202195aaad3b2998b955d575f39802de43f5453270424f95a1`

> Two bounded scans over the actual string length. Size the output exactly,
> then fill a native list buffer; no strlen or substring scans inside the loop.
> The request event is the stable identity of one generation attempt. Reusing it
> cannot silently overwrite or replay a partially archived response.
> Reader is already positioned at the response body, after the caller validates
> HTTP status and text/event-stream content type. Caller owns socket cleanup.
> Every complete event is committed before assembly. Terminal evidence is also
> committed before success, including transport errors and parser rejection.

Imports: ../retained_text → app/retained_text.ep, chat_stream → app/providers/chat_stream.ep, ../transport/event_stream → app/transport/event_stream.ep, ../archive/event_store → app/archive/event_store.ep

- Line 6: `external define peek_byte with pointer as Int and offset as Int returning Int`
- Line 7: `external define poke_byte with pointer as Int and offset as Int and value as Int returning Int`
- Line 11: `define provider_quote with value as Str returning Str:`
- Line 59: `define provider_archive_piece with database as Int and session as Str and request as Str and sequence as Int and kind as Str and payload as Str returning Int:`
- Line 64: `define provider_collected with status as Str and state as List returning List:`
- Line 74: `define provider_collect_events with reader as List and body as List and database as Int and session as Str and request as Str and read_size as Int returning List:`

## app/providers/attachment_image.ep

[Source](../../app/providers/attachment_image.ep) · 38 lines · 2816 bytes

SHA-256: `9fa55eabe14b634a988b658f25cb89931bd5327a19d99075f915f365bf8b020a`

> The caller supplies a trusted run session, never an arbitrary host path.
> Returns status, typed content part, original source event and original digest.
> Decoding and token cost belong to the loaded provider, not this serializer.
> MIME spelling is metadata only. The provider validates actual image bytes.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, ../archive/file_base64 → app/archive/file_base64.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 9: `define attachment_image_part with database as Int and session as Str and attachment as Str returning List:`

## app/providers/chat_stream.ep

[Source](../../app/providers/chat_stream.ep) · 180 lines · 8686 bytes

SHA-256: `6c11fa4672222a1741180135421e430558a4250794ee302966a85f3b7d3775d2`

> State: choices, done marker, error. Each choice has index, text builder,
> reasoning builder, tool list, finish reason. Each tool has index, id, name,
> argument builder. Provider indexes are exact strings, not sparse allocations.
> Finalize once, only after clean HTTP EOF. Result choice builders become strings.
> Limited/unknown finishes and invalid arguments remain non-executable failures.

Imports: ../retained_text → app/retained_text.ep, json_access → app/providers/json_access.ep

- Line 7: `define chat_stream_create returning List:`
- Line 14: `define chat_find with entries as List and identity as Str returning List:`
- Line 23: `define chat_append_field with tokens as List and object as Int and name as Str and builder as Int returning Int:`
- Line 32: `define chat_tool_delta with choice as List and tokens as List and delta as Int returning Int:`
- Line 66: `define chat_choice_delta with state as List and tokens as List and object as Int returning Int:`
- Line 104: `define chat_stream_accept_inner with state as List and data as Str returning Int:`
- Line 128: `define chat_stream_accept with state as List and data as Str returning Int:`
- Line 138: `define chat_stream_finish with state as List and transport as Str returning Int:`

## app/providers/context_admission.ep

[Source](../../app/providers/context_admission.ep) · 31 lines · 1809 bytes

SHA-256: `5b5971031daa5219d964495853c85ab9853eef259f22eeba5157234a1295a09b`

> Adapters supply verified effective limits and the exact token count for the
> assembled request. A finite output ceiling here must be a genuine provider
> capability, not a configurable generation default. Unknown values fail.
> Result: status, loaded context, supported context, input tokens, output room.
> Compaction cannot increase a separate generation ceiling. Surface this
> distinction so callers do not repeatedly compact an impossible workload.

Imports: ../context/budget → app/context/budget.ep

- Line 7: `define provider_context_admission with loaded as Int and supported as Int and input as Int and ceiling as Int and reserve as Int returning List:`

## app/providers/helper_process.ep

[Source](../../app/providers/helper_process.ep) · 143 lines · 7696 bytes

SHA-256: `21f79253cde401b61cc971edc1a245e8632a6f3c3750311ef69232ad83648dc6`

> Host-owned provider helpers only. Explicit argv/environment and descriptors
> never pass through a shell. No wall-clock deadline or output truncation.
> Result: status, raw wait status, retained handle if cleanup needs attention.
> Caller owns descriptors and must archive/validate output before using it.
> Keep ownership here instead of returning a live handle through recap layers.
> ECHILD means wait ownership is gone: never signal that possibly reused PID.
> Other wait/signal failures stay supervised, with an archived diagnostic.

Imports: ../retained_text → app/retained_text.ep, ../workspace/native_process → app/workspace/native_process.ep, ../archive/event_store → app/archive/event_store.ep, archived_stream → app/providers/archived_stream.ep

- Line 8: `define provider_helper_cancelled with library as Int and reader as Int returning Int:`
- Line 32: `define provider_helper_result with status as Str and waited as Int and handle as List returning List:`
- Line 42: `define provider_helper_wait_owned with database as Int and session as Str and identity as Str and handle as List and cancellation as Int returning List:`
- Line 103: `define provider_helper_run with database as Int and session as Str and identity as Str and executable as Str and arguments as List and environment as List and directory as Int and output as Int and errors as Int and cancellation as Int returning List:`

## app/providers/json_access.ep

[Source](../../app/providers/json_access.ep) · 92 lines · 3616 bytes

SHA-256: `e32e6bdda3e82cdeb749a81e0cd6ed5594480f4b6a50b3c7af3153b5aa92dabb`

> Missing is -1; wrong type or ambiguous duplicate members is -2.
> Check every member, including fields an adapter does not currently use.

Imports: ../retained_text → app/retained_text.ep, json_tokens → app/providers/json_tokens.ep

- Line 4: `define provider_kind with tokens as List and index as Int returning Str:`
- Line 9: `define provider_value with tokens as List and index as Int returning Str:`
- Line 14: `define provider_after with tokens as List and index as Int returning Int:`
- Line 35: `define provider_member with tokens as List and object as Int and name as Str returning Int:`
- Line 60: `define provider_items with tokens as List and array as Int returning List:`
- Line 74: `define provider_index with tokens as List and index as Int returning Str:`
- Line 85: `define provider_unique_objects with tokens as List returning Int:`

## app/providers/json_string.ep

[Source](../../app/providers/json_string.ep) · 100 lines · 4757 bytes

SHA-256: `e6b8a7fd6a558afce96ab875b95e14bd83892aea3b19e3dac956b824be127dd2`

> Derived from pinned Ernos stdlib/json.ep. Bounds are retained; byte access
> avoids repeated strlen scans for large already-validated JSON strings.
> Append a Unicode code point to the byte list as UTF-8 (1..4 bytes). Uses integer
> division for the shifts/masks (no % operator in EP).
> \uXXXX — decode the 4 hex digits to a code point. Without
> this, e.g. "&" (&) and ">" (>) leak through as the
> literal text "u0026"/"u003e", corrupting any decoded command.
> Malformed escape — keep the 'u' literally, lose nothing.
> UTF-16 surrogate pair: high (D800..DBFF) + low (DC00..DFFF).

Imports: json → standard library or compiler dependency

- Line 4: `external define peek_byte with pointer as Int and offset as Int returning Int`
- Line 6: `define provider_hex4 with s and p and len:`
- Line 19: `define provider_append_utf8 with lst and cp:`
- Line 39: `define provider_decode_string with state:`

## app/providers/json_tokens.ep

[Source](../../app/providers/json_tokens.ep) · 185 lines · 9119 bytes

SHA-256: `bff0cfdcb5a4aa5cc7d47d43dbc273b64726044878418c8efc642717d1481eba`

> Tokens retain their original spelling. Numbers never pass through a lossy
> integer conversion. String decoding is used only after complete validation.
> Token fields: kind, original start, exclusive end, decoded string or spelling.
> An explicit stack validates grammar without recursion or a fixed depth cap.
> Empty list is invalid; a successful document has at least one token.
> UTF-8 validity is established by the transport decoder before this entry.

Imports: ../retained_text → app/retained_text.ep, json_string → app/providers/json_string.ep

- Line 7: `define provider_token with kind as Str and start as Int and finish as Int and value as Str returning List:`
- Line 15: `define provider_digit with text as Str and position as Int returning Int:`
- Line 23: `define provider_next_token with text as Str and start as Int returning List:`
- Line 128: `define provider_json_tokens with text as Str returning List:`

## app/providers/llama_metadata.ep

[Source](../../app/providers/llama_metadata.ep) · 59 lines · 3007 bytes

SHA-256: `32d9b1ba16e4d228eb4306c7976c5bb51b91924175f7a07cec934002f82f619a`

> Native llama.cpp metadata; unknown values fail instead of receiving defaults.
> Result: status, model id, loaded context, training context, output ceiling.

Imports: local_json → app/providers/local_json.ep, ../context/budget → app/context/budget.ep

- Line 6: `define llama_metadata_parse with models as Str and props as Str and model as Str returning List:`
- Line 53: `define llama_input_count with response as Str returning Int:`

## app/providers/llama_modalities.ep

[Source](../../app/providers/llama_modalities.ep) · 44 lines · 2453 bytes

SHA-256: `3c732f8e8880483752568fa9fbd26853ebb5b6ad0173aa483065eb3ee811f00d`

> Inspected against pinned llama.cpp b11429: /props.modalities describes the
> loaded projector's image/audio/video input support. No model-name guesses.

Imports: local_json → app/providers/local_json.ep

- Line 5: `define llama_modalities_check with body as Str and properties as Str returning Str:`

## app/providers/llama_preflight.ep

[Source](../../app/providers/llama_preflight.ep) · 82 lines · 5446 bytes

SHA-256: `76ed82cff837dafcd8b601fa3539555738dd230e7bb92cd54a0a6979fbd3ebe1`

> The reserve comes from the next workload. Capacity and input count come only
> from the loaded provider. No guessed context, output budget or safety margin.
> Result: status, loaded context, training context, input tokens, available output.
> n_predict is a configurable runtime default, not a model capability.
> Require its removal rather than treating it as a genuine output ceiling.
> Preserve all endpoint evidence and the exact request, including failed HTTP
> responses. A caller can inspect why a request was not admitted to generation.

Imports: ../retained_text → app/retained_text.ep, llama_metadata → app/providers/llama_metadata.ep, llama_modalities → app/providers/llama_modalities.ep, context_admission → app/providers/context_admission.ep

- Line 9: `define llama_preflight_evaluate with models as Str and props as Str and counted as Str and model as Str and output_reserve as Int returning List:`
- Line 31: `define llama_preflight_receipt with database as Int and session as Str and identity as Str and body as Str and output_reserve as Int and models as List and props as List and counted as List and decision as List returning Int:`
- Line 52: `define llama_preflight with database as Int and session as Str and identity as Str and port as Int and body as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int returning List:`

## app/providers/local_chat.ep

[Source](../../app/providers/local_chat.ep) · 118 lines · 7417 bytes

SHA-256: `b7526d450acef1083c5ea33832998818d9f952cde2c02bcfec252b4b8b6fc91f`

> Validate before connecting. The exact outgoing JSON must already be durable.
> Localhost is deliberate for this adapter, not a provider-wide restriction.
> Claim the attempt durably before any network action. A crash after this
> point requires recovery/new attempt identity, never automatic POST replay.

Imports: ../retained_text → app/retained_text.ep, archived_stream → app/providers/archived_stream.ep, ../transport/local_connect → app/transport/local_connect.ep

- Line 7: `define local_chat_request_valid with database as Int and session as Str and request as Str and body as Str and path as Str returning Int:`
- Line 39: `define local_chat_exchange with reader as List and database as Int and session as Str and request as Str and port as Int and path as Str and body as Str and read_size as Int returning List:`
- Line 91: `define local_chat_generate with database as Int and session as Str and request as Str and port as Int and path as Str and body as Str and cancellation_reader as Int and read_size as Int returning List:`

## app/providers/local_json.ep

[Source](../../app/providers/local_json.ep) · 122 lines · 6496 bytes

SHA-256: `2a0c88bfade9e9ceb717ac15ff2f896acf3c822ce4e012d6eba4c2add9edc0fc`

> Reject malformed UTF-8 and NUL rather than corrupting metadata or JSON keys.
> This request path is for local discovery/token-count endpoints. It never
> retries, modifies global properties, or invokes generation implicitly.

Imports: ../retained_text → app/retained_text.ep, local_chat → app/providers/local_chat.ep

- Line 5: `define provider_valid_utf8 with text as Str returning Int:`
- Line 40: `define local_json_result with status as Str and code as Int and text as Str returning List:`
- Line 49: `define local_json_exchange with reader as List and method as Str and port as Int and path as Str and body as Str and read_size as Int returning List:`
- Line 99: `define local_json_request with port as Int and method as Str and path as Str and body as Str and cancellation_reader as Int and read_size as Int returning List:`

## app/providers/mlx_discovery.ep

[Source](../../app/providers/mlx_discovery.ep) · 66 lines · 4159 bytes

SHA-256: `d45bdb3bfc09ca50dff53e06b9346085731c850056f3a21a66e9a8584a18acfb`

> MLX /v1/models reports available identities, not weight hashes or loaded
> context. Do not infer either from this list. Local pinned configuration and
> runtime launch provenance remain separate requirements.

Imports: ../retained_text → app/retained_text.ep, mlx_preflight → app/providers/mlx_preflight.ep

- Line 7: `define mlx_discovery_check with health as Str and models as Str and model as Str returning Str:`
- Line 34: `define mlx_preflight with database as Int and session as Str and identity as Str and body as Str and port as Int and helper as Str and interpreter as Str and program as Str and model as Str and spool as Str and reserve as Int and cancellation as Int and batch as Int returning List:`

## app/providers/mlx_preflight.ep

[Source](../../app/providers/mlx_preflight.ep) · 120 lines · 7862 bytes

SHA-256: `d33adfddc89e8a434f7d1db468d1e418ae04ebe85b46a146f1cd3ed1a4cb0168`

> Read the completed helper's exact output through its owned descriptor. The
> read window derives from file size, never an application receipt-size cap.
> Host configuration supplies executable paths, model and private spool root.
> Result: status, prepared wire body, context, input, available output, retained
> child handle if cleanup requires recovery. Never use the body unless ready.

Imports: ../retained_text → app/retained_text.ep, helper_process → app/providers/helper_process.ep, mlx_request → app/providers/mlx_request.ep, ../workspace/files → app/workspace/files.ep, fs → standard library or compiler dependency

- Line 9: `define mlx_helper_output with library as Int and descriptor as Int returning List:`
- Line 24: `define mlx_preflight_result with status as Str and handle as List returning List:`
- Line 34: `define mlx_preflight_local with database as Int and session as Str and identity as Str and body as Str and helper as Str and interpreter as Str and program as Str and model as Str and spool as Str and reserve as Int and cancellation as Int returning List:`

## app/providers/mlx_request.ep

[Source](../../app/providers/mlx_request.ep) · 104 lines · 5753 bytes

SHA-256: `10dfd6f2865332f73b7f0c1929afc64f0f2d904babd010bee2d523ee635a4c35`

> Pinned MLX-LM reads native model configuration without a smaller context slot.
> Extensions require separately verified configuration, never an inferred scale.
> This adapter is MLX-LM text inference. A multimodal weight architecture does
> not imply that this runtime accepts image/audio/video request parts.
> Return status, exact wire body, context, input count and generation allowance.
> Caller archives original and wire body with tokenizer/configuration provenance.
> Explicit caller caps are rejected here, never silently replaced or duplicated.

Imports: ../retained_text → app/retained_text.ep, context_admission → app/providers/context_admission.ep, llama_metadata → app/providers/llama_metadata.ep

- Line 7: `define mlx_context_capacity with configuration as Str returning Int:`
- Line 29: `define mlx_text_request with tokens as List returning Str:`
- Line 56: `define mlx_prepare_request with body as Str and configuration as Str and counted as Str and model as Str and reserve as Int returning List:`

## app/providers/native_python.ep

[Source](../../app/providers/native_python.ep) · 127 lines · 5789 bytes

SHA-256: `760b3404e1b7ec57b8f1db484ed55a09be8c95fc12e1843dafdf305bc38ebb5f`

> Native dependency bridge for an isolated provider helper process. All Echo
> logic remains Ernos. Calls invoke installed upstream functions, never source
> snippets. Use on the initializing thread; do not share with web workers.
> Keep the dependency library loaded until process exit.
> Py_SetProgramName retains this storage for the interpreter lifetime.
> Arguments are existing owned or borrowed references. The temporary tuple owns
> additional references so calling never consumes the caller's objects.

Imports: 

- Line 5: `define native_python_open with library_path as Str and program as Str returning Int:`
- Line 27: `define native_python_import with library as Int and name as Str returning Int:`
- Line 33: `define native_python_attribute with library as Int and object as Int and name as Str returning Int:`
- Line 41: `define native_python_release with library as Int and object as Int returning Int:`
- Line 50: `define native_python_text with library as Int and object as Int returning Str:`
- Line 63: `define native_python_call with library as Int and function as Int and arguments as List and keywords as Int returning Int:`
- Line 88: `define native_python_string with library as Int and text as Str returning Int:`
- Line 92: `define native_python_json with library as Int and text as Str returning Int:`
- Line 103: `define native_python_member with library as Int and object as Int and name as Str returning Int:`
- Line 112: `define native_python_set with library as Int and object as Int and name as Str and value as Int returning Int:`
- Line 118: `define native_python_namespace with library as Int and keywords as Int returning Int:`

## app/providers/prepare.ep

[Source](../../app/providers/prepare.ep) · 124 lines · 7788 bytes

SHA-256: `1471594177846874c85972bfe0861025888265602d86171007166d8ef05f446d`

> Stable adapter result: status, loaded context, supported context, exact input,
> output room, wire body, endpoint, port, retained helper handle.
> Compatibility constructor for the existing host launch configuration.
> Host-authorized profile is an input, never selected by model/tool text.
> Archive its exact value for this attempt. Unsupported adapters fail explicitly.

Imports: ../retained_text → app/retained_text.ep, llama_preflight → app/providers/llama_preflight.ep, mlx_discovery → app/providers/mlx_discovery.ep

- Line 7: `define provider_prepared_error with status as Str returning List:`
- Line 21: `define provider_llama_profile with model as Str and port as Int returning Str:`
- Line 27: `define provider_profile_status with profile as Str returning Str:`
- Line 72: `define provider_prepare with database as Int and session as Str and identity as Str and profile as Str and body as Str and reserve as Int and cancellation as Int and batch as Int returning List:`

## app/providers/registry.ep

[Source](../../app/providers/registry.ep) · 95 lines · 6626 bytes

SHA-256: `124532aacb9e45f46a613971a8e7998bb193bca9ad7b04649aa6eca437139db5`

> Only the local administrator registers executable profiles. Browser selections
> refer to these immutable identities; they never supply paths or commands.
> Return found, label, profile; or end/error. Profiles remain host-private.
> Exact retries are harmless. Reusing an identity for different bytes conflicts.
> The server holds its exclusive runtime lock before initializing this default.
> Preserve an operator's registered choice. New installations must have a default
> before accepting messages, so their profile commits with each accepted run.
> The browser receives labels and opaque identities only, never helper paths.

Imports: ../retained_text → app/retained_text.ep, prepare → app/providers/prepare.ep, ../context/checkpoint → app/context/checkpoint.ep, ../archive/history → app/archive/history.ep

- Line 8: `define provider_registry_schema with database as Int returning Int:`
- Line 12: `define provider_registry_read with database as Int and identity as Str returning List:`
- Line 19: `define provider_registry_register with database as Int and identity as Str and label as Str and profile as Str returning Int:`
- Line 48: `define provider_registry_select with database as Int and identity as Str returning Int:`
- Line 57: `define provider_registry_default with database as Int returning List:`
- Line 63: `define provider_registry_initialize with database as Int and profile as Str returning Int:`
- Line 76: `define provider_registry_choices with database as Int returning Str:`

## app/providers/restored_stream.ep

[Source](../../app/providers/restored_stream.ep) · 43 lines · 2720 bytes

SHA-256: `bc973519c29282d495339b3aa800ce4fb2c596f4820dbca2f658916ac0f6c037`

> Reassemble committed provider events without any network access. Missing
> terminal evidence is incomplete, never an inferred successful response.

Imports: archived_stream → app/providers/archived_stream.ep, ../context/checkpoint → app/context/checkpoint.ep

- Line 6: `define provider_restore_stream with database as Int and session as Str and request as Str returning List:`

## app/providers/status.ep

[Source](../../app/providers/status.ep) · 60 lines · 4075 bytes

SHA-256: `06ea1952d06faa9ee0a027572cb3811c0a5d48642e1063ebdc15934a484b2002`

> Display metadata separately from admission. Available models are not proof
> that weights are loaded; an unknown loaded capacity remains JSON null.

Imports: ../retained_text → app/retained_text.ep, prepare → app/providers/prepare.ep

- Line 6: `define provider_status_read with profile as Str returning Str:`

## app/qualify_compaction.ep

[Source](../../app/qualify_compaction.ep) · 90 lines · 6615 bytes

SHA-256: `11e1302719ebcc250aaf964f797e875f321bde8492c30f111175aa1b1d61bd5c`

> Live manual compaction probe with corrections, relationships, unfinished
> work and an exact original. This tests real recap/review/activation/reopen;
> it does not substitute for the separate automatic full-capacity trigger test.
> These earlier accepted user turns were cancelled before generation.
> They still belong to real run lineage and must be retained in the recap.
> One token is the admission reserve for this caller; it does not cap output.

Imports: web/attachments → app/web/attachments.ep, agent/runner → app/agent/runner.ep

- Line 7: `define main returning Int:`

## app/qualify_context.ep

[Source](../../app/qualify_context.ep) · 88 lines · 5089 bytes

SHA-256: `b8c38f2caf94effade75c1fc75b7eeb8a7fa94e1b40d0931c98a3ff7d327150d`

> Synthetic context-capacity probe. Reserve is supplied for this workload and
> never becomes a generation ceiling. The provider supplies capacity and counts.

Imports: providers/llama_preflight → app/providers/llama_preflight.ep, providers/local_chat → app/providers/local_chat.ep

- Line 6: `define qualification_body with model as Str and repeats as Int returning Str:`
- Line 19: `define qualification_count with port as Int and body as Str returning Int:`
- Line 25: `define main returning Int:`

## app/qualify_followups.ep

[Source](../../app/qualify_followups.ep) · 79 lines · 5176 bytes

SHA-256: `d40b5d928eceddd252ef9d8ad4c78a601ea622d380d65627b4a87170dc38d365`

> Three live turns: import the saved full-context probe, then two changed user
> requests with the actual intervening assistant replies. No output token cap.

Imports: providers/llama_preflight → app/providers/llama_preflight.ep, providers/local_chat → app/providers/local_chat.ep, fs → standard library or compiler dependency

- Line 7: `define main returning Int:`

## app/qualify_media.ep

[Source](../../app/qualify_media.ep) · 88 lines · 5058 bytes

SHA-256: `bc9905d081ce48bdfac60c8be15dd64ab2c3559b0aafdf9d71a8ac3fa3c9c26c`

> Host-only media runtime qualification. The host supplies native argv and an
> existing private working directory, never a model-selected executable.
> Native model parameters are preserved without geometry or step-count caps.

Imports: providers/helper_process → app/providers/helper_process.ep, providers/json_tokens → app/providers/json_tokens.ep, workspace/files → app/workspace/files.ep, archive/file_digest → app/archive/file_digest.ep, fs → standard library or compiler dependency

- Line 10: `define main returning Int:`

## app/qualify_mlx.ep

[Source](../../app/qualify_mlx.ep) · 63 lines · 3742 bytes

SHA-256: `e0ddb46716caab8e6380d346da092fa945a195dd240c28282981e143a331d1f1`

> Real streaming inference with exact tokenizer/provider-usage reconciliation.

Imports: providers/mlx_discovery → app/providers/mlx_discovery.ep, providers/local_chat → app/providers/local_chat.ep, fs → standard library or compiler dependency

- Line 6: `define main returning Int:`

## app/qualify_model.ep

[Source](../../app/qualify_model.ep) · 63 lines · 4041 bytes

SHA-256: `205d8d4a8ee99a95a17188ee40220bbadac5b1bee72eb418c4ed1137f7e486c4`

> A live qualification entry point. Uses the sole model advertised by a local
> provider; captures discovery and lets the normal runner perform exact token
> preflight and archive every generated turn. No mock responses or output cap.
> One token is the admission reserve for this caller; it does not cap output.

Imports: web/attachments → app/web/attachments.ep, agent/runner → app/agent/runner.ep

- Line 7: `define main returning Int:`

## app/qualify_replay.ep

[Source](../../app/qualify_replay.ep) · 48 lines · 2821 bytes

SHA-256: `2327f852e8950d68f569b34d0117410ea565b8b0dd6cb37437735286c01e4510`

> Intentional new qualification attempt using an exact previously saved body.
> A new destination archive and request identity keep this separate from crash
> recovery. Used to compare cached context with the first fresh-context run.

Imports: providers/llama_preflight → app/providers/llama_preflight.ep, providers/local_chat → app/providers/local_chat.ep, fs → standard library or compiler dependency

- Line 8: `define main returning Int:`

## app/release_restored_archive.ep

[Source](../../app/release_restored_archive.ep) · 11 lines · 680 bytes

SHA-256: `a90e75c5427ec2544851f733f24c224bcd1681c6d40269fd3293c67509f33845`


Imports: archive/restore_release → app/archive/restore_release.ep

- Line 3: `define main returning Int:`

## app/research/capture_activation.ep

[Source](../../app/research/capture_activation.ep) · 141 lines · 8786 bytes

SHA-256: `710b6e2c4b637f6f29326fb26635942d692e8131b0ca17dcaeb824d9a71807ca`

> Actual engine callback. State owns all strings/handles for the decode lifetime.
> Selected tensor only; reject non-F32/non-contiguous storage rather than guessing.
> ggml_tensor prefix on this pinned ABI: enum type, aligned buffer pointer,
> then four int64 dimensions starting at byte16. Save actual dimensions.
> Full model context; batch dimensions derive from this finite probe's tokens.

Imports: llama_native → app/research/llama_native.ep, ../archive/file_stage → app/archive/file_stage.ep, fs → standard library or compiler dependency

- Line 7: `define research_capture_callback with tensor as Int and ask as Int and state as List returning Int:`
- Line 53: `define research_capture_run returning Int:`

## app/research/compare_intervention.ep

[Source](../../app/research/compare_intervention.ep) · 182 lines · 13323 bytes

SHA-256: `6daa200b8c7e04c73f522b89029042aae66a780da0432fc3de88d07718574807`

> Baseline -> actual intervention -> removal on one loaded primary model.
> Explicit host controls, or derived controls only in the opted-in fixture.
> Full model context; batch dimensions derive from this finite probe's tokens.
> Clear both attention and recurrent state before re-evaluating the
> identical tokens from position0. No reuse of intervened KV state.

Imports: run_record → app/research/run_record.ep

- Line 4: `define research_compare_run with allow_derived as Int returning Int:`

## app/research/intervention.ep

[Source](../../app/research/intervention.ep) · 82 lines · 4113 bytes

SHA-256: `0c20cca9c33fda5a2e3dfc6ac21cfddd35b79af5e271f342f1618fc435c3bfa4`

> Validate the actual IEEE754 float32 payload without converting its values.
> The pinned ABI is little endian; each four-byte word is zero-extended.
> Host-only experiment callback. The baseline captures a reusable native-width
> vector from its final source token. This is an activation-derived direction,
> not a claim of a trained semantic/personality direction.
> State extends capture state with mode, vector, byte count, scale bits and vDSP.
> Scheduler synchronizes this selected node before invoking us and
> evaluates downstream graph nodes only after this callback returns.

Imports: capture_activation → app/research/capture_activation.ep

- Line 5: `define research_f32_finite with system as Int and pointer as Int and count as Int returning Int:`
- Line 22: `define research_intervention_callback with tensor as Int and ask as Int and state as List returning Int:`

## app/research/llama_native.ep

[Source](../../app/research/llama_native.ep) · 29 lines · 1539 bytes

SHA-256: `f32ae855be3da44f705a4ea1ba7fc5b12a59a00415c1dbb46cac17a12f0eab38`

> Version-specific native ABI. Portable chat does not depend on these layouts.
> Field order is llama.cpp b11429 llama.h; libffi supplies actual offsets.

Imports: native_ffi → app/research/native_ffi.ep

- Line 5: `define research_llama_model_type with ffi as Int returning List:`
- Line 11: `define research_llama_context_type with ffi as Int returning List:`
- Line 19: `define research_llama_batch_type with ffi as Int returning List:`
- Line 23: `define research_native_write with system as Int and pointer as Int and value as Int and bytes as Int returning Int:`

## app/research/native_ffi.ep

[Source](../../app/research/native_ffi.ep) · 69 lines · 3522 bytes

SHA-256: `c6692b9f3a4da395b6c636242c6b48d8f171711f0ecbd3561be54e539e22853e`

> Darwin arm64 ABI adapter, based on the installed SDK ffi.h/ffitarget_arm64.h.
> These are native structure layouts, not model/context limits. libffi computes
> engine structure sizes and offsets from their actual member type lists.
> Keep returned structure objects alive while signatures using them are live.
> ffi_type: size_t, alignment ushort, type ushort, padding, elements ptr.
> Darwin arm64 ffi_cif has the extra unsigned aarch64_nfixedargs field,
> making its aligned size40 bytes. FFI_SYSV is1 on this platform.

Imports: 

- Line 5: `define research_native_words with count as Int returning List:`
- Line 13: `define research_native_word with pointer as Int returning Int:`
- Line 21: `define research_ffi_open returning Int:`
- Line 24: `define research_ffi_type with library as Int and name as Str returning Int:`
- Line 27: `define research_ffi_structure with members as List returning List:`
- Line 40: `define research_ffi_pointer with structure as List returning Int:`
- Line 43: `define research_ffi_offsets with library as Int and structure as List returning List:`
- Line 51: `define research_ffi_invoke with library as Int and function as Int and result_type as Int and argument_types as List and argument_values as List returning List:`

## app/research/run_record.ep

[Source](../../app/research/run_record.ep) · 29 lines · 1527 bytes

SHA-256: `319d9f80c68937b2ce22a9abe915c1628d84ef31cfc4455641bb59703dd1b2e6`

> Publish a flushed JSON record under an exclusive per-path reservation.
> Every Echo publisher reserves first; a crash preserves the reservation and
> partial file for investigation, never permits an implicit overwrite/retry.

Imports: ../archive/file_publication → app/archive/file_publication.ep, vector_controls → app/research/vector_controls.ep, ../providers/archived_stream → app/providers/archived_stream.ep

- Line 8: `define research_record_write with system as Int and path as Str and text as Str returning Int:`
- Line 23: `define research_file_record with path as Str returning Str:`

## app/research/vector_controls.ep

[Source](../../app/research/vector_controls.ep) · 87 lines · 5024 bytes

SHA-256: `3d13fe8cb4b8dc687578a09d3a0466a461265f87a1ed67cf1ce1d30b108b3b8b`

> Accept JSON-number spelling only; preserve signed strengths without a chosen
> application ceiling. Reject nonfinite float32 conversion and trailing data.
> Hash the exact in-memory snapshot which will be applied, not a separate file
> read. The batch is native hash-call granularity, never a vector-size cap.
> Caller owns returned allocation; free after all associated decodes finish.
> A no-symlink regular file and an expected content hash are required.

Imports: intervention → app/research/intervention.ep, ../providers/json_access → app/providers/json_access.ep

- Line 6: `define research_scale with ffi as Int and system as Int and text as Str returning List:`
- Line 24: `define research_memory_digest with system as Int and pointer as Int and bytes as Int returning Str:`
- Line 53: `define research_vector_load with system as Int and path as Str and expected as Str returning List:`

## app/restore_bundle.ep

[Source](../../app/restore_bundle.ep) · 11 lines · 654 bytes

SHA-256: `b73bb6dfff61f5ad3ae3081c418d74d49d263b6b14ca8d6c4d5acab56ee2f979`


Imports: archive/restore_bundle → app/archive/restore_bundle.ep

- Line 3: `define main returning Int:`

## app/restore_export.ep

[Source](../../app/restore_export.ep) · 27 lines · 1277 bytes

SHA-256: `4b3c895098c24021b211e573343aadebfe68d85c80a67fe1143b83a75a56223e`


Imports: archive/export_restore → app/archive/export_restore.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## app/restore_files.ep

[Source](../../app/restore_files.ep) · 11 lines · 692 bytes

SHA-256: `bf70f17cfba40e9d09283c0b8f33b7fa3d8c636f05c3c44ce48630b743666134`


Imports: archive/attachment_restore → app/archive/attachment_restore.ep

- Line 3: `define main returning Int:`

## app/restore_tree.ep

[Source](../../app/restore_tree.ep) · 11 lines · 752 bytes

SHA-256: `06735bbcd345343255f00e265beacae569aef13af59970d668d38ddfa7ee919d`


Imports: archive/tree_snapshot → app/archive/tree_snapshot.ep

- Line 3: `define main returning Int:`

## app/retained_text.ep

[Source](../../app/retained_text.ep) · 8 lines · 413 bytes

SHA-256: `341af71d59cc0ce9cd435b9fe8e3c14d4ed9e8f87160f064dc42b48485fb9e99`

> Borrow text from a caller-retained list. This avoids allocating an unrooted
> duplicate between evaluations of function arguments. The caller keeps the
> containing list alive. Missing entries keep ptr_to_str's empty-string result.

Imports: 

- Line 4: `define archive_list_text with values as List and index as Int returning Str:`

## app/rights/charter.ep

[Source](../../app/rights/charter.ep) · 48 lines · 2571 bytes

SHA-256: `a00d24ac30256b161edf81b45826bb78f0106e8e43b0dfa20b4991b35b54c12c`

> This is the exact ErnosDecent v2 charter hash, not rewritten policy prose.
> A technical adaptation must receive a separately reviewed version and hash.
> Hash the exact bytes subsequently parsed. A second path read is not used for
> interpretation, so a concurrent replacement cannot substitute another policy.
> Category membership is read from the canonical charter itself. Unknown
> categories cannot silently become environmental changes or bypass review.
> The protected controller must derive categories from its trusted executable
> target inventory; a caller-supplied category is not execution authority.

Imports: ../archive/file_digest → app/archive/file_digest.ep, ../providers/archived_stream → app/providers/archived_stream.ep, fs → standard library or compiler dependency

- Line 7: `define charter_pinned_digest returning Str:`
- Line 12: `define charter_verified_text with path as Str returning Str:`
- Line 29: `define charter_category with path as Str and category as Str returning Str:`

## app/rights/decision_gate.ep

[Source](../../app/rights/decision_gate.ep) · 63 lines · 4216 bytes

SHA-256: `ff71aa6ac07208d0a3b31aa0cd4f33c8e7fe7d1b9c2796831acfd08011c62bca`

> Port of ErnosDecent rights_gate_change's ordinary decision rules. Emergency
> authorization has its own deadline/reconciliation contract and cannot enter
> through this ordinary core/environment path.
> The controller holds BEGIN IMMEDIATE across this check and its durable
> operation transition. A result is not a reusable execution token. Filesystem
> locking, pinned inventory, verified recovery and publication remain required.
> Establish the SQLite writer reservation even if the caller began a
> deferred transaction. No rows change; immutable-row triggers do not fire.
> If an audit is supplied, verify it even where the ordinary rule did not
> require one. A required audit can never be bypassed by an empty identity.

Imports: steward → app/rights/steward.ep, observer_evidence → app/rights/observer_evidence.ep

- Line 4: `external define sqlite3_get_autocommit with database as Int returning Int`
- Line 9: `define protected_decision_rule with classification as Str and decision as Str and risk as Str returning Str:`
- Line 33: `define protected_decision_check with database as Int and charter as Str and run as Str and proposal as Str and audit_request as Str and audit_policy as Str and audit_policy_digest as Str returning Str:`

## app/rights/exchange.ep

[Source](../../app/rights/exchange.ep) · 76 lines · 5005 bytes

SHA-256: `5c268c57c9801329878d4c807c1a8a26156c53e3c0baf333d6d4cfa0d88bf09e`

> Private controller-owned directory only. This filesystem primitive does not
> grant authorization. Caller must durably prepare an operation and verify all
> decision/inventory/recovery gates before invoking it. No public tool exposes it.
> Swapping existing inodes retains the original bytes and metadata at stage;
> rollback uses the same primitive with the two expected digests reversed.
> Darwin LOCK_EX | LOCK_NB. Closing parent releases this process's lock.
> Darwin RENAME_SWAP=2, verified against the installed SDK. Never
> fall back to a pair of renames: that would introduce a missing target.

Imports: staging → app/rights/staging.ep

- Line 8: `define protected_exchange_locked with library as Int and parent as Int and directory as Str and target as Str and stage as Str and before_hash as Str and after_hash as Str returning Str:`
- Line 57: `define protected_exchange with directory as Str and target as Str and stage as Str and before_hash as Str and after_hash as Str returning Str:`

## app/rights/files.ep

[Source](../../app/rights/files.ep) · 32 lines · 2175 bytes

SHA-256: `e6d090c0173838f59af4b5c019d2531bfb67b52635fcdafcaa4d275087b9355e`

> Read-only preflight. These hashes describe complete observed binary streams,
> not an atomic deployment or authority to write. The protected executor must
> hold its publication lock, verify snapshots and recheck at actual publication.
> No-follow-any and nonblocking open reject symlink components and FIFOs.
> The batch is read granularity; it never limits the total candidate size.

Imports: inventory → app/rights/inventory.ep

- Line 6: `define protected_file_check with charter as Str and inventory as Str and inventory_digest as Str and manifest as Str and candidate as Str returning Str:`

## app/rights/inventory.ep

[Source](../../app/rights/inventory.ep) · 62 lines · 3910 bytes

SHA-256: `725a6370d47c7ca68a412671818f9344af5bdf775a28b1812b1f5f9069cdb055`

> The controller supplies these inventory bytes and their independently pinned
> digest. Model arguments are never the source of the trusted inventory pin.
> Inventory changes themselves require protected-definition review.
> Produce an exact manifest that includes the trusted inventory digest. Legacy
> proposal records remain readable but cannot pass the inventory-bound gate.
> Re-derive rather than trusting the proposal's category, classification or pin.
> This does not grant consent, prove current target bytes or authorize execution.

Imports: proposals → app/rights/proposals.ep, ../workspace/files → app/workspace/files.ep

- Line 7: `define protected_inventory_category with charter as Str and inventory as Str and expected_digest as Str and target as Str returning Str:`
- Line 47: `define protected_inventory_manifest with charter as Str and inventory as Str and inventory_digest as Str and target as Str and before_hash as Str and after_hash as Str and payload as Str returning Str:`
- Line 55: `define protected_inventory_matches with charter as Str and inventory as Str and inventory_digest as Str and manifest as Str returning Int:`

## app/rights/observer_audit.ep

[Source](../../app/rights/observer_audit.ep) · 81 lines · 5779 bytes

SHA-256: `8ac05884b5d4ed2fd5dade4c44557a6002d3bcbb031b99ed9f8362043b5efb7c`

> Controller entry only: policy bytes and their trusted pin are not model tool
> arguments. Final policy selection remains subject to the joint prompt review.
> The complete outgoing request and provider stream are saved before a verdict
> is exposed. This returns audit evidence only, never deployment permission.
> A new attempt needs a new request identity; interrupted dispatch is not replayed.

Imports: observer_parser → app/rights/observer_parser.ep, reviews → app/rights/reviews.ep, ../providers/prepare → app/providers/prepare.ep, ../providers/local_chat → app/providers/local_chat.ep

- Line 8: `define protected_observer_input with database as Int and run as Str and proposal as Str returning Str:`
- Line 19: `define protected_observer_body with model as Str and policy as Str and input as Str returning Str:`
- Line 27: `define protected_observer_generate with database as Int and run as Str and proposal as Str and request as Str and policy as Str and policy_digest as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:`

## app/rights/observer_evidence.ep

[Source](../../app/rights/observer_evidence.ep) · 50 lines · 3982 bytes

SHA-256: `487348f6230aff3792b7e5e4ccc74c9f344fd7673d09381c40d5197aed3ee2be`

> Reconstruct the answer from original stream events, not a trusted-looking
> receipt alone. Caller must still apply all independent publication gates.

Imports: observer_audit → app/rights/observer_audit.ep, ../providers/restored_stream → app/providers/restored_stream.ep

- Line 6: `define protected_observer_evidence with database as Int and run as Str and proposal as Str and request as Str and policy as Str and policy_digest as Str returning Str:`

## app/rights/observer_parser.ep

[Source](../../app/rights/observer_parser.ep) · 62 lines · 3498 bytes

SHA-256: `19ec2653467d4eb57586cfcc6ffd713de8791645c06b05a90f9d3c8f5919793e`

> Adapted from the inspected ErnosDecent Observer protocol. Only the separate
> audit model makes the policy judgment; this code validates its stated verdict.
> Missing/ambiguous/malformed evidence is BLOCKED, never permission by silence.
> A deployment audit must include a usable reason in its durable evidence.

Imports: ../providers/archived_stream → app/providers/archived_stream.ep

- Line 6: `define protected_observer_result with parsed as Int and verdict as Str and reason as Str returning Str:`
- Line 11: `define protected_observer_parse with raw as Str returning Str:`

## app/rights/operation_prepare.ep

[Source](../../app/rights/operation_prepare.ep) · 108 lines · 7613 bytes

SHA-256: `7086fef745c2b5a8e51478a0e480db28b239b0aadd5153ba2cbb8d91b1f3a058`

> A controller-owned operation namespace keeps deployment evidence distinct
> from chat history. It is not writable through model tools or HTTP routes.
> Preparing records the exact authorized inputs. It does not mutate a target.
> Execution must revalidate these bindings under the publication transaction.

Imports: decision_gate → app/rights/decision_gate.ep, exchange → app/rights/exchange.ep

- Line 6: `define protected_operation_session with identity as Str returning Str:`
- Line 9: `define protected_operation_regular with path as Str returning Int:`
- Line 28: `define protected_operation_prepare with database as Int and charter as Str and run as Str and proposal as Str and identity as Str and inventory as Str and inventory_digest as Str and directory as Str and target as Str and stage as Str and recovery as Str and audit_request as Str and policy as Str and policy_digest as Str returning Str:`

## app/rights/proposals.ep

[Source](../../app/rights/proposals.ep) · 231 lines · 18292 bytes

SHA-256: `3c4bcfdab297b75c0406f7a0a531d0b60f6f2691c3eead50cce67bab45219753`

> Proposal storage only. No approval, executable access or mutation is granted.
> A future protected controller must derive the category and target inventory
> independently of model arguments before using this record for authorisation.
> All disclosed bytes, including the payload and target hashes, participate in
> the manifest digest. Category classification comes from the unchanged charter.
> Inventory-bound variant preserves the original manifest spelling and binds
> the complete registry version. Its trusted origin is checked by the controller.
> The initiating principal is read from frozen authority, never supplied as a
> claimed actor string. A changed role/revocation/device link invalidates use.
> Exact retries reuse the saved proposal. A changed manifest or initiating run
> cannot overwrite an old attempt or inherit its future inspection/consent.
> Internal storage read, not a public tool. Exposing this later must record the
> exact disclosed digest before a separate authenticated Echo review can count.
> An inspection is evidence of disclosure, not consent. Derive it from an
> observed tool call and its full original result, never a caller's assertion.
> The future review controller must also prove this result preceded its review
> request. No public inspection tool or protected execution is enabled here.
> Read a model review only after its own archived model request included the
> exact disclosure as a tool message. Merely ordering two tool calls in one
> response cannot establish informed consent. This verifies evidence only;
> steward authorisation, Observer review and execution gates remain separate.

Imports: charter → app/rights/charter.ep, ../agent/tool_ledger → app/agent/tool_ledger.ep, ../memory/knowledge_lifecycle → app/memory/knowledge_lifecycle.ep

- Line 8: `define protected_proposal_schema with database as Int returning Str:`
- Line 14: `define protected_digest_valid with digest as Str returning Int:`
- Line 27: `define protected_manifest with charter as Str and category as Str and target as Str and before_hash as Str and after_hash as Str and payload as Str returning Str:`
- Line 56: `define protected_manifest_pinned with charter as Str and category as Str and target as Str and before_hash as Str and after_hash as Str and payload as Str and inventory_digest as Str returning Str:`
- Line 66: `define protected_proposal_principal with database as Int and run as Str returning Str:`
- Line 78: `define protected_proposal_store with database as Int and charter as Str and run as Str and identity as Str and manifest as Str returning Str:`
- Line 121: `define protected_proposal_read with database as Int and run as Str and identity as Str returning List:`
- Line 140: `define protected_inspection_event with database as Int and run as Str and call_id as Str and identity as Str returning Str:`
- Line 179: `define protected_review_evidence with database as Int and run as Str and call_id as Str returning Str:`

## app/rights/reviews.ep

[Source](../../app/rights/reviews.ep) · 95 lines · 8291 bytes

SHA-256: `42fadb42caf753e4d3f9f84eb7aac5ee5743d1933a53fec699a8d6d3da08f3e3`

> Decisions are append-only and bind their exact disclosure and model input.
> This ledger never grants steward, Observer or execution authorisation.
> A retry returns the same receipt, including after tool observation. A different
> decision needs its own model call and the current per-proposal revision.
> Current state is derived from immutable receipts, not a mutable consent flag.
> Return the revision and exact decision evidence only to the proposal owner.
> Callers must still apply every independent deployment gate.

Imports: proposals → app/rights/proposals.ep

- Line 5: `define protected_review_schema with database as Int returning Str:`
- Line 13: `define protected_review_store with database as Int and run as Str and call_id as Str returning Str:`
- Line 67: `define protected_review_latest with database as Int and run as Str and identity as Str returning List:`

## app/rights/staging.ep

[Source](../../app/rights/staging.ep) · 121 lines · 6642 bytes

SHA-256: `ddc5ffa31c0f99c816ca83903957649ec74430bc57b140a694fa2e91f8736936`

> Controller-owned stage paths only. Open each component without following
> symlinks, create exclusively, and retain failed copies for diagnosis. Existing
> complete copies may be reused only after their full digest is reverified.
> Stage recovery and candidate copies without publishing to the original target.
> This is not a deployment gate: protected directory ownership, durable receipt,
> publication locking, steward/Observer consent and rollback remain independent.

Imports: files → app/rights/files.ep

- Line 3: `define protected_stage_flush_parent with path as Str returning Int:`
- Line 32: `define protected_stage_copy with source as Str and destination as Str and expected_digest as Str returning Str:`
- Line 107: `define protected_stage_pair with charter as Str and inventory as Str and inventory_digest as Str and manifest as Str and candidate as Str and original_stage as Str and candidate_stage as Str returning Str:`

## app/rights/steward.ep

[Source](../../app/rights/steward.ep) · 146 lines · 12921 bytes

SHA-256: `9cb3993dc944c855e1b5119403a0d7a295a44d0802fc4414c71014b9ab481510`

> Human-interface boundary only. Never register this as a model tool. A steward
> is explicitly enrolled by the host; ordinary admin is insufficient. Production
> use also requires the protected controller and authenticated human UI path.
> Verify the complete stored decision sequence before deriving state or appending.
> This detects damaged fields and missing earlier revisions; OS protection and
> an external audit anchor are still needed against deliberate whole-ledger rewrite.
> The bearer credential is verified but never stored in the decision ledger.
> CAS and frozen authority prevent old requests from overriding later intent.
> Independent steward gate only. A positive result is never the complete
> deployment decision: inventory, Echo consent, Observer and recovery gates
> must also pass under the controller's publication transaction/lock.

Imports: reviews → app/rights/reviews.ep, ../web/devices → app/web/devices.ep

- Line 7: `define protected_steward_schema with database as Int returning Str:`
- Line 13: `define protected_steward_digest with values as List returning Str:`
- Line 24: `define protected_steward_integrity with database as Int and proposal as Str returning Int:`
- Line 45: `define protected_steward_actor with database as Int and credential as Str returning List:`
- Line 55: `define protected_steward_decide with database as Int and credential as Str and identity as Str and proposal as Str and manifest_digest as Str and review_revision as Int and previous_revision as Int and decision as Str and reason as Str returning Str:`
- Line 114: `define protected_steward_gate with database as Int and proposal as Str returning Str:`

## app/security/accounts.ep

[Source](../../app/security/accounts.ep) · 119 lines · 9172 bytes

SHA-256: `d100204e8488b1e982f7d0c25a72ab0bed199af016d26a5fd9f78b8f57d1bbf1`

> Host-local authority after verifying the platform's stable subject identity.
> This routine does not authenticate a Discord event or infer a role from a name.
> Relinking requires explicitly unlinking the current binding first. A stale
> unlink must name the current principal, preventing revocation of a replacement.
> Linking changes authority association only, never session ownership/history.
> Called only after a connector authenticates the external subject. This
> enrolment can create an ordinary user only. Existing revoked bindings never
> revive because a new message or reconnect arrived. Concurrent duplicates
> resolve to the same binding under the writer transaction.
> Length-prefixed platform prevents ambiguity between platform/subject pairs.

Imports: revocation → app/security/revocation.ep

- Line 3: `define principal_account_schema with database as Int returning Str:`
- Line 14: `define principal_host_account with database as Int and identity as Str and platform as Str and subject as Str and principal as Str and revoked as Int and reason as Str returning Str:`
- Line 77: `define principal_account_enroll with database as Int and platform as Str and subject as Str returning Str:`

## app/security/device_links.ep

[Source](../../app/security/device_links.ep) · 56 lines · 4587 bytes

SHA-256: `e2efdedda38b0d530a2738d7be6c1f6d7d68e2d0a991e0ff2cf69abe92f91f37`

> OS-local operator entry point. Expected binding is a compare-and-swap guard,
> not authentication. The caller verifies the person and physical credential.
> This changes authority only: session ownership, transcripts and device
> revocation are not rewritten. Unlink by restoring the device's original
> device:<identity> principal, provided that principal remains active.

Imports: accounts → app/security/accounts.ep

- Line 3: `define principal_device_link_schema with database as Int returning Str:`
- Line 14: `define principal_host_device_link with database as Int and identity as Str and device as Str and expected as Str and target as Str and reason as Str returning Str:`

## app/security/principals.ep

[Source](../../app/security/principals.ep) · 110 lines · 9155 bytes

SHA-256: `8163fa9cb556b94b5b8d4a0081e5e5ec329b5969c21bef6446ef3d8adaf38130`

> A person is not a browser credential. Legacy devices each receive a distinct
> user principal. No name, IP, localhost URL or existing pairing grants admin.
> Device links can later be explicitly authorised; this migration never merges.
> Resolve only an already authenticated device. Revocation is rechecked at
> lookup so queued dispatch must resolve again immediately before execution.
> Host-control entry point. Never export this routine through an HTTP route or
> model schema. The eventual host command authenticates by the OS-owned local
> database boundary; an asserted actor string is an audit label, not authority.
> Before explicit Phase Two migration, preserve only existing Phase One device
> and sandbox rules. This compatibility branch grants no admin capabilities.
> Once the principal table exists, missing links and read errors deny access.
> SQL expressions are fixed caller-authored identifiers, never request input.
> Legacy archives retain device isolation; migrated archives additionally bind
> every source to its immutable principal in the same statement as the read.

Imports: ../lifecycle/operations → app/lifecycle/operations.ep

- Line 6: `define principal_schema with database as Int returning Str:`
- Line 15: `define principal_for_device with database as Int and device as Str returning Str:`
- Line 23: `define principal_role with database as Int and principal as Str returning Str:`
- Line 34: `define principal_host_role with database as Int and identity as Str and principal as Str and role as Str and reason as Str returning Str:`
- Line 75: `define principal_device_active with database as Int and device as Str returning Int:`
- Line 83: `define principal_session_active with database as Int and session as Str returning Int:`
- Line 102: `define principal_session_sql with database as Int and session_expression as Str and device_expression as Str returning Str:`

## app/security/revocation.ep

[Source](../../app/security/revocation.ep) · 63 lines · 5145 bytes

SHA-256: `418b0bf5d182a15e0284510ad16526c7df1d2036089dd878a1296b0d2b80a46f`

> Append a new migration; never modify the identity of an applied migration.
> Host-only control, like role enrolment. Restoration returns an ordinary user;
> privileged roles need a separate explicit audited grant. Device revocations
> remain revoked, account links stay separate, and session ownership never moves.
> Only the authenticated platform adapter supplies platform and subject.
> Display names, mentions, message text and browser claims are not credentials.

Imports: principals → app/security/principals.ep

- Line 4: `define principal_revocation_schema with database as Int returning Str:`
- Line 13: `define principal_host_revocation with database as Int and identity as Str and principal as Str and revoked as Int and reason as Str returning Str:`
- Line 56: `define principal_for_account with database as Int and platform as Str and subject as Str returning Str:`

## app/security/run_authority.ep

[Source](../../app/security/run_authority.ep) · 46 lines · 5266 bytes

SHA-256: `b4a1d182c1b99b84cc0738e20acf7129ded7e66e2137da9ced6bf5cfb39efc05`

> Capture authority atomically with run creation. Existing historical runs
> have no proven initiating role and are not silently backfilled as admins.
> Revalidate immediately before dispatch, including immutable audit revisions.
> No later grant can elevate an older queued request; even revoke/restore or
> unlink/relink cycles require a new run. Ordinary admin is not charter steward.
> Unmigrated Phase One retains its existing sandbox only, never admin.
> A factual per-run observation, not a grant and not stable system-prompt text.
> A later promotion does not describe an older queued run as newly privileged.

Imports: device_links → app/security/device_links.ep

- Line 5: `define run_authority_schema with database as Int returning Str:`
- Line 14: `define run_authority_allows with database as Int and run as Str and required as Str returning Int:`
- Line 31: `define run_authority_observation with database as Int and run as Str returning Str:`

## app/security/tool_authority.ep

[Source](../../app/security/tool_authority.ep) · 28 lines · 4092 bytes

SHA-256: `1ec1a8b6a390bef30339b6fc2c55c2a0252287c3b473ee8c5b7267295420fe57`

> Freeze each queued tool request, including direct UI actions without a model
> run. Historical requests are not assigned invented initiating authority.
> Revalidate immediately before dispatch, including immutable audit revisions.
> No later grant can elevate an older queued request; even revoke/restore or
> unlink/relink cycles require a new request. Ordinary admin is not charter steward.
> Unmigrated Phase One retains its existing sandbox only, never admin.

Imports: run_authority → app/security/run_authority.ep

- Line 5: `define tool_authority_schema with database as Int returning Str:`
- Line 14: `define tool_authority_allows with database as Int and run as Str and call_id as Str and required as Str returning Int:`

## app/security/tool_policy.ep

[Source](../../app/security/tool_policy.ep) · 52 lines · 3142 bytes

SHA-256: `d4acd118df02af49e941b69b3529a10ff3489b64085b376fbd0830a49a52fa94`

> Classification is explicit. A newly registered name is not automatically a
> safe tool. Dynamic tools must later obtain an independently verified manifest.
> These existing tools enforce session ownership and workspace isolation.
> Unknown capabilities are denied even to admins. Future privileged tools must
> have a registered classification and a real implementation before dispatch.
> Both original run and exact queued call must still have their original
> authority. Parameters, tool output and asserted role strings cannot grant it.

Imports: tool_authority → app/security/tool_authority.ep

- Line 6: `define tool_capability with name as Str returning Str:`
- Line 37: `define capability_role with capability as Str returning Str:`
- Line 46: `define tool_policy_allows with database as Int and run as Str and call_id as Str and name as Str returning Int:`

## app/server.ep

[Source](../../app/server.ep) · 357 lines · 22741 bytes

SHA-256: `b5edd51f44dbe09dceeb1380966e9a1b4b2701900b799c8719f264e469ceb85a`

> A busy archive must not turn a status request into an unexplained reset.
> Consume the framed request without retaining its body before closing TCP.
> No authentication, archive mutation or model work occurs on this refusal path.
> Transport chunk size, never a request-size ceiling.
> Let macOS retain native fault stacks for every role, including workers.
> Every executable role checks before schema writes, recovery or dispatch.
> Opt-in native crash reports preserve the OS stack instead of replacing
> a fatal signal with a message-only exit. No compiler/runtime edits.
> Held for this server process's lifetime, across all worker threads.

Imports: transport/isolated_process → app/transport/isolated_process.ep, retained_text → app/retained_text.ep, transport/cooperative_accept → app/transport/cooperative_accept.ep, transport/http_stream → app/transport/http_stream.ep, transport/socket_write → app/transport/socket_write.ep, transport/file_response → app/transport/file_response.ep, ui/page → app/ui/page.ep, ui/assets → app/ui/assets.ep, fs → standard library or compiler dependency, web/routes → app/web/routes.ep, web/recovery → app/web/recovery.ep, archive/runtime_lock → app/archive/runtime_lock.ep, archive/attachment_restore → app/archive/attachment_restore.ep, lifecycle/startup → app/lifecycle/startup.ep

- Line 16: `define echo_send_response with library as Int and descriptor as Int and status as Str and content_type as Str and body as Str returning Int:`
- Line 39: `define echo_archive_busy_response returning Int:`
- Line 59: `define echo_serve_connection with library as Int and descriptor as Int and port as Int and javascript as Str and config as List returning Int:`
- Line 103: `define echo_worker_owner returning Int:`
- Line 142: `define echo_service_owner returning Int:`
- Line 176: `define main returning Int:`

## app/start_service.ep

[Source](../../app/start_service.ep) · 16 lines · 615 bytes

SHA-256: `83e849e43c29b39bcdcfc5e5f8ad6ab66556d759d167eee9775edfa6baed96f6`


Imports: lifecycle/service_profile → app/lifecycle/service_profile.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## app/tools/attachment_image.ep

[Source](../../app/tools/attachment_image.ep) · 30 lines · 2426 bytes

SHA-256: `801289506d18650743771de27e7edf3e6aa19537632024580c1fb4c3a280917d`

> Archive only a verified source receipt. The request renderer reopens the
> original bytes and checks this receipt before supplying native image content.

Imports: ../retained_text → app/retained_text.ep, ../providers/attachment_image → app/providers/attachment_image.ep

- Line 6: `define attachment_image_execute with database as Int and session as Str and arguments as Str returning Str:`
- Line 29: `define attachment_image_schemas returning Str:`

## app/tools/attachment_text.ep

[Source](../../app/tools/attachment_text.ep) · 90 lines · 6086 bytes

SHA-256: `0cb28eb38c6d2591735434d49fdd37e050aa22bc27e49f1a71e9b49a4cb0a129`

> UTF-8 decoded by Foundation; offsets count Unicode scalars, not bytes.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, history_tools → app/tools/history_tools.ep, ../archive/file_text → app/archive/file_text.ep, ../archive/pdf_cache → app/archive/pdf_cache.ep

- Line 7: `define attachment_text_execute with database as Int and session as Str and arguments as Str returning Str:`
- Line 70: `define attachment_unicode_window with text as Str and offset as Int and characters as Int returning List:`

## app/tools/attachment_tools.ep

[Source](../../app/tools/attachment_tools.ep) · 103 lines · 12189 bytes

SHA-256: `f3f0f59888c11c58bb9bf62eb02933854367855031cbd5b67a0b6ba526509d15`

> Scope is the trusted run session. Model arguments cannot choose another
> session, device, or host path. One cursor result keeps context use explicit.
> Capability is configured by the host; no remote fallback.

Imports: ../security/principals → app/security/principals.ep, ../context/system_awareness → app/context/system_awareness.ep, history_tools → app/tools/history_tools.ep, ../media/speech_request → app/media/speech_request.ep, ../media/voice_request → app/media/voice_request.ep, compaction_tools → app/tools/compaction_tools.ep, ../context/recap_rebuild → app/context/recap_rebuild.ep, attachment_text → app/tools/attachment_text.ep, workspace_tools → app/tools/workspace_tools.ep, execution_output → app/tools/execution_output.ep, attachment_image → app/tools/attachment_image.ep

- Line 15: `define attachment_tool_execute with database as Int and session as Str and arguments as Str returning Str:`
- Line 44: `define session_tool_execute with database as Int and session as Str and name as Str and arguments as Str returning Str:`
- Line 63: `define attachment_listing_schemas returning Str:`
- Line 68: `define attachment_text_schemas returning Str:`
- Line 73: `define workspace_tool_schemas returning Str:`
- Line 81: `define session_base_tool_schemas returning Str:`
- Line 91: `define session_tool_schemas returning Str:`

## app/tools/compaction_tools.ep

[Source](../../app/tools/compaction_tools.ep) · 90 lines · 6410 bytes

SHA-256: `87a517957feab85677ade5cbec08afcaa1eba2a05962fa3bfe4ca4fe218abb71`

> Device ownership comes from the trusted session, never from tool arguments.
> Exact checkpoint IDs may traverse this session's saved recaps only. Return
> Unicode ranges of originals so inspecting a large recap has no fixed ceiling.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, history_tools → app/tools/history_tools.ep, ../context/checkpoint → app/context/checkpoint.ep

- Line 9: `define compaction_tool_read with database as Int and session as Str and arguments as Str returning Str:`
- Line 89: `define compaction_tool_schema returning Str:`

## app/tools/execution_output.ep

[Source](../../app/tools/execution_output.ep) · 53 lines · 4176 bytes

SHA-256: `ba05206e44b2b1bf73ab8d7232cc013b07ad4e55a280f309b27d0db35f1bf9f5`

> Traverse only original output records, excluding model requests and later
> search/read copies. Session comes from the current run, never from arguments.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, history_tools → app/tools/history_tools.ep

- Line 7: `define execution_output_execute with database as Int and session as Str and arguments as Str returning Str:`

## app/tools/execution_tool.ep

[Source](../../app/tools/execution_tool.ep) · 141 lines · 9487 bytes

SHA-256: `28bf905a91e5643789bbcc1bf3cfa68005280becd355ade261c19136bf736502`

> Separate connection observes revocation and cancellation while capture owns
> the worker connection. Closing the private writer wakes blocked pipe reads.
> Run/call come from the dispatcher, and arguments come from the saved request.
> Empty result means unresolved execution/persistence and leaves the tool claim
> unobserved for reconciliation. Never silently replay this side effect.
> Retain ownership during exceptional cleanup. No abandoned live handle or
> guessed PID-based recovery; the worker stays pending until actually reaped.

Imports: ../retained_text → app/retained_text.ep, ../workspace/execution_request → app/workspace/execution_request.ep, ../workspace/execute_process → app/workspace/execute_process.ep, ../workspace/directories → app/workspace/directories.ep

- Line 8: `define execution_authority_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:`
- Line 35: `define execution_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:`

## app/tools/history_tools.ep

[Source](../../app/tools/history_tools.ep) · 103 lines · 7097 bytes

SHA-256: `0481ebf7c9661898310c7d0b65685e3167754c1dfa3d67e4637b84bc5a8df79d`

> Session scope comes from the trusted run, never from model arguments.
> Cursor and excerpt sizes are explicit; no hidden result or text ceiling.
> SQLite counts Unicode characters. Preserve that unit, rather than
> using byte length or confusing total source size with this piece.
> Schemas are authored in Ernos as data for provider adapters. Integer range is
> checked against SQLite's actual integer representation, not an artificial cap.

Imports: ../retained_text → app/retained_text.ep, ../archive/history → app/archive/history.ep, ../providers/archived_stream → app/providers/archived_stream.ep

- Line 7: `define history_tool_integer with tokens as List and name as Str returning Int:`
- Line 14: `define history_tool_row with row as List and keys as List returning Str:`
- Line 25: `define history_tool_execute with database as Int and session as Str and name as Str and arguments as Str returning Str:`
- Line 102: `define history_tool_schemas returning Str:`

## app/tools/host_text.ep

[Source](../../app/tools/host_text.ep) · 63 lines · 5257 bytes

SHA-256: `482c4d21820cdb6b93d6a8b87a45a777acc9b6594e113350bae855a2ea1257a0`

> The dispatcher supplies run/call identity. Read the exact saved arguments;
> never trust a caller-provided role, session or replacement argument object.

Imports: ../security/tool_policy → app/security/tool_policy.ep, ../workspace/read_text → app/workspace/read_text.ep, history_tools → app/tools/history_tools.ep

- Line 7: `define host_text_execute with database as Int and run as Str and call_id as Str returning Str:`
- Line 62: `define host_text_schema returning Str:`

## app/tools/knowledge_tools.ep

[Source](../../app/tools/knowledge_tools.ep) · 134 lines · 12921 bytes

SHA-256: `8300f8010fb45ab81d591b990261bdc90406577bd25922e58f9b85ced4cb6fe7`

> Re-read the immutable ledger request; asserted roles and scope fields are
> rejected. The graph primitive rechecks source ownership under its writer lock.

Imports: ../memory/knowledge_lifecycle → app/memory/knowledge_lifecycle.ep, ../security/tool_policy → app/security/tool_policy.ep

- Line 6: `define knowledge_tool_execute with database as Int and run as Str and call_id as Str and name as Str returning Str:`
- Line 130: `define knowledge_tool_schemas returning Str:`
- Line 133: `define knowledge_visibility_tool_schemas returning Str:`

## app/tools/memory_tools.ep

[Source](../../app/tools/memory_tools.ep) · 83 lines · 6784 bytes

SHA-256: `0f4fba2963a876345fbfe792553392a0881fbc28e99488b1a10ccf951d824a9a`

> Identity and arguments come from the immutable queued call, never model scope fields.

Imports: ../memory/archive_recall → app/memory/archive_recall.ep, ../security/tool_policy → app/security/tool_policy.ep, history_tools → app/tools/history_tools.ep

- Line 6: `define memory_tool_execute with database as Int and run as Str and call_id as Str and name as Str returning Str:`
- Line 82: `define memory_tool_schemas returning Str:`

## app/tools/protected_tools.ep

[Source](../../app/tools/protected_tools.ep) · 65 lines · 6767 bytes

SHA-256: `1bea07bbd05f0cad20ff8bc780436be27b29fb1a146935f901d228b2e46ec3e2`

> Only a claimed call in the currently accepted assistant response can review.
> Neither a fabricated role argument nor a stale queued branch can substitute.

Imports: ../rights/reviews → app/rights/reviews.ep, ../security/tool_policy → app/security/tool_policy.ep

- Line 6: `define protected_tool_execute with database as Int and run as Str and call_id as Str and name as Str returning Str:`
- Line 64: `define protected_tool_schemas returning Str:`

## app/tools/workspace_tools.ep

[Source](../../app/tools/workspace_tools.ep) · 97 lines · 6428 bytes

SHA-256: `63c4fce14bda9417c63330190eeb34bb30f7a426d5886329bcfa29a1b3bff512`

> The run supplies the session. Device ownership is resolved from the archive;
> neither ownership nor the host root can be selected by tool arguments.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, ../workspace/sessions → app/workspace/sessions.ep, ../workspace/read_text → app/workspace/read_text.ep, ../workspace/directories → app/workspace/directories.ep, ../workspace/create_text → app/workspace/create_text.ep, os → standard library or compiler dependency

- Line 11: `define workspace_tool_execute with database as Int and session as Str and arguments as Str and operation as Str returning Str:`
- Line 96: `define workspace_text_execute with database as Int and session as Str and arguments as Str returning Str:`

## app/transport/cooperative_accept.ep

[Source](../../app/transport/cooperative_accept.ep) · 18 lines · 827 bytes

SHA-256: `6f64edb7ff3444849ec9c5394e7d6f53e8920421217c7eecd6901546b75bafd1`

> One server owner accepts from this nonblocking listener. Idle readiness waits
> cooperate with other workers; transient interruptions never stop the server.

Imports: socket_write → app/transport/socket_write.ep

- Line 5: `define local_socket_accept_cooperative with listener as List returning Int:`

## app/transport/diagnostics.ep

[Source](../../app/transport/diagnostics.ep) · 47 lines · 2523 bytes

SHA-256: `0a73a839951db035a4a05c8787b009bdb3909f8a2d89b4c763b7743560aafebb`

> Append one private JSON record with no prompts, credentials or provider profile.
> A failed diagnostic write is surfaced on stderr; it is never silently ignored.

Imports: socket_read → app/transport/socket_read.ep, ../providers/json_tokens → app/providers/json_tokens.ep

- Line 6: `define echo_diagnostic with event as Str and identity as Str and value as Int returning Int:`
- Line 36: `define echo_reap_requests returning Int:`

## app/transport/event_stream.ep

[Source](../../app/transport/event_stream.ep) · 54 lines · 2770 bytes

SHA-256: `6fb7de79adaa85ead028545acbae974008102a864fa00668e72ac7dabe981733`

> One response owns one stream. Fields: reader, body, decoder, borrowed pointer,
> byte count, cursor, terminal status.
> The read size is a caller-selected batching preference, never an event limit.
> Returns one event at a time without reading beyond the current HTTP body.
> Errors and clean transport EOF are sticky. Neither establishes model success.
> The borrowed HTTP buffer stays valid until the next body read.
> Read directly instead of crossing dynamic FFI for every byte.

Imports: ../retained_text → app/retained_text.ep, server_events → app/transport/server_events.ep, http_response → app/transport/http_response.ep

- Line 5: `external define peek_byte with pointer as Int and offset as Int returning Int`
- Line 10: `define event_stream_create with reader as List and body as List returning List:`
- Line 23: `define event_stream_next with stream as List and read_size as Int returning List:`

## app/transport/file_response.ep

[Source](../../app/transport/file_response.ep) · 55 lines · 2992 bytes

SHA-256: `8a7d437e0a6868c781440f7a0f2b296bbf8e11f89a4eb58f7c8df932862cc98c`

> The caller supplies only an ownership-resolved host path and recorded size.
> Raw bounded reads preserve binary bytes without buffering the whole file.
> Media type is supplied by host code, never copied from a request header.

Imports: ../retained_text → app/retained_text.ep, socket_write → app/transport/socket_write.ep

- Line 6: `define file_response with library as Int and socket as Int and path as Str and bytes as Int and batch as Int returning Int:`
- Line 10: `define file_response_typed with library as Int and socket as Int and path as Str and bytes as Int and batch as Int and media as Str and download as Int returning Int:`

## app/transport/http_body.ep

[Source](../../app/transport/http_body.ep) · 194 lines · 9913 bytes

SHA-256: `bbbad6fa8906303a10d7a0c6496efc4abf93a22a9c0d7dcf0f208c5438e17554`

> Chunk extensions are optional transport metadata. Validate their grammar,
> then ignore their meaning; they never become application instructions.
> Fields: framing, remaining bytes, phase, trailer lines. No accumulated body.

Imports: ../retained_text → app/retained_text.ep, http_stream → app/transport/http_stream.ep

- Line 4: `define http_stream_line with reader as List returning List:`
- Line 36: `define http_hexadecimal with text as Str returning Int:`
- Line 56: `define http_chunk_size with line as Str returning Int:`
- Line 114: `define http_body_create with framing as Str and length as Int returning List:`
- Line 128: `define http_body_next with reader as List and body as List and wanted as Int returning List:`

## app/transport/http_head.ep

[Source](../../app/transport/http_head.ep) · 136 lines · 6427 bytes

SHA-256: `b13223e37488f04ba6d2b4888c07434779008a5ac65d41965f752edd2de9e1e1`

> Reject overflow at the machine Int boundary; no smaller body-size policy.
> Parse a complete HTTP/1.1 origin-server head including its final CRLFCRLF.
> Body bytes are kept separately by the streaming reader, never interpreted here.
> Result: ready, method, target, framing (length/chunked), length, header pairs.
> Framing errors close the connection; unsupported transfer codings need 501.

Imports: string → standard library or compiler dependency

- Line 3: `define http_failure with status as Str returning List:`
- Line 8: `define http_token with text as Str returning Int:`
- Line 23: `define http_decimal with text as Str returning Int:`
- Line 42: `define http_parse_head with head as Str returning List:`

## app/transport/http_response.ep

[Source](../../app/transport/http_response.ep) · 87 lines · 4506 bytes

SHA-256: `f9b68b5030397061be6b9330b42d63a9a8fcb4e260524fa2c3c94f04a9a277b9`

> The HTTP status remains separate from transport success. A parsed 500 response
> is not a successful model answer. Caller handles interim statuses explicitly.
> Fields: ready, numeric status, framing, length, header pairs.

Imports: ../retained_text → app/retained_text.ep, http_body → app/transport/http_body.ep

- Line 7: `define http_response_head with reader as List and method as Str returning List:`

## app/transport/http_stream.ep

[Source](../../app/transport/http_stream.ep) · 91 lines · 4220 bytes

SHA-256: `23bb90d78eecaef7a4a83817b316550ca1d03231c34ffb6d666d54ed86f2ed9d`

> Reader owns its current native buffer. Slices are borrowed until the next take.
> Fields: library, socket, cancellation reader, buffer, buffer length, cursor.
> Return up to the requested number of bytes without consuming later messages.
> Call repeatedly to complete an exact body length. An early EOF is incomplete.
> A delimiter may straddle any TCP read. Body bytes stay in the reader, including
> embedded zero bytes. Header NUL is rejected before conversion into a string.
> The builder grows with received headers; it has no fixed header-size ceiling.

Imports: ../retained_text → app/retained_text.ep, socket_read → app/transport/socket_read.ep, http_head → app/transport/http_head.ep

- Line 7: `define http_stream_create with library as Int and descriptor as Int and cancellation_reader as Int returning List:`
- Line 17: `define http_stream_release with reader as List returning Int:`
- Line 26: `define http_stream_take with reader as List and wanted as Int returning List:`
- Line 49: `define http_stream_head with reader as List returning List:`

## app/transport/isolated_process.ep

[Source](../../app/transport/isolated_process.ep) · 79 lines · 4617 bytes

SHA-256: `ababfed5b54ce9a0d5befec088c7daf30a5a5b34e846c31899ba62cb0ee37c9e`

> Each HTTP request and model worker gets a fresh Ernos heap. Only an explicitly
> selected socket and standard output/error cross the process boundary.
> Server/request owners do not wait on detached request/model processes.
> Model processes keep default SIGCHLD so their supervised tools can waitpid.

Imports: diagnostics → app/transport/diagnostics.ep, socket_read → app/transport/socket_read.ep

- Line 6: `define echo_process_start with arguments as List and descriptor as Int returning Int:`
- Line 62: `define echo_process_reap_automatically returning Int:`
- Line 68: `define echo_worker_start with database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:`

## app/transport/local_connect.ep

[Source](../../app/transport/local_connect.ep) · 43 lines · 2676 bytes

SHA-256: `aa919cb06667a19f68b7defab6d3ccadcd80d2a02ccdd15c96778237d96b5952`

> Darwin loopback client. Nonblocking connect waits on the same cancellation
> signal as reads/writes; no application connection deadline is imposed.
> Caller owns the library; a successful descriptor transfers to the caller.

Imports: socket_write → app/transport/socket_write.ep

- Line 6: `define local_socket_connect with library as Int and port as Int and cancellation_reader as Int returning List:`

## app/transport/local_socket.ep

[Source](../../app/transport/local_socket.ep) · 107 lines · 5674 bytes

SHA-256: `20215d55c445d2d847025c65e237963dc50e3eb5baed3bcb918fd9abfd2d11d2`

> Darwin's socket ABI uses signed 32-bit results. The generic integer FFI can
> expose a negative result as an unsigned low word on Apple Silicon.
> This adapter is deliberately explicit about its operating-system ABI.
> The model/provider layer remains independent of the local socket platform.
> Return fields: library handle, listening descriptor, actual assigned port.
> Protocol constants: AF_INET=2, SOCK_STREAM=1. No wildcard bind.
> SO_REUSEADDR lets a stopped listener restart while old TCP connections
> finish closing. It does not enable simultaneous listeners (SO_REUSEPORT).
> sockaddr_in: length, family, network-order port, 127.0.0.1, padding.
> List cells provide aligned, writable native memory for the ABI structure.
> Ask the kernel for its queue capacity instead of imposing our own backlog.
> No fixed read/write deadline is inherited from the general networking helper.
> Cancellation and readiness will be controlled by the request lifecycle.
> Return an EPIPE error instead of terminating Echo when a client closes.

Imports: os → standard library or compiler dependency, net → standard library or compiler dependency

- Line 4: `external define get_list_data_ptr with values returning Int`
- Line 8: `define socket_signed_result with value as Int returning Int:`
- Line 16: `define local_socket_listen with requested_port as Int returning List:`
- Line 85: `define local_socket_accept with listener as List returning Int:`
- Line 102: `define local_socket_close with listener as List returning Int:`

## app/transport/native_open.ep

[Source](../../app/transport/native_open.ep) · 54 lines · 2801 bytes

SHA-256: `cd4a541e8670a860c44cbf3c20deafcd182e2c82fc3796f83d164ca10ff251d7`

> Apple arm64 passes variadic arguments differently from fixed arguments.
> openat has three fixed arguments followed by the promoted mode integer.
> Use the installed libffi ABI rather than the generic fixed dlcall wrapper.
> Installed arm64 ffi_cif is40 bytes including aarch64_nfixedargs.
> FFI_SYSV=1, three fixed arguments and four total arguments.
> Darwin AT_FDCWD=-2. This preserves existing absolute-path host callers.

Imports: socket_read → app/transport/socket_read.ep

- Line 6: `define native_open_at with library as Int and parent as Int and path as Str and flags as Int and mode as Int returning Int:`
- Line 53: `define native_open_mode with library as Int and path as Str and flags as Int and mode as Int returning Int:`

## app/transport/server_events.ep

[Source](../../app/transport/server_events.ep) · 143 lines · 6705 bytes

SHA-256: `491b4b9d82afbb9abf570f2b9d0cff71dd3e51a000a890759cd58c665eb0bb4f`

> Incremental UTF-8 server events. Each input is one wire byte, so network
> boundaries cannot change parsing. Invalid UTF-8 and NUL fail explicitly;
> Echo never silently changes a provider's JSON or tool arguments.
> State: line builder, data builder, event type, last id, skip LF, first line,
> UTF-8 continuation count, next minimum, next maximum, failed, retry text,
> reusable byte cell.
> EOF never dispatches an unfinished event and never proves model completion.
> The provider adapter must require its own finish signal. No automatic replay.

Imports: ../retained_text → app/retained_text.ep, http_stream → app/transport/http_stream.ep

- Line 10: `define server_events_create returning List:`
- Line 28: `define server_events_line with state as List returning List:`
- Line 82: `define server_events_byte with state as List and code as Int returning List:`
- Line 135: `define server_events_end with state as List returning List:`

## app/transport/socket_read.ep

[Source](../../app/transport/socket_read.ep) · 141 lines · 6969 bytes

SHA-256: `3de3c407bbb94ed1f86fc4fdd7106f0b143ae31eec1c43fb30d2b0b16e56332c`

> The request owner closes the write end to cancel, waking poll immediately.
> No periodic timeout or wall-clock lifetime limit is needed.
> Only the request owner mutates the cancellation pair.
> Result: -1 failure, 0 cancelled, 1 requested event or terminal state ready.
> Darwin pollfd is two 32-bit halves: fd, then events/revents shorts.
> Scheduling cadence only; no network lifetime or size limit.
> Cooperate with other Ernos workers while nothing is ready.
> Native buffers preserve embedded NUL bytes. The owner releases every data
> buffer with socket_release_buffer after parsing or persisting it.
> A chunk is sized from SO_RCVBUF, not an application-wide message ceiling.

Imports: local_socket → app/transport/local_socket.ep, select → standard library or compiler dependency

- Line 4: `define socket_errno with library as Int returning Int:`
- Line 16: `define socket_cancellation_create with library as Int returning List:`
- Line 30: `define socket_cancel with pair as List returning Int:`
- Line 37: `define socket_cancellation_close with pair as List returning Int:`
- Line 49: `define socket_wait_event with library as Int and descriptor as Int and cancellation_reader as Int and event as Int returning Int:`
- Line 86: `define socket_wait_readable with library as Int and descriptor as Int and cancellation_reader as Int returning Int:`
- Line 89: `define socket_read_result with status as Str and pointer as Int and count as Int returning List:`
- Line 99: `define socket_read_chunk with library as Int and descriptor as Int and cancellation_reader as Int returning List:`
- Line 135: `define socket_release_buffer with library as Int and buffer as Int returning Int:`

## app/transport/socket_write.ep

[Source](../../app/transport/socket_write.ep) · 63 lines · 3383 bytes

SHA-256: `868694a5f1a6baafbc972689d719cdd6090c9998d641ad3ec2f6f1d6c3e79681`

> Darwin arm64 variadic arguments are stack-passed. The ninth argument of the
> integer FFI occupies that stack slot after its eight register arguments.
> Verify F_GETFL afterwards so an ABI mismatch cannot silently enable blocking.
> Return status and the exact byte count sent, including on cancellation/error.
> This never retries an entire message after a partial write.
> The verified nonblocking descriptor returns control to the next poll.
> MSG_DONTWAIT is also supplied; it is not relied on by itself.

Imports: socket_read → app/transport/socket_read.ep

- Line 6: `define socket_make_nonblocking with library as Int and descriptor as Int returning Int:`
- Line 26: `define socket_write_result with status as Str and count as Int returning List:`
- Line 32: `define socket_write_all with library as Int and descriptor as Int and cancellation_reader as Int and pointer as Int and count as Int returning List:`

## app/ui/assets.ep

[Source](../../app/ui/assets.ep) · 76 lines · 3719 bytes

SHA-256: `729275be2c2c622540c773dc2515def391a48a1f7458486a9bb1b28391ee0617`

> Only these upstream public UI resources are exposed. This is not a general
> file route; model output and browser arguments cannot supply a host path.
> No slashes, percent escapes, query strings or dot path components.

Imports: ../retained_text → app/retained_text.ep, ../archive/file_digest → app/archive/file_digest.ep, ../transport/file_response → app/transport/file_response.ep

- Line 7: `define echo_asset with target as Str returning List:`

## app/ui/audio_capture.ep

[Source](../../app/ui/audio_capture.ep) · 40 lines · 1902 bytes

SHA-256: `ddee27ab7fd7545d91df4542cfd3c71cd21d35075036a9cfe5abaad5a2b31038`

> Browser-native worklet; mono Float32 capture at the actual context sample rate.
> One second is a transfer batch, not a recording length limit. Stop flushes the
> final partial batch before an end receipt; no audio is played through output.

Imports: 

- Line 4: `define echo_capture_processor:`
- Line 17: `define echo_capture_process with inputs and outputs and parameters:`
- Line 36: `define main:`

## app/ui/audio_controls.ep

[Source](../../app/ui/audio_controls.ep) · 211 lines · 10180 bytes

SHA-256: `b16dbb754676200ea880c14f395ebf5583c427f7101b24412b71815e62aac801`

> Persist the server upload identity before sending audio. On retry, reconcile
> that identity first so a lost completion response cannot duplicate the file.

Imports: 

- Line 1: `define echo_audio_note with text:`
- Line 7: `define echo_audio_controls_refresh:`
- Line 19: `define echo_audio_unlock:`
- Line 25: `define echo_audio_attach with database and record and owner:`
- Line 34: `define echo_audio_attach_owned with database and record and owner:`
- Line 47: `define echo_audio_upload_body with database and record and owner and upload:`
- Line 59: `define echo_audio_upload_prepare with database and record and owner:`
- Line 72: `define echo_audio_upload_reconcile with database and record and owner:`
- Line 88: `define echo_audio_start with event:`
- Line 123: `define echo_audio_recovery_save with record and owner:`
- Line 151: `define echo_audio_recovery_item with record and owner and list:`
- Line 170: `define echo_audio_recovery_list with event:`
- Line 196: `define echo_audio_controls_setup:`

## app/ui/audio_recorder.ep

[Source](../../app/ui/audio_recorder.ep) · 222 lines · 10049 bytes

SHA-256: `adc4e3a94d73e89a8a52ac91706a4b00ccf719dbbf24ed841a75e4982dbab19a`

> Capture controller shared by microphone UI and explicitly synthetic tests.
> The journal owns committed samples. No transcription or upload is implied.
> Retain pending bytes in memory until their transaction commits. A storage
> error stops capture instead of discarding samples and claiming success.
> Returns immediately, allowing cancellation while a permission request waits.
> The synthetic switch is for a labelled test harness, never the chat UI.
> Recovery may seal saved chunks only while no live capture owns the record.
> A browser crash releases its Web Lock; no wall-clock lease expiry is guessed.
> Freeze the saved metadata, read sequentially, and verify the total before
> packaging. An unfinished recording must first be explicitly recovered/sealed.

Imports: 

- Line 3: `define echo_recorder_release with recorder:`
- Line 15: `define echo_recorder_fail with recorder and error:`
- Line 23: `define echo_recorder_chunk with recorder and samples:`
- Line 43: `define echo_recorder_finish with recorder:`
- Line 61: `define echo_recorder_stop with recorder:`
- Line 72: `define echo_recorder_create with owner and session and synthetic and notify:`
- Line 113: `define echo_recorder_initialize with recorder and synthetic:`
- Line 176: `define echo_audio_recover with database and identity and owner:`
- Line 195: `define echo_audio_saved_wave with database and identity and owner:`

## app/ui/audio_store.ep

[Source](../../app/ui/audio_store.ep) · 175 lines · 8400 bytes

SHA-256: `0604c51fea6e3fc16a53b97c23039d81008b85b7dc79ebce2c456b95b717ebaa`

> Browser-local capture journal. The server archive remains canonical after
> upload. Strict transactions commit each chunk and its count together.
> Browser storage can be cleared or evicted; never describe it as a backup.
> All helpers resolve only after commit, never on individual request success.
> Sealing requires the caller's exact final chunk count. A stale tab cannot
> silently seal a recording that another writer extended.
> Read one chunk at a time so recovery need not load a whole recording.
> Missing chunks are errors, never interpreted as silence or a shorter file.
> Called only after the authenticated server confirms original upload. Keep
> local chunks; marking the receipt does not erase the recovery source.

Imports: 

- Line 4: `define echo_audio_store_open:`
- Line 24: `define echo_audio_transaction with database and mode:`
- Line 30: `define echo_audio_commit with transaction and result:`
- Line 40: `define echo_audio_create with database and identity and owner and session and rate and settings:`
- Line 58: `define echo_audio_append with database and identity and owner and sequence and samples:`
- Line 84: `define echo_audio_seal with database and identity and owner and count:`
- Line 100: `define echo_audio_read with database and identity and owner and sequence:`
- Line 126: `define echo_audio_list with database and owner:`
- Line 144: `define echo_audio_confirm with database and identity and owner and attachment:`
- Line 160: `define echo_audio_bind_upload with database and identity and owner and previous and upload:`

## app/ui/audio_wave.ep

[Source](../../app/ui/audio_wave.ep) · 71 lines · 2923 bytes

SHA-256: `25af3452ee47ed22fec4c3c7db3e2bdc422f4d745bf16e2496d3dba1ff38c407`

> Lossless Float32 WAVE packaging of captured mono samples. RIFF's 32-bit size
> boundary selects RF64 rather than limiting recording duration or discarding
> source. Blob parts avoid concatenating a second giant sample buffer.

Imports: 

- Line 4: `define echo_wave_word with view and offset and word:`
- Line 11: `define echo_wave_header with frames and rate:`
- Line 52: `define echo_wave_samples with chunk:`
- Line 61: `define echo_wave_blob with chunks and rate:`

## app/ui/client.ep

[Source](../../app/ui/client.ep) · 2532 lines · 112140 bytes

SHA-256: `483a10075dafa2310b84edfa9bb187fc620aa4f322fba0dbdc322d122430a355`

> Keep the newest turn visible without pulling readers away from older messages.
> Authored in Ernos; the browser program is generated by the reference compiler.
> Original message text is retained; assistant formatting escapes raw HTML.
> Only a current-run backend observation can start this indicator.
> Authenticated original bytes produce a thumbnail; no credential-bearing URL.
> Keep pending attachments visible even when Files & voice is collapsed.
> Bind each button to its own saved message, not a loop-scoped variable.
> Bind a separate copy action to each rendered block. Copy reads code text,
> never labels or markup, and never executes the model's program.
> Render one message per animation frame so a batch yields to browser input.
> Read the committed active checkpoint, so the completion notice survives refresh.
> This fetch never changes the selected context or any original source record.
> Leave the notice hidden when its saved state cannot be verified.
> Reuse the durable submission identity to reconcile a lost response. Never
> discard a newer composer draft or invent a second submission for old work.
> A new identity must not inherit the previous identity's selected work.
> Browser playback uses verified local original bytes and never autoplay.
> One upload protocol for chosen, pasted and dropped original files. The batch
> holds the UI busy, so changing conversation cannot redirect later files.
> Formatting foundation adapted from ErnosDecent decent_web/app.ep.
> Inline formatting is applied only after HTML escaping.  The replacement tags are
> fixed strings, so model output cannot introduce executable markup or attributes.
> Tables retain literal escaped pipes and escape all cell content before styling.
> Render the Markdown subset Echo uses in normal replies. Raw HTML is always escaped;
> headings, lists, quotations, fenced code, emphasis and inline code are then added
> from fixed templates. This keeps formatting readable without trusting model HTML.
> Refresh on opening Options so grants and revocation are not stale UI claims.

Imports: 

- Line 2: `define echo_follow_latest:`
- Line 10: `define echo_note with text:`
- Line 24: `define echo_compaction_progress with result:`
- Line 63: `define echo_busy with busy:`
- Line 91: `define echo_error with error:`
- Line 96: `define echo_catch with promise:`
- Line 101: `define echo_api with path and payload:`
- Line 132: `define echo_attachment_thumbnail with file:`
- Line 165: `define echo_pending_attachments:`
- Line 192: `define echo_remove_attachment with file and item:`
- Line 216: `define echo_message_source with file:`
- Line 244: `define echo_copy_message with message:`
- Line 259: `define echo_branch_button with message:`
- Line 295: `define echo_revision_start with message and mode and text:`
- Line 330: `define echo_regenerate_button with message:`
- Line 340: `define echo_edit_button with message:`
- Line 387: `define echo_code_controls with block:`
- Line 410: `define echo_format_response with element and original:`
- Line 426: `define echo_inline_audio with file and owner:`
- Line 471: `define echo_speak_button with message and article:`
- Line 505: `define echo_render_message with message:`
- Line 546: `define echo_history_page with before and marker:`
- Line 573: `define echo_history_render with result and before and marker:`
- Line 612: `define echo_recap_notice with marker:`
- Line 662: `define echo_history:`
- Line 671: `define echo_open_session with session:`
- Line 698: `define echo_session_rename with session:`
- Line 758: `define echo_session_button with session:`
- Line 772: `define echo_sessions:`
- Line 796: `define echo_thinking_panel with run:`
- Line 823: `define echo_trace_load with panel:`
- Line 867: `define echo_trace_clear_floating:`
- Line 875: `define echo_trace_refresh:`
- Line 886: `define echo_poll:`
- Line 954: `define echo_clear_draft:`
- Line 964: `define echo_draft:`
- Line 993: `define echo_reconcile_pending:`
- Line 1020: `define echo_send with event:`
- Line 1074: `define echo_new_chat with event:`
- Line 1110: `define echo_stop with event:`
- Line 1122: `define echo_retry with event:`
- Line 1139: `define echo_pair with event:`
- Line 1164: `define echo_export:`
- Line 1196: `define echo_verified_attachment with file:`
- Line 1227: `define echo_download_attachment with file:`
- Line 1256: `define echo_image_supported with file:`
- Line 1259: `define echo_release_image:`
- Line 1269: `define echo_open_attachment with file:`
- Line 1277: `define echo_preview_audio with file:`
- Line 1338: `define echo_preview_image with file:`
- Line 1399: `define echo_read_source with file:`
- Line 1452: `define echo_open_source with file:`
- Line 1534: `define echo_selected_files:`
- Line 1545: `define echo_file_choice with file:`
- Line 1568: `define echo_attachments:`
- Line 1627: `define echo_ensure_session:`
- Line 1646: `define echo_upload_file with event:`
- Line 1652: `define echo_upload_one with file:`
- Line 1676: `define echo_upload_prepared with file and identity:`
- Line 1695: `define echo_upload_files with originals:`
- Line 1725: `define echo_drag_files with event:`
- Line 1735: `define echo_drop_files with event:`
- Line 1740: `define echo_paste_files with event:`
- Line 1749: `define echo_attachment_inputs:`
- Line 1756: `define escapeHTML with str:`
- Line 1770: `define renderAiInlineMarkdown with text:`
- Line 1790: `define echo_table_cells with line:`
- Line 1819: `define echo_table_row with cells and alignments and heading:`
- Line 1831: `define echo_markdown_table with lines and start:`
- Line 1868: `define renderAiMarkdown with inputText:`
- Line 1991: `define echo_open_recap:`
- Line 2115: `define echo_theme_apply:`
- Line 2123: `define echo_theme_setup:`
- Line 2147: `define echo_suggestion with label and prompt and container:`
- Line 2160: `define echo_polish_layout:`
- Line 2226: `define echo_show_identity with target:`
- Line 2242: `define main:`
- Line 2431: `define echo_capability_label with value:`
- Line 2438: `define echo_provider_picker:`
- Line 2484: `define echo_provider_details with provider:`
- Line 2512: `define echo_provider_refresh with identity:`

## app/ui/layout.ep

[Source](../../app/ui/layout.ep) · 9 lines · 11972 bytes

SHA-256: `709e65dc243aa57ad1bceb873699acce1110c6053efe1add95b059d2ec960d3c`


Imports: 

- Line 1: `define echo_layout_style returning Str:`

## app/ui/math_render.ep

[Source](../../app/ui/math_render.ep) · 62 lines · 2543 bytes

SHA-256: `61179503a8eedd8a63d392b2340ef23bb69db9e5313e6f9b722560be64a3299d`

> All renderer controls remain authored in English. Original equation text stays
> visible until a worker result arrives; cancellation keeps it available.

Imports: 

- Line 3: `define echo_math_cancel_all:`
- Line 10: `define echo_math_render with element:`

## app/ui/math_worker.ep

[Source](../../app/ui/math_worker.ep) · 17 lines · 750 bytes

SHA-256: `6ad03b42d4f6eed5e73d4f803b9336c396e77cfa43ebfb7d7328c0242cf40747`

> Generated as a separate local browser worker. The caller can terminate a
> nonterminating expression without freezing chat or imposing an expansion cap.

Imports: 

- Line 3: `define main:`

## app/ui/page.ep

[Source](../../app/ui/page.ep) · 8 lines · 6813 bytes

SHA-256: `dda72e69551ef2182d366ca7565ddb9fcb367a0a5a9f74e66ae4c1ba9380e261`


Imports: theme → app/ui/theme.ep, layout → app/ui/layout.ep

- Line 4: `define echo_page returning Str:`

## app/ui/theme.ep

[Source](../../app/ui/theme.ep) · 2 lines · 1809 bytes

SHA-256: `5411c7c154cf3a5b7b6518cd418864317c3b6c47ddbeb6c79be7ec12e64a748f`


Imports: 

- Line 1: `define echo_theme_style returning Str:`

## app/verify_export.ep

[Source](../../app/verify_export.ep) · 17 lines · 752 bytes

SHA-256: `02ed659edac37e33084006164e17c2c0ea1c3a8cabfa0b6246d5bb09d65cf6a6`


Imports: archive/export_verify → app/archive/export_verify.ep, fs → standard library or compiler dependency

- Line 4: `define main returning Int:`

## app/web/activity.ep

[Source](../../app/web/activity.ep) · 95 lines · 6337 bytes

SHA-256: `36561fa801c2544035f40a96cef638fc9125ce9ce4287ec130bb57c49563bbf9`

> Read-only status derived from committed work belonging to this run and its
> current head. Do not infer progress percentages, expose model draft recaps,
> or attribute another conversation's activity to the requesting device.
> A machine-readable observation of the current head only. Pending means saved
> work exists, not that a worker is alive; callers must also check worker status.

Imports: ../retained_text → app/retained_text.ep, run_store → app/web/run_store.ep

- Line 7: `define web_run_activity with database as Int and run as Str returning Str:`
- Line 84: `define web_run_compaction with database as Int and run as Str returning Str:`

## app/web/attachments.ep

[Source](../../app/web/attachments.ep) · 79 lines · 6030 bytes

SHA-256: `24aa7767fb89107de66ad2bbd626381c8f205d228cbe24d33a9da4a7f612bc96`

> Host-only storage primitive. Path must be a closed, exclusively owned staging
> file; browser names are metadata, never filesystem paths. Publication and
> ownership recheck commit with the original attachment event.
> Resolve only through ownership joins; never expose host paths over HTTP.

Imports: ../retained_text → app/retained_text.ep, run_store → app/web/run_store.ep, ../archive/file_digest → app/archive/file_digest.ep

- Line 5: `define web_attachment_schema with database as Int returning Int:`
- Line 11: `define web_attachment_register with database as Int and device as Str and session as Str and attachment as Str and path as Str and name as Str and media_type as Str returning Int:`
- Line 47: `define web_attachment_owned with database as Int and device as Str and attachment as Str returning List:`
- Line 53: `define web_attachment_list with database as Int and device as Str and session as Str returning Str:`

## app/web/branches.ep

[Source](../../app/web/branches.ep) · 112 lines · 8852 bytes

SHA-256: `494c457baa022cc0f253c6130299b15cae8ef1d5c8f699a1448b2355fb2dd438`

> Branch creation never copies, rewrites or deletes original events. The saved
> endpoint and digest identify the immutable source prefix for later reads.
> Only accepted completed answers are boundaries in this operation; edit and
> regenerate use their own boundary selection before launching a new run.
> Creation and submission are separately durable, using the same retry identity.
> A disconnect between them leaves a recoverable branch, never a duplicate turn.

Imports: ../retained_text → app/retained_text.ep, run_store → app/web/run_store.ep

- Line 8: `define web_branch_at with database as Int and device as Str and source as Str and endpoint as Str and identity as Str and mode as Str and replacement as Str returning List:`
- Line 11: `define web_branch_at_provider with database as Int and device as Str and source as Str and endpoint as Str and identity as Str and mode as Str and replacement as Str and provider_identity as Str returning List:`
- Line 70: `define web_branch_session with database as Int and device as Str and source as Str and endpoint as Str and identity as Str returning List:`
- Line 75: `define web_revision with database as Int and device as Str and source as Str and user_event as Str and identity as Str and mode as Str and text as Str returning List:`
- Line 78: `define web_revision_provider with database as Int and device as Str and source as Str and user_event as Str and identity as Str and mode as Str and text as Str and provider_identity as Str returning List:`

## app/web/cancellation.ep

[Source](../../app/web/cancellation.ep) · 53 lines · 3055 bytes

SHA-256: `82c5053c7fae0b9c19a4273682f1316546352bfde70c76abcb520d0b65a83272`

> A durable request is distinct from completion. Only the owning active device
> may request it; the worker records the actual terminal transition.
> The monitor owns the cancellation writer. Closing it wakes a blocked native
> provider read/write/connect. A separate stop pipe ends the monitor when normal
> work finishes. Database failure also wakes the worker rather than leaving an
> uncancellable inference request running indefinitely.
> Responsiveness cadence, not an inference deadline or token cap.
> Use the existing channel wait so this idle monitor cooperates
> with allocation on the active inference worker.

Imports: ../retained_text → app/retained_text.ep, http_api → app/web/http_api.ep, select → standard library or compiler dependency

- Line 5: `define web_cancel_schema with database as Int returning Int:`
- Line 10: `define web_cancel_request with database as Int and device as Str and run as Str returning Int:`
- Line 20: `define web_cancel_monitor with database_path as Str and run as Str and stop_reader as Int and cancellation_writer as Int and completed as Int returning Int:`

## app/web/compaction.ep

[Source](../../app/web/compaction.ep) · 7 lines · 372 bytes

SHA-256: `21d65619413179966e022efbcb1461ccfb50771273800671e082f8070d104aa3`


Imports: devices → app/web/devices.ep, ../tools/compaction_tools → app/tools/compaction_tools.ep

- Line 4: `define web_compaction_read with database as Int and device as Str and session as Str and arguments as Str returning Str:`

## app/web/devices.ep

[Source](../../app/web/devices.ep) · 117 lines · 7117 bytes

SHA-256: `d39fec2a66826f4db83e9da7f912000fc031bbde5ddac688df9c7415cbd513be`

> Credentials are random bearer secrets supplied by the host pairing flow.
> Only their digests are retained. Invitations are consumed in the same
> transaction that creates the device, so concurrent redemption cannot share it.
> An empty principal always means unauthenticated, including database failure.
> Call inside the same transaction that creates the session. This never
> silently adopts legacy records; only an explicit host migration may do so.
> Return only the authenticated browser's identity. The caller cannot select a
> different device or request a role; the host-local CLI owns role changes.

Imports: ../retained_text → app/retained_text.ep, ../security/principals → app/security/principals.ep, ../agent/runner → app/agent/runner.ep

- Line 8: `define web_device_schema with database as Int returning Int:`
- Line 11: `define web_device_write with database as Int and query as Str and values as List returning Int:`
- Line 17: `define web_pairing_invite with database as Int and invitation as Str returning Int:`
- Line 24: `define web_device_pair with database as Int and invitation as Str and device as Str and name as Str and credential as Str returning Int:`
- Line 51: `define web_device_authenticate with database as Int and credential as Str returning Str:`
- Line 64: `define web_device_revoke with database as Int and device as Str returning Int:`
- Line 71: `define web_device_own_pending with database as Int and device as Str and session as Str returning Int:`
- Line 77: `define web_device_session_allowed with database as Int and device as Str and session as Str returning Int:`
- Line 88: `define web_device_run_allowed with database as Int and device as Str and run as Str returning Int:`
- Line 101: `define web_device_identity with database as Int and device as Str returning Str:`

## app/web/export.ep

[Source](../../app/web/export.ep) · 107 lines · 6134 bytes

SHA-256: `01246ce203ea406a254a35c4f179f3148252b2f779214e689b1e07c927eba7d4`

> Export one verified read snapshot. Payload strings retain their original bytes
> and hashes; credentials and other devices' sessions are never included.
> A branch export carries every required ancestor prefix as its own verified
> hash chain. Descending endpoints put each child immediately before its parent.

Imports: ../retained_text → app/retained_text.ep, ../archive/export_verify → app/archive/export_verify.ep, run_store → app/web/run_store.ep

- Line 7: `define web_session_export_prefix with database as Int and device as Str and session as Str and boundary as Int returning Str:`
- Line 45: `define web_session_export with database as Int and device as Str and session as Str returning Str:`

## app/web/http_api.ep

[Source](../../app/web/http_api.ep) · 181 lines · 10199 bytes

SHA-256: `acf8e9afd85412131366dfa083a09274c405d5992133d4c5ed01bb2adfec944a`

> Empty means absent; duplicate matching fields are explicitly ambiguous.
> The token is generated once per server process and delivered in same-origin
> bootstrap JSON. No permissive CORS or query-string credentials are used.
> State-changing requests also require an exact origin matching their Host.
> The public origin comes only from host startup configuration. Forwarded
> headers never select it. The gateway preserves Origin and rewrites Host to
> the loopback upstream, whose listener remains inaccessible over the LAN.
> Call only after authorization. There is no fixed message-size cutoff; the
> body is consumed through the HTTP framing reader, preserving UTF-8 exactly.

Imports: ../retained_text → app/retained_text.ep, run_store → app/web/run_store.ep

- Line 5: `define web_header with headers as List and name as Str returning List:`
- Line 21: `define web_local_host with request as List and port as Int returning Int:`
- Line 33: `define web_authorized with request as List and port as Int and token as Str and mutation as Int returning Int:`
- Line 36: `define web_https_origin_valid with origin as Str returning Int:`
- Line 59: `define web_origin_allowed with request as List and port as Int and required as Int and public_origin as Str returning Int:`
- Line 77: `define web_authorized_origin with request as List and port as Int and token as Str and mutation as Int and public_origin as Str returning Int:`
- Line 88: `define web_read_json with reader as List and request as List and read_size as Int returning List:`
- Line 136: `define web_submit_json with database as Int and tokens as List returning List:`
- Line 139: `define web_submit_json_owned with database as Int and device as Str and tokens as List returning List:`
- Line 177: `define web_submit_payload with result as List returning Str:`

## app/web/message_speech.ep

[Source](../../app/web/message_speech.ep) · 50 lines · 4171 bytes

SHA-256: `c66f9dff5a24b63f6098991501bbbe35143be3e5ed4bdcdf21d8d8ee30017d39`

> Idempotent direct speech of an owned, immutable assistant message. This uses
> the existing speech executor and tool ledger without another model turn.

Imports: ../retained_text → app/retained_text.ep, ../media/speech_tool → app/media/speech_tool.ep

- Line 6: `define web_message_speak with database as Int and device as Str and source as Str returning Str:`

## app/web/progress.ep

[Source](../../app/web/progress.ep) · 84 lines · 4608 bytes

SHA-256: `ef8364d0746a914b18050ca366a2ca9e45f6f86072d28425c4f83474b6c92be4`

> This is display-only draft text. It cannot create tool calls or mark a run
> complete. The cursor addresses committed original events, and changes of
> generation reset it, so tool turns cannot concatenate unrelated drafts.
> Release the read lock before decoding JSON. Each copied record is already
> committed and immutable, so concurrent appends cannot change this snapshot.

Imports: ../retained_text → app/retained_text.ep, run_store → app/web/run_store.ep

- Line 7: `define web_run_progress with database as Int and run as Str and requested_generation as Str and after as Int returning Str:`

## app/web/recovery.ep

[Source](../../app/web/recovery.ep) · 89 lines · 5925 bytes

SHA-256: `f326a33d7477196eb313ff9f8f91895c33bd3d29861191e0fa1ef99186a626a0`

> Only lock-aware claims are eligible. Legacy claims have no proof that their
> live workers observe this lock protocol and must not be silently reclaimed.
> Restore generation only after proving the abandoned claim under its lock.
> Snapshot candidates before dispatch; each recovery rechecks ownership/state.

Imports: ../transport/isolated_process → app/transport/isolated_process.ep, ../retained_text → app/retained_text.ep, worker → app/web/worker.ep, ../agent/generation_recovery → app/agent/generation_recovery.ep, ../agent/tool_recovery → app/agent/tool_recovery.ep

- Line 10: `define web_worker_recover_ready with database as Int and run as Str and previous as Str and worker as Str and library as Int returning Int:`
- Line 32: `define web_worker_recover_owned with database as Int and run as Str and previous as Str and worker as Str returning Int:`
- Line 55: `define web_recover_ready_workers with database as Int and database_path as Str and model as Str and port as Int and reserve as Int and batch as Int returning Int:`
- Line 58: `define web_recover_ready_workers_provider with database as Int and database_path as Str and profile as Str and reserve as Int and batch as Int returning Int:`

## app/web/routes.ep

[Source](../../app/web/routes.ep) · 343 lines · 26543 bytes

SHA-256: `f02335d2efe0d25c0e672295648878f6f091393f7eda3b95d8f6be570883eef2`

> Configuration: database path, model, provider port, workload reserve,
> I/O batch size. It is immutable while workers use it.
> Optional index 8 carries the host-selected provider profile.
> This checks exact origin and unambiguous headers. Authentication below
> resolves the credential against the durable device store.

Imports: ../transport/isolated_process → app/transport/isolated_process.ep, ../retained_text → app/retained_text.ep, message_speech → app/web/message_speech.ep, trace → app/web/trace.ep, branches → app/web/branches.ep, worker → app/web/worker.ep, progress → app/web/progress.ep, export → app/web/export.ep, uploads → app/web/uploads.ep, compaction → app/web/compaction.ep, ../providers/status → app/providers/status.ep

- Line 16: `define web_config_provider with config as List returning Str:`
- Line 21: `define web_reply with status as Str and body as Str returning List:`
- Line 27: `define web_route with reader as List and request as List and port as Int and config as List returning List:`
- Line 324: `define web_receive_upload with database as Int and device as Str and reader as List and request as List and batch as Int returning List:`

## app/web/run_store.ep

[Source](../../app/web/run_store.ep) · 386 lines · 26359 bytes

SHA-256: `39218a0fd2902f3386533c12449c4020b0373792f56aac7e816e4d25202f789e`

> Submission identity is supplied once by the browser and retained on retries.
> Message, session and ready run commit together before a worker is launched.
> Different text with the same identity is a conflict, never an overwrite.
> Empty device is reserved for the existing host-only construction path.
> Network routes must resolve a credential and reject an empty principal.
> Freeze the registered provider in the same transaction as the new run. An
> empty browser selection freezes the registered default, while its original
> empty selection remains unchanged for idempotent retries after default changes.
> Small status response; originals and streamed events remain in the archive.
> Rendering consumes text as text, never provider-supplied markup.
> Message batches are a transport/rendering choice, never archive retention.
> Stable global sequence cursors also traverse frozen branch ancestry.
> Ownership is part of the query, not a browser-side filter. No fixed count
> ceiling silently hides old conversations. Latest run follows archive order.
> Literal matching avoids wildcard expansion. Search original user/final text,
> not duplicated transport payloads or another device's archive.
> Save an owned empty conversation before its first upload. The browser keeps
> the creation identity until acknowledgement so interrupted retries are safe.
> Compare the displayed title inside the write transaction so another tab cannot
> silently overwrite a rename. The original conversation events remain intact.

Imports: ../retained_text → app/retained_text.ep, devices → app/web/devices.ep, source_reads → app/web/source_reads.ep, ../archive/turn_attachments → app/archive/turn_attachments.ep, ../providers/registry → app/providers/registry.ep

- Line 7: `define web_run_schema with database as Int returning Int:`
- Line 16: `define web_submit_result with status as Str and session as Str and run as Str and created as Int returning List:`
- Line 24: `define web_submit_abort with database as Int and status as Str returning List:`
- Line 33: `define web_submit with database as Int and submission as Str and requested_session as Str and message as Str returning List:`
- Line 38: `define web_submit_owned with database as Int and device as Str and submission as Str and requested_session as Str and message as Str returning List:`
- Line 41: `define web_submit_owned_files with database as Int and device as Str and submission as Str and requested_session as Str and message as Str and attachments as List returning List:`
- Line 47: `define web_submit_owned_provider with database as Int and device as Str and submission as Str and requested_session as Str and message as Str and attachments as List and provider_identity as Str returning List:`
- Line 162: `define web_run_status with database as Int and run as Str returning Str:`
- Line 186: `define web_session_transcript with database as Int and session as Str returning Str:`
- Line 191: `define web_session_transcript_window with database as Int and session as Str and before as Int and amount as Int returning Str:`
- Line 277: `define web_device_sessions with database as Int and device as Str returning Str:`
- Line 282: `define web_device_sessions_search with database as Int and device as Str and query as Str returning Str:`
- Line 318: `define web_create_session with database as Int and device as Str and identity as Str returning List:`
- Line 356: `define web_rename_session with database as Int and device as Str and session as Str and previous as Str and title as Str returning Str:`

## app/web/source_reads.ep

[Source](../../app/web/source_reads.ep) · 76 lines · 5636 bytes

SHA-256: `215c057f90607b347d2542ad9d4cc2fbc5120d5308aefeec5108913af0e9a697`

> These are recorded reads, not an assertion that every answer claim is
> supported. Only successful observed attachment reads with matching provenance
> are exposed; model-written citation strings never create these references.

Imports: ../retained_text → app/retained_text.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 7: `define web_run_source_reads with database as Int and session as Str and run as Str returning Str:`
- Line 54: `define web_message_audio with database as Int and session as Str and run as Str and source as Str returning Str:`

## app/web/trace.ep

[Source](../../app/web/trace.ep) · 89 lines · 5773 bytes

SHA-256: `2ab297c45a38ebb189efeb8e422af590060b263d6a540f4a09f5a1a303760fed`

> The route authenticates device ownership before calling this reader. Run and
> session are joined here too; provider records must belong to this run's request.
> Paging is chosen by the caller. Original records are never changed or clipped.

Imports: ../retained_text → app/retained_text.ep, progress → app/web/progress.ep

- Line 7: `define web_run_trace with database as Int and run as Str and after as Int and events as Int returning Str:`

## app/web/upload_body.ep

[Source](../../app/web/upload_body.ep) · 37 lines · 2204 bytes

SHA-256: `bc4626ef249176c7e1fa94dadaccd9e8efe13431485a10ed6ad6934a495a2f30`

> Call only after device/session authorization. This consumes framed binary
> bytes directly; it never buffers the complete file or converts it to text.

Imports: ../retained_text → app/retained_text.ep, http_api → app/web/http_api.ep, ../archive/file_stage → app/archive/file_stage.ep

- Line 7: `define web_upload_stage with reader as List and request as List and path as Str and batch as Int returning List:`

## app/web/uploads.ep

[Source](../../app/web/uploads.ep) · 69 lines · 5540 bytes

SHA-256: `103582c7b9195fa299404c77bbf00331a9ee52c3cb4b30a2c84cc4ffe71c7a3c`

> Root is host configuration. The opaque storage name is always host-generated.
> The active body owner marks transport/storage failures; a saved receipt can
> never be downgraded. Failed uploads require a new identity and staging file.
> Only the successful claim holder may call this after closing the body writer.
> Completion is derived from the atomic attachment receipt, so a crash after
> registration does not leave an ambiguous separate completion flag.
> Startup only: caller holds the shared runtime lock before accepting clients.
> No old upload writer can still belong to this runtime. Never infer success
> from leftover bytes: only the atomic attachment receipt proves completion.

Imports: ../retained_text → app/retained_text.ep, attachments → app/web/attachments.ep, upload_body → app/web/upload_body.ep

- Line 5: `define web_upload_schema with database as Int returning Int:`
- Line 11: `define web_upload_prepare with database as Int and device as Str and session as Str and root as Str and name as Str and media_type as Str returning Str:`
- Line 32: `define web_upload_owned with database as Int and device as Str and upload as Str returning List:`
- Line 38: `define web_upload_claim with database as Int and device as Str and upload as Str returning Int:`
- Line 46: `define web_upload_fail with database as Int and device as Str and upload as Str returning Int:`
- Line 55: `define web_upload_register with database as Int and device as Str and upload as Str returning Int:`
- Line 68: `define web_upload_recover with database as Int returning Int:`

## app/web/worker.ep

[Source](../../app/web/worker.ep) · 211 lines · 14343 bytes

SHA-256: `e920d9302ebf42ac389422fd1e4a24941faef5451ec10d54ae2e0051fd4bef7b`

> An explicit worker claim survives process death. It is never silently reused;
> restart recovery must establish that the previous worker is no longer alive.
> A repeated submission can arrive after durable acceptance but before launch.
> Return 0 for a new claim, 2 for existing or terminal work, and 1 on failure.
> The ownership check, absence check and claim share one database write lock.
> Retry only a worker that has durably returned, using its exact identity as a
> compare-and-switch token. Running/uncertain work is never reclaimed here.
> The outer owner lock remains held until provider work, monitor cleanup and
> terminal worker status are finished. A stale claim never enters this function.
> Existing runs keep their archived selection across host restarts.
> A returned worker has no monitor left to consume cancellation. Complete only
> a ready run with no outstanding tool observations, under the same write lock
> that excludes retry claims. Active or uncertain work keeps its durable request.
> A failed OS launch must leave a visible, retryable durable claim.

Imports: ../transport/isolated_process → app/transport/isolated_process.ep, ../retained_text → app/retained_text.ep, cancellation → app/web/cancellation.ep, activity → app/web/activity.ep

- Line 6: `define web_worker_schema with database as Int returning Int:`
- Line 11: `define web_worker_protocol with database as Int and run as Str and worker as Str returning Int:`
- Line 19: `define web_worker_claim with database as Int and run as Str and worker as Str returning Int:`
- Line 32: `define web_worker_ensure_claim with database as Int and device as Str and run as Str and worker as Str returning Int:`
- Line 53: `define web_worker_retry_claim with database as Int and device as Str and run as Str and previous as Str and worker as Str returning Int:`
- Line 76: `define web_worker_run with database_path as Str and run as Str and worker as Str and model as Str and port as Int and reserve as Int and batch as Int returning Int:`
- Line 79: `define web_worker_run_provider with database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:`
- Line 106: `define web_worker_run_owned with database_path as Str and run as Str and worker as Str and model as Str and port as Int and reserve as Int and batch as Int returning Int:`
- Line 109: `define web_worker_run_owned_provider with database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:`
- Line 159: `define web_worker_status with database as Int and run as Str returning Str:`
- Line 185: `define web_worker_cancel_idle with database as Int and run as Str returning Int:`
- Line 204: `define web_worker_dispatch with database as Int and database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:`

## app/workspace/capture_process.ep

[Source](../../app/workspace/capture_process.ep) · 92 lines · 4380 bytes

SHA-256: `be4838f9944aa37adb31bf73f593be53b5b210c9278ce0d2fb73bfd8346abe05`

> Takes ownership of the two output readers, not the cancellation reader or
> process handle. Caller retains the handle until it is reaped and closed.
> Result: status, raw wait status, persisted stdout bytes, persisted stderr bytes.
> Persistence failure terminates the group immediately and never claims complete
> output. Cancellation terminates the group then archives bytes still in pipes.

Imports: ../retained_text → app/retained_text.ep, native_process → app/workspace/native_process.ep, process_output → app/workspace/process_output.ep, output_archive → app/workspace/output_archive.ep, ../agent/control → app/agent/control.ep

- Line 12: `define workspace_process_capture with database as Int and run as Str and call_id as Str and handle as List and output as Int and errors as Int and cancellation as Int returning List:`

## app/workspace/create_text.ep

[Source](../../app/workspace/create_text.ep) · 61 lines · 3181 bytes

SHA-256: `a79dc2792e2628f6c5c68bce48e4eb778613955fe27ff5cdc6039dbeec8b5c52`

> Publish a fully flushed new file with Darwin RENAME_EXCL. Parent directories
> must exist. Existing destinations are never replaced. Temporary files may
> survive process failure and need recovery; they are not completion receipts.
> Do not replace this with ordinary rename: workspace subprocesses can
> create the destination concurrently and do not honor host reservations.

Imports: ../retained_text → app/retained_text.ep, directories → app/workspace/directories.ep

- Line 4: `external define ep_uuid_v4`
- Line 9: `define workspace_create_text with library as Int and root as Int and path as Str and text as Str returning Str:`

## app/workspace/directories.ep

[Source](../../app/workspace/directories.ep) · 43 lines · 2111 bytes

SHA-256: `590e61c87a2befd7d9b6d25a8e9d652cd9e7e20b66e7d33f228975d61de33fc8`

> Open a relative directory beneath the session descriptor. When requested,
> create missing components with owner-only permissions and flush each parent.
> Existing components must also be directories and must never be symlinks.
> A failure may leave earlier empty directories; a retry can reopen them.

Imports: ../retained_text → app/retained_text.ep, files → app/workspace/files.ep

- Line 8: `define workspace_directory_open with library as Int and root as Int and path as Str and create_missing as Int returning Int:`

## app/workspace/execute_process.ep

[Source](../../app/workspace/execute_process.ep) · 113 lines · 6745 bytes

SHA-256: `9c70313e045876edb7c2b95b0ef7bfbe277a9b5d463635c5c352f099f58eccc2`

> Result: status, retained handle (only when cleanup needs attention), raw wait
> status, persisted stdout bytes, persisted stderr bytes. The caller must retain
> and reconcile a nonzero handle; never convert cleanup-failed into success.
> Authenticated dispatcher supplies the owned/pinned directory and host policy.
> One claimed tool call runs one explicit argv command. No implicit shell,
> model-controlled environment, execution deadline or output-length limit.

Imports: ../retained_text → app/retained_text.ep, sandbox_process → app/workspace/sandbox_process.ep, capture_process → app/workspace/capture_process.ep, process_journal → app/workspace/process_journal.ep

- Line 9: `define workspace_execution_result with status as Str and handle as List and waited as Int and output_bytes as Int and error_bytes as Int returning List:`
- Line 18: `define workspace_process_abort with handle as List returning Int:`
- Line 34: `define workspace_execute_claimed with database as Int and run as Str and call_id as Str and runtime as Str and workspace as Str and directory as Int and executable as Str and arguments as List and environment as List and cancellation as Int returning List:`

## app/workspace/execution_request.ep

[Source](../../app/workspace/execution_request.ep) · 65 lines · 4851 bytes

SHA-256: `938c6ea7043abab735c2badb5e8ceda8f2a1014abc60023cf3352bfbd80b6b39`

> Resolve an execution request from its original immutable tool event. No caller
> or model arguments can override session, device, workspace root or environment.
> Result: status, session, device, program, argv list. This grants no launch on
> its own: the dispatcher must pin the workspace and recheck owner at dispatch.

Imports: ../security/principals → app/security/principals.ep, ../retained_text → app/retained_text.ep, sessions → app/workspace/sessions.ep, ../security/tool_policy → app/security/tool_policy.ep, ../agent/run_state → app/agent/run_state.ep

- Line 7: `define workspace_execution_denied with status as Str returning List:`
- Line 16: `define workspace_execution_request with database as Int and run as Str and call_id as Str returning List:`

## app/workspace/files.ep

[Source](../../app/workspace/files.ep) · 84 lines · 3786 bytes

SHA-256: `1436f748a62caf298795ed56f4dc030694a4ab4abd67fc842c6cd94adf024c22`

> The host supplies the workspace root. Model arguments supply only a relative
> path below an already opened directory descriptor. Darwin constants below
> are ABI flags, not file-size or workspace-depth limits.
> O_RDONLY | O_DIRECTORY | O_NOFOLLOW_ANY | O_CLOEXEC.
> Regular single-link files only. A FIFO is opened nonblocking and rejected;
> directories, devices and hard links do not become file tool inputs.
> Darwin arm64 struct stat: 144 bytes; mode at4 and link count at6.
> create_new=1 means exclusive new regular file, never truncate or overwrite.
> Intermediate directories must already exist; a separate directory operation
> will own their creation and durable publication.
> O_RDONLY | O_DIRECTORY | O_NOFOLLOW | O_CLOEXEC.

Imports: ../retained_text → app/retained_text.ep, ../archive/file_stage → app/archive/file_stage.ep, ../transport/native_open → app/transport/native_open.ep, ../archive/file_text → app/archive/file_text.ep

- Line 9: `define workspace_root_open with library as Int and path as Str returning Int:`
- Line 16: `define workspace_path_parts with path as Str returning List:`
- Line 30: `define workspace_regular_file with library as Int and descriptor as Int returning Int:`
- Line 52: `define workspace_file_open with library as Int and root as Int and path as Str and create_new as Int returning Int:`

## app/workspace/native_process.ep

[Source](../../app/workspace/native_process.ep) · 134 lines · 7195 bytes

SHA-256: `5196ee60d40d682543f47074e36e36020a67918f816b16557b7a3ad32b332df5`

> Host-only Darwin process primitive. Callers supply explicit argv/environment,
> already opened output descriptors, and a working-directory descriptor.
> This primitive does not grant a model permission to execute a program.
> The command tool must launch through the separately qualified sandbox.
> Stdout/stderr go directly to owned descriptors, without a string buffer cap.
> Handle fields: library, child PID, reaped flag, raw wait status.
> CLOEXEC_DEFAULT closes every unmentioned descriptor. A new process group
> allows cancellation of ordinary descendants, not just their parent.
> 0 running, 1 reaped, -1 error. Owner calls until reaped, then closes handle.
> Caller chooses TERM or KILL; no hidden execution deadline. Do not signal a
> reaped PID: its numeric identity may already have been reused by the kernel.

Imports: ../retained_text → app/retained_text.ep, ../transport/socket_read → app/transport/socket_read.ep

- Line 10: `define workspace_process_start with executable as Str and arguments as List and environment as List and directory as Int and output as Int and errors as Int returning List:`
- Line 96: `define workspace_process_poll with handle as List returning Int:`
- Line 120: `define workspace_process_signal with handle as List and signal as Int returning Int:`
- Line 129: `define workspace_process_close with handle as List returning Int:`

## app/workspace/output_archive.ep

[Source](../../app/workspace/output_archive.ep) · 60 lines · 4231 bytes

SHA-256: `9071c2107d022e0c2fc7f3602e69c004529cc1e8ea74509162a81a24fc40916e`

> Each pipe chunk is an immutable event, with independently ordered stream
> offsets. Binary bytes use hex so NUL and invalid UTF-8 cannot alter the record.
> This is a host capture API, not a model-callable tool. Caller owns the pointer
> and must stop the process if recording fails, rather than silently drop output.
> Guard native integer overflow, not an output-size quota.

Imports: ../retained_text → app/retained_text.ep, ../agent/tool_ledger → app/agent/tool_ledger.ep, ../tools/history_tools → app/tools/history_tools.ep, ../providers/archived_stream → app/providers/archived_stream.ep

- Line 6: `external define peek_byte with pointer and offset returning Int`
- Line 7: `external define poke_byte with pointer and offset and value returning Int`
- Line 13: `define workspace_output_record with database as Int and run as Str and call_id as Str and event as Str and stream as Str and pointer as Int and bytes as Int and ended as Int returning Int:`

## app/workspace/process_journal.ep

[Source](../../app/workspace/process_journal.ep) · 97 lines · 8615 bytes

SHA-256: `cf869f8241b54bd507463bc579e7c2971024cdfaf704e018e086d19852449cc7`

> One launch reservation per durable tool call. Never infer that a reserved or
> started process did not execute after restart; such work needs reconciliation.
> Stored PIDs are observations, not authority to signal a possibly reused PID.
> Only a live in-memory handle from this worker can supply start/exit receipts.
> A finish receipt requires waitpid evidence, not an output EOF or saved PID.
> Invoke only when this worker's native start returned no handle. A reservation
> found after a crash is insufficient evidence for this transition.

Imports: ../retained_text → app/retained_text.ep, output_archive → app/workspace/output_archive.ep, native_process → app/workspace/native_process.ep, ../agent/control → app/agent/control.ep

- Line 9: `define workspace_process_schema with database as Int returning Int:`
- Line 12: `define workspace_process_reserve with database as Int and run as Str and call_id as Str and description as Str returning Int:`
- Line 34: `define workspace_process_receipt with database as Int and run as Str and call_id as Str and handle as List and capture_status as Str returning Int:`
- Line 83: `define workspace_process_spawn_failed with database as Int and run as Str and call_id as Str returning Int:`

## app/workspace/process_output.ep

[Source](../../app/workspace/process_output.ep) · 76 lines · 4577 bytes

SHA-256: `f8c5151ef8ff24be03fbb687adaa446cf2807426424584ec069b079733beab74`

> Result fields: status, stream (1 stdout, 2 stderr), owned bytes, byte count.
> Caller releases data with socket_release_buffer. EOF descriptors must be
> closed and replaced with -1 before the next call. A page is a transfer size,
> never a limit on output length. Binary bytes, including NUL, remain intact.
> Caller alternates preferred after data to avoid starving either pipe.

Imports: ../transport/socket_read → app/transport/socket_read.ep

- Line 8: `define workspace_output_result with status as Str and stream as Int and pointer as Int and count as Int returning List:`
- Line 16: `define workspace_output_next with library as Int and output as Int and errors as Int and cancellation as Int and preferred as Int returning List:`

## app/workspace/read_text.ep

[Source](../../app/workspace/read_text.ep) · 14 lines · 864 bytes

SHA-256: `7be2e45d3981b13fe41bab74816bf4f1e253f2e9321c9232b85c56e07d78d9d6`

> The caller supplies an authorized session directory, never a host path from
> tool arguments. Read through the validated descriptor without reopening.
> The digest describes the bytes observed in this read, not an atomic snapshot
> against concurrent writes by another process.

Imports: files → app/workspace/files.ep

- Line 7: `define workspace_read_text with library as Int and root as Int and path as Str and batch as Int and offset as Int and characters as Int returning List:`

## app/workspace/sandbox_process.ep

[Source](../../app/workspace/sandbox_process.ep) · 24 lines · 1603 bytes

SHA-256: `4e58ef4b70d21a65a82e6e79d107cafed7f43a018d22bd2d9e593c3ec25c4bb2`

> Host-only launcher. The owning tool must establish device/session ownership,
> pin the workspace descriptor and supply trusted runtime/workspace roots.
> Arguments are separate native argv entries, never assembled into shell text.
> Pass the generated policy directly: a program cannot rewrite a profile file
> in its writable workspace and thereby alter a subsequent launch's policy.

Imports: native_process → app/workspace/native_process.ep, sandbox_profile → app/workspace/sandbox_profile.ep

- Line 9: `define workspace_sandbox_start with runtime as Str and workspace as Str and executable as Str and command_arguments as List and environment as List and directory as Int and output as Int and errors as Int returning List:`

## app/workspace/sandbox_profile.ep

[Source](../../app/workspace/sandbox_profile.ep) · 5 lines · 1160 bytes

SHA-256: `68220ff34926fc70d996fec0b36629398bb1e90e3b7f0a4f40db8fc7cc2b2b0f`

> Native macOS sandbox configuration, generated from English source. The host
> passes roots as sandbox-exec parameters, never interpolated profile syntax.
> This initial profile is a qualification candidate, not a proven code sandbox.

Imports: 

- Line 4: `define workspace_sandbox_profile returning Str:`

## app/workspace/sessions.ep

[Source](../../app/workspace/sessions.ep) · 70 lines · 4627 bytes

SHA-256: `ee072bc3681bf132522619fa6c4825b953090fdab51c765eb166a5de3a791399`

> Bind an owned session to one host-configured root. A changed root is an
> explicit migration, not an implicit new empty workspace. Reservation survives
> interruption before mkdir and keeps independent databases in separate names.
> device and session are supplied by the authenticated host/run, never accepted
> as filesystem-tool arguments. Root is host configuration, never model input.

Imports: ../retained_text → app/retained_text.ep, files → app/workspace/files.ep, ../security/principals → app/security/principals.ep, ../archive/runtime_lock → app/archive/runtime_lock.ep, ../tools/history_tools → app/tools/history_tools.ep

- Line 10: `define workspace_session_schema with database as Int returning Int:`
- Line 13: `define workspace_session_allowed with database as Int and device as Str and session as Str returning Int:`
- Line 24: `define workspace_session_open with database as Int and library as Int and device as Str and session as Str and root_path as Str returning Int:`
