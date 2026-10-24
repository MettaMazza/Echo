> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord latency investigation — 22 October 2026

## Observed failures

A Discord turn stopped before provider dispatch because its child process aborted in `group_memory_source_allowed`, reached from parallel memory preparation. The native crash stack ended in `append_list` and malloc reporting an invalid pointer. Generated C showed an unrooted inline argument list whose first element called an allocating function. The supervisor deliberately stopped rather than implicitly replaying uncertain work. In this case the original run remained ready, with zero model requests and zero tool calls, allowing explicit recovery of the same intake.

Separately, the earlier reaction acceptance recorded 216.914 seconds dispatch-to-first-output followed by 3.617 and 2.769 seconds. The provider log recorded 137,768 evaluated prompt tokens taking 216.629 seconds on the cold request. This is prompt processing, not a Discord network delay. It must not be conflated with the crash or reported as fixed by restarting the worker.

## Changes

- Root the source-authorization argument list, computed session value and query across allocating calls. No compiler or garbage-collector change.
- Within one shared-memory SELECT, check the session-wide history gate once per distinct source session. The open SQLite statement holds its read snapshot. The query still verifies source digests, audience/session boundaries, branch boundaries and attachment visibility for every row. The checked-session list is local to that scan; no authorization result survives into a later call or turn.
- Materialize the eligible prior claim identities once per recall-delta query. Preserve its complete ancestry, source boundary, completed-run, revision and base predicates. This replaces repeated parsing of every prior snapshot for each fresh claim; it does not change which records are selected.
- Retain complete memory records, reasoning, provider routing, tool availability and the selected Warm voice. No context truncation, reasoning suppression or replacement with Instruct.

## Validation and deployment status

The initial root fix passed 1,000 allowed/denied source checks with observed garbage collection, plus the existing shared-memory context suite covering continuity, exact deltas, reload, checkpoint and forget rebasing, source digests, delegated children and private/user/channel/server/DM boundaries. Generated C explicitly roots the new values.

The original interrupted request was recovered and its final reply visibly delivered in General. The first two real follow-up turns retained the test marker and returned correct answers, with reasoning present. Before the scan optimization, their dispatch-to-first-output times were 6.451 and 3.035 seconds; gateway-receipt-to-first-output was 11.851 and 11.178 seconds. These are different metrics and neither is final-message delivery latency.

The session scan optimization passed the same native suite and was deployed. Its first two real turns measured 2.617/2.870 seconds dispatch-to-first-output and 7.544/10.348 seconds gateway-to-first-output, with correct marker recall and reasoning. This exposed a separate 2.2-second recall-delta comparison. On one unchanged live database read snapshot, the old delta query took 2.124/2.086 seconds, and the materialized query took 0.021/0.020 seconds. Returned result hashes matched exactly. The final delta-query change passed the native shared-context regression and was deployed while idle. Three subsequent real Discord turns completed and delivered, including a system_inspect call; all retained the marker and exposed reasoning. Their initial model first-output times were 2.634, 2.628 and 2.725 seconds. Gateway-to-first-output was 7.281, 7.482 and 7.519 seconds. Full replies reached Discord in 16.215, 17.297 and 32.098 seconds respectively (the third included a real tool call and another generation). These full-delivery times remain above the desired target. Warm memory capture took 571 ms total, including 94 ms context preparation, compared with 2,722 ms total / 2,245 ms context preparation before materialization. The exact data and matched SQL equivalence evidence are retained locally. Local canonical evidence is under `Echo/evidence/phase2/discord-latency-20261022`; private conversation payloads and crash reports are not published.

## Remaining performance limits

The historical morning baseline was 1.864 seconds median first output and 31.613 decoded tokens/second over ten warm requests. The current workload has different conversation and prompt sizes, so this is a reference, not a matched benchmark. Cold cache rebuilding remains open. Phase Two and full latency acceptance are not complete.
