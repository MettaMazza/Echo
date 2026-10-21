# Discord follow-up questions — implementation in progress

The web question contract remains authoritative: one to three questions,
zero to three suggestions per question, and explicit free text. A suggested
or preselected choice is never treated as a submitted answer.

`app/discord/question_store.ep` binds a pending immutable question event to
its admitted Discord run, original message, actor, guild and channel. Draft
selections persist in the connector journal. Complete explicit submission
uses the existing web answer validator and appends the same answer and
scoped guidance records, with authenticated Discord environment provenance.
The local check revalidates current run authority and pending question state.
Transport handlers must additionally recheck live channel/source access.

This is not a deployed UI. Remaining integration:

- Discover pending batches from running and delegated Discord jobs and deliver
  them durably to the original channel, without moving the prompt or answer
  into the thinking thread.
- Connect the native confirmed-message control bindings to actual delivery.
  Render all question/option text without silent truncation; use numbered
  choices when Discord label limits are shorter than the source text.
- Save selections as drafts. Provide Other through a text modal and an
  explicit Submit control for the batch. Display the saved draft state.
- Handle modal-open as the initial interaction callback; a deferred callback
  cannot subsequently be replaced by a modal. Keep token material outside
  model/context projections. Modal submission is interaction type 5.
- Refresh actor, message, channel, authority and pending request at execution;
  reject foreign, superseded, cancelled and revoked actions. Reconcile
  uncertain delivery and submission without duplicate guidance.
- Qualify actual multi-question Discord interaction, Other, follow-up,
  reattachment of controls after reload, restart, duplicate/stale clicks,
  steering and cancellation. Keep ordinary channel conversation responsive.

References inspected: [Discord interaction responses](https://docs.discord.com/developers/interactions/receiving-and-responding)
and [component reference](https://docs.discord.com/developers/components/reference).
No final identity/system-prompt change is included.

Source review: Echo's existing `interaction/questions.ep`, web answer validator,
Discord admission/origin resolver and interaction callback worker were inspected.
HIVE scope blob `69c59740c40468b7bb1998d82ce7035ce7f37235` requires exact
user/channel matching; ErnosDecent's `build_discord_interaction_tags` uses
transport account metadata. Neither supplies this durable question workflow.
Echo retains its explicit authority records rather than adopting legacy
role-name or default-admin inference.

Native state acceptance passed on 21 October: actual ordinary Discord admission,
three-question batch, saved drafts/reopen, invalid and foreign submissions,
explicit complete answer, duplicate prevention, revocation and archive integrity.
The receipt is `../releases/1.75/discord-questions-2026-10-21.json`.
This does not establish live Discord component or modal acceptance.

Native control acceptance (not production integration):
`question_controls.ep` binds each choice, Other and Submit control to an
immutable question/outbox record. Resolution requires a confirmed sent
message, its bot author, the original actor, channel and guild, and original
admission provenance. A copied control ID is insufficient. Modal submission
can resolve only an Other control. These bindings do not replace the later
live access and current pending-question checks.

The Other modal uses a generic label and reveals no question content. Its
parser accepts one explicitly typed text field through a current Label or
legacy Action Row, preserves exact Unicode and newlines, and rejects blank,
numeric, duplicate and incorrectly named fields. Opening a modal does not
save an answer or resume a run.

Acknowledgement integration remains pending. Record the callback kind before
sending it: a modal callback has no deferred original message to reconcile.
A confirmed modal-open must finish that interaction without passing through
the ordinary action worker. An interrupted or uncertain modal-open must not
be blindly replayed or reconciled by fetching a nonexistent original reply.
The later modal submission is a distinct authenticated interaction and saves
only a draft; the explicit Submit button completes the batch.
