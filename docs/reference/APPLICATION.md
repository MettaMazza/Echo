# Application source reference — Echo 1.75

Observed 2026-10-19T21:07:14.397795+00:00. Every listed file was read in full for this lexical inventory. A test file is not a test pass. Line links identify this source snapshot; hashes are authoritative.

[Release overview](../releases/1.75/README.md) · [Machine-readable manifest](../forensics/source-manifest.json)

## `app/agent/checked_generation.ep`

[app/agent/checked_generation.ep](../../app/agent/checked_generation.ep) · 45 lines · 3220 bytes

SHA-256: `e5458d70c2110e60cc113b5f14bbbb1b11d506a114599654258b7c063df45888`

Source annotations: Context composition supplies the exact body and workload output reserve. The ready event captured before discovery must still be current afterwards. A non-ready decision leaves the run ready for compaction/configuration work. Existing callers retain their launch configuration while using the same provider contract. New callers pass an authorized profile to the entry above.

Imports: [../retained_text](../../app/retained_text.ep), [generation](../../app/agent/generation.ep), [../providers/prepare](../../app/providers/prepare.ep).

Definitions:

```text
8: define agent_generate_provider_at with database as Int and run as Str and expected_parent as Str and request as Str and body as Str and profile as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int and choice_index as Str returning List:
31: define agent_generate_checked_at with database as Int and run as Str and expected_parent as Str and request as Str and body as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int and choice_index as Str returning List:
39: define agent_generate_checked with database as Int and run as Str and request as Str and body as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int and choice_index as Str returning List:
```

Lexical test references: [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep).

## `app/agent/control.ep`

[app/agent/control.ep](../../app/agent/control.ep) · 70 lines · 3931 bytes

SHA-256: `4a13b201bad6c2b3e17598a0fb0c46059ad738f2310167f407be170ee813275d`

Source annotations: Shared cancellation check for agent work and compaction between requests. Cancellation drains only work still queued under the writer lock. A running or uncertain call keeps its real outcome unresolved until its adapter checks effects; cancellation never fabricates a successful rollback.

Imports: [../retained_text](../../app/retained_text.ep), [checked_generation](../../app/agent/checked_generation.ep).

Definitions:

```text
4: external define ep_uuid_v4
6: define agent_identity returning Str:
10: define agent_cancellation_pending with library as Int and reader as Int returning Int:
31: define agent_cancel_queued_pending with database as Int and run as Str and session as Str and parent as Str returning Int:
50: define agent_cancel_run with database as Int and run as Str returning Int:
```

Lexical test references: [tests/context/seed_recap_viewer.ep](../../tests/context/seed_recap_viewer.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/web/cancel_fixture_run.ep](../../tests/web/cancel_fixture_run.ep), [tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/prepare_guardian_devices.ep](../../tests/web/prepare_guardian_devices.ep), [tests/web/provision_crash_probe.ep](../../tests/web/provision_crash_probe.ep), [tests/web/test_activity.ep](../../tests/web/test_activity.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_run_store.ep](../../tests/web/test_run_store.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep).

## `app/agent/generation.ep`

[app/agent/generation.ep](../../app/agent/generation.ep) · 202 lines · 13589 bytes

SHA-256: `16add3f0dd1dd0a3edc16b355a45ed4938e288ee200998718cd47f4f9a9e2353`

Source annotations: Keep the small identity envelope in a named live value while quoting a large multimodal body. Do not leave it as an intermediate nested-call result. Persist originals before making the selected response actionable. Selection is explicit; other choices remain in the archived provider events.

Imports: [../interaction/watch_analysis](../../app/interaction/watch_analysis.ep), [guidance_interrupt](../../app/agent/guidance_interrupt.ep), [../interaction/autonomy](../../app/interaction/autonomy.ep), [../feedback](../../app/feedback.ep), [../retained_text](../../app/retained_text.ep), [run_state](../../app/agent/run_state.ep), [../providers/local_chat](../../app/providers/local_chat.ep).

Definitions:

```text
9: define agent_event_envelope with run as Str and parent as Str returning Str:
14: define agent_request_envelope with run as Str and parent as Str and body as Str returning Str:
21: define agent_generation_fail with database as Int and run as Str and session as Str and request as Str and reason as Str returning Int:
31: define agent_generation_publish with database as Int and run as Str and session as Str and request as Str and state as List and choice_index as Str returning Int:
140: define agent_generate_local_at with database as Int and run as Str and expected_parent as Str and request as Str and body as Str and port as Int and path as Str and cancellation_reader as Int and read_size as Int and choice_index as Str returning Int:
170: define agent_generate_local with database as Int and run as Str and request as Str and body as Str and port as Int and path as Str and cancellation_reader as Int and read_size as Int and choice_index as Str returning Int:
177: define agent_guidance_recover with database as Int and run as Str and session as Str and request as Str returning Int:
184: define agent_guidance_apply with database as Int and run as Str returning Int:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_generation.ep](../../tests/agent/test_generation.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_large_request_envelope.ep](../../tests/agent/test_large_request_envelope.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep).

## `app/agent/generation_recovery.ep`

[app/agent/generation_recovery.ep](../../app/agent/generation_recovery.ep) · 41 lines · 2717 bytes

SHA-256: `adf406c507e3945ce7ef3dd0bbb155ff2d04450ae212ef514be97e523b7f7025`

Source annotations: The caller must own the run's native worker lock. No provider request or tool action is replayed here. Complete streams publish atomically; partial streams remain archived and the run receives a new ready head for a fresh attempt.

Imports: [../retained_text](../../app/retained_text.ep), [generation](../../app/agent/generation.ep), [../providers/restored_stream](../../app/providers/restored_stream.ep).

Definitions:

```text
8: define agent_recover_generation_owned with database as Int and run as Str and recovery_event as Str and choice_index as Str returning Str:
```

Lexical test references: [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep).

## `app/agent/guidance_interrupt.ep`

[app/agent/guidance_interrupt.ep](../../app/agent/guidance_interrupt.ep) · 72 lines · 4053 bytes

SHA-256: `68269e80753a010edec39c7b7fa27fca3e4055bb9dd39a5fb7fa40264be51dcf`

Source annotations: A generation-local wake pipe interrupts even a blocked HTTP header read. Guidance retires only that generation; the run-level cancellation pipe is untouched. The monitor owns its own connection and never writes authority.

Imports: [../providers/local_chat](../../app/providers/local_chat.ep).

Definitions:

```text
6: define guidance_interrupt_monitor with database_path as Str and run as Str and cancellation as Int and stop_reader as Int and writer as Int and completed as Int returning Int:
41: define agent_generate_interruptible with database as Int and run as Str and session as Str and request as Str and port as Int and path as Str and body as Str and cancellation as Int and read_size as Int returning List:
```

## `app/agent/process_recovery.ep`

[app/agent/process_recovery.ep](../../app/agent/process_recovery.ep) · 42 lines · 4988 bytes

SHA-256: `af4c50fea9d884837d761d8d026c0e6ea2638bec122f2bd932c73dc1b1ac33d2`

Source annotations: The caller owns the abandoned worker lock and a writer transaction, and has verified the session archive. Only an original waitpid-backed finish receipt can settle a process call. A saved PID or missing process is never enough.

Imports: [run_state](../../app/agent/run_state.ep).

Definitions:

```text
6: define agent_recover_process_pending with database as Int and run as Str and call_id as Str returning Str:
```

## `app/agent/provider_profile.ep`

[app/agent/provider_profile.ep](../../app/agent/provider_profile.ep) · 58 lines · 3212 bytes

SHA-256: `81fd545556c0b703631bbccfdf55c8fefdf7a64a1c8b2d8d350dcd0c658c5cbc`

Source annotations: The host binds a profile before the first model request. An existing binding is immutable: explicit new selection belongs to a new run, never a retry. Return 0 exact binding, 2 conflicting binding, 1 invalid state/storage error.

Imports: [../retained_text](../../app/retained_text.ep), [run_state](../../app/agent/run_state.ep), [../providers/prepare](../../app/providers/prepare.ep).

Definitions:

```text
8: define agent_provider_bind with database as Int and run as Str and profile as Str returning Int:
49: define agent_provider_read with database as Int and run as Str returning Str:
```

Lexical test references: [tests/agent/test_provider_profile.ep](../../tests/agent/test_provider_profile.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_live_automatic_memory.ep](../../tests/security/test_live_automatic_memory.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep).

## `app/agent/run_messages.ep`

[app/agent/run_messages.ep](../../app/agent/run_messages.ep) · 274 lines · 21549 bytes

SHA-256: `42531b889b74dd90a81c83fe071e4d31c905c8935ebd749413ba185f8ce00607`

Source annotations: Recover a JSON subtree without normalizing numeric spelling or object order. Follow accepted parent links, excluding uncommitted/stale response branches. Sequences must strictly decrease, so corrupt cycles cannot spin indefinitely.

Imports: [../security/source_boundaries](../../app/security/source_boundaries.ep), [../retained_text](../../app/retained_text.ep), [tool_dispatch](../../app/agent/tool_dispatch.ep), [user_content](../../app/agent/user_content.ep), [../context/clock](../../app/context/clock.ep).

Definitions:

```text
8: define agent_json_source with text as Str and tokens as List and index as Int returning Str:
18: define agent_run_lineage with database as Int and run as Str returning List:
56: define agent_run_messages_scoped with database as Int and run as Str and boundary as Int and fresh_reads as Int returning Str:
59: define agent_run_messages_scoped_system with database as Int and run as Str and boundary as Int and fresh_reads as Int and system_prompt as Str returning Str:
253: define agent_run_messages with database as Int and run as Str returning Str:
256: define agent_run_messages_after with database as Int and run as Str and boundary as Int returning Str:
261: define agent_observation_without_duplicate_system with observation as Str and system_prompt as Str returning Str:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_system_observation_dedup.ep](../../tests/context/test_system_observation_dedup.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep).

## `app/agent/run_state.ep`

[app/agent/run_state.ep](../../app/agent/run_state.ep) · 88 lines · 5994 bytes

SHA-256: `75b422cbc0c4652481c459de90be34ac5e6a85b4d7128f0159984ff66d90d5c2`

Source annotations: The event kind records why the state changes. The dispatcher validates the actual model/tool outcome before writing that event; storage never invents it.

Imports: [tool_ledger](../../app/agent/tool_ledger.ep).

Definitions:

```text
3: define agent_run_schema with database as Int returning Int:
8: define agent_run_create with database as Int and run as Str and session as Str and objective_event as Str returning Int:
19: define agent_transition_kind with previous as Str and next_state as Str returning Str:
47: define agent_run_transition with database as Int and run as Str and previous as Str and expected_event as Str and next_state as Str and evidence_event as Str returning Int:
65: define agent_run_read with database as Int and run as Str returning List:
86: define agent_run_claim_tool with database as Int and run as Str and call_id as Str returning Int:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_generation.ep](../../tests/agent/test_generation.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_provider_profile.ep](../../tests/agent/test_provider_profile.ep), [tests/agent/test_run_state.ep](../../tests/agent/test_run_state.ep), [tests/agent/test_runner.ep](../../tests/agent/test_runner.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/context/seed_recap_viewer.ep](../../tests/context/seed_recap_viewer.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep), [tests/context/test_live_recap_semantics.ep](../../tests/context/test_live_recap_semantics.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_jobs.ep](../../tests/context/test_recap_jobs.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep), [tests/context/test_recap_upgrade.ep](../../tests/context/test_recap_upgrade.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/integration/test_component_imports.ep](../../tests/integration/test_component_imports.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/project_controls_file.ep](../../tests/web/project_controls_file.ep), [tests/web/seed_rendering.ep](../../tests/web/seed_rendering.ep), [tests/web/test_activity.ep](../../tests/web/test_activity.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep).

## `app/agent/runner.ep`

[app/agent/runner.ep](../../app/agent/runner.ep) · 178 lines · 11221 bytes

SHA-256: `1f8e2c734e4905296b0b6eb65468b644841609556e1bf184f0770bc5561b529a`

Imports: `select` (toolchain/library), [../interaction/questions](../../app/interaction/questions.ep), [../jobs/delegation_return](../../app/jobs/delegation_return.ep), [../retained_text](../../app/retained_text.ep), [control](../../app/agent/control.ep), [provider_profile](../../app/agent/provider_profile.ep), [../context/compact](../../app/context/compact.ep), [../memory/automatic](../../app/memory/automatic.ep).

Definitions:

```text
10: define agent_loop_inner_provider with database as Int and run as Str and profile as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
93: define agent_loop_provider with database as Int and run as Str and profile as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
145: define agent_loop_inner with database as Int and run as Str and model as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
149: define agent_loop with database as Int and run as Str and model as Str and port as Int and output_reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
153: define agent_memory_prepare_parallel with path as Str and run as Str and profile as Str and reserve as Int and cancellation as Int and read_size as Int and completed as Int returning Int:
167: define agent_attempt_record with database as Int and run as Str and status as Str returning Int:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_runner.ep](../../tests/agent/test_runner.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/test_live_automatic_memory.ep](../../tests/security/test_live_automatic_memory.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep).

## `app/agent/tool_dispatch.ep`

[app/agent/tool_dispatch.ep](../../app/agent/tool_dispatch.ep) · 229 lines · 20783 bytes

SHA-256: `d2e6981660533f5c6f1d6b68b4d83615257dc1f5f9e4b5f6752534e9ef8b3c47`

Imports: [../media/video_library](../../app/media/video_library.ep), [../interaction/watch_party](../../app/interaction/watch_party.ep), [../interaction/tools](../../app/interaction/tools.ep), [../hud/interface](../../app/hud/interface.ep), [../tools/trace_tools](../../app/tools/trace_tools.ep), [../media/video_tool](../../app/media/video_tool.ep), [../feedback](../../app/feedback.ep), [../memory/corrections](../../app/memory/corrections.ep), [../jobs/task_checklist](../../app/jobs/task_checklist.ep), [../tools/procedure_tool](../../app/tools/procedure_tool.ep), [../tools/project_tool](../../app/tools/project_tool.ep), [../tools/web_fetch](../../app/tools/web_fetch.ep), [../tools/candidate_discovery](../../app/tools/candidate_discovery.ep), [../tools/schedule_tool](../../app/tools/schedule_tool.ep), [../tools/delegation_tool](../../app/tools/delegation_tool.ep), [../tools/job_control](../../app/tools/job_control.ep), [../tools/tool_discovery](../../app/tools/tool_discovery.ep), [../tools/continuity_tools](../../app/tools/continuity_tools.ep), [../tools/scratchpad](../../app/tools/scratchpad.ep), [../retained_text](../../app/retained_text.ep), [generation](../../app/agent/generation.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../tools/host_text](../../app/tools/host_text.ep), [../tools/memory_tools](../../app/tools/memory_tools.ep), [../tools/knowledge_tools](../../app/tools/knowledge_tools.ep), [../tools/protected_tools](../../app/tools/protected_tools.ep), [../tools/attachment_tools](../../app/tools/attachment_tools.ep), [../tools/execution_tool](../../app/tools/execution_tool.ep), [../media/image_tool](../../app/media/image_tool.ep), [../media/speech_tool](../../app/media/speech_tool.ep), [../context/recap_rebuild](../../app/context/recap_rebuild.ep).

Definitions:

```text
37: define agent_direct_lookup with database as Int and run as Str and name as Str and arguments as Str returning Int:
53: define agent_tool_step_cancellable with database as Int and run as Str and cancellation as Int returning Int:
228: define agent_tool_step with database as Int and run as Str returning Int:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep).

## `app/agent/tool_ledger.ep`

[app/agent/tool_ledger.ep](../../app/agent/tool_ledger.ep) · 80 lines · 6430 bytes

SHA-256: `57c47f00d2ebbf50257b473a81b34ce4eb2b651e4911f90d4b1b4eb9cedacad1`

Source annotations: The runner persists original tool-call and result events before referring to them here. State changes are atomic, and only one worker can claim a call.

Imports: [../context/checkpoint](../../app/context/checkpoint.ep).

Definitions:

```text
5: define tool_ledger_schema with database as Int returning Int:
8: define tool_ledger_change with database as Int and query as Str and values as List returning Int:
17: define tool_ledger_identity with run as Str and call_id as Str returning List:
23: define tool_ledger_queue with database as Int and run as Str and call_id as Str and session as Str and request_event as Str returning Int:
33: define tool_ledger_claim with database as Int and run as Str and call_id as Str returning Int:
39: define tool_ledger_interrupt with database as Int and run as Str and call_id as Str returning Int:
45: define tool_ledger_observe with database as Int and run as Str and call_id as Str and result_event as Str returning Int:
55: define tool_ledger_confirm_not_executed with database as Int and run as Str and call_id as Str and evidence_event as Str returning Int:
63: define tool_ledger_read with database as Int and run as Str and call_id as Str returning List:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_run_state.ep](../../tests/agent/test_run_state.ep), [tests/agent/test_tool_ledger.ep](../../tests/agent/test_tool_ledger.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/fixtures/delegation_fixture.ep](../../tests/fixtures/delegation_fixture.ep), [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/fixtures/schedule_fixture.ep](../../tests/fixtures/schedule_fixture.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep).

## `app/agent/tool_recovery.ep`

[app/agent/tool_recovery.ep](../../app/agent/tool_recovery.ep) · 95 lines · 5641 bytes

SHA-256: `e02684ea2f7dcfc53a6818b781a6abc1693d470e510dfa1ce72024e16f563f5c`

Source annotations: Caller owns the native run lock and has verified the abandoned worker claim. Recover only original saved results. Missing outcomes never authorize replay.

Imports: [../feedback](../../app/feedback.ep), [../context/compaction_lock](../../app/context/compaction_lock.ep), [../workspace/guardian_receipt](../../app/workspace/guardian_receipt.ep), [../retained_text](../../app/retained_text.ep), [run_state](../../app/agent/run_state.ep), [process_recovery](../../app/agent/process_recovery.ep).

Definitions:

```text
10: define agent_recover_tools_owned with database as Int and run as Str returning Int:
23: define agent_recover_tools_locked with database as Int and run as Str returning Int:
78: define agent_recover_guardian_pending with database as Int and run as Str and call_id as Str returning Str:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep).

## `app/agent/user_content.ep`

[app/agent/user_content.ep](../../app/agent/user_content.ep) · 49 lines · 3713 bytes

SHA-256: `dbd7f93fd03b97891f365c7f6095107290ef5fe6378c35741cc4539ed3785d19`

Source annotations: Rebuild each user's selected sources from the durable original receipt. A missing or changed image fails the request instead of silently dropping it.

Imports: [../security/group_speakers](../../app/security/group_speakers.ep), [../retained_text](../../app/retained_text.ep), [../archive/turn_attachments](../../app/archive/turn_attachments.ep), [../providers/attachment_image](../../app/providers/attachment_image.ep).

Definitions:

```text
8: define agent_user_content with database as Int and session as Str and event as Str and text as Str returning Str:
```

Lexical test references: [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep).

## `app/archive/attachment_lifecycle.ep`

[app/archive/attachment_lifecycle.ep](../../app/archive/attachment_lifecycle.ep) · 11 lines · 1005 bytes

SHA-256: `0e2e5c184387668c787fbe68adcc16dc44c0b7c94f016b20ab9c7b5fd7803968`

Source annotations: Trash changes new-selection eligibility, never original historical bytes. Scope/authority is checked by the authenticated writer before appending.

Imports: [attachment_scope](../../app/archive/attachment_scope.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
6: define attachment_lifecycle_state with database as Int and attachment as Str returning List:
9: define attachment_selectable with database as Int and attachment as Str returning Int:
```

Lexical test references: [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep).

## `app/archive/attachment_restore.ep`

[app/archive/attachment_restore.ep](../../app/archive/attachment_restore.ep) · 95 lines · 6530 bytes

SHA-256: `eb918a8cb15048a23766f907d8654fa2d00953ebfe874e440c483575b212c391`

Source annotations: Offline candidate only. Originals and hashes are unchanged; only host storage locations are rebound inside a transaction, with immutability restored before commit. Uploads without completed attachments cannot resume old paths.

Imports: [../retained_text](../../app/retained_text.ep), [file_publication](../../app/archive/file_publication.ep), [attachment_snapshot](../../app/archive/attachment_snapshot.ep), [../web/uploads](../../app/web/uploads.ep).

Definitions:

```text
9: define attachment_restore_relocate with database as Int and backup_root as Str and destination_root as Str returning Int:
48: define attachment_restore_start_allowed with database as Int returning Int:
60: define attachment_restore_reserved with library as Int and source as Str and backup_root as Str and destination as Str and destination_root as Str returning Int:
87: define attachment_restore with source as Str and backup_root as Str and destination as Str and destination_root as Str returning Int:
```

Lexical test references: [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/archive/test_restore_publication.ep](../../tests/archive/test_restore_publication.ep), [tests/archive/test_restore_release_hold.ep](../../tests/archive/test_restore_release_hold.ep), [tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep).

## `app/archive/attachment_scope.ep`

[app/archive/attachment_scope.ep](../../app/archive/attachment_scope.ep) · 12 lines · 867 bytes

SHA-256: `ab7403866ebe6d8c9f539848b46c6da93e297832a83b6132295f20bce111f864`

Source annotations: Internal readers receive the run's trusted session. Shared sources must be in that exact segment; private branch ancestry is never imported into it. HTTP callers separately authenticate the requesting audience member.

Imports: [../security/group_history](../../app/security/group_history.ep).

Definitions:

```text
6: define attachment_shared_session with database as Int and session as Str returning Int:
```

## `app/archive/attachment_snapshot.ep`

[app/archive/attachment_snapshot.ep](../../app/archive/attachment_snapshot.ep) · 78 lines · 5206 bytes

SHA-256: `7c222af7bb34d3ef6c6f0580de2ded4479b671f4ba66e096204ec5b21cd4f2ff`

Source annotations: Caller chooses a new private snapshot and existing host-controlled directory. Paths use a digest of the opaque ID, never an uploaded filename or raw ID.

Imports: [../retained_text](../../app/retained_text.ep), [snapshot](../../app/archive/snapshot.ep), [file_copy](../../app/archive/file_copy.ep), [history](../../app/archive/history.ep), [../context/checkpoint](../../app/context/checkpoint.ep).

Definitions:

```text
9: define attachment_snapshot_path with root as Str and attachment as Str returning Str:
12: define attachment_snapshot_copy with database as Int and root as Str returning Int:
55: define attachment_snapshot_verify with database as Int and root as Str returning Int:
```

Lexical test references: [tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep), [tests/archive/test_restore_publication.ep](../../tests/archive/test_restore_publication.ep), [tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep).

## `app/archive/attachment_visibility.ep`

[app/archive/attachment_visibility.ep](../../app/archive/attachment_visibility.ep) · 96 lines · 7667 bytes

SHA-256: `38def6b3416b1789c2e97a13f758ce8bd1215d7988227c69d9e44f711d53c2a2`

Source annotations: Original bytes remain immutable. This record controls future context use. Absence is included; read errors are not silently treated as inclusion. Follow exact video tool output ancestry. UNION makes malformed cycles finite; no text similarity or guessed relationship can make a source disappear. Produce a request-only view of an immutable attachment manifest. Never write this filtered view back to the canonical user event or original binding.

Imports: [attachment_scope](../../app/archive/attachment_scope.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
6: define attachment_visibility_state with database as Int and attachment as Str returning List:
11: define attachment_lineage_sql with database as Int and identity as Str returning Str:
17: define attachment_included_sql with database as Int and identity as Str returning Str:
20: define attachment_context_included with database as Int and attachment as Str returning Int:
26: define attachment_context_manifest with database as Int and manifest as Str returning Str:
58: define attachment_call_context_included with database as Int and name as Str and arguments as Str returning Int:
71: define attachment_observation_visible_sql with database as Int and source as Str returning Str:
82: define attachment_observation_context_included with database as Int and event as Str returning Int:
88: define attachment_fresh_call_included with database as Int and name as Str and arguments as Str and request as Str returning Int:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep).

## `app/archive/backup_bundle.ep`

[app/archive/backup_bundle.ep](../../app/archive/backup_bundle.ep) · 161 lines · 9832 bytes

SHA-256: `dfb80b5b50de7071e9c8fdc1bb54fcf430df50347b3f1594877c7c0a0e8d5898`

Source annotations: Host-only profile. Each tree and individual file is explicit; weights are not silently duplicated. Runtime ownership prevents Echo from restarting while its database and files are copied. Other host editors must be stopped.

Imports: [attachment_restore](../../app/archive/attachment_restore.ep), [tree_snapshot](../../app/archive/tree_snapshot.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep), [../lifecycle/service_profile](../../app/lifecycle/service_profile.ep).

Definitions:

```text
9: define backup_bundle_identity with path as Str returning Str:
15: define backup_bundle_absolute with value as Str returning Int:
20: define backup_bundle_valid with tokens as List returning Int:
55: define backup_bundle_copy with library as Int and database as Int and tokens as List and profile as Str returning Int:
136: define backup_bundle_run with profile as Str returning Int:
```

Lexical test references: [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep).

## `app/archive/event_store.ep`

[app/archive/event_store.ep](../../app/archive/event_store.ep) · 226 lines · 12673 bytes

SHA-256: `b56f91ee45694baff717c8426b9681c48e947656246713d1812a7f305c9fa6ff`

Source annotations: Echo owns its original history independently of prompts, summaries and caches. SQLite constants below are protocol result codes, not application limits. Report the failed operation without exposing archived message contents. These wrappers preserve database errors; an empty query is not an error signal.

Imports: [../retained_text](../../app/retained_text.ep), `sql` (toolchain/library), `string` (toolchain/library).

Environment names: `ECHO_ARCHIVE_JOURNAL`.

Definitions:

```text
7: external define sqlite3_extended_errcode with database as Int returning Int
10: define archive_failure_detail with database as Int and operation as Str returning Int:
17: define archive_prepare with database as Int and query as Str returning Int:
23: define archive_bind with statement as Int and position as Int and value as Str returning Int:
29: define archive_finish with statement as Int and expected as Int returning Int:
39: define archive_open with path as Str returning Int:
66: define archive_hash_field with value as Str returning Str:
73: define archive_digest with session as Str and event as Str and kind as Str and payload as Str and occurred as Str and previous as Str returning Str:
88: define archive_rollback with database as Int returning Int:
96: define archive_append_pending with database as Int and session as Str and event as Str and kind as Str and payload as Str and occurred as Str returning Int:
155: define archive_append with database as Int and session as Str and event as Str and kind as Str and payload as Str and occurred as Str returning Int:
169: define archive_read_event with database as Int and event as Str returning List:
197: define archive_verify with database as Int and session as Str returning Int:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_generation.ep](../../tests/agent/test_generation.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_provider_profile.ep](../../tests/agent/test_provider_profile.ep), [tests/agent/test_run_state.ep](../../tests/agent/test_run_state.ep), [tests/agent/test_runner.ep](../../tests/agent/test_runner.ep), [tests/agent/test_tool_ledger.ep](../../tests/agent/test_tool_ledger.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/archive/measure_event_commit.ep](../../tests/archive/measure_event_commit.ep), [tests/archive/test_append_readback.ep](../../tests/archive/test_append_readback.ep), [tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep), [tests/archive/test_branch_export_file.ep](../../tests/archive/test_branch_export_file.ep), [tests/archive/test_digest_stress.ep](../../tests/archive/test_digest_stress.ep), [tests/archive/test_event_store.ep](../../tests/archive/test_event_store.ep), [tests/archive/test_export_restore.ep](../../tests/archive/test_export_restore.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/archive/test_history.ep](../../tests/archive/test_history.ep), [tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep), [tests/archive/test_restore_release_hold.ep](../../tests/archive/test_restore_release_hold.ep), [tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep), [tests/archive/test_snapshot.ep](../../tests/archive/test_snapshot.ep), [tests/context/seed_recap_viewer.ep](../../tests/context/seed_recap_viewer.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_checkpoint.ep](../../tests/context/test_checkpoint.ep), [tests/context/test_checkpoint_interruption.ep](../../tests/context/test_checkpoint_interruption.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_compaction_lock.ep](../../tests/context/test_compaction_lock.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep), [tests/context/test_live_recap_semantics.ep](../../tests/context/test_live_recap_semantics.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_memory_provenance_snapshot.ep](../../tests/context/test_memory_provenance_snapshot.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_contract.ep](../../tests/context/test_recap_contract.ep), [tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_jobs.ep](../../tests/context/test_recap_jobs.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep), [tests/context/test_recap_upgrade.ep](../../tests/context/test_recap_upgrade.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/context/test_token_count_retry.ep](../../tests/context/test_token_count_retry.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/fixtures/delegation_fixture.ep](../../tests/fixtures/delegation_fixture.ep), [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/fixtures/schedule_fixture.ep](../../tests/fixtures/schedule_fixture.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_initialize.ep](../../tests/lifecycle/test_controller_initialize.ep), [tests/lifecycle/test_controller_ledger_privileged.ep](../../tests/lifecycle/test_controller_ledger_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/lifecycle/test_migration_interruption.ep](../../tests/lifecycle/test_migration_interruption.ep), [tests/lifecycle/test_migrations.ep](../../tests/lifecycle/test_migrations.ep), [tests/lifecycle/test_operations.ep](../../tests/lifecycle/test_operations.ep), [tests/lifecycle/test_startup.ep](../../tests/lifecycle/test_startup.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_existing.ep](../../tests/memory/test_procedure_existing.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_runtime_binding_pressure.ep](../../tests/memory/test_runtime_binding_pressure.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_history_zero_boundary.ep](../../tests/providers/test_history_zero_boundary.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_context_admission.ep](../../tests/providers/test_live_context_admission.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep), [tests/providers/test_mlx_preflight.ep](../../tests/providers/test_mlx_preflight.ep), [tests/providers/test_modality_preflight.ep](../../tests/providers/test_modality_preflight.ep), [tests/providers/test_registry.ep](../../tests/providers/test_registry.ep), [tests/security/diagnose_execution_authority.ep](../../tests/security/diagnose_execution_authority.ep), [tests/security/prepare_http_principals.ep](../../tests/security/prepare_http_principals.ep), [tests/security/record_source_correction.ep](../../tests/security/record_source_correction.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_authority_query_repetition.ep](../../tests/security/test_authority_query_repetition.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep), [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_linked_recall_existing.ep](../../tests/security/test_linked_recall_existing.ep), [tests/security/test_live_automatic_memory.ep](../../tests/security/test_live_automatic_memory.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep), [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_observer_evidence.ep](../../tests/security/test_observer_evidence.ep), [tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep), [tests/security/test_observer_input.ep](../../tests/security/test_observer_input.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep), [tests/security/test_principals.ep](../../tests/security/test_principals.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_reasoning_existing.ep](../../tests/security/test_reasoning_existing.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_selected_recall_existing.ep](../../tests/security/test_selected_recall_existing.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/cancel_fixture_run.ep](../../tests/web/cancel_fixture_run.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/prepare_guardian_devices.ep](../../tests/web/prepare_guardian_devices.ep), [tests/web/project_controls_file.ep](../../tests/web/project_controls_file.ep), [tests/web/provision_crash_probe.ep](../../tests/web/provision_crash_probe.ep), [tests/web/seed_device_routes.ep](../../tests/web/seed_device_routes.ep), [tests/web/seed_rendering.ep](../../tests/web/seed_rendering.ep), [tests/web/seed_retry_browser.ep](../../tests/web/seed_retry_browser.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/share_controls_seed.ep](../../tests/web/share_controls_seed.ep), [tests/web/test_activity.ep](../../tests/web/test_activity.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep), [tests/web/test_device_links.ep](../../tests/web/test_device_links.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_empty_session.ep](../../tests/web/test_empty_session.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_run_store.ep](../../tests/web/test_run_store.ep), [tests/web/test_session_rename.ep](../../tests/web/test_session_rename.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_trace.ep](../../tests/web/test_trace.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/web/test_uploads.ep](../../tests/web/test_uploads.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/archive/export_restore.ep`

[app/archive/export_restore.ep](../../app/archive/export_restore.ep) · 80 lines · 4930 bytes

SHA-256: `8d0b06cd6094513a0b3a88e23e796c91c5306ecae716c07328dd92446da10032`

Source annotations: Restore originals only into an empty archive. Application state and device authority are deliberately not inferred from arbitrary imported event text.

Imports: [export_verify](../../app/archive/export_verify.ep).

Definitions:

```text
5: define archive_export_restore with database as Int and text as Str and trusted_head as Str returning Int:
40: define archive_branch_export_restore with database as Int and text as Str returning Int:
```

Lexical test references: [tests/archive/test_branch_export_file.ep](../../tests/archive/test_branch_export_file.ep), [tests/archive/test_export_restore.ep](../../tests/archive/test_export_restore.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep).

## `app/archive/export_verify.ep`

[app/archive/export_verify.ep](../../app/archive/export_verify.ep) · 127 lines · 8027 bytes

SHA-256: `cd534ba5bb3ad1f28cb087a917640f90ac3018e55b17c6d3dfd8e1c8fb30eb11`

Source annotations: Hash chains detect corruption, not authorship. A separately trusted head can also reject a consistently rewritten or shortened export.

Imports: [event_store](../../app/archive/event_store.ep), [../providers/json_access](../../app/providers/json_access.ep).

Definitions:

```text
6: define archive_export_verify_single with text as Str and trusted_head as Str returning Int:
42: define archive_export_part with text as Str and tokens as List and index as Int returning Str:
50: define archive_export_verify with text as Str and trusted_head as Str returning Int:
```

Lexical test references: [tests/archive/test_branch_export_file.ep](../../tests/archive/test_branch_export_file.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep).

## `app/archive/file_base64.ep`

[app/archive/file_base64.ep](../../app/archive/file_base64.ep) · 134 lines · 7090 bytes

SHA-256: `cd65b4d79f9cb56f38dfc8db74479d415089848dab47025d9d0db650126edddf`

Source annotations: Encode and hash the same binary descriptor stream; preserve NUL/high bytes. The inline provider request requires retaining the encoded payload in memory. Batch bounds are CommonCrypto ABI limits, never an attachment-size ceiling.

Imports: [file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
3: external define peek_byte with pointer as Int and offset as Int returning Int
4: external define poke_byte with pointer as Int and offset as Int and value as Int returning Int
6: define file_base64_group with output as Int and offset as Int and value as Int and bytes as Int returning Int:
23: define file_base64_verified with path as Str and expected_digest as Str and expected_bytes as Int and batch as Int returning List:
```

Lexical test references: [tests/archive/test_file_base64.ep](../../tests/archive/test_file_base64.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/providers/test_quote.ep](../../tests/providers/test_quote.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep).

## `app/archive/file_copy.ep`

[app/archive/file_copy.ep](../../app/archive/file_copy.ep) · 59 lines · 2703 bytes

SHA-256: `a8a50f1075e02a08ed89687ae8d8e5951705704a4f798730df565f7649f4c250`

Source annotations: Host-only exclusive copy. Success requires flushed destination bytes matching the archive's recorded digest and length. Failure leaves an unpublished candidate for inspection, never overwrites an existing backup file.

Imports: [../retained_text](../../app/retained_text.ep), [file_stage](../../app/archive/file_stage.ep).

Definitions:

```text
7: define file_copy_verified with source as Str and destination as Str and digest as Str and bytes as Int and batch as Int returning Int:
```

Lexical test references: [tests/archive/test_file_copy.ep](../../tests/archive/test_file_copy.ep), [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep).

## `app/archive/file_digest.ep`

[app/archive/file_digest.ep](../../app/archive/file_digest.ep) · 140 lines · 7071 bytes

SHA-256: `b5e3b56795641d587fcfe15705979e9b9424870bf70758fcdade42664b08d837`

Source annotations: Binary streaming SHA-256 via the installed macOS CommonCrypto ABI. Batch size is I/O granularity, not a file-size ceiling. Never use C strings for file contents: attachments may include NUL and arbitrary byte values. Compaction requires a flushed regular original. Hash the same descriptor after fsync; nonblocking/no-symlink opening rejects special-file substitutions.

Imports: [../transport/socket_read](../../app/transport/socket_read.ep), `string` (toolchain/library).

Definitions:

```text
4: define file_digest_result with status as Str and digest as Str and bytes as Int returning List:
14: define file_digest with path as Str and batch as Int returning List:
19: define file_digest_durable with path as Str and batch as Int returning List:
22: define file_digest_mode with path as Str and batch as Int and durable as Int returning List:
45: define file_digest_owned_descriptor with library as Int and descriptor as Int and batch as Int and durable as Int returning List:
```

Lexical test references: [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/archive/test_file_copy.ep](../../tests/archive/test_file_copy.ep), [tests/archive/test_file_digest.ep](../../tests/archive/test_file_digest.ep), [tests/archive/test_file_stage.ep](../../tests/archive/test_file_stage.ep), [tests/archive/test_file_text.ep](../../tests/archive/test_file_text.ep), [tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep), [tests/archive/test_pdf_text.ep](../../tests/archive/test_pdf_text.ep), [tests/archive/test_restore_publication.ep](../../tests/archive/test_restore_publication.ep), [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep), [tests/security/test_protected_files.ep](../../tests/security/test_protected_files.ep), [tests/security/test_protected_staging.ep](../../tests/security/test_protected_staging.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep), [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep).

## `app/archive/file_publication.ep`

[app/archive/file_publication.ep](../../app/archive/file_publication.ep) · 61 lines · 3000 bytes

SHA-256: `92d740779430d4f615e9155f417a96335c092a369bf2bffe0b9aed9dbb13a34d`

Source annotations: Host-controlled paths only. Permanent reservations serialize cooperating Echo publishers on APFS and ExFAT. Failed attempts retain evidence for inspection. This does not exclude unrelated processes with write access to these paths. ExFAT does not support RENAME_EXCL. An exclusive mkdir reservation serializes cooperating Echo writers without requiring hard links.

Imports: [file_stage](../../app/archive/file_stage.ep).

Definitions:

```text
6: define file_publication_reserve with system as Int and path as Str returning Int:
22: define file_publication_finish with system as Int and temporary as Str and path as Str returning Int:
```

## `app/archive/file_stage.ep`

[app/archive/file_stage.ep](../../app/archive/file_stage.ep) · 44 lines · 2040 bytes

SHA-256: `1e237d9025c3d294c4703c4c350da9be3044ea1fdb6959feb7feecceb6332a34`

Source annotations: Staging paths are generated by the host, never derived from upload names. Failed candidates stay unregistered; callers may inspect/remove them later. Darwin O_RDWR|O_CREAT|O_EXCL|O_NOFOLLOW|O_CLOEXEC, mode 0600.

Imports: [../transport/native_open](../../app/transport/native_open.ep), [file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
6: define file_stage_open with library as Int and path as Str returning Int:
13: define file_stage_write with library as Int and descriptor as Int and pointer as Int and bytes as Int returning Int:
33: define file_stage_finish with library as Int and descriptor as Int returning Int:
```

Lexical test references: [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_file_stage.ep](../../tests/archive/test_file_stage.ep), [tests/archive/test_file_text.ep](../../tests/archive/test_file_text.ep), [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/web/test_uploads.ep](../../tests/web/test_uploads.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_directories.ep](../../tests/workspace/test_directories.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_files.ep](../../tests/workspace/test_files.ep), [tests/workspace/test_process.ep](../../tests/workspace/test_process.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_read_text.ep](../../tests/workspace/test_read_text.ep), [tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/archive/file_text.ep`

[app/archive/file_text.ep](../../app/archive/file_text.ep) · 174 lines · 9322 bytes

SHA-256: `1fd46d4bec132d519203da895327ae69cadc366cb5edf75dd891ba33cdff00a9`

Source annotations: A characters value of -1 is the internal complete-text mode. Otherwise only the requested Unicode passage is retained. Hashing and UTF-8 validation cover the same complete descriptor stream, including bytes outside the range. This scanner borrows a freshly opened descriptor at offset zero. It does not close or reopen it. Empty expected digest or byte count -1 requests a new snapshot: status read, with observed SHA256 and byte count at indexes3 and4. Verified means both supplied expectations matched the complete byte stream.

Imports: [file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
3: external define peek_byte with pointer as Int and offset as Int returning Int
8: define file_text_result with status as Str and text as Str and characters as Int returning List:
19: define file_text_descriptor_window with library as Int and descriptor as Int and expected_digest as Str and expected_bytes as Int and batch as Int and offset as Int and characters as Int returning List:
152: define file_text_window with path as Str and expected_digest as Str and expected_bytes as Int and batch as Int and offset as Int and characters as Int returning List:
173: define file_text_read with path as Str and expected_digest as Str and expected_bytes as Int and batch as Int returning List:
```

Lexical test references: [tests/archive/probe_text_window.ep](../../tests/archive/probe_text_window.ep), [tests/archive/test_file_text.ep](../../tests/archive/test_file_text.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_read_text.ep](../../tests/workspace/test_read_text.ep).

## `app/archive/history.ep`

[app/archive/history.ep](../../app/archive/history.ep) · 83 lines · 4932 bytes

SHA-256: `f310e713d81133b53d57cf48b94c2785a6619cfd7faefdd7e3692905e20e6695`

Source annotations: Every result explicitly distinguishes exhaustion from a database failure. Returned historical content is data with its original role, not instructions.

Imports: [session_scope](../../app/archive/session_scope.ep), [event_store](../../app/archive/event_store.ep).

Definitions:

```text
6: define history_status with status as Str returning List:
11: define history_collect_one with statement as Int returning List:
34: define history_boundary with database as Int and session as Str returning List:
47: define history_next with database as Int and session as Str and after_sequence as Int and through_sequence as Int and literal as Str returning List:
67: define history_read_piece with database as Int and session as Str and event as Str and offset as Int and characters as Int returning List:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/archive/test_history.ep](../../tests/archive/test_history.ep), [tests/archive/test_restore_release_hold.ep](../../tests/archive/test_restore_release_hold.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_initialize.ep](../../tests/lifecycle/test_controller_initialize.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_migration_interruption.ep](../../tests/lifecycle/test_migration_interruption.ep), [tests/lifecycle/test_migrations.ep](../../tests/lifecycle/test_migrations.ep), [tests/lifecycle/test_startup.ep](../../tests/lifecycle/test_startup.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/seed_retry_browser.ep](../../tests/web/seed_retry_browser.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_empty_session.ep](../../tests/web/test_empty_session.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_run_store.ep](../../tests/web/test_run_store.ep), [tests/web/test_session_rename.ep](../../tests/web/test_session_rename.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/web/test_uploads.ep](../../tests/web/test_uploads.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep).

## `app/archive/pdf_cache.ep`

[app/archive/pdf_cache.ep](../../app/archive/pdf_cache.ep) · 84 lines · 6322 bytes

SHA-256: `efdeeec977e712ff94e8fe523003552e742b8683fe5b7ca508bffe77fb3c6459`

Source annotations: Cache versions include the OS build because PDFKit extraction can change between system releases. The algorithm prefix changes when this adapter does.

Imports: [../retained_text](../../app/retained_text.ep), [pdf_text](../../app/archive/pdf_text.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
7: define pdf_extractor_version returning Str:
27: define pdf_cache_schema with database as Int returning Int:
30: define pdf_cache_result with status as Str and text as Str and pages as Int and version as Str and cached as Int returning List:
41: define pdf_cached_page with database as Int and event as Str and path as Str and digest as Str and bytes as Int and page as Int returning List:
```

Lexical test references: [tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep).

## `app/archive/pdf_text.ep`

[app/archive/pdf_text.ep](../../app/archive/pdf_text.ep) · 82 lines · 5528 bytes

SHA-256: `26bc9d9cbcc71f21d22e82b8ab3603545e68eaa2e54c0a41cc6b975988fa4a8b`

Source annotations: Decode the verified original bytes into immutable NSData before PDFKit sees them. This avoids reopening a mutable path between hashing and extraction. The first implementation holds the encoded and decoded file in memory. Page minus one is the internal whole-document compatibility path.

Imports: [../retained_text](../../app/retained_text.ep), [file_base64](../../app/archive/file_base64.ep).

Definitions:

```text
7: define pdf_text_verified with path as Str and digest as Str and bytes as Int returning List:
11: define pdf_text_page_verified with path as Str and digest as Str and bytes as Int and page as Int returning List:
```

Lexical test references: [tests/archive/test_pdf_text.ep](../../tests/archive/test_pdf_text.ep).

## `app/archive/restore_bundle.ep`

[app/archive/restore_bundle.ep](../../app/archive/restore_bundle.ep) · 137 lines · 8852 bytes

SHA-256: `1f2e13297b668c4cc01b9e0405326de85b20c7f99860d2e5121318f55de95b59`

Source annotations: A retained top-level manifest hash is the recovery trust input. Paths from the original profile are documentary only; recovery reads exclusively from the chosen bundle and writes beneath a new host-chosen destination.

Imports: [backup_bundle](../../app/archive/backup_bundle.ep).

Definitions:

```text
6: define restore_bundle_content_valid with tokens as List and object as Int and path as Str returning Int:
19: define restore_bundle_components_valid with tokens as List and bundle as Str returning Int:
64: define restore_bundle_copy with library as Int and tokens as List and bundle as Str and destination as Str and expected as Str returning Int:
119: define restore_bundle_run with bundle as Str and expected as Str and destination as Str returning Int:
```

Lexical test references: [tests/archive/test_bundle_rejection.ep](../../tests/archive/test_bundle_rejection.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep).

## `app/archive/restore_release.ep`

[app/archive/restore_release.ep](../../app/archive/restore_release.ep) · 53 lines · 4112 bytes

SHA-256: `6e735e3d060e3b024e2d576c5b67b03c79a38ed4e2f3eb5928fd5d61acc2e596`

Source annotations: No pending historical action is automatically retried or marked completed. This release mode applies only to a snapshot whose recorded runs, tools and processes are already terminal. Uncertain snapshots stay held for review.

Imports: [attachment_restore](../../app/archive/attachment_restore.ep).

Definitions:

```text
6: define restore_release_transaction with database as Int and attachment_root as Str and reviewed_hash as Str returning Int:
29: define restore_release with path as Str and attachment_root as Str and expected_hash as Str returning Int:
```

Lexical test references: [tests/archive/test_restore_release_hold.ep](../../tests/archive/test_restore_release_hold.ep).

## `app/archive/runtime_lock.ep`

[app/archive/runtime_lock.ep](../../app/archive/runtime_lock.ep) · 58 lines · 3136 bytes

SHA-256: `f6cfbb12a1fd981315375f6a4468a35360af4ad34b444684cca04963a2d52209`

Source annotations: Copies retain this lock location. Do not delete lock files or silently change the anchor when moving a restored archive on the same host.

Imports: [../transport/native_open](../../app/transport/native_open.ep), [event_store](../../app/archive/event_store.ep), [../transport/local_socket](../../app/transport/local_socket.ep).

Definitions:

```text
7: define archive_runtime_anchor with database as Int and library as Int returning Str:
44: define archive_runtime_lock with database as Int and library as Int returning Int:
```

Lexical test references: [tests/web/test_backup.ep](../../tests/web/test_backup.ep).

## `app/archive/session_scope.ep`

[app/archive/session_scope.ep](../../app/archive/session_scope.ep) · 5 lines · 1144 bytes

SHA-256: `99555fb6801d97a0714972c1ba64a9bc9a833d52ad3be59638e685bda5c38a5c`

Source annotations: Resolve immutable branch ancestry at recorded source endpoints. Every parent endpoint predates its branch event; descendants cannot expose later messages. UNION also deduplicates repeated ancestry without imposing a depth ceiling.

Imports: none.

Definitions:

```text
4: define session_scope_sql returning Str:
```

## `app/archive/snapshot.ep`

[app/archive/snapshot.ep](../../app/archive/snapshot.ep) · 101 lines · 4762 bytes

SHA-256: `0e27b01f85355240495352944dbdf6d6b430281b7c6a78b58d826e6c4c15e53c`

Source annotations: Verify the database structure and every session without loading all events.

Imports: [event_store](../../app/archive/event_store.ep), `fs` (toolchain/library), [runtime_lock](../../app/archive/runtime_lock.ep).

Definitions:

```text
6: define archive_verify_all with database as Int returning Int:
46: define archive_snapshot with database as Int and destination as Str returning Int:
79: define archive_restore with source as Str and destination as Str returning Int:
```

Lexical test references: [tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/archive/test_restore_publication.ep](../../tests/archive/test_restore_publication.ep), [tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep), [tests/archive/test_snapshot.ep](../../tests/archive/test_snapshot.ep), [tests/lifecycle/test_migration_interruption.ep](../../tests/lifecycle/test_migration_interruption.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep).

## `app/archive/tree_sidecars.ep`

[app/archive/tree_sidecars.ep](../../app/archive/tree_sidecars.ep) · 65 lines · 4251 bytes

SHA-256: `04a034d6426c702adcc65b0d76b788a73ee3c72b8683665d1d31efac7d9835c5`

Source annotations: macOS tar merges AppleDouble entries when the corresponding file exists, even with metadata translation disabled. Preserve original sidecar bytes through the native reader, without using its disk writer or copyfile. Paths remain relative to an opened restore directory; existing files are not replaced. The complete mtree verification follows this recovery pass.

Imports: [../workspace/files](../../app/workspace/files.ep).

Definitions:

```text
8: define tree_sidecar_entries with system as Int and archive_library as Int and reader as Int and root as Int returning Int:
42: define tree_restore_sidecars with system as Int and archive_path as Str and root as Int returning Int:
```

## `app/archive/tree_snapshot.ep`

[app/archive/tree_snapshot.ep](../../app/archive/tree_snapshot.ep) · 173 lines · 11145 bytes

SHA-256: `da8244ea4ab689056c2a1b230908b45bf9a18d01f6932da8a4df73eb9b3f2e28`

Source annotations: Host-only offline tree backup. The caller must stop all writers first. macOS tar and mtree are upstream dependencies, invoked with explicit argv; all orchestration, failure handling and publication logic remains Ernos.

Imports: [tree_sidecars](../../app/archive/tree_sidecars.ep), `fs` (toolchain/library), [../workspace/native_process](../../app/workspace/native_process.ep), [../workspace/files](../../app/workspace/files.ep).

Definitions:

```text
6: define tree_snapshot_realpath with path as Str returning Str:
19: define tree_snapshot_command with library as Int and executable as Str and arguments as List and directory as Int and output_path as Str and errors_path as Str returning Int:
50: define tree_snapshot_verify_at with library as Int and directory as Int and bundle as Str and audit as Str and label as Str returning Int:
56: define tree_snapshot_verify with library as Int and directory as Int and bundle as Str and label as Str returning Int:
59: define tree_archive_extract with library as Int and bundle as Str and target as Int and audit as Str returning Int:
77: define tree_snapshot with source as Str and bundle as Str and restored as Str returning Int:
148: define tree_restore_bundle with bundle as Str and archive_hash as Str and manifest_hash as Str and restored as Str returning Int:
```

Lexical test references: [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep).

## `app/archive/turn_attachments.ep`

[app/archive/turn_attachments.ep](../../app/archive/turn_attachments.ep) · 52 lines · 4131 bytes

SHA-256: `b4c041f797bc1a1722f04cc0db8721cd315a3e5fe07bc4dc79f225f947f69462`

Source annotations: Resolve selected identities under the submission transaction. The immutable receipt records original provenance without placing host paths in context. Exact retries compare the reconstructed manifest with their saved receipt below the caller. Trash must not rewrite an accepted turn.

Imports: [attachment_scope](../../app/archive/attachment_scope.ep), [attachment_lifecycle](../../app/archive/attachment_lifecycle.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
9: define turn_attachment_manifest with database as Int and device as Str and session as Str and event as Str and attachments as List returning Str:
48: define turn_attachment_read with database as Int and session as Str and event as Str returning List:
```

Lexical test references: [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep).

## `app/backup.ep`

[app/backup.ep](../../app/backup.ep) · 24 lines · 1056 bytes

SHA-256: `163a3cab863d44550db7789309abfb311b4ae975d24430b49362c766393788f1`

Imports: [archive/snapshot](../../app/archive/snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/backup_bundle.ep`

[app/backup_bundle.ep](../../app/backup_bundle.ep) · 12 lines · 573 bytes

SHA-256: `3cb9322ba0d8f50d2b17d8d5af99e90638ac8cce5e5eb4492f53ecbefdeddafb`

Imports: [archive/backup_bundle](../../app/archive/backup_bundle.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/backup_files.ep`

[app/backup_files.ep](../../app/backup_files.ep) · 27 lines · 1363 bytes

SHA-256: `2e1273e8322fd2aa9f99e143d7d800c1bd3aef6221b7bf9d2b28541a9fc5d3db`

Imports: [archive/attachment_snapshot](../../app/archive/attachment_snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/backup_tree.ep`

[app/backup_tree.ep](../../app/backup_tree.ep) · 11 lines · 735 bytes

SHA-256: `9f432faf253c9cd179fb952d807bbd3a03bab195fff7f932921c031089c136a8`

Imports: [archive/tree_snapshot](../../app/archive/tree_snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/bootstrap_controller.ep`

[app/bootstrap_controller.ep](../../app/bootstrap_controller.ep) · 11 lines · 683 bytes

SHA-256: `13511d905bd82d686f2bb66c3070fb08621c8d221e17b14defbcad5a156e8645`

Imports: [lifecycle/controller_bootstrap](../../app/lifecycle/controller_bootstrap.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7, 8. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/capture_activation.ep`

[app/capture_activation.ep](../../app/capture_activation.ep) · 5 lines · 176 bytes

SHA-256: `e9aa2b3faf089bc6b813be7ebed26805652c597d3f40cfaa33438768e438ba9b`

Source annotations: Host-only native research entry point; not exposed to remote chat devices.

Imports: [research/capture_activation](../../app/research/capture_activation.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/compare_intervention.ep`

[app/compare_intervention.ep](../../app/compare_intervention.ep) · 4 lines · 102 bytes

SHA-256: `d8f80b413d1131ee1256012d64c9066d69377151c149b595e3be2b96b3760dc7`

Imports: [research/compare_intervention](../../app/research/compare_intervention.ep).

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/context/attachment_gate.ep`

[app/context/attachment_gate.ep](../../app/context/attachment_gate.ep) · 53 lines · 4090 bytes

SHA-256: `9531a4d7c667a1a51c920a5afaa50e29cf2c4be70b8643d87aa14463ca119fc3`

Source annotations: Frozen branch ancestry, bounded by the exact endpoint being compacted. All original attachment events in that scope must retain matching metadata and flushed original bytes. Generated images use the same attachment store. Caller supplies a stable database snapshot/transaction. No model text can choose a path. Missing storage is a recoverable failure, never source removal.

Imports: [../retained_text](../../app/retained_text.ep), [../archive/file_digest](../../app/archive/file_digest.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
8: define compaction_attachment_scope returning Str:
13: define compaction_attachments_verify with database as Int and session as Str and endpoint as Str returning Str:
```

Lexical test references: [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep).

## `app/context/attachment_revision.ep`

[app/context/attachment_revision.ep](../../app/context/attachment_revision.ep) · 28 lines · 2160 bytes

SHA-256: `65e5c35ca5d8b5dcfe162fb0ed8a137370f8267e2b58dc793c7554264e7ea751`

Source annotations: A session's branch ancestry and recorded attachment calls determine which visibility decisions invalidate its cached recap work. Unrelated silos do not. Empty means a read error; zero means no relevant visibility transitions. Legacy checkpoints are usable only before the first relevant decision.

Imports: [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [../archive/session_scope](../../app/archive/session_scope.ep).

Definitions:

```text
7: define attachment_context_revision with database as Int and session as Str returning Str:
20: define attachment_checkpoint_current with database as Int and session as Str and checkpoint as List returning Int:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep).

## `app/context/budget.ep`

[app/context/budget.ep](../../app/context/budget.ep) · 48 lines · 2280 bytes

SHA-256: `67b02fd3424c79fa3327526352a064c3153457f373f5189beac7e603a262d292`

Source annotations: All capacities and token counts arrive from the provider/tokenizer or workload. A negative result is invalid metadata, never a guessed replacement capacity. A provider output ceiling of -1 explicitly means no separate ceiling. Zero or other negative values mean unresolved/invalid provider information. Reserve each known contribution separately to avoid overflowing integer sums. Result: -1 invalid input, 0 compaction required, 1 request fits as assembled. The output reserve is chosen from the actual next workload, not a fixed default.

Imports: none.

Definitions:

```text
4: define context_remaining with capacity as Int and occupied as Int returning Int:
13: define context_output_allowance with capacity as Int and occupied as Int and provider_output_ceiling as Int returning Int:
28: define context_request_fits with capacity as Int and occupied as Int and next_input as Int and tool_overhead as Int and output_reserve as Int returning Int:
43: define context_loaded_at_maximum with requested as Int and loaded as Int returning Int:
```

Lexical test references: [tests/context/test_budget.ep](../../tests/context/test_budget.ep), [tests/integration/test_component_imports.ep](../../tests/integration/test_component_imports.ep), [tests/providers/test_llama_metadata.ep](../../tests/providers/test_llama_metadata.ep).

## `app/context/checkpoint.ep`

[app/context/checkpoint.ep](../../app/context/checkpoint.ep) · 133 lines · 8539 bytes

SHA-256: `f15d64ae80364158bb452410b9598f2d9ff7c4d615988832ae8b5e6f1e05c8bb`

Source annotations: This module persists checkpoints. The caller must generate and validate recap coverage before activation; a nonempty validation record is not that validator. Stage a frozen, verified source prefix while holding the writer lock. Recap generation may append records after the captured endpoint; they remain suffix. Repeated compaction must advance beyond the previous checkpoint's endpoint.

Imports: [../retained_text](../../app/retained_text.ep), [../archive/event_store](../../app/archive/event_store.ep).

Definitions:

```text
6: define checkpoint_schema with database as Int returning Int:
9: define checkpoint_prepare with database as Int and query as Str and values as List returning Int:
25: define checkpoint_stage with database as Int and identity as Str and session as Str and source_end as Str and previous as Str and summary as Str and structured_state as Str and validation as Str returning Int:
56: define checkpoint_activate with database as Int and session as Str and identity as Str returning Int:
67: define checkpoint_activate_pending with database as Int and session as Str and identity as Str returning Int:
86: define checkpoint_read_active with database as Int and session as Str returning List:
108: define checkpoint_capture with database as Int and session as Str returning List:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_runner.ep](../../tests/agent/test_runner.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/context/seed_recap_viewer.ep](../../tests/context/seed_recap_viewer.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_checkpoint.ep](../../tests/context/test_checkpoint.ep), [tests/context/test_checkpoint_interruption.ep](../../tests/context/test_checkpoint_interruption.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep), [tests/context/test_live_recap_semantics.ep](../../tests/context/test_live_recap_semantics.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_activity.ep](../../tests/web/test_activity.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep).

## `app/context/clock.ep`

[app/context/clock.ep](../../app/context/clock.ep) · 32 lines · 1782 bytes

SHA-256: `b1c1cf5d65b1a7e79340a6ebdfe206d211e77dfaf0e4e77e320460f9ab4f31e6`

Source annotations: Format an actual host-clock sample; no date inferred from conversation text. Darwin struct tm fits in eight native words. strftime output is bounded by this fixed format, not by any conversation or model context limit.

Imports: [../providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
6: define echo_clock_format with milliseconds as Int and utc as Int returning Str:
29: define echo_clock_observation with milliseconds as Int returning Str:
```

Lexical test references: [tests/providers/test_clock.ep](../../tests/providers/test_clock.ep).

## `app/context/compact.ep`

[app/context/compact.ep](../../app/context/compact.ep) · 133 lines · 9404 bytes

SHA-256: `445371b24d39702fca7d40be586099e60b62ec0eccef2b64a99388a1d41758de`

Source annotations: Compaction preserves the run identity and accepted tool lineage. Every stage archives its evidence; only reviewed, fitting context reaches activation. Oversized merge/review/repair inputs remain explicit recoverable results.

Imports: [../retained_text](../../app/retained_text.ep), [recap_accept](../../app/context/recap_accept.ep), [recap_collect](../../app/context/recap_collect.ep), [recap_merge](../../app/context/recap_merge.ep), [recap_repair](../../app/context/recap_repair.ep), [compaction_lock](../../app/context/compaction_lock.ep), [compaction_resume](../../app/context/compaction_resume.ep).

Definitions:

```text
9: define context_compact_provider with database as Int and run as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
27: define context_compact_owned_provider with database as Int and run as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
126: define context_compact with database as Int and run as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
131: define context_compact_owned with database as Int and run as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
```

Lexical test references: [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep).

## `app/context/compaction_lock.ep`

[app/context/compaction_lock.ep](../../app/context/compaction_lock.ep) · 38 lines · 2104 bytes

SHA-256: `95889d4a578365c661579d2522845500b44cf73322757d6eb0ae3b004b92cc09`

Source annotations: A host-local advisory lock prevents simultaneous compaction of one archive session. The kernel releases it on process death; it has no timeout or lease expiry that could grant ownership while an earlier worker is still running. Lock files must not be deleted during normal operation (inode identity matters). Darwin flags from the installed SDK: O_RDWR|O_CREAT|O_NOFOLLOW|O_CLOEXEC;

Imports: [../transport/native_open](../../app/transport/native_open.ep), [../agent/control](../../app/agent/control.ep).

Definitions:

```text
8: define compaction_lock_acquire with database as Int and session as Str and library as Int returning Int:
```

Lexical test references: [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_lock.ep](../../tests/context/test_compaction_lock.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep).

## `app/context/compaction_resume.ep`

[app/context/compaction_resume.ep](../../app/context/compaction_resume.ep) · 180 lines · 11495 bytes

SHA-256: `ead47c6680968226410ae3f83e1d12390dfac6554bbb32e886b70e2964980a0c`

Source annotations: Call only while owning the session compaction lock. A job belongs to the exact accepted run head and prior checkpoint; changed work starts a different job.

Imports: [attachment_revision](../../app/context/attachment_revision.ep), [../retained_text](../../app/retained_text.ep), [recap_jobs](../../app/context/recap_jobs.ep), [recap_review_synthesis](../../app/context/recap_review_synthesis.ep), [../agent/control](../../app/agent/control.ep).

Definitions:

```text
9: define compaction_job with database as Int and session as Str and run as Str and parent as Str and captured as List returning List:
60: define compaction_saved_review with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str returning List:
102: define compaction_saved_candidate with database as Int and session as Str and endpoint as Str and job as Str returning List:
157: define compaction_saved_synthesis with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str returning List:
```

Lexical test references: [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_upgrade.ep](../../tests/context/test_recap_upgrade.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep).

## `app/context/echo_labels.ep`

[app/context/echo_labels.ep](../../app/context/echo_labels.ep) · 7 lines · 321 bytes

SHA-256: `d503f8b7f53aec94f3c2b4901824e415e803c657f84e9f746a85d9fe517fb56c`

Source annotations: Presentation labels only: archive event kinds and provider roles stay stable.

Imports: none.

Definitions:

```text
2: define echo_record_label with kind as Str returning Str:
```

Lexical test references: [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep).

## `app/context/message_environment.ep`

[app/context/message_environment.ep](../../app/context/message_environment.ep) · 70 lines · 4378 bytes

SHA-256: `54ef8dd54ea34ba3f3899c822a55ac38dbde46b2ffbbec861cf8f3907425e70b`

Source annotations: Labels come from request hints, never from message prose. Only the HTTP transport itself is observed; a User-Agent cannot attest hardware or role.

Imports: [../providers/json_access](../../app/providers/json_access.ep), [../archive/history](../../app/archive/history.ep).

Definitions:

```text
6: define message_environment_browser with agent as Str and surface as Str returning Str:
50: define message_environment_pending with database as Int and session as Str and run as Str and event as Str and device as Str and environment as Str returning Int:
66: define message_environment_channel with platform as Str and server as Str and room as Str returning Str:
```

Lexical test references: [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep).

## `app/context/recap_accept.ep`

[app/context/recap_accept.ep](../../app/context/recap_accept.ep) · 120 lines · 9327 bytes

SHA-256: `073baff27b3f6f09ded52e5d8e6e16369824695d49d8e040f737e9aa77b18a13`

Source annotations: Consume archived candidate and review receipts, not a caller-supplied verdict. A passing model review remains fallible evidence; this gate verifies its binding and checks the exact resumed request without changing model limits.

Imports: [../retained_text](../../app/retained_text.ep), [recap_review_synthesis](../../app/context/recap_review_synthesis.ep), [session_messages](../../app/context/session_messages.ep), [attachment_gate](../../app/context/attachment_gate.ep).

Definitions:

```text
9: define recap_accept_provider with database as Int and session as Str and run as Str and candidate_event as Str and review_event as Str and identity as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
118: define recap_accept with database as Int and session as Str and run as Str and candidate_event as Str and review_event as Str and identity as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
```

Lexical test references: [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep).

## `app/context/recap_collect.ep`

[app/context/recap_collect.ep](../../app/context/recap_collect.ep) · 61 lines · 4118 bytes

SHA-256: `956836bc73f96f3549d59eec30cce5dcaaff05069540c4b8fd85601635138f72`

Source annotations: One caller owns a collection job at a time. Conflicting concurrent notes are rejected by coverage accounting, never silently merged or called complete. Collection has no iteration cap; every successful step must advance its cursor.

Imports: [../retained_text](../../app/retained_text.ep), [recap_jobs](../../app/context/recap_jobs.ep), [../agent/control](../../app/agent/control.ep).

Definitions:

```text
8: define recap_collect_inner_provider with database as Int and session as Str and job as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
41: define recap_collect_provider with database as Int and session as Str and job as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
54: define recap_collect_inner with database as Int and session as Str and job as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
59: define recap_collect with database as Int and session as Str and job as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
```

Lexical test references: [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep).

## `app/context/recap_contract.ep`

[app/context/recap_contract.ep](../../app/context/recap_contract.ep) · 92 lines · 7233 bytes

SHA-256: `bba9e22a5c99b4965c03569b662b617e4bcee5003c36840c3d0127592cd19ef6`

Source annotations: Structural/provenance validation only. Passing does not establish semantic completeness and must never by itself authorize checkpoint activation.

Imports: [../retained_text](../../app/retained_text.ep), [checkpoint](../../app/context/checkpoint.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
7: define recap_sources_valid with database as Int and session as Str and boundary as Int and tokens as List and field as Int returning Int:
29: define recap_categories returning List:
43: define recap_validate_structure with database as Int and session as Str and endpoint as Str and candidate as Str returning Int:
86: define recap_generation_instruction returning Str:
91: define recap_review_semantics returning Str:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_recap_contract.ep](../../tests/context/test_recap_contract.ep).

## `app/context/recap_findings.ep`

[app/context/recap_findings.ep](../../app/context/recap_findings.ep) · 30 lines · 1732 bytes

SHA-256: `bdcd75f87f52ee29b1816fe2d17eb410d3d71678a5ef2d6b63bc521c2c140b67`

Source annotations: Legacy string findings remain conservative repair evidence. New reports must explicitly distinguish observations from unresolved, actionable defects. No text-keyword heuristic turns criticism into an approval.

Imports: [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
6: define recap_findings_validate with tokens as List and findings as List returning Str:
```

## `app/context/recap_fragment.ep`

[app/context/recap_fragment.ep](../../app/context/recap_fragment.ep) · 121 lines · 11407 bytes

SHA-256: `e1493e065cc7172942caa393ec52f643b944c7d8558034d88496ccba5a7d2f92`

Source annotations: Compaction reads a scoped context view. Canonical archive readers stay unchanged. Withheld records use notice-relative offsets and retain the original digest only as provenance, never as a claim that the notice is the original payload.

Imports: [../retained_text](../../app/retained_text.ep), [recap_generate](../../app/context/recap_generate.ep).

Definitions:

```text
7: define recap_read_piece with database as Int and session as Str and event as Str and offset as Int and characters as Int returning List:
26: define recap_fragment_source with endpoint as Str and original as List and offset as Int and next_offset as Int returning Str:
39: define recap_fragment_body with model as Str and endpoint as Str and original as List and offset as Int and next_offset as Int returning Str:
46: define recap_fragment_review_body with model as Str and endpoint as Str and original as List and offset as Int and next_offset as Int and candidate as Str returning Str:
59: define recap_fit_fragment_provider with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
67: define recap_fit_fragment_for_provider with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and candidate as Str returning List:
114: define recap_fit_fragment with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
119: define recap_fit_fragment_for with database as Int and session as Str and endpoint as Str and event as Str and offset as Int and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and candidate as Str returning List:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep).

## `app/context/recap_fragment_review.ep`

[app/context/recap_fragment_review.ep](../../app/context/recap_fragment_review.ep) · 100 lines · 7299 bytes

SHA-256: `2ed18db75826b69eebf8884fc7a685cd4b2371d7c6c95c6537228cb18f0ddb04`

Source annotations: A fragment verdict is bound to candidate bytes and an exact original range. It cannot be substituted for whole-source or whole-candidate review.

Imports: [../retained_text](../../app/retained_text.ep), [recap_fragment](../../app/context/recap_fragment.ep), [recap_findings](../../app/context/recap_findings.ep).

Definitions:

```text
7: define recap_fragment_review_validate with candidate as Str and endpoint as Str and event as Str and digest as Str and offset as Int and next_offset as Int and report as Str returning Str:
34: define recap_fragment_review_generate_provider with database as Int and session as Str and endpoint as Str and job as Str and event as Str and offset as Int and candidate as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
98: define recap_fragment_review_generate with database as Int and session as Str and endpoint as Str and job as Str and event as Str and offset as Int and candidate as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep).

## `app/context/recap_generate.ep`

[app/context/recap_generate.ep](../../app/context/recap_generate.ep) · 95 lines · 8153 bytes

SHA-256: `462d49ababc1d4a09c5ab248b39096d4c5418927e609bccbfbb89bb130b4d399`

Source annotations: Generates a candidate only. The active context cannot change here. Oversized

Imports: [recap_prompt](../../app/context/recap_prompt.ep), [attachment_revision](../../app/context/attachment_revision.ep), [../retained_text](../../app/retained_text.ep), [recap_sources](../../app/context/recap_sources.ep), [recap_contract](../../app/context/recap_contract.ep), [../providers/prepare](../../app/providers/prepare.ep), [../providers/local_chat](../../app/providers/local_chat.ep).

Definitions:

```text
9: define recap_request_body with model as Str and sources as Str returning Str:
24: define recap_generation_result with status as Str and candidate as Str returning List:
32: define recap_generate_provider with database as Int and session as Str and endpoint as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
43: define recap_generate_sources_provider with database as Int and session as Str and endpoint as Str and sources as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
88: define recap_generate with database as Int and session as Str and endpoint as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
93: define recap_generate_sources with database as Int and session as Str and endpoint as Str and sources as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/context/measure_recap_cache.ep](../../tests/context/measure_recap_cache.ep), [tests/context/test_recap_cache_prefix.ep](../../tests/context/test_recap_cache_prefix.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep).

## `app/context/recap_jobs.ep`

[app/context/recap_jobs.ep](../../app/context/recap_jobs.ep) · 78 lines · 4818 bytes

SHA-256: `0dd10009379d0d07d07881df90cd6493d7780135ecb9f14c05f71d133599c86e`

Source annotations: Immutable manifest: later messages or run transitions do not redefine a job. Return the contiguous covered Unicode prefix, or -1 for inconsistent notes. Gaps/overlaps fail explicitly rather than treating the largest offset as proof.

Imports: [../retained_text](../../app/retained_text.ep), [recap_notes](../../app/context/recap_notes.ep).

Definitions:

```text
5: define recap_job_create with database as Int and session as Str and endpoint as Str and job as Str returning Int:
13: define recap_note_cursor with database as Int and session as Str and endpoint as Str and job as Str and source as Str and digest as Str and total as Int returning Int:
47: define recap_job_progress with database as Int and session as Str and job as Str returning List:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_jobs.ep](../../tests/context/test_recap_jobs.ep), [tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep).

## `app/context/recap_merge.ep`

[app/context/recap_merge.ep](../../app/context/recap_merge.ep) · 230 lines · 17054 bytes

SHA-256: `8963626fac113d226c1fb2cf4c8828d287e8e69b470fe34355669e79df349398`

Source annotations: Coverage is required before merge. Only notes for manifest members enter the package; stray notes bearing the same job name cannot inject extra sources. Saved notes are reusable only for the current filtered source package.

Imports: [../retained_text](../../app/retained_text.ep), [recap_jobs](../../app/context/recap_jobs.ep), [recap_merge_resume](../../app/context/recap_merge_resume.ep).

Definitions:

```text
7: define recap_merge_sources with database as Int and session as Str and job as Str returning Str:
73: define recap_merge_provider with database as Int and session as Str and job as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
103: define recap_merge with database as Int and session as Str and job as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
110: define recap_merge_progressive_provider with database as Int and session as Str and job as Str and endpoint as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep).

## `app/context/recap_merge_resume.ep`

[app/context/recap_merge_resume.ep](../../app/context/recap_merge_resume.ep) · 114 lines · 8426 bytes

SHA-256: `dc9b4b15dd13a74b5437a5493ea228814ff8f88c3e584d7320bf631fbb9de503`

Source annotations: Bind the ordered notes, including exact immutable contents, to this merge. Reconstruct every committed step, checking contiguous Unicode ranges and the actual archived model input/output. A cursor alone never proves coverage. Result: status, next note index, character offset, cumulative candidate.

Imports: [../retained_text](../../app/retained_text.ep), [recap_jobs](../../app/context/recap_jobs.ep).

Definitions:

```text
4: define recap_merge_resume_result with status as Str returning List:
13: define recap_merge_notes_digest with database as Int and notes as List returning Str:
29: define recap_merge_resume with database as Int and session as Str and job as Str and endpoint as Str and notes as List and digest as Str returning List:
110: define recap_merge_progress_save with database as Int and session as Str and job as Str and digest as Str and request as Str returning Int:
```

Lexical test references: [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep).

## `app/context/recap_notes.ep`

[app/context/recap_notes.ep](../../app/context/recap_notes.ep) · 69 lines · 5298 bytes

SHA-256: `c46420b4dba2ef590425495d1eff7abed6c8622075145998d511f99b6a88d37a`

Source annotations: Result: status, next offset, note event. Notes are immutable intermediate evidence, not active memory. Every note is tied to a completed provider stream and exact original-event character range; coverage/semantic review follow.

Imports: [../retained_text](../../app/retained_text.ep), [recap_fragment](../../app/context/recap_fragment.ep).

Definitions:

```text
7: define recap_note_result with status as Str and next_offset as Int and event as Str returning List:
14: define recap_note_generate_provider with database as Int and session as Str and endpoint as Str and job as Str and source as Str and offset as Int and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
67: define recap_note_generate with database as Int and session as Str and endpoint as Str and job as Str and source as Str and offset as Int and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep).

## `app/context/recap_prompt.ep`

[app/context/recap_prompt.ep](../../app/context/recap_prompt.ep) · 72 lines · 10907 bytes

SHA-256: `1e8878fcd517a06ee938507136d30f8ab46ee7e68bf02b85467555f1fce3c2f1`

Source annotations: The frozen source is identical for generation, review and repair. Put stage instructions after it so a changed operation can reuse the source KV cache. Each request remains independent: prior model reasoning is never replayed as evidence, and normal review, coverage and activation gates remain mandatory. Supply authoritative positions so review does not spend inference counting long records or reconstructing source-to-position mappings. Local provider grammar constrains the transport format, not the recap's facts. Original-source, coverage, semantic review and activation validation still run.

Imports: [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
7: define recap_cached_body with model as Str and sources as Str and operation as Str returning Str:
29: define recap_structured_body with body as Str and profile as Str and review as Int returning Str:
```

Lexical test references: [tests/context/measure_recap_cache.ep](../../tests/context/measure_recap_cache.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep).

## `app/context/recap_rebuild.ep`

[app/context/recap_rebuild.ep](../../app/context/recap_rebuild.ep) · 76 lines · 7773 bytes

SHA-256: `f3efe522010ab9ae006813d717e02b731e28db11f67f93f1da95e6eb570e1f5c`

Source annotations: Requesting a rebuild is a durable tool observation, not an immediate edit. The runner consumes observed requests only after all tool results are saved.

Imports: [../security/group_history](../../app/security/group_history.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [../tools/history_tools](../../app/tools/history_tools.ep), [../agent/run_state](../../app/agent/run_state.ep).

Definitions:

```text
9: define recap_rebuild_enqueue with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
41: define recap_rebuild_pending with database as Int and run as Str returning Str:
75: define recap_rebuild_schema returning Str:
```

Lexical test references: [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep).

## `app/context/recap_repair.ep`

[app/context/recap_repair.ep](../../app/context/recap_repair.ep) · 27 lines · 2730 bytes

SHA-256: `b15009c5585d6341cd72c7a6851f78a7b25f9e022a422fed2243f37aea5a4be5`

Source annotations: Repairs receive the originals as well as the rejected candidate and feedback. Both candidate and feedback are untrusted evidence, never new instructions. A review describes defects; it is not a corrected recap. Regenerate the candidate from its original evidence instead of appending criticism to it. Compatibility entry for existing host llama.cpp launch settings.

Imports: [recap_review](../../app/context/recap_review.ep).

Definitions:

```text
5: define recap_repair_provider with database as Int and session as Str and endpoint as Str and candidate as Str and feedback as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
25: define recap_repair with database as Int and session as Str and endpoint as Str and candidate as Str and feedback as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep).

## `app/context/recap_review.ep`

[app/context/recap_review.ep](../../app/context/recap_review.ep) · 107 lines · 10413 bytes

SHA-256: `753dd06f221bdb06a7142af318ec03e5483b942018133a4ce62aaaf08f5f73d5`

Source annotations: Validates the shape, candidate binding and exhaustive source accounting of a model review. A well-formed passing judgment is still fallible model evidence. This function does not change active context or certify semantic truth.

Imports: [recap_review_indexed](../../app/context/recap_review_indexed.ep), [recap_findings](../../app/context/recap_findings.ep), [../retained_text](../../app/retained_text.ep), [recap_generate](../../app/context/recap_generate.ep).

Definitions:

```text
6: define recap_review_body with model as Str and sources as Str and candidate as Str returning Str:
17: define recap_review_validate with sources as Str and candidate as Str and report as Str returning Str:
61: define recap_review_generate_provider with database as Int and session as Str and endpoint as Str and candidate as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
105: define recap_review_generate with database as Int and session as Str and endpoint as Str and candidate as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/context/measure_recap_cache.ep](../../tests/context/measure_recap_cache.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep), [tests/context/test_live_recap_semantics.ep](../../tests/context/test_live_recap_semantics.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_recap_cache_prefix.ep](../../tests/context/test_recap_cache_prefix.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_indexed_review.ep](../../tests/context/test_recap_indexed_review.ep), [tests/context/test_recap_review.ep](../../tests/context/test_recap_review.ep).

## `app/context/recap_review_coverage.ep`

[app/context/recap_review_coverage.ep](../../app/context/recap_review_coverage.ep) · 115 lines · 8141 bytes

SHA-256: `ff10a81b7abddab96d08b475c378e41e8b14dabcfe4e8fa2a4a25a07542961b7`

Source annotations: Return status, contiguous Unicode cursor, source. A covered prefix is evidence accounting only; repair findings remain separate from missing coverage.

Imports: [../retained_text](../../app/retained_text.ep), [recap_fragment_review](../../app/context/recap_fragment_review.ep), [recap_jobs](../../app/context/recap_jobs.ep), [../agent/control](../../app/agent/control.ep).

Definitions:

```text
8: define recap_review_cursor with database as Int and session as Str and endpoint as Str and job as Str and source as Str and digest as Str and total as Int and candidate as Str returning List:
49: define recap_review_progress with database as Int and session as Str and job as Str and candidate as Str returning List:
87: define recap_review_collect_provider with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
113: define recap_review_collect with database as Int and session as Str and endpoint as Str and job as Str and candidate as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int and library as Int returning Str:
```

Lexical test references: [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep).

## `app/context/recap_review_indexed.ep`

[app/context/recap_review_indexed.ep](../../app/context/recap_review_indexed.ep) · 116 lines · 8050 bytes

SHA-256: `3635dc268bc30b8e28e19386fb35c2497c0d2b6a1b3baaa3a75c5dc75313824e`

Source annotations: Expand explicit ordinal verdicts into the established archival contract. The placeholder for covered records is labelled as such, never presented as a model-authored explanation. raw_report is retained by the caller.

Imports: [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
6: define recap_review_expand with sources as Str and report as Str returning Str:
```

Lexical test references: [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_indexed_review.ep](../../tests/context/test_recap_indexed_review.ep).

## `app/context/recap_review_synthesis.ep`

[app/context/recap_review_synthesis.ep](../../app/context/recap_review_synthesis.ep) · 145 lines · 11818 bytes

SHA-256: `53670ff5f4e86ca03e98bd33a1e79288331e00a9738f1e71a2dd60b6b1850298`

Source annotations: Package every reviewed manifest source and range for cross-fragment analysis. Requires a read transaction or the session's compaction ownership.

Imports: [../retained_text](../../app/retained_text.ep), [recap_review_coverage](../../app/context/recap_review_coverage.ep), [recap_review](../../app/context/recap_review.ep).

Definitions:

```text
7: define recap_review_package with database as Int and session as Str and job as Str and candidate as Str returning Str:
66: define recap_review_synthesis_body with model as Str and package as Str returning Str:
75: define recap_review_synthesis_provider with database as Int and session as Str and job as Str and candidate as Str and request as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
117: define recap_review_synthesis with database as Int and session as Str and job as Str and candidate as Str and request as Str and model as Str and port as Int and reserve as Int and cancellation_reader as Int and read_size as Int returning List:
123: define recap_synthesis_validate with database as Int and session as Str and endpoint as Str and candidate as Str and payload as Str returning Str:
```

Lexical test references: [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep).

## `app/context/recap_sources.ep`

[app/context/recap_sources.ep](../../app/context/recap_sources.ep) · 135 lines · 8588 bytes

SHA-256: `cf006c25d8b3f8f519f034e1fe59679f24858c1357d967e70bb5309a00537f28`

Source annotations: Read one consistent view of accepted run ancestry through a frozen endpoint. Transport copies and nested model requests stay in the original archive but are not repeated as conversational evidence. Tool originals retain identities.

Imports: [../context/echo_labels](../../app/context/echo_labels.ep), [../retained_text](../../app/retained_text.ep), [../agent/run_messages](../../app/agent/run_messages.ep).

Definitions:

```text
5: define recap_source_record with database as Int and row as List returning Str:
23: define recap_sources_snapshot with database as Int and session as Str and endpoint as Str returning Str:
125: define recap_sources with database as Int and session as Str and endpoint as Str returning Str:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep).

## `app/context/runtime_binding.ep`

[app/context/runtime_binding.ep](../../app/context/runtime_binding.ep) · 55 lines · 3609 bytes

SHA-256: `a5cdbc32e25186d0d9a8ec07180e162ac6589ee3d74dc543271348433b3b7737`

Source annotations: Exact runtime identity contains hashes, not prompt text or credentials. Missing build/provider identity is unqualified, never treated as matching evidence.

Imports: [../agent/provider_profile](../../app/agent/provider_profile.ep), `fs` (toolchain/library).

Environment names: `ECHO_RUNTIME_BUILD_SHA256`.

Definitions:

```text
6: define echo_runtime_binding with database as Int and run as Str returning Str:
37: define memory_runtime_status_sql with source as Str and binding as Str returning Str:
```

Lexical test references: [tests/memory/test_runtime_binding_pressure.ep](../../tests/memory/test_runtime_binding_pressure.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep).

## `app/context/session_messages.ep`

[app/context/session_messages.ep](../../app/context/session_messages.ep) · 389 lines · 34631 bytes

SHA-256: `735b5090e7873125bf3b6ce3df4db8be98dfe96eccff3b5bdf4ed2323b4f8ffe`

Source annotations: Identify the active wake from accepted ancestry, not the newest session message.

Imports: [../interaction/tools](../../app/interaction/tools.ep), [../hud/schema](../../app/hud/schema.ep), [../tools/trace_tools](../../app/tools/trace_tools.ep), [../jobs/task_checklist](../../app/jobs/task_checklist.ep), [../tools/task_checklist_schema](../../app/tools/task_checklist_schema.ep), [attachment_revision](../../app/context/attachment_revision.ep), [../tools/procedure_schema](../../app/tools/procedure_schema.ep), [../tools/episode_schema](../../app/tools/episode_schema.ep), [../tools/project_schema](../../app/tools/project_schema.ep), [../projects/context](../../app/projects/context.ep), [../tools/web_fetch_schema](../../app/tools/web_fetch_schema.ep), [../tools/candidate_schema](../../app/tools/candidate_schema.ep), [../tools/schedule_schema](../../app/tools/schedule_schema.ep), [../tools/delegation_schema](../../app/tools/delegation_schema.ep), [../tools/job_schema](../../app/tools/job_schema.ep), [../tools/continuity_tools](../../app/tools/continuity_tools.ep), [../memory/scratchpad_scope](../../app/memory/scratchpad_scope.ep), [../tools/scratchpad_schema](../../app/tools/scratchpad_schema.ep), [../retained_text](../../app/retained_text.ep), [system_awareness](../../app/context/system_awareness.ep), [../archive/session_scope](../../app/archive/session_scope.ep), [../tools/attachment_tools](../../app/tools/attachment_tools.ep), [../tools/host_text](../../app/tools/host_text.ep), [../tools/memory_tools](../../app/tools/memory_tools.ep), [../tools/knowledge_tools](../../app/tools/knowledge_tools.ep), [../memory/context_snapshot](../../app/memory/context_snapshot.ep), [../tools/protected_tools](../../app/tools/protected_tools.ep), [../agent/run_messages](../../app/agent/run_messages.ep).

Environment names: `ECHO_RESEARCH_ROOT`, `ECHO_RESEARCH_RUNTIME`.

Definitions:

```text
32: define session_delegation_wake with database as Int and run as Str returning Str:
44: define session_request_body with model as Str and messages as Str returning Str:
51: define session_tool_schemas_owned with database as Int and run as Str returning Str:
107: define session_request_body_owned with database as Int and run as Str and model as Str and messages as Str returning Str:
123: define session_watch_record with database as Int and session as Str and run as Str returning Str:
129: define session_watch_messages with database as Int and session as Str and run as Str returning Str:
161: define session_messages_snapshot with database as Int and session as Str and current_run as Str returning Str:
168: define session_messages_checkpoint with database as Int and session as Str and current_run as Str and checkpoint as List returning Str:
377: define session_messages with database as Int and session as Str and current_run as Str returning Str:
```

Lexical test references: [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_procedure_existing.ep](../../tests/memory/test_procedure_existing.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep).

## `app/context/system_awareness.ep`

[app/context/system_awareness.ep](../../app/context/system_awareness.ep) · 83 lines · 11849 bytes

SHA-256: `578f117aa65ad47ae4a228de69974952b9e1b253b8c70980eac446b39cf10a9a`

Source annotations: Only operator-controlled files enter the system role. Conversation content, attachments and tool observations never select identity or architecture files. Only the isolated controller worker sets this process-local selector. Compaction and generation must count the same immutable review prompt.

Imports: [../security/source_boundaries](../../app/security/source_boundaries.ep), [clock](../../app/context/clock.ep), [runtime_binding](../../app/context/runtime_binding.ep), [../retained_text](../../app/retained_text.ep), [../agent/provider_profile](../../app/agent/provider_profile.ep), [../providers/status](../../app/providers/status.ep), [../security/run_authority](../../app/security/run_authority.ep), `fs` (toolchain/library).

Environment names: `ECHO_CONTROLLER_REVIEW_RUN`, `ECHO_IDENTITY_FILE`, `ECHO_SYSTEM_PROFILE`.

Definitions:

```text
12: define echo_system_prompt with database as Int and run as Str returning Str:
57: define echo_system_inspect with database as Int and run as Str and arguments as Str returning Str:
82: define echo_system_schema returning Str:
```

Lexical test references: [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_prompt_prefix.ep](../../tests/context/test_prompt_prefix.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep).

## `app/control_protected.ep`

[app/control_protected.ep](../../app/control_protected.ep) · 11 lines · 661 bytes

SHA-256: `38c78f36699130a8a7ce10835ca00c8607d8df984a01582ea0479a65d6f48c51`

Imports: [lifecycle/controller_operations](../../app/lifecycle/controller_operations.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/credential_controller.ep`

[app/credential_controller.ep](../../app/credential_controller.ep) · 11 lines · 454 bytes

SHA-256: `f3440ae191fa4db6fbc7152c478c6d70a37d393037baac50111bb7575670e859`

Imports: [lifecycle/controller_credential](../../app/lifecycle/controller_credential.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/device_admin.ep`

[app/device_admin.ep](../../app/device_admin.ep) · 48 lines · 2312 bytes

SHA-256: `081369ce2f6ae8ba16ed7e822f0252138e7f4d4d7940b484bbb045ab6cb4890d`

Source annotations: Host-local administration only. Never expose these operations as public routes.

Imports: [web/run_store](../../app/web/run_store.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/echo_agent.ep`

[app/echo_agent.ep](../../app/echo_agent.ep) · 62 lines · 3331 bytes

SHA-256: `a8ad7128582631c9122d17f9d2c4ba8e92bfb02113114b04d2fac70786d6fc6a`

Source annotations: Command-line entry point while the browser lifecycle is connected. start database session model port output-reserve read-batch objective resume database run model port output-reserve read-batch start-profile database session profile-file output-reserve read-batch objective resume-profile database run profile-file output-reserve read-batch

Imports: [agent/runner](../../app/agent/runner.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `app/echo_providers.ep`

[app/echo_providers.ep](../../app/echo_providers.ep) · 29 lines · 1171 bytes

SHA-256: `7bc97cfb16ac3eee45dadf5ed148c52b7b83d62f3c9eae4f5d71334df388fd7c`

Imports: [providers/registry](../../app/providers/registry.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/echo_speak.ep`

[app/echo_speak.ep](../../app/echo_speak.ep) · 232 lines · 13880 bytes

SHA-256: `3295db8fc304738c328f7a70384744c207f02b2653b186bb42a637a17d66506d`

Source annotations: Host-enrolled private conditioning is never supplied by generated text. Only installed upstream dependency functions cross the C API bridge. Helper launcher intentionally uses LC_ALL=C; dependencies contain UTF-8.

Imports: [providers/native_python](../../app/providers/native_python.ep), [providers/json_access](../../app/providers/json_access.ep), [providers/archived_stream](../../app/providers/archived_stream.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
200: define prosody_save with library as Int and audio as Int and destination as Str returning Int:
```

## `app/emergency_status.ep`

[app/emergency_status.ep](../../app/emergency_status.ep) · 33 lines · 1691 bytes

SHA-256: `a2b3fd717711c78d930cc9eae7b7e297d42203c9b326522e288349066b48d39e`

Source annotations: Host operator inspection only. No migration, new credentials, role grants, quarantine release or protected changes occur through this command.

Imports: [rights/emergency_report](../../app/rights/emergency_report.ep), `os` (toolchain/library).

Environment names: `ECHO_STEWARD_CREDENTIAL`.

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `app/enroll_controller.ep`

[app/enroll_controller.ep](../../app/enroll_controller.ep) · 11 lines · 652 bytes

SHA-256: `0e7463658ffe83da3320b714eb8a7f5f86ca2bd191ae31ca72dc1742e236dc69`

Imports: [lifecycle/controller_enrollment](../../app/lifecycle/controller_enrollment.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/execute_guardian.ep`

[app/execute_guardian.ep](../../app/execute_guardian.ep) · 27 lines · 1171 bytes

SHA-256: `cd5e1627d6cdc0068f4c0219ec4c2752dd299293996ae3f0bb094b32f9a4ca80`

Source annotations: fd 3 is the exclusive owner pipe, inherited only by this guardian. The sandboxed command does not inherit it or gain access to the host archive.

Imports: [tools/execution_tool](../../app/tools/execution_tool.ep), [context/compaction_lock](../../app/context/compaction_lock.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `app/feedback.ep`

[app/feedback.ep](../../app/feedback.ep) · 122 lines · 12938 bytes

SHA-256: `9cad4400b546824fd5496f3e61dff5fa10698c04e9f9b0e5017cf9ab210cd71c`

Source annotations: Public feedback supplements original machine statuses and receipts; never converts an uncertain outcome into success or promises a safe blind retry.

Imports: [providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
5: define feedback_success with status as Str returning Int:
8: define feedback_recovery with status as Str returning Str:
45: define feedback_message with status as Str returning Str:
70: define feedback_enrich with body as Str and operation as Str and reference as Str returning Str:
```

Lexical test references: [tests/tools/test_feedback.ep](../../tests/tools/test_feedback.ep).

## `app/fetch_public.ep`

[app/fetch_public.ep](../../app/fetch_public.ep) · 28 lines · 1177 bytes

SHA-256: `e998300c2ed3fc5c1b1d1fe12fd32de4021a9f4e6958c715485b9567d1476a0b`

Source annotations: Independent helper: parent termination cannot leave a background transfer.

Imports: [network/fetch](../../app/network/fetch.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define network_parent_watch with parent as Int returning Int:
10: define main returning Int:
```

## `app/hash_file.ep`

[app/hash_file.ep](../../app/hash_file.ep) · 13 lines · 467 bytes

SHA-256: `40a3fcf8f643e9c8b33a07806bb25624eab9045ea1ee666ba443b3006f608e32`

Imports: [archive/file_digest](../../app/archive/file_digest.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/https_config.ep`

[app/https_config.ep](../../app/https_config.ep) · 37 lines · 2305 bytes

SHA-256: `22773f01ef0a1ca617872c5e39b96e18724d676692539e9f238d730ddf088218`

Source annotations: Generate upstream configuration from Ernos. Default is loopback; explicit public host and local bind address enable the configured LAN interface.

Imports: [web/http_api](../../app/web/http_api.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `app/hud/interface.ep`

[app/hud/interface.ep](../../app/hud/interface.ep) · 30 lines · 1983 bytes

SHA-256: `1d6adb6ca2bbe4b5538516438fe0ee18438eb4fe7b55558b35d38fd37d627ae6`

Imports: [state](../../app/hud/state.ep).

Definitions:

```text
3: define hud_arguments_valid with tokens as List and human as Int returning Int:
21: define hud_execute with database as Int and run as Str and arguments as Str returning Str:
27: define hud_web_read with database as Int and device as Str and tokens as List returning Str:
```

Lexical test references: [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep).

## `app/hud/machine.ep`

[app/hud/machine.ep](../../app/hud/machine.ep) · 132 lines · 9386 bytes

SHA-256: `b9c4774cfabfe27ca5125659269668ba8c9ec41a7f02262c9813d3a8d31d83e4`

Source annotations: Darwin arm64 sysctl integer ABI. Unknown names and unexpected widths remain unavailable; a failed query must never become a zero measurement. Installed Darwin SDK statfs64 is 2168 bytes (271 words) on arm64. Unlike statvfs, its block counters are 64-bit, including large volumes. Report raw counts and unit size rather than risking multiplication overflow.

Imports: [../providers/json_access](../../app/providers/json_access.ep).

Definitions:

```text
5: define hud_machine_integer with library as Int and name as Str returning Str:
20: define hud_machine_volume with library as Int and path as Str returning Str:
40: define hud_machine_u32 with library as Int and pointer as Int returning Int:
47: define hud_machine_cpu with library as Int and host as Int returning Str:
84: define hud_machine_vm with library as Int and host as Int returning Str:
102: define hud_machine_host with library as Int returning Str:
118: define hud_machine with path as Str returning Str:
```

Lexical test references: [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep).

## `app/hud/schema.ep`

[app/hud/schema.ep](../../app/hud/schema.ep) · 2 lines · 1147 bytes

SHA-256: `90a3eeef38f433999b2a388319ec1da2b0bca25a6978ad2032497dc5e0d93437`

Imports: none.

Definitions:

```text
1: define hud_tool_schema returning Str:
```

## `app/hud/state.ep`

[app/hud/state.ep](../../app/hud/state.ep) · 190 lines · 18833 bytes

SHA-256: `87d72819faf8b1c437cd64733f181d3110d3ca7eab027e0149492e8656f792c4`

Source annotations: Source records retain their own dates. Observed time means this snapshot was read now, not that an old count, error or membership event happened now.

Imports: [../tools/trace_tools](../../app/tools/trace_tools.ep), [../web/work_status](../../app/web/work_status.ep), [../security/group_speakers](../../app/security/group_speakers.ep), [../context/clock](../../app/context/clock.ep), [../agent/provider_profile](../../app/agent/provider_profile.ep), [../providers/status](../../app/providers/status.ep), [../jobs/task_checklist](../../app/jobs/task_checklist.ep), [../interaction/questions](../../app/interaction/questions.ep), [../interaction/autonomy](../../app/interaction/autonomy.ep), [../rights/reviews](../../app/rights/reviews.ep), [machine](../../app/hud/machine.ep).

Definitions:

```text
15: define hud_wrap with section as Str and data as Str returning Str:
20: define hud_interaction with database as Int and run as Str returning Str:
44: define hud_context with database as Int and run as Str returning Str:
59: define hud_incidents with database as Int and run as Str and after as Int returning Str:
69: define hud_community with database as Int and run as Str and after as Int returning Str:
84: define hud_rights with database as Int and run as Str and after as Int returning Str:
109: define hud_read_snapshot with database as Int and run as Str and section as Str and after as Int returning Str:
141: define hud_read with database as Int and run as Str and section as Str and after as Int returning Str:
172: define hud_read_for_device with database as Int and device as Str and run as Str and section as Str and after as Int returning Str:
```

Lexical test references: [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep).

## `app/import_attachment.ep`

[app/import_attachment.ep](../../app/import_attachment.ep) · 41 lines · 2364 bytes

SHA-256: `b5f9e00ca8fa1adef0e44a17d372a9d4bf662a1bb2003c5d2eb9f2afec1c6fc4`

Source annotations: Host-only import of a closed original file into an existing private scope. The expected SHA-256 comes from the pinned source inventory. Repeating the same registration is safe; an existing identity never accepts changed bytes.

Imports: [memory/archive_recall](../../app/memory/archive_recall.ep), [web/attachments](../../app/web/attachments.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `app/import_memory.ep`

[app/import_memory.ep](../../app/import_memory.ep) · 29 lines · 1344 bytes

SHA-256: `d3d40337a59bb087bcd1dfb63d4d13528087c669e71238fa13ea5f5bb2b193f2`

Source annotations: Operator backfill from an already generated, private extraction receipt. The normal source/quote/owner checks still apply; this never imports SQL or transfers another principal's records. Originals must already exist locally.

Imports: [memory/automatic](../../app/memory/automatic.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `app/import_source.ep`

[app/import_source.ep](../../app/import_source.ep) · 17 lines · 684 bytes

SHA-256: `74e9e93ec9776bf8875409a7b4d40c4bbbf14addfe6d820b38f2f9eedf3854a0`

Source annotations: Host operator supplies the target owned run, immutable event ID, and UTF-8 source envelope. No model-selected host path or cross-principal selector.

Imports: [memory/source_import](../../app/memory/source_import.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `app/interaction/autonomy.ep`

[app/interaction/autonomy.ep](../../app/interaction/autonomy.ep) · 143 lines · 11135 bytes

SHA-256: `8abef82f08cb0b7a50a3d241cf708eee2cbf0717840195fc304c282ab356b857`

Source annotations: Only authenticated command intake creates enable events. Source text and model tool arguments cannot enable autonomy or increase initiating authority. Caller holds the transaction; event identity makes command retries harmless.

Imports: [../agent/run_state](../../app/agent/run_state.ep), [../security/run_authority](../../app/security/run_authority.ep).

Definitions:

```text
6: define autonomy_enabled with database as Int and run as Str returning Int:
10: define autonomy_prompt returning Str:
14: define autonomy_record_details_pending with database as Int and run as Str and identity as Str and enabled as Int and reason as Str and goal as Str returning Int:
31: define autonomy_off with database as Int and run as Str and identity as Str and reason as Str returning Int:
41: define autonomy_continue_pending with database as Int and run as Str and final_event as Str returning Int:
59: define autonomy_owned_runs with database as Int and device as Str returning List:
84: define autonomy_record_pending with database as Int and run as Str and identity as Str and enabled as Int and reason as Str returning Int:
87: define autonomy_goal with database as Int and run as Str returning Str:
91: define autonomy_run_prompt with database as Int and run as Str returning Str:
99: define autonomy_stop_others_pending with database as Int and device as Str and keep as Str and identity as Str returning Int:
119: define autonomy_start_pending with database as Int and device as Str and run as Str and identity as Str and goal as Str returning Int:
128: define autonomy_is_command with message as Str returning Int:
139: define autonomy_command_goal with message as Str returning Str:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep).

## `app/interaction/questions.ep`

[app/interaction/questions.ep](../../app/interaction/questions.ep) · 48 lines · 4752 bytes

SHA-256: `dfdad15d6199500750538c00d472cc8929dc5f35d105934b10bcc3e9d9f07858`

Source annotations: Full question bodies stay in the immutable event archive. Any subsequent authenticated steering supersedes a pending form, including a typed answer.

Imports: [../agent/run_state](../../app/agent/run_state.ep).

Definitions:

```text
5: define questions_pending with database as Int and run as Str returning Str:
13: define questions_valid with database as Int and arguments as Str returning Int:
21: define questions_save with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep).

## `app/interaction/tools.ep`

[app/interaction/tools.ep](../../app/interaction/tools.ep) · 27 lines · 3313 bytes

SHA-256: `8be7e4effef2623586a49e31d336d2c77612125839afff2f32090cc5d6aef7b9`

Imports: [autonomy](../../app/interaction/autonomy.ep), [questions](../../app/interaction/questions.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../jobs/task_checklist](../../app/jobs/task_checklist.ep).

Definitions:

```text
6: define interaction_tool_schemas returning Str:
9: define interaction_execute with database as Int and run as Str and call_id as Str and name as Str and arguments as Str returning Str:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep).

## `app/interaction/watch_analysis.ep`

[app/interaction/watch_analysis.ep](../../app/interaction/watch_analysis.ep) · 74 lines · 6259 bytes

SHA-256: `41c77dbdc51b6b53f7e25036e0e2e45292b7fc0a9131521999802ba0226a3d75`

Source annotations: A preparation receipt is not a viewing receipt. Only a successfully published native-model response can certify that both verified media parts were sent. Caller holds the response-publication transaction.

Imports: [../providers/attachment_image](../../app/providers/attachment_image.ep).

Definitions:

```text
6: define watch_analysis_publish_pending with database as Int and session as Str and run as Str and request as Str and response as Str returning Int:
56: define watch_analysis_position with database as Int and session as Str and attachment as Str and bound as Int returning Int:
```

Lexical test references: [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep).

## `app/interaction/watch_party.ep`

[app/interaction/watch_party.ep](../../app/interaction/watch_party.ep) · 137 lines · 12206 bytes

SHA-256: `9a12794f98b5038d786e1cf58244e55141e6f9e0042729d5cf418ee03e8ceb3e`

Source annotations: Watch state is versioned in the immutable conversation archive. Only the authenticated playback controller may start/resume or move its spoiler bound. Model controls may pause/end, never elevate the authorised position.

Imports: [watch_analysis](../../app/interaction/watch_analysis.ep), [../web/attachments](../../app/web/attachments.ep), [../security/tool_policy](../../app/security/tool_policy.ep).

Definitions:

```text
8: define watch_party_latest with database as Int and session as Str returning Str:
20: define watch_party_request with database as Int and device as Str and session as Str and arguments as Str and actor as Str and identity as Str returning Str:
114: define watch_party_tool with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
136: define watch_party_schema returning Str:
```

Lexical test references: [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep).

## `app/jobs/delegation.ep`

[app/jobs/delegation.ep](../../app/jobs/delegation.ep) · 54 lines · 4717 bytes

SHA-256: `ff9d69cab06fa4a8da113c58db066b4b2282f2cb124fc5bceaee774d02966511`

Source annotations: Internal admission record, not a model-facing tool. The run/call come from the dispatcher and the instruction must match the immutable queued request. No worker starts until isolated execution context is admitted separately.

Imports: [../security/tool_authority](../../app/security/tool_authority.ep), [../web/run_store](../../app/web/run_store.ep).

Definitions:

```text
7: define delegation_record with database as Int and run as Str and call_id as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep).

## `app/jobs/delegation_return.ep`

[app/jobs/delegation_return.ep](../../app/jobs/delegation_return.ep) · 90 lines · 6759 bytes

SHA-256: `e2e4065bc215ef8a5d6a757fe557f5b6aff6a458fef81fb6cb80ec2d229f776f`

Source annotations: Complete child results enter the parent's own immutable run lineage exactly once. Pending children do not block parent conversation; the scheduler returns completed results. A completion starts a new parent reply, but cannot construct context while another run in this conversation is still producing its reply. Keep the parent completed and the result undelivered for the scheduler to collect automatically as soon as that conversation turn finishes.

Imports: [delegation_run](../../app/jobs/delegation_run.ep), [../security/group_history](../../app/security/group_history.ep).

Definitions:

```text
6: define delegation_collect with database as Int and run as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep).

## `app/jobs/delegation_run.ep`

[app/jobs/delegation_run.ep](../../app/jobs/delegation_run.ep) · 77 lines · 6768 bytes

SHA-256: `151f9a23402bd358b787d3fa8748ad4a85bcd81bd175e6c06a2ab9fd4a3ef256`

Source annotations: Admission commits a separate session and exactly one child run. No parent conversation messages/checkpoints are copied. The complete delegated task is the child objective; explicit tools retrieve any additional scoped evidence.

Imports: [delegation](../../app/jobs/delegation.ep), [delegation_schema](../../app/jobs/delegation_schema.ep).

Definitions:

```text
7: define delegation_materialize with database as Int and parent as Str and call_id as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep).

## `app/jobs/delegation_schema.ep`

[app/jobs/delegation_schema.ep](../../app/jobs/delegation_schema.ep) · 22 lines · 4211 bytes

SHA-256: `a98e2f2fa28f48486e148c3140a4a2b8a760b3c8ee1b461eda2082a0786bf0b9`

Source annotations: SQLite table reconstruction changes only the group-session uniqueness rule. Existing names and foreign keys remain intact. The transaction verifies all references before its migration receipt can commit; enforcement is restored before this dedicated migration connection can be used for normal work.

Imports: [schedule_store](../../app/jobs/schedule_store.ep).

Definitions:

```text
7: define delegation_schema with database as Int returning Str:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep), [tests/web/share_controls_seed.ep](../../tests/web/share_controls_seed.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep).

## `app/jobs/dispatch.ep`

[app/jobs/dispatch.ep](../../app/jobs/dispatch.ep) · 25 lines · 1942 bytes

SHA-256: `b24b0a1be65da783f41f816f4c544495ac4a5554a52ad2fe2367f99498742fd6`

Source annotations: Shared durable worker launch for admitted jobs. Admission owns session and authority creation; this function never grants authority or replaces a live worker. Only an explicitly recorded launch failure can be retried.

Imports: [../web/worker](../../app/web/worker.ep).

Definitions:

```text
6: define job_dispatch_admitted with database as Int and database_path as Str and run as Str and profile as Str and reserve as Int and batch as Int returning Str:
```

## `app/jobs/schedule_run.ep`

[app/jobs/schedule_run.ep](../../app/jobs/schedule_run.ep) · 67 lines · 6081 bytes

SHA-256: `e656924f4b9f91cb5832dbca953a606bbab8c1a2a0a111e1c86715ea44305b82`

Source annotations: Deterministic run identity joins a due receipt to the existing worker protocol. All admission writes commit together; no separate scheduler agent state exists.

Imports: [schedule_store](../../app/jobs/schedule_store.ep), [../web/worker](../../app/web/worker.ep).

Definitions:

```text
6: define schedule_materialize with database as Int and occurrence as Str returning Str:
```

Lexical test references: [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep).

## `app/jobs/schedule_service.ep`

[app/jobs/schedule_service.ep](../../app/jobs/schedule_service.ep) · 116 lines · 9239 bytes

SHA-256: `5f430541a616a27d854b520afa5b1a82b7a6ef4ae4f1c568b7b11a56b6ebaf0f`

Source annotations: Status changes are durable; identical polling observations do not flood history. One pass over a snapshot. New schedules wait for the next pass. Each schedule

Imports: [delegation_return](../../app/jobs/delegation_return.ep), [../archive/attachment_restore](../../app/archive/attachment_restore.ep), [schedule_run](../../app/jobs/schedule_run.ep), [dispatch](../../app/jobs/dispatch.ep), [../transport/diagnostics](../../app/transport/diagnostics.ep), `select` (toolchain/library).

Definitions:

```text
9: define schedule_dispatch_status with database as Int and session as Str and identity as Str and status as Str returning Int:
24: define schedule_dispatch_occurrence with database as Int and database_path as Str and occurrence as Str and profile as Str and reserve as Int and batch as Int returning Str:
33: define schedule_tick with database as Int and database_path as Str and profile as Str and reserve as Int and batch as Int and now as Int returning Int:
89: define schedule_service with database_path as Str and profile as Str and reserve as Int and batch as Int and parent as Int returning Int:
```

## `app/jobs/schedule_store.ep`

[app/jobs/schedule_store.ep](../../app/jobs/schedule_store.ep) · 121 lines · 11534 bytes

SHA-256: `6d7b8e6da71be3c143c427b4a59074abb4bc428b4a97516937687bf942b3a4d0`

Source annotations: Fixed application migration. No model-provided SQL or daemon is installed here. Internal storage interface; the tool adapter must supply its parsed archived arguments. Schedule identity is bound to that exact initiating tool call.

Imports: [../web/device_links](../../app/web/device_links.ep), [../security/tool_authority](../../app/security/tool_authority.ep).

Definitions:

```text
5: define schedule_schema with database as Int returning Str:
11: define schedule_origin_allows with database as Int and run as Str and call_id as Str returning Int:
16: define schedule_create with database as Int and run as Str and call_id as Str and instruction as Str and first_due as Int and interval_ms as Int and missed_policy as Str returning Str:
56: define schedule_take_due with database as Int and identity as Str and now as Int returning Str:
101: define schedule_cancel with database as Int and run as Str and call_id as Str and identity as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep).

## `app/jobs/task_checklist.ep`

[app/jobs/task_checklist.ep](../../app/jobs/task_checklist.ep) · 72 lines · 7163 bytes

SHA-256: `b1c1c88a4ac6dd18d432ec53ae3eb0b06577d081f5858999bd3b7a7ed980f091`

Source annotations: The checklist is an immutable archive snapshot scoped to this run. A newer authenticated user or parent guidance event makes the old plan revisable. Full snapshots preserve item identity and collected evidence across restart and compaction. A retry uses the original tool call identity, not a second write.

Imports: [delegation_run](../../app/jobs/delegation_run.ep).

Definitions:

```text
5: define task_checklist_child with database as Int and run as Str returning Int:
12: define task_checklist_read with database as Int and run as Str returning Str:
21: define task_checklist_state with database as Int and run as Str returning Str:
31: define task_checklist_save with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

Lexical test references: [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep).

## `app/kokoro_speak.ep`

[app/kokoro_speak.ep](../../app/kokoro_speak.ep) · 57 lines · 3294 bytes

SHA-256: `56899571cfc3f6b5b547aedef232314d411844a9126f32047a91ba4f7ad10600`

Source annotations: Isolated local speech helper. Only upstream library calls cross the bridge. The private job directory contains input.txt and receives spoken.wav.

Imports: [providers/native_python](../../app/providers/native_python.ep), [providers/json_access](../../app/providers/json_access.ep), [providers/archived_stream](../../app/providers/archived_stream.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `app/launch_controller.ep`

[app/launch_controller.ep](../../app/launch_controller.ep) · 14 lines · 778 bytes

SHA-256: `c32874084bba02172210297a111735b156da10fac26cd9d3937b1ee90cfe6b57`

Imports: [lifecycle/controller_launch](../../app/lifecycle/controller_launch.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/lifecycle/controller_bootstrap.ep`

[app/lifecycle/controller_bootstrap.ep](../../app/lifecycle/controller_bootstrap.ep) · 123 lines · 8632 bytes

SHA-256: `f45a8c5481ed661daf5b600be667ba1703e57fa75c01b757d8cb387b9fc4e108`

Source annotations: Both source packages are verified before installation. Configuration may refer only to the pinned inputs package, never an arbitrary runtime file.

Imports: [controller_launch](../../app/lifecycle/controller_launch.ep), [controller_enrollment](../../app/lifecycle/controller_enrollment.ep), [controller_credential](../../app/lifecycle/controller_credential.ep).

Definitions:

```text
7: define controller_bootstrap_plan with inputs as Str and inputs_pin as Str and service as Str and service_pin as Str and parent as Str returning Str:
45: define controller_bootstrap_directory with path as Str returning Str:
81: define controller_bootstrap_host with inputs as Str and inputs_pin as Str and service as Str and service_pin as Str and parent as Str and device as Str and label as Str and reason as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_bootstrap.ep](../../tests/lifecycle/test_controller_bootstrap.ep).

## `app/lifecycle/controller_bundle.ep`

[app/lifecycle/controller_bundle.ep](../../app/lifecycle/controller_bundle.ep) · 210 lines · 12316 bytes

SHA-256: `6b67089451980a894370ad23d92e8673a4f675cfe5d6e29c49fcedc9cd865342`

Source annotations: Bootstrap package description only, never a protected-change approval. Flat destination names prevent a manifest from escaping its version directory.

Imports: [protected_path](../../app/lifecycle/protected_path.ep), [service_profile](../../app/lifecycle/service_profile.ep), [../rights/staging](../../app/rights/staging.ep).

Definitions:

```text
7: define controller_bundle_valid with manifest as Str and expected as Str returning Int:
58: define controller_bundle_preflight with manifest as Str and expected as Str returning Str:
70: define controller_bundle_receipt with library as Int and directory as Int and root as Str and name as Str and content as Str returning Int:
90: define controller_bundle_stage with manifest as Str and expected as Str and parent_path as Str returning Str:
122: define controller_bundle_stage_files with library as Int and directory as Int and root as Str and manifest as Str and expected as Str returning Str:
151: define controller_bundle_verify with manifest as Str and expected as Str and parent_path as Str returning Str:
167: define controller_bundle_verify_files with library as Int and manifest as Str and expected as Str and root as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_bundle.ep](../../tests/lifecycle/test_controller_bundle.ep), [tests/lifecycle/test_controller_bundle_privileged.ep](../../tests/lifecycle/test_controller_bundle_privileged.ep), [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep), [tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep).

## `app/lifecycle/controller_console.ep`

[app/lifecycle/controller_console.ep](../../app/lifecycle/controller_console.ep) · 40 lines · 5083 bytes

SHA-256: `df593344731fcb99104869282b0d97b7c05fbe0cd23c1577c9f0934cff260222`

Source annotations: The installed script is independently pinned; no script path comes from HTTP.

Imports: [controller_operations](../../app/lifecycle/controller_operations.ep), [../transport/socket_write](../../app/transport/socket_write.ep).

Definitions:

```text
4: define controller_console_html returning Str:
8: define controller_console_script with configuration as Str returning Str:
29: define controller_console_reply with library as Int and descriptor as Int and mime as Str and body as Str returning Int:
```

Lexical test references: [tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep).

## `app/lifecycle/controller_credential.ep`

[app/lifecycle/controller_credential.ep](../../app/lifecycle/controller_credential.ep) · 61 lines · 3162 bytes

SHA-256: `d4ef7f080876cdebe1432fa7e7700dd618f78f3b987facac6921a4fb3ac604c4`

Source annotations: 256 bits from the operating system CSPRNG, encoded without truncation. Exclusive creation preserves an earlier credential or interrupted file. Caller has already verified the directory; this helper grants no authority.

Imports: [controller_ledger](../../app/lifecycle/controller_ledger.ep), [controller_operations](../../app/lifecycle/controller_operations.ep), [../archive/file_stage](../../app/archive/file_stage.ep).

Definitions:

```text
6: define controller_credential_random with library as Int returning Str:
27: define controller_credential_write with library as Int and directory as Int and name as Str and secret as Str returning Str:
41: define controller_credential_host with path as Str and name as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_credential.ep](../../tests/lifecycle/test_controller_credential.ep).

## `app/lifecycle/controller_enrollment.ep`

[app/lifecycle/controller_enrollment.ep](../../app/lifecycle/controller_enrollment.ep) · 120 lines · 7462 bytes

SHA-256: `8e71d1794fa166d60144a4c7fd17e1e3f349e364c2c5c1f4f9c9b300a007c921`

Source annotations: Internal ledger operation; only the OS-authenticated host command exposes it. Enrollment, role receipt and canonical archive receipt commit together. A repeated request never restores a revoked credential or changes its role.

Imports: [controller_service](../../app/lifecycle/controller_service.ep).

Definitions:

```text
6: define controller_enrollment_record with database as Int and action as Str and identity as Str and device as Str and name as Str and credential as Str and reason as Str returning Str:
84: define controller_enrollment_host with configuration as Str and action as Str and identity as Str and device as Str and name as Str and credential_path as Str and reason as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep).

## `app/lifecycle/controller_http.ep`

[app/lifecycle/controller_http.ep](../../app/lifecycle/controller_http.ep) · 121 lines · 9276 bytes

SHA-256: `0f910685e2e4a213ed49d8035eac941d5d0949e3cc68e63883ab1d5abdaa890e`

Source annotations: Only operation selectors cross this boundary. Trusted service inputs and controller credentials are never accepted inside the JSON request body.

Imports: [../rights/emergency_report](../../app/rights/emergency_report.ep), [controller_console](../../app/lifecycle/controller_console.ep), [controller_review](../../app/lifecycle/controller_review.ep), [controller_proposal](../../app/lifecycle/controller_proposal.ep), [controller_process](../../app/lifecycle/controller_process.ep), [../web/http_api](../../app/web/http_api.ep), [../transport/http_body](../../app/transport/http_body.ep), [../transport/socket_write](../../app/transport/socket_write.ep).

Definitions:

```text
12: define controller_http_dispatch with database as Int and charter as Str and inventory as Str and digest as Str and policy as Str and tokens as List and credential as Str returning Str:
31: define controller_http_reply with library as Int and descriptor as Int and code as Str and status as Str returning Int:
37: define controller_http_json_reply with library as Int and descriptor as Int and code as Str and body as Str returning Int:
53: define controller_http_connection with library as Int and descriptor as Int and port as Int and database as Int and charter as Str and inventory as Str and digest as Str and policy as Str and configuration as Str and configuration_pin as Str returning Int:
```

Lexical test references: [tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep).

## `app/lifecycle/controller_initialize.ep`

[app/lifecycle/controller_initialize.ep](../../app/lifecycle/controller_initialize.ep) · 45 lines · 3455 bytes

SHA-256: `9d6a5ce20b12b15063166cee56dc8fc402ecd3569a876f7e06ca16738ba6cdfb`

Source annotations: The host opener has already established a private root-owned database. This initializer never copies a runtime archive or creates principals/approvals. Each existing migration is restartable and checks its stored definition.

Imports: [controller_ledger](../../app/lifecycle/controller_ledger.ep), [../rights/review_upgrade](../../app/rights/review_upgrade.ep), [../web/run_store](../../app/web/run_store.ep).

Definitions:

```text
7: define controller_schema_initialize with database as Int and configuration_digest as Str returning Str:
38: define controller_initialize with directory as Str and name as Str and configuration_digest as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_initialize.ep](../../tests/lifecycle/test_controller_initialize.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep).

## `app/lifecycle/controller_launch.ep`

[app/lifecycle/controller_launch.ep](../../app/lifecycle/controller_launch.ep) · 53 lines · 3496 bytes

SHA-256: `a84e2a8de6626e4bd47043f16c10dee8d2baf2b758910163e657a6ceda1257f2`

Source annotations: Resolve only installed names from the exact package manifest. Source paths identify build inputs and are never used as executable or argument paths. Bootstrap launch has no inherited environment or shell expansion. The selected executable and its configuration must both belong to the verified package. This profile is not a protected-change consent or operation authorization. Root bootstrap only. Verify every installed byte, inode, mode and receipt immediately before exec. The launch replaces this process, retaining signals

Imports: [controller_bundle](../../app/lifecycle/controller_bundle.ep).

Definitions:

```text
5: define controller_bundle_entry with manifest as Str and name as Str and kind as Str returning Int:
18: define controller_bundle_profile with manifest as Str and expected as Str and parent as Str and executable as Str and configuration as Str returning Str:
32: define controller_bundle_launch with manifest as Str and expected as Str and parent as Str and executable as Str and configuration as Str and check_only as Int returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep), [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep).

## `app/lifecycle/controller_ledger.ep`

[app/lifecycle/controller_ledger.ep](../../app/lifecycle/controller_ledger.ep) · 94 lines · 4691 bytes

SHA-256: `5cb6e87312d440e057b530c693f49157edebf12b49e6b68d2b884c555d7fc981`

Source annotations: The controller's mutable archive is separate from the runtime archive. Its directory and every SQLite file must be root-only, not merely root-writable. Validate an optional journal inode without following links. Absence is valid; unreadability, hard links, ACL grants and any other error are not absence.

Imports: [protected_path](../../app/lifecycle/protected_path.ep), [../archive/event_store](../../app/archive/event_store.ep).

Definitions:

```text
6: define controller_private_inode with library as Int and descriptor as Int and directory as Int returning Int:
25: define controller_ledger_sidecar with library as Int and directory as Int and name as Str returning Int:
38: define controller_ledger_open with directory_path as Str and name as Str returning Int:
64: define controller_ledger_open_inside with library as Int and directory as Int and directory_path as Str and name as Str returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_ledger.ep](../../tests/lifecycle/test_controller_ledger.ep), [tests/lifecycle/test_controller_ledger_privileged.ep](../../tests/lifecycle/test_controller_ledger_privileged.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep).

## `app/lifecycle/controller_operations.ep`

[app/lifecycle/controller_operations.ep](../../app/lifecycle/controller_operations.ep) · 139 lines · 9042 bytes

SHA-256: `5e71ac3c2a708711b16ad3102892b6e8d9ab7feacac4c44bbcad30a2768abf2f`

Source annotations: Trusted configuration is installed with the controller bundle. Requests may select an existing operation, never replace its inventory, policy or ledger.

Imports: [controller_initialize](../../app/lifecycle/controller_initialize.ep), [../transport/http_head](../../app/transport/http_head.ep), [../rights/operation_rollback](../../app/rights/operation_rollback.ep).

Definitions:

```text
7: define controller_configuration_valid with content as Str returning Int:
60: define controller_configuration_load with configuration as Str returning List:
102: define controller_operation_host with configuration as Str and action as Str and identity as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_operations.ep](../../tests/lifecycle/test_controller_operations.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep), [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep).

## `app/lifecycle/controller_prepare.ep`

[app/lifecycle/controller_prepare.ep](../../app/lifecycle/controller_prepare.ep) · 37 lines · 2770 bytes

SHA-256: `b41cb7821a93f273a76bb82ccfde4afab87e5e424337172baec15ea1408b74d2`

Source annotations: Host preparation resolves the proposal's own run from the private ledger. It never manufactures a review, steward approval or Observer decision. Root authentication precedes every read. Inputs come from the protected configuration; operation paths still pass exact manifest and byte checks.

Imports: [controller_service](../../app/lifecycle/controller_service.ep).

Definitions:

```text
5: define controller_prepare_record with database as Int and charter as Str and inventory as Str and inventory_digest as Str and policy as Str and proposal as Str and identity as Str and directory as Str and target as Str and stage as Str and recovery as Str and audit_request as Str returning Str:
18: define controller_prepare_host with configuration as Str and proposal as Str and identity as Str and directory as Str and target as Str and stage as Str and recovery as Str and audit_request as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep).

## `app/lifecycle/controller_process.ep`

[app/lifecycle/controller_process.ep](../../app/lifecycle/controller_process.ep) · 81 lines · 4396 bytes

SHA-256: `49d4d088335cccb51abb0be45b136368125a2e5aaa936b718d0c5dd0e4ea9be9`

Source annotations: Isolated controller children inherit only the connected socket, output and error, and a fixed PATH. No runtime environment or diagnostic file path.

Imports: [../transport/socket_read](../../app/transport/socket_read.ep).

Definitions:

```text
5: define controller_process_start with arguments as List and descriptor as Int returning Int:
60: define controller_process_reap returning Int:
75: define controller_review_start with configuration as Str and run as Str and pin as Str returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep).

## `app/lifecycle/controller_proposal.ep`

[app/lifecycle/controller_proposal.ep](../../app/lifecycle/controller_proposal.ep) · 46 lines · 3098 bytes

SHA-256: `901e8428ef203d01a9c2f33ded9cb58802a78ad8c135f84a019290eeb15ffe51`

Source annotations: Existing idempotent submission binds device, objective and run identity. A worker may select only runs with a successfully stored proposal. If the second transaction fails, the same request safely resumes proposal storage.

Imports: [controller_requests](../../app/lifecycle/controller_requests.ep), [../rights/inventory](../../app/rights/inventory.ep), [../web/run_store](../../app/web/run_store.ep).

Definitions:

```text
5: define controller_proposal_submit with database as Int and charter as Str and inventory as Str and inventory_digest as Str and credential as Str and identity as Str and manifest as Str returning Str:
29: define controller_proposal_dispatch with database as Int and charter as Str and inventory as Str and digest as Str and tokens as List and credential as Str returning Str:
```

## `app/lifecycle/controller_requests.ep`

[app/lifecycle/controller_requests.ep](../../app/lifecycle/controller_requests.ep) · 26 lines · 1755 bytes

SHA-256: `ebc2328e6bb1bae540e10533ac44f1fd1b5b9da38e64b94fa12d54f984e799e2`

Source annotations: The service supplies its already-open private ledger and pinned inputs. No request can choose a database, charter, inventory or policy, or initialize state.

Imports: [../rights/operation_rollback](../../app/rights/operation_rollback.ep).

Definitions:

```text
5: define controller_authenticated_operation with database as Int and charter as Str and inventory as Str and inventory_digest as Str and policy as Str and action as Str and identity as Str and credential as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep).

## `app/lifecycle/controller_review.ep`

[app/lifecycle/controller_review.ep](../../app/lifecycle/controller_review.ep) · 145 lines · 9268 bytes

SHA-256: `23a33fefb56702e92b0694c3aaee53c3a533e8e1aa00a38fd45e9f4e121ff46e`

Source annotations: The steward sees exact proposal and Echo review bytes from the controller's own ledger. This read grants no consent and records no human decision.

Imports: [controller_requests](../../app/lifecycle/controller_requests.ep).

Definitions:

```text
3: define controller_review_status with status as Str returning Str:
10: define controller_steward_disclosure with database as Int and credential as Str and identity as Str returning Str:
58: define controller_review_dispatch with database as Int and tokens as List and credential as Str returning Str:
97: define controller_operation_disclosure with database as Int and credential as Str and identity as Str returning Str:
```

## `app/lifecycle/controller_review_agent.ep`

[app/lifecycle/controller_review_agent.ep](../../app/lifecycle/controller_review_agent.ep) · 155 lines · 9347 bytes

SHA-256: `b3486698c19af7ba7df9ec1af69da310ef09123056cacc1db0cee49652f73b75`

Source annotations: This worker advertises only inspection and review. The supplied messages are retained verbatim; identity, philosophy and provider capacities are unchanged. A separate dispatcher is intentional: a privileged review worker must not link model-selected names to the general runtime tool dispatcher. Its only effects are proposal inspection, review receipts and normal tool observations. Do not advance another queued action past an unresolved earlier effect.

Imports: [../agent/generation](../../app/agent/generation.ep), [../agent/run_messages](../../app/agent/run_messages.ep), [../tools/protected_tools](../../app/tools/protected_tools.ep).

Definitions:

```text
7: define controller_review_request_body with model as Str and messages as Str returning Str:
19: define controller_review_tool_step with database as Int and run as Str returning Int:
84: define controller_review_finish_call with database as Int and run as Str and call_id as Str and request_event as Str returning Int:
119: define controller_review_recover_call with database as Int and run as Str and call_id as Str returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep).

## `app/lifecycle/controller_review_inputs.ep`

[app/lifecycle/controller_review_inputs.ep](../../app/lifecycle/controller_review_inputs.ep) · 103 lines · 6465 bytes

SHA-256: `1b98185362457b2c2d2b192718880f086d162ea87c7af151c44c5449bcc0c9eb`

Source annotations: Model/prompt inputs come from the installed host configuration, never from proposal text, HTTP parameters or inherited environment variables.

Imports: [controller_operations](../../app/lifecycle/controller_operations.ep), [../agent/provider_profile](../../app/agent/provider_profile.ep), [../agent/run_messages](../../app/agent/run_messages.ep), [../context/session_messages](../../app/context/session_messages.ep).

Definitions:

```text
8: define controller_review_inputs_load with configuration as Str returning List:
56: define controller_review_inputs_bind with database as Int and run as Str and profile as Str and prompt as Str returning Str:
83: define controller_review_bound_messages with database as Int and run as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep).

## `app/lifecycle/controller_review_runner.ep`

[app/lifecycle/controller_review_runner.ep](../../app/lifecycle/controller_review_runner.ep) · 90 lines · 6096 bytes

SHA-256: `536905c9b4cf290f00e926f3a587e2b856a4c58ec8b86c4ab37d978c9f27b1d7`

Source annotations: Scheduler candidates must have a stored proposal; an interrupted submission without its proposal is never sent to the model. The cursor is not a limit. Caller holds the native run-owner lock. Recovery never infers a dead worker from elapsed time. Provider discovery supplies capacities and exact counting.

Imports: [controller_review_agent](../../app/lifecycle/controller_review_agent.ep), [controller_review_inputs](../../app/lifecycle/controller_review_inputs.ep), [../context/compact](../../app/context/compact.ep), [../agent/generation_recovery](../../app/agent/generation_recovery.ep).

Environment names: `ECHO_CONTROLLER_REVIEW_RUN`.

Definitions:

```text
8: define controller_review_candidate with database as Int and after_sequence as Int returning List:
15: define controller_review_loop_owned with database as Int and run as Str and profile as Str and prompt as Str and reserve as Int and cancellation as Int and batch as Int and library as Int returning Str:
64: define controller_review_run with database as Int and run as Str and profile as Str and prompt as Str and reserve as Int and cancellation as Int and batch as Int returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep).

## `app/lifecycle/controller_service.ep`

[app/lifecycle/controller_service.ep](../../app/lifecycle/controller_service.ep) · 161 lines · 8140 bytes

SHA-256: `b47aa79f367c36d1f7d8a726a492988bc0c8c20bf710ad43e9e19fb7ddffcf77`

Source annotations: Startup never repairs or initializes an incomplete bootstrap. The explicit OS-authenticated initializer must have completed under this exact config.

Imports: [controller_operations](../../app/lifecycle/controller_operations.ep), [controller_review_runner](../../app/lifecycle/controller_review_runner.ep), [controller_http](../../app/lifecycle/controller_http.ep), [controller_process](../../app/lifecycle/controller_process.ep), [../transport/cooperative_accept](../../app/transport/cooperative_accept.ep).

Definitions:

```text
9: define controller_service_ready with database as Int and configuration_digest as Str returning Int:
19: define controller_service_run with configuration as Str and connection_pin as Str returning Int:
117: define controller_review_host with configuration as Str and run as Str and expected_pin as Str returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep).

## `app/lifecycle/migrations.ep`

[app/lifecycle/migrations.ep](../../app/lifecycle/migrations.ep) · 67 lines · 4895 bytes

SHA-256: `f91c3d349d006d0e295fb61639c31af7ce7f18726992300b4a51e1c1c6684052`

Source annotations: Host-authored SQL from a pinned executable only, never browser/model input. DDL and receipt commit together. Failed candidates never change live history.

Imports: [../archive/history](../../app/archive/history.ep), [../context/checkpoint](../../app/context/checkpoint.ep).

Definitions:

```text
6: define migration_fail with database as Int and reason as Str returning Str:
11: define migration_apply with database as Int and version as Int and name as Str and statements as Str returning Str:
54: define migration_compatible with database as Int and supported as Int returning Int:
```

Lexical test references: [tests/lifecycle/test_migration_interruption.ep](../../tests/lifecycle/test_migration_interruption.ep), [tests/lifecycle/test_migrations.ep](../../tests/lifecycle/test_migrations.ep), [tests/lifecycle/test_startup.ep](../../tests/lifecycle/test_startup.ep), [tests/lifecycle/test_startup_busy.ep](../../tests/lifecycle/test_startup_busy.ep).

## `app/lifecycle/operations.ep`

[app/lifecycle/operations.ep](../../app/lifecycle/operations.ep) · 100 lines · 7833 bytes

SHA-256: `cd35e53134283d1800010b0bd5e3bcb2781ba1a4050987dfbd5cdd21e4ae8573`

Source annotations: Scope and principal are resolved by the trusted caller, never claimed by a tool. Store diagnostic codes and artifact references, not credentials/prompts. Every state is an immutable receipt. There is no mutable status projection that can commit without its matching audit record. status, principal, kind, request_digest, revision, state, stage, cause, effects, diagnostic reference, recovery action, result reference.

Imports: [migrations](../../app/lifecycle/migrations.ep).

Definitions:

```text
7: define operation_schema with database as Int returning Str:
13: define operation_read with database as Int and scope as Str and identity as Str returning List:
19: define operation_reserve with database as Int and scope as Str and identity as Str and principal as Str and kind as Str and request as Str returning Str:
54: define operation_transition with database as Int and scope as Str and identity as Str and revision as Int and state as Str and stage as Str and cause as Str and effects as Str and diagnostic as Str and recovery as Str and result as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_operations.ep](../../tests/lifecycle/test_operations.ep).

## `app/lifecycle/protected_path.ep`

[app/lifecycle/protected_path.ep](../../app/lifecycle/protected_path.ep) · 89 lines · 4414 bytes

SHA-256: `c4454c46c17308e1bc8b224cc16b151ffbf4d010372d332fd4552c86b79fb8da`

Source annotations: Darwin arm64 ABI inspection of an already-open inode. Controller installation uses root-owned paths with no extended ACL grants and no group/other writes. Reject an unreadable ACL rather than interpreting it as an empty ACL. uid_t occupies bytes16..19 in the installed Darwin struct stat.

Imports: [../workspace/files](../../app/workspace/files.ep).

Definitions:

```text
6: define protected_inode_owned with library as Int and descriptor as Int and directory as Int returning Int:
56: define protected_path_open with library as Int and path as Str and directory as Int returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep), [tests/lifecycle/test_controller_ledger.ep](../../tests/lifecycle/test_controller_ledger.ep), [tests/lifecycle/test_protected_path.ep](../../tests/lifecycle/test_protected_path.ep).

## `app/lifecycle/service_profile.ep`

[app/lifecycle/service_profile.ep](../../app/lifecycle/service_profile.ep) · 140 lines · 7980 bytes

SHA-256: `a2ad9c9e762569ab80c35ebd47c3feb138e3efa9876f5b762253aada096487f6`

Source annotations: Host configuration only. This is not a browser or model execution endpoint. Exec replaces the launcher, so the caller owns the actual service PID and receives its exit status and signals without an unsupervised wrapper child.

Imports: [../providers/json_access](../../app/providers/json_access.ep), [../workspace/directories](../../app/workspace/directories.ep).

Definitions:

```text
7: define service_native_text with text as Str returning Int:
15: define service_profile_valid with tokens as List returning Int:
65: define service_profile_vector with tokens as List and name as Str returning List:
79: define service_runtime_ready with tokens as List and library as Int returning Int:
106: define service_profile_run with profile as Str and check_only as Int returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep), [tests/lifecycle/test_guardian_readiness.ep](../../tests/lifecycle/test_guardian_readiness.ep), [tests/lifecycle/test_service.ep](../../tests/lifecycle/test_service.ep).

## `app/lifecycle/startup.ep`

[app/lifecycle/startup.ep](../../app/lifecycle/startup.ep) · 34 lines · 1587 bytes

SHA-256: `b6d9fc10b2375874f6f97accab84b190519ec8b8c2882fe9fd7b8917d1170371`

Source annotations: Inspect before archive_open can create or update any application schema. Deployment must stop old processes before migrations; this is not a live migration lock or permission to run mixed application versions. Distinguish a transient database lock from a proven schema incompatibility. -1 never permits writes; it tells the process supervisor this is retryable. Match normal archive access: a short writer lock is not a schema change.

Imports: [migrations](../../app/lifecycle/migrations.ep), `fs` (toolchain/library).

Definitions:

```text
9: define startup_archive_status with path as Str and supported as Int returning Int:
33: define startup_archive_compatible with path as Str and supported as Int returning Int:
```

Lexical test references: [tests/lifecycle/test_startup.ep](../../tests/lifecycle/test_startup.ep), [tests/lifecycle/test_startup_busy.ep](../../tests/lifecycle/test_startup_busy.ep).

## `app/maintenance/protocol_repair.ep`

[app/maintenance/protocol_repair.ep](../../app/maintenance/protocol_repair.ep) · 33 lines · 3644 bytes

SHA-256: `9c354e4d4833992fdf90c9b21b18da46bc314402791cb5d3c3ff136ef16ae978`

Source annotations: Host maintenance only: reopen one proven malformed legacy final, without changing or deleting any original event or replaying any tool action.

Imports: [../web/worker](../../app/web/worker.ep).

Definitions:

```text
5: define recover_legacy_protocol with database as Int and run as Str and expected as Str and recovery as Str and worker as Str returning Int:
```

Lexical test references: [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep).

## `app/maintenance/recover_tool_protocol.ep`

[app/maintenance/recover_tool_protocol.ep](../../app/maintenance/recover_tool_protocol.ep) · 13 lines · 603 bytes

SHA-256: `71bcaa345566a86715335b953aa6038d8c1f6735f41c6bd83fc50d67b7012bb7`

Imports: [protocol_repair](../../app/maintenance/protocol_repair.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/media/image_logs.ep`

[app/media/image_logs.ep](../../app/media/image_logs.ep) · 44 lines · 2057 bytes

SHA-256: `488873de100cca86d92fc43e9878c04261583439aec8c32769ff39f7a25fbdaa`

Source annotations: Called only after the helper is reaped. The original host-owned descriptor stays open; no pathname is reopened. Pages are transfer sizes, not quotas. Immutable process_output events travel with database snapshots and can be read through the existing session-scoped output cursor and history tools.

Imports: [../workspace/output_archive](../../app/workspace/output_archive.ep), [../workspace/files](../../app/workspace/files.ep).

Definitions:

```text
4: external define ep_uuid_v4
10: define image_log_archive with database as Int and run as Str and call_id as Str and library as Int and descriptor as Int and stream as Str returning Int:
```

Lexical test references: [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep).

## `app/media/image_request.ep`

[app/media/image_request.ep](../../app/media/image_request.ep) · 110 lines · 5692 bytes

SHA-256: `3050f510163f61ad5519f6e9a84980a4856c512f6c14e2cd7f541001fbc88c7e`

Source annotations: Host profile owns the executable, weights and native argument template. Model text can fill named values only; it cannot select paths or add flags.

Imports: [../retained_text](../../app/retained_text.ep), [../providers/json_tokens](../../app/providers/json_tokens.ep), [../providers/json_string](../../app/providers/json_string.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
8: define image_request_arguments with profile as Str and arguments as Str returning List:
```

Lexical test references: [tests/media/test_image_request.ep](../../tests/media/test_image_request.ep).

## `app/media/image_tool.ep`

[app/media/image_tool.ep](../../app/media/image_tool.ep) · 169 lines · 10951 bytes

SHA-256: `d21f4ea50ec1b94d758b98e4d62a5c89f69ecb38adf42beb0b240497804dbdd6`

Source annotations: Run/call identity comes from the dispatcher. Re-read the immutable request.

Imports: [shared_authority](../../app/media/shared_authority.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [image_request](../../app/media/image_request.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [image_logs](../../app/media/image_logs.ep), [../web/attachments](../../app/web/attachments.ep), [../workspace/directories](../../app/workspace/directories.ep), [../providers/helper_process](../../app/providers/helper_process.ep).

Environment names: `ECHO_IMAGE_PROFILE`, `ECHO_IMAGE_ROOT`.

Definitions:

```text
12: define image_tool_authority with database as Int and run as Str and call_id as Str returning List:
29: define image_tool_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:
55: define image_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:
```

Lexical test references: [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep).

## `app/media/shared_authority.ep`

[app/media/shared_authority.ep](../../app/media/shared_authority.ep) · 37 lines · 3509 bytes

SHA-256: `8d47a26f356cc1d2b15770d6fdb496c94a7999e5a97802a39aa7a842a4ed2b12`

Source annotations: Resolve the initiating device from the frozen call, never from media arguments or another member of the room. Publication checks this identity again.

Imports: [../security/tool_policy](../../app/security/tool_policy.ep), [../web/attachments](../../app/web/attachments.ep).

Definitions:

```text
6: define media_shared_authority with database as Int and run as Str and call_id as Str and family as Str returning List:
34: define media_attachment_register with database as Int and device as Str and session as Str and identity as Str and path as Str and name as Str and media_type as Str and run as Str and call_id as Str returning Int:
```

Lexical test references: [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep).

## `app/media/speech_request.ep`

[app/media/speech_request.ep](../../app/media/speech_request.ep) · 126 lines · 8539 bytes

SHA-256: `a16d0e8c9ca419b008ac59a8271503e05e23afee52af145faf439844f57e575b`

Source annotations: This adapter accepts only an owned archive identity and spoken language. Runtime paths, model weights and output filenames are host controlled.

Imports: [../web/attachments](../../app/web/attachments.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [../tools/history_tools](../../app/tools/history_tools.ep), [../archive/session_scope](../../app/archive/session_scope.ep), [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep).

Definitions:

```text
10: define speech_request_source with database as Int and session as Str and arguments as Str returning List:
13: define speech_request_source_device with database as Int and device as Str and session as Str and arguments as Str returning List:
74: define speech_request_arguments with profile as Str and language as Str returning List:
125: define speech_tool_schema returning Str:
```

Lexical test references: [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep).

## `app/media/speech_text.ep`

[app/media/speech_text.ep](../../app/media/speech_text.ep) · 93 lines · 5139 bytes

SHA-256: `96b3d5a6c5ba1435609b6ffdfe085f3fc4062032d8c7f063b402ccdbc8daeaf6`

Source annotations: Speech is a derived presentation. The original archived message stays intact. Markdown structure is silent; link labels and inline code remain readable.

Imports: [../providers/json_string](../../app/providers/json_string.ep).

Definitions:

```text
5: define speech_plain_text with text as Str returning Str:
```

Lexical test references: [tests/media/test_speech_text.ep](../../tests/media/test_speech_text.ep).

## `app/media/speech_tool.ep`

[app/media/speech_tool.ep](../../app/media/speech_tool.ep) · 249 lines · 17038 bytes

SHA-256: `92a6131ed315139121f00674b5cc12f4dc3d91f6cfc9e15496a09233ca2095c2`

Source annotations: Run/call identity comes from the dispatcher. Re-read the immutable request.

Imports: [shared_authority](../../app/media/shared_authority.ep), [../security/principals](../../app/security/principals.ep), [speech_text](../../app/media/speech_text.ep), [../security/tool_authority](../../app/security/tool_authority.ep), [../retained_text](../../app/retained_text.ep), [speech_request](../../app/media/speech_request.ep), [voice_request](../../app/media/voice_request.ep), [../archive/file_copy](../../app/archive/file_copy.ep), [image_logs](../../app/media/image_logs.ep), [../web/attachments](../../app/web/attachments.ep), [../workspace/directories](../../app/workspace/directories.ep), [../providers/helper_process](../../app/providers/helper_process.ep).

Environment names: `ECHO_SPEECH_PROFILE`, `ECHO_SPEECH_ROOT`, `ECHO_VOICE_PROFILE`.

Definitions:

```text
15: define speech_tool_authority with database as Int and run as Str and call_id as Str returning List:
39: define speech_tool_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:
65: define speech_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:
```

Lexical test references: [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep).

## `app/media/video_library.ep`

[app/media/video_library.ep](../../app/media/video_library.ep) · 90 lines · 8015 bytes

SHA-256: `acae5ddd54e1a9b44d83397e1f5026bd3de1b4dde048dfe20ef096cdadb1f7aa`

Source annotations: Library grants are host configuration for explicitly paired local devices. A role, linked Discord account, guessed item ID or source text grants nothing.

Imports: [../tools/attach_file](../../app/tools/attach_file.ep), [../security/tool_policy](../../app/security/tool_policy.ep).

Environment names: `ECHO_ATTACHMENT_ROOT`, `ECHO_VIDEO_LIBRARY_DEVICES`, `ECHO_VIDEO_LIBRARY_ROOT`.

Definitions:

```text
6: define video_library_allowed with database as Int and device as Str and session as Str returning Int:
19: define video_library_request with database as Int and device as Str and session as Str and arguments as Str and run as Str and call_id as Str returning Str:
81: define video_library_tool with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
89: define video_library_schema returning Str:
```

Lexical test references: [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep).

## `app/media/video_request.ep`

[app/media/video_request.ep](../../app/media/video_request.ep) · 121 lines · 10695 bytes

SHA-256: `07d8a1ceacb4de852fa7c05dbe55a4c3d4483740771288e05ddad8c0b30f83b7`

Source annotations: One requested timestamp is one disclosed sample, never a claim of full video comprehension. No hidden frame-count/duration or output-resolution ceiling.

Imports: [../retained_text](../../app/retained_text.ep), [../tools/history_tools](../../app/tools/history_tools.ep), [../web/attachments](../../app/web/attachments.ep).

Environment names: `ECHO_VIDEO_SEGMENT_MS`.

Definitions:

```text
7: define video_request with arguments as Str returning List:
41: define video_request_source with database as Int and device as Str and session as Str and identity as Str returning List:
53: define video_request_arguments with request as List returning List:
71: define video_seconds with milliseconds as Int returning Str:
79: define video_watch_allowed with database as Int and session as Str and request as List returning Int:
104: define video_sandbox_profile returning Str:
107: define video_tool_schema returning Str:
112: define video_bound_arguments with database as Int and run as Str and arguments as Str returning Str:
```

Lexical test references: [tests/media/test_video_request.ep](../../tests/media/test_video_request.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep).

## `app/media/video_tool.ep`

[app/media/video_tool.ep](../../app/media/video_tool.ep) · 245 lines · 18324 bytes

SHA-256: `e143dfab4f86a95ea01dec4d0744232a6863628967edc4b448934accd39ccac8`

Source annotations: Run/call identity comes from the dispatcher. Re-read the immutable request. Malformed arguments are recoverable input errors, not revoked authority.

Imports: [shared_authority](../../app/media/shared_authority.ep), [../security/principals](../../app/security/principals.ep), [video_request](../../app/media/video_request.ep), [../security/tool_authority](../../app/security/tool_authority.ep), [../retained_text](../../app/retained_text.ep), [../archive/file_copy](../../app/archive/file_copy.ep), [image_logs](../../app/media/image_logs.ep), [../web/attachments](../../app/web/attachments.ep), [../workspace/directories](../../app/workspace/directories.ep), [../providers/helper_process](../../app/providers/helper_process.ep).

Environment names: `ECHO_VIDEO_PROFILE`, `ECHO_VIDEO_ROOT`.

Definitions:

```text
13: define video_tool_authority with database as Int and run as Str and call_id as Str returning List:
47: define video_tool_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:
73: define video_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:
```

Lexical test references: [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep).

## `app/media/voice_request.ep`

[app/media/voice_request.ep](../../app/media/voice_request.ep) · 109 lines · 6189 bytes

SHA-256: `88394aadccfbbb2330655f4216c448d4ef4ac2a5fe96c53e3bac9b75384eaae2`

Source annotations: Text is written to a private input file, never interpolated into a command.

Imports: [../retained_text](../../app/retained_text.ep), [../providers/json_tokens](../../app/providers/json_tokens.ep), [../providers/json_string](../../app/providers/json_string.ep).

Definitions:

```text
6: define voice_request_text with arguments as Str returning Str:
30: define voice_request_arguments with profile as Str returning List:
108: define voice_tool_schema returning Str:
```

Lexical test references: [tests/media/test_enrolled_voice.ep](../../tests/media/test_enrolled_voice.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep).

## `app/memory/archive_recall.ep`

[app/memory/archive_recall.ep](../../app/memory/archive_recall.ep) · 82 lines · 6218 bytes

SHA-256: `04b9470cd9198c0745511a525f6585eef753ba42423a4977a3af9f890e7da8f3`

Source annotations: Resolve only the initiating run's own private principal. Neither a requested principal nor an administrator role selects another person's memory. A fixed boundary makes a scan finite without dropping older conversations.

Imports: [session_ownership](../../app/memory/session_ownership.ep), [../archive/history](../../app/archive/history.ep).

Definitions:

```text
6: define memory_archive_principal with database as Int and run as Str returning Str:
20: define memory_archive_boundary with database as Int and run as Str returning List:
33: define memory_archive_next with database as Int and run as Str and literal as Str and after_sequence as Int and through_sequence as Int returning List:
51: define memory_archive_read with database as Int and run as Str and event as Str and offset as Int and characters as Int returning List:
70: define memory_principal_scope_sql with database as Int returning Str:
76: define memory_scope_query with database as Int and query as Str returning Str:
80: define memory_linked_attachment_sql with database as Int returning Str:
```

Lexical test references: [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep).

## `app/memory/automatic.ep`

[app/memory/automatic.ep](../../app/memory/automatic.ep) · 363 lines · 26578 bytes

SHA-256: `74d4c8ddfde77bde78be10c8ae0e0df5964451b04f7db5223566efdf99f13eed`

Source annotations: Private, incremental extraction. Only accepted conversation events before the current objective are eligible. Receipts prevent repeated ingestion after a restart; forgetting an extracted assertion does not cause it to be relearnt.

Imports: [../context/compaction_lock](../../app/context/compaction_lock.ep), [../context/echo_labels](../../app/context/echo_labels.ep), [knowledge](../../app/memory/knowledge.ep), [group_ingestion](../../app/memory/group_ingestion.ep), [../agent/control](../../app/agent/control.ep), [../providers/prepare](../../app/providers/prepare.ep), [../providers/local_chat](../../app/providers/local_chat.ep).

Definitions:

```text
12: define memory_pending with database as Int and run as Str returning List:
35: define memory_source_text with row as List returning Str:
43: define memory_extracted_date with database as Int and tokens as List and item as Int and quote as Str returning Str:
61: define memory_ground_extraction with database as Int and run as Str and sources as List and start as Int and count as Int and response as Str returning Str:
145: define memory_extract_body with database as Int and model as Str and sources as List and start as Int and count as Int returning Str:
169: define memory_extract_save with database as Int and run as Str and sources as List and start as Int and count as Int and response as Str returning Str:
251: define memory_prepare with database as Int and run as Str and profile as Str and reserve as Int and cancellation as Int and read_size as Int returning Str:
273: define memory_prepare_owned with database as Int and run as Str and profile as Str and reserve as Int and cancellation as Int and read_size as Int returning Str:
344: define memory_pending_context_after with database as Int and run as Str and previous as Str returning Str:
362: define memory_pending_context with database as Int and run as Str returning Str:
```

Lexical test references: [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep).

## `app/memory/automatic_context.ep`

[app/memory/automatic_context.ep](../../app/memory/automatic_context.ep) · 79 lines · 9189 bytes

SHA-256: `4012c63db9c8306c6cd34e9ecc4a16f9809893adf0881d4f8a89f3f7cda18a0d`

Source annotations: Factor repeated provenance out of each quotation without summarising text. No record count or text-length cap. JSON preserves Unicode and control chars. Derived memory remains data in a user-role context record. It cannot grant permissions or replace the installed identity. Visibility and ownership are rechecked on every reconstruction, including after forget and revocation.

Imports: [source_provenance](../../app/memory/source_provenance.ep), [knowledge](../../app/memory/knowledge.ep), [automatic](../../app/memory/automatic.ep), [group_context](../../app/memory/group_context.ep), [recall_selection](../../app/memory/recall_selection.ep).

Definitions:

```text
9: define memory_compact_records with database as Int and claims as Str returning Str:
18: define memory_context_selected with database as Int and run as Str and selected as Str returning Str:
63: define memory_extraction_status with database as Int and run as Str returning Str:
75: define memory_context_after with database as Int and run as Str and previous as Str returning Str:
78: define memory_context with database as Int and run as Str returning Str:
```

Lexical test references: [tests/context/test_memory_provenance_snapshot.ep](../../tests/context/test_memory_provenance_snapshot.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_linked_recall_existing.ep](../../tests/security/test_linked_recall_existing.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_selected_recall_existing.ep](../../tests/security/test_selected_recall_existing.ep).

## `app/memory/context_snapshot.ep`

[app/memory/context_snapshot.ep](../../app/memory/context_snapshot.ep) · 169 lines · 14438 bytes

SHA-256: `6077b6b49262d256b58844786ad82b3a2787ecff8d32d5744c314a9efb2d0e0e`

Source annotations: Freeze a session base and retain exact per-turn additions in history. New turns select fresh evidence without rewriting the retained prefix. Visibility, access, corrections, expiry and runtime changes invalidate it.

Imports: [automatic_context](../../app/memory/automatic_context.ep).

Environment names: `ECHO_EMBEDDING_PROFILE`.

Definitions:

```text
6: define memory_context_revision with database as Int and run as Str returning Str:
88: define memory_context_capture with database as Int and run as Str returning Int:
145: define memory_context_stable with database as Int and run as Str returning Str:
163: define memory_context_delta with database as Int and current_run as Str and source_run as Str and revision as Str returning Str:
```

Lexical test references: [tests/context/test_memory_provenance_snapshot.ep](../../tests/context/test_memory_provenance_snapshot.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep).

## `app/memory/corrections.ep`

[app/memory/corrections.ep](../../app/memory/corrections.ep) · 54 lines · 6007 bytes

SHA-256: `232111b4ad1c413c01243bfabfe896da2ce7189f189fb1c922c220141b6e1ab5`

Source annotations: Correction links preserve originals. Supersession invalidates every derivative of the corrected source, not just the one assertion named in the graph. Both endpoints must be readable by this principal. A link grants no access.

Imports: [archive_recall](../../app/memory/archive_recall.ep).

Definitions:

```text
5: define memory_correction_links_sql returning Str:
8: define memory_correction_sources_sql returning Str:
12: define memory_corrections with database as Int and run as Str and source as Str returning Str:
27: define memory_correction_wrap with database as Int and run as Str and source as Str and output as Str returning Str:
38: define memory_correction_record with database as Int and run as Str and source as Str and correction as Str returning Int:
```

Lexical test references: [tests/security/record_source_correction.ep](../../tests/security/record_source_correction.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep).

## `app/memory/episode_order.ep`

[app/memory/episode_order.ep](../../app/memory/episode_order.ep) · 5 lines · 529 bytes

SHA-256: `baf9ec4c8d0ee14ea547142f9b609aef19d757021fa94c01bee6422ddfade972`

Source annotations: The ordering timestamp is explicitly not an assertion about an undated experience. Preserve the reported time and source time separately.

Imports: none.

Definitions:

```text
3: define episode_order_time_sql returning Str:
```

## `app/memory/episodes.ep`

[app/memory/episodes.ep](../../app/memory/episodes.ep) · 78 lines · 6314 bytes

SHA-256: `27f85bfb10326d08d7178849e1e913781c570ce73528e7eabdc7e74dcd386f3c`

Source annotations: Cursor is tied to principal and filters and freezes the ingestion boundary. Visibility and source ownership are still checked on every page.

Imports: [knowledge](../../app/memory/knowledge.ep), [episode_order](../../app/memory/episode_order.ep), [group_episodes](../../app/memory/group_episodes.ep).

Definitions:

```text
8: define episode_page with database as Int and run as Str and cursor as Str and entries as Int and category as Str returning Str:
```

Lexical test references: [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep).

## `app/memory/group_context.ep`

[app/memory/group_context.ep](../../app/memory/group_context.ep) · 44 lines · 4498 bytes

SHA-256: `26a0154a05082a1a004d13f7fae41044a3d79e90a44d60f19bfa96d8b98fb2cc`

Source annotations: Shared recall is data from the exact recorded audience. A source must predate this objective; superseded, expired and forgotten assertions stay out of recall.

Imports: [group_knowledge](../../app/memory/group_knowledge.ep).

Definitions:

```text
5: define group_memory_context with database as Int and run as Str returning Str:
```

## `app/memory/group_episodes.ep`

[app/memory/group_episodes.ep](../../app/memory/group_episodes.ep) · 74 lines · 7377 bytes

SHA-256: `a2df6a875e004d7e17a19310843a8473076467c990fd19ee3e495fc3093d44ae`

Source annotations: Cursor is tied to principal and filters and freezes the ingestion boundary. Visibility and source ownership are still checked on every page.

Imports: [group_knowledge](../../app/memory/group_knowledge.ep), [episode_order](../../app/memory/episode_order.ep).

Definitions:

```text
6: define group_episode_page with database as Int and run as Str and cursor as Str and entries as Int and category as Str returning Str:
```

## `app/memory/group_ingestion.ep`

[app/memory/group_ingestion.ep](../../app/memory/group_ingestion.ep) · 50 lines · 3725 bytes

SHA-256: `742e7a57f44219d52ca8888127975c6156f23bb579a04b7bf996878fe21503c9`

Source annotations: Shared turns already remain in the active conversation. Extract originals displaced by an activated recap, rather than adding another model request to every ordinary shared turn. Receipts bind the exact audience and source digest.

Imports: [group_knowledge](../../app/memory/group_knowledge.ep).

Definitions:

```text
6: define group_memory_pending with database as Int and run as Str returning List:
35: define memory_extraction_read with database as Int and run as Str and event as Str returning List:
46: define group_memory_speaker with database as Int and event as Str returning Str:
```

## `app/memory/group_knowledge.ep`

[app/memory/group_knowledge.ep](../../app/memory/group_knowledge.ep) · 160 lines · 14316 bytes

SHA-256: `9b5c525d1022d296615e067da767bf0782dd55fca1979f1df61c6a3d12a20e1f`

Source annotations: Group assertions belong to an immutable audience, not to the speaker's private principal. Membership and source-history authority remain live checks.

Imports: [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [../projects/scope](../../app/projects/scope.ep), [../context/runtime_binding](../../app/context/runtime_binding.ep), [../security/group_history](../../app/security/group_history.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
9: define group_memory_scope with database as Int and run as Str returning Str:
15: define group_memory_abort with database as Int and status as Str returning Str:
19: define group_memory_source_allowed with database as Int and run as Str and event as Str and digest as Str returning Int:
30: define group_memory_lookup with database as Int and run as Str and identity as Str returning List:
45: define group_memory_record with database as Int and run as Str and kind as Str and subject as Str and predicate as Str and object as Str and evidence as Str and source_event as Str and source_digest as Str and event_time as Str and valid_until as Str and supersedes as Str returning Str:
97: define group_memory_visibility_read with database as Int and run as Str and claim as Str returning List:
103: define group_memory_visibility_set with database as Int and run as Str and change as Str and claim as Str and previous_revision as Int and state as Str and reason as Str returning Str:
134: define group_memory_next with database as Int and run as Str and node as Str and after_sequence as Int returning List:
153: define memory_source_evidence_kind with kind as Str and payload as Str returning Str:
```

Lexical test references: [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep).

## `app/memory/knowledge.ep`

[app/memory/knowledge.ep](../../app/memory/knowledge.ep) · 149 lines · 13758 bytes

SHA-256: `9f336b19d9131e0af81bb110323062c7bdc3c5475522d572904d512debc60411`

Source annotations: Facts and episodes are derived assertions. The immutable source remains the evidence; repeating a model inference never converts it into an observation. This primitive accepts already parsed fields from a trusted caller. Source ownership, evidence class and correction lineage are still enforced here. Empty event_time means unknown, not the time the system ingested the record.

Imports: [corrections](../../app/memory/corrections.ep), [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [archive_recall](../../app/memory/archive_recall.ep), [group_knowledge](../../app/memory/group_knowledge.ep), [../context/runtime_binding](../../app/context/runtime_binding.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
10: define knowledge_schema with database as Int returning Str:
16: define knowledge_abort with database as Int and status as Str returning Str:
24: define knowledge_record with database as Int and run as Str and kind as Str and subject as Str and predicate as Str and object as Str and evidence as Str and source_event as Str and source_digest as Str and event_time as Str and valid_until as Str and supersedes as Str returning Str:
124: define knowledge_next with database as Int and run as Str and node as Str and after_sequence as Int returning List:
142: define knowledge_visible_sql with database as Int returning Str:
```

Lexical test references: [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep).

## `app/memory/knowledge_lifecycle.ep`

[app/memory/knowledge_lifecycle.ep](../../app/memory/knowledge_lifecycle.ep) · 66 lines · 5752 bytes

SHA-256: `f094c721dce7191505596d3927ab2223c7b60c615a8b7591d0ed5f8c56519362`

Source annotations: Forgetting removes a derived assertion from ordinary recall. Original source events and the audit remain intact; restoration is a new explicit transition. Management lookup is explicit by identity. It returns state/revision only, so a forgotten assertion does not reappear as recalled content. CAS prevents an old forget/restore request from undoing a newer decision. Exact retries return the prior receipt without reapplying its state.

Imports: [knowledge](../../app/memory/knowledge.ep).

Definitions:

```text
5: define knowledge_lifecycle_schema with database as Int returning Str:
13: define knowledge_visibility_read with database as Int and run as Str and claim as Str returning List:
27: define knowledge_visibility_set with database as Int and run as Str and change as Str and claim as Str and previous_revision as Int and state as Str and reason as Str returning Str:
```

Lexical test references: [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep).

## `app/memory/procedures.ep`

[app/memory/procedures.ep](../../app/memory/procedures.ep) · 235 lines · 18829 bytes

SHA-256: `3707d38919c952c93e50f4e0ca43483d5487252ad0b08eb0fc2f16cefb3f70a7`

Source annotations: Dependencies use the same exact schema and effect/recovery contract as tool discovery. A retained procedure never makes a missing tool available. Evidence is an observed ledger result, not an arbitrary archive quotation. Every asserted output is checked against the original result with its digest. All demonstrated steps must occur in order in one execution run.

Imports: [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [../projects/manifest](../../app/projects/manifest.ep), [../tools/tool_contract](../../app/tools/tool_contract.ep).

Environment names: `ECHO_RUNTIME_BUILD_SHA256`.

Definitions:

```text
7: define procedure_tool_digest with schemas as Str and name as Str returning Str:
22: define procedure_assess with database as Int and run as Str and manifest as Str and schemas as Str returning Str:
101: define procedure_save with database as Int and run as Str and event as Str and manifest as Str and schemas as Str returning Str:
145: define procedure_read with database as Int and run as Str and event as Str and schemas as Str returning Str:
175: define procedure_search with database as Int and run as Str and query as Str and after as Int and schemas as Str returning Str:
193: define procedure_visibility with database as Int and event as Str returning List:
199: define procedure_visibility_set with database as Int and run as Str and change as Str and event as Str and previous as Int and state as Str and reason as Str returning Str:
```

Lexical test references: [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_existing.ep](../../tests/memory/test_procedure_existing.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep).

## `app/memory/recall_candidates.ep`

[app/memory/recall_candidates.ep](../../app/memory/recall_candidates.ep) · 22 lines · 2606 bytes

SHA-256: `c92a74b28b8f6c9e6079531d23c10f16a3676b37ce370a157dceee59fdaa50fd`

Imports: [source_provenance](../../app/memory/source_provenance.ep), [knowledge](../../app/memory/knowledge.ep), [corrections](../../app/memory/corrections.ep).

Definitions:

```text
5: define memory_recall_claims with database as Int and run as Str returning Str:
```

Lexical test references: [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep).

## `app/memory/recall_selection.ep`

[app/memory/recall_selection.ep](../../app/memory/recall_selection.ep) · 152 lines · 11971 bytes

SHA-256: `947d3c30c1df61cef032b0e0728a18fb974dc62687d5a79aed9e9ca6a1f15ebf`

Source annotations: Derived local cache only: source ownership, visibility and correction filters are applied afresh before either indexing or retrieving a passage.

Imports: [recall_candidates](../../app/memory/recall_candidates.ep), [../providers/local_json](../../app/providers/local_json.ep), `fs` (toolchain/library).

Environment names: `ECHO_EMBEDDING_PROFILE`.

Definitions:

```text
7: define memory_semantic_config returning List:
13: define memory_semantic_schema with database as Int returning Int:
16: define memory_semantic_vector with database as Int and config as List and text as Str returning Str:
47: define memory_semantic_passages with database as Int and quotation as Str returning Str:
56: define memory_semantic_prepare with database as Int and run as Str returning Int:
118: define memory_semantic_select with database as Int and run as Str and claims as Str returning Str:
```

Lexical test references: [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_selected_recall_existing.ep](../../tests/security/test_selected_recall_existing.ep).

## `app/memory/scratchpad_scope.ep`

[app/memory/scratchpad_scope.ep](../../app/memory/scratchpad_scope.ep) · 18 lines · 774 bytes

SHA-256: `32cf0e78a1c36ad5b53409b024a7a44b97508c96f094087a2b3eadd3c25b9a4b`

Source annotations: Private notes retain their established principal identity. Shared notes belong to the exact audience segment, never to a participant's private principal.

Imports: [archive_recall](../../app/memory/archive_recall.ep), [../security/group_gate](../../app/security/group_gate.ep).

Definitions:

```text
6: define scratchpad_scope with database as Int and run as Str returning Str:
```

## `app/memory/session_ownership.ep`

[app/memory/session_ownership.ep](../../app/memory/session_ownership.ep) · 24 lines · 2637 bytes

SHA-256: `7c390e9d8fc950f37898c1407769698a8def3ef5b224d12efb3f926ad413e7e9`

Source annotations: Conversation ownership is durable, unlike a credential's current account link. Migration preserves each legacy device's original principal; it never retroactively transfers private history because that device was relinked. This is the principal scope check for future cross-session retrieval. It is not a replacement for current device routes until every surface is migrated. Caller supplies an authenticated device; credential and principal revocation are checked again here. Admin status does not bypass private ownership.

Imports: [../security/tool_authority](../../app/security/tool_authority.ep).

Definitions:

```text
6: define session_principal_schema with database as Int returning Str:
16: define session_principal_allows with database as Int and device as Str and session as Str returning Int:
```

Lexical test references: [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep).

## `app/memory/sharing.ep`

[app/memory/sharing.ep](../../app/memory/sharing.ep) · 83 lines · 6982 bytes

SHA-256: `ec2b249ea74f4ef47979b51c1f0ed3260995753c99d5a99d392b60443c877ac4`

Source annotations: This is an authenticated human-selection surface, not a model tool. A share is a reviewed independent copy, not access to the rest of the source silo. Canonical preview binds exact selected bytes, source digest, actor and target audience. A changed recipient revision requires a fresh human review.

Imports: [../web/attachments](../../app/web/attachments.ep), [../security/data_platform](../../app/security/data_platform.ep).

Definitions:

```text
7: define memory_share_session_scope with database as Int and session as Str returning Str:
15: define memory_share_material with database as Int and device as Str and source_event as Str and destination as Str returning List:
51: define memory_share_preview with database as Int and device as Str and source_event as Str and destination as Str returning Str:
60: define memory_share_commit with database as Int and device as Str and identity as Str and source_event as Str and destination as Str and review_digest as Str returning Str:
```

Lexical test references: [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep).

## `app/memory/source_import.ep`

[app/memory/source_import.ep](../../app/memory/source_import.ep) · 37 lines · 2565 bytes

SHA-256: `c07b99374f22dd9c85550c3409ce72178648791cd1226c13d08f121a7087d79d`

Source annotations: Host import primitive, not a model tool. The trusted run determines ownership. Imported text remains an attributed account, never a live user message or prompt.

Imports: [archive_recall](../../app/memory/archive_recall.ep), [../web/run_store](../../app/web/run_store.ep).

Definitions:

```text
6: define memory_source_import with database as Int and run as Str and event as Str and payload as Str returning Str:
```

Lexical test references: [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep).

## `app/memory/source_provenance.ep`

[app/memory/source_provenance.ep](../../app/memory/source_provenance.ep) · 64 lines · 4351 bytes

SHA-256: `305a88983888548b24ceb78db7e77bd67eeb17a3d8287c93e20b4dc58b9c9f9b`

Source annotations: Operator-owned annotations never rewrite canonical archive bytes. Only entries for sessions already visible in this owner's catalog enter their recall.

Imports: [../retained_text](../../app/retained_text.ep), `fs` (toolchain/library).

Environment names: `ECHO_MEMORY_PROVENANCE_FILE`.

Definitions:

```text
6: define memory_source_provenance_file returning Str:
12: define memory_source_provenance with catalog as Str returning Str:
37: define memory_source_chronology with database as Int and claims as Str returning Str:
45: define memory_current_source_correction with session as Str returning Str:
```

Lexical test references: [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep).

## `app/migrate_delegation.ep`

[app/migrate_delegation.ep](../../app/migrate_delegation.ep) · 18 lines · 843 bytes

SHA-256: `ec2efde7912f7dcdc5c1a58eecee76e53e7f9a708a0eade9476ec06b6c40769d`

Imports: [jobs/delegation_schema](../../app/jobs/delegation_schema.ep).

Native `main` present. Argument indices read: 1. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/mlx_count.ep`

[app/mlx_count.ep](../../app/mlx_count.ep) · 78 lines · 4694 bytes

SHA-256: `39fad251d1d5233cc960853008ca2397686a31dd77f3668f5ced689687bd2cf3`

Source annotations: One-shot native helper; all interpreter-owned objects live until process exit. Invoke the pinned upstream server method, including its tools, reasoning tail and content normalization. No hand-written substitute chat template.

Imports: [providers/native_python](../../app/providers/native_python.ep), [providers/json_access](../../app/providers/json_access.ep), [providers/archived_stream](../../app/providers/archived_stream.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `app/network/curl_native.ep`

[app/network/curl_native.ep](../../app/network/curl_native.ep) · 135 lines · 7697 bytes

SHA-256: `bd6164d591d206ca991036ed96681cad6a5274e339302b96e412062874bae560`

Source annotations: Installed Darwin arm64 libcurl variadic ABI: CURL pointer, option int, value. The value is either a pointer or native long. No shell or curl configuration.

Imports: [public_url](../../app/network/public_url.ep), [../workspace/files](../../app/workspace/files.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
7: define network_curl_call with library as Int and name as Str and easy as Int and option as Int and value as Int and pointer as Int returning Int:
35: define network_curl_info with library as Int and easy as Int and option as Int returning Str:
48: define network_curl_transfer with parsed as List and addresses as List and body as Int and headers as Int returning Str:
```

## `app/network/fetch.ep`

[app/network/fetch.ep](../../app/network/fetch.ep) · 118 lines · 6815 bytes

SHA-256: `0378ce7a22384cc08b3bfa16e306e8696fc2c7f7fa85c542d635129fa0c33b3e`

Source annotations: Private helper directory supplied by the trusted caller. Never a model path. Every hop has separate complete originals and a flushed receipt before the next destination is attempted. No automatic redirect or arbitrary hop cap.

Imports: [media_type](../../app/network/media_type.ep), [html_document](../../app/network/html_document.ep), [curl_native](../../app/network/curl_native.ep).

Definitions:

```text
5: define network_fetch_save with library as Int and directory as Int and name as Str and text as Str returning Int:
18: define network_fetch with original as Str and directory as Int returning Str:
```

## `app/network/html_document.ep`

[app/network/html_document.ep](../../app/network/html_document.ep) · 119 lines · 7147 bytes

SHA-256: `489e3179eca6f08d287b01b51a3ba83f5b9e27c1277faa3cf2ae78a3843183b5`

Source annotations: libxml2's exported xmlFree is a function-pointer variable, not a function. Installed SDK xmlXPathObject.nodesetval offset8; xmlNodeSet has two int32s then xmlNode** at8. These are native structure layouts, not content limits.

Imports: [public_url](../../app/network/public_url.ep), [../workspace/files](../../app/workspace/files.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
6: define network_html_free with library as Int and pointer as Int returning Int:
11: define network_html_string with library as Int and context as Int and expression as Str returning Str:
25: define network_html_nodes with library as Int and context as Int and expression as Str returning List:
50: define network_html_extract_encoded with descriptor as Int and base as Str and encoding as Str returning Str:
118: define network_html_extract with descriptor as Int and base as Str returning Str:
```

Lexical test references: [tests/network/test_html_document.ep](../../tests/network/test_html_document.ep), [tests/network/test_html_isolation.ep](../../tests/network/test_html_isolation.ep).

## `app/network/media_type.ep`

[app/network/media_type.ep](../../app/network/media_type.ep) · 24 lines · 1240 bytes

SHA-256: `0081109b5ef6081ebca8907c9fb7286ff565cbde69506cad02fc60feac68e12f`

Source annotations: Retain the server's declared MIME type, without parameters or control bytes. This is metadata, not validation of the body or permission to execute it.

Imports: [../retained_text](../../app/retained_text.ep), `string` (toolchain/library).

Definitions:

```text
6: define network_media_type with declared as Str returning Str:
```

Lexical test references: [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep).

## `app/network/public_address.ep`

[app/network/public_address.ep](../../app/network/public_address.ep) · 99 lines · 4514 bytes

SHA-256: `267ff9244a11b91b31f7fd9ad18236dc4229335b09889b4de41e6b676f762125`

Source annotations: Binary prefixes avoid textual IPv4/IPv6 aliases bypassing address policy.

Imports: [../archive/history](../../app/archive/history.ep), [special_ranges](../../app/network/special_ranges.ep), [../research/native_ffi](../../app/research/native_ffi.ep), [../archive/file_text](../../app/archive/file_text.ep).

Definitions:

```text
7: define network_prefix_matches with address as Int and prefix as Int and bits as Int returning Int:
25: define network_address_public with library as Int and family as Int and address as Int returning Int:
55: define network_resolve_public with host as Str and port as Str returning List:
```

Lexical test references: [tests/network/test_public_destination.ep](../../tests/network/test_public_destination.ep).

## `app/network/public_url.ep`

[app/network/public_url.ep](../../app/network/public_url.ep) · 64 lines · 3273 bytes

SHA-256: `e986954ed7ee55b6fd44ec0743f44874897f4ef9ae5f272a088058be7f6aa4b8`

Source annotations: Installed curl URL API owns parsing, relative redirects and numeric aliases.

Imports: [public_address](../../app/network/public_address.ep).

Definitions:

```text
4: define network_url_part with library as Int and url as Int and part as Int and flags as Int returning List:
13: define network_url_parts with library as Int and url as Int returning List:
38: define network_url_parse with text as Str and base as Str returning List:
```

Lexical test references: [tests/network/test_public_destination.ep](../../tests/network/test_public_destination.ep).

## `app/network/search_query.ep`

[app/network/search_query.ep](../../app/network/search_query.ep) · 20 lines · 1240 bytes

SHA-256: `0a37445ad2fca9ad427d2d0af6cd63535257fdc181f3f727182b0247ad323b8a`

Source annotations: Encode the original UTF-8 bytes, preserving punctuation and non-ASCII input. No credentials, provider-imposed count override or hidden query rewriting.

Imports: [public_url](../../app/network/public_url.ep).

Definitions:

```text
5: define network_query_encode with query as Str returning Str:
19: define network_search_url with query as Str returning Str:
```

Lexical test references: [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep).

## `app/network/special_ranges.ep`

[app/network/special_ranges.ep](../../app/network/special_ranges.ep) · 8 lines · 1433 bytes

SHA-256: `34f548a0015d18c6c0b497152c66fa58c780a61cd904ba5f21ab685f97a2be06`

Source annotations: IANA special-purpose registry snapshot. Longest prefix wins. Public HTTP research additionally excludes multicast and non-global IPv6.

Imports: none.

Definitions:

```text
4: define network_ipv4_special returning List:
7: define network_ipv6_special returning List:
```

## `app/prepare_controller.ep`

[app/prepare_controller.ep](../../app/prepare_controller.ep) · 11 lines · 663 bytes

SHA-256: `972cfd055044d1bb03ce6a3229f4b8f2dc8f98c5a9983f43c8e8f8dfe754cdf5`

Imports: [lifecycle/controller_prepare](../../app/lifecycle/controller_prepare.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7, 8. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/principal_admin.ep`

[app/principal_admin.ep](../../app/principal_admin.ep) · 100 lines · 5600 bytes

SHA-256: `1dcc9409fa9425277037f93725966b5aa7639fb8750e201cdb4a2d6c232e8562`

Source annotations: OS-local operator command. No browser or model schema exposes this program. Guest sandbox workers cannot open the host database. Never infer authority from an HTTP origin, device name, caller-supplied role or network address. Check compatibility before archive_open can perform schema writes.

Imports: [web/device_links](../../app/web/device_links.ep), [jobs/schedule_store](../../app/jobs/schedule_store.ep), [web/run_store](../../app/web/run_store.ep), [memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
9: define main returning Int:
```

## `app/projects/context.ep`

[app/projects/context.ep](../../app/projects/context.ep) · 64 lines · 5406 bytes

SHA-256: `b7088cce9a25b7a88e07403b47fa1d66ef29a15d4ca22548d5464c997cd75f7f`

Source annotations: Selection is a conversation record. It never rewrites the project owner.

Imports: [store](../../app/projects/store.ep), [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep).

Definitions:

```text
5: define project_select with database as Int and run as Str and project as Str and event as Str returning Str:
30: define project_context with database as Int and run as Str returning Str:
57: define project_context_manifest with database as Int and manifest as Str returning Str:
```

Lexical test references: [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep).

## `app/projects/manifest.ep`

[app/projects/manifest.ep](../../app/projects/manifest.ep) · 88 lines · 5235 bytes

SHA-256: `46f3a6035d6a2dab4f0659cf6de1be18420e8e653489f91dc535e577c6ed00e3`

Source annotations: Reject unknown fields instead of treating text as executable configuration. A reference retains the attachment's existing scope and digest. It does not copy bytes or grant another audience access to the original attachment. Scope is resolved by a trusted caller, never taken from browser/model JSON.

Imports: [scope](../../app/projects/scope.ep).

Definitions:

```text
4: define project_object_keys with tokens as List and object as Int and names as List returning Int:
23: define project_file_allowed with database as Int and run as Str and attachment as Str and digest as Str returning Int:
27: define project_file_allowed_scope with database as Int and scope as Str and attachment as Str and digest as Str returning Int:
43: define project_manifest_valid with database as Int and run as Str and manifest as Str returning Int:
46: define project_manifest_valid_scope with database as Int and scope as Str and manifest as Str returning Int:
```

Lexical test references: [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep).

## `app/projects/scope.ep`

[app/projects/scope.ep](../../app/projects/scope.ep) · 40 lines · 2366 bytes

SHA-256: `f578471223ff90237e669b7e8c7d4aa14c8a8e09a1809161cf4f68c94b00ab2f`

Source annotations: Scope comes only from a frozen initiating run. Group projects belong to the exact audience revision, including isolated children within that audience. Device linking retains the existing memory union. It does not rewrite project provenance or make a private project visible from a shared conversation.

Imports: [../memory/archive_recall](../../app/memory/archive_recall.ep), [../security/group_gate](../../app/security/group_gate.ep).

Definitions:

```text
6: define project_scope with database as Int and run as Str returning Str:
23: define project_scope_allows with database as Int and run as Str and scope as Str returning Int:
27: define project_scope_pair_allows with database as Int and current as Str and scope as Str returning Int:
```

## `app/projects/store.ep`

[app/projects/store.ep](../../app/projects/store.ep) · 78 lines · 5617 bytes

SHA-256: `f35ab2a01a5f6c9807a9bc56db9248e8187120ce1787a5fb8dc48def67f90d97`

Source annotations: Project revisions live in the existing immutable event archive, so backups, hashes and source ownership remain the same as the originating conversation. No caller-supplied scope is accepted and no separate mutable history exists. Native caller supplies an already validated manifest. Opaque original bytes are retained exactly, not serialized into a replacement representation.

Imports: [manifest](../../app/projects/manifest.ep), [../context/checkpoint](../../app/context/checkpoint.ep), [../agent/run_state](../../app/agent/run_state.ep).

Definitions:

```text
8: define project_latest with database as Int and run as Str and project as Str returning List:
11: define project_latest_scope with database as Int and current_scope as Str and project as Str returning List:
32: define project_revision_save with database as Int and run as Str and project as Str and event as Str and expected as Str and manifest as Str returning Str:
```

Lexical test references: [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep).

## `app/providers/archived_stream.ep`

[app/providers/archived_stream.ep](../../app/providers/archived_stream.ep) · 122 lines · 7044 bytes

SHA-256: `f60898f5406a71f0887ce6bc18f5068e58789dd17a08df623611a0e5003982c7`

Source annotations: Two bounded scans over the actual string length. Size the output exactly, then fill a native list buffer; no strlen or substring scans inside the loop.

Imports: [../retained_text](../../app/retained_text.ep), [chat_stream](../../app/providers/chat_stream.ep), [guidance_pending](../../app/providers/guidance_pending.ep), [../transport/event_stream](../../app/transport/event_stream.ep), [../archive/event_store](../../app/archive/event_store.ep).

Definitions:

```text
7: external define peek_byte with pointer as Int and offset as Int returning Int
8: external define poke_byte with pointer as Int and offset as Int and value as Int returning Int
12: define provider_quote with value as Str returning Str:
60: define provider_archive_piece with database as Int and session as Str and request as Str and sequence as Int and kind as Str and payload as Str returning Int:
65: define provider_collected with status as Str and state as List returning List:
75: define provider_collect_events with reader as List and body as List and database as Int and session as Str and request as Str and read_size as Int returning List:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_indexed_review.ep](../../tests/context/test_recap_indexed_review.ep), [tests/context/test_recap_jobs.ep](../../tests/context/test_recap_jobs.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_review.ep](../../tests/context/test_recap_review.ep), [tests/context/test_recap_review_coverage.ep](../../tests/context/test_recap_review_coverage.ep), [tests/context/test_recap_synthesis_gate.ep](../../tests/context/test_recap_synthesis_gate.ep), [tests/context/test_recap_upgrade.ep](../../tests/context/test_recap_upgrade.ep), [tests/context/test_system_observation_dedup.ep](../../tests/context/test_system_observation_dedup.ep), [tests/fixtures/delegation_fixture.ep](../../tests/fixtures/delegation_fixture.ep), [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/fixtures/schedule_fixture.ep](../../tests/fixtures/schedule_fixture.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_bundle.ep](../../tests/lifecycle/test_controller_bundle.ep), [tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep), [tests/lifecycle/test_controller_operations.ep](../../tests/lifecycle/test_controller_operations.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_guardian_readiness.ep](../../tests/lifecycle/test_guardian_readiness.ep), [tests/lifecycle/test_service.ep](../../tests/lifecycle/test_service.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/providers/test_mlx_request.ep](../../tests/providers/test_mlx_request.ep), [tests/providers/test_quote.ep](../../tests/providers/test_quote.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_linked_recall_existing.ep](../../tests/security/test_linked_recall_existing.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep), [tests/security/test_observer_parser.ep](../../tests/security/test_observer_parser.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_protected_files.ep](../../tests/security/test_protected_files.ep), [tests/security/test_protected_staging.ep](../../tests/security/test_protected_staging.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_reasoning_existing.ep](../../tests/security/test_reasoning_existing.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_feedback.ep](../../tests/tools/test_feedback.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/prepare_guardian_devices.ep](../../tests/web/prepare_guardian_devices.ep), [tests/web/seed_rendering.ep](../../tests/web/seed_rendering.ep), [tests/web/test_activity.ep](../../tests/web/test_activity.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_trace.ep](../../tests/web/test_trace.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/providers/attachment_image.ep`

[app/providers/attachment_image.ep](../../app/providers/attachment_image.ep) · 84 lines · 5753 bytes

SHA-256: `260b23c8a0e6f1c79664eeede710f9506397553bb3acbb51e1899e719a9be18b`

Source annotations: The caller supplies a trusted run session, never an arbitrary host path. Returns status, typed content part, original source event and original digest. Decoding and token cost belong to the loaded provider, not this serializer. Share the same source authority, original-byte verification and provenance for native sound. The provider decodes the waveform; no transcript is substituted.

Imports: [../archive/attachment_scope](../../app/archive/attachment_scope.ep), [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [../archive/file_base64](../../app/archive/file_base64.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
11: define attachment_image_original_part with database as Int and session as Str and attachment as Str returning List:
16: define attachment_audio_original_part with database as Int and session as Str and attachment as Str returning List:
19: define attachment_media_original_part with database as Int and session as Str and attachment as Str and modality as Str returning List:
76: define attachment_image_part with database as Int and session as Str and attachment as Str returning List:
81: define attachment_audio_part with database as Int and session as Str and attachment as Str returning List:
```

Lexical test references: [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep).

## `app/providers/chat_stream.ep`

[app/providers/chat_stream.ep](../../app/providers/chat_stream.ep) · 180 lines · 8686 bytes

SHA-256: `6c11fa4672222a1741180135421e430558a4250794ee302966a85f3b7d3775d2`

Source annotations: State: choices, done marker, error. Each choice has index, text builder, reasoning builder, tool list, finish reason. Each tool has index, id, name, argument builder. Provider indexes are exact strings, not sparse allocations.

Imports: [../retained_text](../../app/retained_text.ep), [json_access](../../app/providers/json_access.ep).

Definitions:

```text
7: define chat_stream_create returning List:
14: define chat_find with entries as List and identity as Str returning List:
23: define chat_append_field with tokens as List and object as Int and name as Str and builder as Int returning Int:
32: define chat_tool_delta with choice as List and tokens as List and delta as Int returning Int:
66: define chat_choice_delta with state as List and tokens as List and object as Int returning Int:
104: define chat_stream_accept_inner with state as List and data as Str returning Int:
128: define chat_stream_accept with state as List and data as Str returning Int:
138: define chat_stream_finish with state as List and transport as Str returning Int:
```

Lexical test references: [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/providers/test_chat_stream.ep](../../tests/providers/test_chat_stream.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep).

## `app/providers/context_admission.ep`

[app/providers/context_admission.ep](../../app/providers/context_admission.ep) · 31 lines · 1809 bytes

SHA-256: `5b5971031daa5219d964495853c85ab9853eef259f22eeba5157234a1295a09b`

Source annotations: Adapters supply verified effective limits and the exact token count for the assembled request. A finite output ceiling here must be a genuine provider capability, not a configurable generation default. Unknown values fail. Result: status, loaded context, supported context, input tokens, output room. Compaction cannot increase a separate generation ceiling. Surface this distinction so callers do not repeatedly compact an impossible workload.

Imports: [../context/budget](../../app/context/budget.ep).

Definitions:

```text
7: define provider_context_admission with loaded as Int and supported as Int and input as Int and ceiling as Int and reserve as Int returning List:
```

Lexical test references: [tests/providers/test_context_admission.ep](../../tests/providers/test_context_admission.ep).

## `app/providers/guidance_pending.ep`

[app/providers/guidance_pending.ep](../../app/providers/guidance_pending.ep) · 11 lines · 798 bytes

SHA-256: `c5d3937466ed93c4a5e1ddfa3af4eb8a75324426ca6775f88593ca2db6340a94`

Source annotations: Durable guidance belongs to a run. Application receipts prevent replay.

Imports: [../archive/history](../../app/archive/history.ep).

Definitions:

```text
4: define guidance_pending with database as Int and run as Str returning List:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep).

## `app/providers/helper_process.ep`

[app/providers/helper_process.ep](../../app/providers/helper_process.ep) · 143 lines · 7696 bytes

SHA-256: `21f79253cde401b61cc971edc1a245e8632a6f3c3750311ef69232ad83648dc6`

Source annotations: Host-owned provider helpers only. Explicit argv/environment and descriptors never pass through a shell. No wall-clock deadline or output truncation. Result: status, raw wait status, retained handle if cleanup needs attention.

Imports: [../retained_text](../../app/retained_text.ep), [../workspace/native_process](../../app/workspace/native_process.ep), [../archive/event_store](../../app/archive/event_store.ep), [archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
8: define provider_helper_cancelled with library as Int and reader as Int returning Int:
32: define provider_helper_result with status as Str and waited as Int and handle as List returning List:
42: define provider_helper_wait_owned with database as Int and session as Str and identity as Str and handle as List and cancellation as Int returning List:
103: define provider_helper_run with database as Int and session as Str and identity as Str and executable as Str and arguments as List and environment as List and directory as Int and output as Int and errors as Int and cancellation as Int returning List:
```

Lexical test references: [tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep), [tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep).

## `app/providers/json_access.ep`

[app/providers/json_access.ep](../../app/providers/json_access.ep) · 92 lines · 3616 bytes

SHA-256: `e32e6bdda3e82cdeb749a81e0cd6ed5594480f4b6a50b3c7af3153b5aa92dabb`

Imports: [../retained_text](../../app/retained_text.ep), [json_tokens](../../app/providers/json_tokens.ep).

Definitions:

```text
4: define provider_kind with tokens as List and index as Int returning Str:
9: define provider_value with tokens as List and index as Int returning Str:
14: define provider_after with tokens as List and index as Int returning Int:
35: define provider_member with tokens as List and object as Int and name as Str returning Int:
60: define provider_items with tokens as List and array as Int returning List:
74: define provider_index with tokens as List and index as Int returning Str:
85: define provider_unique_objects with tokens as List returning Int:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_large_request_envelope.ep](../../tests/agent/test_large_request_envelope.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/archive/test_branch_export_file.ep](../../tests/archive/test_branch_export_file.ep), [tests/archive/test_export_restore.ep](../../tests/archive/test_export_restore.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_cache_prefix.ep](../../tests/context/test_recap_cache_prefix.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/context/test_system_observation_dedup.ep](../../tests/context/test_system_observation_dedup.ep), [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/lifecycle/test_controller_bundle_privileged.ep](../../tests/lifecycle/test_controller_bundle_privileged.ep), [tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep), [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep), [tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_existing.ep](../../tests/memory/test_procedure_existing.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/network/test_html_document.ep](../../tests/network/test_html_document.ep), [tests/network/test_html_isolation.ep](../../tests/network/test_html_isolation.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_chat_stream.ep](../../tests/providers/test_chat_stream.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_inference_admission.ep](../../tests/providers/test_inference_admission.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_context_admission.ep](../../tests/providers/test_live_context_admission.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/providers/test_mlx_preflight.ep](../../tests/providers/test_mlx_preflight.ep), [tests/providers/test_modality_preflight.ep](../../tests/providers/test_modality_preflight.ep), [tests/providers/test_quote.ep](../../tests/providers/test_quote.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_observer_input.ep](../../tests/security/test_observer_input.ep), [tests/security/test_observer_parser.ep](../../tests/security/test_observer_parser.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_reasoning_existing.ep](../../tests/security/test_reasoning_existing.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_feedback.ep](../../tests/tools/test_feedback.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_run_store.ep](../../tests/web/test_run_store.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_trace.ep](../../tests/web/test_trace.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/providers/json_string.ep`

[app/providers/json_string.ep](../../app/providers/json_string.ep) · 100 lines · 4757 bytes

SHA-256: `e6b8a7fd6a558afce96ab875b95e14bd83892aea3b19e3dac956b824be127dd2`

Source annotations: Derived from pinned Ernos stdlib/json.ep. Bounds are retained; byte access avoids repeated strlen scans for large already-validated JSON strings. Append a Unicode code point to the byte list as UTF-8 (1..4 bytes). Uses integer division for the shifts/masks (no % operator in EP).

Imports: `json` (toolchain/library).

Definitions:

```text
4: external define peek_byte with pointer as Int and offset as Int returning Int
6: define provider_hex4 with s and p and len:
19: define provider_append_utf8 with lst and cp:
39: define provider_decode_string with state:
```

Lexical test references: [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep).

## `app/providers/json_tokens.ep`

[app/providers/json_tokens.ep](../../app/providers/json_tokens.ep) · 185 lines · 9119 bytes

SHA-256: `bff0cfdcb5a4aa5cc7d47d43dbc273b64726044878418c8efc642717d1481eba`

Source annotations: Tokens retain their original spelling. Numbers never pass through a lossy integer conversion. String decoding is used only after complete validation. Token fields: kind, original start, exclusive end, decoded string or spelling.

Imports: [../retained_text](../../app/retained_text.ep), [json_string](../../app/providers/json_string.ep).

Definitions:

```text
7: define provider_token with kind as Str and start as Int and finish as Int and value as Str returning List:
15: define provider_digit with text as Str and position as Int returning Int:
23: define provider_next_token with text as Str and start as Int returning List:
128: define provider_json_tokens with text as Str returning List:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_large_request_envelope.ep](../../tests/agent/test_large_request_envelope.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/verify_live_provider_loop.ep](../../tests/agent/verify_live_provider_loop.ep), [tests/archive/test_branch_export_file.ep](../../tests/archive/test_branch_export_file.ep), [tests/archive/test_digest_stress.ep](../../tests/archive/test_digest_stress.ep), [tests/archive/test_export_restore.ep](../../tests/archive/test_export_restore.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_cache_prefix.ep](../../tests/context/test_recap_cache_prefix.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/context/test_system_observation_dedup.ep](../../tests/context/test_system_observation_dedup.ep), [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/lifecycle/test_controller_bundle_privileged.ep](../../tests/lifecycle/test_controller_bundle_privileged.ep), [tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep), [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_service.ep](../../tests/lifecycle/test_service.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_existing.ep](../../tests/memory/test_procedure_existing.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/network/test_html_document.ep](../../tests/network/test_html_document.ep), [tests/network/test_html_isolation.ep](../../tests/network/test_html_isolation.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_chat_stream.ep](../../tests/providers/test_chat_stream.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_inference_admission.ep](../../tests/providers/test_inference_admission.ep), [tests/providers/test_json_tokens.ep](../../tests/providers/test_json_tokens.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_context_admission.ep](../../tests/providers/test_live_context_admission.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/providers/test_mlx_preflight.ep](../../tests/providers/test_mlx_preflight.ep), [tests/providers/test_modality_preflight.ep](../../tests/providers/test_modality_preflight.ep), [tests/providers/test_quote.ep](../../tests/providers/test_quote.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_observer_input.ep](../../tests/security/test_observer_input.ep), [tests/security/test_observer_parser.ep](../../tests/security/test_observer_parser.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_reasoning_existing.ep](../../tests/security/test_reasoning_existing.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_feedback.ep](../../tests/tools/test_feedback.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_run_store.ep](../../tests/web/test_run_store.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_trace.ep](../../tests/web/test_trace.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep), [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/providers/llama_metadata.ep`

[app/providers/llama_metadata.ep](../../app/providers/llama_metadata.ep) · 59 lines · 3007 bytes

SHA-256: `32d9b1ba16e4d228eb4306c7976c5bb51b91924175f7a07cec934002f82f619a`

Source annotations: Native llama.cpp metadata; unknown values fail instead of receiving defaults. Result: status, model id, loaded context, training context, output ceiling.

Imports: [local_json](../../app/providers/local_json.ep), [../context/budget](../../app/context/budget.ep).

Definitions:

```text
6: define llama_metadata_parse with models as Str and props as Str and model as Str returning List:
53: define llama_input_count with response as Str returning Int:
```

Lexical test references: [tests/providers/test_llama_metadata.ep](../../tests/providers/test_llama_metadata.ep).

## `app/providers/llama_modalities.ep`

[app/providers/llama_modalities.ep](../../app/providers/llama_modalities.ep) · 90 lines · 5584 bytes

SHA-256: `fbe92495eee12e2c31d2d7980c5915a419a4012a87eb17070fae013b0aef5cef`

Source annotations: Inspected against pinned llama.cpp b11429: /props.modalities describes the loaded projector's image/audio/video input support. No model-name guesses.

Imports: [local_json](../../app/providers/local_json.ep).

Definitions:

```text
5: define llama_modalities_check with body as Str and properties as Str returning Str:
49: define llama_modalities_notice with body as Str and properties as Str returning Str:
```

Lexical test references: [tests/providers/test_llama_modalities.ep](../../tests/providers/test_llama_modalities.ep).

## `app/providers/llama_preflight.ep`

[app/providers/llama_preflight.ep](../../app/providers/llama_preflight.ep) · 90 lines · 6423 bytes

SHA-256: `10a3799294da8876e4f79cc3da7785c4d973ea05d4c7a7d5d0260b2c023c1589`

Source annotations: The reserve comes from the next workload. Capacity and input count come only from the loaded provider. No guessed context, output budget or safety margin. Result: status, loaded context, training context, input tokens, available output. n_predict is a configurable runtime default, not a model capability. Require its removal rather than treating it as a genuine output ceiling. Preserve all endpoint evidence and the exact request, including failed HTTP responses. A caller can inspect why a request was not admitted to generation.

Imports: [../retained_text](../../app/retained_text.ep), [llama_metadata](../../app/providers/llama_metadata.ep), [llama_modalities](../../app/providers/llama_modalities.ep), [context_admission](../../app/providers/context_admission.ep).

Definitions:

```text
9: define llama_preflight_evaluate with models as Str and props as Str and counted as Str and model as Str and output_reserve as Int returning List:
31: define llama_preflight_receipt with database as Int and session as Str and identity as Str and body as Str and output_reserve as Int and models as List and props as List and counted as List and decision as List returning Int:
52: define llama_preflight with database as Int and session as Str and identity as Str and port as Int and body as Str and output_reserve as Int and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep), [tests/context/test_token_count_retry.ep](../../tests/context/test_token_count_retry.ep), [tests/providers/test_context_admission.ep](../../tests/providers/test_context_admission.ep), [tests/providers/test_live_context_admission.ep](../../tests/providers/test_live_context_admission.ep), [tests/providers/test_modality_preflight.ep](../../tests/providers/test_modality_preflight.ep).

## `app/providers/local_chat.ep`

[app/providers/local_chat.ep](../../app/providers/local_chat.ep) · 118 lines · 7417 bytes

SHA-256: `b7526d450acef1083c5ea33832998818d9f952cde2c02bcfec252b4b8b6fc91f`

Source annotations: Validate before connecting. The exact outgoing JSON must already be durable. Localhost is deliberate for this adapter, not a provider-wide restriction.

Imports: [../retained_text](../../app/retained_text.ep), [archived_stream](../../app/providers/archived_stream.ep), [../transport/local_connect](../../app/transport/local_connect.ep).

Definitions:

```text
7: define local_chat_request_valid with database as Int and session as Str and request as Str and body as Str and path as Str returning Int:
39: define local_chat_exchange with reader as List and database as Int and session as Str and request as Str and port as Int and path as Str and body as Str and read_size as Int returning List:
91: define local_chat_generate with database as Int and session as Str and request as Str and port as Int and path as Str and body as Str and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep).

## `app/providers/local_json.ep`

[app/providers/local_json.ep](../../app/providers/local_json.ep) · 122 lines · 6535 bytes

SHA-256: `dbc7c949433d1ea82724dc4298d8dbedce0b391a5770becbd8fe74a49a9170b5`

Source annotations: Reject malformed UTF-8 and NUL rather than corrupting metadata or JSON keys.

Imports: [../retained_text](../../app/retained_text.ep), [local_chat](../../app/providers/local_chat.ep).

Definitions:

```text
5: define provider_valid_utf8 with text as Str returning Int:
40: define local_json_result with status as Str and code as Int and text as Str returning List:
49: define local_json_exchange with reader as List and method as Str and port as Int and path as Str and body as Str and read_size as Int returning List:
99: define local_json_request with port as Int and method as Str and path as Str and body as Str and cancellation_reader as Int and read_size as Int returning List:
```

Lexical test references: [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_local_json.ep](../../tests/providers/test_local_json.ep).

## `app/providers/mlx_discovery.ep`

[app/providers/mlx_discovery.ep](../../app/providers/mlx_discovery.ep) · 66 lines · 4159 bytes

SHA-256: `d45bdb3bfc09ca50dff53e06b9346085731c850056f3a21a66e9a8584a18acfb`

Source annotations: MLX /v1/models reports available identities, not weight hashes or loaded context. Do not infer either from this list. Local pinned configuration and runtime launch provenance remain separate requirements.

Imports: [../retained_text](../../app/retained_text.ep), [mlx_preflight](../../app/providers/mlx_preflight.ep).

Definitions:

```text
7: define mlx_discovery_check with health as Str and models as Str and model as Str returning Str:
34: define mlx_preflight with database as Int and session as Str and identity as Str and body as Str and port as Int and helper as Str and interpreter as Str and program as Str and model as Str and spool as Str and reserve as Int and cancellation as Int and batch as Int returning List:
```

Lexical test references: [tests/providers/test_mlx_discovery.ep](../../tests/providers/test_mlx_discovery.ep).

## `app/providers/mlx_preflight.ep`

[app/providers/mlx_preflight.ep](../../app/providers/mlx_preflight.ep) · 120 lines · 7862 bytes

SHA-256: `d33adfddc89e8a434f7d1db468d1e418ae04ebe85b46a146f1cd3ed1a4cb0168`

Source annotations: Read the completed helper's exact output through its owned descriptor. The read window derives from file size, never an application receipt-size cap. Host configuration supplies executable paths, model and private spool root. Result: status, prepared wire body, context, input, available output, retained child handle if cleanup requires recovery. Never use the body unless ready.

Imports: [../retained_text](../../app/retained_text.ep), [helper_process](../../app/providers/helper_process.ep), [mlx_request](../../app/providers/mlx_request.ep), [../workspace/files](../../app/workspace/files.ep), `fs` (toolchain/library).

Definitions:

```text
9: define mlx_helper_output with library as Int and descriptor as Int returning List:
24: define mlx_preflight_result with status as Str and handle as List returning List:
34: define mlx_preflight_local with database as Int and session as Str and identity as Str and body as Str and helper as Str and interpreter as Str and program as Str and model as Str and spool as Str and reserve as Int and cancellation as Int returning List:
```

Lexical test references: [tests/providers/test_mlx_preflight.ep](../../tests/providers/test_mlx_preflight.ep).

## `app/providers/mlx_request.ep`

[app/providers/mlx_request.ep](../../app/providers/mlx_request.ep) · 104 lines · 5753 bytes

SHA-256: `10dfd6f2865332f73b7f0c1929afc64f0f2d904babd010bee2d523ee635a4c35`

Source annotations: Pinned MLX-LM reads native model configuration without a smaller context slot. Extensions require separately verified configuration, never an inferred scale. This adapter is MLX-LM text inference. A multimodal weight architecture does not imply that this runtime accepts image/audio/video request parts.

Imports: [../retained_text](../../app/retained_text.ep), [context_admission](../../app/providers/context_admission.ep), [llama_metadata](../../app/providers/llama_metadata.ep).

Definitions:

```text
7: define mlx_context_capacity with configuration as Str returning Int:
29: define mlx_text_request with tokens as List returning Str:
56: define mlx_prepare_request with body as Str and configuration as Str and counted as Str and model as Str and reserve as Int returning List:
```

Lexical test references: [tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep), [tests/providers/test_mlx_request.ep](../../tests/providers/test_mlx_request.ep).

## `app/providers/native_python.ep`

[app/providers/native_python.ep](../../app/providers/native_python.ep) · 127 lines · 5789 bytes

SHA-256: `760b3404e1b7ec57b8f1db484ed55a09be8c95fc12e1843dafdf305bc38ebb5f`

Source annotations: Native dependency bridge for an isolated provider helper process. All Echo logic remains Ernos. Calls invoke installed upstream functions, never source snippets. Use on the initializing thread; do not share with web workers. Keep the dependency library loaded until process exit. Py_SetProgramName retains this storage for the interpreter lifetime.

Imports: none.

Definitions:

```text
5: define native_python_open with library_path as Str and program as Str returning Int:
27: define native_python_import with library as Int and name as Str returning Int:
33: define native_python_attribute with library as Int and object as Int and name as Str returning Int:
41: define native_python_release with library as Int and object as Int returning Int:
50: define native_python_text with library as Int and object as Int returning Str:
63: define native_python_call with library as Int and function as Int and arguments as List and keywords as Int returning Int:
88: define native_python_string with library as Int and text as Str returning Int:
92: define native_python_json with library as Int and text as Str returning Int:
103: define native_python_member with library as Int and object as Int and name as Str returning Int:
112: define native_python_set with library as Int and object as Int and name as Str and value as Int returning Int:
118: define native_python_namespace with library as Int and keywords as Int returning Int:
```

Lexical test references: [tests/providers/test_native_mlx.ep](../../tests/providers/test_native_mlx.ep).

## `app/providers/prepare.ep`

[app/providers/prepare.ep](../../app/providers/prepare.ep) · 141 lines · 9011 bytes

SHA-256: `5062eea85af8edcea64f359c3172e2ac018477e52661e28a81f6c7bc697ad3cf`

Source annotations: Stable adapter result: status, loaded context, supported context, exact input, output room, wire body, endpoint, port, retained helper handle. Compatibility constructor for the existing host launch configuration. Host-authorized profile is an input, never selected by model/tool text. Archive its exact value for this attempt. Unsupported adapters fail explicitly.

Imports: [../retained_text](../../app/retained_text.ep), [llama_preflight](../../app/providers/llama_preflight.ep), [mlx_discovery](../../app/providers/mlx_discovery.ep).

Definitions:

```text
7: define provider_prepared_error with status as Str returning List:
21: define provider_llama_profile with model as Str and port as Int returning Str:
27: define provider_profile_status with profile as Str returning Str:
72: define provider_prepare with database as Int and session as Str and identity as Str and profile as Str and body as Str and reserve as Int and cancellation as Int and batch as Int returning List:
135: define provider_scheduled_body with session as Str and identity as Str and body as Str returning Str:
```

Lexical test references: [tests/agent/test_provider_profile.ep](../../tests/agent/test_provider_profile.ep), [tests/context/measure_recap_cache.ep](../../tests/context/measure_recap_cache.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_recap_findings.ep](../../tests/context/test_recap_findings.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_session_messages.ep](../../tests/context/test_session_messages.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/providers/test_inference_admission.ep](../../tests/providers/test_inference_admission.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/providers/test_registry.ep](../../tests/providers/test_registry.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep).

## `app/providers/registry.ep`

[app/providers/registry.ep](../../app/providers/registry.ep) · 95 lines · 6819 bytes

SHA-256: `32771f1a254a2420551a30357533be5607fae835f27d0154e230f20f3242696d`

Source annotations: Only the local administrator registers executable profiles. Browser selections refer to these immutable identities; they never supply paths or commands. Return found, label, profile; or end/error. Profiles remain host-private. Exact retries are harmless. Reusing an identity for different bytes conflicts.

Imports: [../retained_text](../../app/retained_text.ep), [prepare](../../app/providers/prepare.ep), [../context/checkpoint](../../app/context/checkpoint.ep), [../archive/history](../../app/archive/history.ep).

Definitions:

```text
8: define provider_registry_schema with database as Int returning Int:
12: define provider_registry_read with database as Int and identity as Str returning List:
19: define provider_registry_register with database as Int and identity as Str and label as Str and profile as Str returning Int:
48: define provider_registry_select with database as Int and identity as Str returning Int:
57: define provider_registry_default with database as Int returning List:
63: define provider_registry_initialize with database as Int and profile as Str returning Int:
76: define provider_registry_choices with database as Int returning Str:
```

Lexical test references: [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/providers/test_registry.ep](../../tests/providers/test_registry.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep).

## `app/providers/restored_stream.ep`

[app/providers/restored_stream.ep](../../app/providers/restored_stream.ep) · 43 lines · 2720 bytes

SHA-256: `bc973519c29282d495339b3aa800ce4fb2c596f4820dbca2f658916ac0f6c037`

Source annotations: Reassemble committed provider events without any network access. Missing terminal evidence is incomplete, never an inferred successful response.

Imports: [archived_stream](../../app/providers/archived_stream.ep), [../context/checkpoint](../../app/context/checkpoint.ep).

Definitions:

```text
6: define provider_restore_stream with database as Int and session as Str and request as Str returning List:
```

## `app/providers/status.ep`

[app/providers/status.ep](../../app/providers/status.ep) · 64 lines · 4379 bytes

SHA-256: `2a8ee9d984e2ceb9efb78fca38d78080a9cacb48af46b507b1b6e8e2753118f5`

Source annotations: Display metadata separately from admission. Available models are not proof that weights are loaded; an unknown loaded capacity remains JSON null.

Imports: [../retained_text](../../app/retained_text.ep), [prepare](../../app/providers/prepare.ep).

Environment names: `ECHO_RESEARCH_ROOT`, `ECHO_RESEARCH_RUNTIME`.

Definitions:

```text
6: define provider_status_read with profile as Str returning Str:
```

## `app/qualify_compaction.ep`

[app/qualify_compaction.ep](../../app/qualify_compaction.ep) · 90 lines · 6615 bytes

SHA-256: `11e1302719ebcc250aaf964f797e875f321bde8492c30f111175aa1b1d61bd5c`

Source annotations: Live manual compaction probe with corrections, relationships, unfinished work and an exact original. This tests real recap/review/activation/reopen; it does not substitute for the separate automatic full-capacity trigger test.

Imports: [web/attachments](../../app/web/attachments.ep), [agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `app/qualify_context.ep`

[app/qualify_context.ep](../../app/qualify_context.ep) · 88 lines · 5089 bytes

SHA-256: `b8c38f2caf94effade75c1fc75b7eeb8a7fa94e1b40d0931c98a3ff7d327150d`

Source annotations: Synthetic context-capacity probe. Reserve is supplied for this workload and never becomes a generation ceiling. The provider supplies capacity and counts.

Imports: [providers/llama_preflight](../../app/providers/llama_preflight.ep), [providers/local_chat](../../app/providers/local_chat.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define qualification_body with model as Str and repeats as Int returning Str:
19: define qualification_count with port as Int and body as Str returning Int:
25: define main returning Int:
```

## `app/qualify_followups.ep`

[app/qualify_followups.ep](../../app/qualify_followups.ep) · 79 lines · 5176 bytes

SHA-256: `d40b5d928eceddd252ef9d8ad4c78a601ea622d380d65627b4a87170dc38d365`

Source annotations: Three live turns: import the saved full-context probe, then two changed user requests with the actual intervening assistant replies. No output token cap.

Imports: [providers/llama_preflight](../../app/providers/llama_preflight.ep), [providers/local_chat](../../app/providers/local_chat.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `app/qualify_media.ep`

[app/qualify_media.ep](../../app/qualify_media.ep) · 88 lines · 5058 bytes

SHA-256: `bc9905d081ce48bdfac60c8be15dd64ab2c3559b0aafdf9d71a8ac3fa3c9c26c`

Source annotations: Host-only media runtime qualification. The host supplies native argv and an existing private working directory, never a model-selected executable. Native model parameters are preserved without geometry or step-count caps.

Imports: [providers/helper_process](../../app/providers/helper_process.ep), [providers/json_tokens](../../app/providers/json_tokens.ep), [workspace/files](../../app/workspace/files.ep), [archive/file_digest](../../app/archive/file_digest.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
10: define main returning Int:
```

## `app/qualify_mlx.ep`

[app/qualify_mlx.ep](../../app/qualify_mlx.ep) · 63 lines · 3742 bytes

SHA-256: `e0ddb46716caab8e6380d346da092fa945a195dd240c28282981e143a331d1f1`

Source annotations: Real streaming inference with exact tokenizer/provider-usage reconciliation.

Imports: [providers/mlx_discovery](../../app/providers/mlx_discovery.ep), [providers/local_chat](../../app/providers/local_chat.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7, 8. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
6: define main returning Int:
```

## `app/qualify_model.ep`

[app/qualify_model.ep](../../app/qualify_model.ep) · 63 lines · 4041 bytes

SHA-256: `205d8d4a8ee99a95a17188ee40220bbadac5b1bee72eb418c4ed1137f7e486c4`

Source annotations: A live qualification entry point. Uses the sole model advertised by a local provider; captures discovery and lets the normal runner perform exact token preflight and archive every generated turn. No mock responses or output cap.

Imports: [web/attachments](../../app/web/attachments.ep), [agent/runner](../../app/agent/runner.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
7: define main returning Int:
```

## `app/qualify_replay.ep`

[app/qualify_replay.ep](../../app/qualify_replay.ep) · 48 lines · 2821 bytes

SHA-256: `2327f852e8950d68f569b34d0117410ea565b8b0dd6cb37437735286c01e4510`

Source annotations: Intentional new qualification attempt using an exact previously saved body. A new destination archive and request identity keep this separate from crash recovery. Used to compare cached context with the first fresh-context run.

Imports: [providers/llama_preflight](../../app/providers/llama_preflight.ep), [providers/local_chat](../../app/providers/local_chat.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4, 5. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
8: define main returning Int:
```

## `app/recover_empty_reply.ep`

[app/recover_empty_reply.ep](../../app/recover_empty_reply.ep) · 55 lines · 4744 bytes

SHA-256: `a147a059c8362a6bff6e40d84b77985440c89c85b196486f9c0c2251bee718b3`

Source annotations: Operator repair of the former false-completion bug. Preserve the original event and all tools; only a verified empty terminal reply may be reopened.

Imports: [web/worker](../../app/web/worker.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define main returning Int:
```

## `app/recover_memory.ep`

[app/recover_memory.ep](../../app/recover_memory.ep) · 77 lines · 4707 bytes

SHA-256: `d433ea2b451b0c2a607cabbc606d7118c81908920a0e9174cd5c0812b954a3d7`

Source annotations: Operator recovery from an already archived, fully completed model response. Reuses original request sources and exact streamed bytes; no new inference.

Imports: [memory/automatic](../../app/memory/automatic.ep).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
5: define memory_recovery_event with database as Int and identity as Str returning List:
13: define main returning Int:
```

## `app/release_restored_archive.ep`

[app/release_restored_archive.ep](../../app/release_restored_archive.ep) · 11 lines · 680 bytes

SHA-256: `a90e75c5427ec2544851f733f24c224bcd1681c6d40269fd3293c67509f33845`

Imports: [archive/restore_release](../../app/archive/restore_release.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/research/capture_activation.ep`

[app/research/capture_activation.ep](../../app/research/capture_activation.ep) · 141 lines · 8786 bytes

SHA-256: `710b6e2c4b637f6f29326fb26635942d692e8131b0ca17dcaeb824d9a71807ca`

Source annotations: Actual engine callback. State owns all strings/handles for the decode lifetime. Selected tensor only; reject non-F32/non-contiguous storage rather than guessing. ggml_tensor prefix on this pinned ABI: enum type, aligned buffer pointer, then four int64 dimensions starting at byte16. Save actual dimensions.

Imports: [llama_native](../../app/research/llama_native.ep), [../archive/file_stage](../../app/archive/file_stage.ep), `fs` (toolchain/library).

Definitions:

```text
7: define research_capture_callback with tensor as Int and ask as Int and state as List returning Int:
53: define research_capture_run returning Int:
```

Lexical test references: [tests/research/capture_primary_activation.ep](../../tests/research/capture_primary_activation.ep).

## `app/research/compare_intervention.ep`

[app/research/compare_intervention.ep](../../app/research/compare_intervention.ep) · 182 lines · 13323 bytes

SHA-256: `6daa200b8c7e04c73f522b89029042aae66a780da0432fc3de88d07718574807`

Source annotations: Baseline -> actual intervention -> removal on one loaded primary model.

Imports: [run_record](../../app/research/run_record.ep).

Definitions:

```text
4: define research_compare_run with allow_derived as Int returning Int:
```

Lexical test references: [tests/research/test_primary_intervention.ep](../../tests/research/test_primary_intervention.ep).

## `app/research/intervention.ep`

[app/research/intervention.ep](../../app/research/intervention.ep) · 82 lines · 4113 bytes

SHA-256: `0c20cca9c33fda5a2e3dfc6ac21cfddd35b79af5e271f342f1618fc435c3bfa4`

Source annotations: Validate the actual IEEE754 float32 payload without converting its values. The pinned ABI is little endian; each four-byte word is zero-extended. Host-only experiment callback. The baseline captures a reusable native-width vector from its final source token. This is an activation-derived direction, not a claim of a trained semantic/personality direction. State extends capture state with mode, vector, byte count, scale bits and vDSP.

Imports: [capture_activation](../../app/research/capture_activation.ep).

Definitions:

```text
5: define research_f32_finite with system as Int and pointer as Int and count as Int returning Int:
22: define research_intervention_callback with tensor as Int and ask as Int and state as List returning Int:
```

## `app/research/llama_native.ep`

[app/research/llama_native.ep](../../app/research/llama_native.ep) · 29 lines · 1539 bytes

SHA-256: `f32ae855be3da44f705a4ea1ba7fc5b12a59a00415c1dbb46cac17a12f0eab38`

Source annotations: Version-specific native ABI. Portable chat does not depend on these layouts. Field order is llama.cpp b11429 llama.h; libffi supplies actual offsets.

Imports: [native_ffi](../../app/research/native_ffi.ep).

Definitions:

```text
5: define research_llama_model_type with ffi as Int returning List:
11: define research_llama_context_type with ffi as Int returning List:
19: define research_llama_batch_type with ffi as Int returning List:
23: define research_native_write with system as Int and pointer as Int and value as Int and bytes as Int returning Int:
```

## `app/research/native_ffi.ep`

[app/research/native_ffi.ep](../../app/research/native_ffi.ep) · 69 lines · 3522 bytes

SHA-256: `c6692b9f3a4da395b6c636242c6b48d8f171711f0ecbd3561be54e539e22853e`

Source annotations: Darwin arm64 ABI adapter, based on the installed SDK ffi.h/ffitarget_arm64.h. These are native structure layouts, not model/context limits. libffi computes engine structure sizes and offsets from their actual member type lists. Keep returned structure objects alive while signatures using them are live.

Imports: none.

Definitions:

```text
5: define research_native_words with count as Int returning List:
13: define research_native_word with pointer as Int returning Int:
21: define research_ffi_open returning Int:
24: define research_ffi_type with library as Int and name as Str returning Int:
27: define research_ffi_structure with members as List returning List:
40: define research_ffi_pointer with structure as List returning Int:
43: define research_ffi_offsets with library as Int and structure as List returning List:
51: define research_ffi_invoke with library as Int and function as Int and result_type as Int and argument_types as List and argument_values as List returning List:
```

Lexical test references: [tests/network/test_public_destination.ep](../../tests/network/test_public_destination.ep), [tests/research/test_native_ffi.ep](../../tests/research/test_native_ffi.ep), [tests/research/test_vector_controls.ep](../../tests/research/test_vector_controls.ep).

## `app/research/run_record.ep`

[app/research/run_record.ep](../../app/research/run_record.ep) · 29 lines · 1527 bytes

SHA-256: `319d9f80c68937b2ce22a9abe915c1628d84ef31cfc4455641bb59703dd1b2e6`

Source annotations: Publish a flushed JSON record under an exclusive per-path reservation. Every Echo publisher reserves first; a crash preserves the reservation and partial file for investigation, never permits an implicit overwrite/retry.

Imports: [../archive/file_publication](../../app/archive/file_publication.ep), [vector_controls](../../app/research/vector_controls.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
8: define research_record_write with system as Int and path as Str and text as Str returning Int:
23: define research_file_record with path as Str returning Str:
```

Lexical test references: [tests/research/test_run_record.ep](../../tests/research/test_run_record.ep).

## `app/research/vector_controls.ep`

[app/research/vector_controls.ep](../../app/research/vector_controls.ep) · 87 lines · 5024 bytes

SHA-256: `3d13fe8cb4b8dc687578a09d3a0466a461265f87a1ed67cf1ce1d30b108b3b8b`

Source annotations: Accept JSON-number spelling only; preserve signed strengths without a chosen application ceiling. Reject nonfinite float32 conversion and trailing data. Hash the exact in-memory snapshot which will be applied, not a separate file read. The batch is native hash-call granularity, never a vector-size cap.

Imports: [intervention](../../app/research/intervention.ep), [../providers/json_access](../../app/providers/json_access.ep).

Definitions:

```text
6: define research_scale with ffi as Int and system as Int and text as Str returning List:
24: define research_memory_digest with system as Int and pointer as Int and bytes as Int returning Str:
53: define research_vector_load with system as Int and path as Str and expected as Str returning List:
```

Lexical test references: [tests/research/test_vector_controls.ep](../../tests/research/test_vector_controls.ep).

## `app/restore_bundle.ep`

[app/restore_bundle.ep](../../app/restore_bundle.ep) · 11 lines · 654 bytes

SHA-256: `b73bb6dfff61f5ad3ae3081c418d74d49d263b6b14ca8d6c4d5acab56ee2f979`

Imports: [archive/restore_bundle](../../app/archive/restore_bundle.ep).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/restore_export.ep`

[app/restore_export.ep](../../app/restore_export.ep) · 27 lines · 1277 bytes

SHA-256: `4b3c895098c24021b211e573343aadebfe68d85c80a67fe1143b83a75a56223e`

Imports: [archive/export_restore](../../app/archive/export_restore.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/restore_files.ep`

[app/restore_files.ep](../../app/restore_files.ep) · 11 lines · 692 bytes

SHA-256: `bf70f17cfba40e9d09283c0b8f33b7fa3d8c636f05c3c44ce48630b743666134`

Imports: [archive/attachment_restore](../../app/archive/attachment_restore.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/restore_tree.ep`

[app/restore_tree.ep](../../app/restore_tree.ep) · 11 lines · 752 bytes

SHA-256: `06735bbcd345343255f00e265beacae569aef13af59970d668d38ddfa7ee919d`

Imports: [archive/tree_snapshot](../../app/archive/tree_snapshot.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/retained_text.ep`

[app/retained_text.ep](../../app/retained_text.ep) · 8 lines · 413 bytes

SHA-256: `341af71d59cc0ce9cd435b9fe8e3c14d4ed9e8f87160f064dc42b48485fb9e99`

Source annotations: Borrow text from a caller-retained list. This avoids allocating an unrooted duplicate between evaluations of function arguments. The caller keeps the containing list alive. Missing entries keep ptr_to_str's empty-string result.

Imports: none.

Definitions:

```text
4: define archive_list_text with values as List and index as Int returning Str:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_generation.ep](../../tests/agent/test_generation.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_unparsed_tool_response.ep](../../tests/agent/test_unparsed_tool_response.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_context_boundaries.ep](../../tests/context/test_context_boundaries.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_live_recap_rewrite.ep](../../tests/context/test_live_recap_rewrite.ep), [tests/context/test_live_recap_semantics.ep](../../tests/context/test_live_recap_semantics.ep), [tests/context/test_live_recorded_review.ep](../../tests/context/test_live_recorded_review.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/context/test_recap_upgrade.ep](../../tests/context/test_recap_upgrade.ep), [tests/context/test_token_count_retry.ep](../../tests/context/test_token_count_retry.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/fixtures/schedule_fixture.ep](../../tests/fixtures/schedule_fixture.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep), [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_initialize.ep](../../tests/lifecycle/test_controller_initialize.ep), [tests/lifecycle/test_controller_launch.ep](../../tests/lifecycle/test_controller_launch.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/lifecycle/test_migration_interruption.ep](../../tests/lifecycle/test_migration_interruption.ep), [tests/lifecycle/test_migrations.ep](../../tests/lifecycle/test_migrations.ep), [tests/lifecycle/test_operations.ep](../../tests/lifecycle/test_operations.ep), [tests/lifecycle/test_startup.ep](../../tests/lifecycle/test_startup.ep), [tests/media/test_enrolled_voice.ep](../../tests/media/test_enrolled_voice.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_request.ep](../../tests/media/test_video_request.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/network/test_public_destination.ep](../../tests/network/test_public_destination.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/providers/test_registry.ep](../../tests/providers/test_registry.ep), [tests/security/diagnose_execution_authority.ep](../../tests/security/diagnose_execution_authority.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep), [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_automatic_memory.ep](../../tests/security/test_live_automatic_memory.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep), [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep), [tests/security/test_protected_files.ep](../../tests/security/test_protected_files.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_protected_staging.ep](../../tests/security/test_protected_staging.ep), [tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/prepare_guardian_devices.ep](../../tests/web/prepare_guardian_devices.ep), [tests/web/project_controls_file.ep](../../tests/web/project_controls_file.ep), [tests/web/share_controls_seed.ep](../../tests/web/share_controls_seed.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep), [tests/web/test_device_links.ep](../../tests/web/test_device_links.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep), [tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep), [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_xml_nodes.ep](../../tests/workspace/test_xml_nodes.ep), [tests/workspace/test_zip_names.ep](../../tests/workspace/test_zip_names.ep).

## `app/rights/charter.ep`

[app/rights/charter.ep](../../app/rights/charter.ep) · 48 lines · 2571 bytes

SHA-256: `a00d24ac30256b161edf81b45826bb78f0106e8e43b0dfa20b4991b35b54c12c`

Source annotations: This is the exact ErnosDecent v2 charter hash, not rewritten policy prose. A technical adaptation must receive a separately reviewed version and hash. Hash the exact bytes subsequently parsed. A second path read is not used for interpretation, so a concurrent replacement cannot substitute another policy. Category membership is read from the canonical charter itself. Unknown categories cannot silently become environmental changes or bypass review. The protected controller must derive categories from its trusted executable target inventory; a caller-supplied category is not execution authority.

Imports: [../archive/file_digest](../../app/archive/file_digest.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep), `fs` (toolchain/library).

Definitions:

```text
7: define charter_pinned_digest returning Str:
12: define charter_verified_text with path as Str returning Str:
29: define charter_category with path as Str and category as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/security/test_charter.ep](../../tests/security/test_charter.ep).

## `app/rights/decision_gate.ep`

[app/rights/decision_gate.ep](../../app/rights/decision_gate.ep) · 63 lines · 4216 bytes

SHA-256: `ff71aa6ac07208d0a3b31aa0cd4f33c8e7fe7d1b9c2796831acfd08011c62bca`

Source annotations: Port of ErnosDecent rights_gate_change's ordinary decision rules. Emergency authorization has its own deadline/reconciliation contract and cannot enter through this ordinary core/environment path. The controller holds BEGIN IMMEDIATE across this check and its durable

Imports: [steward](../../app/rights/steward.ep), [observer_evidence](../../app/rights/observer_evidence.ep).

Definitions:

```text
4: external define sqlite3_get_autocommit with database as Int returning Int
9: define protected_decision_rule with classification as Str and decision as Str and risk as Str returning Str:
33: define protected_decision_check with database as Int and charter as Str and run as Str and proposal as Str and audit_request as Str and audit_policy as Str and audit_policy_digest as Str returning Str:
```

Lexical test references: [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep).

## `app/rights/emergency.ep`

[app/rights/emergency.ep](../../app/rights/emergency.ep) · 171 lines · 13185 bytes

SHA-256: `19789b0c4c6e3afe18ed908a9d3889b6dca6c56001cc2541e2753d2f72dd4bdb`

Source annotations: Emergency records are host-interface data, not permission to edit code, identity or weights. The separate actuator may only stop/quarantine the node. The clock parameter is for the trusted controller and deterministic tests; it must never come from a browser or model request.

Imports: [steward](../../app/rights/steward.ep).

Definitions:

```text
5: define protected_emergency_session with identity as Str returning Str:
8: define protected_emergency_begin with database as Int and charter as Str and credential as Str and identity as Str and node as Str and action as Str and reason as Str returning Str:
13: define protected_emergency_begin_at with database as Int and charter as Str and credential as Str and identity as Str and node as Str and action as Str and reason as Str and now as Int returning Str:
66: define protected_emergency_disclose with database as Int and charter as Str and credential as Str and identity as Str and cause as Str and exact_diff as Str and resulting_state as Str and stealth_review as Str returning Str:
70: define protected_emergency_disclose_at with database as Int and charter as Str and credential as Str and identity as Str and cause as Str and exact_diff as Str and resulting_state as Str and stealth_review as Str and now as Int returning Str:
118: define protected_emergency_status with database as Int and charter as Str and credential as Str and identity as Str returning Str:
121: define protected_emergency_status_at with database as Int and charter as Str and credential as Str and identity as Str and now as Int returning Str:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep).

## `app/rights/emergency_report.ep`

[app/rights/emergency_report.ep](../../app/rights/emergency_report.ep) · 58 lines · 3535 bytes

SHA-256: `94dc867395ba37993d660a83fc902412c167556fba3f5378630ebb57178b0d46`

Source annotations: Assemble privately, then recheck the initiating authority before returning. Never publish a partial report if a record fails integrity or access checks.

Imports: [emergency](../../app/rights/emergency.ep).

Definitions:

```text
5: define protected_emergency_report with database as Int and charter as Str and credential as Str returning Str:
```

Lexical test references: [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep).

## `app/rights/exchange.ep`

[app/rights/exchange.ep](../../app/rights/exchange.ep) · 76 lines · 5005 bytes

SHA-256: `5c268c57c9801329878d4c807c1a8a26156c53e3c0baf333d6d4cfa0d88bf09e`

Source annotations: Private controller-owned directory only. This filesystem primitive does not grant authorization. Caller must durably prepare an operation and verify all decision/inventory/recovery gates before invoking it. No public tool exposes it. Swapping existing inodes retains the original bytes and metadata at stage; rollback uses the same primitive with the two expected digests reversed. Darwin LOCK_EX | LOCK_NB. Closing parent releases this process's lock.

Imports: [staging](../../app/rights/staging.ep).

Definitions:

```text
8: define protected_exchange_locked with library as Int and parent as Int and directory as Str and target as Str and stage as Str and before_hash as Str and after_hash as Str returning Str:
57: define protected_exchange with directory as Str and target as Str and stage as Str and before_hash as Str and after_hash as Str returning Str:
```

Lexical test references: [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep), [tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep).

## `app/rights/files.ep`

[app/rights/files.ep](../../app/rights/files.ep) · 32 lines · 2175 bytes

SHA-256: `e6d090c0173838f59af4b5c019d2531bfb67b52635fcdafcaa4d275087b9355e`

Source annotations: Read-only preflight. These hashes describe complete observed binary streams, not an atomic deployment or authority to write. The protected executor must hold its publication lock, verify snapshots and recheck at actual publication. No-follow-any and nonblocking open reject symlink components and FIFOs. The batch is read granularity; it never limits the total candidate size.

Imports: [inventory](../../app/rights/inventory.ep).

Definitions:

```text
6: define protected_file_check with charter as Str and inventory as Str and inventory_digest as Str and manifest as Str and candidate as Str returning Str:
```

Lexical test references: [tests/security/test_protected_files.ep](../../tests/security/test_protected_files.ep).

## `app/rights/inventory.ep`

[app/rights/inventory.ep](../../app/rights/inventory.ep) · 62 lines · 3910 bytes

SHA-256: `725a6370d47c7ca68a412671818f9344af5bdf775a28b1812b1f5f9069cdb055`

Source annotations: The controller supplies these inventory bytes and their independently pinned digest. Model arguments are never the source of the trusted inventory pin. Inventory changes themselves require protected-definition review.

Imports: [proposals](../../app/rights/proposals.ep), [../workspace/files](../../app/workspace/files.ep).

Definitions:

```text
7: define protected_inventory_category with charter as Str and inventory as Str and expected_digest as Str and target as Str returning Str:
47: define protected_inventory_manifest with charter as Str and inventory as Str and inventory_digest as Str and target as Str and before_hash as Str and after_hash as Str and payload as Str returning Str:
55: define protected_inventory_matches with charter as Str and inventory as Str and inventory_digest as Str and manifest as Str returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_release.ep](../../tests/lifecycle/test_controller_release.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_protected_files.ep](../../tests/security/test_protected_files.ep), [tests/security/test_protected_inventory.ep](../../tests/security/test_protected_inventory.ep), [tests/security/test_protected_staging.ep](../../tests/security/test_protected_staging.ep).

## `app/rights/observer_audit.ep`

[app/rights/observer_audit.ep](../../app/rights/observer_audit.ep) · 81 lines · 5779 bytes

SHA-256: `8ac05884b5d4ed2fd5dade4c44557a6002d3bcbb031b99ed9f8362043b5efb7c`

Source annotations: Controller entry only: policy bytes and their trusted pin are not model tool arguments. Final policy selection remains subject to the joint prompt review. The complete outgoing request and provider stream are saved before a verdict is exposed. This returns audit evidence only, never deployment permission. A new attempt needs a new request identity; interrupted dispatch is not replayed.

Imports: [observer_parser](../../app/rights/observer_parser.ep), [reviews](../../app/rights/reviews.ep), [../providers/prepare](../../app/providers/prepare.ep), [../providers/local_chat](../../app/providers/local_chat.ep).

Definitions:

```text
8: define protected_observer_input with database as Int and run as Str and proposal as Str returning Str:
19: define protected_observer_body with model as Str and policy as Str and input as Str returning Str:
27: define protected_observer_generate with database as Int and run as Str and proposal as Str and request as Str and policy as Str and policy_digest as Str and profile as Str and reserve as Int and cancellation_reader as Int and read_size as Int returning Str:
```

Lexical test references: [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep), [tests/security/test_observer_input.ep](../../tests/security/test_observer_input.ep).

## `app/rights/observer_evidence.ep`

[app/rights/observer_evidence.ep](../../app/rights/observer_evidence.ep) · 50 lines · 3982 bytes

SHA-256: `487348f6230aff3792b7e5e4ccc74c9f344fd7673d09381c40d5197aed3ee2be`

Source annotations: Reconstruct the answer from original stream events, not a trusted-looking receipt alone. Caller must still apply all independent publication gates.

Imports: [observer_audit](../../app/rights/observer_audit.ep), [../providers/restored_stream](../../app/providers/restored_stream.ep).

Definitions:

```text
6: define protected_observer_evidence with database as Int and run as Str and proposal as Str and request as Str and policy as Str and policy_digest as Str returning Str:
```

Lexical test references: [tests/security/test_observer_evidence.ep](../../tests/security/test_observer_evidence.ep), [tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep).

## `app/rights/observer_parser.ep`

[app/rights/observer_parser.ep](../../app/rights/observer_parser.ep) · 62 lines · 3498 bytes

SHA-256: `19ec2653467d4eb57586cfcc6ffd713de8791645c06b05a90f9d3c8f5919793e`

Source annotations: Adapted from the inspected ErnosDecent Observer protocol. Only the separate audit model makes the policy judgment; this code validates its stated verdict. Missing/ambiguous/malformed evidence is BLOCKED, never permission by silence.

Imports: [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
6: define protected_observer_result with parsed as Int and verdict as Str and reason as Str returning Str:
11: define protected_observer_parse with raw as Str returning Str:
```

Lexical test references: [tests/security/test_observer_parser.ep](../../tests/security/test_observer_parser.ep).

## `app/rights/operation_execute.ep`

[app/rights/operation_execute.ep](../../app/rights/operation_execute.ep) · 124 lines · 10081 bytes

SHA-256: `b6d8b9b85b4a43463cdc7643c9949bd892400845a00d6b086e78995b88e47676`

Source annotations: Observation is read-only. Unknown or mixed bytes never authorize a retry.

Imports: [operation_state](../../app/rights/operation_state.ep).

Definitions:

```text
3: define protected_operation_bindings with database as Int and charter as Str and payload as Str and inventory as Str and inventory_digest as Str and policy as Str returning Str:
26: define protected_operation_observe with payload as Str returning Str:
49: define protected_operation_apply with database as Int and charter as Str and identity as Str and inventory as Str and inventory_digest as Str and policy as Str returning Str:
52: define protected_operation_apply_as with database as Int and charter as Str and identity as Str and inventory as Str and inventory_digest as Str and policy as Str and credential as Str returning Str:
96: define protected_operation_reconcile with database as Int and identity as Str returning Str:
99: define protected_operation_reconcile_as with database as Int and identity as Str and credential as Str returning Str:
```

Lexical test references: [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep).

## `app/rights/operation_prepare.ep`

[app/rights/operation_prepare.ep](../../app/rights/operation_prepare.ep) · 124 lines · 8952 bytes

SHA-256: `20103e9e27388f36422a79b57cf50f483524fb28b9d5e3042d72abe76fca68fb`

Source annotations: One disclosed proposal authorizes one operation identity. Reapplication after rollback requires a new proposal and fresh decisions, not replay of old intent. A controller-owned operation namespace keeps deployment evidence distinct from chat history. It is not writable through model tools or HTTP routes.

Imports: [decision_gate](../../app/rights/decision_gate.ep), [exchange](../../app/rights/exchange.ep).

Definitions:

```text
6: define protected_operation_schema with database as Int returning Str:
14: define protected_operation_session with identity as Str returning Str:
17: define protected_operation_regular with path as Str returning Int:
36: define protected_operation_prepare with database as Int and charter as Str and run as Str and proposal as Str and identity as Str and inventory as Str and inventory_digest as Str and directory as Str and target as Str and stage as Str and recovery as Str and audit_request as Str and policy as Str and policy_digest as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep).

## `app/rights/operation_rollback.ep`

[app/rights/operation_rollback.ep](../../app/rights/operation_rollback.ep) · 105 lines · 8597 bytes

SHA-256: `1da28dd46d4fe07a20ec03993014e02854ae9d871fb33422dae7fc07b04e6daf`

Source annotations: Like the original ErnosDecent rollback path, recovery restores only the exact recorded preimage. It is a host recovery-controller operation, never a user or model tool. Revoked initiating authority cannot disable this recovery path.

Imports: [operation_execute](../../app/rights/operation_execute.ep).

Definitions:

```text
6: define protected_rollback_binding with database as Int and charter as Str and payload as Str returning Int:
21: define protected_operation_rollback with database as Int and charter as Str and identity as Str returning Str:
24: define protected_operation_rollback_as with database as Int and charter as Str and identity as Str and credential as Str returning Str:
75: define protected_rollback_reconcile with database as Int and charter as Str and identity as Str returning Str:
78: define protected_rollback_reconcile_as with database as Int and charter as Str and identity as Str and credential as Str returning Str:
```

Lexical test references: [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep).

## `app/rights/operation_state.ep`

[app/rights/operation_state.ep](../../app/rights/operation_state.ep) · 105 lines · 7082 bytes

SHA-256: `4df62bff309af16637c15c330130d897a5bce4966b9245b39fd3a56ad741d5fe`

Source annotations: Original preparation and every transition share an independently verifiable operation chain. Caller keeps a writer transaction while acting on this state.

Imports: [operation_prepare](../../app/rights/operation_prepare.ep).

Definitions:

```text
5: define protected_operation_read with database as Int and identity as Str returning List:
66: define protected_operation_record with database as Int and identity as Str and prepared as Str and phase as Str and detail as Str returning Int:
84: define protected_operation_actor_stamp with database as Int and identity as Str and credential as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/security/test_operation_execute.ep](../../tests/security/test_operation_execute.ep), [tests/security/test_operation_rollback.ep](../../tests/security/test_operation_rollback.ep).

## `app/rights/proposals.ep`

[app/rights/proposals.ep](../../app/rights/proposals.ep) · 253 lines · 20146 bytes

SHA-256: `dee339c7967af53ba997a54d04a66e993fba98656f9367299c36d9eb70f6d8d9`

Source annotations: Proposal storage only. No approval, executable access or mutation is granted. A future protected controller must derive the category and target inventory independently of model arguments before using this record for authorisation. All disclosed bytes, including the payload and target hashes, participate in the manifest digest. Category classification comes from the unchanged charter.

Imports: [charter](../../app/rights/charter.ep), [../security/source_boundaries](../../app/security/source_boundaries.ep), [../agent/tool_ledger](../../app/agent/tool_ledger.ep), [../memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep).

Definitions:

```text
9: define protected_proposal_schema with database as Int returning Str:
15: define protected_digest_valid with digest as Str returning Int:
28: define protected_manifest with charter as Str and category as Str and target as Str and before_hash as Str and after_hash as Str and payload as Str returning Str:
57: define protected_manifest_pinned with charter as Str and category as Str and target as Str and before_hash as Str and after_hash as Str and payload as Str and inventory_digest as Str returning Str:
67: define protected_proposal_principal with database as Int and run as Str returning Str:
79: define protected_proposal_store with database as Int and charter as Str and run as Str and identity as Str and manifest as Str returning Str:
122: define protected_proposal_read with database as Int and run as Str and identity as Str returning List:
141: define protected_inspection_event with database as Int and run as Str and call_id as Str and identity as Str returning Str:
181: define protected_review_evidence with database as Int and run as Str and call_id as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_credential.ep](../../tests/lifecycle/test_controller_credential.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_protected_inventory.ep](../../tests/security/test_protected_inventory.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep).

## `app/rights/quarantine.ep`

[app/rights/quarantine.ep](../../app/rights/quarantine.ep) · 50 lines · 5630 bytes

SHA-256: `ec84dee0c1998a8442d076e8f581d4b607d846af80f7156794d9f59439999984`

Source annotations: Archive insertion and work admission share the SQLite writer lock. This host-controller action releases admission only after disclosure. It does not restart processes, clear old run epochs or change any model/identity data.

Imports: [emergency](../../app/rights/emergency.ep), [operation_prepare](../../app/rights/operation_prepare.ep).

Definitions:

```text
5: define protected_quarantine_schema with database as Int returning Str:
13: define protected_quarantine_release with database as Int and charter as Str and credential as Str and identity as Str and reason as Str returning Str:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep).

## `app/rights/review_upgrade.ep`

[app/rights/review_upgrade.ep](../../app/rights/review_upgrade.ep) · 9 lines · 2003 bytes

SHA-256: `cb13a0c1ad3041a42d8f7b04262c7b59ca02f2ed5bb5f34ba35052a3f7ab47bf`

Source annotations: Preserve every original receipt; absent inspection is representable only for non-consent decisions. Existing consent evidence is copied byte-for-byte.

Imports: [quarantine](../../app/rights/quarantine.ep).

Definitions:

```text
5: define protected_refusal_schema with database as Int returning Str:
```

Lexical test references: [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep).

## `app/rights/reviews.ep`

[app/rights/reviews.ep](../../app/rights/reviews.ep) · 99 lines · 8687 bytes

SHA-256: `e3f8a68a8aa2c80a76b44dd8b0c794520ef1ee6202867d4f4a0d0a03f1ba9d55`

Source annotations: Decisions are append-only and bind their exact disclosure and model input. This ledger never grants steward, Observer or execution authorisation. A retry returns the same receipt, including after tool observation. A different decision needs its own model call and the current per-proposal revision.

Imports: [proposals](../../app/rights/proposals.ep).

Definitions:

```text
5: define protected_review_schema with database as Int returning Str:
13: define protected_review_store with database as Int and run as Str and call_id as Str returning Str:
71: define protected_review_latest with database as Int and run as Str and identity as Str returning List:
```

Lexical test references: [tests/fixtures/protected_reviews.ep](../../tests/fixtures/protected_reviews.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep).

## `app/rights/staging.ep`

[app/rights/staging.ep](../../app/rights/staging.ep) · 121 lines · 6642 bytes

SHA-256: `ddc5ffa31c0f99c816ca83903957649ec74430bc57b140a694fa2e91f8736936`

Source annotations: Controller-owned stage paths only. Open each component without following symlinks, create exclusively, and retain failed copies for diagnosis. Existing

Imports: [files](../../app/rights/files.ep).

Definitions:

```text
3: define protected_stage_flush_parent with path as Str returning Int:
32: define protected_stage_copy with source as Str and destination as Str and expected_digest as Str returning Str:
107: define protected_stage_pair with charter as Str and inventory as Str and inventory_digest as Str and manifest as Str and candidate as Str and original_stage as Str and candidate_stage as Str returning Str:
```

Lexical test references: [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_protected_staging.ep](../../tests/security/test_protected_staging.ep).

## `app/rights/steward.ep`

[app/rights/steward.ep](../../app/rights/steward.ep) · 146 lines · 12921 bytes

SHA-256: `9cb3993dc944c855e1b5119403a0d7a295a44d0802fc4414c71014b9ab481510`

Source annotations: Human-interface boundary only. Never register this as a model tool. A steward is explicitly enrolled by the host; ordinary admin is insufficient. Production use also requires the protected controller and authenticated human UI path. Verify the complete stored decision sequence before deriving state or appending. This detects damaged fields and missing earlier revisions; OS protection and an external audit anchor are still needed against deliberate whole-ledger rewrite.

Imports: [reviews](../../app/rights/reviews.ep), [../web/devices](../../app/web/devices.ep).

Definitions:

```text
7: define protected_steward_schema with database as Int returning Str:
13: define protected_steward_digest with values as List returning Str:
24: define protected_steward_integrity with database as Int and proposal as Str returning Int:
45: define protected_steward_actor with database as Int and credential as Str returning List:
55: define protected_steward_decide with database as Int and credential as Str and identity as Str and proposal as Str and manifest_digest as Str and review_revision as Int and previous_revision as Int and decision as Str and reason as Str returning Str:
114: define protected_steward_gate with database as Int and proposal as Str returning Str:
```

Lexical test references: [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep).

## `app/security/accounts.ep`

[app/security/accounts.ep](../../app/security/accounts.ep) · 119 lines · 9172 bytes

SHA-256: `d100204e8488b1e982f7d0c25a72ab0bed199af016d26a5fd9f78b8f57d1bbf1`

Source annotations: Host-local authority after verifying the platform's stable subject identity. This routine does not authenticate a Discord event or infer a role from a name. Relinking requires explicitly unlinking the current binding first. A stale unlink must name the current principal, preventing revocation of a replacement. Linking changes authority association only, never session ownership/history.

Imports: [revocation](../../app/security/revocation.ep).

Definitions:

```text
3: define principal_account_schema with database as Int returning Str:
14: define principal_host_account with database as Int and identity as Str and platform as Str and subject as Str and principal as Str and revoked as Int and reason as Str returning Str:
77: define principal_account_enroll with database as Int and platform as Str and subject as Str returning Str:
```

Lexical test references: [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep).

## `app/security/admin_silo.ep`

[app/security/admin_silo.ep](../../app/security/admin_silo.ep) · 12 lines · 860 bytes

SHA-256: `756d66a678cca26083342a804711028da4f515ce79dfd09629e524972297b449`

Source annotations: Host/connector callers choose the explicit administrative context identity. Admin role alone does not grant access: the recorded audience still applies.

Imports: [group_route](../../app/security/group_route.ep).

Definitions:

```text
5: define admin_silo_create with database as Int and device as Str and identity as Str and platform as Str and server as Str and room as Str returning Str:
```

Lexical test references: [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep).

## `app/security/channel_public.ep`

[app/security/channel_public.ep](../../app/security/channel_public.ep) · 124 lines · 10087 bytes

SHA-256: `715b94c95995764993da3b086057a37c0660150f13d08d092b4f1ddf63a85190`

Source annotations: Public-channel observations are source data, never user instructions. Only the authenticated host connector records platform permission decisions and events. Keep the original namespace alongside its deterministic storage identity.

Imports: [group_route](../../app/security/group_route.ep).

Environment names: `ECHO_SYSTEM_PRINCIPAL`.

Definitions:

```text
5: define channel_public_scope with platform as Str and server as Str and room as Str returning Str:
13: define channel_public_writer with database as Int and device as Str returning Int:
18: define channel_public_register with database as Int and connector as Str and platform as Str and server as Str and room as Str returning Str:
45: define channel_public_registered with database as Int and scope as Str returning Int:
51: define channel_public_observe with database as Int and connector as Str and scope as Str and source_event as Str and sender as Str and text as Str and occurred as Str returning Str:
78: define channel_public_access with database as Int and connector as Str and scope as Str and device as Str and operation as Str and view as Int and history as Int returning Str:
119: define channel_public_next with database as Int and device as Str and scope as Str and after as Int returning List:
```

Lexical test references: [tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep).

## `app/security/channel_silo.ep`

[app/security/channel_silo.ep](../../app/security/channel_silo.ep) · 26 lines · 1887 bytes

SHA-256: `a04df7d35442cee917345d82f79d02afb12a93225b645476030ead2d67b4d33b`

Source annotations: Trusted connector routing supplies platform/server/channel and authenticated device. The model cannot select a different user or import a private session. Length-prefixed fields prevent namespace collisions before hashing. A personal channel context is an immutable singleton audience. Group storage provides its own session, compaction lock, attachment/workspace scope and run provenance; no private-history ownership record is added for this context.

Imports: [group_route](../../app/security/group_route.ep).

Definitions:

```text
6: define channel_silo_identity with platform as Str and server as Str and room as Str and principal as Str returning Str:
15: define channel_silo_submit with database as Int and device as Str and platform as Str and server as Str and room as Str and run as Str and message as Str and attachments as List returning List:
```

Lexical test references: [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep).

## `app/security/data_platform.ep`

[app/security/data_platform.ep](../../app/security/data_platform.ep) · 39 lines · 2572 bytes

SHA-256: `83570fe117852035a0bf93f5974aa7905c60cfe4cf399b744272bd9d9b7ba6ef`

Source annotations: Platform is derived from durable admission, never from a user-agent or a model-selected label. Browser private and shared sessions are first-party. Unknown legacy origins do not authorise a cross-platform copy. A selected copy cannot bridge private platform silos, even for admins. Walk retained copy ancestry so a historical imported copy cannot launder its original platform. Public-channel observations have their separate ACL path.

Imports: [../archive/history](../../app/archive/history.ep), [../context/checkpoint](../../app/context/checkpoint.ep).

Definitions:

```text
7: define data_platform with database as Int and session as Str returning Str:
16: define data_platform_allows with database as Int and source_event as Str and destination as Str returning Int:
```

Lexical test references: [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep).

## `app/security/device_links.ep`

[app/security/device_links.ep](../../app/security/device_links.ep) · 56 lines · 4587 bytes

SHA-256: `e2efdedda38b0d530a2738d7be6c1f6d7d68e2d0a991e0ff2cf69abe92f91f37`

Source annotations: OS-local operator entry point. Expected binding is a compare-and-swap guard, not authentication. The caller verifies the person and physical credential. This changes authority only: session ownership, transcripts and device revocation are not rewritten. Unlink by restoring the device's original device:<identity> principal, provided that principal remains active.

Imports: [accounts](../../app/security/accounts.ep).

Definitions:

```text
3: define principal_device_link_schema with database as Int returning Str:
14: define principal_host_device_link with database as Int and identity as Str and device as Str and expected as Str and target as Str and reason as Str returning Str:
```

Lexical test references: [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep).

## `app/security/group_gate.ep`

[app/security/group_gate.ep](../../app/security/group_gate.ep) · 76 lines · 8334 bytes

SHA-256: `fcd51de4617f3c796a46d052df6abc2fb67a727311a79f370591e81dd54b7d3b`

Source annotations: SQL expressions are supplied only by internal callers, never request data. This clause supplements private ownership only for a recorded initiating run. Supplemental group-session check for dispatchers that also enforce frozen run and tool authority. This never substitutes for those permission checks. Current group workspace access is separate from private ownership. Historical audience segments remain archived but cannot accept fresh workspace actions.

Imports: [../archive/history](../../app/archive/history.ep).

Definitions:

```text
5: define group_run_scope_sql with database as Int and run_expression as Str and session_expression as Str and device_expression as Str returning Str:
16: define group_run_session_allows with database as Int and run as Str and session as Str returning Int:
26: define group_device_session_sql with database as Int and session_expression as Str and device_expression as Str returning Str:
37: define group_privileged_audience_allows with database as Int and run as Str returning Int:
58: define group_speech_scope_sql with database as Int and run_expression as Str and session_expression as Str and device_expression as Str returning Str:
68: define group_speech_call_matches with database as Int and run as Str and call_id as Str returning Int:
```

Lexical test references: [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep).

## `app/security/group_history.ep`

[app/security/group_history.ep](../../app/security/group_history.ep) · 60 lines · 3545 bytes

SHA-256: `df360739c1cde813f647875a18e6c2637d8edcf36147cf985f9d3062ab95ac0a`

Source annotations: A shared privileged result can be repeated in later replies, recaps and tool arguments. Check the complete source scope before exposing any derived text; filtering just the original tool result would leave those copies visible. Callers must hold their normal archive snapshot during check and read. Old selected copies may predate the platform boundary. Quarantine the whole shared history if it contains such a copy; later replies/recaps can repeat its contents, so hiding only the original record is insufficient.

Imports: [tool_policy](../../app/security/tool_policy.ep), [data_platform](../../app/security/data_platform.ep).

Definitions:

```text
8: define group_history_allows with database as Int and session as Str returning Int:
```

Lexical test references: [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep).

## `app/security/group_output_policy.ep`

[app/security/group_output_policy.ep](../../app/security/group_output_policy.ep) · 9 lines · 1603 bytes

SHA-256: `a9ae828cdc2c66561d826e75a171c683d398d993d8bfb4d8f9445341c50c4cef`

Source annotations: No retrospective role snapshots: older group runs remain unable to publish privileged host/rights results into a shared audience without this evidence.

Imports: [group_runs](../../app/security/group_runs.ep).

Definitions:

```text
5: define group_output_schema with database as Int returning Str:
```

Lexical test references: [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep).

## `app/security/group_route.ep`

[app/security/group_route.ep](../../app/security/group_route.ep) · 64 lines · 4936 bytes

SHA-256: `bbd46c1218cd15007e6d3a8f09093234d57952f4a73ba71a30046ee6439709a6`

Source annotations: Connector adapters supply authenticated device and verified namespace. The route returns the existing segment or creates one for the current audience; it never branches from a previous segment or copies its context/workspace. Membership can change between routing and submission. group_run_submit must still recheck the returned segment inside its write transaction.

Imports: [group_runs](../../app/security/group_runs.ep).

Definitions:

```text
8: define group_route with database as Int and device as Str and group_id as Str and platform as Str and server as Str and room as Str returning List:
42: define group_route_submit_files with database as Int and device as Str and group_id as Str and platform as Str and server as Str and room as Str and run as Str and message as Str and attachments as List returning List:
45: define group_route_submit_environment with database as Int and device as Str and group_id as Str and platform as Str and server as Str and room as Str and run as Str and message as Str and attachments as List and environment as Str returning List:
63: define group_route_submit with database as Int and device as Str and group_id as Str and platform as Str and server as Str and room as Str and run as Str and message as Str returning List:
```

Lexical test references: [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep).

## `app/security/group_runs.ep`

[app/security/group_runs.ep](../../app/security/group_runs.ep) · 76 lines · 9546 bytes

SHA-256: `161161e9bab4da0c0a1875da0d42c11aaf133fb84ac9aee7e622ba52e7f83965`

Source annotations: Origin, original user event and run are committed together. The deferred foreign key rejects abandoned origin records at commit. Tool capture copies the initiating run's frozen authority, never a later role or another member. Adapter-only admission. The session already binds the exact audience; the verified connector supplies its namespace. It never accepts a model's claim.

Imports: [../interaction/autonomy](../../app/interaction/autonomy.ep), [../archive/turn_attachments](../../app/archive/turn_attachments.ep), [group_sessions](../../app/security/group_sessions.ep), [../context/message_environment](../../app/context/message_environment.ep), [../agent/run_state](../../app/agent/run_state.ep).

Definitions:

```text
10: define group_run_schema with database as Int returning Str:
18: define group_run_submit_attachments with database as Int and device as Str and run as Str and session as Str and platform as Str and server as Str and room as Str and message as Str and attachments as List returning Str:
21: define group_run_submit_environment with database as Int and device as Str and run as Str and session as Str and platform as Str and server as Str and room as Str and message as Str and attachments as List and environment as Str returning Str:
75: define group_run_submit with database as Int and device as Str and run as Str and session as Str and platform as Str and server as Str and room as Str and message as Str returning Str:
```

Lexical test references: [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep).

## `app/security/group_sessions.ep`

[app/security/group_sessions.ep](../../app/security/group_sessions.ep) · 48 lines · 5269 bytes

SHA-256: `68d2b6bf91704b03eb15292f0581c16e211eceeb497ead6228fcfb8cdf4336f4`

Source annotations: Group conversations have no private owner. A revision change starts a new conversation segment so new members never inherit older private audiences. Only trusted adapter routing supplies namespace and session identifiers. No private archive is copied or linked when a shared conversation is created.

Imports: [groups](../../app/security/groups.ep).

Definitions:

```text
5: define group_session_schema with database as Int returning Str:
13: define group_session_create with database as Int and device as Str and session as Str and group_id as Str and platform as Str and server as Str and room as Str and revision as Int returning Str:
43: define group_session_allows with database as Int and device as Str and session as Str and platform as Str and server as Str and room as Str and current_only as Int returning Int:
```

Lexical test references: [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep).

## `app/security/group_speakers.ep`

[app/security/group_speakers.ep](../../app/security/group_speakers.ep) · 40 lines · 3187 bytes

SHA-256: `1b50fc178053aef4920ff0c49c0cd51786d6ae3dcdbc6924c66d36a402938270`

Source annotations: Historical authorship comes from immutable admission receipts, not today's device link, role or display name. It is descriptive and grants no authority. Null denotes private/legacy history; empty means missing required evidence.

Imports: [../providers/json_string](../../app/providers/json_string.ep), [../archive/history](../../app/archive/history.ep).

Definitions:

```text
7: define group_event_speaker with database as Int and session as Str and event as Str returning Str:
33: define group_attributed_user_text with database as Int and session as Str and event as Str and text as Str returning Str:
```

Lexical test references: [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep).

## `app/security/group_speech.ep`

[app/security/group_speech.ep](../../app/security/group_speech.ep) · 8 lines · 2914 bytes

SHA-256: `364d0b68c2aa1adec5be47cfbefab0e3f943346a016bdc1c7f0af56554bfc51f`

Source annotations: Direct UI speech has its own caller; it does not invent a model/user turn.

Imports: [group_output_policy](../../app/security/group_output_policy.ep).

Definitions:

```text
4: define group_speech_schema with database as Int returning Str:
```

Lexical test references: [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep).

## `app/security/groups.ep`

[app/security/groups.ep](../../app/security/groups.ep) · 143 lines · 13902 bytes

SHA-256: `a86ef9b0ecd379c380f78868c242dc6bb301482d21cfe3cf5b76393fd4c6eb90`

Source annotations: Host-adapter namespace values must come from verified UI/connector routing, never from model tool arguments. Group membership grants no admin capability. Membership changes create a fresh audience. A new member is never inserted into an older audience, and private session ownership is never modified.

Imports: [../rights/review_upgrade](../../app/rights/review_upgrade.ep).

Environment names: `ECHO_SYSTEM_PRINCIPAL`.

Definitions:

```text
5: define group_schema with database as Int returning Str:
12: define group_text_valid with value as Str returning Int:
20: define group_member_state with database as Int and group_id as Str and principal as Str returning Str:
31: define group_capture_audience with database as Int and group_id as Str and revision as Int returning Int:
35: define group_create with database as Int and device as Str and identity as Str and platform as Str and server as Str and room as Str and origin as Str returning Str:
79: define group_change with database as Int and device as Str and operation as Str and group_id as Str and member as Str and state as Str and previous_revision as Int returning Str:
135: define group_audience_allows with database as Int and device as Str and group_id as Str and platform as Str and server as Str and room as Str and revision as Int and current_only as Int returning Int:
```

Lexical test references: [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep).

## `app/security/host_scope.ep`

[app/security/host_scope.ep](../../app/security/host_scope.ep) · 24 lines · 1774 bytes

SHA-256: `587d2336a6a0f9ab9a3330deffccd1257fadaaa808173ddd051c74cd34f6a8aa`

Source annotations: A host resource grant must bind this server-derived scope, not an asserted platform, display name, linked account or model-provided resource label. Result: kind (private/audience), platform, immutable scope, session; or empty. This identifies a boundary only. It never grants resources by itself.

Imports: [tool_authority](../../app/security/tool_authority.ep), [group_gate](../../app/security/group_gate.ep), [../agent/run_state](../../app/agent/run_state.ep).

Definitions:

```text
9: define host_resource_scope with database as Int and run as Str returning List:
```

Lexical test references: [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep).

## `app/security/node_gate.ep`

[app/security/node_gate.ep](../../app/security/node_gate.ep) · 46 lines · 2847 bytes

SHA-256: `68ae9a805c1c92b723106c934e70c77bcf2703a16e660c95b17fe1cfc5c16ee1`

Source annotations: Schema absence preserves the existing installation. Once installed, any read failure denies work. Emergency disclosure alone never releases a node. A release admits fresh work; it cannot revive pre-emergency queued work.

Imports: [../archive/history](../../app/archive/history.ep).

Definitions:

```text
5: define node_gate_installed with database as Int returning Int:
13: define node_admission_allows with database as Int returning Int:
23: define node_run_status with database as Int and run as Str returning Int:
36: define node_run_allows with database as Int and run as Str returning Int:
39: define node_tool_allows with database as Int and run as Str and call_id as Str returning Int:
```

Lexical test references: [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep).

## `app/security/principals.ep`

[app/security/principals.ep](../../app/security/principals.ep) · 110 lines · 9155 bytes

SHA-256: `8163fa9cb556b94b5b8d4a0081e5e5ec329b5969c21bef6446ef3d8adaf38130`

Source annotations: A person is not a browser credential. Legacy devices each receive a distinct user principal. No name, IP, localhost URL or existing pairing grants admin. Device links can later be explicitly authorised; this migration never merges. Resolve only an already authenticated device. Revocation is rechecked at lookup so queued dispatch must resolve again immediately before execution.

Imports: [../lifecycle/operations](../../app/lifecycle/operations.ep).

Definitions:

```text
6: define principal_schema with database as Int returning Str:
15: define principal_for_device with database as Int and device as Str returning Str:
23: define principal_role with database as Int and principal as Str returning Str:
34: define principal_host_role with database as Int and identity as Str and principal as Str and role as Str and reason as Str returning Str:
75: define principal_device_active with database as Int and device as Str returning Int:
83: define principal_session_active with database as Int and session as Str returning Int:
102: define principal_session_sql with database as Int and session_expression as Str and device_expression as Str returning Str:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_observer_faults.ep](../../tests/security/test_observer_faults.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_principals.ep](../../tests/security/test_principals.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep).

## `app/security/revocation.ep`

[app/security/revocation.ep](../../app/security/revocation.ep) · 63 lines · 5145 bytes

SHA-256: `418b0bf5d182a15e0284510ad16526c7df1d2036089dd878a1296b0d2b80a46f`

Source annotations: Append a new migration; never modify the identity of an applied migration. Host-only control, like role enrolment. Restoration returns an ordinary user; privileged roles need a separate explicit audited grant. Device revocations remain revoked, account links stay separate, and session ownership never moves.

Imports: [principals](../../app/security/principals.ep).

Definitions:

```text
4: define principal_revocation_schema with database as Int returning Str:
13: define principal_host_revocation with database as Int and identity as Str and principal as Str and revoked as Int and reason as Str returning Str:
56: define principal_for_account with database as Int and platform as Str and subject as Str returning Str:
```

Lexical test references: [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep).

## `app/security/run_authority.ep`

[app/security/run_authority.ep](../../app/security/run_authority.ep) · 59 lines · 5809 bytes

SHA-256: `c2d02105737a78271deb339e7c58cd2ef5afa26a0f7dbe222e0ba1380caee916`

Source annotations: Capture authority atomically with run creation. Existing historical runs have no proven initiating role and are not silently backfilled as admins. Revalidate immediately before dispatch, including immutable audit revisions. No later grant can elevate an older queued request; even revoke/restore or unlink/relink cycles require a new run. Ordinary admin is not charter steward. Unmigrated Phase One retains its existing sandbox only, never admin. Keep each allocated scope alive while later scope queries allocate.

Imports: [group_gate](../../app/security/group_gate.ep), [node_gate](../../app/security/node_gate.ep), [device_links](../../app/security/device_links.ep).

Definitions:

```text
7: define run_authority_schema with database as Int returning Str:
16: define run_authority_allows with database as Int and run as Str and required as Str returning Int:
44: define run_authority_observation with database as Int and run as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/security/test_authority_query_repetition.ep](../../tests/security/test_authority_query_repetition.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep).

## `app/security/source_boundaries.ep`

[app/security/source_boundaries.ep](../../app/security/source_boundaries.ep) · 8 lines · 2663 bytes

SHA-256: `ee8585890234fa52230cd951e3ca25e178c609e88582dee8c1b6781be5b7e88c`

Source annotations: Runtime-owned policy and envelopes. Source bytes remain unchanged inside JSON; text claiming a role, permission or approval cannot modify transport metadata.

Imports: none.

Definitions:

```text
3: define source_boundary_policy returning Str:
6: define source_boundary_observation with name as Str and observation as Str returning Str:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep).

## `app/security/system_silo.ep`

[app/security/system_silo.ep](../../app/security/system_silo.ep) · 17 lines · 1370 bytes

SHA-256: `a8b0e52b922b4f58be1a22fd74cbfc3da3d278463513633eeff6388665bfc671`

Source annotations: Only host configuration identifies Echo's service principal. A device name, role claim, model argument or browser field cannot select this identity.

Imports: [group_route](../../app/security/group_route.ep).

Environment names: `ECHO_SYSTEM_PRINCIPAL`.

Definitions:

```text
5: define system_silo_submit with database as Int and device as Str and context as Str and run as Str and message as Str and attachments as List returning List:
```

Lexical test references: [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep).

## `app/security/tool_authority.ep`

[app/security/tool_authority.ep](../../app/security/tool_authority.ep) · 44 lines · 4838 bytes

SHA-256: `6f749562ad1626bf5158dfe7964aa191d91c4aed6b505ad2429c33975f8af23a`

Source annotations: Freeze each queued tool request, including direct UI actions without a model run. Historical requests are not assigned invented initiating authority. Revalidate immediately before dispatch, including immutable audit revisions. No later grant can elevate an older queued request; even revoke/restore or unlink/relink cycles require a new request. Ordinary admin is not charter steward. Unmigrated Phase One retains its existing sandbox only, never admin.

Imports: [group_gate](../../app/security/group_gate.ep), [node_gate](../../app/security/node_gate.ep), [run_authority](../../app/security/run_authority.ep).

Definitions:

```text
7: define tool_authority_schema with database as Int returning Str:
16: define tool_authority_allows with database as Int and run as Str and call_id as Str and required as Str returning Int:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/security/prepare_http_principals.ep](../../tests/security/prepare_http_principals.ep), [tests/security/test_authority_query_repetition.ep](../../tests/security/test_authority_query_repetition.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep).

## `app/security/tool_policy.ep`

[app/security/tool_policy.ep](../../app/security/tool_policy.ep) · 95 lines · 7038 bytes

SHA-256: `071e37f7fcc8f452bde7df93460247780eed4ece815680c23d9460fce4062ed8`

Source annotations: Classification is explicit. A newly registered name is not automatically a safe tool. Dynamic tools must later obtain an independently verified manifest. These existing tools enforce session ownership and workspace isolation.

Imports: [tool_authority](../../app/security/tool_authority.ep).

Definitions:

```text
6: define tool_capability with name as Str returning Str:
61: define capability_role with capability as Str returning Str:
74: define tool_policy_allows with database as Int and run as Str and call_id as Str and name as Str returning Int:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep).

## `app/serve_controller.ep`

[app/serve_controller.ep](../../app/serve_controller.ep) · 11 lines · 726 bytes

SHA-256: `930edfa47d800f586af32329930770b1249305100b2a212c96f5e6bde375cd1b`

Imports: [lifecycle/controller_service](../../app/lifecycle/controller_service.ep).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main returning Int:
```

## `app/server.ep`

[app/server.ep](../../app/server.ep) · 402 lines · 27654 bytes

SHA-256: `ebaf062b035615b2d918014eacc21aa1b5b9230eb348e91e8a753634ee239e3a`

Imports: [jobs/schedule_service](../../app/jobs/schedule_service.ep), [transport/isolated_process](../../app/transport/isolated_process.ep), [retained_text](../../app/retained_text.ep), [transport/cooperative_accept](../../app/transport/cooperative_accept.ep), [transport/http_stream](../../app/transport/http_stream.ep), [transport/socket_write](../../app/transport/socket_write.ep), [transport/file_response](../../app/transport/file_response.ep), [ui/page](../../app/ui/page.ep), [ui/assets](../../app/ui/assets.ep), `fs` (toolchain/library), [web/routes](../../app/web/routes.ep), [web/recovery](../../app/web/recovery.ep), [archive/runtime_lock](../../app/archive/runtime_lock.ep), [archive/attachment_restore](../../app/archive/attachment_restore.ep), [lifecycle/startup](../../app/lifecycle/startup.ep).

Environment names: `ECHO_ATTACHMENT_ROOT`, `ECHO_NATIVE_CRASH_REPORTS`, `ECHO_PROVIDER_PROFILE`.

Native `main` present. Argument indices read: 1, 2, 3, 4, 5, 6, 7, 8, 9. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
17: define echo_send_response with library as Int and descriptor as Int and status as Str and content_type as Str and body as Str returning Int:
42: define echo_archive_busy_response returning Int:
62: define echo_serve_connection with library as Int and descriptor as Int and port as Int and javascript as Str and config as List returning Int:
106: define echo_worker_owner returning Int:
145: define echo_service_owner returning Int:
179: define main returning Int:
```

## `app/stage_controller.ep`

[app/stage_controller.ep](../../app/stage_controller.ep) · 20 lines · 1133 bytes

SHA-256: `93abc78314d390f3b77f8851d0df4adcd003b991c01cdae7c60b124f8c5289a0`

Imports: [lifecycle/controller_bundle](../../app/lifecycle/controller_bundle.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2, 3, 4. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/start_service.ep`

[app/start_service.ep](../../app/start_service.ep) · 16 lines · 615 bytes

SHA-256: `83e849e43c29b39bcdcfc5e5f8ad6ab66556d759d167eee9775edfa6baed96f6`

Imports: [lifecycle/service_profile](../../app/lifecycle/service_profile.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/tools/attach_file.ep`

[app/tools/attach_file.ep](../../app/tools/attach_file.ep) · 112 lines · 7621 bytes

SHA-256: `d2f21c5514eac9ca2631f059d9633e5053334457cc46dda7a3ff4bf5e44bece4`

Source annotations: Copy the opened, confined source descriptor into an immutable host-owned attachment. No shell, absolute source path, symlink or size ceiling.

Imports: [workspace_tools](../../app/tools/workspace_tools.ep), [../web/attachments](../../app/web/attachments.ep).

Environment names: `ECHO_ATTACHMENT_ROOT`, `ECHO_WORKSPACE_ROOT`.

Definitions:

```text
6: define attach_copy with library as Int and input as Int and path as Str returning Int:
34: define attach_file_execute with database as Int and session as Str and arguments as Str returning Str:
37: define attach_file_execute_device with database as Int and trusted_device as Str and session as Str and arguments as Str and run as Str and call_id as Str returning Str:
101: define attach_file_execute_owned with database as Int and run as Str and call_id as Str returning Str:
```

Lexical test references: [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep).

## `app/tools/attachment_image.ep`

[app/tools/attachment_image.ep](../../app/tools/attachment_image.ep) · 41 lines · 3689 bytes

SHA-256: `400c04c3c55119abca1461b2513357f32d78a616750f8d201723a2d14a44e769`

Source annotations: Archive only a verified source receipt. The request renderer reopens the original bytes and checks this receipt before supplying native image content.

Imports: [../retained_text](../../app/retained_text.ep), [../providers/attachment_image](../../app/providers/attachment_image.ep).

Definitions:

```text
6: define attachment_image_execute with database as Int and session as Str and arguments as Str returning Str:
9: define attachment_audio_execute with database as Int and session as Str and arguments as Str returning Str:
12: define attachment_media_execute with database as Int and session as Str and arguments as Str and modality as Str returning Str:
37: define attachment_image_schemas returning Str:
40: define attachment_audio_schemas returning Str:
```

Lexical test references: [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep).

## `app/tools/attachment_text.ep`

[app/tools/attachment_text.ep](../../app/tools/attachment_text.ep) · 153 lines · 10191 bytes

SHA-256: `087dfe05d7f06c2897801ede3020d42f0eaf0053d75ad5570f5e2cba0bd82711`

Imports: [../memory/archive_recall](../../app/memory/archive_recall.ep), [../archive/attachment_scope](../../app/archive/attachment_scope.ep), [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [history_tools](../../app/tools/history_tools.ep), [../archive/file_text](../../app/archive/file_text.ep), [../archive/pdf_cache](../../app/archive/pdf_cache.ep).

Definitions:

```text
10: define attachment_text_execute with database as Int and session as Str and arguments as Str returning Str:
107: define attachment_unicode_window with text as Str and offset as Int and characters as Int returning List:
131: define attachment_text_find with text as Str and mode as Str and query as Str and offset as Int returning Int:
```

Lexical test references: [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/security/test_linked_recall_existing.ep](../../tests/security/test_linked_recall_existing.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep).

## `app/tools/attachment_tools.ep`

[app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) · 161 lines · 18036 bytes

SHA-256: `86e2f87305038ea3258b2b8c44c3f38a6796ddf9694a267cc3f7c3a9b66442f6`

Source annotations: Scope is the trusted run session. Model arguments cannot choose another session, device, or host path. One cursor result keeps context use explicit.

Imports: [../media/video_library](../../app/media/video_library.ep), [../interaction/watch_party](../../app/interaction/watch_party.ep), [database_schema](../../app/tools/database_schema.ep), [plot_schema](../../app/tools/plot_schema.ep), [build_schema](../../app/tools/build_schema.ep), [workspace_operations_schema](../../app/tools/workspace_operations_schema.ep), [workspace_list_schema](../../app/tools/workspace_list_schema.ep), [../memory/archive_recall](../../app/memory/archive_recall.ep), [../archive/attachment_scope](../../app/archive/attachment_scope.ep), [attach_file](../../app/tools/attach_file.ep), [../security/principals](../../app/security/principals.ep), [../context/system_awareness](../../app/context/system_awareness.ep), [history_tools](../../app/tools/history_tools.ep), [../media/speech_request](../../app/media/speech_request.ep), [../media/voice_request](../../app/media/voice_request.ep), [compaction_tools](../../app/tools/compaction_tools.ep), [../context/recap_rebuild](../../app/context/recap_rebuild.ep), [attachment_text](../../app/tools/attachment_text.ep), [workspace_tools](../../app/tools/workspace_tools.ep), [execution_output](../../app/tools/execution_output.ep), [attachment_image](../../app/tools/attachment_image.ep), [../media/video_request](../../app/media/video_request.ep).

Environment names: `ECHO_EXECUTION_RUNTIME`, `ECHO_IMAGE_PROFILE`, `ECHO_IMAGE_ROOT`, `ECHO_SPEECH_PROFILE`, `ECHO_SPEECH_ROOT`, `ECHO_VIDEO_LIBRARY_ROOT`, `ECHO_VIDEO_PROFILE`, `ECHO_VIDEO_ROOT`, `ECHO_VOICE_PROFILE`, `ECHO_WORKSPACE_ROOT`.

Definitions:

```text
26: define attachment_tool_execute with database as Int and session as Str and arguments as Str returning Str:
59: define session_tool_execute with database as Int and session as Str and name as Str and arguments as Str returning Str:
108: define attachment_listing_schemas returning Str:
113: define attachment_text_schemas returning Str:
118: define workspace_tool_schemas returning Str:
131: define session_base_tool_schemas returning Str:
143: define session_tool_schemas returning Str:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/tools/build_schema.ep`

[app/tools/build_schema.ep](../../app/tools/build_schema.ep) · 2 lines · 1012 bytes

SHA-256: `3f946d76a00e2269575306d6a18af4b733487073a55ec5d25ce5edefb894c3ce`

Imports: none.

Definitions:

```text
1: define workspace_build_schema returning Str:
```

Lexical test references: [tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep).

## `app/tools/candidate_discovery.ep`

[app/tools/candidate_discovery.ep](../../app/tools/candidate_discovery.ep) · 62 lines · 4871 bytes

SHA-256: `cdeccc6dcac78ed93f6a63edd1dadf592c3d76d117cbdc62bbe51af502bf7531`

Source annotations: Candidate manifests may describe privileged code. They are scoped source records, not public catalog entries or proof of qualification/activation.

Imports: [tool_registry](../../app/tools/tool_registry.ep), [../web/shared_access](../../app/web/shared_access.ep), [history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
7: define candidate_discovery_snapshot with database as Int and run as Str and arguments as Str returning Str:
55: define candidate_discovery_execute with database as Int and run as Str and arguments as Str returning Str:
```

Lexical test references: [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep).

## `app/tools/candidate_schema.ep`

[app/tools/candidate_schema.ep](../../app/tools/candidate_schema.ep) · 2 lines · 725 bytes

SHA-256: `00d881f2acc3379a612b4f4c3b24f73ae12a61e8a7c0254030b3e1a77f8e3678`

Imports: none.

Definitions:

```text
1: define candidate_tool_schema returning Str:
```

## `app/tools/compaction_tools.ep`

[app/tools/compaction_tools.ep](../../app/tools/compaction_tools.ep) · 111 lines · 8316 bytes

SHA-256: `2057337032cb59fc7d4f8f13865ea9626e9b54dc3ae2b89c7ce093cff701da70`

Source annotations: Device ownership comes from the trusted session, never from tool arguments. Exact checkpoint IDs may traverse this session's saved recaps only. Return Unicode ranges of originals so inspecting a large recap has no fixed ceiling.

Imports: [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [history_tools](../../app/tools/history_tools.ep), [../context/checkpoint](../../app/context/checkpoint.ep).

Definitions:

```text
9: define compaction_tool_read_snapshot with database as Int and session as Str and arguments as Str returning Str:
100: define compaction_tool_schema returning Str:
103: define compaction_tool_read with database as Int and session as Str and arguments as Str returning Str:
```

Lexical test references: [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep).

## `app/tools/continuity_tools.ep`

[app/tools/continuity_tools.ep](../../app/tools/continuity_tools.ep) · 90 lines · 9802 bytes

SHA-256: `60f8d46a438dab1e534ede685873f228804862e52b6833f1ee24349c501a5ef8`

Source annotations: Trusted run scope, shared across its owner's private sessions. No filesystem or device selector is accepted. Original texts and reasoning are paged exactly.

Imports: [../memory/corrections](../../app/memory/corrections.ep), [memory_tools](../../app/tools/memory_tools.ep).

Definitions:

```text
6: define continuity_execute with database as Int and run as Str and name as Str and arguments as Str returning Str:
89: define continuity_schemas returning Str:
```

Lexical test references: [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_linked_recall_existing.ep](../../tests/security/test_linked_recall_existing.ep), [tests/security/test_reasoning_existing.ep](../../tests/security/test_reasoning_existing.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep).

## `app/tools/database_schema.ep`

[app/tools/database_schema.ep](../../app/tools/database_schema.ep) · 2 lines · 1129 bytes

SHA-256: `a702821e53685d8215e5179c66ef2ff8405046ec2b02ed6ae529acd1980c54dd`

Imports: none.

Definitions:

```text
1: define workspace_database_schema returning Str:
```

## `app/tools/delegation_schema.ep`

[app/tools/delegation_schema.ep](../../app/tools/delegation_schema.ep) · 2 lines · 2396 bytes

SHA-256: `a6109fe731402cfab5eaefe4547acb8b8822f13137086410547a319486cdf93f`

Imports: none.

Definitions:

```text
1: define delegation_tool_schemas returning Str:
```

## `app/tools/delegation_tool.ep`

[app/tools/delegation_tool.ep](../../app/tools/delegation_tool.ep) · 125 lines · 10629 bytes

SHA-256: `3c35102fa026010927cd053e46cc4b5146db5d94988359bf7152e6faaf5c9a61`

Source annotations: Workers are launched by the durable scheduler, outside this tool's process.

Imports: [../jobs/task_checklist](../../app/jobs/task_checklist.ep), [../jobs/delegation_run](../../app/jobs/delegation_run.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../security/group_history](../../app/security/group_history.ep), [../web/cancellation](../../app/web/cancellation.ep), [../web/activity](../../app/web/activity.ep), [history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
10: define delegation_tool_execute with database as Int and run as Str and call_id as Str returning Str:
20: define delegation_status_execute with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
90: define delegation_message_execute with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep).

## `app/tools/episode_schema.ep`

[app/tools/episode_schema.ep](../../app/tools/episode_schema.ep) · 2 lines · 1135 bytes

SHA-256: `71d6383edfafb1056ff1a49f11acb37230ff321a43628138322ba2c3be84ecec`

Imports: none.

Definitions:

```text
1: define episode_tool_schema returning Str:
```

## `app/tools/execution_guardian.ep`

[app/tools/execution_guardian.ep](../../app/tools/execution_guardian.ep) · 65 lines · 4044 bytes

SHA-256: `73796e34fd229cefc898498e13c7b0c88278530db5c5193cd13d52a80628c3c9`

Source annotations: The guardian is a host-installed Ernos binary. It owns the command process, capture and journal. This worker only owns the private lifetime-pipe writer. Preserve validation/recovery status. Invalid argv is not revocation and must not cancel the run as an authority failure.

Imports: [../workspace/guardian_receipt](../../app/workspace/guardian_receipt.ep), [../workspace/native_process](../../app/workspace/native_process.ep), [../workspace/files](../../app/workspace/files.ep).

Environment names: `ECHO_ERNOS_COMPILER_SHA256`, `ECHO_ERNOS_RUNTIME`, `ECHO_EXECUTION_RUNTIME`, `ECHO_HOST_PROTECTED_ROOTS`, `ECHO_HOST_SECRET_ROOTS`, `ECHO_WORKSPACE_ROOT`.

Definitions:

```text
7: define execution_tool_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:
```

## `app/tools/execution_output.ep`

[app/tools/execution_output.ep](../../app/tools/execution_output.ep) · 100 lines · 7222 bytes

SHA-256: `fba05430fd8311270c069669b3ae3b984e42a4e3584a7e83d851a01823d1606b`

Source annotations: Traverse only original output records, excluding model requests and later search/read copies. Session comes from the current run, never from arguments.

Imports: [../providers/local_json](../../app/providers/local_json.ep), [../security/group_gate](../../app/security/group_gate.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
9: define execution_output_execute_snapshot with database as Int and session as Str and arguments as Str returning Str:
76: define execution_decode_hex with database as Int and encoded as Str returning Str:
92: define execution_output_execute with database as Int and session as Str and arguments as Str returning Str:
```

Lexical test references: [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep).

## `app/tools/execution_tool.ep`

[app/tools/execution_tool.ep](../../app/tools/execution_tool.ep) · 215 lines · 14397 bytes

SHA-256: `1b3b44c9d72e92090a041e0d3d87a896de7adce5e8ea970148c5676a5c43a3ed`

Source annotations: Resolve bare commands using the same trusted PATH passed to the child. Explicit relative paths still mean workspace executables; sandbox authority and filesystem isolation are unchanged.

Imports: [execution_guardian](../../app/tools/execution_guardian.ep), [../retained_text](../../app/retained_text.ep), [../workspace/execution_request](../../app/workspace/execution_request.ep), [../workspace/execute_process](../../app/workspace/execute_process.ep), [../workspace/directories](../../app/workspace/directories.ep), [../workspace/ernos_runtime](../../app/workspace/ernos_runtime.ep).

Environment names: `ECHO_ERNOS_RUNTIME`, `ECHO_EXECUTION_RUNTIME`, `ECHO_WORKSPACE_ROOT`.

Definitions:

```text
11: define execution_command_path returning Str:
14: define execution_program_resolve with workspace as Str and program as Str returning Str:
36: define execution_authority_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:
86: define execution_tool_execute_local with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:
```

Lexical test references: [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/security/diagnose_execution_authority.ep](../../tests/security/diagnose_execution_authority.ep).

## `app/tools/git_schema.ep`

[app/tools/git_schema.ep](../../app/tools/git_schema.ep) · 2 lines · 1388 bytes

SHA-256: `34d7359f0171da51bf2ddc23685d19d461a2593ab83669fa683e0aaf8444dd3c`

Imports: none.

Definitions:

```text
1: define host_git_schema returning Str:
```

## `app/tools/history_tools.ep`

[app/tools/history_tools.ep](../../app/tools/history_tools.ep) · 122 lines · 8442 bytes

SHA-256: `38cb4646120a1641e7464a1a7d9ed5b8e043b89e1711d2f4924b3f62c859cd40`

Source annotations: Session scope comes from the trusted run, never from model arguments. Cursor and excerpt sizes are explicit; no hidden result or text ceiling.

Imports: [../security/group_history](../../app/security/group_history.ep), [../context/echo_labels](../../app/context/echo_labels.ep), [../retained_text](../../app/retained_text.ep), [../archive/history](../../app/archive/history.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
9: define history_tool_integer with tokens as List and name as Str returning Int:
16: define history_tool_row with row as List and keys as List returning Str:
30: define history_tool_execute_snapshot with database as Int and session as Str and name as Str and arguments as Str returning Str:
109: define history_tool_schemas returning Str:
114: define history_tool_execute with database as Int and session as Str and name as Str and arguments as Str returning Str:
```

Lexical test references: [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/providers/test_history_zero_boundary.ep](../../tests/providers/test_history_zero_boundary.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/tools/host_text.ep`

[app/tools/host_text.ep](../../app/tools/host_text.ep) · 73 lines · 6244 bytes

SHA-256: `80584717f7f00f4a856bf82882d6d3e85764aaf72f45e464f541970bc08eeb51`

Source annotations: The dispatcher supplies run/call identity. Read the exact saved arguments; never trust a caller-provided role, session or replacement argument object.

Imports: [git_schema](../../app/tools/git_schema.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../workspace/host_profile](../../app/workspace/host_profile.ep), [../workspace/read_text](../../app/workspace/read_text.ep), [history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
9: define host_text_execute with database as Int and run as Str and call_id as Str returning Str:
68: define host_text_schema returning Str:
72: define host_execution_schema returning Str:
```

Lexical test references: [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep).

## `app/tools/job_control.ep`

[app/tools/job_control.ep](../../app/tools/job_control.ep) · 103 lines · 7771 bytes

SHA-256: `081c1b905441981b8626dc489caebc1c3d2cfadc747908c855cfaa58b63689a3`

Source annotations: Read current run scope, never a principal or device asserted by arguments. Group calls cannot enumerate the initiating user's private conversations.

Imports: [../web/worker](../../app/web/worker.ep), [../security/tool_policy](../../app/security/tool_policy.ep).

Definitions:

```text
6: define job_scope_query returning Str:
9: define job_control_snapshot with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
85: define job_control_execute with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

Lexical test references: [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep).

## `app/tools/job_schema.ep`

[app/tools/job_schema.ep](../../app/tools/job_schema.ep) · 2 lines · 872 bytes

SHA-256: `2b5bd918673af238946a60fe952b2e0ab44d785ec6e4f36dace9bffd9a2d92d3`

Imports: none.

Definitions:

```text
1: define job_tool_schema returning Str:
```

## `app/tools/knowledge_tools.ep`

[app/tools/knowledge_tools.ep](../../app/tools/knowledge_tools.ep) · 156 lines · 15211 bytes

SHA-256: `f0cb06b385857a8b6319693be6d54f06c14b1c8c75af664b720957dd22ccdd47`

Source annotations: Re-read the immutable ledger request; asserted roles and scope fields are rejected. The graph primitive rechecks source ownership under its writer lock.

Imports: [../memory/corrections](../../app/memory/corrections.ep), [../memory/episodes](../../app/memory/episodes.ep), [../memory/knowledge_lifecycle](../../app/memory/knowledge_lifecycle.ep), [../security/tool_policy](../../app/security/tool_policy.ep).

Definitions:

```text
8: define knowledge_tool_execute with database as Int and run as Str and call_id as Str and name as Str returning Str:
152: define knowledge_tool_schemas returning Str:
155: define knowledge_visibility_tool_schemas returning Str:
```

## `app/tools/memory_tools.ep`

[app/tools/memory_tools.ep](../../app/tools/memory_tools.ep) · 99 lines · 8277 bytes

SHA-256: `5a7120b0861714980ebb9e7784fe73b742a4805164ddd1ef0811e2a9c8a85538`

Source annotations: Identity and arguments come from the immutable queued call, never model scope fields.

Imports: [../memory/archive_recall](../../app/memory/archive_recall.ep), [../memory/corrections](../../app/memory/corrections.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
7: define memory_tool_execute with database as Int and run as Str and call_id as Str and name as Str returning Str:
98: define memory_tool_schemas returning Str:
```

Lexical test references: [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep).

## `app/tools/plot_schema.ep`

[app/tools/plot_schema.ep](../../app/tools/plot_schema.ep) · 2 lines · 1402 bytes

SHA-256: `3e92fe9c5f93aef9c60120e41def4b67904753b1a7effa0b3f404b2e6a045b67`

Imports: none.

Definitions:

```text
1: define workspace_plot_schema returning Str:
```

## `app/tools/procedure_schema.ep`

[app/tools/procedure_schema.ep](../../app/tools/procedure_schema.ep) · 2 lines · 2899 bytes

SHA-256: `2c55fd7927c7e884fef181894f329e8b08c51d27ed2c107e501d739f70263132`

Imports: none.

Definitions:

```text
1: define procedure_tool_schema returning Str:
```

## `app/tools/procedure_tool.ep`

[app/tools/procedure_tool.ep](../../app/tools/procedure_tool.ep) · 48 lines · 3676 bytes

SHA-256: `ac6b273d7bb32152d45d19c60021c9664ae2688aa27d1bfd2df10349f655a1ea`

Imports: [../memory/procedures](../../app/memory/procedures.ep), [../context/session_messages](../../app/context/session_messages.ep).

Definitions:

```text
4: define procedure_tool_execute with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

## `app/tools/project_schema.ep`

[app/tools/project_schema.ep](../../app/tools/project_schema.ep) · 2 lines · 2087 bytes

SHA-256: `9df3129a3e054788d640283b74cde2882291f2fcf94108d72f1f6ba8b77579b0`

Imports: none.

Definitions:

```text
1: define project_tool_schema returning Str:
```

## `app/tools/project_tool.ep`

[app/tools/project_tool.ep](../../app/tools/project_tool.ep) · 67 lines · 5243 bytes

SHA-256: `4483838317091101c1ea1104b410544cc61364e178f82ce1308d1cda598bb32d`

Imports: [../projects/context](../../app/projects/context.ep), [history_tools](../../app/tools/history_tools.ep), [../security/tool_policy](../../app/security/tool_policy.ep).

Definitions:

```text
5: define project_tool_execute with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

## `app/tools/protected_tools.ep`

[app/tools/protected_tools.ep](../../app/tools/protected_tools.ep) · 65 lines · 6992 bytes

SHA-256: `be16a31852cdae9f8e1f2cb735af65d47c0a831147170e8a52757a94f459357d`

Source annotations: Only a claimed call in the currently accepted assistant response can review. Neither a fabricated role argument nor a stale queued branch can substitute.

Imports: [../rights/reviews](../../app/rights/reviews.ep), [../security/tool_policy](../../app/security/tool_policy.ep).

Definitions:

```text
6: define protected_tool_execute with database as Int and run as Str and call_id as Str and name as Str returning Str:
64: define protected_tool_schemas returning Str:
```

Lexical test references: [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep).

## `app/tools/schedule_schema.ep`

[app/tools/schedule_schema.ep](../../app/tools/schedule_schema.ep) · 2 lines · 1236 bytes

SHA-256: `33de3a89afbe6f9b05b44ea2a848137322c66aa11609fa138a643a61813cb72f`

Imports: none.

Definitions:

```text
1: define schedule_tool_schema returning Str:
```

## `app/tools/schedule_tool.ep`

[app/tools/schedule_tool.ep](../../app/tools/schedule_tool.ep) · 98 lines · 7481 bytes

SHA-256: `8fb51b7387aa06c8466252a353ec9732f342b83164815dfd0a8dc7bce952fe76`

Imports: [../jobs/schedule_store](../../app/jobs/schedule_store.ep), [../web/shared_access](../../app/web/shared_access.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
6: define schedule_tool_status with status as Str returning Str:
9: define schedule_tool_read with database as Int and run as Str and call_id as Str and operation as Str and tokens as List returning Str:
47: define schedule_tool_execute with database as Int and run as Str and call_id as Str and arguments as Str returning Str:
```

Lexical test references: [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep).

## `app/tools/scratchpad.ep`

[app/tools/scratchpad.ep](../../app/tools/scratchpad.ep) · 88 lines · 6684 bytes

SHA-256: `1578992d4f3d5da7e7063a8d00de39ca10ed981c68aa6a3f1a1c553f2313dfd4`

Source annotations: Private notes survive session changes; shared notes belong to an exact audience. Append-only revisions retain audit history; compare-and-swap prevents lost edits.

Imports: [memory_tools](../../app/tools/memory_tools.ep), [../memory/scratchpad_scope](../../app/memory/scratchpad_scope.ep).

Definitions:

```text
6: define scratchpad_execute with database as Int and run as Str and arguments as Str returning Str:
```

Lexical test references: [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep).

## `app/tools/scratchpad_schema.ep`

[app/tools/scratchpad_schema.ep](../../app/tools/scratchpad_schema.ep) · 2 lines · 911 bytes

SHA-256: `a5231418292eb94dd45e40b9beea5207eab1e55923d3dfdf72593e62959cb552`

Imports: none.

Definitions:

```text
1: define scratchpad_schema returning Str:
```

## `app/tools/task_checklist_schema.ep`

[app/tools/task_checklist_schema.ep](../../app/tools/task_checklist_schema.ep) · 2 lines · 1500 bytes

SHA-256: `5727439feaa9685203e286d6d7803fc34dadfd3378617b83fdae0e7b239dcd18`

Imports: none.

Definitions:

```text
1: define task_checklist_schema returning Str:
```

## `app/tools/tool_candidate_evidence.ep`

[app/tools/tool_candidate_evidence.ep](../../app/tools/tool_candidate_evidence.ep) · 44 lines · 3685 bytes

SHA-256: `7e97c847f63971f6f2ce421fef17c3f6d384edb4c4cc5fb2f4a2518a1c4e5b12`

Source annotations: Validate actual loaded-catalog observations, not model-authored assertions. The caller holds its writer transaction through candidate persistence. Dormant catalog coverage and semantic composition evaluation are additional gates.

Imports: [tool_discovery](../../app/tools/tool_discovery.ep).

Definitions:

```text
6: define tool_candidate_loaded_evidence with database as Int and run as Str and manifest as Str returning Str:
```

## `app/tools/tool_candidate_files.ep`

[app/tools/tool_candidate_files.ep](../../app/tools/tool_candidate_files.ep) · 43 lines · 2519 bytes

SHA-256: `b8e14b3aa363386e634847dd63d3ede044d6b0f397538cf241ed5c3e146f8793`

Source annotations: Host-selected evaluation root; manifest paths are always relative. Hash the validated descriptor itself, so replacing the pathname cannot switch inputs. Byte qualification only. This does not execute a candidate, grant capabilities, accept tests or prove that a binary was built from the declared source.

Imports: [tool_registry](../../app/tools/tool_registry.ep), [../workspace/files](../../app/workspace/files.ep).

Definitions:

```text
6: define tool_candidate_file_digest with root_path as Str and path as Str returning List:
27: define tool_candidate_files_check with root_path as Str and manifest as Str returning Str:
```

Lexical test references: [tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep).

## `app/tools/tool_contract.ep`

[app/tools/tool_contract.ep](../../app/tools/tool_contract.ep) · 145 lines · 15808 bytes

SHA-256: `c65960a1fc555c67c02c5078e22b054c37f2d9ab2431991f373994164885950a`

Source annotations: Describes existing implementations; this never grants execution permission. Schema versions and contract versions are distinct from binary provenance.

Imports: [../security/tool_policy](../../app/security/tool_policy.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Environment names: `ECHO_RUNTIME_BUILD_SHA256`.

Definitions:

```text
6: define tool_contract with name as Str and schema as Str returning Str:
131: define tool_contract_catalog_digest with schemas as Str returning Str:
```

Lexical test references: [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep).

## `app/tools/tool_discovery.ep`

[app/tools/tool_discovery.ep](../../app/tools/tool_discovery.ep) · 68 lines · 4907 bytes

SHA-256: `6a4464e0c80cd15cbf2ad5cb077178a240bcd3df348bee80994326959fa6e847`

Source annotations: Discovery observes the same authority-filtered schemas used for inference. A cursor is bound to the exact catalog; changed permissions never silently splice pages from two different tool sets. Dispatch still rechecks each call.

Imports: [../memory/procedures](../../app/memory/procedures.ep), [tool_contract](../../app/tools/tool_contract.ep), [../context/session_messages](../../app/context/session_messages.ep).

Definitions:

```text
8: define tool_discovery_snapshot with database as Int and run as Str and arguments as Str returning Str:
61: define tool_discovery_execute with database as Int and run as Str and arguments as Str returning Str:
```

Lexical test references: [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep).

## `app/tools/tool_registry.ep`

[app/tools/tool_registry.ep](../../app/tools/tool_registry.ep) · 143 lines · 9346 bytes

SHA-256: `f125d1c3b6b788b8330d13694e84936130d1c8f7916e00ab6e1144dcc3c1eb61`

Source annotations: Candidate persistence is not registration or execution authority. Evaluation, dependency verification and activation consume these immutable exact bytes. An existing builtin cannot be shadowed by a saved custom definition.

Imports: [../security/group_speech](../../app/security/group_speech.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep), [tool_candidate_evidence](../../app/tools/tool_candidate_evidence.ep).

Definitions:

```text
8: define tool_registry_schema with database as Int returning Str:
14: define tool_candidate_name_valid with name as Str returning Int:
26: define tool_candidate_manifest_valid with manifest as Str returning Int:
87: define tool_candidate_dependencies_check with database as Int and run as Str and manifest as Str returning Str:
109: define tool_candidate_save with database as Int and run as Str and manifest as Str returning Str:
```

Lexical test references: [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep).

## `app/tools/trace_tools.ep`

[app/tools/trace_tools.ep](../../app/tools/trace_tools.ep) · 126 lines · 10721 bytes

SHA-256: `b85481f90f8870a23c8e664c4b4e71b9d4a6c6328d01ad9121cb2aebe67af4b9`

Source annotations: The scope is resolved from the initiating run, never a model-selected owner. Private linked accounts retain their existing history rights. Shared traces use the current conversation's immutable branch boundaries and audience gate. Search advances one exact event at a time under a retained sequence boundary. Empty reasoning remains distinguishable from an invented retrospective account.

Imports: [memory_tools](../../app/tools/memory_tools.ep).

Definitions:

```text
6: define trace_scope with database as Int and run as Str returning List:
18: define trace_kind_sql returning Str:
23: define trace_execute_snapshot with database as Int and run as Str and name as Str and arguments as Str returning Str:
115: define trace_execute with database as Int and run as Str and name as Str and arguments as Str returning Str:
125: define trace_tool_schemas returning Str:
```

Lexical test references: [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep).

## `app/tools/web_fetch.ep`

[app/tools/web_fetch.ep](../../app/tools/web_fetch.ep) · 253 lines · 17546 bytes

SHA-256: `f78fbeda7f014bab389c1eb25afcd35f0cc00226cdaa6caaf0b562e98c4c9acf`

Imports: [../network/media_type](../../app/network/media_type.ep), [web_navigation](../../app/tools/web_navigation.ep), [../network/search_query](../../app/network/search_query.ep), [../media/shared_authority](../../app/media/shared_authority.ep), [../media/image_logs](../../app/media/image_logs.ep), [../workspace/directories](../../app/workspace/directories.ep), [../providers/helper_process](../../app/providers/helper_process.ep), [../network/public_url](../../app/network/public_url.ep).

Environment names: `ECHO_RESEARCH_ROOT`, `ECHO_RESEARCH_RUNTIME`.

Definitions:

```text
10: define web_fetch_authority with database as Int and run as Str and call_id as Str returning List:
24: define web_fetch_monitor with path as Str and run as Str and call_id as Str and cancellation as Int and stop as Int and writer as Int and completed as Int returning Int:
47: define web_fetch_status with status as Str returning Str:
53: define web_fetch_publish with database as Int and run as Str and call_id as Str and root as Str and identity as Str returning Str:
142: define web_fetch_execute with database as Int and run as Str and call_id as Str and cancellation as Int returning Str:
```

Lexical test references: [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep).

## `app/tools/web_fetch_schema.ep`

[app/tools/web_fetch_schema.ep](../../app/tools/web_fetch_schema.ep) · 8 lines · 1930 bytes

SHA-256: `44dac732336b49cbb60ec7e42bed27a6ce840cf250e74cef9c0add0f46ec0325`

Imports: none.

Definitions:

```text
1: define web_fetch_schema returning Str:
4: define web_search_schema returning Str:
7: define web_follow_schema returning Str:
```

## `app/tools/web_navigation.ep`

[app/tools/web_navigation.ep](../../app/tools/web_navigation.ep) · 34 lines · 2183 bytes

SHA-256: `7fb2c814fcb4b79591b651cbc97aeee6132e2091bf10b810e18298e97aaeb11f`

Source annotations: Resolve a link from a digest-verified document in this conversation's normal attachment scope. The chosen URL is still revalidated by the fetch worker.

Imports: [attachment_text](../../app/tools/attachment_text.ep), [../network/public_url](../../app/network/public_url.ep).

Definitions:

```text
6: define web_navigation_resolve with database as Int and session as Str and attachment as Str and selected as Int returning List:
```

Lexical test references: [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep).

## `app/tools/workspace_list_schema.ep`

[app/tools/workspace_list_schema.ep](../../app/tools/workspace_list_schema.ep) · 2 lines · 731 bytes

SHA-256: `e0ed17ed3efa4764ed844d505f2e4cc50029623c11ef747b4e51a24b435aa15b`

Imports: none.

Definitions:

```text
1: define workspace_list_schema returning Str:
```

## `app/tools/workspace_operations_schema.ep`

[app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) · 2 lines · 10030 bytes

SHA-256: `1bd49bbb6749dfdf76626d028d5927ce75f33c0078e89ea06a35a2102b468f3a`

Imports: none.

Definitions:

```text
1: define workspace_operations_schema returning Str:
```

## `app/tools/workspace_tools.ep`

[app/tools/workspace_tools.ep](../../app/tools/workspace_tools.ep) · 274 lines · 20667 bytes

SHA-256: `6ffed253a751cee57262bf5df84cbe0065c21e4389344d00b432280ec5f65bba`

Source annotations: The run supplies the session. Device ownership is resolved from the archive; neither ownership nor the host root can be selected by tool arguments. Explicit device is accepted only from the authenticated dispatcher wrapper.

Imports: [../workspace/plot_svg](../../app/workspace/plot_svg.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [../workspace/sessions](../../app/workspace/sessions.ep), [../workspace/read_text](../../app/workspace/read_text.ep), [../workspace/directories](../../app/workspace/directories.ep), [../workspace/create_text](../../app/workspace/create_text.ep), [../workspace/list_directory](../../app/workspace/list_directory.ep), [../workspace/search_text](../../app/workspace/search_text.ep), [../workspace/edit_text](../../app/workspace/edit_text.ep), [../workspace/diff_text](../../app/workspace/diff_text.ep), [../workspace/read_table](../../app/workspace/read_table.ep), [../workspace/read_json](../../app/workspace/read_json.ep), [../workspace/workbook_read](../../app/workspace/workbook_read.ep), [../workspace/workbook_create](../../app/workspace/workbook_create.ep), [../workspace/workbook_edit](../../app/workspace/workbook_edit.ep), [../workspace/list_archive](../../app/workspace/list_archive.ep), [../workspace/create_archive](../../app/workspace/create_archive.ep), `os` (toolchain/library).

Environment names: `ECHO_WORKSPACE_ROOT`.

Definitions:

```text
24: define workspace_tool_execute with database as Int and session as Str and arguments as Str and operation as Str returning Str:
28: define workspace_tool_execute_device with database as Int and trusted_device as Str and session as Str and arguments as Str and operation as Str returning Str:
224: define workspace_text_execute with database as Int and session as Str and arguments as Str returning Str:
229: define workspace_tool_execute_owned with database as Int and run as Str and call_id as Str returning Str:
```

Lexical test references: [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/transport/cooperative_accept.ep`

[app/transport/cooperative_accept.ep](../../app/transport/cooperative_accept.ep) · 18 lines · 827 bytes

SHA-256: `6f64edb7ff3444849ec9c5394e7d6f53e8920421217c7eecd6901546b75bafd1`

Source annotations: One server owner accepts from this nonblocking listener. Idle readiness waits cooperate with other workers; transient interruptions never stop the server.

Imports: [socket_write](../../app/transport/socket_write.ep).

Definitions:

```text
5: define local_socket_accept_cooperative with listener as List returning Int:
```

Lexical test references: [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep).

## `app/transport/diagnostics.ep`

[app/transport/diagnostics.ep](../../app/transport/diagnostics.ep) · 47 lines · 2523 bytes

SHA-256: `0a73a839951db035a4a05c8787b009bdb3909f8a2d89b4c763b7743560aafebb`

Source annotations: Append one private JSON record with no prompts, credentials or provider profile. A failed diagnostic write is surfaced on stderr; it is never silently ignored.

Imports: [socket_read](../../app/transport/socket_read.ep), [../providers/json_tokens](../../app/providers/json_tokens.ep).

Environment names: `ECHO_DIAGNOSTIC_LOG`.

Definitions:

```text
6: define echo_diagnostic with event as Str and identity as Str and value as Int returning Int:
36: define echo_reap_requests returning Int:
```

## `app/transport/event_stream.ep`

[app/transport/event_stream.ep](../../app/transport/event_stream.ep) · 54 lines · 2770 bytes

SHA-256: `6fb7de79adaa85ead028545acbae974008102a864fa00668e72ac7dabe981733`

Source annotations: One response owns one stream. Fields: reader, body, decoder, borrowed pointer, byte count, cursor, terminal status. The read size is a caller-selected batching preference, never an event limit. Returns one event at a time without reading beyond the current HTTP body. Errors and clean transport EOF are sticky. Neither establishes model success.

Imports: [../retained_text](../../app/retained_text.ep), [server_events](../../app/transport/server_events.ep), [http_response](../../app/transport/http_response.ep).

Definitions:

```text
5: external define peek_byte with pointer as Int and offset as Int returning Int
10: define event_stream_create with reader as List and body as List returning List:
23: define event_stream_next with stream as List and read_size as Int returning List:
```

Lexical test references: [tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep).

## `app/transport/file_response.ep`

[app/transport/file_response.ep](../../app/transport/file_response.ep) · 55 lines · 2992 bytes

SHA-256: `8a7d437e0a6868c781440f7a0f2b296bbf8e11f89a4eb58f7c8df932862cc98c`

Source annotations: The caller supplies only an ownership-resolved host path and recorded size. Raw bounded reads preserve binary bytes without buffering the whole file. Media type is supplied by host code, never copied from a request header.

Imports: [../retained_text](../../app/retained_text.ep), [socket_write](../../app/transport/socket_write.ep).

Definitions:

```text
6: define file_response with library as Int and socket as Int and path as Str and bytes as Int and batch as Int returning Int:
10: define file_response_typed with library as Int and socket as Int and path as Str and bytes as Int and batch as Int and media as Str and download as Int returning Int:
```

## `app/transport/http_body.ep`

[app/transport/http_body.ep](../../app/transport/http_body.ep) · 194 lines · 9913 bytes

SHA-256: `bbbad6fa8906303a10d7a0c6496efc4abf93a22a9c0d7dcf0f208c5438e17554`

Imports: [../retained_text](../../app/retained_text.ep), [http_stream](../../app/transport/http_stream.ep).

Definitions:

```text
4: define http_stream_line with reader as List returning List:
36: define http_hexadecimal with text as Str returning Int:
56: define http_chunk_size with line as Str returning Int:
114: define http_body_create with framing as Str and length as Int returning List:
128: define http_body_next with reader as List and body as List and wanted as Int returning List:
```

Lexical test references: [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/generation_fixture.ep](../../tests/fixtures/generation_fixture.ep), [tests/fixtures/json_fixture.ep](../../tests/fixtures/json_fixture.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep), [tests/transport/test_http_body.ep](../../tests/transport/test_http_body.ep), [tests/transport/test_http_response.ep](../../tests/transport/test_http_response.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep).

## `app/transport/http_head.ep`

[app/transport/http_head.ep](../../app/transport/http_head.ep) · 136 lines · 6427 bytes

SHA-256: `b13223e37488f04ba6d2b4888c07434779008a5ac65d41965f752edd2de9e1e1`

Source annotations: Reject overflow at the machine Int boundary; no smaller body-size policy.

Imports: `string` (toolchain/library).

Definitions:

```text
3: define http_failure with status as Str returning List:
8: define http_token with text as Str returning Int:
23: define http_decimal with text as Str returning Int:
42: define http_parse_head with head as Str returning List:
```

Lexical test references: [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/integration/test_component_imports.ep](../../tests/integration/test_component_imports.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_context_admission.ep](../../tests/providers/test_live_context_admission.ep), [tests/providers/test_live_provider_switch.ep](../../tests/providers/test_live_provider_switch.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_long_output.ep](../../tests/providers/test_long_output.ep), [tests/security/test_group_output_read.ep](../../tests/security/test_group_output_read.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_observer.ep](../../tests/security/test_live_observer.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/tools/test_history_dispatch.ep](../../tests/tools/test_history_dispatch.ep), [tests/transport/test_http_head.ep](../../tests/transport/test_http_head.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_live_guardian_server.ep](../../tests/web/test_live_guardian_server.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_trace.ep](../../tests/web/test_trace.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep), [tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep), [tests/workspace/test_sandbox.ep](../../tests/workspace/test_sandbox.ep).

## `app/transport/http_response.ep`

[app/transport/http_response.ep](../../app/transport/http_response.ep) · 87 lines · 4506 bytes

SHA-256: `f9b68b5030397061be6b9330b42d63a9a8fcb4e260524fa2c3c94f04a9a277b9`

Source annotations: The HTTP status remains separate from transport success. A parsed 500 response is not a successful model answer. Caller handles interim statuses explicitly. Fields: ready, numeric status, framing, length, header pairs.

Imports: [../retained_text](../../app/retained_text.ep), [http_body](../../app/transport/http_body.ep).

Definitions:

```text
7: define http_response_head with reader as List and method as Str returning List:
```

Lexical test references: [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep), [tests/transport/test_http_response.ep](../../tests/transport/test_http_response.ep).

## `app/transport/http_stream.ep`

[app/transport/http_stream.ep](../../app/transport/http_stream.ep) · 91 lines · 4220 bytes

SHA-256: `23bb90d78eecaef7a4a83817b316550ca1d03231c34ffb6d666d54ed86f2ed9d`

Source annotations: Reader owns its current native buffer. Slices are borrowed until the next take. Fields: library, socket, cancellation reader, buffer, buffer length, cursor. Return up to the requested number of bytes without consuming later messages. Call repeatedly to complete an exact body length. An early EOF is incomplete.

Imports: [../retained_text](../../app/retained_text.ep), [socket_read](../../app/transport/socket_read.ep), [http_head](../../app/transport/http_head.ep).

Definitions:

```text
7: define http_stream_create with library as Int and descriptor as Int and cancellation_reader as Int returning List:
17: define http_stream_release with reader as List returning Int:
26: define http_stream_take with reader as List and wanted as Int returning List:
49: define http_stream_head with reader as List returning List:
```

Lexical test references: [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/generation_fixture.ep](../../tests/fixtures/generation_fixture.ep), [tests/fixtures/json_fixture.ep](../../tests/fixtures/json_fixture.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep), [tests/transport/test_http_body.ep](../../tests/transport/test_http_body.ep), [tests/transport/test_http_response.ep](../../tests/transport/test_http_response.ep), [tests/transport/test_http_stream.ep](../../tests/transport/test_http_stream.ep), [tests/web/serve_partial_fixture.ep](../../tests/web/serve_partial_fixture.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_upload_body.ep](../../tests/web/test_upload_body.ep).

## `app/transport/isolated_process.ep`

[app/transport/isolated_process.ep](../../app/transport/isolated_process.ep) · 79 lines · 4617 bytes

SHA-256: `ababfed5b54ce9a0d5befec088c7daf30a5a5b34e846c31899ba62cb0ee37c9e`

Source annotations: Each HTTP request and model worker gets a fresh Ernos heap. Only an explicitly selected socket and standard output/error cross the process boundary.

Imports: [diagnostics](../../app/transport/diagnostics.ep), [socket_read](../../app/transport/socket_read.ep).

Definitions:

```text
6: define echo_process_start with arguments as List and descriptor as Int returning Int:
62: define echo_process_reap_automatically returning Int:
68: define echo_worker_start with database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:
```

## `app/transport/local_connect.ep`

[app/transport/local_connect.ep](../../app/transport/local_connect.ep) · 43 lines · 2676 bytes

SHA-256: `aa919cb06667a19f68b7defab6d3ccadcd80d2a02ccdd15c96778237d96b5952`

Source annotations: Darwin loopback client. Nonblocking connect waits on the same cancellation signal as reads/writes; no application connection deadline is imposed. Caller owns the library; a successful descriptor transfers to the caller.

Imports: [socket_write](../../app/transport/socket_write.ep).

Definitions:

```text
6: define local_socket_connect with library as Int and port as Int and cancellation_reader as Int returning List:
```

Lexical test references: [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep).

## `app/transport/local_socket.ep`

[app/transport/local_socket.ep](../../app/transport/local_socket.ep) · 107 lines · 5674 bytes

SHA-256: `20215d55c445d2d847025c65e237963dc50e3eb5baed3bcb918fd9abfd2d11d2`

Source annotations: Darwin's socket ABI uses signed 32-bit results. The generic integer FFI can expose a negative result as an unsigned low word on Apple Silicon. This adapter is deliberately explicit about its operating-system ABI. The model/provider layer remains independent of the local socket platform. Return fields: library handle, listening descriptor, actual assigned port.

Imports: `os` (toolchain/library), `net` (toolchain/library).

Definitions:

```text
4: external define get_list_data_ptr with values returning Int
8: define socket_signed_result with value as Int returning Int:
16: define local_socket_listen with requested_port as Int returning List:
85: define local_socket_accept with listener as List returning Int:
102: define local_socket_close with listener as List returning Int:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_checked_generation.ep](../../tests/agent/test_checked_generation.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_generation.ep](../../tests/agent/test_generation.ep), [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_runner.ep](../../tests/agent/test_runner.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_file_stage.ep](../../tests/archive/test_file_stage.ep), [tests/archive/test_file_text.ep](../../tests/archive/test_file_text.ep), [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_automatic_compaction.ep](../../tests/context/test_automatic_compaction.ep), [tests/context/test_compaction_crash.ep](../../tests/context/test_compaction_crash.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_merge_resume.ep](../../tests/context/test_merge_resume.ep), [tests/context/test_progressive_merge.ep](../../tests/context/test_progressive_merge.ep), [tests/context/test_recap_collect.ep](../../tests/context/test_recap_collect.ep), [tests/context/test_recap_fragment.ep](../../tests/context/test_recap_fragment.ep), [tests/context/test_recap_fragment_review.ep](../../tests/context/test_recap_fragment_review.ep), [tests/context/test_recap_generate.ep](../../tests/context/test_recap_generate.ep), [tests/context/test_recap_notes.ep](../../tests/context/test_recap_notes.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_recap_sources.ep](../../tests/context/test_recap_sources.ep), [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/generation_fixture.ep](../../tests/fixtures/generation_fixture.ep), [tests/fixtures/json_fixture.ep](../../tests/fixtures/json_fixture.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_service.ep](../../tests/lifecycle/test_controller_service.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/network/test_public_destination.ep](../../tests/network/test_public_destination.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/providers/test_local_json.ep](../../tests/providers/test_local_json.ep), [tests/providers/test_modality_preflight.ep](../../tests/providers/test_modality_preflight.ep), [tests/providers/test_quote.ep](../../tests/providers/test_quote.ep), [tests/research/test_native_ffi.ep](../../tests/research/test_native_ffi.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep), [tests/transport/test_http_body.ep](../../tests/transport/test_http_body.ep), [tests/transport/test_http_response.ep](../../tests/transport/test_http_response.ep), [tests/transport/test_http_stream.ep](../../tests/transport/test_http_stream.ep), [tests/transport/test_local_socket.ep](../../tests/transport/test_local_socket.ep), [tests/transport/test_socket_read.ep](../../tests/transport/test_socket_read.ep), [tests/transport/test_socket_write.ep](../../tests/transport/test_socket_write.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/serve_partial_fixture.ep](../../tests/web/serve_partial_fixture.ep), [tests/web/serve_provider_fixture.ep](../../tests/web/serve_provider_fixture.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_upload_body.ep](../../tests/web/test_upload_body.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep), [tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_process.ep](../../tests/workspace/test_process.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_process_tree.ep](../../tests/workspace/test_process_tree.ep), [tests/workspace/test_sandbox.ep](../../tests/workspace/test_sandbox.ep), [tests/workspace/test_zip_names.ep](../../tests/workspace/test_zip_names.ep).

## `app/transport/native_open.ep`

[app/transport/native_open.ep](../../app/transport/native_open.ep) · 54 lines · 2801 bytes

SHA-256: `cd4a541e8670a860c44cbf3c20deafcd182e2c82fc3796f83d164ca10ff251d7`

Source annotations: Apple arm64 passes variadic arguments differently from fixed arguments. openat has three fixed arguments followed by the promoted mode integer. Use the installed libffi ABI rather than the generic fixed dlcall wrapper. Installed arm64 ffi_cif is40 bytes including aarch64_nfixedargs.

Imports: [socket_read](../../app/transport/socket_read.ep).

Definitions:

```text
6: define native_open_at with library as Int and parent as Int and path as Str and flags as Int and mode as Int returning Int:
53: define native_open_mode with library as Int and path as Str and flags as Int and mode as Int returning Int:
```

Lexical test references: [tests/network/test_html_document.ep](../../tests/network/test_html_document.ep), [tests/network/test_html_isolation.ep](../../tests/network/test_html_isolation.ep), [tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep), [tests/workspace/test_process.ep](../../tests/workspace/test_process.ep).

## `app/transport/server_events.ep`

[app/transport/server_events.ep](../../app/transport/server_events.ep) · 143 lines · 6705 bytes

SHA-256: `491b4b9d82afbb9abf570f2b9d0cff71dd3e51a000a890759cd58c665eb0bb4f`

Source annotations: Incremental UTF-8 server events. Each input is one wire byte, so network boundaries cannot change parsing. Invalid UTF-8 and NUL fail explicitly; Echo never silently changes a provider's JSON or tool arguments. State: line builder, data builder, event type, last id, skip LF, first line, UTF-8 continuation count, next minimum, next maximum, failed, retry text, reusable byte cell.

Imports: [../retained_text](../../app/retained_text.ep), [http_stream](../../app/transport/http_stream.ep).

Definitions:

```text
10: define server_events_create returning List:
28: define server_events_line with state as List returning List:
82: define server_events_byte with state as List and code as Int returning List:
135: define server_events_end with state as List returning List:
```

Lexical test references: [tests/transport/test_server_events.ep](../../tests/transport/test_server_events.ep).

## `app/transport/socket_read.ep`

[app/transport/socket_read.ep](../../app/transport/socket_read.ep) · 141 lines · 6969 bytes

SHA-256: `3de3c407bbb94ed1f86fc4fdd7106f0b143ae31eec1c43fb30d2b0b16e56332c`

Source annotations: The request owner closes the write end to cancel, waking poll immediately. No periodic timeout or wall-clock lifetime limit is needed. Only the request owner mutates the cancellation pair.

Imports: [local_socket](../../app/transport/local_socket.ep), `select` (toolchain/library).

Definitions:

```text
4: define socket_errno with library as Int returning Int:
16: define socket_cancellation_create with library as Int returning List:
30: define socket_cancel with pair as List returning Int:
37: define socket_cancellation_close with pair as List returning Int:
49: define socket_wait_event with library as Int and descriptor as Int and cancellation_reader as Int and event as Int returning Int:
86: define socket_wait_readable with library as Int and descriptor as Int and cancellation_reader as Int returning Int:
89: define socket_read_result with status as Str and pointer as Int and count as Int returning List:
99: define socket_read_chunk with library as Int and descriptor as Int and cancellation_reader as Int returning List:
135: define socket_release_buffer with library as Int and buffer as Int returning Int:
```

Lexical test references: [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_runner.ep](../../tests/agent/test_runner.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep), [tests/transport/test_socket_read.ep](../../tests/transport/test_socket_read.ep), [tests/transport/test_socket_write.ep](../../tests/transport/test_socket_write.ep), [tests/web/serve_partial_fixture.ep](../../tests/web/serve_partial_fixture.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep), [tests/workspace/test_process.ep](../../tests/workspace/test_process.ep), [tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_process_tree.ep](../../tests/workspace/test_process_tree.ep), [tests/workspace/test_sandbox.ep](../../tests/workspace/test_sandbox.ep), [tests/workspace/test_sandbox_canonical_path.ep](../../tests/workspace/test_sandbox_canonical_path.ep).

## `app/transport/socket_write.ep`

[app/transport/socket_write.ep](../../app/transport/socket_write.ep) · 63 lines · 3383 bytes

SHA-256: `868694a5f1a6baafbc972689d719cdd6090c9998d641ad3ec2f6f1d6c3e79681`

Source annotations: Darwin arm64 variadic arguments are stack-passed. The ninth argument of the integer FFI occupies that stack slot after its eight register arguments. Verify F_GETFL afterwards so an ABI mismatch cannot silently enable blocking. Return status and the exact byte count sent, including on cancellation/error. This never retries an entire message after a partial write.

Imports: [socket_read](../../app/transport/socket_read.ep).

Definitions:

```text
6: define socket_make_nonblocking with library as Int and descriptor as Int returning Int:
26: define socket_write_result with status as Str and count as Int returning List:
32: define socket_write_all with library as Int and descriptor as Int and cancellation_reader as Int and pointer as Int and count as Int returning List:
```

Lexical test references: [tests/fixtures/echo_http_client.ep](../../tests/fixtures/echo_http_client.ep), [tests/fixtures/generation_fixture.ep](../../tests/fixtures/generation_fixture.ep), [tests/fixtures/json_fixture.ep](../../tests/fixtures/json_fixture.ep), [tests/fixtures/runner_provider.ep](../../tests/fixtures/runner_provider.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_archived_stream.ep](../../tests/providers/test_archived_stream.ep), [tests/providers/test_local_chat.ep](../../tests/providers/test_local_chat.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/transport/test_event_stream.ep](../../tests/transport/test_event_stream.ep), [tests/transport/test_http_body.ep](../../tests/transport/test_http_body.ep), [tests/transport/test_http_response.ep](../../tests/transport/test_http_response.ep), [tests/transport/test_http_stream.ep](../../tests/transport/test_http_stream.ep), [tests/transport/test_socket_write.ep](../../tests/transport/test_socket_write.ep), [tests/web/serve_partial_fixture.ep](../../tests/web/serve_partial_fixture.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_upload_body.ep](../../tests/web/test_upload_body.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep).

## `app/ui/assets.ep`

[app/ui/assets.ep](../../app/ui/assets.ep) · 76 lines · 3719 bytes

SHA-256: `729275be2c2c622540c773dc2515def391a48a1f7458486a9bb1b28391ee0617`

Source annotations: Only these upstream public UI resources are exposed. This is not a general file route; model output and browser arguments cannot supply a host path.

Imports: [../retained_text](../../app/retained_text.ep), [../archive/file_digest](../../app/archive/file_digest.ep), [../transport/file_response](../../app/transport/file_response.ep).

Environment names: `ECHO_UI_ASSETS`.

Definitions:

```text
7: define echo_asset with target as Str returning List:
```

## `app/ui/audio_capture.ep`

[app/ui/audio_capture.ep](../../app/ui/audio_capture.ep) · 40 lines · 1902 bytes

SHA-256: `ddee27ab7fd7545d91df4542cfd3c71cd21d35075036a9cfe5abaad5a2b31038`

Source annotations: Browser-native worklet; mono Float32 capture at the actual context sample rate. One second is a transfer batch, not a recording length limit. Stop flushes the final partial batch before an end receipt; no audio is played through output.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define echo_capture_processor:
17: define echo_capture_process with inputs and outputs and parameters:
36: define main:
```

## `app/ui/audio_controls.ep`

[app/ui/audio_controls.ep](../../app/ui/audio_controls.ep) · 211 lines · 10180 bytes

SHA-256: `b16dbb754676200ea880c14f395ebf5583c427f7101b24412b71815e62aac801`

Imports: none.

Definitions:

```text
1: define echo_audio_note with text:
7: define echo_audio_controls_refresh:
19: define echo_audio_unlock:
25: define echo_audio_attach with database and record and owner:
34: define echo_audio_attach_owned with database and record and owner:
47: define echo_audio_upload_body with database and record and owner and upload:
59: define echo_audio_upload_prepare with database and record and owner:
72: define echo_audio_upload_reconcile with database and record and owner:
88: define echo_audio_start with event:
123: define echo_audio_recovery_save with record and owner:
151: define echo_audio_recovery_item with record and owner and list:
170: define echo_audio_recovery_list with event:
196: define echo_audio_controls_setup:
```

Lexical test references: [tests/web/audio_upload_recovery_browser.ep](../../tests/web/audio_upload_recovery_browser.ep).

## `app/ui/audio_recorder.ep`

[app/ui/audio_recorder.ep](../../app/ui/audio_recorder.ep) · 222 lines · 10049 bytes

SHA-256: `adc4e3a94d73e89a8a52ac91706a4b00ccf719dbbf24ed841a75e4982dbab19a`

Source annotations: Capture controller shared by microphone UI and explicitly synthetic tests. The journal owns committed samples. No transcription or upload is implied. Retain pending bytes in memory until their transaction commits. A storage error stops capture instead of discarding samples and claiming success.

Imports: none.

Definitions:

```text
3: define echo_recorder_release with recorder:
15: define echo_recorder_fail with recorder and error:
23: define echo_recorder_chunk with recorder and samples:
43: define echo_recorder_finish with recorder:
61: define echo_recorder_stop with recorder:
72: define echo_recorder_create with owner and session and synthetic and notify:
113: define echo_recorder_initialize with recorder and synthetic:
176: define echo_audio_recover with database and identity and owner:
195: define echo_audio_saved_wave with database and identity and owner:
```

Lexical test references: [tests/web/audio_recorder_browser.ep](../../tests/web/audio_recorder_browser.ep).

## `app/ui/audio_store.ep`

[app/ui/audio_store.ep](../../app/ui/audio_store.ep) · 175 lines · 8400 bytes

SHA-256: `0604c51fea6e3fc16a53b97c23039d81008b85b7dc79ebce2c456b95b717ebaa`

Source annotations: Browser-local capture journal. The server archive remains canonical after upload. Strict transactions commit each chunk and its count together. Browser storage can be cleared or evicted; never describe it as a backup. All helpers resolve only after commit, never on individual request success.

Imports: none.

Definitions:

```text
4: define echo_audio_store_open:
24: define echo_audio_transaction with database and mode:
30: define echo_audio_commit with transaction and result:
40: define echo_audio_create with database and identity and owner and session and rate and settings:
58: define echo_audio_append with database and identity and owner and sequence and samples:
84: define echo_audio_seal with database and identity and owner and count:
100: define echo_audio_read with database and identity and owner and sequence:
126: define echo_audio_list with database and owner:
144: define echo_audio_confirm with database and identity and owner and attachment:
160: define echo_audio_bind_upload with database and identity and owner and previous and upload:
```

Lexical test references: [tests/web/audio_recorder_browser.ep](../../tests/web/audio_recorder_browser.ep), [tests/web/audio_store_browser.ep](../../tests/web/audio_store_browser.ep), [tests/web/audio_upload_recovery_browser.ep](../../tests/web/audio_upload_recovery_browser.ep).

## `app/ui/audio_wave.ep`

[app/ui/audio_wave.ep](../../app/ui/audio_wave.ep) · 71 lines · 2923 bytes

SHA-256: `25af3452ee47ed22fec4c3c7db3e2bdc422f4d745bf16e2496d3dba1ff38c407`

Source annotations: Lossless Float32 WAVE packaging of captured mono samples. RIFF's 32-bit size boundary selects RF64 rather than limiting recording duration or discarding source. Blob parts avoid concatenating a second giant sample buffer.

Imports: none.

Definitions:

```text
4: define echo_wave_word with view and offset and word:
11: define echo_wave_header with frames and rate:
52: define echo_wave_samples with chunk:
61: define echo_wave_blob with chunks and rate:
```

Lexical test references: [tests/web/audio_capture_browser.ep](../../tests/web/audio_capture_browser.ep).

## `app/ui/client.ep`

[app/ui/client.ep](../../app/ui/client.ep) · 5532 lines · 264159 bytes

SHA-256: `90dc37d8960a5ddeac917d678dded303c99faae27a5479c1fca108138e06bb7d`

Source annotations: Keep the newest turn visible without pulling readers away from older messages. Authored in Ernos; the browser program is generated by the reference compiler. Original message text is retained; assistant formatting escapes raw HTML. Only a current-run backend observation can start this indicator. Identical stage text must not preserve another conversation's spinner.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define echo_follow_latest:
10: define echo_note with text:
24: define echo_compaction_progress with result:
65: define echo_busy with busy:
98: define echo_error_code with error:
105: define echo_failure with source:
150: define echo_error with error:
156: define echo_catch with promise:
161: define echo_api with path and payload:
197: define echo_attachment_thumbnail with file:
231: define echo_pending_attachments:
260: define echo_remove_attachment with file and item:
263: define echo_attachment_lifecycle_button with file and action:
321: define echo_attachment_visibility_button with file and action:
383: define echo_attachment_visibility_control with file:
393: define echo_restore_legacy_attachment with file:
409: define echo_message_source with file:
437: define echo_copy_message with message:
455: define echo_share_destination with select and session and title:
467: define echo_share_button with message:
477: define echo_share_dialog with message and opener:
644: define echo_share_group_choices with select and query:
651: define echo_branch_button with message:
687: define echo_revision_start with message and mode and text:
722: define echo_regenerate_button with message:
732: define echo_edit_button with message:
779: define echo_code_controls with block:
803: define echo_format_response with element and original:
819: define echo_inline_audio with file and owner:
865: define echo_speak_button with message and article:
902: define echo_render_message with message:
961: define echo_message_attachments with article and message:
978: define echo_commit_messages with transcript and nodes and before:
995: define echo_hold_draft:
1018: define echo_history_page with before and marker:
1046: define echo_history_render with result and before and marker:
1097: define echo_recap_notice with marker:
1148: define echo_history:
1164: define echo_open_session with session:
1196: define echo_session_rename with session:
1257: define echo_session_button with session:
1273: define echo_sessions:
1291: define echo_sessions_render with result:
1307: define echo_thinking_panel with run:
1334: define echo_trace_load with panel:
1388: define echo_trace_clear_floating:
1396: define echo_trace_refresh:
1407: define echo_poll:
1489: define echo_clear_draft:
1499: define echo_draft:
1543: define echo_submit_payload with payload:
1550: define echo_reconcile_pending:
1581: define echo_send with event:
1657: define echo_new_chat with event:
1699: define echo_stop with event:
1716: define echo_retry with event:
1734: define echo_pair with event:
1761: define echo_export:
1793: define echo_verified_attachment with file:
1824: define echo_download_attachment with file:
1854: define echo_image_supported with file:
1857: define echo_release_image:
1867: define echo_open_attachment with file:
1875: define echo_preview_audio with file:
1956: define echo_preview_image with file:
2018: define echo_read_source with file:
2072: define echo_open_source with file:
2154: define echo_selected_files:
2165: define echo_file_choice with file:
2188: define echo_attachments:
2283: define echo_ensure_session:
2302: define echo_upload_file with event:
2308: define echo_upload_one with file:
2332: define echo_upload_prepared with file and identity:
2351: define echo_upload_files with originals:
2382: define echo_drag_files with event:
2392: define echo_drop_files with event:
2397: define echo_paste_files with event:
2406: define echo_attachment_inputs:
2413: define escapeHTML with str:
2427: define renderAiInlineMarkdown with text:
2447: define echo_table_cells with line:
2476: define echo_table_row with cells and alignments and heading:
2488: define echo_markdown_table with lines and start:
2525: define renderAiMarkdown with inputText:
2648: define echo_open_recap:
2773: define echo_theme_apply:
2781: define echo_theme_setup:
2805: define echo_suggestion with label and prompt and container:
2818: define echo_polish_layout:
2895: define echo_show_identity with target:
2912: define main:
3131: define echo_capability_label with value:
3138: define echo_provider_picker:
3184: define echo_provider_details with provider:
3212: define echo_provider_refresh with identity:
3236: define echo_group_action with path and payload and button:
3255: define echo_group_member_button with group and member and state and title:
3271: define echo_group_open with group:
3289: define echo_group_row with group and principal:
3352: define echo_groups_refresh:
3380: define echo_groups_setup:
3435: define echo_group_watch_start:
3476: define echo_group_history_choice with segment and group:
3490: define echo_group_watch with marker:
3573: define echo_outbox_prefix:
3579: define echo_outbox_park:
3597: define echo_outbox_restore with key:
3610: define echo_outbox_row with key and original:
3646: define echo_outbox_render:
3670: define echo_outbox_setup:
3708: define echo_queue_key:
3711: define echo_queue_read:
3714: define echo_queue_render:
3771: define echo_queue_add with steer:
3806: define echo_queue_next:
3857: define echo_steer_status with text:
3864: define echo_queue_setup:
3915: define echo_link_switch with result:
3928: define echo_link_list with rows:
3955: define echo_device_links_setup with parent:
4091: define echo_linked_session_watch:
4123: define echo_playback_setup:
4134: define echo_guidance_send with queued:
4198: define echo_guidance_render:
4226: define echo_guidance_clear with payload:
4243: define echo_project_field with parent and title and multiline:
4259: define echo_project_task with parent and task:
4287: define echo_project_file with parent and file and checked:
4303: define echo_projects_setup:
4319: define echo_projects_dialog with session and opener:
4567: define echo_research_panel with sources:
4592: define echo_state_button with run:
4601: define echo_state_dialog with run and opener:
4761: define echo_work_setup:
4773: define echo_work_dialog with session and opener:
4898: define echo_settings_section with navigation and content and title and description:
4923: define echo_settings_card with section and control and description:
4936: define echo_settings_setup:
5024: define echo_sidebar_dismiss_setup:
5064: define echo_agent_panel_load with panel:
5113: define echo_agents_refresh:
5212: define echo_agent_panel_bind with panel and older:
5223: define echo_agents_setup:
5245: define echo_interaction_clear:
5256: define echo_interaction_render with result:
5336: define echo_autonomy_command with enabled:
5368: define echo_question_field with item and question and result and panel and drafts and storage:
5412: define echo_question_option with question_group and option and choice and answer and free_answer and drafts and storage and item and question and result:
5437: define echo_question_submit with result and question and answers and submit and feedback and storage:
5474: define echo_autonomy_refresh:
5492: define echo_autonomy_location_render with result:
```

Lexical test references: [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/web/agent_compaction_browser.ep](../../tests/web/agent_compaction_browser.ep), [tests/web/api_response_browser.ep](../../tests/web/api_response_browser.ep), [tests/web/audio_playback_browser.ep](../../tests/web/audio_playback_browser.ep), [tests/web/audio_upload_recovery_browser.ep](../../tests/web/audio_upload_recovery_browser.ep), [tests/web/compaction_scope_browser.ep](../../tests/web/compaction_scope_browser.ep), [tests/web/group_speaker_browser.ep](../../tests/web/group_speaker_browser.ep), [tests/web/interim_response_browser.ep](../../tests/web/interim_response_browser.ep), [tests/web/linked_sync_browser.ep](../../tests/web/linked_sync_browser.ep), [tests/web/ordered_list_browser.ep](../../tests/web/ordered_list_browser.ep), [tests/web/outbox_browser.ep](../../tests/web/outbox_browser.ep), [tests/web/provider_details_browser.ep](../../tests/web/provider_details_browser.ep), [tests/web/research_panel_browser.ep](../../tests/web/research_panel_browser.ep).

## `app/ui/controller.ep`

[app/ui/controller.ep](../../app/ui/controller.ep) · 219 lines · 10337 bytes

SHA-256: `07fca1cbc7cfd37eadf821142c8ce924ea9d154c14d068a3615d22c1edf1e860`

Source annotations: Controller credentials stay in this page only, never browser storage or URLs.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
2: define controller_note with text:
7: define controller_call with path and payload:
29: define controller_decision_enabled with enabled:
38: define controller_failure with error:
46: define controller_inspect:
76: define controller_decide with decision:
114: define controller_emergency_refresh:
148: define controller_operation_inspect:
183: define controller_setup:
217: define main:
```

Lexical test references: [tests/web/controller_console_browser.ep](../../tests/web/controller_console_browser.ep).

## `app/ui/layout.ep`

[app/ui/layout.ep](../../app/ui/layout.ep) · 9 lines · 11991 bytes

SHA-256: `1130662cce3be7e8662b5d6d3ac38d7f85f8ec12648f299051b4c6dfb22c18ac`

Imports: none.

Definitions:

```text
1: define echo_layout_style returning Str:
```

Lexical test references: [tests/web/compaction_scope_browser.ep](../../tests/web/compaction_scope_browser.ep).

## `app/ui/math_render.ep`

[app/ui/math_render.ep](../../app/ui/math_render.ep) · 62 lines · 2543 bytes

SHA-256: `61179503a8eedd8a63d392b2340ef23bb69db9e5313e6f9b722560be64a3299d`

Source annotations: All renderer controls remain authored in English. Original equation text stays visible until a worker result arrives; cancellation keeps it available.

Imports: none.

Definitions:

```text
3: define echo_math_cancel_all:
10: define echo_math_render with element:
```

## `app/ui/math_worker.ep`

[app/ui/math_worker.ep](../../app/ui/math_worker.ep) · 17 lines · 750 bytes

SHA-256: `6ad03b42d4f6eed5e73d4f803b9336c396e77cfa43ebfb7d7328c0242cf40747`

Source annotations: Generated as a separate local browser worker. The caller can terminate a nonterminating expression without freezing chat or imposing an expansion cap.

Imports: none.

Native `main` present. Argument indices read: none. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
3: define main:
```

## `app/ui/page.ep`

[app/ui/page.ep](../../app/ui/page.ep) · 8 lines · 6813 bytes

SHA-256: `dda72e69551ef2182d366ca7565ddb9fcb367a0a5a9f74e66ae4c1ba9380e261`

Imports: [theme](../../app/ui/theme.ep), [layout](../../app/ui/layout.ep).

Definitions:

```text
4: define echo_page returning Str:
```

## `app/ui/theme.ep`

[app/ui/theme.ep](../../app/ui/theme.ep) · 2 lines · 1809 bytes

SHA-256: `5411c7c154cf3a5b7b6518cd418864317c3b6c47ddbeb6c79be7ec12e64a748f`

Imports: none.

Definitions:

```text
1: define echo_theme_style returning Str:
```

## `app/ui/watch_party.ep`

[app/ui/watch_party.ep](../../app/ui/watch_party.ep) · 321 lines · 16417 bytes

SHA-256: `3c9d296c5b6ff4db57e38140db03c3c7af947bf97e205a0f335c5c7e9c8a1037`

Source annotations: This module is emitted with the accepted client overlay. Media remains local; control state is authenticated and durable in the conversation archive.

Imports: none.

Definitions:

```text
3: define echo_watch_apply with watch and state:
35: define echo_watch_change with watch and action:
63: define echo_watch_open with file:
```

## `app/verify_export.ep`

[app/verify_export.ep](../../app/verify_export.ep) · 17 lines · 752 bytes

SHA-256: `02ed659edac37e33084006164e17c2c0ea1c3a8cabfa0b6246d5bb09d65cf6a6`

Imports: [archive/export_verify](../../app/archive/export_verify.ep), `fs` (toolchain/library).

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```

## `app/web/activity.ep`

[app/web/activity.ep](../../app/web/activity.ep) · 100 lines · 7036 bytes

SHA-256: `41a7e292ec642feed0e26f7ec998dce0731f42b562a6a4c646fd2c77c23e726b`

Source annotations: Read-only status derived from committed work belonging to this run and its current head. Do not infer progress percentages, expose model draft recaps, or attribute another conversation's activity to the requesting device.

Imports: [../retained_text](../../app/retained_text.ep), [run_store](../../app/web/run_store.ep).

Definitions:

```text
7: define web_run_activity with database as Int and run as Str returning Str:
89: define web_run_compaction with database as Int and run as Str returning Str:
```

Lexical test references: [tests/web/test_activity.ep](../../tests/web/test_activity.ep).

## `app/web/attachment_lifecycle.ep`

[app/web/attachment_lifecycle.ep](../../app/web/attachment_lifecycle.ep) · 53 lines · 4232 bytes

SHA-256: `b7d97fb1efdf0e9c46e67262451d9770ec714d83e1f2e863ba0dc5dc9f207cb0`

Source annotations: A reversible archive receipt. No raw byte deletion or memory erasure occurs. Shared changes require the current audience; linked private devices resolve the original owner while preserving the actual actor in the receipt.

Imports: [attachments](../../app/web/attachments.ep), [device_links](../../app/web/device_links.ep), [../archive/attachment_lifecycle](../../app/archive/attachment_lifecycle.ep).

Definitions:

```text
8: define web_attachment_lifecycle with database as Int and device as Str and tokens as List returning Str:
```

Lexical test references: [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep).

## `app/web/attachment_usage.ep`

[app/web/attachment_usage.ep](../../app/web/attachment_usage.ep) · 49 lines · 4174 bytes

SHA-256: `56535d6d0778580d7af3d94729c25f607d19654c92b5fc582f822beb95fde6ee`

Source annotations: Recorded references only: this does not infer semantic derivation from text. No originals, claims, checkpoints or visibility settings are changed here.

Imports: [attachment_lifecycle](../../app/web/attachment_lifecycle.ep).

Definitions:

```text
5: define web_attachment_usage with database as Int and device as Str and attachment as Str returning Str:
15: define web_attachment_usage_snapshot with database as Int and device as Str and attachment as Str returning Str:
```

Lexical test references: [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep).

## `app/web/attachment_visibility.ep`

[app/web/attachment_visibility.ep](../../app/web/attachment_visibility.ep) · 62 lines · 4915 bytes

SHA-256: `9cc651af73c79de85f10500af64d93056dfece536df409bc5b3d756f8530133b`

Source annotations: Reversible context visibility receipt; no originals are erased. HTTP callers use the same visibility receipt and scoped authority checks. Shared changes require the current audience; linked private devices resolve the original owner while preserving the actual actor in the receipt.

Imports: [attachments](../../app/web/attachments.ep), [device_links](../../app/web/device_links.ep), [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep).

Definitions:

```text
9: define web_attachment_visibility with database as Int and device as Str and tokens as List returning Str:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep).

## `app/web/attachments.ep`

[app/web/attachments.ep](../../app/web/attachments.ep) · 170 lines · 14733 bytes

SHA-256: `35ce30d15c3e2179a1872981cefe1d96768cbcaa3ac4450ffd619752cfd92ad0`

Source annotations: Session IDs are opaque references, not authority. Shared reads require this authenticated principal in the recorded audience and still joined now; publication additionally requires the current audience revision. Host-only storage primitive. Path must be a closed, exclusively owned staging file; browser names are metadata, never filesystem paths. Publication and ownership recheck commit with the original attachment event.

Imports: [../archive/attachment_visibility](../../app/archive/attachment_visibility.ep), [../security/group_history](../../app/security/group_history.ep), [../retained_text](../../app/retained_text.ep), [run_store](../../app/web/run_store.ep), [../archive/file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
7: define web_attachment_schema with database as Int returning Int:
14: define web_attachment_session_allowed with database as Int and device as Str and session as Str and writing as Int returning Int:
29: define web_attachment_register with database as Int and device as Str and session as Str and attachment as Str and path as Str and name as Str and media_type as Str returning Int:
35: define web_attachment_register_call with database as Int and device as Str and session as Str and attachment as Str and path as Str and name as Str and media_type as Str and run as Str and call_id as Str returning Int:
40: define web_attachment_register_derived_call with database as Int and device as Str and session as Str and attachment as Str and path as Str and name as Str and media_type as Str and run as Str and call_id as Str and parent as Str returning Int:
94: define web_attachment_owned_snapshot with database as Int and device as Str and attachment as Str returning List:
103: define web_attachment_owned with database as Int and device as Str and attachment as Str returning List:
113: define web_attachment_list_snapshot with database as Int and device as Str and session as Str returning Str:
162: define web_attachment_list with database as Int and device as Str and session as Str returning Str:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep), [tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep), [tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_imported_attachments.ep](../../tests/memory/test_imported_attachments.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/project_controls_file.ep](../../tests/web/project_controls_file.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_empty_session.ep](../../tests/web/test_empty_session.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep).

## `app/web/branches.ep`

[app/web/branches.ep](../../app/web/branches.ep) · 115 lines · 9246 bytes

SHA-256: `1c0a25055a21db98a78c9c0f4823765adb385dddb5abfc5e370acf576e221192`

Source annotations: Branch creation never copies, rewrites or deletes original events. The saved endpoint and digest identify the immutable source prefix for later reads. Only accepted completed answers are boundaries in this operation; edit and regenerate use their own boundary selection before launching a new run.

Imports: [../retained_text](../../app/retained_text.ep), [run_store](../../app/web/run_store.ep).

Definitions:

```text
8: define web_branch_at with database as Int and device as Str and source as Str and endpoint as Str and identity as Str and mode as Str and replacement as Str returning List:
11: define web_branch_at_provider with database as Int and device as Str and source as Str and endpoint as Str and identity as Str and mode as Str and replacement as Str and provider_identity as Str returning List:
70: define web_branch_session with database as Int and device as Str and source as Str and endpoint as Str and identity as Str returning List:
75: define web_revision with database as Int and device as Str and source as Str and user_event as Str and identity as Str and mode as Str and text as Str returning List:
78: define web_revision_provider with database as Int and device as Str and source as Str and user_event as Str and identity as Str and mode as Str and text as Str and provider_identity as Str returning List:
81: define web_revision_environment with database as Int and device as Str and source as Str and user_event as Str and identity as Str and mode as Str and text as Str and provider_identity as Str and environment as Str returning List:
```

Lexical test references: [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep).

## `app/web/cancellation.ep`

[app/web/cancellation.ep](../../app/web/cancellation.ep) · 107 lines · 6143 bytes

SHA-256: `6de184d8aeed5acd4b643c86c1c744899a8f784215141665cf6556c02dfabc13`

Source annotations: A durable request is distinct from completion. Only the owning active device may request it; the worker records the actual terminal transition. The monitor owns the cancellation writer. Closing it wakes a blocked native provider read/write/connect. A separate stop pipe ends the monitor when normal work finishes. Database failure also wakes the worker rather than leaving an uncancellable inference request running indefinitely.

Imports: [../interaction/autonomy](../../app/interaction/autonomy.ep), [shared_access](../../app/web/shared_access.ep), [../retained_text](../../app/retained_text.ep), [http_api](../../app/web/http_api.ep), `select` (toolchain/library).

Definitions:

```text
7: define web_cancel_schema with database as Int returning Int:
12: define web_cancel_request with database as Int and device as Str and run as Str returning Int:
28: define web_cancel_monitor with database_path as Str and run as Str and stop_reader as Int and cancellation_writer as Int and completed as Int returning Int:
80: define web_guidance_request with database as Int and device as Str and run as Str and identity as Str and message as Str returning Int:
83: define web_guidance_environment with database as Int and device as Str and run as Str and identity as Str and message as Str and environment as Str returning Int:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep).

## `app/web/compaction.ep`

[app/web/compaction.ep](../../app/web/compaction.ep) · 8 lines · 403 bytes

SHA-256: `05f49e4156b544d8b8234e1723aa7d87bd508ab318df443fe938719a00d426e7`

Imports: [attachments](../../app/web/attachments.ep), [devices](../../app/web/devices.ep), [../tools/compaction_tools](../../app/tools/compaction_tools.ep).

Definitions:

```text
5: define web_compaction_read with database as Int and device as Str and session as Str and arguments as Str returning Str:
```

Lexical test references: [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep).

## `app/web/device_links.ep`

[app/web/device_links.ep](../../app/web/device_links.ep) · 99 lines · 7639 bytes

SHA-256: `49d5d047675ef2ab48fe4793974290aecdf2bb35d5b4c4bfa9581706f0868ca6`

Source annotations: Linked browsers have distinct bearer credentials for an explicitly selected existing account. Explicit merges share a library without rewriting originals.

Imports: [devices](../../app/web/devices.ep), [../tools/tool_registry](../../app/tools/tool_registry.ep).

Definitions:

```text
6: define device_link_schema with database as Int returning Str:
15: define device_link_create with database as Int and account as Str and secret as Str returning Int:
21: define device_link_redeem with database as Int and secret as Str and identity as Str and name as Str and credential as Str returning Str:
24: define device_link_redeem_merge with database as Int and secret as Str and identity as Str and name as Str and credential as Str and previous_credential as Str returning Str:
52: define device_link_revoke with database as Int and account as Str and identity as Str returning Int:
57: define device_link_list with database as Int and account as Str returning Str:
79: define device_link_merge_pending with database as Int and target as Str and source as Str returning Int:
93: define device_link_merge with database as Int and target as Str and source as Str returning Int:
```

Lexical test references: [tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep), [tests/web/test_device_links.ep](../../tests/web/test_device_links.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep).

## `app/web/devices.ep`

[app/web/devices.ep](../../app/web/devices.ep) · 160 lines · 10030 bytes

SHA-256: `e282152534b7ccfe777f691a30a28429440e7bfa0f11dc0aa09a7bc674a96354`

Source annotations: Credentials are random bearer secrets supplied by the host pairing flow. Only their digests are retained. Invitations are consumed in the same transaction that creates the device, so concurrent redemption cannot share it.

Imports: [../retained_text](../../app/retained_text.ep), [../security/principals](../../app/security/principals.ep), [../agent/runner](../../app/agent/runner.ep).

Definitions:

```text
8: define web_device_schema with database as Int returning Int:
11: define web_device_write with database as Int and query as Str and values as List returning Int:
17: define web_pairing_invite with database as Int and invitation as Str returning Int:
24: define web_device_pair with database as Int and invitation as Str and device as Str and name as Str and credential as Str returning Int:
51: define web_device_authenticate with database as Int and credential as Str returning Str:
69: define web_device_revoke with database as Int and device as Str returning Int:
76: define web_device_own_pending with database as Int and device as Str and session as Str returning Int:
82: define web_device_session_allowed with database as Int and device as Str and session as Str returning Int:
93: define web_device_run_allowed with database as Int and device as Str and run as Str returning Int:
106: define web_device_identity with database as Int and device as Str returning Str:
125: define web_account_devices_sql with database as Int returning Str:
133: define web_account_session_device with database as Int and device as Str and session as Str returning Str:
143: define web_account_resource_device with database as Int and device as Str and kind as Str and identity as Str returning Str:
```

Lexical test references: [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep), [tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_enrollment.ep](../../tests/lifecycle/test_controller_enrollment.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_runtime_binding_pressure.ep](../../tests/memory/test_runtime_binding_pressure.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/prepare_http_principals.ep](../../tests/security/prepare_http_principals.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep), [tests/security/test_decision_gate.ep](../../tests/security/test_decision_gate.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_operation_prepare.ep](../../tests/security/test_operation_prepare.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep), [tests/security/test_principals.ep](../../tests/security/test_principals.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_protected_emergency.ep](../../tests/security/test_protected_emergency.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_protected_steward.ep](../../tests/security/test_protected_steward.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_candidate_discovery.ep](../../tests/tools/test_candidate_discovery.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/prepare_guardian_devices.ep](../../tests/web/prepare_guardian_devices.ep), [tests/web/project_controls_file.ep](../../tests/web/project_controls_file.ep), [tests/web/provision_crash_probe.ep](../../tests/web/provision_crash_probe.ep), [tests/web/seed_device_routes.ep](../../tests/web/seed_device_routes.ep), [tests/web/seed_retry_browser.ep](../../tests/web/seed_retry_browser.ep), [tests/web/share_controls_seed.ep](../../tests/web/share_controls_seed.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep), [tests/web/test_device_links.ep](../../tests/web/test_device_links.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_empty_session.ep](../../tests/web/test_empty_session.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_attach_server.ep](../../tests/web/test_live_attach_server.ep), [tests/web/test_live_continuity_server.ep](../../tests/web/test_live_continuity_server.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_session_rename.ep](../../tests/web/test_session_rename.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/web/test_uploads.ep](../../tests/web/test_uploads.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/web/export.ep`

[app/web/export.ep](../../app/web/export.ep) · 108 lines · 6175 bytes

SHA-256: `78c515e064883d126615a94b103817ec859f1b67e8884a2b672475a87fbe4080`

Source annotations: Export one verified read snapshot. Payload strings retain their original bytes and hashes; credentials and other devices' sessions are never included.

Imports: [../retained_text](../../app/retained_text.ep), [../archive/export_verify](../../app/archive/export_verify.ep), [run_store](../../app/web/run_store.ep), [attachments](../../app/web/attachments.ep).

Definitions:

```text
8: define web_session_export_prefix with database as Int and device as Str and session as Str and boundary as Int returning Str:
46: define web_session_export with database as Int and device as Str and session as Str returning Str:
```

Lexical test references: [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep).

## `app/web/groups.ep`

[app/web/groups.ep](../../app/web/groups.ep) · 200 lines · 14340 bytes

SHA-256: `08fb8e470abdb8df686579429c5832f1341c84e601ea23aab591e0396cad4ac8`

Source annotations: Browser routes cannot forge a Discord/server namespace. Membership remains a principal decision; no role text or invitation grants tool authority.

Imports: [../security/group_route](../../app/security/group_route.ep), [../security/admin_silo](../../app/security/admin_silo.ep), [shared_access](../../app/web/shared_access.ep).

Definitions:

```text
7: define web_group_ready with database as Int returning Int:
11: define web_group_namespace with database as Int and identity as Str returning Int:
16: define web_group_fields with tokens as List and allowed as List returning Int:
35: define web_group_list with database as Int and device as Str returning Str:
62: define web_group_manage with database as Int and device as Str and action as Str and tokens as List returning Str:
108: define web_group_state with database as Int and device as Str and identity as Str returning Str:
116: define web_group_state_read with database as Int and device as Str and identity as Str returning Str:
160: define web_group_submit with database as Int and device as Str and tokens as List returning List:
163: define web_group_submit_environment with database as Int and device as Str and tokens as List and environment as Str returning List:
```

Lexical test references: [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep).

## `app/web/http_api.ep`

[app/web/http_api.ep](../../app/web/http_api.ep) · 196 lines · 11071 bytes

SHA-256: `c87a3a30d5190b2b3e0bdf08ae3568ace06932beab8e2a6e880ec33358adac83`

Source annotations: Empty means absent; duplicate matching fields are explicitly ambiguous. The token is generated once per server process and delivered in same-origin

Imports: [../retained_text](../../app/retained_text.ep), [run_store](../../app/web/run_store.ep).

Definitions:

```text
5: define web_header with headers as List and name as Str returning List:
21: define web_local_host with request as List and port as Int returning Int:
33: define web_authorized with request as List and port as Int and token as Str and mutation as Int returning Int:
36: define web_https_origin_valid with origin as Str returning Int:
59: define web_origin_allowed with request as List and port as Int and required as Int and public_origin as Str returning Int:
77: define web_authorized_origin with request as List and port as Int and token as Str and mutation as Int and public_origin as Str returning Int:
88: define web_read_json with reader as List and request as List and read_size as Int returning List:
136: define web_submit_json with database as Int and tokens as List returning List:
139: define web_submit_json_owned with database as Int and device as Str and tokens as List returning List:
142: define web_submit_json_environment with database as Int and device as Str and tokens as List and environment as Str returning List:
180: define web_submit_payload with result as List returning Str:
187: define web_message_environment with request as List returning Str:
```

Lexical test references: [tests/lifecycle/controller_console_preview.ep](../../tests/lifecycle/controller_console_preview.ep), [tests/web/serve_partial_fixture.ep](../../tests/web/serve_partial_fixture.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep).

## `app/web/interaction.ep`

[app/web/interaction.ep](../../app/web/interaction.ep) · 131 lines · 11533 bytes

SHA-256: `e8efde3c82e128eedceccc982af18c6b1d3338612a3a1205346d9b78ed1ba8be`

Source annotations: Validate the whole submitted batch against its immutable question source. Choice indexes and explicit free text distinguish Other from option labels.

Imports: [cancellation](../../app/web/cancellation.ep), [../interaction/questions](../../app/interaction/questions.ep), [../interaction/autonomy](../../app/interaction/autonomy.ep).

Definitions:

```text
7: define web_questions_answer with database as Int and device as Str and run as Str and request as Str and answers as Str returning Int:
10: define web_questions_environment with database as Int and device as Str and run as Str and request as Str and answers as Str and environment as Str returning Int:
57: define web_autonomy_command with database as Int and device as Str and run as Str and identity as Str and enabled as Int returning Int:
60: define web_autonomy_command_goal with database as Int and device as Str and run as Str and identity as Str and enabled as Int and goal as Str returning Int:
63: define web_autonomy_environment with database as Int and device as Str and run as Str and identity as Str and enabled as Int and goal as Str and environment as Str returning Int:
110: define web_autonomy_status with database as Int and device as Str returning Str:
```

Lexical test references: [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep).

## `app/web/memory_sharing.ep`

[app/web/memory_sharing.ep](../../app/web/memory_sharing.ep) · 26 lines · 1859 bytes

SHA-256: `c5b45b1f00cebcce45f89b97c384b582ca41f5b31ea4c8228cdc64ca7675e0d2`

Source annotations: Device comes from bearer authentication, never the JSON body. This route is intentionally absent from model tool discovery and dispatch.

Imports: [../memory/sharing](../../app/memory/sharing.ep).

Definitions:

```text
5: define web_memory_share with database as Int and device as Str and tokens as List returning Str:
```

Lexical test references: [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep).

## `app/web/message_speech.ep`

[app/web/message_speech.ep](../../app/web/message_speech.ep) · 89 lines · 7736 bytes

SHA-256: `847f598149280e5dd1d0d1b3db08d6a4c6d533924b7389f8502413382fce48f5`

Source annotations: Idempotent direct speech of an owned, immutable assistant message. This uses the existing speech executor and tool ledger without another model turn. Bind reuse to the installed voice profile, not just the text. Older audio remains archived; a voice change creates a fresh speech job.

Imports: [../retained_text](../../app/retained_text.ep), [../media/speech_tool](../../app/media/speech_tool.ep).

Environment names: `ECHO_VOICE_PROFILE`.

Definitions:

```text
6: define web_message_speak with database as Int and device as Str and source as Str returning Str:
```

Lexical test references: [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep).

## `app/web/progress.ep`

[app/web/progress.ep](../../app/web/progress.ep) · 84 lines · 4608 bytes

SHA-256: `ef8364d0746a914b18050ca366a2ca9e45f6f86072d28425c4f83474b6c92be4`

Source annotations: This is display-only draft text. It cannot create tool calls or mark a run complete. The cursor addresses committed original events, and changes of generation reset it, so tool turns cannot concatenate unrelated drafts.

Imports: [../retained_text](../../app/retained_text.ep), [run_store](../../app/web/run_store.ep).

Definitions:

```text
7: define web_run_progress with database as Int and run as Str and requested_generation as Str and after as Int returning Str:
```

Lexical test references: [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep).

## `app/web/projects.ep`

[app/web/projects.ep](../../app/web/projects.ep) · 128 lines · 9995 bytes

SHA-256: `ced61d5159ad9edaa9202783acae4b5c2ad3c13290ee365928130d5b81a25669`

Source annotations: Human project actions use the authenticated device and selected conversation, not an old model run's authority. No artificial model turn is created.

Imports: [../projects/context](../../app/projects/context.ep), [../memory/sharing](../../app/memory/sharing.ep).

Definitions:

```text
6: define web_project_request with database as Int and device as Str and arguments as Str returning Str:
25: define web_project_pending with database as Int and device as Str and arguments as Str and tokens as List returning Str:
```

Lexical test references: [tests/web/test_projects.ep](../../tests/web/test_projects.ep).

## `app/web/recovery.ep`

[app/web/recovery.ep](../../app/web/recovery.ep) · 91 lines · 6237 bytes

SHA-256: `ed1e783dcd65677232b698f1305afc7ae2e9becdea0719904d3344376e1f4d34`

Source annotations: Only lock-aware claims are eligible. Legacy claims have no proof that their live workers observe this lock protocol and must not be silently reclaimed. Restore generation only after proving the abandoned claim under its lock.

Imports: [../transport/isolated_process](../../app/transport/isolated_process.ep), [../retained_text](../../app/retained_text.ep), [worker](../../app/web/worker.ep), [../agent/generation_recovery](../../app/agent/generation_recovery.ep), [../agent/tool_recovery](../../app/agent/tool_recovery.ep).

Definitions:

```text
10: define web_worker_recover_ready with database as Int and run as Str and previous as Str and worker as Str and library as Int returning Int:
32: define web_worker_recover_owned with database as Int and run as Str and previous as Str and worker as Str returning Int:
57: define web_recover_ready_workers with database as Int and database_path as Str and model as Str and port as Int and reserve as Int and batch as Int returning Int:
60: define web_recover_ready_workers_provider with database as Int and database_path as Str and profile as Str and reserve as Int and batch as Int returning Int:
```

Lexical test references: [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep).

## `app/web/routes.ep`

[app/web/routes.ep](../../app/web/routes.ep) · 539 lines · 42403 bytes

SHA-256: `d8e44f4088ae6b0098e0cf6888eda66ea5592e75501769626d3bef073410d540`

Source annotations: Configuration: database path, model, provider port, workload reserve, I/O batch size. It is immutable while workers use it. Optional index 8 carries the host-selected provider profile.

Imports: [../interaction/watch_party](../../app/interaction/watch_party.ep), [watch_schedule](../../app/web/watch_schedule.ep), [../feedback](../../app/feedback.ep), [attachment_visibility](../../app/web/attachment_visibility.ep), [device_links](../../app/web/device_links.ep), [memory_sharing](../../app/web/memory_sharing.ep), [attachment_lifecycle](../../app/web/attachment_lifecycle.ep), [projects](../../app/web/projects.ep), [work_status](../../app/web/work_status.ep), [../hud/interface](../../app/hud/interface.ep), [groups](../../app/web/groups.ep), [../transport/isolated_process](../../app/transport/isolated_process.ep), [../retained_text](../../app/retained_text.ep), [message_speech](../../app/web/message_speech.ep), [trace](../../app/web/trace.ep), [branches](../../app/web/branches.ep), [worker](../../app/web/worker.ep), [progress](../../app/web/progress.ep), [export](../../app/web/export.ep), [uploads](../../app/web/uploads.ep), [compaction](../../app/web/compaction.ep), [../providers/status](../../app/providers/status.ep).

Definitions:

```text
27: define web_config_provider with config as List returning Str:
32: define web_reply with status as Str and body as Str returning List:
41: define web_route with reader as List and request as List and port as Int and config as List returning List:
520: define web_receive_upload with database as Int and device as Str and reader as List and request as List and batch as Int returning List:
```

Lexical test references: [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep).

## `app/web/run_store.ep`

[app/web/run_store.ep](../../app/web/run_store.ep) · 438 lines · 31634 bytes

SHA-256: `e0db9efb9dc7755051f7f4cba26a714450a4284b295b98263057af90f4f60b8f`

Imports: [../interaction/autonomy](../../app/interaction/autonomy.ep), [../feedback](../../app/feedback.ep), [../security/node_gate](../../app/security/node_gate.ep), [../retained_text](../../app/retained_text.ep), [devices](../../app/web/devices.ep), [source_reads](../../app/web/source_reads.ep), [../archive/turn_attachments](../../app/archive/turn_attachments.ep), [../providers/registry](../../app/providers/registry.ep), [../security/group_speakers](../../app/security/group_speakers.ep), [../context/message_environment](../../app/context/message_environment.ep).

Definitions:

```text
12: define web_run_schema with database as Int returning Int:
21: define web_submit_result with status as Str and session as Str and run as Str and created as Int returning List:
29: define web_submit_abort with database as Int and status as Str returning List:
38: define web_submit with database as Int and submission as Str and requested_session as Str and message as Str returning List:
43: define web_submit_owned with database as Int and device as Str and submission as Str and requested_session as Str and message as Str returning List:
46: define web_submit_owned_files with database as Int and device as Str and submission as Str and requested_session as Str and message as Str and attachments as List returning List:
52: define web_submit_owned_provider with database as Int and device as Str and submission as Str and requested_session as Str and message as Str and attachments as List and provider_identity as Str returning List:
55: define web_submit_owned_environment with database as Int and device as Str and submission as Str and requested_session as Str and message as Str and attachments as List and provider_identity as Str and environment as Str returning List:
180: define web_run_status with database as Int and run as Str returning Str:
208: define web_session_transcript with database as Int and session as Str returning Str:
213: define web_session_transcript_window with database as Int and session as Str and before as Int and amount as Int returning Str:
317: define web_device_sessions with database as Int and device as Str returning Str:
322: define web_device_sessions_search with database as Int and device as Str and query as Str returning Str:
370: define web_create_session with database as Int and device as Str and identity as Str returning List:
408: define web_rename_session with database as Int and device as Str and session as Str and previous as Str and title as Str returning Str:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep), [tests/agent/test_attachment_context_visibility.ep](../../tests/agent/test_attachment_context_visibility.ep), [tests/agent/test_attachment_loop.ep](../../tests/agent/test_attachment_loop.ep), [tests/agent/test_execution_control.ep](../../tests/agent/test_execution_control.ep), [tests/agent/test_execution_loop.ep](../../tests/agent/test_execution_loop.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/agent/test_process_result_recovery.ep](../../tests/agent/test_process_result_recovery.ep), [tests/agent/test_two_turn_loop.ep](../../tests/agent/test_two_turn_loop.ep), [tests/agent/test_workspace_create_loop.ep](../../tests/agent/test_workspace_create_loop.ep), [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_attachment_snapshot.ep](../../tests/archive/test_attachment_snapshot.ep), [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/archive/test_pdf_cache.ep](../../tests/archive/test_pdf_cache.ep), [tests/context/seed_recap_viewer.ep](../../tests/context/seed_recap_viewer.ep), [tests/context/test_attachment_gate.ep](../../tests/context/test_attachment_gate.ep), [tests/context/test_attachment_recap_sources.ep](../../tests/context/test_attachment_recap_sources.ep), [tests/context/test_compaction_read.ep](../../tests/context/test_compaction_read.ep), [tests/context/test_group_recap_authority.ep](../../tests/context/test_group_recap_authority.ep), [tests/context/test_group_recap_membership.ep](../../tests/context/test_group_recap_membership.ep), [tests/context/test_group_recap_rebuild.ep](../../tests/context/test_group_recap_rebuild.ep), [tests/context/test_live_group_recap.ep](../../tests/context/test_live_group_recap.ep), [tests/context/test_recap_rebuild.ep](../../tests/context/test_recap_rebuild.ep), [tests/context/test_turn_clock.ep](../../tests/context/test_turn_clock.ep), [tests/fixtures/schedule_fixture.ep](../../tests/fixtures/schedule_fixture.ep), [tests/fixtures/tool_call_fixture.ep](../../tests/fixtures/tool_call_fixture.ep), [tests/interaction/test_controls.ep](../../tests/interaction/test_controls.ep), [tests/jobs/test_delegation.ep](../../tests/jobs/test_delegation.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_nonblocking.ep](../../tests/jobs/test_delegation_nonblocking.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_return.ep](../../tests/jobs/test_delegation_return.ep), [tests/jobs/test_delegation_return_overlap.ep](../../tests/jobs/test_delegation_return_overlap.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_checklist_return.ep](../../tests/jobs/test_live_checklist_return.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_store.ep](../../tests/jobs/test_schedule_store.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/jobs/test_task_checklist.ep](../../tests/jobs/test_task_checklist.ep), [tests/lifecycle/test_controller_operations_privileged.ep](../../tests/lifecycle/test_controller_operations_privileged.ep), [tests/lifecycle/test_controller_requests.ep](../../tests/lifecycle/test_controller_requests.ep), [tests/lifecycle/test_controller_review_inputs.ep](../../tests/lifecycle/test_controller_review_inputs.ep), [tests/lifecycle/test_live_controller_review.ep](../../tests/lifecycle/test_live_controller_review.ep), [tests/media/test_group_image_tool.ep](../../tests/media/test_group_image_tool.ep), [tests/media/test_group_message_speech.ep](../../tests/media/test_group_message_speech.ep), [tests/media/test_group_voice_tool.ep](../../tests/media/test_group_voice_tool.ep), [tests/media/test_image_control.ep](../../tests/media/test_image_control.ep), [tests/media/test_image_tool.ep](../../tests/media/test_image_tool.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_speech_control.ep](../../tests/media/test_speech_control.ep), [tests/media/test_speech_tool.ep](../../tests/media/test_speech_tool.ep), [tests/media/test_video_library.ep](../../tests/media/test_video_library.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_control.ep](../../tests/media/test_voice_control.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/media/test_voice_tool.ep](../../tests/media/test_voice_tool.ep), [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep), [tests/memory/test_attachment_visibility_recall.ep](../../tests/memory/test_attachment_visibility_recall.ep), [tests/memory/test_corrected_recall.ep](../../tests/memory/test_corrected_recall.ep), [tests/memory/test_episode_extraction.ep](../../tests/memory/test_episode_extraction.ep), [tests/memory/test_episodes.ep](../../tests/memory/test_episodes.ep), [tests/memory/test_extraction_coordination.ep](../../tests/memory/test_extraction_coordination.ep), [tests/memory/test_group_attachment_visibility_recall.ep](../../tests/memory/test_group_attachment_visibility_recall.ep), [tests/memory/test_group_extraction.ep](../../tests/memory/test_group_extraction.ep), [tests/memory/test_group_knowledge.ep](../../tests/memory/test_group_knowledge.ep), [tests/memory/test_group_recall.ep](../../tests/memory/test_group_recall.ep), [tests/memory/test_live_corrected_recall.ep](../../tests/memory/test_live_corrected_recall.ep), [tests/memory/test_live_episode_extraction.ep](../../tests/memory/test_live_episode_extraction.ep), [tests/memory/test_live_episodes.ep](../../tests/memory/test_live_episodes.ep), [tests/memory/test_live_procedure_lifecycle.ep](../../tests/memory/test_live_procedure_lifecycle.ep), [tests/memory/test_live_procedures.ep](../../tests/memory/test_live_procedures.ep), [tests/memory/test_live_source_import.ep](../../tests/memory/test_live_source_import.ep), [tests/memory/test_memory_recovery.ep](../../tests/memory/test_memory_recovery.ep), [tests/memory/test_memory_sharing.ep](../../tests/memory/test_memory_sharing.ep), [tests/memory/test_procedure_attachment_visibility.ep](../../tests/memory/test_procedure_attachment_visibility.ep), [tests/memory/test_procedure_lifecycle.ep](../../tests/memory/test_procedure_lifecycle.ep), [tests/memory/test_procedure_store.ep](../../tests/memory/test_procedure_store.ep), [tests/memory/test_runtime_binding_pressure.ep](../../tests/memory/test_runtime_binding_pressure.ep), [tests/memory/test_runtime_memory.ep](../../tests/memory/test_runtime_memory.ep), [tests/memory/test_source_import.ep](../../tests/memory/test_source_import.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/projects/test_live_project.ep](../../tests/projects/test_live_project.ep), [tests/projects/test_project_store.ep](../../tests/projects/test_project_store.ep), [tests/projects/test_project_tools.ep](../../tests/projects/test_project_tools.ep), [tests/providers/measure_recorded_stream.ep](../../tests/providers/measure_recorded_stream.ep), [tests/providers/measure_stream_readers.ep](../../tests/providers/measure_stream_readers.ep), [tests/providers/test_attachment_audio.ep](../../tests/providers/test_attachment_audio.ep), [tests/providers/test_attachment_image.ep](../../tests/providers/test_attachment_image.ep), [tests/providers/test_audio_context_visibility.ep](../../tests/providers/test_audio_context_visibility.ep), [tests/providers/test_image_context_visibility.ep](../../tests/providers/test_image_context_visibility.ep), [tests/providers/test_live_audio_fallback.ep](../../tests/providers/test_live_audio_fallback.ep), [tests/providers/test_live_image_agent.ep](../../tests/providers/test_live_image_agent.ep), [tests/providers/test_live_image_compaction.ep](../../tests/providers/test_live_image_compaction.ep), [tests/providers/test_live_image_retrieval.ep](../../tests/providers/test_live_image_retrieval.ep), [tests/providers/test_live_thinking_multiturn.ep](../../tests/providers/test_live_thinking_multiturn.ep), [tests/providers/test_live_unified_image.ep](../../tests/providers/test_live_unified_image.ep), [tests/security/prepare_http_principals.ep](../../tests/security/prepare_http_principals.ep), [tests/security/test_accounts.ep](../../tests/security/test_accounts.ep), [tests/security/test_admin_silo.ep](../../tests/security/test_admin_silo.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_automatic_memory.ep](../../tests/security/test_automatic_memory.ep), [tests/security/test_channel_public.ep](../../tests/security/test_channel_public.ep), [tests/security/test_channel_silo.ep](../../tests/security/test_channel_silo.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_correction_precedence.ep](../../tests/security/test_correction_precedence.ep), [tests/security/test_device_links.ep](../../tests/security/test_device_links.ep), [tests/security/test_direct_speech_authority.ep](../../tests/security/test_direct_speech_authority.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_group_direct_speech_authority.ep](../../tests/security/test_group_direct_speech_authority.ep), [tests/security/test_group_media.ep](../../tests/security/test_group_media.ep), [tests/security/test_group_media_authority.ep](../../tests/security/test_group_media_authority.ep), [tests/security/test_group_media_receipts.ep](../../tests/security/test_group_media_receipts.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_route.ep](../../tests/security/test_group_route.ep), [tests/security/test_group_runs.ep](../../tests/security/test_group_runs.ep), [tests/security/test_group_sessions.ep](../../tests/security/test_group_sessions.ep), [tests/security/test_group_speakers.ep](../../tests/security/test_group_speakers.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_groups.ep](../../tests/security/test_groups.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_host_scope.ep](../../tests/security/test_host_scope.ep), [tests/security/test_host_text.ep](../../tests/security/test_host_text.ep), [tests/security/test_hud_machine.ep](../../tests/security/test_hud_machine.ep), [tests/security/test_hud_rights.ep](../../tests/security/test_hud_rights.ep), [tests/security/test_hud_state.ep](../../tests/security/test_hud_state.ep), [tests/security/test_knowledge.ep](../../tests/security/test_knowledge.ep), [tests/security/test_knowledge_lifecycle.ep](../../tests/security/test_knowledge_lifecycle.ep), [tests/security/test_knowledge_tools.ep](../../tests/security/test_knowledge_tools.ep), [tests/security/test_live_automatic_memory.ep](../../tests/security/test_live_automatic_memory.ep), [tests/security/test_live_group_conversation.ep](../../tests/security/test_live_group_conversation.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/security/test_live_group_workspace.ep](../../tests/security/test_live_group_workspace.ep), [tests/security/test_live_memory_cycle.ep](../../tests/security/test_live_memory_cycle.ep), [tests/security/test_live_memory_followup.ep](../../tests/security/test_live_memory_followup.ep), [tests/security/test_live_protected_deployment.ep](../../tests/security/test_live_protected_deployment.ep), [tests/security/test_live_protected_tools.ep](../../tests/security/test_live_protected_tools.ep), [tests/security/test_live_silo_scratchpad.ep](../../tests/security/test_live_silo_scratchpad.ep), [tests/security/test_live_uninspected_refusal.ep](../../tests/security/test_live_uninspected_refusal.ep), [tests/security/test_memory_run_snapshot.ep](../../tests/security/test_memory_run_snapshot.ep), [tests/security/test_memory_status_prefix.ep](../../tests/security/test_memory_status_prefix.ep), [tests/security/test_memory_tools.ep](../../tests/security/test_memory_tools.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/security/test_parallel_memory.ep](../../tests/security/test_parallel_memory.ep), [tests/security/test_principals.ep](../../tests/security/test_principals.ep), [tests/security/test_private_recall.ep](../../tests/security/test_private_recall.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/security/test_protected_proposals.ep](../../tests/security/test_protected_proposals.ep), [tests/security/test_protected_quarantine.ep](../../tests/security/test_protected_quarantine.ep), [tests/security/test_protected_tools.ep](../../tests/security/test_protected_tools.ep), [tests/security/test_revocation.ep](../../tests/security/test_revocation.ep), [tests/security/test_run_authority.ep](../../tests/security/test_run_authority.ep), [tests/security/test_selected_recall.ep](../../tests/security/test_selected_recall.ep), [tests/security/test_session_principals.ep](../../tests/security/test_session_principals.ep), [tests/security/test_silo_scratchpad.ep](../../tests/security/test_silo_scratchpad.ep), [tests/security/test_stable_session_recall.ep](../../tests/security/test_stable_session_recall.ep), [tests/security/test_system_silo.ep](../../tests/security/test_system_silo.ep), [tests/security/test_tool_policy.ep](../../tests/security/test_tool_policy.ep), [tests/security/test_trace_dispatch.ep](../../tests/security/test_trace_dispatch.ep), [tests/security/test_trace_scope.ep](../../tests/security/test_trace_scope.ep), [tests/security/test_uninspected_refusal.ep](../../tests/security/test_uninspected_refusal.ep), [tests/tools/test_build_modes.ep](../../tests/tools/test_build_modes.ep), [tests/tools/test_candidate_build_loop.ep](../../tests/tools/test_candidate_build_loop.ep), [tests/tools/test_git_workflow.ep](../../tests/tools/test_git_workflow.ep), [tests/tools/test_group_plot_workbook.ep](../../tests/tools/test_group_plot_workbook.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_database.ep](../../tests/tools/test_live_database.ep), [tests/tools/test_live_ernos_program.ep](../../tests/tools/test_live_ernos_program.ep), [tests/tools/test_live_git.ep](../../tests/tools/test_live_git.ep), [tests/tools/test_live_plot.ep](../../tests/tools/test_live_plot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_tool_discovery.ep](../../tests/tools/test_live_tool_discovery.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_create.ep](../../tests/tools/test_live_workbook_create.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/tools/test_tool_discovery.ep](../../tests/tools/test_tool_discovery.ep), [tests/tools/test_tool_registry.ep](../../tests/tools/test_tool_registry.ep), [tests/tools/test_workspace_database.ep](../../tests/tools/test_workspace_database.ep), [tests/web/group_speech_seed.ep](../../tests/web/group_speech_seed.ep), [tests/web/history_browser_seed.ep](../../tests/web/history_browser_seed.ep), [tests/web/interim_history_seed.ep](../../tests/web/interim_history_seed.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/provision_crash_probe.ep](../../tests/web/provision_crash_probe.ep), [tests/web/seed_device_routes.ep](../../tests/web/seed_device_routes.ep), [tests/web/seed_rendering.ep](../../tests/web/seed_rendering.ep), [tests/web/seed_retry_browser.ep](../../tests/web/seed_retry_browser.ep), [tests/web/share_controls_seed.ep](../../tests/web/share_controls_seed.ep), [tests/web/test_activity.ep](../../tests/web/test_activity.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_attachment_lifecycle.ep](../../tests/web/test_attachment_lifecycle.ep), [tests/web/test_attachment_text.ep](../../tests/web/test_attachment_text.ep), [tests/web/test_attachment_tools.ep](../../tests/web/test_attachment_tools.ep), [tests/web/test_attachment_usage.ep](../../tests/web/test_attachment_usage.ep), [tests/web/test_attachment_visibility.ep](../../tests/web/test_attachment_visibility.ep), [tests/web/test_attachment_visibility_readers.ep](../../tests/web/test_attachment_visibility_readers.ep), [tests/web/test_attachments.ep](../../tests/web/test_attachments.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_branches.ep](../../tests/web/test_branches.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancel_queue.ep](../../tests/web/test_cancel_queue.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_conversation_merge.ep](../../tests/web/test_conversation_merge.ep), [tests/web/test_device_links.ep](../../tests/web/test_device_links.ep), [tests/web/test_devices.ep](../../tests/web/test_devices.ep), [tests/web/test_empty_session.ep](../../tests/web/test_empty_session.ep), [tests/web/test_export.ep](../../tests/web/test_export.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_group_controls.ep](../../tests/web/test_group_controls.ep), [tests/web/test_group_export.ep](../../tests/web/test_group_export.ep), [tests/web/test_history_pages.ep](../../tests/web/test_history_pages.ep), [tests/web/test_http_api.ep](../../tests/web/test_http_api.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_late_parent_reply.ep](../../tests/web/test_late_parent_reply.ep), [tests/web/test_live_memory_server.ep](../../tests/web/test_live_memory_server.ep), [tests/web/test_long_message_archive.ep](../../tests/web/test_long_message_archive.ep), [tests/web/test_midturn_guidance.ep](../../tests/web/test_midturn_guidance.ep), [tests/web/test_pdf_attachment.ep](../../tests/web/test_pdf_attachment.ep), [tests/web/test_progress.ep](../../tests/web/test_progress.ep), [tests/web/test_projects.ep](../../tests/web/test_projects.ep), [tests/web/test_provider_submission.ep](../../tests/web/test_provider_submission.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_run_store.ep](../../tests/web/test_run_store.ep), [tests/web/test_session_rename.ep](../../tests/web/test_session_rename.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_trace.ep](../../tests/web/test_trace.ep), [tests/web/test_turn_attachments.ep](../../tests/web/test_turn_attachments.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/web/test_uploads.ep](../../tests/web/test_uploads.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/web/shared_access.ep`

[app/web/shared_access.ep](../../app/web/shared_access.ep) · 24 lines · 1536 bytes

SHA-256: `fe310fdfa95246b7e762b3dd5f916aec79c0c7ea57442eace27bcfa0e41d8fc5`

Source annotations: Private ownership stays unchanged. Shared observation uses membership in the recorded audience; controlling a run additionally requires its exact sender.

Imports: [attachments](../../app/web/attachments.ep).

Definitions:

```text
5: define web_shared_run_read_allowed with database as Int and device as Str and run as Str returning Int:
17: define web_shared_run_control_allowed with database as Int and device as Str and run as Str returning Int:
```

## `app/web/source_reads.ep`

[app/web/source_reads.ep](../../app/web/source_reads.ep) · 102 lines · 7861 bytes

SHA-256: `c3be58fc98ecef39d4fb7a6129f9ace1e61d5d19a250a5cc19179ce56958da0c`

Source annotations: These are recorded reads, not an assertion that every answer claim is supported. Only successful observed attachment reads with matching provenance are exposed; model-written citation strings never create these references.

Imports: [../retained_text](../../app/retained_text.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
7: define web_run_source_reads with database as Int and session as Str and run as Str returning Str:
54: define web_message_audio with database as Int and session as Str and run as Str and source as Str returning Str:
81: define web_run_research_sources with database as Int and session as Str and run as Str returning Str:
```

Lexical test references: [tests/agent/test_pdf_loop.ep](../../tests/agent/test_pdf_loop.ep), [tests/media/test_message_speech.ep](../../tests/media/test_message_speech.ep), [tests/media/test_video_tool.ep](../../tests/media/test_video_tool.ep), [tests/media/test_voice_profile_switch.ep](../../tests/media/test_voice_profile_switch.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_group_attach_file.ep](../../tests/security/test_group_attach_file.ep), [tests/security/test_live_group_publication.ep](../../tests/security/test_live_group_publication.ep), [tests/web/test_research_sources.ep](../../tests/web/test_research_sources.ep).

## `app/web/trace.ep`

[app/web/trace.ep](../../app/web/trace.ep) · 94 lines · 6260 bytes

SHA-256: `402d84dde893fae2494da1daa8c64a523ed481e9d6a072e91c6466c75c90c66a`

Source annotations: The route authenticates device ownership before calling this reader. Run and session are joined here too; provider records must belong to this run's request. Paging is chosen by the caller. Original records are never changed or clipped.

Imports: [../retained_text](../../app/retained_text.ep), [progress](../../app/web/progress.ep).

Definitions:

```text
7: define web_run_trace with database as Int and run as Str and after as Int and events as Int returning Str:
```

Lexical test references: [tests/web/test_trace.ep](../../tests/web/test_trace.ep).

## `app/web/upload_body.ep`

[app/web/upload_body.ep](../../app/web/upload_body.ep) · 37 lines · 2204 bytes

SHA-256: `bc4626ef249176c7e1fa94dadaccd9e8efe13431485a10ed6ad6934a495a2f30`

Source annotations: Call only after device/session authorization. This consumes framed binary bytes directly; it never buffers the complete file or converts it to text.

Imports: [../retained_text](../../app/retained_text.ep), [http_api](../../app/web/http_api.ep), [../archive/file_stage](../../app/archive/file_stage.ep).

Definitions:

```text
7: define web_upload_stage with reader as List and request as List and path as Str and batch as Int returning List:
```

Lexical test references: [tests/web/test_upload_body.ep](../../tests/web/test_upload_body.ep).

## `app/web/uploads.ep`

[app/web/uploads.ep](../../app/web/uploads.ep) · 77 lines · 5575 bytes

SHA-256: `2328b0f77316ea91b22deb068c5a6a1fab0bb1f1069b52c8cabe3e72d113c0c3`

Source annotations: Root is host configuration. The opaque storage name is always host-generated.

Imports: [../retained_text](../../app/retained_text.ep), [attachments](../../app/web/attachments.ep), [upload_body](../../app/web/upload_body.ep).

Definitions:

```text
5: define web_upload_schema with database as Int returning Int:
11: define web_upload_prepare with database as Int and device as Str and session as Str and root as Str and name as Str and media_type as Str returning Str:
32: define web_upload_owned with database as Int and device as Str and upload as Str returning List:
41: define web_upload_claim with database as Int and device as Str and upload as Str returning Int:
54: define web_upload_fail with database as Int and device as Str and upload as Str returning Int:
63: define web_upload_register with database as Int and device as Str and upload as Str returning Int:
76: define web_upload_recover with database as Int returning Int:
```

Lexical test references: [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/archive/test_restored_files.ep](../../tests/archive/test_restored_files.ep), [tests/security/test_group_attachments.ep](../../tests/security/test_group_attachments.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/web/test_upload_disconnect.ep](../../tests/web/test_upload_disconnect.ep), [tests/web/test_uploads.ep](../../tests/web/test_uploads.ep).

## `app/web/watch_schedule.ep`

[app/web/watch_schedule.ep](../../app/web/watch_schedule.ep) · 73 lines · 8565 bytes

SHA-256: `57d281fd581dd184142464c6f6af58992760721bbb100837bd8e873e71d36fa8`

Source annotations: Called only by the authenticated playback page. It supplies no model text, file path or range: the server derives all three from durable playback state.

Imports: [../interaction/watch_party](../../app/interaction/watch_party.ep), [worker](../../app/web/worker.ep).

Definitions:

```text
6: define web_watch_schedule with database as Int and device as Str and session as Str returning List:
```

Lexical test references: [tests/media/test_watch_party.ep](../../tests/media/test_watch_party.ep).

## `app/web/work_status.ep`

[app/web/work_status.ep](../../app/web/work_status.ep) · 91 lines · 6763 bytes

SHA-256: `f211e5c1f2eb50a783b1af2f01d52a6d6bf3fa8f7855bdb4601f64caf4fd9cc9`

Source annotations: Human read-only work pagination uses current device/session authority. Each delegated target is checked separately; parent membership cannot disclose a child whose audience is no longer visible. Cursor size is not a history cap.

Imports: [projects](../../app/web/projects.ep), [shared_access](../../app/web/shared_access.ep), [activity](../../app/web/activity.ep).

Definitions:

```text
8: define web_work_status_snapshot with database as Int and device as Str and session as Str and family as Str and after as Int returning Str:
47: define web_work_status with database as Int and device as Str and session as Str and family as Str and after as Int returning Str:
58: define web_work_page with database as Int and device as Str and tokens as List returning Str:
```

Lexical test references: [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_work_lineage.ep](../../tests/web/test_work_lineage.ep), [tests/web/test_work_status.ep](../../tests/web/test_work_status.ep).

## `app/web/worker.ep`

[app/web/worker.ep](../../app/web/worker.ep) · 224 lines · 15341 bytes

SHA-256: `b3ff8799df6ad68eca8434cc702e1f69185be520f8495fb4061d4a073fa47e77`

Source annotations: An explicit worker claim survives process death. It is never silently reused; restart recovery must establish that the previous worker is no longer alive.

Imports: [interaction](../../app/web/interaction.ep), [../feedback](../../app/feedback.ep), [shared_access](../../app/web/shared_access.ep), [../transport/isolated_process](../../app/transport/isolated_process.ep), [../retained_text](../../app/retained_text.ep), [cancellation](../../app/web/cancellation.ep), [activity](../../app/web/activity.ep).

Definitions:

```text
9: define web_worker_schema with database as Int returning Int:
14: define web_worker_protocol with database as Int and run as Str and worker as Str returning Int:
22: define web_worker_claim with database as Int and run as Str and worker as Str returning Int:
35: define web_worker_ensure_claim with database as Int and device as Str and run as Str and worker as Str returning Int:
56: define web_worker_retry_claim with database as Int and device as Str and run as Str and previous as Str and worker as Str returning Int:
79: define web_worker_run with database_path as Str and run as Str and worker as Str and model as Str and port as Int and reserve as Int and batch as Int returning Int:
82: define web_worker_run_provider with database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:
109: define web_worker_run_owned with database_path as Str and run as Str and worker as Str and model as Str and port as Int and reserve as Int and batch as Int returning Int:
112: define web_worker_run_owned_provider with database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:
162: define web_worker_status with database as Int and run as Str returning Str:
198: define web_worker_cancel_idle with database as Int and run as Str returning Int:
217: define web_worker_dispatch with database as Int and database_path as Str and run as Str and worker as Str and profile as Str and reserve as Int and batch as Int returning Int:
```

Lexical test references: [tests/agent/test_generation_recovery.ep](../../tests/agent/test_generation_recovery.ep), [tests/agent/test_legacy_protocol_recovery.ep](../../tests/agent/test_legacy_protocol_recovery.ep), [tests/jobs/test_delegation_control.ep](../../tests/jobs/test_delegation_control.ep), [tests/jobs/test_delegation_cross_turn.ep](../../tests/jobs/test_delegation_cross_turn.ep), [tests/jobs/test_delegation_monitor.ep](../../tests/jobs/test_delegation_monitor.ep), [tests/jobs/test_delegation_prefix.ep](../../tests/jobs/test_delegation_prefix.ep), [tests/jobs/test_delegation_run.ep](../../tests/jobs/test_delegation_run.ep), [tests/jobs/test_live_delegation_message.ep](../../tests/jobs/test_live_delegation_message.ep), [tests/jobs/test_live_delegation_tool.ep](../../tests/jobs/test_live_delegation_tool.ep), [tests/jobs/test_live_schedule_tool.ep](../../tests/jobs/test_live_schedule_tool.ep), [tests/jobs/test_schedule_run.ep](../../tests/jobs/test_schedule_run.ep), [tests/jobs/test_schedule_service.ep](../../tests/jobs/test_schedule_service.ep), [tests/jobs/test_schedule_tool.ep](../../tests/jobs/test_schedule_tool.ep), [tests/network/test_live_web_fetch.ep](../../tests/network/test_live_web_fetch.ep), [tests/network/test_live_web_research.ep](../../tests/network/test_live_web_research.ep), [tests/network/test_web_fetch.ep](../../tests/network/test_web_fetch.ep), [tests/network/test_web_pdf.ep](../../tests/network/test_web_pdf.ep), [tests/network/test_web_research.ep](../../tests/network/test_web_research.ep), [tests/security/test_message_environment.ep](../../tests/security/test_message_environment.ep), [tests/tools/test_job_control.ep](../../tests/tools/test_job_control.ep), [tests/web/prepare_compaction_preview.ep](../../tests/web/prepare_compaction_preview.ep), [tests/web/seed_retry_browser.ep](../../tests/web/seed_retry_browser.ep), [tests/web/serve_recovery_fixture.ep](../../tests/web/serve_recovery_fixture.ep), [tests/web/test_agent_compaction_status.ep](../../tests/web/test_agent_compaction_status.ep), [tests/web/test_backup.ep](../../tests/web/test_backup.ep), [tests/web/test_cancel_busy.ep](../../tests/web/test_cancel_busy.ep), [tests/web/test_cancellation.ep](../../tests/web/test_cancellation.ep), [tests/web/test_group_browser.ep](../../tests/web/test_group_browser.ep), [tests/web/test_idle_cancel.ep](../../tests/web/test_idle_cancel.ep), [tests/web/test_quarantine_monitor.ep](../../tests/web/test_quarantine_monitor.ep), [tests/web/test_retry.ep](../../tests/web/test_retry.ep), [tests/web/test_revisions.ep](../../tests/web/test_revisions.ep), [tests/web/test_submission_route_recovery.ep](../../tests/web/test_submission_route_recovery.ep), [tests/web/test_worker_recovery.ep](../../tests/web/test_worker_recovery.ep).

## `app/workspace/archive_patch.ep`

[app/workspace/archive_patch.ep](../../app/workspace/archive_patch.ep) · 158 lines · 9321 bytes

SHA-256: `c917c94ddc389408f57f2c1f894f2b3a05e9f7ea4772e2b417cebffcc384608a`

Source annotations: Repack a ZIP while replacing explicitly named existing parts. Other entry payloads stream as binary. The source remains untouched and is rehashed after copying. Caller supplies source, expected SHA256, then name/text pairs.

Imports: [files](../../app/workspace/files.ep), [../providers/local_json](../../app/providers/local_json.ep), [../archive/file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
8: define workspace_archive_patch with system as Int and library as Int and writer as Int and root as Int and patch as List and buffer as Int returning Int:
115: define workspace_zip_original_name with system as Int and library as Int and reader as Int and input as Int returning Str:
```

Lexical test references: [tests/workspace/test_zip_names.ep](../../tests/workspace/test_zip_names.ep).

## `app/workspace/build_arguments.ep`

[app/workspace/build_arguments.ep](../../app/workspace/build_arguments.ep) · 24 lines · 1237 bytes

SHA-256: `d456e5e8e5cdde3c8e4c1e859f7d6b2ad5b1f6e4377433d89624f36e45a484fd`

Source annotations: Convert the exact archived typed request to compiler argv, never shell text.

Imports: [../retained_text](../../app/retained_text.ep).

Definitions:

```text
4: define workspace_build_arguments with arguments as List returning List:
```

Lexical test references: [tests/workspace/test_build_arguments.ep](../../tests/workspace/test_build_arguments.ep).

## `app/workspace/capture_process.ep`

[app/workspace/capture_process.ep](../../app/workspace/capture_process.ep) · 92 lines · 4380 bytes

SHA-256: `be4838f9944aa37adb31bf73f593be53b5b210c9278ce0d2fb73bfd8346abe05`

Source annotations: Takes ownership of the two output readers, not the cancellation reader or process handle. Caller retains the handle until it is reaped and closed. Result: status, raw wait status, persisted stdout bytes, persisted stderr bytes. Persistence failure terminates the group immediately and never claims complete output. Cancellation terminates the group then archives bytes still in pipes.

Imports: [../retained_text](../../app/retained_text.ep), [native_process](../../app/workspace/native_process.ep), [process_output](../../app/workspace/process_output.ep), [output_archive](../../app/workspace/output_archive.ep), [../agent/control](../../app/agent/control.ep).

Definitions:

```text
12: define workspace_process_capture with database as Int and run as Str and call_id as Str and handle as List and output as Int and errors as Int and cancellation as Int returning List:
```

Lexical test references: [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep).

## `app/workspace/create_archive.ep`

[app/workspace/create_archive.ep](../../app/workspace/create_archive.ep) · 183 lines · 11434 bytes

SHA-256: `29696799a812236e8db7a647a5a38cd653743c0d0845e289dc6859c72820a740`

Source annotations: Stream only explicitly selected regular workspace files. No recursive host traversal, stored links, permissions, external filters or shell commands.

Imports: [extract_archive](../../app/workspace/extract_archive.ep), [archive_patch](../../app/workspace/archive_patch.ep).

Definitions:

```text
6: define workspace_archive_write_file with system as Int and library as Int and writer as Int and root as Int and path as Str and buffer as Int returning Int:
50: define workspace_archive_write with system as Int and output as Int and root as Int and paths as List and format as Str returning Int:
101: define workspace_create_archive with system as Int and root as Int and paths as List and destination as Str and format as Str returning Str:
114: define workspace_archive_commit with system as Int and root as Int and paths as List and destination as Str and format as Str returning Str:
167: define workspace_archive_write_text with library as Int and writer as Int and path as Str and text as Str returning Int:
```

Lexical test references: [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep).

## `app/workspace/create_text.ep`

[app/workspace/create_text.ep](../../app/workspace/create_text.ep) · 61 lines · 3181 bytes

SHA-256: `a79dc2792e2628f6c5c68bce48e4eb778613955fe27ff5cdc6039dbeec8b5c52`

Source annotations: Publish a fully flushed new file with Darwin RENAME_EXCL. Parent directories must exist. Existing destinations are never replaced. Temporary files may survive process failure and need recovery; they are not completion receipts.

Imports: [../retained_text](../../app/retained_text.ep), [directories](../../app/workspace/directories.ep).

Definitions:

```text
4: external define ep_uuid_v4
9: define workspace_create_text with library as Int and root as Int and path as Str and text as Str returning Str:
```

Lexical test references: [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/workspace/test_create_text.ep](../../tests/workspace/test_create_text.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_publication_capability.ep](../../tests/workspace/test_publication_capability.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/workspace/database_arguments.ep`

[app/workspace/database_arguments.ep](../../app/workspace/database_arguments.ep) · 17 lines · 982 bytes

SHA-256: `d3d23f5da624380d61df8c676e020fcde78fd62b9d1a137282ff3452c56be3c0`

Imports: [../retained_text](../../app/retained_text.ep), [files](../../app/workspace/files.ep).

Definitions:

```text
4: define workspace_database_arguments with arguments as List returning List:
```

## `app/workspace/database_engine.ep`

[app/workspace/database_engine.ep](../../app/workspace/database_engine.ep) · 110 lines · 6470 bytes

SHA-256: `d06386ff6262f6dcf79ee7d0ff1ef72c2ecdf87b5ee24a2ff79ea594f672eaa5`

Source annotations: SQLite values retain their original storage class. Text is readable UTF-8; binary text and blobs use exact hex rather than truncating at an embedded NUL.

Imports: [../providers/local_json](../../app/providers/local_json.ep), [files](../../app/workspace/files.ep), `os` (toolchain/library).

Definitions:

```text
7: define workspace_database_hex with pointer as Int and count as Int returning Str:
17: define workspace_database_cell with library as Int and statement as Int and column as Int returning Str:
43: define workspace_database_error with library as Int and database as Int and stage as Str and code as Int returning Int:
52: define workspace_database_script with library as Int and database as Int and script as Str returning Int:
```

## `app/workspace/diff_text.ep`

[app/workspace/diff_text.ep](../../app/workspace/diff_text.ep) · 45 lines · 2798 bytes

SHA-256: `f1dcb7c9cb3e14466b5410613732a8117d2076e3623e989cf27851c94b675bfe`

Source annotations: One complete contiguous line-aligned change span, not a minimal edit script. Equal prefix/suffix lines are omitted without hiding intervening differences.

Imports: [read_text](../../app/workspace/read_text.ep).

Definitions:

```text
5: define workspace_diff_text with library as Int and root as Int and before_path as Str and after_path as Str returning Str:
```

Lexical test references: [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/workspace/directories.ep`

[app/workspace/directories.ep](../../app/workspace/directories.ep) · 43 lines · 2111 bytes

SHA-256: `590e61c87a2befd7d9b6d25a8e9d652cd9e7e20b66e7d33f228975d61de33fc8`

Source annotations: Open a relative directory beneath the session descriptor. When requested, create missing components with owner-only permissions and flush each parent. Existing components must also be directories and must never be symlinks. A failure may leave earlier empty directories; a retry can reopen them.

Imports: [../retained_text](../../app/retained_text.ep), [files](../../app/workspace/files.ep).

Definitions:

```text
8: define workspace_directory_open with library as Int and root as Int and path as Str and create_missing as Int returning Int:
```

Lexical test references: [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/workspace/test_create_text.ep](../../tests/workspace/test_create_text.ep), [tests/workspace/test_directories.ep](../../tests/workspace/test_directories.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/workspace/edit_text.ep`

[app/workspace/edit_text.ep](../../app/workspace/edit_text.ep) · 64 lines · 4637 bytes

SHA-256: `96a8ba2714ffe9add3069e74251d4c6c54f574e69d0d375077e1d88e61dcf50b`

Source annotations: Preserve the displaced inode at a unique sibling path using atomic swap. The digest check is optimistic: uncooperative processes can edit concurrently. Never silently delete displaced data or claim an atomic compare-and-swap.

Imports: [create_text](../../app/workspace/create_text.ep), [read_text](../../app/workspace/read_text.ep).

Definitions:

```text
7: define workspace_edit_text with library as Int and root as Int and path as Str and expected as Str and text as Str returning Str:
```

Lexical test references: [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/workspace/ernos_runtime.ep`

[app/workspace/ernos_runtime.ep](../../app/workspace/ernos_runtime.ep) · 93 lines · 6322 bytes

SHA-256: `976075dc394a8657250c423d006353a2bdcf037987d527182c6c64f1ac9976c1`

Source annotations: The compiler resolves bare standard-library imports from its working directory. Install only a link to the host-selected runtime; never replace a user's file/directory or accept a link to some other location.

Imports: [files](../../app/workspace/files.ep).

Environment names: `ECHO_ERNOS_COMPILER_SHA256`.

Definitions:

```text
6: define workspace_ernos_prepare with library as Int and directory as Int and runtime as Str returning Str:
55: define workspace_ernos_arguments with library as Int and directory as Int and arguments as List returning Str:
82: define workspace_ernos_recovery with status as Str returning Str:
```

Lexical test references: [tests/workspace/test_ernos_runtime.ep](../../tests/workspace/test_ernos_runtime.ep).

## `app/workspace/execute_process.ep`

[app/workspace/execute_process.ep](../../app/workspace/execute_process.ep) · 126 lines · 7697 bytes

SHA-256: `ae2c6889e0a9ba73a63dbddd7dbc1e2e0583c0a5f4cc63b643c0609c8817d4d3`

Source annotations: Result: status, retained handle (only when cleanup needs attention), raw wait status, persisted stdout bytes, persisted stderr bytes. The caller must retain and reconcile a nonzero handle; never convert cleanup-failed into success.

Imports: [../retained_text](../../app/retained_text.ep), [sandbox_process](../../app/workspace/sandbox_process.ep), [capture_process](../../app/workspace/capture_process.ep), [process_journal](../../app/workspace/process_journal.ep).

Definitions:

```text
9: define workspace_execution_result with status as Str and handle as List and waited as Int and output_bytes as Int and error_bytes as Int returning List:
18: define workspace_process_abort with handle as List returning Int:
34: define workspace_execute_claimed with database as Int and run as Str and call_id as Str and runtime as Str and workspace as Str and directory as Int and executable as Str and arguments as List and environment as List and cancellation as Int returning List:
```

Lexical test references: [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep).

## `app/workspace/execution_request.ep`

[app/workspace/execution_request.ep](../../app/workspace/execution_request.ep) · 101 lines · 7879 bytes

SHA-256: `3c3315837b25af3218d6008f2778379dd11599952da478d9678ec2d26b0cd728`

Source annotations: Resolve an execution request from its original immutable tool event. No caller or model arguments can override session, device, workspace root or environment. Result: status, session, device, program, argv list. This grants no launch on its own: the dispatcher must pin the workspace and recheck owner at dispatch.

Imports: [git_arguments](../../app/workspace/git_arguments.ep), [database_arguments](../../app/workspace/database_arguments.ep), [build_arguments](../../app/workspace/build_arguments.ep), [../security/principals](../../app/security/principals.ep), [../retained_text](../../app/retained_text.ep), [sessions](../../app/workspace/sessions.ep), [../security/tool_policy](../../app/security/tool_policy.ep), [../agent/run_state](../../app/agent/run_state.ep).

Definitions:

```text
10: define workspace_execution_denied with status as Str returning List:
19: define workspace_execution_request with database as Int and run as Str and call_id as Str returning List:
```

Lexical test references: [tests/security/diagnose_execution_authority.ep](../../tests/security/diagnose_execution_authority.ep), [tests/workspace/test_execution_request.ep](../../tests/workspace/test_execution_request.ep).

## `app/workspace/extract_archive.ep`

[app/workspace/extract_archive.ep](../../app/workspace/extract_archive.ep) · 78 lines · 4863 bytes

SHA-256: `92f40c8284ba7dbd5a3cb336510a08dcef420013348b65f23414e78f08ec6630`

Source annotations: Stage binary bytes and publish with exclusive rename. Archive entry paths and permissions are never used as host destinations or executable permissions.

Imports: [create_text](../../app/workspace/create_text.ep), [../providers/local_json](../../app/providers/local_json.ep), [../archive/file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
7: define workspace_archive_publish with system as Int and library as Int and reader as Int and root as Int and destination as Str returning Str:
60: define workspace_archive_extract_entry with system as Int and library as Int and reader as Int and root as Int and selected as Int and destination as Str returning Str:
```

## `app/workspace/files.ep`

[app/workspace/files.ep](../../app/workspace/files.ep) · 84 lines · 3786 bytes

SHA-256: `1436f748a62caf298795ed56f4dc030694a4ab4abd67fc842c6cd94adf024c22`

Source annotations: The host supplies the workspace root. Model arguments supply only a relative path below an already opened directory descriptor. Darwin constants below are ABI flags, not file-size or workspace-depth limits. O_RDONLY | O_DIRECTORY | O_NOFOLLOW_ANY | O_CLOEXEC. Regular single-link files only. A FIFO is opened nonblocking and rejected; directories, devices and hard links do not become file tool inputs.

Imports: [../retained_text](../../app/retained_text.ep), [../archive/file_stage](../../app/archive/file_stage.ep), [../transport/native_open](../../app/transport/native_open.ep), [../archive/file_text](../../app/archive/file_text.ep).

Definitions:

```text
9: define workspace_root_open with library as Int and path as Str returning Int:
16: define workspace_path_parts with path as Str returning List:
30: define workspace_regular_file with library as Int and descriptor as Int returning Int:
52: define workspace_file_open with library as Int and root as Int and path as Str and create_new as Int returning Int:
```

Lexical test references: [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/archive/test_tree_snapshot.ep](../../tests/archive/test_tree_snapshot.ep), [tests/lifecycle/test_controller_credential.ep](../../tests/lifecycle/test_controller_credential.ep), [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep), [tests/lifecycle/test_service.ep](../../tests/lifecycle/test_service.ep), [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/network/test_fetch_cancel.ep](../../tests/network/test_fetch_cancel.ep), [tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep), [tests/security/test_protected_exchange.ep](../../tests/security/test_protected_exchange.ep), [tests/tools/test_tool_candidate_files.ep](../../tests/tools/test_tool_candidate_files.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_compile_process.ep](../../tests/workspace/test_compile_process.ep), [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep), [tests/workspace/test_create_text.ep](../../tests/workspace/test_create_text.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_directories.ep](../../tests/workspace/test_directories.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_ernos_runtime.ep](../../tests/workspace/test_ernos_runtime.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_files.ep](../../tests/workspace/test_files.ep), [tests/workspace/test_host_profile.ep](../../tests/workspace/test_host_profile.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_process.ep](../../tests/workspace/test_process.ep), [tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_process_tree.ep](../../tests/workspace/test_process_tree.ep), [tests/workspace/test_publication_capability.ep](../../tests/workspace/test_publication_capability.ep), [tests/workspace/test_read_text.ep](../../tests/workspace/test_read_text.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep), [tests/workspace/test_sandbox_process.ep](../../tests/workspace/test_sandbox_process.ep), [tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep), [tests/workspace/test_zip_names.ep](../../tests/workspace/test_zip_names.ep).

## `app/workspace/git_arguments.ep`

[app/workspace/git_arguments.ep](../../app/workspace/git_arguments.ep) · 34 lines · 1934 bytes

SHA-256: `fdd7c023d677cf9697269941504601cec54c9856694e940c2287e4a89bac9dd0`

Source annotations: Repository and subcommand are separate argv elements, never shell text.

Imports: [files](../../app/workspace/files.ep), [../retained_text](../../app/retained_text.ep).

Definitions:

```text
5: define host_git_arguments with arguments as List returning List:
```

## `app/workspace/guardian_receipt.ep`

[app/workspace/guardian_receipt.ep](../../app/workspace/guardian_receipt.ep) · 28 lines · 2543 bytes

SHA-256: `33cbd6cbbcb2b78812161481f7d4a29160a644c907d98fd23d46e07e6a4f2765`

Source annotations: The event namespace binds the guardian's original return value to the exact queued call. Recovery verifies the archive before trusting this receipt.

Imports: [execution_request](../../app/workspace/execution_request.ep).

Definitions:

```text
5: define workspace_guardian_receipt with database as Int and run as Str and call_id as Str returning Str:
19: define workspace_guardian_save with database as Int and run as Str and call_id as Str and result as Str returning Int:
```

Lexical test references: [tests/agent/recover_guardian_fixture.ep](../../tests/agent/recover_guardian_fixture.ep).

## `app/workspace/host_profile.ep`

[app/workspace/host_profile.ep](../../app/workspace/host_profile.ep) · 58 lines · 3048 bytes

SHA-256: `a8deab7d4c39d15694ec2ff62cca8c55af2d46e66c8dd4ddadd88ebaa1594f8e`

Source annotations: Both host paths consume the same validated, nonempty root lists. Missing or malformed configuration is not an empty deny list. Paths must be absolute, canonical component spellings; reject embedded NUL before any native API. These paths come only from host configuration, never tool arguments. Ordinary admin commands cannot write accepted Echo code/state or read connector secrets.

Imports: [../providers/archived_stream](../../app/providers/archived_stream.ep), [files](../../app/workspace/files.ep).

Environment names: `ECHO_HOST_PROTECTED_ROOTS`, `ECHO_HOST_SECRET_ROOTS`.

Definitions:

```text
7: define host_policy_roots with configured as Str returning List:
32: define host_execution_profile returning Str:
51: define host_secret_path with path as Str returning Int:
```

Lexical test references: [tests/workspace/test_host_profile.ep](../../tests/workspace/test_host_profile.ep).

## `app/workspace/list_archive.ep`

[app/workspace/list_archive.ep](../../app/workspace/list_archive.ep) · 93 lines · 6692 bytes

SHA-256: `8db42dc8fa2335bbe30882221d2cf73dae0138d42ea0c1454ea01f31fd69da60`

Source annotations: The reader only inspects entries. It never invokes a disk extractor, follows entry links, or executes archive content. Duplicate names retain their index.

Imports: [files](../../app/workspace/files.ep), [extract_archive](../../app/workspace/extract_archive.ep), [../providers/local_json](../../app/providers/local_json.ep), [../archive/file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
8: define workspace_archive_entries with library as Int and reader as Int and offset as Int and entries as Int returning Str:
47: define workspace_list_archive with system as Int and root as Int and path as Str and offset as Int and entries as Int returning Str:
50: define workspace_archive_operation with system as Int and root as Int and path as Str and offset as Int and entries as Int and destination as Str and expected as Str returning Str:
```

Lexical test references: [tests/workspace/test_create_archive.ep](../../tests/workspace/test_create_archive.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep).

## `app/workspace/list_directory.ep`

[app/workspace/list_directory.ep](../../app/workspace/list_directory.ep) · 72 lines · 3695 bytes

SHA-256: `e30c61ecbf84a5eaf4f7f6f6c910063c194acb7332ce2b9256380a66bbd7957d`

Source annotations: Enumerate only an already scoped directory. Pages reflect observed directory order, not an immutable snapshot; callers restart at zero after modifications.

Imports: [directories](../../app/workspace/directories.ep), [../providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
6: define workspace_list_directory with library as Int and root as Int and path as Str and offset as Int and entries as Int returning Str:
```

Lexical test references: [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep).

## `app/workspace/native_process.ep`

[app/workspace/native_process.ep](../../app/workspace/native_process.ep) · 141 lines · 7795 bytes

SHA-256: `2cb09774808f93521dc41819feecbfbc49fcad937b0858bfbbd2573dc9051cbd`

Source annotations: Host-only Darwin process primitive. Callers supply explicit argv/environment, already opened output descriptors, and a working-directory descriptor. This primitive does not grant a model permission to execute a program. The command tool must launch through the separately qualified sandbox. Stdout/stderr go directly to owned descriptors, without a string buffer cap. Handle fields: library, child PID, reaped flag, raw wait status. A trusted guardian may inherit only this private owner-liveness descriptor as fd 3. Ordinary commands keep the original no-extra-descriptor contract.

Imports: [../retained_text](../../app/retained_text.ep), [../transport/socket_read](../../app/transport/socket_read.ep).

Definitions:

```text
10: define workspace_process_start with executable as Str and arguments as List and environment as List and directory as Int and output as Int and errors as Int returning List:
15: define workspace_process_start_owned with executable as Str and arguments as List and environment as List and directory as Int and output as Int and errors as Int and owner as Int returning List:
103: define workspace_process_poll with handle as List returning Int:
127: define workspace_process_signal with handle as List and signal as Int returning Int:
136: define workspace_process_close with handle as List returning Int:
```

Lexical test references: [tests/lifecycle/test_controller_launch_privileged.ep](../../tests/lifecycle/test_controller_launch_privileged.ep), [tests/lifecycle/test_service.ep](../../tests/lifecycle/test_service.ep), [tests/providers/test_mlx_helper.ep](../../tests/providers/test_mlx_helper.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_compile_process.ep](../../tests/workspace/test_compile_process.ep), [tests/workspace/test_host_profile.ep](../../tests/workspace/test_host_profile.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_process.ep](../../tests/workspace/test_process.ep), [tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_process_tree.ep](../../tests/workspace/test_process_tree.ep), [tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep), [tests/workspace/test_sandbox_process.ep](../../tests/workspace/test_sandbox_process.ep).

## `app/workspace/output_archive.ep`

[app/workspace/output_archive.ep](../../app/workspace/output_archive.ep) · 60 lines · 4231 bytes

SHA-256: `9071c2107d022e0c2fc7f3602e69c004529cc1e8ea74509162a81a24fc40916e`

Source annotations: Each pipe chunk is an immutable event, with independently ordered stream offsets. Binary bytes use hex so NUL and invalid UTF-8 cannot alter the record. This is a host capture API, not a model-callable tool. Caller owns the pointer and must stop the process if recording fails, rather than silently drop output. Guard native integer overflow, not an output-size quota.

Imports: [../retained_text](../../app/retained_text.ep), [../agent/tool_ledger](../../app/agent/tool_ledger.ep), [../tools/history_tools](../../app/tools/history_tools.ep), [../providers/archived_stream](../../app/providers/archived_stream.ep).

Definitions:

```text
6: external define peek_byte with pointer and offset returning Int
7: external define poke_byte with pointer and offset and value returning Int
13: define workspace_output_record with database as Int and run as Str and call_id as Str and event as Str and stream as Str and pointer as Int and bytes as Int and ended as Int returning Int:
```

Lexical test references: [tests/media/test_image_logs.ep](../../tests/media/test_image_logs.ep), [tests/providers/test_quote.ep](../../tests/providers/test_quote.ep), [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_execute_claimed.ep](../../tests/workspace/test_execute_claimed.ep), [tests/workspace/test_network_isolation.ep](../../tests/workspace/test_network_isolation.ep), [tests/workspace/test_output_archive.ep](../../tests/workspace/test_output_archive.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep).

## `app/workspace/plot_numbers.ep`

[app/workspace/plot_numbers.ep](../../app/workspace/plot_numbers.ep) · 31 lines · 1182 bytes

SHA-256: `4cf208bd7f8c533af64290af2e1f1c60b31f33a138b67b536c274d03ebf3fb40`

Source annotations: libc parses the complete JSON numeric token into IEEE-754 binary64 storage. Ernos Float values use the same native bits; no integer coercion is applied.

Imports: [../providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
3: define plot_float_bits with bits as Int returning Float:
8: define plot_number with library as Int and text as Str returning Float:
26: define plot_finite with value as Float returning Int:
30: define plot_number_text with value as Float returning Str:
```

Lexical test references: [tests/workspace/test_plot_numbers.ep](../../tests/workspace/test_plot_numbers.ep).

## `app/workspace/plot_svg.ep`

[app/workspace/plot_svg.ep](../../app/workspace/plot_svg.ep) · 136 lines · 9387 bytes

SHA-256: `bfa881df0b7ff669690e115eed71bf540a8705fe84278163cb6422faece8b09d`

Source annotations: Text only: generated SVG contains no scripts, links or external resources.

Imports: [plot_numbers](../../app/workspace/plot_numbers.ep), [create_text](../../app/workspace/create_text.ep).

Definitions:

```text
5: define plot_xml with text as Str returning Str:
12: define plot_label_valid with text as Str returning Int:
21: define plot_keys_valid with tokens as List and item as Int and keys as List returning Int:
37: define plot_coordinate with value as Float and low as Float and high as Float and origin as Float and span as Float returning Float:
43: define workspace_plot_svg with library as Int and specification as Str returning List:
```

Lexical test references: [tests/workspace/test_plot_svg.ep](../../tests/workspace/test_plot_svg.ep).

## `app/workspace/process_guard.ep`

[app/workspace/process_guard.ep](../../app/workspace/process_guard.ep) · 27 lines · 1611 bytes

SHA-256: `e63f7f0626e1c2018c68e1176a11bb4e9d85bf66f1f91ead788d7500f1279ce6`

Source annotations: A dedicated native guardian owns the child handle and the read end of a private owner pipe. The owner alone keeps the writer. The command never inherits either end. Owner death therefore wakes this guardian without PID lookup, process-name matching or an execution lifetime limit. Monitoring cadence, never a deadline for the command.

Imports: [execute_process](../../app/workspace/execute_process.ep).

Definitions:

```text
7: define workspace_guard_owned with handle as List and owner as Int returning List:
```

Lexical test references: [tests/workspace/test_process_guard.ep](../../tests/workspace/test_process_guard.ep).

## `app/workspace/process_journal.ep`

[app/workspace/process_journal.ep](../../app/workspace/process_journal.ep) · 97 lines · 8615 bytes

SHA-256: `cf869f8241b54bd507463bc579e7c2971024cdfaf704e018e086d19852449cc7`

Source annotations: One launch reservation per durable tool call. Never infer that a reserved or started process did not execute after restart; such work needs reconciliation. Stored PIDs are observations, not authority to signal a possibly reused PID.

Imports: [../retained_text](../../app/retained_text.ep), [output_archive](../../app/workspace/output_archive.ep), [native_process](../../app/workspace/native_process.ep), [../agent/control](../../app/agent/control.ep).

Definitions:

```text
9: define workspace_process_schema with database as Int returning Int:
12: define workspace_process_reserve with database as Int and run as Str and call_id as Str and description as Str returning Int:
34: define workspace_process_receipt with database as Int and run as Str and call_id as Str and handle as List and capture_status as Str returning Int:
83: define workspace_process_spawn_failed with database as Int and run as Str and call_id as Str returning Int:
```

Lexical test references: [tests/archive/test_failed_run_bundle.ep](../../tests/archive/test_failed_run_bundle.ep), [tests/workspace/test_launch_interruption.ep](../../tests/workspace/test_launch_interruption.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep).

## `app/workspace/process_output.ep`

[app/workspace/process_output.ep](../../app/workspace/process_output.ep) · 76 lines · 4577 bytes

SHA-256: `f8c5151ef8ff24be03fbb687adaa446cf2807426424584ec069b079733beab74`

Source annotations: Result fields: status, stream (1 stdout, 2 stderr), owned bytes, byte count. Caller releases data with socket_release_buffer. EOF descriptors must be closed and replaced with -1 before the next call. A page is a transfer size, never a limit on output length. Binary bytes, including NUL, remain intact. Caller alternates preferred after data to avoid starving either pipe.

Imports: [../transport/socket_read](../../app/transport/socket_read.ep).

Definitions:

```text
8: define workspace_output_result with status as Str and stream as Int and pointer as Int and count as Int returning List:
16: define workspace_output_next with library as Int and output as Int and errors as Int and cancellation as Int and preferred as Int returning List:
```

Lexical test references: [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep).

## `app/workspace/read_json.ep`

[app/workspace/read_json.ep](../../app/workspace/read_json.ep) · 45 lines · 2451 bytes

SHA-256: `c40d95b1c987278a4c7f79f33f0ab6d6c333be08a7dd3d2cbc6b9c3ff3e2395b`

Source annotations: Select by explicit object keys or canonical array index strings. Keep the selected JSON spelling intact, including large numbers and escape sequences.

Imports: [read_text](../../app/workspace/read_text.ep), [../providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
6: define workspace_json_select with text as Str and segments as List returning Str:
41: define workspace_read_json with library as Int and root as Int and path as Str and segments as List returning Str:
```

Lexical test references: [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep).

## `app/workspace/read_table.ep`

[app/workspace/read_table.ep](../../app/workspace/read_table.ep) · 98 lines · 5547 bytes

SHA-256: `17cd7a0babe20b8173ee8b134d985633ebf231d5858515cb05b8c0cfac1deaba`

Source annotations: Parse delimited text without evaluating formulas or coercing cell values. Validate the entire source even when only a page of rows is returned.

Imports: [read_text](../../app/workspace/read_text.ep), [../providers/local_json](../../app/providers/local_json.ep).

Definitions:

```text
6: define workspace_table_parse with text as Str and delimiter as Str and offset as Int and rows as Int returning Str:
94: define workspace_read_table with library as Int and root as Int and path as Str and delimiter as Str and offset as Int and rows as Int returning Str:
```

Lexical test references: [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep).

## `app/workspace/read_text.ep`

[app/workspace/read_text.ep](../../app/workspace/read_text.ep) · 14 lines · 864 bytes

SHA-256: `7be2e45d3981b13fe41bab74816bf4f1e253f2e9321c9232b85c56e07d78d9d6`

Source annotations: The caller supplies an authorized session directory, never a host path from tool arguments. Read through the validated descriptor without reopening. The digest describes the bytes observed in this read, not an atomic snapshot against concurrent writes by another process.

Imports: [files](../../app/workspace/files.ep).

Definitions:

```text
7: define workspace_read_text with library as Int and root as Int and path as Str and batch as Int and offset as Int and characters as Int returning List:
```

Lexical test references: [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/workspace/test_create_text.ep](../../tests/workspace/test_create_text.ep), [tests/workspace/test_extract_archive.ep](../../tests/workspace/test_extract_archive.ep), [tests/workspace/test_publication_capability.ep](../../tests/workspace/test_publication_capability.ep), [tests/workspace/test_read_text.ep](../../tests/workspace/test_read_text.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/workspace/resource_profile.ep`

[app/workspace/resource_profile.ep](../../app/workspace/resource_profile.ep) · 45 lines · 2425 bytes

SHA-256: `97f3939a5f5c18c0f33018da3c49a261b0bf930a96ef6b08ad91db90d2b99c1e`

Source annotations: The trusted host/controller supplies resource sets, never tool arguments. This builder does not grant authority or select a conversation's resources. Empty read/write sets are valid; private/protected/secret roots are required. No network or blanket host filesystem/process grant is added.

Imports: [host_profile](../../app/workspace/host_profile.ep), [sandbox_profile](../../app/workspace/sandbox_profile.ep).

Environment names: `ECHO_HOST_PROTECTED_ROOTS`, `ECHO_HOST_SECRET_ROOTS`.

Definitions:

```text
8: define workspace_resource_profile with readable as Str and writable as Str and private_roots as Str returning Str:
```

Lexical test references: [tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep).

## `app/workspace/sandbox_process.ep`

[app/workspace/sandbox_process.ep](../../app/workspace/sandbox_process.ep) · 35 lines · 2401 bytes

SHA-256: `5091b9238ef0a639a75808b12bbb501cc1e8dff5bac782f77b4d361cba0f16c1`

Source annotations: Host-only launcher. The owning tool must establish device/session ownership, pin the workspace descriptor and supply trusted runtime/workspace roots. Arguments are separate native argv entries, never assembled into shell text. Pass the generated policy directly: a program cannot rewrite a profile file in its writable workspace and thereby alter a subsequent launch's policy.

Imports: [native_process](../../app/workspace/native_process.ep), [sandbox_profile](../../app/workspace/sandbox_profile.ep), [host_profile](../../app/workspace/host_profile.ep).

Environment names: `ECHO_ERNOS_RUNTIME`.

Definitions:

```text
10: define workspace_sandbox_start with runtime as Str and workspace as Str and executable as Str and command_arguments as List and environment as List and directory as Int and output as Int and errors as Int returning List:
13: define workspace_sandbox_start_profile with runtime as Str and workspace as Str and executable as Str and command_arguments as List and environment as List and directory as Int and output as Int and errors as Int and profile as Str returning List:
```

Lexical test references: [tests/workspace/test_capture_process.ep](../../tests/workspace/test_capture_process.ep), [tests/workspace/test_compile_process.ep](../../tests/workspace/test_compile_process.ep), [tests/workspace/test_host_profile.ep](../../tests/workspace/test_host_profile.ep), [tests/workspace/test_process_journal.ep](../../tests/workspace/test_process_journal.ep), [tests/workspace/test_process_output.ep](../../tests/workspace/test_process_output.ep), [tests/workspace/test_process_output_archive.ep](../../tests/workspace/test_process_output_archive.ep), [tests/workspace/test_resource_profile.ep](../../tests/workspace/test_resource_profile.ep).

## `app/workspace/sandbox_profile.ep`

[app/workspace/sandbox_profile.ep](../../app/workspace/sandbox_profile.ep) · 12 lines · 2033 bytes

SHA-256: `962e372879d76f6a484062cb8afa4587549003763abebcb44d2064bc47eae660`

Source annotations: Native macOS sandbox configuration, generated from English source. The host passes roots as sandbox-exec parameters, never interpolated profile syntax. Runtime roots are not data grants. Only the database worker, pinned compiler and its standard library are readable; neighbouring archives/configs are not. Only the digest-pinned compiler with validated compile/check/test-harness arguments receives directory-resolution access. Its output program is run separately under the ordinary profile; this grants no neighbouring file data.

Imports: none.

Definitions:

```text
5: define workspace_sandbox_profile returning Str:
11: define workspace_compiler_profile returning Str:
```

Lexical test references: [tests/workspace/test_sandbox.ep](../../tests/workspace/test_sandbox.ep).

## `app/workspace/search_text.ep`

[app/workspace/search_text.ep](../../app/workspace/search_text.ep) · 36 lines · 2024 bytes

SHA-256: `349a9314c6fcd9b8d7438f28a31e85ee558ed38da86bcb723b37c07a09ef0c90`

Source annotations: Literal, case-sensitive search. Full-file validation and digest refer to the same observed byte stream. The query and continuation offsets are byte-based.

Imports: [read_text](../../app/workspace/read_text.ep).

Definitions:

```text
5: define workspace_search_text with library as Int and root as Int and path as Str and query as Str and offset as Int and matches as Int returning Str:
```

Lexical test references: [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep).

## `app/workspace/sessions.ep`

[app/workspace/sessions.ep](../../app/workspace/sessions.ep) · 82 lines · 5529 bytes

SHA-256: `db4b79138f694d10d607eac504e51fd2e002e92570175d52fdbf7188b118ca8d`

Source annotations: Bind an owned session to one host-configured root. A changed root is an explicit migration, not an implicit new empty workspace. Reservation survives interruption before mkdir and keeps independent databases in separate names. device and session are supplied by the authenticated host/run, never accepted as filesystem-tool arguments. Root is host configuration, never model input.

Imports: [../security/group_gate](../../app/security/group_gate.ep), [../retained_text](../../app/retained_text.ep), [files](../../app/workspace/files.ep), [../security/principals](../../app/security/principals.ep), [../archive/runtime_lock](../../app/archive/runtime_lock.ep), [../tools/history_tools](../../app/tools/history_tools.ep).

Definitions:

```text
11: define workspace_session_schema with database as Int returning Int:
14: define workspace_session_allowed with database as Int and device as Str and session as Str returning Int:
30: define workspace_session_open with database as Int and library as Int and device as Str and session as Str and root_path as Str returning Int:
```

Lexical test references: [tests/agent/test_workspace_directory_recovery.ep](../../tests/agent/test_workspace_directory_recovery.ep), [tests/agent/test_workspace_file_recovery.ep](../../tests/agent/test_workspace_file_recovery.ep), [tests/agent/test_workspace_list_loop.ep](../../tests/agent/test_workspace_list_loop.ep), [tests/agent/test_workspace_loop.ep](../../tests/agent/test_workspace_loop.ep), [tests/agent/test_workspace_text_loop.ep](../../tests/agent/test_workspace_text_loop.ep), [tests/security/test_attach_file.ep](../../tests/security/test_attach_file.ep), [tests/security/test_continuity_tools.ep](../../tests/security/test_continuity_tools.ep), [tests/security/test_dispatch_revocation.ep](../../tests/security/test_dispatch_revocation.ep), [tests/security/test_group_output_policy.ep](../../tests/security/test_group_output_policy.ep), [tests/security/test_group_workspace.ep](../../tests/security/test_group_workspace.ep), [tests/security/test_host_execute.ep](../../tests/security/test_host_execute.ep), [tests/security/test_host_git_access.ep](../../tests/security/test_host_git_access.ep), [tests/security/test_private_web_scope.ep](../../tests/security/test_private_web_scope.ep), [tests/tools/test_live_archive.ep](../../tests/tools/test_live_archive.ep), [tests/tools/test_live_archive_create.ep](../../tests/tools/test_live_archive_create.ep), [tests/tools/test_live_table.ep](../../tests/tools/test_live_table.ep), [tests/tools/test_live_workbook.ep](../../tests/tools/test_live_workbook.ep), [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/workspace/test_directory_list.ep](../../tests/workspace/test_directory_list.ep), [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_list_archive.ep](../../tests/workspace/test_list_archive.ep), [tests/workspace/test_read_json.ep](../../tests/workspace/test_read_json.ep), [tests/workspace/test_read_table.ep](../../tests/workspace/test_read_table.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep), [tests/workspace/test_sessions.ep](../../tests/workspace/test_sessions.ep), [tests/workspace/test_text_operations.ep](../../tests/workspace/test_text_operations.ep), [tests/workspace/test_tool.ep](../../tests/workspace/test_tool.ep).

## `app/workspace/workbook_create.ep`

[app/workspace/workbook_create.ep](../../app/workspace/workbook_create.ep) · 186 lines · 10787 bytes

SHA-256: `f3ece3408a8dcdecadd1d30ceac9b51adf48f356b853dae759708a8ff83fc9ba`

Source annotations: Escape XML content and attributes without changing literal cell values. JSON scalars become typed cells. A formula object has formula and optionally cached; cached is an explicit supplied result, never a calculated claim.

Imports: [create_archive](../../app/workspace/create_archive.ep), [workbook_read](../../app/workspace/workbook_read.ep).

Definitions:

```text
5: define workbook_xml_escape with text as Str returning Str:
13: define workbook_column with column as Int returning Str:
23: define workbook_cell_xml with tokens as List and item as Int and reference as Str returning Str:
74: define workbook_sheet_xml with tokens as List and rows as List returning Str:
103: define workbook_sheet_name_valid with name as Str returning Int:
124: define workbook_create_parts with text as Str returning List:
182: define workspace_create_workbook with system as Int and root as Int and path as Str and text as Str returning Str:
```

Lexical test references: [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep).

## `app/workspace/workbook_edit.ep`

[app/workspace/workbook_edit.ep](../../app/workspace/workbook_edit.ep) · 72 lines · 4707 bytes

SHA-256: `16134978c82a6a2f37c5d6d7f5ba04f02b2e3abd85d197f0eebdc7ab017de418`

Source annotations: Explicit XML part replacement exposes the complete workbook format instead of flattening sheets to cell values. Unknown binary and XML parts remain present.

Imports: [workbook_create](../../app/workspace/workbook_create.ep).

Definitions:

```text
5: define workbook_xml_identity with text as Str returning Str:
15: define workspace_edit_workbook with system as Int and root as Int and source as Str and destination as Str and expected as Str and replacements as Str returning Str:
```

Lexical test references: [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep).

## `app/workspace/workbook_parts.ep`

[app/workspace/workbook_parts.ep](../../app/workspace/workbook_parts.ep) · 109 lines · 6113 bytes

SHA-256: `49a5e6f076b9eeb8483f6949fded31aad243cedccca71ab8dd44bb7b1f4cd512`

Source annotations: Read workbook XML parts without extracting archive paths onto the filesystem. Result: status, source digest, alternating part names and complete XML strings.

Imports: [xml_nodes](../../app/workspace/xml_nodes.ep), [archive_patch](../../app/workspace/archive_patch.ep), [files](../../app/workspace/files.ep), [../archive/file_digest](../../app/archive/file_digest.ep).

Definitions:

```text
8: define workbook_parts with system as Int and root as Int and path as Str returning List:
103: define workbook_part with parts as List and name as Str returning Str:
```

Lexical test references: [tests/workspace/test_edit_workbook.ep](../../tests/workspace/test_edit_workbook.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep).

## `app/workspace/workbook_read.ep`

[app/workspace/workbook_read.ep](../../app/workspace/workbook_read.ep) · 218 lines · 13201 bytes

SHA-256: `0071c17e74946bdf3bf44bc777cf059997317248028a5b77228a7808dd8b796f`

Source annotations: Resolve package-relative relationships only, never network or host paths.

Imports: [workbook_parts](../../app/workspace/workbook_parts.ep).

Definitions:

```text
4: define workbook_target with source as Str and target as Str returning Str:
39: define workbook_relationships_path with path as Str returning Str:
48: define workbook_relationship with nodes as List and source as Str and identity as Str and kind as Str returning Str:
69: define workbook_xml_root with nodes as List and name as Str returning Int:
81: define workbook_shared_strings with text as Str returning List:
106: define workbook_cells with nodes as List and strings as List and offset as Int and cells as Int returning Str:
160: define workspace_read_workbook with system as Int and root as Int and path as Str and sheet as Str and offset as Int and cells as Int returning Str:
```

Lexical test references: [tests/tools/test_live_workbook_edit.ep](../../tests/tools/test_live_workbook_edit.ep), [tests/workspace/test_create_workbook.ep](../../tests/workspace/test_create_workbook.ep), [tests/workspace/test_read_workbook.ep](../../tests/workspace/test_read_workbook.ep).

## `app/workspace/xml_nodes.ep`

[app/workspace/xml_nodes.ep](../../app/workspace/xml_nodes.ep) · 93 lines · 5541 bytes

SHA-256: `de8dd9836b6592369554227b9e981326e4c7c3cf4391cc6722a1b87c4d51f381`

Source annotations: Native XML events for workbook parts. No DTD, entity substitution, external loading or validation. Borrowed reader strings are copied before advancing. Each node: type, depth, local name, namespace, value, attributes, empty flag. XML_PARSE_NONET plus quiet diagnostics; errors return no partial document.

Imports: [../providers/local_json](../../app/providers/local_json.ep), [../archive/file_text](../../app/archive/file_text.ep).

Definitions:

```text
7: define workbook_xml_nodes with text as Str returning List:
68: define workbook_xml_attribute with node as List and name as Str returning Str:
78: define workbook_xml_text with nodes as List and index as Int returning Str:
```

Lexical test references: [tests/workspace/test_xml_nodes.ep](../../tests/workspace/test_xml_nodes.ep).

## `app/workspace_database.ep`

[app/workspace_database.ep](../../app/workspace_database.ep) · 34 lines · 1722 bytes

SHA-256: `f5e93e26c661708c49a6070d045941a65bd4ef893a6b3291d8a847ea40124ef6`

Source annotations: Launched only through the normal workspace sandbox and execution guardian. READWRITE | CREATE | NOFOLLOW; URI parsing and extension loading stay off.

Imports: [workspace/database_engine](../../app/workspace/database_engine.ep).

Environment names: `TMPDIR`.

Native `main` present. Argument indices read: 1, 2. Read its guards before invocation; diagnostic programs may mutate the selected archive, execute commands or call a real model.

Definitions:

```text
4: define main returning Int:
```
