> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Selective automatic recall — 17 October 2026

## Defect and implementation

Every private user turn previously injected all eligible derived claims plus full pending originals and a session-ID catalog. A new greeting carried 108,438 input tokens, including 83,581 memory tokens. The earlier compact serialization reduced repeated metadata but did not select relevant material.

`app/memory/recall_selection.ep` derives lexical query terms from the current objective, removing common function words and greeting terms. `automatic_context.ep` automatically includes source-backed user preferences, corrections and relationships, plus claims matching those terms. Pending original messages enter automatic context only when they match. The context explicitly explains that selection is incomplete and exposes memory search, graph, original reads and session enumeration for additional detail. The full session catalog is replaced with its count. There is no arbitrary token or record cutoff, archive deletion or quotation truncation. Existing ownership, visibility, expiry, supersession and source-digest checks remain in the selection query. Within-run snapshots retain their existing stability; the selection revision invalidates old all-memory snapshots. Shared-group recall is unchanged.

This is lexical selection, not semantic embedding retrieval. Synonyms and ambiguous references may require memory tools; a selected set is not proof that unselected memories do not exist. Broad topic terms can select substantial context. Identity and tool schemas were not reduced or changed in this repair.

## Evidence

Native fixture `tests/security/test_selected_recall.ep` passed: greeting retains preference but not unrelated episode/pending original; topic retrieves exact episode; foreign records excluded; omitted source remains readable; frozen snapshot and archive integrity preserved. Fixture database: `/private/tmp/echo-selective-recall-r2-20261017.sqlite`.

`test_selected_recall_existing.ep` reconstructed recall from the real earlier run without changing its saved snapshot. Two further fresh sessions used compiled client send functions with isolated DOM/storage doubles, actual authenticated HTTP, actual recall and model, and checked final reply visibility through the session API. This was not a visual browser test.

Live greeting input: 32,709 tokens, including 7,852 memory tokens (previously 108,438 / 83,581). First fresh session: first model output 14.341 seconds, first reply text 29.552 seconds, final reply 34.369 seconds; it made a system_inspect tool call. Second fresh session: first model output 3.392 seconds, first reply text 14.001 seconds, final reply 17.460 seconds. Second request reused 32,193 tokens and processed 516 new tokens. These are warm-model observations with available prefix caches, not a claim about a fully cold model. Another user run began during validation, so this is not an isolated throughput benchmark.

Runtime receipts: `maintenance/selected-recall-live-results.json`, `selected-recall-deployment.json`, and `selected-recall-idle.json`. Original archives and complete memory claims remain available. Test runs and remaining active work were requested to stop for the user's idle handoff.

## Release

`phase2-selected-recall-20261017`, server SHA256 `cecc15860009248d56cf424b4417dc825938dbafe5b95087e5d73219d47efbff`. The approved identity wording is unchanged. No Phase Two scope expansion.
