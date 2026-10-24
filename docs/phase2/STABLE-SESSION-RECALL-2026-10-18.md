> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Stable session recall and direct lookups — 18 October 2026

## Observed failure

The user's new three-turn conversation reached visible reply text after 21.0, 20.7 and 23.9 seconds. Each turn changed the automatic recall block before conversation history. The engine reused 26,624 tokens but reprocessed 9,641–10,728 tokens, spending 14–15 seconds on prompt processing. A later conversational lookup encountered checklist-required before reading memory. Its preparation delay was 14 seconds; the old receipts did not identify that delay's internal stages.

## Implemented behavior

- `app/memory/context_snapshot.ep` saves a stable base per session and revision, the current selected records, and exact new quotations as a delta. A new selection does not rewrite the existing base. Source-aware claim comparison excludes already retained quotations. Original history is unchanged.
- `app/context/session_messages.ep` places each delta before its original turn and replays it in that position. Access, visibility, correction, runtime and compaction revisions invalidate both the base and obsolete deltas. Shared-group recall continues its existing reconstruction path; private-session reuse is not applied across audiences.
- `app/memory/automatic_context.ep` accepts the already selected records, keeping rendering separate from retrieval.
- `app/memory/recall_selection.ep` records fully indexed quotations in the derived `echo_recall_indexed` cache, keyed by embedding model fingerprint and exact quotation. It skips passage processing for completed entries. Failed indexing does not receive a completed marker. Visibility-filtered candidate selection remains in force.
- `app/agent/tool_dispatch.ep` allows direct, read-only memory/history lookups and scratchpad list/read when a parent conversation has no task checklist. Existing task, delegated-agent and mutation requirements remain enforced. Authorization checks still run first.
- `app/tools/continuity_tools.ep` clarifies that a scratchpad list call returns one page containing one note, and further pages should be requested only as needed.
- Snapshot receipts now retain `prepare_ms`, `context_ms` and `capture_ms` to distinguish preparation work from inference delays.

No token/context cap, memory deletion or reduction of reasoning was introduced. Compaction remains the existing mechanism for rebasing accumulated context.

## Verification

Native Ernos assertions passed for stable cross-turn recall, exact new additions, visibility invalidation, durable reopen and direct-read versus write gating. The final delta SQL was independently checked against mixed non-JSON history, preserving new claims without duplicating retained claims. Fixture output includes diagnostics for unavailable optional fixture records; the live checks below are separate evidence.

Live tests used compiled browser submission functions, the running HTTP service, actual memory and inference, archived tool results, and the session API. DOM doubles were used; this is not a visual-browser test claim. Every tested final reply was confirmed in the session API.

| Measurement | Before | Final verification |
| --- | ---: | ---: |
| Follow-up fresh prompt tokens | 9,641 | 1,315 |
| Follow-up prompt processing | 14.18s | 2.28s |
| Follow-up first visible text | 20.67s | 11.66s |
| Fresh session, warm model/cache: visible text | — | 7.69s |
| Fresh session, warm model/cache: completed | — | 9.14s |

The final build's first request after changing the tool schema required a cold prefix rebuild and took 47.94 seconds to finish. That is not hidden in the warm-cache measurements. Reply length and generated reasoning also affect total latency. The prior 14-second preparation delay was not reproduced: measured final capture stages took 3.06–3.80 seconds, including 1.43–2.08 seconds of indexing preparation and 1.62–1.72 seconds of context construction. These measurements do not establish that every future preparation delay is eliminated.

The final three-turn test retained an identical base hash throughout. New recall deltas were 2,009 and 2,228 characters. The lookup used one list page and one read, with no checklist-required response, task-checklist call, or mutation. Both operations returned found and the final answer reached the session API.

## Deployment and evidence

Release: `phase2-stable-session-recall-20261018-r2`.

Server SHA-256: `e61a7faea3a2a68910a61344e5f7b57234d22c1061fc12b2f42d61aedcb19d1b`.

Runtime receipts under `work/echo-runtime/maintenance`: `user-new-session-times-20261018.json`, `stable-session-final-live-times.json`, `stable-session-final-proof.json`, `stable-session-warm-new-session.json`, and `stable-session-idle.json`. These contain machine-local evidence, not a portable public test environment.

Echo is relaunched idle after verification; inference stays loaded to preserve the warmed cache. Original sessions, notes and memory records are retained.
