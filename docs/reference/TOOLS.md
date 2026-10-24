# Callable tool catalogue — Echo 1.9

Names and descriptions extracted from literal source schemas. Availability is filtered by runtime, principal and audience. Presence here is not an executed acceptance test. Dynamic schemas may extend this catalogue. [Full contracts](TOOL-SCHEMAS.json).

| Tool | Source | Purpose |
|---|---|---|
| `attach_file` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Attach a file from the current session workspace to your reply |
| `attachment_list` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Find the next saved attachment across this private account and its linked devices, or the current shared conversation |
| `attachment_listen_audio` | [app/tools/attachment_image.ep](../../app/tools/attachment_image.ep) | Listen to an original saved WAV or MP3 audio attachment through the primary model's native audio input |
| `attachment_read_text` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Read verified UTF-8 text or extracted PDF page text from this private account and linked devices, or the current shared conversation |
| `attachment_transcribe` | [app/media/speech_request.ep](../../app/media/speech_request.ep) | Transcribe a saved WAV audio attachment locally |
| `attachment_video` | [app/media/video_request.ep](../../app/media/video_request.ep) | Inspect an authorized local video, sample a frame, or use audiovisual to supply its video and decoded soundtrack together to the primary native model |
| `attachment_view_image` | [app/tools/attachment_image.ep](../../app/tools/attachment_image.ep) | Reopen an original saved image in this session, including after compaction |
| `autoOff` | [app/interaction/tools.ep](../../app/interaction/tools.ep) | Turn off your continuous autonomy mode whenever you want to stop or encounter an issue |
| `delegate_task` | [app/tools/delegation_schema.ep](../../app/tools/delegation_schema.ep) | Start an independent durable task using your current user/group authority |
| `delegation_message` | [app/tools/delegation_schema.ep](../../app/tools/delegation_schema.ep) | Send guidance to a child task created in this conversation by your authenticated owner, without cancelling it or creating another run |
| `delegation_status` | [app/tools/delegation_schema.ep](../../app/tools/delegation_schema.ep) | Inspect a child task created in this conversation by your authenticated owner |
| `discord_audit_log` | [app/tools/discord_audit_schema.ep](../../app/tools/discord_audit_schema.ep) | Read a configured guild's Discord audit log in a configured admin's private one-to-one DM with Echo |
| `discord_automod` | [app/tools/discord_automod_schema.ep](../../app/tools/discord_automod_schema.ep) | Manage Discord automod rules in the configured admin's private one-to-one DM only, for an explicitly configured guild |
| `discord_channels` | [app/tools/discord_channel_schema.ep](../../app/tools/discord_channel_schema.ep) | Configured Discord admin only |
| `discord_events` | [app/tools/discord_event_schema.ep](../../app/tools/discord_event_schema.ep) | Configured Discord admin only: list/inspect/users/create/edit/start/end/cancel/delete scheduled events in this authenticated guild |
| `discord_expressions` | [app/tools/discord_expression_schema.ep](../../app/tools/discord_expression_schema.ep) | Configured Discord admin only: list, inspect, create, edit or delete guild emoji and stickers |
| `discord_guild_settings` | [app/tools/discord_guild_settings_schema.ep](../../app/tools/discord_guild_settings_schema.ep) | Inspect or update supported server settings in the configured admin's private one-to-one DM with Echo |
| `discord_invites` | [app/tools/discord_invite_schema.ep](../../app/tools/discord_invite_schema.ep) | Configured Discord admin only |
| `discord_members` | [app/tools/discord_member_schema.ep](../../app/tools/discord_member_schema.ep) | Configured Discord admin only, current authenticated guild |
| `discord_message_read` | [app/tools/discord_message_read_schema.ep](../../app/tools/discord_message_read_schema.ep) | Read message text and author identity from the authenticated current Discord channel, or page its history newest first |
| `discord_moderate_message` | [app/tools/discord_moderation_schema.ep](../../app/tools/discord_moderation_schema.ep) | Configured Discord admin only: inspect a current-channel message, delete a permitted message, remove one user reaction, clear one emoji or all reactions |
| `discord_onboarding` | [app/tools/discord_onboarding_schema.ep](../../app/tools/discord_onboarding_schema.ep) | Inspect and update guild onboarding in the configured admin's private one-to-one DM with Echo |
| `discord_pins` | [app/tools/discord_pin_schema.ep](../../app/tools/discord_pin_schema.ep) | List current-channel pins or inspect a message pin state |
| `discord_post` | [app/tools/discord_post_schema.ep](../../app/tools/discord_post_schema.ep) | Create an explicit reply to the current request with text, text embeds, a native poll or one retained attachment; inspect/edit/delete your tool post for this requester in this channel |
| `discord_reaction` | [app/tools/discord_reaction_schema.ep](../../app/tools/discord_reaction_schema.ep) | Inspect reactions on the current Discord request, discover usable server/application custom emoji, or add/remove Echo’s own reaction to that request |
| `discord_roles` | [app/tools/discord_role_schema.ep](../../app/tools/discord_role_schema.ep) | Configured Discord admin only |
| `discord_threads` | [app/tools/discord_thread_schema.ep](../../app/tools/discord_thread_schema.ep) | Configured Discord admin only |
| `discord_voice` | [app/tools/discord_voice_schema.ep](../../app/tools/discord_voice_schema.ep) | Send only the fields for the selected operation; omit unused fields entirely, never empty placeholders |
| `history_read` | [app/tools/history_tools.ep](../../app/tools/history_tools.ep) | Reread original event text from this session using Unicode character offset and requested characters |
| `history_search` | [app/tools/history_tools.ep](../../app/tools/history_tools.ep) | Find the next original event in this session containing a literal query |
| `host_execute` | [app/tools/host_text.ep](../../app/tools/host_text.ep) | Admin host command under the Mac account permissions |
| `host_git` | [app/tools/git_schema.ep](../../app/tools/git_schema.ep) | Admin Git workflow at an absolute repository/directory path using installed Git |
| `host_read_text` | [app/tools/host_text.ep](../../app/tools/host_text.ep) | Admin only: read UTF-8 text from an absolute host path with the Mac process permissions |
| `image_generate` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Generate a local image from a prompt |
| `job_control` | [app/tools/job_schema.ep](../../app/tools/job_schema.ep) | Inspect your scoped run queue and request cancellation |
| `memory_graph` | [app/tools/knowledge_tools.ep](../../app/tools/knowledge_tools.ep) | Traverse evidence-linked facts and episodes within the current private or exact group audience scope by exact subject or object name; empty node scans all |
| `memory_messages` | [app/tools/continuity_tools.ep](../../app/tools/continuity_tools.ep) | Read original user and Echo message text in an owned session in one batch |
| `memory_read` | [app/tools/memory_tools.ep](../../app/tools/memory_tools.ep) | Read an original message or attributed imported source record within the initiating private principal, own accessible Discord channel conversations, or exact shared audience, by event ID with Unicode offsets |
| `memory_record` | [app/tools/knowledge_tools.ep](../../app/tools/knowledge_tools.ep) | Persist a derived fact or episode in the current private or exact group audience scope with original source_event and exact source_digest from history/memory tools |
| `memory_search` | [app/tools/memory_tools.ep](../../app/tools/memory_tools.ep) | Search attributed imported source records and original user and Echo messages across your authenticated private conversations, or your own Discord conversations across accessible channels in this guild |
| `memory_sessions` | [app/tools/continuity_tools.ep](../../app/tools/continuity_tools.ep) | List your authenticated private owner’s sessions in chronological batches |
| `memory_set_visibility` | [app/tools/knowledge_tools.ep](../../app/tools/knowledge_tools.ep) | Withdraw a derived assertion in the current private or exact group audience scope from ordinary graph recall, or explicitly restore it |
| `memory_status` | [app/tools/knowledge_tools.ep](../../app/tools/knowledge_tools.ep) | Inspect only visibility and revision for a known memory assertion identity in the current private or exact group audience scope, including a forgotten one |
| `memory_timeline` | [app/tools/episode_schema.ep](../../app/tools/episode_schema.ep) | Read source-backed private episodic memories chronologically |
| `procedure` | [app/tools/procedure_schema.ep](../../app/tools/procedure_schema.ep) | Save and retrieve scoped procedural memory without executing it |
| `project` | [app/tools/project_schema.ep](../../app/tools/project_schema.ep) | Persistent scoped projects |
| `protected_change_inspect` | [app/tools/protected_tools.ep](../../app/tools/protected_tools.ep) | Read the exact full protected proposal, digest and current review revision |
| `protected_change_review` | [app/tools/protected_tools.ep](../../app/tools/protected_tools.ep) | Record your explicit consent, objection, rejection or counterproposal |
| `reasoning_read` | [app/tools/continuity_tools.ep](../../app/tools/continuity_tools.ep) | Read captured model reasoning from your private history |
| `request_user_input` | [app/interaction/tools.ep](../../app/interaction/tools.ep) | Ask the user clarifying or follow-up questions during the current task |
| `schedule` | [app/tools/schedule_schema.ep](../../app/tools/schedule_schema.ep) | Create, inspect or cancel a durable scoped schedule |
| `scratchpad` | [app/tools/continuity_tools.ep](../../app/tools/continuity_tools.ep) | Persistent private working notes across this owner’s sessions |
| `scratchpad` | [app/tools/scratchpad_schema.ep](../../app/tools/scratchpad_schema.ep) | Persistent working notes: private notes follow their owner across sessions; shared notes belong only to the current audience segment |
| `session_get_compaction` | [app/tools/compaction_tools.ep](../../app/tools/compaction_tools.ep) | Read an original saved recap from this session |
| `session_rebuild_recap` | [app/context/recap_rebuild.ep](../../app/context/recap_rebuild.ep) | Request a fresh recap from original session records, for example after a user correction or finding an omission |
| `speech_synthesize` | [app/media/voice_request.ep](../../app/media/voice_request.ep) | Create a saved spoken reply locally from supplied text using the host-selected installed voice |
| `state_inspect` | [app/hud/schema.ep](../../app/hud/schema.ep) | Inspect current authorized interaction, exact context-count receipts and compaction, conversation audience, authorized protected proposals/reviews, admin machine measurements, work, schedules or original incidents |
| `system_inspect` | [app/context/system_awareness.ep](../../app/context/system_awareness.ep) | Inspect the current run's effective authority, actual selected model, live provider health, reported context and modalities, and installed Echo architecture |
| `task_checklist` | [app/tools/task_checklist_schema.ep](../../app/tools/task_checklist_schema.ep) | Save the durable checklist for your current task, whether working directly for the user or as a delegated agent |
| `tool_candidates` | [app/tools/candidate_schema.ep](../../app/tools/candidate_schema.ep) | Search your scoped saved tool candidates before authoring another tool |
| `tool_search` | [app/context/session_messages.ep](../../app/context/session_messages.ep) | Discover currently loaded tools before choosing or creating one |
| `trace_read` | [app/tools/trace_tools.ep](../../app/tools/trace_tools.ep) | Read an exact event returned by trace_search, with Unicode offset/characters and next_offset |
| `trace_search` | [app/tools/trace_tools.ep](../../app/tools/trace_tools.ep) | Search exact original reasoning/tool/error trace records in your authorized private linked history or current shared conversation and its branch ancestry |
| `video_library` | [app/media/video_library.ep](../../app/media/video_library.ep) | Browse or search the private local video library, then select an existing item for this conversation |
| `watch_party` | [app/interaction/watch_party.ep](../../app/interaction/watch_party.ep) | Read, pause or end the current shared watch session |
| `web_fetch` | [app/tools/web_fetch_schema.ep](../../app/tools/web_fetch_schema.ep) | Fetch a public HTTP(S) URL and preserve complete source bodies, response headers, redirect provenance and SHA256 attachments |
| `web_follow` | [app/tools/web_fetch_schema.ep](../../app/tools/web_fetch_schema.ep) | Navigate a numbered link from a saved extracted HTML document attachment |
| `web_search` | [app/tools/web_fetch_schema.ep](../../app/tools/web_fetch_schema.ep) | Search the public web using the host-configured search provider (Brave API when configured; otherwise DuckDuckGo HTML) |
| `workspace_archive_create` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Create a new ZIP, TAR or gzip TAR at workspace path from explicit regular workspace sources |
| `workspace_archive_extract` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Extract one regular ZIP/TAR/gzip TAR entry by zero-based index from workspace_archive_list into an explicit new workspace destination |
| `workspace_archive_list` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Inspect ZIP, TAR or gzip TAR metadata in the scoped workspace without extracting or following links |
| `workspace_build` | [app/tools/build_schema.ep](../../app/tools/build_schema.ep) | Build or check a plain-English Ernos source in this session workspace with the installed pinned compiler |
| `workspace_create_text` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Create a new UTF-8 file at a relative path in this session workspace |
| `workspace_create_workbook` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Create a new XLSX in your workspace |
| `workspace_database` | [app/tools/database_schema.ep](../../app/tools/database_schema.ep) | Run SQLite SQL against a relative database path in your isolated workspace, creating it if absent |
| `workspace_diff` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Compare two UTF-8 files in this workspace |
| `workspace_edit_text` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Replace a UTF-8 workspace file with complete new text |
| `workspace_edit_workbook` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Save an edited XLSX to a NEW workspace destination, preserving the source and every unreplaced entry payload |
| `workspace_execute` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Execute one command in this session workspace under the local sandbox |
| `workspace_list` | [app/tools/workspace_list_schema.ep](../../app/tools/workspace_list_schema.ep) | List entries in the current session workspace |
| `workspace_make_directory` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Ensure a relative directory path exists in this session workspace, creating missing parents |
| `workspace_output_next` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Find the next original stdout/stderr event for an execution in this session |
| `workspace_plot` | [app/tools/plot_schema.ep](../../app/tools/plot_schema.ep) | Create a reproducible SVG line/scatter plot at a new relative workspace path |
| `workspace_read_json` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Read a UTF-8 JSON workspace file |
| `workspace_read_table` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Read CSV, TSV or another single ASCII delimiter UTF-8 table in the scoped workspace |
| `workspace_read_text` | [app/tools/attachment_tools.ep](../../app/tools/attachment_tools.ep) | Read a UTF-8 passage from a relative file path in this session workspace |
| `workspace_read_workbook` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Read XLSX stored cell data from your workspace |
| `workspace_search` | [app/tools/workspace_operations_schema.ep](../../app/tools/workspace_operations_schema.ep) | Search a UTF-8 file for a literal case-sensitive query |
