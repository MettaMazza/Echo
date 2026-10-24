> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Delegated task completion and independent returns — 17 October 2026

## Fault established from the live run

Parent `9621a635-ca3f-4ef5-bdfd-d966d0b8f1d2` had two research children. The physics child explicitly recorded that its core assignment was complete, but continued optional historical and source investigations before returning. The repository child repeated reads and reconstructed its work from history instead of a durable task progress record. These observations support a task-boundary and progress-tracking fix; they do not imply that the assigned research was too large.

The physics child completed at 18:26:08 UTC. The repository child remained active. The old `delegation_collect` implementation returned `pending` whenever any accessible child was nonterminal, preventing delivery of the already completed physics result. No parent `delegation_results` event existed at the investigation snapshot.

## Change

Delegated children now use `task_checklist`. Each item has a stable ID, the assigned requirement, pending/done/unresolved status, and evidence or a concrete limitation. The initial plan is requested before other work. Updates preserve the set of assigned requirements; explicit new parent guidance permits revision. This is a worker's task record, not an expansion of the Phase Two plan.

Checklist snapshots are immutable `task_checklist` archive events. Their identities include the child run and tool call, making interrupted-call retries idempotent. Normal archive digests protect the complete snapshot. Current progress is reconstructed directly into every child request, including after compaction, and is exposed to the authorized parent through `delegation_status`.

When no pending requirements remain, the model request uses `tool_choice: none` for the final report. Dispatch also refuses additional tool effects after completion, including extra calls in the same batch. New authenticated parent guidance allows revising the checklist and resuming work. Unresolved requirements remain explicit; they are not represented as successful findings. The model remains responsible for accurately mapping the assignment and citing real observations: checklist status alone is not independent proof of factual correctness.

Completed child results are collected independently. Prior delivery events identify each delivered child/head, so later batches do not repeat earlier results. Collection and parent state transition remain in one SQLite transaction. A collected batch remains eligible for worker admission retry even while another child is pending. Existing results, traces, authority checks, full context, and model capacity are preserved. The checklist schema stays in the shared authorized tool catalog to preserve the parent/child cache prefix; execution is restricted to an actual delegated child.

## Source map

- `app/jobs/task_checklist.ep`: validated durable snapshots, readback and completion state.
- `app/tools/task_checklist_schema.ep`: native tool schema and task-boundary guidance.
- `app/context/session_messages.ep`: shared tool catalog (child-only execution), current progress reconstruction and final-response tool choice.
- `app/agent/tool_dispatch.ep`, `app/security/tool_policy.ep`: authorized dispatch and completion enforcement.
- `app/tools/delegation_tool.ep`: parent-visible checklist.
- `app/jobs/delegation_return.ep`: independent, exactly-once result batches.
- `tests/jobs/test_task_checklist.ep`: persistence, requirement preservation, completion and explicit revision.
- `tests/jobs/test_delegation_return.ep`: first child delivery before sibling completion, later delivery, retry and reopen deduplication.
- `tests/jobs/test_live_checklist_return.ep`: real local model child/checklist/final/parent-report cycle with two children.

## Verification and deployment

The self-hosted Ernos compiler built the server and regression executables successfully. `test_task_checklist` passed against a fresh isolated archive, including reopen, missing-item rejection, resolved/unresolved evidence and parent-guidance revision. `test_delegation_return` passed against a fresh isolated archive, including independent first delivery, retry admission while a sibling remains unfinished, second delivery, full Unicode results, compaction sources, and reopen deduplication.

The real-model two-child test executable was built but was **not run**: the user then explicitly requested stopping all tasks and leaving the system idle for manual testing. No live-model checklist behavior is claimed verified by the synthetic tests.

Deployed release: `phase2-delegation-checklist-20261017`, server SHA-256 `fae18089929566894616dcd5f8f8375f1708e961fe250ba3af9b2102633a736a`. The application was restarted; inference and the gateway were not restarted. Active research was cancelled through the authenticated application API at the user's request; saved progress and completed results were retained. Final idle verification is recorded locally under runtime `maintenance/checklist-idle-verification.json`. General development is paused for manual testing.
