# Discord UI parity — active implementation, 20 October 2026

Maria prioritised this work before the remaining Phase Two work. This is an
acceptance matrix, not a completion claim. The source inventory is
`app/web/routes.ep`, the canonical Phase Two A12 requirements and the explicit
20 October clarification. Add new web controls here as they appear.

| Web capability | Discord equivalent | Current acceptance |
| --- | --- | --- |
| Send and reply | Native mention and reply-to-message | Prior live text acceptance exists |
| User/account/source labels | Native account plus authenticated source in context/HUD | Name propagation tested; full HUD presentation pending |
| New session and session selection | `/new`, session selector/history controls | `/new` deployed; real ordinary-user command, consecutive turns and restart passed (`discord-new-command-2026-10-20.json`). `/history` and `/resume` now deployed with native isolation/replay checks and actual two-turn retained-context acceptance; see DISCORD-SESSION-HISTORY.md. |
| Thinking and tool activity | Persistent thread on originating user message | Full recorded reasoning/tool detail deployed; consecutive turns, restart and reload passed (`discord-trace-fidelity-2026-10-20.json`). Host data stays isolated. Finished/caught-up threads archive without deletion and reopen for resumed delivery; sentence-aware chunking and scoped thread invalidation passed targeted native/live acceptance (`discord-sentence-archive-2026-10-21.json`). |
| Interim and subsequent replies | Separate durable replies bound to immutable final events | Live delegation source produced two sent replies; broader UI parity pending |
| Speak | Public audio toggle on original bot reply | Show/hide/restore, burst coalescing, exact cache reuse, playback, restart and reload passed (`discord-shared-speech-toggle-2026-10-20.json`). A separate second-account exercise remains unperformed. |
| Queue, steer and cancel | Queue reactions plus labelled controls, steering confirmation, cancel | Native reservation/intake foundations; live controls pending |
| Questions | Buttons and Other modal; up to three questions/three suggestions | Foreground real UI, Unicode Other, persisted drafts across restart, explicit Submit and next-turn recall passed (`discord-question-shutdown-2026-10-21.json`). Child questions route through parent; delegated fault recovery remains open. |
| Free autonomy | Admin-only `/auto` | Installed and registered; native authorized-path and live guest denial tested. Maria explicitly deferred real admin interaction; do not repeat this request or claim it passed. |
| Task autonomy | Admin-only `/auto` with optional task | Installed; same deferred real-admin acceptance as free autonomy. |
| Autonomy stop and transfer | `/autooff`, shared ownership semantics | Stop implemented with native authorized/replay/isolation tests and live guest denial; admin positive/transfer exercise remains deferred/open. |
| Attachments | Native uploads, retained-source links, lifecycle controls | `/attachments` list/trash/restore-selection/exclude-context/restore-context deployed; actual Discord UI completed all four changes and restored the synthetic original with unchanged bytes (`discord-attachment-commands-2026-10-21.json`). Native isolation/retry/reopen passed. Live post-restart controls and complete retrieval-consumer coverage remain pending. |
| Audio, video and watch parties | Native attachments plus scoped playback/analysis controls | Full parity pending |
| Retry, regenerate, edit and branch | Native message edit plus bound buttons/commands | Source edit cancellation exists; remaining controls pending |
| History, search and export | `/history`, `/search query:`, scoped exports | History/resume, bounded literal search and private single-file verified export deployed and live-tested; multipart export and richer selectors remain open. See DISCORD-SESSION-HISTORY.md. |
| Projects and memory controls | Commands/selectors/forms | Native `/project` deployed: private list/create/read/use/clear and restart passed, task/file editing passed native tests. Live task creation, retained-file reference addition and combined readback passed; removal/second-actor exercises remain open. Reviewed `/share` preview/confirm deployed and live-tested across permitted channels in the same guild; richer selectors, second-account/revocation checks and memory UI remain open. See DISCORD-PROJECTS.md. |
| Schedules | Commands/forms and durable status/cancel controls | Background controller implemented; Discord UI acceptance pending |
| Compaction | Activity status and explanation with original history preserved | Pending |
| Settings, providers and HUD | `/hud run: section: after:` plus remaining settings controls | HUD deployed: native owner/member reads across six ordinary sections, isolation/revocation, command binding and frozen retries passed. Actual ordinary-account interaction and context/provider snapshots appeared privately; machine access was denied with account-specific feedback. Settings/provider mutation UI, live admin-positive and remaining section/paging exercises remain pending. See `discord-hud-commands-2026-10-21.json`. |
| Group membership and sharing | Native audience plus explicit authorised group controls | Full parity pending; no implicit Host sharing |
| Device/account linking | Authorised linking flow with separate platform memory | Full parity pending |

Every component must resolve an immutable server-side action binding, then
recheck actor, source, channel, session/run and current authority. Command
visibility alone is not authorization. Ordinary users cannot start, stop or
transfer autonomy; they retain permitted conversation functionality. Native
attachments remain native rather than being replaced with a slash-only flow.

Public threads inherit their parent audience. Discord-run reasoning and tool
activity belong in the thread attached to the prompting message, for everyone
with channel access. There is no private-activity button or alternate DM trace
stream. Host memory, credentials, raw voice references and embeddings remain
isolated at source; admin status does not import them into Discord. Historical
private delivery receipts are preserved as audit history, not executable routes.

Delivery must retain intended and confirmed state, reconcile uncertain network
outcomes and avoid duplicate messages after restart. Native tests are required
but not sufficient: qualify actual controls over multiple turns, reconnect,
reload, duplicate/stale clicks, revoked authority and two ordinary users.
No command or button is registered as usable before its handler exists.

Maria's explicit placement clarification: the prompt stays in the channel and
Echo's conversational replies stay there too. Only reasoning, tool activity
and progress belong in the attached thread. Completion must not delete it.
Discord auto-archiving is not deletion; reopening must retain the original
thread identity and history. A new user turn gets its own source binding.

Placement acceptance must check the original channel message ID, the thread's
source binding, and each conversational reply's channel. Creating a thread
alone does not pass automatic activity delivery. Never repost the user's prompt
inside the thread or redirect subsequent ordinary conversation into it.

20 October live placement check: source `1562189561566732461` remained in
channel `1562137652680204358`; conversational reply `1562189611852369961`
was confirmed in that channel. The attached activity thread retained the source
message identity and received two activity pages. This proves placement for
that turn, not complete UI parity or restricted trace acceptance.

API references: [threads](https://docs.discord.com/developers/resources/channel),
[interactions](https://docs.discord.com/developers/interactions/receiving-and-responding).
