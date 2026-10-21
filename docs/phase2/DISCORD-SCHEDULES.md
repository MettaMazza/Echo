# Discord schedule controls — 21 October 2026

Candidate implementation; not registered or deployed yet. Schedule creation continues through Echo's existing scoped `schedule` tool. The new `/schedules` command adds explicit human list, inspect, occurrences and cancel controls, using a conversation ID from `/history`. It does not create a second scheduler.

The actor comes from authenticated Discord intake and the schedule must belong to that principal in the exact Discord conversation/channel. Linked Host identity and administrator status do not export private schedules. List and occurrence results use a `next_after` cursor. Cancelling stops future reservations; previously reserved or running occurrences retain their identities and ordinary cancellation controls.

The original interaction binds the command, actor, channel and options. Cancellation has a durable source event, repeated requests are idempotent, and conflicting reuse is rejected. Response bytes are frozen for delivery recovery, with current membership/ownership checked again before reuse. The receipt is private to the requesting account, unlike public conversation replies and their Thinking threads.

Native storage acceptance passed pagination, inspection, retained occurrences, owner-only cancellation, replay/conflicting reuse, reopen, Host/channel/account separation and membership revocation. Its initial fixture failed because it attempted a second origin while the first fixture still had a pending tool; the fixture now explicitly terminates its simulated origin. No production admission rule was changed. Command-path acceptance, final build and actual Discord acceptance remain pending.

Private evidence: `evidence/phase2/discord-schedule-controls-20261021/`. No completion claim is made for the remaining Discord parity matrix or Phase Two.
