-- Schema only from isolated 1.75 validation archive, not production. Observed 2026-10-20T12:14:17.710799+00:00; no private rows.

CREATE INDEX echo_events_kind_sequence ON echo_events(kind,sequence);

CREATE INDEX echo_events_session ON echo_events(session,sequence);

CREATE INDEX echo_memory_object ON echo_memory_claims(principal,object,sequence);

CREATE UNIQUE INDEX echo_memory_single_successor ON echo_memory_claims(supersedes) WHERE supersedes IS NOT NULL;

CREATE INDEX echo_memory_subject ON echo_memory_claims(principal,subject,predicate,sequence);

CREATE INDEX echo_memory_visibility_claim ON echo_memory_visibility(claim,sequence);

CREATE UNIQUE INDEX echo_one_operation_per_proposal ON echo_events(json_extract(payload,'$.proposal')) WHERE kind='protected_operation_prepared';

CREATE INDEX echo_operations_scope ON echo_operations(scope,identity,revision);

CREATE INDEX echo_protected_proposals_principal ON echo_protected_proposals(principal,identity);

CREATE TABLE echo_account_link_events(identity TEXT PRIMARY KEY,platform TEXT NOT NULL,subject TEXT NOT NULL,previous_principal TEXT NOT NULL,previous_revoked INTEGER NOT NULL,principal TEXT NOT NULL REFERENCES echo_principals(identity),revoked INTEGER NOT NULL CHECK(revoked IN (0,1)),actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_active_context (session TEXT PRIMARY KEY, checkpoint TEXT NOT NULL REFERENCES echo_checkpoints(identity));

CREATE TABLE echo_attachment_derivations(attachment TEXT PRIMARY KEY REFERENCES echo_attachments(attachment),parent TEXT NOT NULL REFERENCES echo_attachments(attachment),event TEXT NOT NULL UNIQUE REFERENCES echo_events(event),CHECK(attachment<>parent));

CREATE TABLE echo_attachments(attachment TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_sessions(session),path TEXT NOT NULL UNIQUE,name TEXT NOT NULL,media_type TEXT NOT NULL,digest TEXT NOT NULL,bytes INTEGER NOT NULL CHECK(bytes>=0),event TEXT NOT NULL UNIQUE REFERENCES echo_events(event));

CREATE TABLE echo_authority_events(identity TEXT PRIMARY KEY,principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_role TEXT NOT NULL,role TEXT NOT NULL,actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_cancel_requests(run TEXT PRIMARY KEY REFERENCES echo_runs(run),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_checkpoints (identity TEXT PRIMARY KEY, session TEXT NOT NULL, source_end TEXT NOT NULL REFERENCES echo_events(event), source_digest TEXT NOT NULL, previous TEXT NOT NULL, summary TEXT NOT NULL, structured_state TEXT NOT NULL, validation TEXT NOT NULL);

CREATE TABLE echo_conversation_accounts(device TEXT PRIMARY KEY REFERENCES echo_devices(device),account TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_conversation_merges(identity INTEGER PRIMARY KEY AUTOINCREMENT,source TEXT NOT NULL,target TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);

CREATE TABLE echo_delegations(child TEXT PRIMARY KEY REFERENCES echo_runs(run) DEFERRABLE INITIALLY DEFERRED,parent TEXT NOT NULL REFERENCES echo_runs(run),origin_call TEXT NOT NULL,session TEXT NOT NULL UNIQUE REFERENCES echo_sessions(session) DEFERRABLE INITIALLY DEFERRED,request_event TEXT NOT NULL REFERENCES echo_events(event),UNIQUE(parent,origin_call),FOREIGN KEY(parent,origin_call) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_device_link_codes(digest TEXT PRIMARY KEY,account TEXT NOT NULL REFERENCES echo_devices(device),expires INTEGER NOT NULL,consumed INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_device_link_events(identity TEXT PRIMARY KEY,device TEXT NOT NULL REFERENCES echo_devices(device),previous_principal TEXT NOT NULL REFERENCES echo_principals(identity),principal TEXT NOT NULL REFERENCES echo_principals(identity),actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_devices(device TEXT PRIMARY KEY,name TEXT NOT NULL,credential_hash TEXT NOT NULL UNIQUE,revoked INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_events (sequence INTEGER PRIMARY KEY AUTOINCREMENT, session TEXT NOT NULL, event TEXT NOT NULL UNIQUE, kind TEXT NOT NULL, payload TEXT NOT NULL, occurred TEXT NOT NULL, previous TEXT NOT NULL, digest TEXT NOT NULL);

CREATE TABLE echo_group_audiences(group_id TEXT NOT NULL REFERENCES echo_groups(identity),revision INTEGER NOT NULL CHECK(revision>0),principal TEXT NOT NULL REFERENCES echo_principals(identity),PRIMARY KEY(group_id,revision,principal));

CREATE TABLE echo_group_membership(identity TEXT PRIMARY KEY,group_id TEXT NOT NULL REFERENCES echo_groups(identity),revision INTEGER NOT NULL CHECK(revision>0),principal TEXT NOT NULL REFERENCES echo_principals(identity),actor TEXT NOT NULL REFERENCES echo_principals(identity),state TEXT NOT NULL CHECK(state IN ('invited','joined','left','removed')),UNIQUE(group_id,revision));

CREATE TABLE echo_group_run_origins(run TEXT PRIMARY KEY REFERENCES echo_runs(run) DEFERRABLE INITIALLY DEFERRED,session TEXT NOT NULL REFERENCES echo_group_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_group_run_recipients(run TEXT NOT NULL REFERENCES echo_runs(run),principal TEXT NOT NULL REFERENCES echo_principals(identity),role TEXT NOT NULL,role_revision INTEGER NOT NULL,revocation_revision INTEGER NOT NULL,PRIMARY KEY(run,principal));

CREATE TABLE echo_group_sessions(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),group_id TEXT NOT NULL REFERENCES echo_groups(identity),revision INTEGER NOT NULL CHECK(revision>0));

CREATE TABLE echo_group_speech_jobs(job TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_group_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device),source TEXT NOT NULL REFERENCES echo_events(event));

CREATE TABLE echo_groups(identity TEXT PRIMARY KEY,creator TEXT NOT NULL REFERENCES echo_principals(identity),platform TEXT NOT NULL,server TEXT NOT NULL,channel TEXT NOT NULL,origin TEXT NOT NULL,UNIQUE(platform,server,channel,origin));

CREATE TABLE echo_linked_browsers(identity TEXT PRIMARY KEY,account TEXT NOT NULL REFERENCES echo_devices(device),name TEXT NOT NULL,credential_hash TEXT NOT NULL UNIQUE,revoked INTEGER NOT NULL DEFAULT 0,created TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);

CREATE TABLE echo_memory_claims(sequence INTEGER PRIMARY KEY AUTOINCREMENT,identity TEXT NOT NULL UNIQUE,principal TEXT NOT NULL REFERENCES echo_principals(identity),kind TEXT NOT NULL CHECK(kind IN ('semantic','episodic')),subject TEXT NOT NULL REFERENCES echo_memory_nodes(identity),predicate TEXT NOT NULL,object TEXT NOT NULL REFERENCES echo_memory_nodes(identity),evidence TEXT NOT NULL CHECK(evidence IN ('source-account','inference','tool-observation')),source_event TEXT NOT NULL REFERENCES echo_events(event),source_digest TEXT NOT NULL,event_time TEXT NOT NULL,valid_until TEXT NOT NULL,supersedes TEXT REFERENCES echo_memory_claims(identity),recorded_at TEXT NOT NULL,canonical TEXT NOT NULL);

CREATE TABLE echo_memory_nodes(identity TEXT PRIMARY KEY,principal TEXT NOT NULL REFERENCES echo_principals(identity),name TEXT NOT NULL,UNIQUE(principal,name));

CREATE TABLE echo_memory_visibility(sequence INTEGER PRIMARY KEY AUTOINCREMENT,identity TEXT NOT NULL UNIQUE,claim TEXT NOT NULL REFERENCES echo_memory_claims(identity),principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_revision INTEGER NOT NULL CHECK(previous_revision>=0),state TEXT NOT NULL CHECK(state IN ('visible','forgotten')),reason TEXT NOT NULL,run TEXT NOT NULL REFERENCES echo_runs(run),recorded_at TEXT NOT NULL);

CREATE TABLE echo_node_run_epochs(run TEXT PRIMARY KEY REFERENCES echo_runs(run),epoch INTEGER NOT NULL);

CREATE TABLE echo_node_tool_epochs(run TEXT NOT NULL,call_id TEXT NOT NULL,epoch INTEGER NOT NULL,PRIMARY KEY(run,call_id),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_operations (identity TEXT NOT NULL, scope TEXT NOT NULL, principal TEXT NOT NULL, kind TEXT NOT NULL, request_digest TEXT NOT NULL, revision INTEGER NOT NULL CHECK(revision>=0), state TEXT NOT NULL CHECK(state IN ('queued','running','uncertain','succeeded','failed')), stage TEXT NOT NULL, cause TEXT NOT NULL, effects TEXT NOT NULL CHECK(effects IN ('none','unknown','confirmed')), diagnostic TEXT NOT NULL, recovery TEXT NOT NULL, result TEXT NOT NULL, occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')), PRIMARY KEY(identity,revision));

CREATE TABLE echo_pairing(invitation_hash TEXT PRIMARY KEY,consumed INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_principal_accounts(platform TEXT NOT NULL,subject TEXT NOT NULL,principal TEXT NOT NULL REFERENCES echo_principals(identity),revoked INTEGER NOT NULL DEFAULT 0 CHECK(revoked IN (0,1)),PRIMARY KEY(platform,subject));

CREATE TABLE echo_principal_devices(device TEXT PRIMARY KEY REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity));

CREATE TABLE echo_principal_revocations(identity TEXT PRIMARY KEY,principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_revoked INTEGER NOT NULL,revoked INTEGER NOT NULL CHECK(revoked IN (0,1)),previous_role TEXT NOT NULL,role TEXT NOT NULL,actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_principals(identity TEXT PRIMARY KEY,role TEXT NOT NULL DEFAULT 'user' CHECK(role IN ('user','admin','steward')),revoked INTEGER NOT NULL DEFAULT 0 CHECK(revoked IN (0,1)));

CREATE TABLE echo_protected_proposals(identity TEXT PRIMARY KEY,run TEXT NOT NULL REFERENCES echo_runs(run),principal TEXT NOT NULL REFERENCES echo_principals(identity),manifest TEXT NOT NULL,manifest_digest TEXT NOT NULL,created TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_protected_reviews(sequence INTEGER PRIMARY KEY AUTOINCREMENT,proposal TEXT NOT NULL REFERENCES echo_protected_proposals(identity),run TEXT NOT NULL,call_id TEXT NOT NULL,principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_revision INTEGER NOT NULL CHECK(previous_revision>=0),inspection_event TEXT REFERENCES echo_events(event),review_event TEXT NOT NULL UNIQUE REFERENCES echo_events(event),model_request TEXT NOT NULL REFERENCES echo_events(event),evidence TEXT NOT NULL,evidence_digest TEXT NOT NULL,created TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')),UNIQUE(run,call_id),UNIQUE(proposal,previous_revision),CHECK(inspection_event IS NOT NULL OR (json_valid(evidence) AND COALESCE(json_extract(evidence,'$.decision'),'') IN ('reject','counter','object'))),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE "echo_protected_reviews_pre16"(sequence INTEGER PRIMARY KEY AUTOINCREMENT,proposal TEXT NOT NULL REFERENCES echo_protected_proposals(identity),run TEXT NOT NULL,call_id TEXT NOT NULL,principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_revision INTEGER NOT NULL CHECK(previous_revision>=0),inspection_event TEXT NOT NULL REFERENCES echo_events(event),review_event TEXT NOT NULL UNIQUE REFERENCES echo_events(event),model_request TEXT NOT NULL REFERENCES echo_events(event),evidence TEXT NOT NULL,evidence_digest TEXT NOT NULL,created TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')),UNIQUE(run,call_id),UNIQUE(proposal,previous_revision),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_protected_steward(sequence INTEGER PRIMARY KEY AUTOINCREMENT,identity TEXT NOT NULL UNIQUE,proposal TEXT NOT NULL REFERENCES echo_protected_proposals(identity),manifest_digest TEXT NOT NULL,review_revision INTEGER NOT NULL CHECK(review_revision>=0),previous_revision INTEGER NOT NULL CHECK(previous_revision>=0),device TEXT NOT NULL REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity),role_revision INTEGER NOT NULL,revocation_revision INTEGER NOT NULL,link_revision INTEGER NOT NULL,decision TEXT NOT NULL CHECK(decision IN ('authorize','withdraw','reject')),reason TEXT NOT NULL,receipt_digest TEXT NOT NULL,created TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')),UNIQUE(proposal,previous_revision));

CREATE TABLE echo_provider_default(singleton INTEGER PRIMARY KEY CHECK(singleton=1),identity TEXT NOT NULL REFERENCES echo_provider_profiles(identity));

CREATE TABLE echo_provider_profiles(identity TEXT PRIMARY KEY,label TEXT NOT NULL,profile TEXT NOT NULL);

CREATE TABLE echo_provider_retirements(identity TEXT PRIMARY KEY REFERENCES echo_provider_profiles(identity),reason TEXT NOT NULL);

CREATE TABLE echo_run_authority(run TEXT PRIMARY KEY REFERENCES echo_runs(run),device TEXT NOT NULL REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity),role TEXT NOT NULL,role_revision INTEGER NOT NULL,revocation_revision INTEGER NOT NULL,link_revision INTEGER NOT NULL);

CREATE TABLE echo_runs (run TEXT PRIMARY KEY, session TEXT NOT NULL, objective_event TEXT NOT NULL REFERENCES echo_events(event), state TEXT NOT NULL CHECK(state IN ('ready','generating','tools','waiting','failed','cancelled','completed')), last_event TEXT NOT NULL REFERENCES echo_events(event));

CREATE TABLE echo_runtime_anchor(identity INTEGER PRIMARY KEY CHECK(identity=1),path TEXT NOT NULL);

CREATE TABLE echo_schedule_occurrences(identity TEXT PRIMARY KEY,schedule TEXT NOT NULL REFERENCES echo_schedules(identity),scheduled_for INTEGER NOT NULL,missed_intervals INTEGER NOT NULL,origin_event TEXT NOT NULL REFERENCES echo_events(event),UNIQUE(schedule,scheduled_for));

CREATE TABLE echo_schedule_state(identity TEXT PRIMARY KEY REFERENCES echo_schedules(identity),next_due INTEGER NOT NULL,status TEXT NOT NULL CHECK(status IN ('enabled','cancelled','exhausted')),last_event TEXT NOT NULL REFERENCES echo_events(event));

CREATE TABLE echo_schedules(identity TEXT PRIMARY KEY,origin_run TEXT NOT NULL REFERENCES echo_runs(run),origin_call TEXT NOT NULL,definition_event TEXT NOT NULL REFERENCES echo_events(event),instruction TEXT NOT NULL,first_due INTEGER NOT NULL CHECK(first_due>=0),interval_ms INTEGER NOT NULL CHECK(interval_ms>=0),missed_policy TEXT NOT NULL CHECK(missed_policy IN ('catch-up','coalesce')),FOREIGN KEY(origin_run,origin_call) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_schema_migrations (version INTEGER PRIMARY KEY CHECK(version>0), name TEXT NOT NULL UNIQUE, digest TEXT NOT NULL, applied_at TEXT NOT NULL);

CREATE TABLE echo_session_owners(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_session_principals(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),principal TEXT NOT NULL REFERENCES echo_principals(identity),provenance TEXT NOT NULL CHECK(provenance IN ('legacy-device','authenticated-owner')));

CREATE TABLE echo_sessions(session TEXT PRIMARY KEY,title TEXT NOT NULL,created TEXT NOT NULL);

CREATE TABLE echo_tool_authority(run TEXT NOT NULL,call_id TEXT NOT NULL,device TEXT NOT NULL REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity),role TEXT NOT NULL,role_revision INTEGER NOT NULL,revocation_revision INTEGER NOT NULL,link_revision INTEGER NOT NULL,PRIMARY KEY(run,call_id),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_tool_calls (run TEXT NOT NULL, call_id TEXT NOT NULL, session TEXT NOT NULL, request_event TEXT NOT NULL UNIQUE REFERENCES echo_events(event), state TEXT NOT NULL CHECK(state IN ('queued','running','uncertain','observed')), result_event TEXT UNIQUE REFERENCES echo_events(event), reconciliation_event TEXT REFERENCES echo_events(event), PRIMARY KEY(run,call_id));

CREATE TABLE echo_tool_candidates(sequence INTEGER PRIMARY KEY AUTOINCREMENT,run TEXT NOT NULL REFERENCES echo_runs(run),name TEXT NOT NULL,version INTEGER NOT NULL CHECK(version>0),manifest TEXT NOT NULL,digest TEXT NOT NULL,created TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')),UNIQUE(run,name,version));

CREATE TABLE echo_uploads(upload TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device),path TEXT NOT NULL UNIQUE,name TEXT NOT NULL,media_type TEXT NOT NULL,state TEXT NOT NULL CHECK(state IN ('prepared','receiving','failed')));

CREATE TABLE echo_web_workers(run TEXT PRIMARY KEY REFERENCES echo_runs(run),worker TEXT NOT NULL,status TEXT NOT NULL);

CREATE TABLE echo_worker_lock_protocol(worker TEXT PRIMARY KEY,run TEXT NOT NULL REFERENCES echo_runs(run));

CREATE TRIGGER echo_account_links_no_delete BEFORE DELETE ON echo_account_link_events BEGIN SELECT RAISE(ABORT,'Account link history is immutable'); END;

CREATE TRIGGER echo_account_links_no_update BEFORE UPDATE ON echo_account_link_events BEGIN SELECT RAISE(ABORT,'Account link history is immutable'); END;

CREATE TRIGGER echo_attachment_derivation_no_delete BEFORE DELETE ON echo_attachment_derivations BEGIN SELECT RAISE(ABORT,'Attachment lineage is immutable'); END;

CREATE TRIGGER echo_attachment_derivation_no_update BEFORE UPDATE ON echo_attachment_derivations BEGIN SELECT RAISE(ABORT,'Attachment lineage is immutable'); END;

CREATE TRIGGER echo_attachment_no_delete BEFORE DELETE ON echo_attachments BEGIN SELECT RAISE(ABORT,'Attachment originals are immutable'); END;

CREATE TRIGGER echo_attachment_no_update BEFORE UPDATE ON echo_attachments BEGIN SELECT RAISE(ABORT,'Attachment originals are immutable'); END;

CREATE TRIGGER echo_authority_no_delete BEFORE DELETE ON echo_authority_events BEGIN SELECT RAISE(ABORT,'Authority history is immutable'); END;

CREATE TRIGGER echo_authority_no_update BEFORE UPDATE ON echo_authority_events BEGIN SELECT RAISE(ABORT,'Authority history is immutable'); END;

CREATE TRIGGER echo_checkpoints_no_delete BEFORE DELETE ON echo_checkpoints BEGIN SELECT RAISE(ABORT,'Saved checkpoints are immutable'); END;

CREATE TRIGGER echo_checkpoints_no_update BEFORE UPDATE ON echo_checkpoints BEGIN SELECT RAISE(ABORT,'Saved checkpoints are immutable'); END;

CREATE TRIGGER echo_conversation_account_capture AFTER INSERT ON echo_devices BEGIN INSERT INTO echo_conversation_accounts VALUES(NEW.device,NEW.device); END;

CREATE TRIGGER echo_conversation_merges_no_delete BEFORE DELETE ON echo_conversation_merges BEGIN SELECT RAISE(ABORT,'Merge history is immutable'); END;

CREATE TRIGGER echo_conversation_merges_no_update BEFORE UPDATE ON echo_conversation_merges BEGIN SELECT RAISE(ABORT,'Merge history is immutable'); END;

CREATE TRIGGER echo_delegation_before_child BEFORE INSERT ON echo_delegations WHEN EXISTS(SELECT 1 FROM echo_runs WHERE run=NEW.child) OR NEW.child=NEW.parent BEGIN SELECT RAISE(ABORT,'Delegation origin must precede child'); END;

CREATE TRIGGER echo_delegation_no_delete BEFORE DELETE ON echo_delegations BEGIN SELECT RAISE(ABORT,'Delegation lineage is immutable'); END;

CREATE TRIGGER echo_delegation_no_update BEFORE UPDATE ON echo_delegations BEGIN SELECT RAISE(ABORT,'Delegation lineage is immutable'); END;

CREATE TRIGGER echo_device_links_no_delete BEFORE DELETE ON echo_device_link_events BEGIN SELECT RAISE(ABORT,'Device link history is immutable'); END;

CREATE TRIGGER echo_device_links_no_update BEFORE UPDATE ON echo_device_link_events BEGIN SELECT RAISE(ABORT,'Device link history is immutable'); END;

CREATE TRIGGER echo_device_principal AFTER INSERT ON echo_devices BEGIN INSERT INTO echo_principals(identity) VALUES('device:'||NEW.device); INSERT INTO echo_principal_devices(device,principal) VALUES(NEW.device,'device:'||NEW.device); END;

CREATE TRIGGER echo_events_no_delete BEFORE DELETE ON echo_events BEGIN SELECT RAISE(ABORT,'Original events are immutable'); END;

CREATE TRIGGER echo_events_no_update BEFORE UPDATE ON echo_events BEGIN SELECT RAISE(ABORT,'Original events are immutable'); END;

CREATE TRIGGER echo_group_audiences_no_delete BEFORE DELETE ON echo_group_audiences BEGIN SELECT RAISE(ABORT,'Audience history is immutable'); END;

CREATE TRIGGER echo_group_audiences_no_update BEFORE UPDATE ON echo_group_audiences BEGIN SELECT RAISE(ABORT,'Audience history is immutable'); END;

CREATE TRIGGER echo_group_canonical_unique BEFORE INSERT ON echo_group_sessions WHEN NOT EXISTS(SELECT 1 FROM echo_delegations WHERE session=NEW.session) AND EXISTS(SELECT 1 FROM echo_group_sessions s WHERE s.group_id=NEW.group_id AND s.revision=NEW.revision AND NOT EXISTS(SELECT 1 FROM echo_delegations d WHERE d.session=s.session)) BEGIN SELECT RAISE(ABORT,'Canonical group session already exists'); END;

CREATE TRIGGER echo_group_membership_no_delete BEFORE DELETE ON echo_group_membership BEGIN SELECT RAISE(ABORT,'Membership history is immutable'); END;

CREATE TRIGGER echo_group_membership_no_update BEFORE UPDATE ON echo_group_membership BEGIN SELECT RAISE(ABORT,'Membership history is immutable'); END;

CREATE TRIGGER echo_group_recipients_capture AFTER INSERT ON echo_runs BEGIN INSERT INTO echo_group_run_recipients SELECT NEW.run,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events WHERE principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations WHERE principal=p.identity) FROM echo_group_run_origins g JOIN echo_group_sessions s ON s.session=g.session JOIN echo_group_audiences a ON a.group_id=s.group_id AND a.revision=s.revision JOIN echo_principals p ON p.identity=a.principal WHERE g.run=NEW.run AND g.session=NEW.session; END;

CREATE TRIGGER echo_group_recipients_no_delete BEFORE DELETE ON echo_group_run_recipients BEGIN SELECT RAISE(ABORT,'Run audience authority is immutable'); END;

CREATE TRIGGER echo_group_recipients_no_update BEFORE UPDATE ON echo_group_run_recipients BEGIN SELECT RAISE(ABORT,'Run audience authority is immutable'); END;

CREATE TRIGGER echo_group_run_origin_before_run BEFORE INSERT ON echo_group_run_origins WHEN EXISTS(SELECT 1 FROM echo_runs WHERE run=NEW.run) BEGIN SELECT RAISE(ABORT,'Origin must precede run creation'); END;

CREATE TRIGGER echo_group_run_origin_no_delete BEFORE DELETE ON echo_group_run_origins BEGIN SELECT RAISE(ABORT,'Shared initiating speaker is immutable'); END;

CREATE TRIGGER echo_group_run_origin_no_update BEFORE UPDATE ON echo_group_run_origins BEGIN SELECT RAISE(ABORT,'Shared initiating speaker is immutable'); END;

CREATE TRIGGER echo_group_session_no_delete BEFORE DELETE ON echo_group_sessions BEGIN SELECT RAISE(ABORT,'Group session audience is immutable'); END;

CREATE TRIGGER echo_group_session_no_update BEFORE UPDATE ON echo_group_sessions BEGIN SELECT RAISE(ABORT,'Group session audience is immutable'); END;

CREATE TRIGGER echo_group_session_not_private BEFORE INSERT ON echo_group_sessions BEGIN SELECT CASE WHEN EXISTS(SELECT 1 FROM echo_session_owners WHERE session=NEW.session) OR EXISTS(SELECT 1 FROM echo_session_principals WHERE session=NEW.session) THEN RAISE(ABORT,'Private conversation cannot become shared') END; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_group_audiences WHERE group_id=NEW.group_id AND revision=NEW.revision) THEN RAISE(ABORT,'Recorded audience required') END; END;

CREATE TRIGGER echo_group_speech_before_call BEFORE INSERT ON echo_group_speech_jobs WHEN EXISTS(SELECT 1 FROM echo_tool_calls WHERE run=NEW.job) OR EXISTS(SELECT 1 FROM echo_runs WHERE run=NEW.job) BEGIN SELECT RAISE(ABORT,'Speech origin must precede call'); END;

CREATE TRIGGER echo_group_speech_no_delete BEFORE DELETE ON echo_group_speech_jobs BEGIN SELECT RAISE(ABORT,'Speech origin is immutable'); END;

CREATE TRIGGER echo_group_speech_no_update BEFORE UPDATE ON echo_group_speech_jobs BEGIN SELECT RAISE(ABORT,'Speech origin is immutable'); END;

CREATE TRIGGER echo_groups_no_delete BEFORE DELETE ON echo_groups BEGIN SELECT RAISE(ABORT,'Group identity is immutable'); END;

CREATE TRIGGER echo_groups_no_update BEFORE UPDATE ON echo_groups BEGIN SELECT RAISE(ABORT,'Group identity is immutable'); END;

CREATE TRIGGER echo_memory_claims_no_delete BEFORE DELETE ON echo_memory_claims BEGIN SELECT RAISE(ABORT,'Memory assertion history is immutable'); END;

CREATE TRIGGER echo_memory_claims_no_update BEFORE UPDATE ON echo_memory_claims BEGIN SELECT RAISE(ABORT,'Memory assertion history is immutable'); END;

CREATE TRIGGER echo_memory_nodes_no_delete BEFORE DELETE ON echo_memory_nodes BEGIN SELECT RAISE(ABORT,'Memory node identity is immutable'); END;

CREATE TRIGGER echo_memory_nodes_no_update BEFORE UPDATE ON echo_memory_nodes BEGIN SELECT RAISE(ABORT,'Memory node identity is immutable'); END;

CREATE TRIGGER echo_memory_visibility_no_delete BEFORE DELETE ON echo_memory_visibility BEGIN SELECT RAISE(ABORT,'Memory lifecycle history is immutable'); END;

CREATE TRIGGER echo_memory_visibility_no_update BEFORE UPDATE ON echo_memory_visibility BEGIN SELECT RAISE(ABORT,'Memory lifecycle history is immutable'); END;

CREATE TRIGGER echo_migrations_no_delete BEFORE DELETE ON echo_schema_migrations BEGIN SELECT RAISE(ABORT,'Migration receipts are immutable'); END;

CREATE TRIGGER echo_migrations_no_update BEFORE UPDATE ON echo_schema_migrations BEGIN SELECT RAISE(ABORT,'Migration receipts are immutable'); END;

CREATE TRIGGER echo_node_run_epochs_capture AFTER INSERT ON echo_runs BEGIN SELECT CASE WHEN EXISTS(SELECT 1 FROM echo_events s WHERE s.kind='emergency_started' AND NOT EXISTS(SELECT 1 FROM echo_events r WHERE r.session=s.session AND r.kind='emergency_released')) THEN RAISE(ABORT,'Node quarantined') END; INSERT INTO echo_node_run_epochs VALUES(NEW.run,(SELECT COUNT(*) FROM echo_events WHERE kind='emergency_started')); END;

CREATE TRIGGER echo_node_run_epochs_no_delete BEFORE DELETE ON echo_node_run_epochs BEGIN SELECT RAISE(ABORT,'Node epoch is immutable'); END;

CREATE TRIGGER echo_node_run_epochs_no_update BEFORE UPDATE ON echo_node_run_epochs BEGIN SELECT RAISE(ABORT,'Node epoch is immutable'); END;

CREATE TRIGGER echo_node_tool_epochs_capture AFTER INSERT ON echo_tool_calls BEGIN SELECT CASE WHEN EXISTS(SELECT 1 FROM echo_events s WHERE s.kind='emergency_started' AND NOT EXISTS(SELECT 1 FROM echo_events r WHERE r.session=s.session AND r.kind='emergency_released')) THEN RAISE(ABORT,'Node quarantined') END; INSERT INTO echo_node_tool_epochs VALUES(NEW.run,NEW.call_id,(SELECT COUNT(*) FROM echo_events WHERE kind='emergency_started')); END;

CREATE TRIGGER echo_node_tool_epochs_no_delete BEFORE DELETE ON echo_node_tool_epochs BEGIN SELECT RAISE(ABORT,'Node epoch is immutable'); END;

CREATE TRIGGER echo_node_tool_epochs_no_update BEFORE UPDATE ON echo_node_tool_epochs BEGIN SELECT RAISE(ABORT,'Node epoch is immutable'); END;

CREATE TRIGGER echo_operations_no_delete BEFORE DELETE ON echo_operations BEGIN SELECT RAISE(ABORT,'Operation receipts are immutable'); END;

CREATE TRIGGER echo_operations_no_update BEFORE UPDATE ON echo_operations BEGIN SELECT RAISE(ABORT,'Operation receipts are immutable'); END;

CREATE TRIGGER echo_private_owner_not_group BEFORE INSERT ON echo_session_owners WHEN EXISTS(SELECT 1 FROM echo_group_sessions WHERE session=NEW.session) BEGIN SELECT RAISE(ABORT,'Group conversation cannot become private'); END;

CREATE TRIGGER echo_private_principal_not_group BEFORE INSERT ON echo_session_principals WHEN EXISTS(SELECT 1 FROM echo_group_sessions WHERE session=NEW.session) BEGIN SELECT RAISE(ABORT,'Group conversation cannot become private'); END;

CREATE TRIGGER echo_protected_proposals_no_delete BEFORE DELETE ON echo_protected_proposals BEGIN SELECT RAISE(ABORT,'Protected proposals are immutable'); END;

CREATE TRIGGER echo_protected_proposals_no_update BEFORE UPDATE ON echo_protected_proposals BEGIN SELECT RAISE(ABORT,'Protected proposals are immutable'); END;

CREATE TRIGGER echo_protected_reviews_legacy_no_insert BEFORE INSERT ON echo_protected_reviews_pre16 BEGIN SELECT RAISE(ABORT,'Legacy review snapshot is immutable'); END;

CREATE TRIGGER echo_protected_reviews_no_delete BEFORE DELETE ON "echo_protected_reviews_pre16" BEGIN SELECT RAISE(ABORT,'Protected decisions are immutable'); END;

CREATE TRIGGER echo_protected_reviews_no_update BEFORE UPDATE ON "echo_protected_reviews_pre16" BEGIN SELECT RAISE(ABORT,'Protected decisions are immutable'); END;

CREATE TRIGGER echo_protected_reviews_v16_no_delete BEFORE DELETE ON echo_protected_reviews BEGIN SELECT RAISE(ABORT,'Protected decisions are immutable'); END;

CREATE TRIGGER echo_protected_reviews_v16_no_update BEFORE UPDATE ON echo_protected_reviews BEGIN SELECT RAISE(ABORT,'Protected decisions are immutable'); END;

CREATE TRIGGER echo_protected_steward_no_delete BEFORE DELETE ON echo_protected_steward BEGIN SELECT RAISE(ABORT,'Steward decisions are immutable'); END;

CREATE TRIGGER echo_protected_steward_no_update BEFORE UPDATE ON echo_protected_steward BEGIN SELECT RAISE(ABORT,'Steward decisions are immutable'); END;

CREATE TRIGGER echo_provider_profiles_no_delete BEFORE DELETE ON echo_provider_profiles BEGIN SELECT RAISE(ABORT,'Provider versions are immutable'); END;

CREATE TRIGGER echo_provider_profiles_no_update BEFORE UPDATE ON echo_provider_profiles BEGIN SELECT RAISE(ABORT,'Provider versions are immutable'); END;

CREATE TRIGGER echo_revocations_no_delete BEFORE DELETE ON echo_principal_revocations BEGIN SELECT RAISE(ABORT,'Revocation history is immutable'); END;

CREATE TRIGGER echo_revocations_no_update BEFORE UPDATE ON echo_principal_revocations BEGIN SELECT RAISE(ABORT,'Revocation history is immutable'); END;

CREATE TRIGGER echo_run_authority_capture AFTER INSERT ON echo_runs BEGIN INSERT INTO echo_run_authority SELECT NEW.run,d.device,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events e WHERE e.principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations v WHERE v.principal=p.identity),(SELECT COUNT(*) FROM echo_device_link_events l WHERE l.device=d.device) FROM echo_session_owners o JOIN echo_devices d ON d.device=o.device JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE o.session=NEW.session AND d.revoked=0 AND p.revoked=0; INSERT INTO echo_run_authority SELECT NEW.run,d.device,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events e WHERE e.principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations v WHERE v.principal=p.identity),(SELECT COUNT(*) FROM echo_device_link_events l WHERE l.device=d.device) FROM echo_group_run_origins g JOIN echo_devices d ON d.device=g.device JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE g.run=NEW.run AND g.session=NEW.session AND d.revoked=0 AND p.revoked=0; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_run_authority WHERE run=NEW.run) THEN RAISE(ABORT,'Authenticated initiating authority required') END; END;

CREATE TRIGGER echo_run_authority_no_delete BEFORE DELETE ON echo_run_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_run_authority_no_update BEFORE UPDATE ON echo_run_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_run_identity_immutable BEFORE UPDATE OF run,session,objective_event ON echo_runs BEGIN SELECT RAISE(ABORT,'Run identity is immutable'); END;

CREATE TRIGGER echo_runtime_anchor_no_delete BEFORE DELETE ON echo_runtime_anchor BEGIN SELECT RAISE(ABORT,'Runtime anchor is immutable'); END;

CREATE TRIGGER echo_runtime_anchor_no_update BEFORE UPDATE ON echo_runtime_anchor BEGIN SELECT RAISE(ABORT,'Runtime anchor is immutable'); END;

CREATE TRIGGER echo_schedule_occurrences_no_delete BEFORE DELETE ON echo_schedule_occurrences BEGIN SELECT RAISE(ABORT,'Occurrences are immutable'); END;

CREATE TRIGGER echo_schedule_occurrences_no_update BEFORE UPDATE ON echo_schedule_occurrences BEGIN SELECT RAISE(ABORT,'Occurrences are immutable'); END;

CREATE TRIGGER echo_schedules_no_delete BEFORE DELETE ON echo_schedules BEGIN SELECT RAISE(ABORT,'Schedule definitions are immutable'); END;

CREATE TRIGGER echo_schedules_no_update BEFORE UPDATE ON echo_schedules BEGIN SELECT RAISE(ABORT,'Schedule definitions are immutable'); END;

CREATE TRIGGER echo_session_principal_capture AFTER INSERT ON echo_session_owners BEGIN INSERT INTO echo_session_principals SELECT NEW.session,p.identity,'authenticated-owner' FROM echo_devices d JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE d.device=NEW.device AND d.revoked=0 AND p.revoked=0; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_session_principals WHERE session=NEW.session) THEN RAISE(ABORT,'Authenticated session principal required') END; END;

CREATE TRIGGER echo_session_principal_no_delete BEFORE DELETE ON echo_session_principals BEGIN SELECT RAISE(ABORT,'Session principal is immutable'); END;

CREATE TRIGGER echo_session_principal_no_update BEFORE UPDATE ON echo_session_principals BEGIN SELECT RAISE(ABORT,'Session principal is immutable'); END;

CREATE TRIGGER echo_tool_authority_capture AFTER INSERT ON echo_tool_calls BEGIN INSERT INTO echo_tool_authority SELECT NEW.run,NEW.call_id,d.device,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events e WHERE e.principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations v WHERE v.principal=p.identity),(SELECT COUNT(*) FROM echo_device_link_events l WHERE l.device=d.device) FROM echo_session_owners o JOIN echo_devices d ON d.device=o.device JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE o.session=NEW.session AND d.revoked=0 AND p.revoked=0; INSERT INTO echo_tool_authority SELECT a.run,NEW.call_id,a.device,a.principal,a.role,a.role_revision,a.revocation_revision,a.link_revision FROM echo_run_authority a JOIN echo_group_run_origins g ON g.run=a.run JOIN echo_runs r ON r.run=g.run AND r.session=g.session WHERE a.run=NEW.run AND g.session=NEW.session; INSERT INTO echo_tool_authority SELECT NEW.run,NEW.call_id,d.device,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events WHERE principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations WHERE principal=p.identity),(SELECT COUNT(*) FROM echo_device_link_events WHERE device=d.device) FROM echo_group_speech_jobs j JOIN echo_devices d ON d.device=j.device JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE j.job=NEW.run AND j.session=NEW.session AND NEW.call_id='speak' AND d.revoked=0 AND p.revoked=0; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_tool_authority WHERE run=NEW.run AND call_id=NEW.call_id) THEN RAISE(ABORT,'Authenticated initiating authority required') END; END;

CREATE TRIGGER echo_tool_authority_no_delete BEFORE DELETE ON echo_tool_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_tool_authority_no_update BEFORE UPDATE ON echo_tool_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_tool_candidates_no_delete BEFORE DELETE ON echo_tool_candidates BEGIN SELECT RAISE(ABORT,'Tool candidate versions are immutable'); END;

CREATE TRIGGER echo_tool_candidates_no_update BEFORE UPDATE ON echo_tool_candidates BEGIN SELECT RAISE(ABORT,'Tool candidate versions are immutable'); END;

CREATE TRIGGER echo_tool_identity_immutable BEFORE UPDATE OF run,call_id,session,request_event ON echo_tool_calls BEGIN SELECT RAISE(ABORT,'Tool identity is immutable'); END;

CREATE TRIGGER echo_tool_result_immutable BEFORE UPDATE ON echo_tool_calls WHEN OLD.state='observed' BEGIN SELECT RAISE(ABORT,'Observed tool result is immutable'); END;

CREATE TRIGGER echo_upload_identity_immutable BEFORE UPDATE OF upload,session,device,path,name,media_type ON echo_uploads BEGIN SELECT RAISE(ABORT,'Upload identity is immutable'); END;
