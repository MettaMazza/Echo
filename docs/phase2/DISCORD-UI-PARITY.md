# Discord UI parity — active implementation, 20 October 2026

Maria prioritised this work before the remaining Phase Two work. This is an
acceptance matrix, not a completion claim. The source inventory is
`app/web/routes.ep`, the canonical Phase Two A12 requirements and the explicit
20 October clarification. Add new web controls here as they appear.

| Web capability | Discord equivalent | Current acceptance |
| --- | --- | --- |
| Send and reply | Native mention and reply-to-message | Prior live text acceptance exists |
| User/account/source labels | Native account plus authenticated source in context/HUD | Name propagation tested; full HUD presentation pending |
| New session and session selection | `/new`, session selector/history controls | Pending; preserve same user/channel silo and eligible long-term memory |
| Thinking and tool activity | Persistent thread attached to the exact originating user message; private equivalent where threads cannot preserve access | Scoped projection and cursor-recovery native passes; automatic public activity discovery delivered across consecutive channel turns, restart retained existing posts without duplication. HAS_THREAD metadata cancellation regression fixed and two further live turns passed. Restricted full-trace UI and wider permission/fault acceptance remain pending |
| Interim and subsequent replies | Separate durable replies bound to immutable final events | Live delegation source produced two sent replies; broader UI parity pending |
| Speak | Button on bot response, actual Warm synthesis of that response | Live two-turn synthesis/playback, repeat-click cache, connector restart and client reload passed. Isolated-child missing-button regression corrected. One acknowledgement timeout retained; read-only recovery of an accepted response passed against Discord using a private cloned journal. Additional multi-user/revocation checks remain open; never substitute the audition WAV |
| Queue, steer and cancel | Queue reactions plus labelled controls, steering confirmation, cancel | Native reservation/intake foundations; live controls pending |
| Questions | Buttons, selects and Other modal; up to three questions and three suggestions each | Pending |
| Free autonomy | Admin-only `/auto` | Pending |
| Task autonomy | Admin-only `/auto` with optional task text | Pending |
| Autonomy stop and transfer | `/autooff`, status/stop buttons; transfer on new enable | Pending; same shared ownership and completion semantics as web |
| Attachments | Native uploads, retained-source links, lifecycle controls | Audio route has live evidence; complete lifecycle/parity pending |
| Audio, video and watch parties | Native attachments plus scoped playback/analysis controls | Full parity pending |
| Retry, regenerate, edit and branch | Native message edit plus bound buttons/commands | Source edit cancellation exists; remaining controls pending |
| History, search and export | `/history`, scoped search/forms and downloadable exports | Pending |
| Projects and memory controls | Commands/selectors/forms | Pending |
| Schedules | Commands/forms and durable status/cancel controls | Background controller implemented; Discord UI acceptance pending |
| Compaction | Activity status and explanation with original history preserved | Pending |
| Settings, providers and HUD | Admin controls with read-only permitted status for ordinary users | Pending |
| Group membership and sharing | Native audience plus explicit authorised group controls | Full parity pending; no implicit Host sharing |
| Device/account linking | Authorised linking flow with separate platform memory | Full parity pending |

Every component must resolve an immutable server-side action binding, then
recheck actor, source, channel, session/run and current authority. Command
visibility alone is not authorization. Ordinary users cannot start, stop or
transfer autonomy; they retain permitted conversation functionality. Native
attachments remain native rather than being replaced with a slash-only flow.

Public threads inherit their parent audience. Privileged traces cannot be
posted there merely because the requesting author is an admin. Resolve a
permitted restricted destination before exporting privileged records. Do not
expose private Host memory, credentials, raw voice references or embeddings.

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
