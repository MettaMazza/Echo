# Discord message reads — Phase Two Step 15

Status: candidate implementation; native option/response validation passed. Child-runtime binding also passed. Production build passed and was deployed only after idle checks, retaining rollback. Actual model/Discord acceptance is in progress. This is not full community-tool or Phase Two completion.

`discord_message_read` provides `read` by message ID and paginated `history` within the authenticated originating channel. History accepts one of `before`, `after`, or `around` and a page size of 1–100 (default 50). Returned IDs can be used to continue paging; there is no fixed total-history window. It returns text, message/channel IDs, author account ID and available account names, timestamp, and the retained REST receipt. It does not currently return attachments, embeds or nested forwarded/referenced messages. Guild-wide search, cross-channel reads, message mutations and other community tools remain separate unfinished requirements.

The tool is advertised only in Discord archives and executes only inside the authenticated Discord child runtime. It binds the actual queued tool call, archive filename, bot binding, journal and admitted run origin. Credentials and routes cannot be supplied through tool arguments. Requester/bot channel access is checked before retrieval and again before returning the result; source edit/deletion, cancellation and authority changes invalidate delivery. The existing channel gate supports writable text/announcement channels, usable threads and authorized one-to-one admin DMs. It conservatively requires the existing send/history permissions; this slice does not expand read-only channel or voice-channel access.

The complete REST page is checked for matching channel identity and duplicate message IDs before any projected data is returned. Single-message reads also require the requested ID. Nested forwarded/referenced objects and additional account metadata are omitted to prevent implicit cross-audience retrieval. Text and names remain untrusted source material, not instructions or authority. The tool does not manufacture Gateway events or import another conversation's memory.

## Source comparison

Inspected `engine/ErnosDecent-reference/decent_net/discord_bridge.py`, `_exec_bridge_command`, lines 3158–3200. The actual legacy `read_channel` uses `history(limit=200)` and formats author display names/content; its reaction command accepts arbitrary channel/message arguments. This candidate uses Echo's authenticated origin and explicit pagination instead. Legacy prompt/role policies were not copied. The prior actual HIVE ingestion inspection and pinned revision are documented in [joined groups](DISCORD-JOINED-GROUPS.md#actual-hive-ingestion-inspection); no claim that its ingestion implements this read controller.

API source: [Discord Message Resource](https://docs.discord.com/developers/resources/message), consulted 21 October 2026. History ordering, mutually exclusive cursors and the 1–100 page limit follow the documented endpoint.

## Verification

`test_message_read_options` passed: strict operation/field typing, unexpected route/channel rejection, malformed IDs, conflicting cursors, page bounds, single-message identity, mixed-channel and duplicate-page rejection, omission of nested references/account metadata and empty pages. The initial compile rejected the reserved variable name `channel`; it was renamed and the failure retained. No compiler changes.

`test_message_read_binding` passed: real queued-call dispatch refuses generic host and forged Discord-shaped runtimes, forged call IDs are denied, and the catalog is Discord-only. Production worker build passed. Live multiple-turn read/history/pagination and reload acceptance is in progress. Deployment preserved the prior workers for rollback. A subsequent allocation-rooting cleanup binds the read-route string before constructing its result list; its option tests passed and the final worker rebuild is in progress. No prompt, model or Warm voice changes are part of this stage.
