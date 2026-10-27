> Current runtime and October 27 verification: [current build](../../CURRENT-BUILD.md). Earlier dated evidence below remains historical; it does not supersede that record.

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
| Discord administration | Channels, threads, messages/pins, members, roles, invites, events, emoji/stickers, AutoMod, onboarding, guild settings and audit reads | [Discord schemas](../../../app/tools/); permissions/hierarchy enforced; autonomous policy controller incomplete |
| Discord voice | Membership, transport/encryption, playback, consent-based recording, transcription and artifact pipeline | [voice_tool.ep](../../../app/discord/voice_tool.ep); full live workflow acceptance pending |
| Inspection | HUDs, original model reasoning, tool observations and source-linked traces | [HUD](../../../app/hud/); research/controller sections partially implemented |
| Protected changes | Charter, manifests, consent/reviews, staging, operation records and rollback foundations | [Rights](../../../app/rights/); full recursive lifecycle qualification pending |
| Generated tools | Discovery, candidate manifests, immutable versions and dependency checks | [tool_registry.ep](../../../app/tools/tool_registry.ep); candidates saved, not activated/reloaded |
| Model research | Native activation capture and steering/intervention components | [Research](../../../app/research/); full agent/UI integration, training/promotion/HUD acceptance pending |
| External inference | Local llama.cpp and MLX adapters | [prepare.ep](../../../app/providers/prepare.ep); two external API families and origin/isolation work pending |
| Gmail | Planned mailbox integration | Required by [Phase Two](../../../phase2.md); not implemented in this source update |
| Recovery | Backup/restore, service lifecycle and durable interrupted-operation handling | [Operations](../../OPERATIONS.md); final reboot/restore/soak matrix pending |

[V1 remaining work](REMAINING.md) · [Verification](VERIFICATION.md) · [Join Discord](https://discord.gg/9QU2rXwGVF)
