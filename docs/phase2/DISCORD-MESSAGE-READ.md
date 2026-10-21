# Discord message reads — Phase Two Step 15

Status: implemented, deployed at idle with rollback, and qualified across four actual Qwen3.8 Discord turns plus reload. Native option/response and child-runtime binding checks passed. This is not full community-tool or Phase Two completion.

`discord_message_read` provides `read` by message ID and paginated `history` within the authenticated originating channel. History accepts one of `before`, `after`, or `around` and a page size of 1–100 (default 50). Returned IDs can be used to continue paging; there is no fixed total-history window. It returns text, message/channel IDs, author account ID and available account names, timestamp, and the retained REST receipt. It does not currently return attachments, embeds or nested forwarded/referenced messages. Guild-wide search, cross-channel reads, message mutations and other community tools remain separate unfinished requirements.

The tool is advertised only in Discord archives and executes only inside the authenticated Discord child runtime. It binds the actual queued tool call, archive filename, bot binding, journal and admitted run origin. Credentials and routes cannot be supplied through tool arguments. Requester/bot channel access is checked before retrieval and again before returning the result; source edit/deletion, cancellation and authority changes invalidate delivery. The existing channel gate supports writable text/announcement channels, usable threads and authorized one-to-one admin DMs. It conservatively requires the existing send/history permissions; this slice does not expand read-only channel or voice-channel access.

The complete REST page is checked for matching channel identity and duplicate message IDs before any projected data is returned. Single-message reads also require the requested ID. Nested forwarded/referenced objects and additional account metadata are omitted to prevent implicit cross-audience retrieval. Text and names remain untrusted source material, not instructions or authority. The tool does not manufacture Gateway events or import another conversation's memory.

## Source comparison

Inspected `engine/ErnosDecent-reference/decent_net/discord_bridge.py`, `_exec_bridge_command`, lines 3158–3200. The actual legacy `read_channel` uses `history(limit=200)` and formats author display names/content; its reaction command accepts arbitrary channel/message arguments. This candidate uses Echo's authenticated origin and explicit pagination instead. Legacy prompt/role policies were not copied. The prior actual HIVE ingestion inspection and pinned revision are documented in [joined groups](DISCORD-JOINED-GROUPS.md#actual-hive-ingestion-inspection); no claim that its ingestion implements this read controller.

API source: [Discord Message Resource](https://docs.discord.com/developers/resources/message), consulted 21 October 2026. History ordering, mutually exclusive cursors and the 1–100 page limit follow the documented endpoint.

## Verification

`test_message_read_options` passed: strict operation/field typing, unexpected route/channel rejection, malformed IDs, conflicting cursors, page bounds, single-message identity, mixed-channel and duplicate-page rejection, omission of nested references/account metadata and empty pages. The initial compile rejected the reserved variable name `channel`; it was renamed and the failure retained. No compiler changes.

`test_message_read_binding` passed: real queued-call dispatch refuses generic host and forged Discord-shaped runtimes, forged call IDs are denied, and the catalog is Discord-only. Production worker build passed. The final allocation-rooting cleanup binds the read-route string before constructing its result list; option tests and the rebuilt worker passed, and it was promoted at idle before the third/fourth live turns. Prior workers remain available for rollback. No prompt, model or Warm voice changes are part of this stage.


## Actual four-turn acceptance, 21 October 2026

1. History with limit 2 and an explicit `before` cursor returned two actual channel messages. Echo then read one by ID; an independent comparison of the complete projected objects proved equality.
2. Continuing before the older returned ID retrieved the preceding human message and account name. Echo retained the current conversation marker and treated the old instructions as source text; the trace contains only a read call.
3. After the final worker restart, another exact-ID read returned the correct author, retained marker and arithmetic result.
4. A normal no-tool follow-up returned the same marker and correct arithmetic. All four replies remained visible after Discord reload, with attached Thinking threads and Speak controls. Zero active runs at the observation point.

Actual requests were bound to Qwen3.8 and advertised the new tool. Raw provider reasoning and four successful REST tool results are retained. Private evidence: `evidence/phase2/discord-message-read-20261021/{native.json,rooted/candidate.json,verified-comparisons.json,live-acceptance.json}`. Source/test/build failures are retained alongside passes; the initial reserved-word compile failure was corrected without compiler changes.

Performance is recorded, not called solved: the first request processed 87,613 uncached prompt tokens and reached first output after 135.482 seconds. Subsequent user-turn first-output times were 2.778 and 3.285 seconds for the paginated and post-restart reads; the final no-tool turn reached first output in 1.646 seconds. Tool continuations reused 87,553–91,354 cached tokens with 1.238–2.070 second first-output times. Decode was approximately 27.48–27.82 tokens/second in these tool turns. These are dispatch-to-first-output observations, not full response latency or a controlled comparison with the historical morning median 1.864 seconds / 31.613 tokens/second. Whole-system latency/endurance acceptance remains open.
