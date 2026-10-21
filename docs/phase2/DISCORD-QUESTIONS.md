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
- Qualify the modal-aware initial callback and type-5 intake against live
  Discord before rollout. The native callback ledger and intake are implemented;
  live delivery and action execution remain unqualified.
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

Acknowledgement and intake source integration is now implemented, not deployed.
`interaction_callback.ep` records the callback kind and source/callback digests
before transmission. The ACK worker selects the validated generic Other modal
as its initial callback, then recomputes the remaining deadline. Confirmed
modal-open goes directly to completed without entering the action worker.
An interrupted/uncertain modal-open retains its receipt and expires without
replay or a request for a nonexistent deferred original. Older deferred
callbacks keep the existing original-response reconciliation path.

Type-5 submissions use the same authenticated Gateway intake, guild allowlist,
admin-only DMs, token separation, cursor transaction and replay identity as
other interactions. Admission is not answer authorization. Execution still
needs the current control binding, live source/access check and pending request.
The answer action worker is now connected in source; renderer/delivery and
live qualification remain unfinished. Modal submission saves only a draft;
explicit Submit completes the batch.

Answer execution (source integration, not deployed):
`question_execution.ep` rechecks the configured guild scope, current Discord
access and original message before passing an acknowledged, exact stored
interaction to `question_action.ep`. Local checks resolve the confirmed
control, current run authority, cancellation and pending request.

Draft changes and their operation receipt commit in one journal transaction.
A retry of an older click returns its existing receipt without overwriting a
newer draft. Submit freezes the exact answer batch before calling the existing
archive transaction. Recovery reuses that snapshot and the shared validator's
request-keyed answer receipt; it cannot silently pick up later draft changes
or append a second guidance event. A new explicit click is needed after an
incomplete submission. No answer content is included in status acknowledgements.
