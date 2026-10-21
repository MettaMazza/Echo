# Authorised local Discord history search

Phase Two Step 15 message-search slice implemented and deployed. Native and actual multi-turn Discord acceptance passed after the two retained failures below. This does not close the full community catalogue or Phase Two.

`discord_message_read` adds `search_local` with a case-sensitive literal `query`, page `limit` (1–100), and optional `before` taken from its returned `next_before`. Search cursors are local index positions, not message IDs. An empty result page can still have a cursor because deleted or changed candidates were filtered; continue until the cursor is empty. Concurrent edits can change ordering. This is an index of locally observed history, not a claim to have searched every server message.

The derived index consumes immutable Gateway create/update/delete/bulk-delete observations and explicitly labelled, receipt-linked REST message observations. Partial updates without content preserve the prior indexed text. Tombstones clear indexed text and cannot be resurrected by replay. A transaction advances the durable input cursor together with changes; interruption rolls back the incomplete refresh. Original events remain retained. Verified read/history results feed the index without fabricating Gateway events.

The model cannot supply a channel, guild, account, route or credential. Candidates come only from the authenticated originating channel. The index supplies IDs, never result text: each candidate is freshly read from Discord, checked for exact message/channel identity and rematched against its current content. Deleted and edited-away matches are omitted. Other observation failures abort rather than releasing stale cached text. Requester/bot access, current run/call authority, source identity, revocation barriers and cancellation are checked by the existing read controller before release. Host/private conversation archives are never searched. Nested forwards/references and unrelated account fields remain excluded.

The source reuse inspection is recorded in [message read](DISCORD-MESSAGE-READ.md) and [joined groups](DISCORD-JOINED-GROUPS.md#actual-hive-ingestion-inspection). Reinspected the actual ErnosDecent bridge's `_exec_bridge_command` and scoped `search_rag_database`: the former reads channel history, the latter delegates session RAG. Neither was treated as an existing public-history index implementation. Echo's index uses its own immutable journal and fixed-origin REST/access layer; no legacy prompt or role policy was copied.

`test_message_index` passed current-channel isolation, full/partial edits, individual and bulk tombstones, literal matching, cursor exhaustion, repeated refresh, reopen and REST observation indexing. Invalid search parameters and cross-channel arguments are refused. Original `test_message_read_options` regression and the final production worker build passed. No compiler, model, voice or identity/emotive prompt changes are part of this stage.

## Live finding retained

The initial real search returned `message-search-observation:rate-response-unresolved` without releasing cached text; a later read-only retry succeeded. The implementation had retained an SQLite iterator while HTTP receipt recording attempted a write transaction. Candidate rows are now detached and the iterator finalized before any network operation. A production-WAL concurrent-writer regression reproduces the old pattern and checks the corrected boundary. Failed-observation receipt names are now propagated instead of being dropped. Initial receipts and build attempts remain retained; the corrected worker passed the live checks recorded below.


## Interrupted search and recovery

A subsequent live paginated search crashed before retaining its tool result. Generated C showed the detached candidate list lacked a garbage-collector root. An initial list seed did not root this local in the full production compilation and was rejected before deployment. The application now passes the detached list through an explicitly typed parameter before allocating, without changing the compiler. The native snapshot regression forces and verifies a collection, then checks the retained candidate identity; it passes alongside the WAL receipt-write regression.

Recovery records `interrupted-read` for an abandoned, currently authorised `discord_message_read` call with valid read options. It does not reconstruct a result, perform HTTP, or replay an action. Mutations, revoked authority and cancelled runs remain unreconciled by this handler. Native recovery checks passed those cases, native ownership exclusion and repeat-recovery receipt reuse. A host maintenance entry point handles confirmed-dead foreground reads and returns the original request to normal source/access preflight only after every call is observed. The native foreground fixture and actual confirmed-dead labelled request both recovered. The original request resumed without being resent; normal source/access preflight ran again.


## Actual acceptance — 22 October 2026

The recovered original request returned two fresh matches, then independently read the older match with identical projected fields. A new user turn continued the cursor and returned the final original user message with an empty cursor. Another ordinary turn, without tools, returned the correct marker, arithmetic result and account username. Six projected observations were independently compared against their retained REST bodies: exact IDs, current channel, text, timestamp and author fields; no additional nested account/reference fields were released.

Reload preserved the channel replies, Speak controls and original-message Thinking threads. Opening the original thread confirmed actual model reasoning, the honest interruption record, the new read call/results and independent-read evidence. All six workers were alive and no runs active at the check. Voice, model selection, identity/emotive prompts and compiler were unchanged.

The model called the earlier interruption “transient rather than broken.” That wording is not the engineering diagnosis: the recorded GC crash required the repair described above. The failure remains visible rather than being rewritten as a successful first attempt.

Warm first-output times for the two subsequent user turns were 3.340 and 3.081 seconds; decode rates were about 25 tokens/second with roughly 109–111k total prompt tokens. These are observations, not a controlled equivalence to the morning baseline of 1.864 seconds and 31.613 tokens/second. The initial catalogue-change cold request took 159 seconds before output; latency/endurance qualification remains open.

Evidence: canonical `evidence/phase2/discord-message-search-20261021/` contains native fixtures/build logs, the original failures, generated-code root verification, recovery and deployment receipts, `verified-receipts.json`, and `live-acceptance.json`. Public documentation intentionally excludes conversation payloads and credentials.
