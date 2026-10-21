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
- Bind controls to confirmed outbound messages, not merely a custom ID.
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
