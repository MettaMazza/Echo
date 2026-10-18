# Correction precedence — 18 October 2026

## Incident

The original supersedes field excluded only its named graph assertion from automatic context. An episodic duplicate from the same source remained eligible, and original-message tools returned contaminated text without the later correction. The Corvus/Dewthread history demonstrated the gap: an imported family-history account was corrected explicitly by the user, but remained retrievable independently of that correction.

## Implementation

`app/memory/corrections.ep` resolves original-source → correcting-source links from existing graph supersession and immutable `memory_source_correction` archive annotations. Both source digests are checked. Explicit annotations require a later original user correction and access to both source events. Original event bytes, digests, timestamps and roles are never rewritten. Annotation events are appended through the normal chained archive writer.

Automatic private recall quarantines all derivatives of a corrected source, including duplicate episodic records, and always supplies the scoped correction ledger independently of semantic relevance ranking. Snapshot revision includes the ledger, so a newly recorded correction invalidates frozen recall. This deliberately quarantines the source as a whole: unaffected details remain readable in the original, but are not silently re-admitted from that source as new graph claims.

`memory_record` rejects new claims sourced from a corrected original. `memory_search`, `memory_read`, `memory_messages`, `memory_graph` and dispatched `history_search`/`history_read` attach `source_status: corrected-history` plus correcting event, digest and exact text. The original text is still returned unmodified. Transitive corrections are included chronologically so the latest correction takes precedence. If the correction lookup fails, original-reading wrappers return a lookup failure instead of unqualified historical text.

Private and linked-account authorization applies to both endpoints; a correction does not grant access to another person's records. This repair covers the private/linked memory path involved in the incident; it does not redesign group-memory extraction. It enforces recorded correction relationships, not an infallible semantic detector for every contradiction in arbitrary prose. Supersession recorded through the existing memory_record tool is picked up automatically; previously unlinked contaminated sources can be repaired with the scoped native annotation helper.

## Historical repair

Five verified pre-correction messages were linked to the existing user correction `23931f2b-744a-484d-82a1-b332c2db1895:user`: the story document, family-history letter, and three Echo replies repeating or defending those names. The private before/after receipt is `maintenance/correction-originals-before.json`. All five original payloads and digests, and the correction itself, were compared unchanged. No fabricated replacement history or derived-memory backfill was used.

## Verification

`tests/security/test_correction_precedence.ep` exercises supersession, duplicate-source exclusion, rejection of relearning from a corrected source, scoped annotation, unchanged original payload/digest, foreign-user denial and a later correction in the chain. `tests/security/record_source_correction.ep` is an operator maintenance command, not an unscoped public tool.

The first deployed live regression used the compiled browser submission functions, authenticated HTTP, real memory tools, local model inference, and the session API. Both requested original reads carried corrected-history metadata. Echo answered that Corvus and Dewthread were not real predecessors and cited the correcting event. The final response was visible through the session API. This was not a visual browser test. Private receipt: `maintenance/correction-live-results.json`; final release verification is recorded below.

Final native regression database: `/private/tmp/echo-correction-chain-20261018.sqlite`; passed including rejection of relearning and the transitive 73 → 31 → 42 gram correction. Deployment: `phase2-correction-precedence-r2-20261018`, server SHA256 `5be484e30ff718e99b435409e105d75b5ad9617885523a73b163b722636c88fd`. Browser assets, model weights and context capacity are unchanged.

Final-build live check passed: run `80c0a00f-4c3a-4f85-b418-7824e4418cec`; both original memory reads returned corrected-history and the correct correction event; Echo rejected the false lineage and cited `23931f2b-744a-484d-82a1-b332c2db1895:user`. The final response was verified in the session API. All six checked original/correction payloads and digests remained unchanged. Evidence: runtime `maintenance/correction-verification.json`, `correction-deployment.json`, and `correction-live-results.json`. Diagnostic processing was stopped after the final reply; user work was not cancelled.
