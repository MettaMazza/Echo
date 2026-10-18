# Saved candidate discovery

Current deployment: these tools are included in the deployed schema-26 server (`phase2-git-20261016-233505`). Earlier statements below about non-deployment describe their original qualification date; they are superseded by the research release and subsequent release manifests. Current configured runtime dependencies and source hashes were checked during the Step 08 reconciliation.

Step 08 implementation, 16 October 2026. `tool_candidates` searches immutable saved manifests by name/purpose and pages versions by sequence. It is advertised to current admins only when the registry exists. Private retrieval follows the same principal and current source access; shared calls see only their original shared session. Source paths and privileged candidate descriptions are not exposed to guests or other principals.

Results include the original manifest text, its SHA-256, parsed manifest, source run and next cursor. The original text preserves whitespace/bytes through JSON serialization. Each result verifies digest, manifest structure and indexed name/version before returning. Candidate records are explicitly `runnable: false` and do not establish qualification. Dependencies and schemas can inform reuse/composition inspection, but no code is loaded or executed by this operation.

Native streaming dispatch passed against candidates saved by the registry regression. Checks cover cross-session retrieval, case-insensitive search, version paging, exact text/digest, foreign and shared/private isolation, reopen, corruption refusal and revocation. The deliberately corrupted fixture row was restored to its original bytes and its digest rechecked. Evidence: `evidence/phase2/candidate-discovery-20261016/verification.json`.

This supplies saved-candidate discovery. Full discover/compose-before-create enforcement, qualified registration, loading and invocation remain at their original make-tool steps. It does not relabel unqualified candidates as approved dormant tools. Production has not been updated with this tool.
