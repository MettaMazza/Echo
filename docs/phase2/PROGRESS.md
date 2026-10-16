# Phase Two implementation progress

Authorised 16 October 2026. Goal remains active. No Phase Two feature is complete merely because it is planned or compiles.

## Current work

- Step 01: source/runtime identities captured privately; requirement ledger created. A29 (Ernos self-hosting) is explicitly included. Existing acceptance evidence remains valid within its recorded scope and needs mapping to each ledger row.
- Step 02: inspecting ErnosDecent rights/extension/training controllers and HIVE routing. Admin ID is supplied privately; Maria authorised PC credential setup and disposable test channels. Remaining continuity sources are on GitHub; My Brain is explicitly excluded.
- Step 03: native verified archive snapshot passed; restoration to separate internal storage passed native verification before schema work. A database snapshot is not a complete attachment/configuration backup.

## External inputs

Discord server/application/admin IDs and rotated token location; disposable Discord test area; Gmail OAuth and authorised test recipient; external-provider qualification accounts; missing continuity archive locations. No credentials belong in this ledger.

Proposed acceptance workload: two simultaneous full-context users and four hours of mixed tasks. Maria delegated workload selection. Use this as the initial workload, extending investigation if resource growth or instability appears; no artificial context reduction is permitted. Cold/import latency is measured separately from cached follow-ups.

## Verified foundation

The native archive snapshot and clean internal restoration passed; all table rows matched. `tests/lifecycle/test_migrations.ep` passed against a fresh database, verifying transactional DDL rollback, idempotent replay, changed SQL rejection, ordering, immutable receipts, reopening and incompatible downgrade checks. Evidence: private `evidence/phase2/foundation-verification.json` and build log. Production is unchanged. Native SIGKILL-during-migration recovery and durable operation journal tests now pass. Startup integration and full attachment/configuration restore remain required before step 03 closes.

## Current implementation evidence

- `tests/lifecycle/test_migration_interruption.ep`: the driver observed a nonempty rollback journal, killed the native migration with SIGKILL, reopened the database and verified no partial DDL/data/receipt remained; original archive hashes verified and a later clean migration succeeded.
- `tests/lifecycle/test_operations.ep`: scoped durable identity, exact reservation replay, conflicting payload rejection, one execution claim, reopen, uncertain-effect reconciliation, failed outcome-write rollback and immutable receipts passed. Real connector calls will use this journal; it does not yet prove Discord or Gmail delivery.
- `tests/security/test_principals.ep`: existing and newly paired devices retain separate default-user identities; names do not grant authority; host role changes and their audit records commit atomically; conflicting replay and revoked device resolution are rejected. No session ownership was moved.
- `app/principal_admin.ep`: native host command built and granted/inspected admin only in the disposable fixture database. There is no HTTP role-grant route. Production has not yet been migrated or enrolled.
- GitHub metadata now confirms Echo is public. Publication is tracked by exact local/remote tree identity; final candidate publication remains step 31.

## Next action

Qualify startup version guards and complete backup bundle; integrate principals and explicit host enrolment with the live UI; then enforce capabilities at every dispatch boundary. Discord account bindings, reversible device linking, principal revocation and protected-change authority still require implementation/testing before step 04 closes.


Restored real-archive qualification: all 21 original tables remained byte-value equivalent after foundation migration; seven devices mapped to seven distinct users, with no implicit admin. The explicit host migration command passed on that copy.
