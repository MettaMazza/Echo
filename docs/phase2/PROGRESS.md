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

Startup compatibility qualification: `app/lifecycle/startup.ep` now runs before server, supervisor, connection, worker and worker-owner archive writes. The native candidate rejected a future schema with exit 78 in all five roles; the database SHA256 stayed unchanged. A missing archive remains absent during preflight. Supervisor child exit 78 is terminal, preventing an incompatibility restart loop. Candidate build and tests passed; deployment remains pending full bundle backup and authority integration. Private evidence: `evidence/phase2/startup-roles.json`. Existing live service remained running.

Maria’s memory expansion is now explicit in the plan and ledger: episodic autobiographical chronology, semantic knowledge graph and procedural workflows across sessions. Implementation and live acceptance remain pending; step 09 orders ingestion, extraction/corrections, consolidation, procedure discovery and integration. My Brain remains excluded. Backup inspection found the offline bundle gate also rejects a saved failed run despite no active model/tool/process work; correction and recovery qualification are the next operational task.

Current-installation backup and independent restore passed on 16 October. Bundle includes database, registered attachments, application/tests, identity, configuration, workspaces, UI assets, running server/browser and configured provider/media profiles. The failed-run backup gate was corrected and native regression-tested; failed history remains failed and restore execution remains held. Original events/runs/tool ledger/devices/session ownership match the snapshot exactly. Weights and full dependency runtimes are not duplicated in this bundle; complete installer/recovery qualification remains step 26. The live service was restarted successfully with its existing executable and configuration.

Principal revocation foundation: migration 3 adds immutable revocation receipts; host revoke/restore commands atomically change principal access and record previous role/state. Restore returns ordinary-user access and cannot revive independently revoked devices/accounts. Native tests passed denial through device/account lookups, failed-write rollback, reopen, replay conflict and delayed replay after restoration. CLI migrate/revoke/restore passed in a disposable database. Live account linking and dispatch integration remain pending; no production roles or schema changed.

Maria’s prompt correction: the Codex-addressed handover was wrongly installed as continuity material and has now been removed from the live private bundle; inherited ErnosDecent prompt text is preserved. Deployed a targeted system-prefix correction eliminating the changing run ID before conversation history. Native same-provider cross-run prompt bytes match; live cold/repeat/appended-follow-up cache tests passed. This targeted build excludes unfinished dispatch changes. Earlier conversation history remains preserved.

Dispatch integration qualification: migrated principal revocation now gates browser authentication, session/run and workspace access, queued tool dispatch, and image/speech authority monitors. A real queued workspace file action was denied with an archived no-effects result and cancelled run; restoration did not replay it. Existing two-turn HTTP/tool and device isolation suites pass both before and after migration. An isolated native HTTP server returned 200 for two users, 403 only for the revoked user, and 200 after ordinary-user restoration. Production remains on the targeted prompt-cache build pending complete capability/host-enrolment deployment. Account linking and privileged capability policy are not yet complete.
