> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Semantic passage recall — 17 October 2026

## Failure being replaced

The first selected-recall release admitted an entire quotation on any matching word. The real second conversational prompt selected 308 claims and produced 95,672 input tokens. Greeting-only validation did not cover this failure. This release removes lexical admission from automatic private recall.

## Runtime design

A local Nomic Embed Text v1.5 F16 model produces normalized semantic vectors through a CPU-only llama.cpp embedding service on loopback port 8081. It is separate from the chat inference engine; it does not invoke the large generative model for retrieval. Model file identity and endpoint are configured in `environments/memory-embedding.json`; service startup arguments are in `environments/embedding-service.json`. The model is stored on One Touch. The original publisher is [nomic-ai](https://huggingface.co/nomic-ai/nomic-embed-text-v1.5-GGUF). Model-required search_query and search_document prefixes distinguish query and document embeddings.

All application logic is Ernos: `recall_candidates.ep` applies the existing owner/visibility/expiry/supersession/source-digest gates; `recall_selection.ep` segments eligible claims into exact sentence/paragraph passages, caches their vectors by model hash and text digest, embeds the current objective, and ranks cosine similarity. One strongest passage represents each candidate claim. The high-relevance tail above the query's mean plus the configured standard-deviation threshold (currently two; bounded by the attainable score spread for a small candidate set) is selected; this is an adaptive relevance threshold, not a fixed item/token cap. Standing user preference, correction and relationship records remain available in full. This statistical threshold is an implementation choice, not a universal standard or a guarantee of semantic completeness.

The selected context contains explicitly labelled exact excerpts with original event IDs, dates and evidence status. These are extractive navigation summaries, not newly generated assertions or complete episodes. Original claims and archived messages remain unchanged and retrievable. Full pending transcripts and full session-ID catalogs are not injected. Unindexed originals remain accessible through existing memory tools. Shared-group recall is unchanged by this private-memory repair.

Indexing occurs before the context snapshot write transaction and is cached across runs. Frozen run snapshots skip repeat indexing. Permission and visibility revisions still force re-evaluation. Embedding failures produce a durable memory_retrieval_status event and a visible-to-model incompleteness notice; they do not fall back to injecting the entire archive. Standing memory and original retrieval tools remain usable. Cache rows are derived data, never authority to access their source; retrieval always joins freshly authorized candidate claims.

## Verification

Native fixture: a greeting excludes an unrelated quartz episode; the differently worded query “How heavy was the mineral specimen?” retrieves the quartz mass. Foreign records remain excluded, the omitted original remains readable, the snapshot remains stable, and the source archive verifies. Fixture database: `/private/tmp/echo-semantic-recall-20261017.sqlite`.

Actual-archive and deployed live timing receipts are recorded separately under runtime `maintenance/semantic-*`. This document is completed with observed results at handoff. No visual browser pass is implied by HTTP/client-function verification.

The first deployed live attempt encountered an empty-body HTTP 400 from input-token counting. The exact archived request counted successfully on replay and retry. The preflight now preserves a diagnostic receipt and retries that specific read-only failure once; nonempty semantic error responses are not retried. A local fault-injection server verified an identical-body retry and successful admission. Evidence: `/private/tmp/echo-token-count-retry-20261017.sqlite`. The first validation timings include manual recovery and are retained separately; they are not presented as clean latency measurements.


The final-build two-turn validation exposed a native SIGSEGV in `memory_semantic_prepare` on the second turn. The macOS crash report identified `get_list` during passage iteration. Inspection of generated code showed unrooted iteration lists. The application now uses explicit list indexing with retained list variables; compiler sources are unchanged. The failed validation receipt is retained as `semantic-recall-crash-validation.json`; it is not counted as a passing test. The corrected release requires the complete two-turn test again.


## Corrected deployment and live result — 18 October

Release `phase2-semantic-recall-r3-20261018`, server SHA256 `7968ab9207ae552d8c1235726c47b00aa11ff7ee5451031f91315f7148c26909`. Generated native code was inspected to confirm both iteration lists have retained roots. No compiler changes were made.

The complete two-turn live test passed through the compiled client send functions, authenticated HTTP, production recall, real local inference and the session API. DOM doubles were used; this was not a visual browser test. Both final messages were present in the session API. No worker recovery, token-count retry or incomplete-memory event occurred in this passing session.

- Greeting: 34,097 input tokens; first model output 4.68 seconds; visible text 5.58 seconds; complete 7.32 seconds. Warm prefix cache, not a cold-model benchmark.
- Same-session broad follow-up about three days and the relationship: initial input 33,838 tokens versus the earlier 95,672-token lexical request. First model output 17.45 seconds; visible reply text 52.94 seconds; complete 62.71 seconds. Echo performed three memory tool calls across three inference requests; the last request was 35,719 tokens. This remains a roughly one-minute tool-assisted answer, not a claim of instant replies.

Private receipt: runtime `maintenance/semantic-recall-live-results.json`. Passing runs: `006006ba-8048-4b61-a2a7-15495e9242df` and `f278bbcb-463f-4917-b027-78f96660a7c5`. Prior failed attempts remain documented. Source memories were not rewritten or deleted. The main model retains its full context capacity. The CPU embedding service remains part of the idle local runtime; reboot auto-start for that service has not been established by this repair.


## 18 October, 06:07 — fresh-session crash follow-up

The user's `Good morning echo` run `50389c04-44c2-4dc5-bda5-3085c0780b42` crashed before foreground inference. macOS report `server-2026-10-18-060734.ips` records SIGABRT: invalid free/realloc, `append_list` → `memory_recall_claims` → `memory_semantic_prepare`. The candidate argument-list literal evaluated `echo_runtime_binding` while the partially constructed list was not retained. The prior loop repair did not cover this distinct allocation path.

The app now evaluates the runtime binding before constructing an explicitly retained parameter list. All parameter-list literals in the new semantic selection module were similarly replaced with named lists and explicit append operations. Generated code was inspected for retained list roots. No compiler sources changed. Deployed release `phase2-semantic-recall-r4-20261018`, SHA256 `9cb6ae738931de4252381a5fb6fe12ec3bc99e40e0e2b092928c138c209c1a97`.

The original greeting and subsequent user turn completed on a retry before the new release was deployed; their replies are confirmed in the original session. Those completions alone are not validation of r4. Separate post-deployment compiled-client/HTTP/live-model regression receipts are in `maintenance/morning-recall-validation.json`.

The separately reported missing research results were investigated read-only. The mathematics and pre-Echo corpus agents (`51ed200c…:delegate:…` and `f22b12d7…:delegate:…`) had no final reports; both were cancelled at 23:21:48 BST on 17 October. Parent delegation-result events contain cancellation notices, and the parents were cancelled before delivering follow-ups. Their observations remain archived. This is not evidence of completed reports being delivered. The earlier CODATA diagnostic did have both a child final and a parent final. Cancelled research was not restarted during this repair.

Post-deployment r4 checks passed: fresh greeting completed in 18.13 seconds, follow-up in 23.20 seconds; both final events were verified through the session API. No worker recovery, agent failure or incomplete-retrieval event occurred in the diagnostic session. User conversation activity overlapped these checks, so these timings are not isolated speed benchmarks. Diagnostic runs were stopped after completion; the active user conversation was left running.
