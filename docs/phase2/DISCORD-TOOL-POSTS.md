# Model-callable Discord tool posts — Phase Two Step 15

Status: deployed in the approved testing stage; native option, child-runtime binding and store/recovery tests passed. Live create/edit/delete and independent status reads have succeeded; final reload/restart acceptance remains in progress. The full message/moderation catalogue remains open.

`discord_post` implements explicit text `reply`, `status`, known-pending `retry`, `edit` and `delete` for posts created by this controller. A reply targets the current authenticated request in its channel. Subsequent actions require the returned post ID and the same originating requester/channel. It cannot modify another user's posts, ordinary final replies, status cards or Thinking traces. Normal conversation and its Speak controls retain their existing separate path; editing those accepted replies needs coordinated revision/Speak handling and is not claimed by this implementation.

A post retains its original run, requester, channel, outbound intent, nonce, attempt, remote message ID and delivery receipts. Identity is derived from the immutable run/tool-call pair. A repeat of that identity must match its original body. The generic reply worker excludes tool-post intents, so it cannot race the tool controller between enqueue and authority verification. Delivery uses the existing nonce-verifying outbox. Sending/uncertain states never return to pending without evidence; authenticated Gateway observations may reconcile them. `retry` only dispatches a known pending post under a fresh authorized request. It does not retry uncertain sends.

The controller verifies the real child runtime, current run/call authority, archive/journal/bot binding, source request and Discord access. It checks remote channel/message/author identity before modification, and rechecks access before edits/deletions. Names and remote content cannot confer authority. A wrong requester/channel receives a scoped refusal. Original receipt history remains retained after edits/deletion; a later 404 is reported as currently not found, without inventing who deleted it.

Text is limited to Discord's single-post 2,000 UTF-16-unit limit, with UTF-8 validation. This is not a conversation/history ceiling; ordinary long replies retain existing sentence-aware chunking. Creation and edits explicitly suppress parsed mentions and reply-user pings. No arbitrary destination, URL, credential, component or raw REST body is accepted. This slice does not implement files, embeds, polls, crossposting, moderation of other users or changes to normal final-reply/Speak bindings.

## Source and verification

Uses the actual Echo `discord_outbox_enqueue`, `discord_delivery_send`, delivery reconciliation and fixed-origin access transport; no new generic network execution path. The inspected ErnosDecent `_exec_bridge_command` implements read/channel/reaction paths but not this own-post controller. Prior HIVE ingestion inspection is pinned in [joined groups](DISCORD-JOINED-GROUPS.md#actual-hive-ingestion-inspection); no HIVE prompt policy is copied.

API: [Discord Message Resource](https://docs.discord.com/developers/resources/message), consulted 21 October 2026: create-message reply/nonce/allowed-mentions semantics and edit/delete ownership. Nonce enforcement has a short Discord-side window; durable local uncertain-state handling remains necessary and is retained.

Native `test_post_options` passed strict fields/operations, no model-supplied channel, malformed operations, Unicode length and disabled mention payloads. Initial isolated test compilation lacked an explicit UTF-8 helper import; corrected import, no compiler changes, failure preserved. `test_post_binding` passed actual queued dispatch refusal outside the trusted child runtime, forged-call denial and Discord-only schema. `test_post_store` passed wrong actor/channel isolation, immutable provenance, blocked generic dispatch, crash/reopen and uncertain-send refusal. Full worker build passed. Real post/edit/status/delete across multiple turns and reload evidence remain pending.

## Live regression findings

The first real create/status calls exposed a shared formatter omission: `sent` was absent from `feedback_success`, so successful receipts acquired contradictory failure advice. Echo independently reread the post and did not duplicate it. The correction recognizes `sent`; native coverage also ensures `sending`, `uncertain` and `pending` remain unresolved. Original failed feedback receipts are retained.

The queue isolation fixture now includes an earlier pending tool post in the same channel. It cannot occupy or block normal reply delivery. Its old expectation that a later same-origin reply overtake a deferred first reply conflicted with the established FIFO contract in `test_reply_schedule`; that expectation was corrected, preserving the initial exit-8 failure. The corrected test passes through close/reopen. No production FIFO relaxation was made.
