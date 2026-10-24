> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Compaction latency and live delegation monitoring — 17 October 2026

## Observed fault

The active children of run `1bd7ba1a-48cc-4c82-81bf-d332b769be85` were labelled ready because the UI displayed the scheduler state. Their workers were running compaction. Candidate generation already disabled thinking; independent review enabled it. The transcript candidate required 317.369 seconds of prompt evaluation (143,147 tokens) and 363.949 seconds for 4,587 generated tokens. Subsequent reviews generated thousands of reasoning characters before emitting their structured reports. Large-source ingestion remains real work and was not removed or truncated.

## Changes

- `app/context/recap_prompt.ep`: independent full-source review emits its structured verdict directly, without a separate free-form reasoning pass. Source coverage, candidate binding, structural validation, semantic comparison, repair and checkpoint activation gates remain in place. No input or output token ceiling was added.
- `app/context/recap_contract.ep`: explicitly distinguishes actions described inside tool-returned documents from actions performed by the current run. The actual review found this confusion in the saved transcript candidate; this is a real continuity defect, not stylistic review feedback.
- `app/web/work_status.ep`: adds current activity, worker status and compaction observation to authorised run results.
- `app/ui/client.ep`: delegated panels show Compacting context and the current stage while running; a pending recap without a running worker is labelled waiting to resume. Internal scheduling state is unchanged.
- `app/tools/delegation_tool.ep` and `delegation_schema.ep`: `delegation_status` exposes live activity, compaction stage and the latest model-request event. Optional `history` accepts JSON-encoded history-search or history-read arguments. Echo can inspect current model input, messages, tool results and recorded traces while a child is active. Reads remain restricted to the authorised child, same initiating principal and originating conversation, with existing group-history checks. Pages use explicit caller-selected character sizes and cursors; there is no fixed total history limit. Recorded input is evidence, not new instructions.

## Verification

A replay of the transcript review against the identical complete source and candidate took 88.372 seconds under concurrent load: 14.576 seconds prompt work, 71.553 seconds generation, 715 generated tokens, zero reasoning characters. It returned ten positional verdicts and concrete defects requiring repair, including misattributed document actions. This is not an unloaded timing or a guarantee for every recap.

The native live semantic test accepted a faithful numerical paraphrase and rejected an invented file deletion. `test_agent_compaction_status.ep` passed pending-compaction/current-worker status, cancellation, access and archive checks. `test_delegation_monitor.ep` passed active child-context reading, cross-turn access, child-only history, foreign and unrelated-conversation denial, guidance and revocation checks. The Ernos browser fixture displayed `Agent · Compacting context` and its PASS result in the actual browser. Generated production JavaScript passed syntax checking.

## Deployment and continuity

Release: `phase2-compaction-monitor-20261017-100927`.

The model service was not restarted. Only the two affected compaction workers were restarted, after verifying complete saved candidates, through normal lock-aware worker recovery. Both retained the same accepted run heads and resumed review with `enable_thinking:false`. All 26 captured candidate, binding, user, tool-result and accepted-tool-call receipts retained their original digests. No archive row was deleted, replaced or reconstructed. Partial interrupted provider streams also remain archived; completed candidates were reused. New application requests use the updated monitoring schema. Existing browser tabs require reloading once to load the new JavaScript.

The agents continue their original tasks. Completion is not asserted here. Broader Phase Two remains paused; these changes address the reported bugs and requested active-agent visibility only.
