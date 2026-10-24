> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# General task tracking — 18 October 2026

## Cause and scope
The parent model advertised task_checklist, but saving a checklist was restricted to delegated children. The reviewed finance run repeatedly planned work without durable parent task progress. Its bare `python3` execution was also resolved as a nonexistent workspace executable, so the calculation had not run. This repair applies to all tool-based tasks; it contains no finance-specific routing or arithmetic.

## Implementation
- `app/jobs/task_checklist.ep`: authenticated parent runs may save the same immutable checklist snapshots as children. Requirements and IDs remain stable unless newer authenticated guidance revises the task. Retries preserve call identity; snapshots survive reopen and compaction.
- `app/agent/tool_dispatch.ep`: before a parent starts tool work it must record its requested outcomes. The gate returns a recoverable, no-effects result. Existing authority checks remain in place.
- `app/context/session_messages.ep`: ordinary conversation can answer without a checklist. Pending work requires tool continuation; resolved work requests a final response without further tool calls. Guidance asks for a small plan, observed evidence, preserved unknowns and completed reads, not optional additional work.
- `app/tools/task_checklist_schema.ep`: the tool describes both parent and child use.
- `app/tools/execution_tool.ep`: bare executable names resolve through the same trusted PATH used by the child environment. Workspace paths and existing sandbox restrictions remain. A missing command produces a specific no-process-started error. Results direct the model to decoded workspace_output_next output and require checking the raw wait status. The separate execute_guardian executable must be rebuilt and deployed with these changes.

A checklist stores model-reported evidence; it does not independently prove every claim or guarantee perfect model decisions. Unavailable requirements can be marked unresolved with an explicit limitation so the user still receives a result.

## Verification
The compiled `tests/jobs/test_task_checklist.ep` fixture passed against a fresh isolated SQLite archive. It checks parent and child initialization, preserved requirements, omitted-item rejection, idempotence, evidence after reopen, guidance revision, request tool-choice transitions, and executable resolution including missing names and traversal rejection.

The first live test caught a stale execution helper in the candidate release: the parent checklist worked, but bare Python still failed. That diagnostic was stopped and retained. Both server and execution helper were rebuilt for release phase2-general-task-tracking-r2-20261018.

Final live run `2187f11b-3678-49d0-862e-348694119801` passed through the compiled client submission functions, live HTTP, real local model, tool dispatcher, execution guardian, archive and session API. The browser harness used DOM doubles; this is not a visual browser test.

Without an explicit checklist request, Echo saved five requested outcomes, preserved their IDs and descriptions, executed bare `python3` with wait_status 0, read decoded stdout (A=18, B=13, one unknown), saved and read back a report, attached it, completed the same checklist and returned known total 31 with the missing quantity still unknown. No tool calls followed checklist completion. The attachment was present on the final session message and downloaded with matching SHA-256 `2a93c3fecd33cb7f654ddbbfc54fa17f8266738afc1312f465903266ad401a44` (1,032 bytes). Final delivery took 240.947 seconds over the multi-step task.

Ordinary-chat follow-up `0aae98d3-fbbb-4b3a-bb5e-c683d87571ce` delivered in 44.853 seconds with no tools or checklist. These are observed timings, not an overall latency acceptance claim.

Evidence in the local runtime maintenance directory: `general-task-native-verification.json`, `general-task-first-live-failure.json`, `general-task-deployment.json`, `general-task-live-results-r2.json`, `general-task-live-assertions.json`, `general-task-session-api.json`, and `general-task-idle.json`. Original conversation records and workspace files were preserved. This targeted repair does not mark all Phase Two work complete.


Final relaunch: HTTP 200, zero active runs, all four model slots and the embedding slot idle. Development compilers and test observers have exited. Echo remains running for manual testing.
