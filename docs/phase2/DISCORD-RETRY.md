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

Command binding, final combined fixture, deployed registration and actual Discord
acceptance remain pending. This document does not claim that `/retry` is already
available in the live connector or that Phase Two is complete.
