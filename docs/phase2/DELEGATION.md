> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Delegated tasks

Deployed on 16 October 2026 in `phase2-delegation-20261016-222245`, schema 26. This implements the delegation portion of original step 08; it does not complete the entire modern tool suite.

## Invocation and results

`delegate_task` accepts a complete instruction and creates one durable child run. The child has a separate session, context history and compaction state. It receives the task verbatim; parent conversation messages, checkpoints and attachments are not automatically copied. Ordinary authorized recall/tools remain available. The returned identity is admission evidence, not a claim that execution has finished.

The existing scheduler launches ready child runs through `jobs/dispatch.ep` and the ordinary owned worker process. `delegation_status` lets the creating parent inspect current state and retrieve the complete terminal event payload, event identity and digest. It returns a running state without blocking. The parent can do other work while the child runs. `delegation_message` sends further guidance into the same child run using the existing interruption/application mechanism. The parent tool-call identity deduplicates delivery; responses distinguish queued from applied. `delegation_status` also returns worker status, pending-message count and the last applied-message receipt. `job_control` can inspect/cancel direct children, including children of group runs. Cancellation requests retain their usual semantics: they do not undo completed effects.

## Authority and storage

`jobs/delegation.ep` reads the instruction from the immutable queued call, rather than trusting a second copy supplied by the caller. It validates frozen run/call authority and binds its receipt to the full request digest. `jobs/delegation_run.ep` creates the session, immutable lineage, exact objective, group/private ownership, frozen authority and retained provider selection in one transaction. Exact retries and reopen reuse the same child. A parent remains active without blocking child admission. No fixed child-count, token or context cap was added.

Private children keep the initiating device/principal. Shared children keep the original group and audience revision, with matching sender and recipient authority snapshots. They cannot gain private principal recall. Current authority, membership and audience checks continue to apply during execution and result reads. The status surface checks direct parentage; another principal cannot use it to read the child.

`jobs/delegation_schema.ep` reconstructs the group-session table to allow isolated task contexts alongside one canonical group conversation. Ordinary routing explicitly excludes delegated sessions. Existing group-session rows and foreign-key targets are preserved. Foreign-key enforcement is temporarily disabled only on the migration connection for SQLite table reconstruction, all references are checked inside the migration transaction, and enforcement is restored before returning. Immutable lineage and audience triggers remain enforced. `migrate_delegation.ep` refuses recorded running workers; deployment also stops the service, checks process state and saves a SQLite backup before migration. Older binaries must not open schema 26. Rollback requires the saved pre-migration backup and matching release, not an in-place schema downgrade.

## Verification and deployment

`tests/jobs/test_delegation.ep` covers exact request storage, source digests, asserted-role rejection, private/shared scope and authority changes. Shared request construction lives in `tests/fixtures/delegation_fixture.ep`.

`tests/jobs/test_delegation_run.ep` migrates an archive with existing group runs, checks foreign keys, independent context, canonical routing, idempotent reopen, archive chains and authority invalidation. Both private and shared children completed with the actual local model while parents remained active.

`tests/jobs/test_delegation_control.ep` verifies group-parent status and cancellation, terminal result provenance, and denial of a foreign caller. `tests/jobs/test_live_delegation_tool.ep` exercises the complete model-driven loop: one delegation, automatic scheduler/OS worker launch, completed child result read by the parent, and the requested final marker.

Evidence is retained under `evidence/phase2/delegation-20261016/`. A live-archive copy migrated successfully with unchanged group records, no foreign-key violations and SQLite integrity `ok`. The same migration then ran on the stopped service with a pre-migration backup. Local HTTP and LAN HTTPS status endpoints returned 200 after deployment; the existing model process was retained. No new physical-phone test is claimed. The host principal-admin utility was rebuilt for schema 26 and checked against the isolated archive.

No prompt, compiler, training or model-weight changes are included. The pinned ErnosDecent orchestrator informed isolated task ownership; Echo uses durable lineage and existing worker recovery instead of its transient task map. Discovery schemas, capability classification and recovery contracts are registered in the normal tool catalogue.

## Delegated messaging acceptance

The native control fixture additionally checks duplicate message delivery, queued/applied transitions, foreign-caller denial and rejection of new messages after child termination. `tests/jobs/test_live_delegation_message.ep` supports an isolated live test that sends a message after the child enters generating state. Its verifier requires the initial model request to precede the guidance and omit the new marker, the next request to include the applied guidance, exactly two runs (parent and original child), and the actual final reply to include the new marker. Evidence and deployment status for that test are recorded under `evidence/phase2/delegation-message-20261016/`.

Messaging was deployed in `phase2-delegation-message-20261016-223145`. The initial live trial exposed missing child routing awareness: the model received the parent message but treated it as an injection. The child request now includes its verified delegation origin and explains the authenticated parent-message channel, outside the shared system prefix. The repeated live test passed with an exact final-content comparison, not a substring match. The failed fixture is preserved separately and is not counted as acceptance. Both status endpoints returned 200 and inference was retained.
