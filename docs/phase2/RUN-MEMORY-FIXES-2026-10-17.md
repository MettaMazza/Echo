# Cross-turn delegation and stable run memory — 17 October 2026

## Observed failures

A subsequent turn's `delegation_status` returned `not-found` for a live child created by the preceding turn. The query required the current run to be the original parent. `job_control` could see the child, which produced contradictory observations.

Automatic extraction changed the recalled memory before a delegated task's next tool step. The archived requests showed the memory message changing from 349,267 to 349,143 characters. Independently, a memory extraction answered an instruction inside its source record with prose instead of extraction JSON, producing `memory-invalid-json`. The large delegated task and its attachment read were legitimate workload, not defects.

## Implemented resolution

`delegation_status` and `delegation_message` accept a child created by an earlier turn in the same conversation with the same initiating principal. Tool authorization, child authority and audience checks remain in force. Guidance uses the current caller's authenticated device. Tool descriptions reflect the cross-turn behavior. Other conversations and other principals cannot use this continuation path.

`app/memory/context_snapshot.ep` captures recalled bytes once per run and authorization/runtime revision. The immutable archive record is `memory_context_snapshot`, with the run ID, revision and content covered by the ordinary archive digest. Tool steps and process restarts reuse it. A new run captures fresh memory. Original conversations and explicit memory tools remain available; no context or attachment limit was introduced.

Snapshot capture holds a short SQLite transaction, released before inference. Visibility changes, principal/device changes, account-link changes, group membership changes, corrections, expirations and runtime changes invalidate snapshots. Reuse rechecks these revisions under the caller's database snapshot. Invalidation is conservative: unrelated access changes can also cause a refresh. The seven production revision queries together measured approximately 1 ms in this verification.

Automatic extraction now requests strict `response_format: json_schema`. The schema requires a memories array and complete source, kind, category, quotation and temporal fields. Existing source authorization and exact-quotation validation still run before saving. Invalid output leaves original records pending and does not mark them indexed.

## Verification

- `tests/jobs/test_delegation_cross_turn.ep`: later-turn inspection and guidance; queued/applied delivery; private and shared conversations; cancellation and terminal provenance; unrelated conversation/foreign principal rejection; device revocation.
- `tests/security/test_memory_run_snapshot.ep`: byte-identical recall after extraction and database reopen; refreshed next turn; immediate forgetting and revocation; memory failure preserves the earlier prompt prefix; malformed extraction does not consume pending originals; schema present in extraction request.
- Replayed the actual failed extraction request against the production local model with the new format. It returned valid JSON with an empty memories array and a normal stop in 1.24 seconds. This confirms format compliance for that input, not that every future extraction is semantically correct.
- PASS: live extraction and recall verification used `tests/security/test_live_memory_cycle.ep` against the local inference server with an isolated archive.

The live service is relaunched only after build and regression checks. User runs are cancelled through the existing cancellation path; original messages are preserved. Broader Phase Two development remains paused for manual testing.

## Deployed and paused

Release: `phase2-run-memory-fixes-20261017-090448`. Deployed server SHA-256: `3914f893f9510a3b1ae82e044019072ba2f1dc355275200b83ac0e20564633e3`, matching the verified build. Local page, JavaScript and LAN HTTPS page returned HTTP 200. Post-deployment: zero active runs, zero active workers, all four inference slots idle. Test and compiler processes exited. Only the normal Echo services remain running. Evidence is stored locally under `evidence/phase2/run-memory-fixes-20261017/`; private extraction inputs are not published.
