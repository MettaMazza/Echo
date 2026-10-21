# Discord schedule controls — 21 October 2026

Candidate implementation; not registered or deployed yet. Schedule creation continues through Echo's existing scoped `schedule` tool. The new `/schedules` command adds explicit human list, inspect, occurrences and cancel controls, using a conversation ID from `/history`. It does not create a second scheduler.

The actor comes from authenticated Discord intake and the schedule must belong to that principal in the exact Discord conversation/channel. Linked Host identity and administrator status do not export private schedules. List and occurrence results use a `next_after` cursor. Cancelling stops future reservations; previously reserved or running occurrences retain their identities and ordinary cancellation controls.

The original interaction binds the command, actor, channel and options. Cancellation has a durable source event, repeated requests are idempotent, and conflicting reuse is rejected. Response bytes are frozen for delivery recovery, with current membership/ownership checked again before reuse. The receipt is private to the requesting account, unlike public conversation replies and their Thinking threads.

Native storage acceptance passed pagination, inspection, retained occurrences, owner-only cancellation, replay/conflicting reuse, reopen, Host/channel/account separation and membership revocation. Its initial fixture failed because it attempted a second origin while the first fixture still had a pending tool; the fixture now explicitly terminates its simulated origin. No production admission rule was changed. Native command-path acceptance also passed registration/options, exact acknowledged interaction binding, list/cancel, frozen response replay, spoof refusal, reopen and revoked-account refusal. Its first isolated fixture lacked the schedule migration; that fixture was corrected and passed. The worker build and actual Discord acceptance remain pending.

Private evidence: `evidence/phase2/discord-schedule-controls-20261021/`. No completion claim is made for the remaining Discord parity matrix or Phase Two.

## Live acceptance interruption

The labelled schedule request reached the existing schedule tool, which first required its task checklist. During the next checklist-generation stream, the previously installed foreground worker exited with signal 11. No schedule was created. The new schedule controls were not deployed or involved. The test was cancelled using its Discord Cancel button; native recovery verified the cancellation barrier and absence of running tool calls on a copied archive before applying the same recovery live. The same previously installed binaries were restarted idle. Original archives and partial stream remain preserved.

The partial provider bytes passed a socket/archive/cancellation-monitor replay, but that does not identify the original crash cause. Full-request concurrent-monitor diagnosis remains in progress. The six new worker/registration builds completed with an unchanged application-source manifest; promotion and registration are withheld pending this failure investigation and live acceptance.

The exact saved 75,002-token request subsequently completed in a separate archive under AddressSanitizer with a concurrent Discord cancellation monitor (18.609 s). A full copied-agent run, respecting the original runtime lock during an idle maintenance pause, also completed its real model/checklist/schedule/inspection/final sequence under AddressSanitizer (207.714 s). Neither reproduced the crash; neither delivered to Discord or ran a scheduler. The prior connector was restored afterward. These diagnostic timings are not a controlled production latency comparison.

Candidate correction: explicitly retain the cancellation monitor's query list and the agent wait list, plus all temporary lists in the new schedule controls. This removes a known unsafe allocation pattern without changing permissions or cancellation semantics. The corrected schedule command test passed. Cancellation regression, rebuilt workers and live acceptance remain pending. The original crash cause is still unproven.
