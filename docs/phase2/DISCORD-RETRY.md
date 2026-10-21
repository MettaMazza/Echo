# Discord saved-run retry

`/retry message:` continues a saved, safely retryable run rather than submitting
its prompt again. The original human message, a public command anchor or a
confirmed delivered reply can identify the source. IDs convey no authority:
the executor checks current Discord access, exact source actor/channel/guild,
current local run authority and the worker's native ownership lock.

Only a ready run with a durably returned worker, no cancellation and every tool
call observed is eligible. Active, completed, failed-terminal and cancelled runs
are not reset. A new generation for a finished request remains `/regenerate`.
Uncertain tool work must be reconciled before retry; the command cannot declare
it safe or re-execute it. Existing completed results remain attached to the run.

A request receipt preserves the exact run, source, actor device, previous worker
and previous failure status. The receipt and dispatch eligibility change commit
in the same archive transaction. The journal retains/reserves conversation
ownership. An interruption between those stores is repairable using the same
receipt. After another worker takes the run, replay is a no-op. The usual isolated
Discord worker performs the eventual continuation and rechecks live access.

Initial native controller acceptance passed in 10.750 s, binary
`d8c49991edc66cb4f552a7af3c738699d1d745a20d85a68178b9407e174954fa`.
It covered wrong actor/channel, live ownership, running worker, unobserved tools,
cancellation, request replay, reopen, interrupted journal recovery and rejection
of an old request against a newer worker. Private receipt:
`manual-retry-20261021/1792608006613281000/result.json`.

Final combined native acceptance passed in 14.442 s, binary
`d84bfb26ea21245d532138bbf7b2412acb18790b8f5b3f255860eb0fc7dbdfd8`.
It adds registered command binding, source-message resolution, first-use anchor
schema, foreground requests with no prior background reservation, conversation
ownership conflict and revoked-account denial. Receipt:
`manual-retry-20261021/qualified-1792608688338756000/result.json`.
The earlier fixture failed at its expected-success assertion because its prior
cancelled run still held a reservation. That rejection is now asserted before
explicitly releasing the fixture reservation; production ownership checks were
not relaxed. Both receipts are retained.

Deployed registration and actual Discord acceptance remain pending. Native
acceptance alone does not establish live recovery or Phase Two completion.

Live registration succeeded after handler deployment. The first real command
against a completed foreground run safely refused retry but described it as
account-unavailable because that run had no worker row. The corrected lookup
checks terminal run state even without a worker record. Expanded native
acceptance passed in 14.637 s, SHA
`c1fe27bc7df439897ea9d8629090a3fbcff32346fd27abd3eb6be8e5ba546f7a`;
receipt `manual-retry-20261021/completed-fix-1792608872357414000/result.json`.
The original live feedback failure remains recorded. Corrected live acceptance
and positive live recovery remain pending.
