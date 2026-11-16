> Current runtime and 16 November verification: [current build](../../CURRENT-BUILD.md). Earlier dated evidence below remains historical; it does not supersede that record.

# Feature matrix — Echo 1.9

“Implemented” means a callable source path exists; deployment and acceptance may be narrower. “Partial” identifies unfinished integration or qualification. Configuration, permissions and external service availability still apply. Exact schemas: [tool catalogue](../../reference/TOOLS.md).

| Area | What exists | Boundary / source |
|---|---|---|
| Browser chat | Durable multi-turn conversations, streamed activity, pairing, history, branching/revisions and cancellation | [Web](../../../app/web/), [UI](../../../app/ui/); device/browser permissions apply |
| Discord conversations | Mentions, replies, reactions, durable delivery, questions, groups, revisions, history/search and exports | [Discord](../../../app/discord/); source/audience checks |
| Controlled testing | Admin-only Discord `/testing`, replay-safe toggle and admission | [testing_mode.ep](../../../app/discord/testing_mode.ep); no web restriction or prompt flag |
| Public community memory | Cross-user/public-room retrieval with speaker/session attribution and current visibility | [discord_retrieval.ep](../../../app/memory/discord_retrieval.ep); private/import ancestry checked |
| Private continuity | Explicitly linked private owner histories across DM/web | [private_bridge.ep](../../../app/memory/private_bridge.ep); denied in public/group runs |
| Memory | Episodes, semantic facts, procedures, correction/provenance, scoped automatic ingestion, source recall | [Memory](../../../app/memory/); source permissions remain authoritative |
| Long history | Exact originals, source-linked compaction, bounded multi-message recall and stable-prefix preparation | [Context](../../../app/context/), [continuity tools](../../../app/tools/continuity_tools.ep) |
| Time awareness | Historical timestamps separated from current observations and elapsed time | [temporal.ep](../../../app/context/temporal.ep) |
| Projects and tasks | Project files/instructions, checklists, scheduled/delegated work and result inspection | [Projects](../../../app/projects/), [jobs](../../../app/jobs/) |
| Foreground lifecycle | Completion handoff independent of background memory extraction; terminal activity quieting | [foreground_release.ep](../../../app/discord/foreground_release.ep) |
| Research | Configured Brave search, web fetch/navigation, originals and readable previews | [Network](../../../app/network/); external search requires operator key |
| Workspace/data | Scoped files, commands, CSV/TSV, JSON, archives, workbooks, local databases and plots | [Tools](../../../app/tools/); execution authority is scoped |
| Code and Git | Ernos build/execute, retained stdout and admin Git operations | [Workspace](../../../app/workspace/); host operations require authority |
| Images/documents | Attachment upload/read, image inspection and configured local image generation with attached delivery | [Media](../../../app/media/), [attachments](../../../app/discord/attachments.ep) |
| Speech | Local transcription and source-bound speech playback; selected Nano CPU8 reference voice | [chatterbox_speak.ep](../../../app/chatterbox_speak.ep); local dependencies/reference required |
| Video/watch | Retained library and bounded playback/observation workflows | [Media](../../../app/media/); provider-dependent, not every-frame/full-film acceptance |
| Discord administration | Channels, threads, messages/pins, members, roles, invites, events, emoji/stickers, AutoMod, onboarding, guild settings and audit reads | [Discord schemas](../../../app/tools/); permissions/hierarchy enforced |
| Discord voice | Membership, transport/encryption, playback, consent-based recording, transcription and artifact pipeline | [voice_tool.ep](../../../app/discord/voice_tool.ep); live operation depends on media services, consent and Discord permissions |
| Inspection | Authenticated HUDs, original model reasoning, tool observations, source-linked traces and owner-scoped retained research evidence | [HUD](../../../app/hud/); source records distinguish historical evidence from live state. |
| Protected changes | Charter, manifests, consent/reviews, staging, operation records and rollback foundations | [Rights](../../../app/rights/); candidate admission, tests/review, activation, health checks and rollback gates |
| Generated tools | Discovery/reuse evidence, Ernos candidate manifests, independent build/behaviour qualification, protected registration, scoped load/invocation, persistent versions and protected lifecycle requests | [candidate registry](../../../app/tools/tool_registry.ep), [runtime](../../../app/tools/registered_tools.ep), [registration worker](../../../app/lifecycle/tool_registration_worker.ep), [version worker](../../../app/lifecycle/tool_version_worker.ep). Deployed source; protected lifecycle actions retain their own qualification gates. |
| Model research | Native activation capture, steering/intervention and verified baseline restoration; owner-scoped retained experiment records in the authenticated HUD | [Research](../../../app/research/), [research HUD](../../../app/hud/research.ep). The HUD is included in the deployed restoration. Historical records do not establish currently active weights or completed training/promotion. |
| External inference | Local llama.cpp/MLX plus explicitly selected Anthropic Messages and OpenAI Responses, owner-bound credentials, native replay and context accounting | [native integration](../../../app/providers/external_native.ep), [accounts](../../../app/providers/external_accounts.ep). Deployed adapters; owner credentials and consent are required for live provider use. |
| Gmail / Contacts | Private mailbox read/sync, compose/reply/forward, drafts, attachments, Contacts, scoped sending and delivery reconciliation | [Mail](../../../app/mail/); deployed source, personal OAuth and correspondence permissions required |
| Recovery | Backup/restore, service lifecycle and durable interrupted-operation handling | [Operations](../../OPERATIONS.md); bounded retention and scoped unattended service administration |

[V1 remaining work](REMAINING.md) · [Verification](VERIFICATION.md) · [Join Discord](https://discord.gg/9QU2rXwGVF)

| Additional deployed capability | Source / boundary |
|---|---|
| Persistent inference cache | [Cache](../../persistent-session-cache-20261116.md); bounded compatible state, full useful context retained |
| Echo QoL | [Four features](../../echo-qol-20261116.md); scoped note, recall awareness, change notices, disclaimer awareness |
| Findings-only self-monitor | [Monitor](../../self-monitor.md); separate idle-time review, evidence-linked findings, no automatic RSI submission |

Current implementation and dated verification take precedence over historical pending-work descriptions.
