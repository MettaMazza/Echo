# Durable scheduling

Current status, 16 October 2026: scheduling storage and automatic dispatch are deployed; the live archive has `echo_schedules` and the service runs the scheduler. Step 08 remains in progress, including delegation. The sections below retain the sequence of earlier acceptance checks; their statements about production refer to those checks, not the current deployment.

Inspected ErnosDecent's pinned `decent_agent/scheduler.ep` and `scheduler_tool.ep`. They route scheduled inference through the ordinary IPC agent runner rather than concurrently invoking the ReAct state directly. Echo will similarly consume occurrences through its normal isolated run workers. Echo does not copy the reference's owner-tagged instruction or fixed final-reply waiting limit: source authority is retained in database identities, not instruction text.

Definitions retain the exact origin run/call, complete instruction, first due UTC epoch milliseconds, interval and explicit missed-run policy. A zero interval means once. `catch-up` reserves each missed interval in order; `coalesce` reserves the latest due interval and records the skipped count. Recurrence stays anchored to scheduled time rather than completion time. The maximum representable timestamp is handled without integer wrap; this is an arithmetic boundary, not an artificial scheduling or context limit.

Reservation and next-due advancement are one SQLite writer transaction. Immutable occurrence rows survive interruption for subsequent dispatch. Reopening continues the stored schedule without duplicate occurrences. Creation retries must match exact definition bytes including source identities. Future reservations can be cancelled by the same currently authorised principal in the appropriate private/shared scope. Already reserved occurrences remain visible, and cancellation does not assert rollback of their effects.

Every reservation revalidates the original run and call's frozen authority. Grant, revocation or link changes cannot silently promote an old schedule. The forthcoming worker adapter must revalidate that origin again at dispatch and while executing; storing a due occurrence does not itself grant execution authority.

Native fixture evidence: `evidence/phase2/schedule-store-20261016/verification.json`, build log and archive snapshot. Once/interval behavior, both missed-run policies, cancellation isolation, immutable receipts, reopen and authority-change denial pass. Automatic worker dispatch, recurring service integration and delegation remain unfinished within original step 08. No running scheduler or production schema migration is claimed.

## Ordinary run admission and worker evidence

`schedule_run.ep` now atomically materializes an occurrence into the original private/shared conversation, with its full instruction, scheduled-origin receipt, exact frozen authority and copied archived provider selection/profile when present. Busy sessions retain the occurrence for retry. Existing runs are reused on exact retries and after reopening. Shared runs retain their recorded sender and group boundary. Existing runtime authority checks continue to invalidate later promotions, revocations and membership changes.

An isolated occurrence completed through `web_worker_run_owned` using the live local Qwen model and returned its requested marker. This exercised the ordinary worker's provider loop, cancellation monitor and durable terminal result. Separate native checks covered shared-run scope and absence of private memory access; that shared fixture was not sent to the model. Evidence: `evidence/phase2/schedule-run-20261016/verification.json` and archive snapshot. Automatic OS dispatch/ticking remained pending at that check; the model-facing tool evidence follows below. Production was not migrated or restarted.

## Model-facing schedule interface

The `schedule` tool now creates, lists, inspects and cancels definitions and pages occurrence history. Its schema is advertised only when schedule storage exists. Inputs reject asserted principal/device authority and unknown operation fields. Read queries follow current private/group scope; occurrence records distinguish reserved work from an actual run and expose its last evidence event. The discovery contract describes persistence, cancellation effects and recovery.

Both streamed dispatcher tests and the actual local Qwen model passed. The model created the exact one-off definition, inspected its returned identity, cancelled it and completed with the requested marker. Native tests additionally cover foreign isolation, invalid scope, occurrence paging, discovery and revocation. Evidence: `evidence/phase2/schedule-tool-20261016/verification.json`. The background ticker and automatic process dispatch remain pending; production still has no scheduling migration or tool exposure.

## Automatic background dispatch

The candidate server supports schema 25 and starts a separate `--scheduler` process only when schedule storage is installed and restoration permits execution. The scheduler holds an archive-specific native lock, exits when its launching server exits and is restarted by that server after process failure. Polling once per second is a responsiveness cadence, not a task/context/turn limit. Normal model work remains in the existing owned worker processes.

Each pass snapshots due schedules and reserves one outstanding occurrence per definition. Catch-up retains original due times; coalescing records missed intervals. A live or unclaimed occurrence prevents unbounded reservation ahead of execution. Busy conversations defer materialization, retaining the due receipt. Existing worker claims prevent duplicate launches. Confirmed OS launch failures have a compare-and-switch retry path; running/uncertain work is not reclaimed by the ticker. Changed dispatch outcomes are archived as `schedule_dispatch_status`, with identical observations deduplicated.

A disposable full server automatically reserved and executed a due occurrence with the actual local model. The native verifier passed after completion, after forced scheduler termination/restart, and after a complete server restart. Exactly one occurrence/run origin remained. Parent termination also stopped its scheduler. Saved process logs, native verification metadata and archive are in `evidence/phase2/schedule-service-20261016/`. The forced OS launch-failure branch and interruption during model generation were not exercised by this specific test. All fixture server/scheduler processes were stopped. Production remains on its previous release/schema; no live migration occurred.

## Shared job dispatch and current deployment

`app/jobs/dispatch.ep` now owns the existing admitted-run worker launch path. Scheduling still performs its original atomic admission first, then calls this shared function. Authority checks, durable worker claims, explicit launch-failure retries and terminal-state handling are unchanged. Delegation will use this path after its own admission; no delegation tool is implemented by this extraction.

The rebuilt native scheduler fixture launched an isolated occurrence through the ordinary OS worker and actual local Qwen model. It completed with the requested marker; a second dispatch pass retained exactly one occurrence and run origin. Evidence: `evidence/phase2/job-dispatch-20261016/`. The earlier one-shot fixture lost its child processes when the command environment ended and is not counted as a pass; the successful fixture kept the command environment alive until worker completion.

Deployed release: `phase2-job-dispatch-20261016-221213`. Local HTTP and LAN HTTPS status endpoints returned 200; the existing model process remained running. This is host-side connectivity evidence, not a new physical-phone test.
