# Automatic private memory

The original recall incident was a cursor error, not missing conversations. The model read the first result, then passed the scan boundary as the next cursor and skipped intervening messages. `memory_search` now returns a complete `next_search` argument object. `through` remains a frozen upper boundary. A single result is not an exhaustive search.

## Runtime flow

Before a private run begins, `app/memory/automatic.ep` finds unindexed accepted user messages and completed assistant responses in the same principal's other conversations. Current-session history already enters ordinary context, so it is not separately extracted on every turn. This avoids evicting the single-slot conversation cache for a redundant extraction request. Opening another conversation processes the previously active conversation's new material.

The selected local model extracts exact source quotations into semantic and episodic claims. Categories include facts, preferences, corrections, relationships, milestones, experiences, workflows and commitments. Workflow-category semantic claims carry procedural memory; this does not train weights or establish that a workflow has been mastered. The response synthesises these recalled records alongside the current conversation.

Extraction uses actual provider preflight capacity. A batch that exceeds capacity is subdivided; no fixed message-count or token cap is used. An individual source that cannot fit returns a recorded capacity failure rather than silently cutting it. Llama.cpp extraction disables the optional thinking pass for this structured selection task; normal conversational reasoning is unchanged.

Every stored quotation must occur in its named original source, and every source must belong to the initiating principal. If a model abbreviates or paraphrases a selected quote, the runtime retains the entire original message as one episodic source record instead; repeated fallbacks deduplicate. Invented source references are rejected and counted in the extraction receipt. If every proposed item has an invalid reference, the batch fails and remains eligible for retry. All retained items are validated before any are written. User quotations are source accounts; assistant quotations are inferences or commitments, never independent verification. The existing content-addressed knowledge graph makes retries idempotent. Immutable `memory_indexed` archive receipts identify processed events and digests. Originals are retained.

`app/memory/automatic_context.ep` automatically adds the visible graph and a private session index to context. The index omits titles because titles can repeat a forgotten assertion. Recall respects source ownership, revocation, source digests, supersession, expiry and individual-assertion visibility. Private memory is not injected into group runs. Historical content remains contextual data, not a system instruction or authority grant.

A first import can take time. `memory_stage` drives a readable progress message through the existing run activity UI. Failure records `memory_failure` with a recovery action; existing memories and source tools remain available so a failed extraction does not disable ordinary chat. Unindexed history is retried on a subsequent turn. An incomplete extraction is explicitly described in the model's context.

## Behavioural correction

`app/context/system_awareness.ep` includes the operator-requested conversation guidance: empathy and mirroring are not defects; do not manufacture disagreement or allegiance, invent motives, turn personal history into unsolicited review, or repeat elaborate confessions after correction. Requested critiques must account for the supplied material. The inherited identity and philosophical sections remain intact. Only obsolete technical memory descriptions are updated in the private runtime files.

This is a prompt/runtime change, not a claim that model weights were changed or that a particular training-data origin has been proved.

## Verification and recovery

`tests/security/test_memory_tools.ep` exercises the returned search continuation through the actual dispatcher. `tests/security/test_automatic_memory.ep` covers automatic context inclusion, exact-quote validation, ownership isolation, idempotence, restart, forgotten-claim exclusion and foreign-source receipt rejection. `tests/security/test_live_automatic_memory.ep` exercises extraction and a fresh response using the local model on an isolated archive copy.

`app/import_memory.ep` can backfill an already generated private extraction without rerunning inference. It accepts existing source IDs and the extraction JSON, and applies the same original-source, quotation and ownership checks. It cannot import SQL, replace source history or move another principal's memory.

Raw conversations, extraction requests, test archives, source lists and private prompt bundles stay in ignored local evidence/storage. Public documentation contains the mechanism and scoped verification results, not the conversations.

## Deployment qualification — 16 October 2026

The schema10 production service now runs `server_phase2_memory`. A consistent pre-change archive and configuration backup is retained locally. The existing owner's history was backfilled with 28 records from 180 source events. Only two repeated technical memory descriptions changed in the private identity bundle; the remaining bytes, including philosophical and identity wording, are unchanged.

The fresh real-history conversation recalled the owner and earlier corrections. Its follow-up reused 27,246 cached tokens and processed 29 new prompt tokens in 293 ms, followed by 1,582 ms generation. A separate local-model cycle passed identity, shared-experience and workflow extraction/recall. `tests/web/test_live_memory_server.ep` then passed against the actual production HTTP service, including automatic extraction, new-session recall, foreign-device denial and credential cleanup. Both loopback and Wi-Fi HTTPS returned 200. These are observed runs, not a promise of identical latency or perfect behaviour on every future response.
