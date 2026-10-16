CREATE INDEX echo_events_session ON echo_events(session,sequence);

CREATE INDEX echo_memory_object ON echo_memory_claims(principal,object,sequence);

CREATE UNIQUE INDEX echo_memory_single_successor ON echo_memory_claims(supersedes) WHERE supersedes IS NOT NULL;

CREATE INDEX echo_memory_subject ON echo_memory_claims(principal,subject,predicate,sequence);

CREATE INDEX echo_memory_visibility_claim ON echo_memory_visibility(claim,sequence);

CREATE INDEX echo_operations_scope ON echo_operations(scope,identity,revision);

CREATE TABLE echo_account_link_events(identity TEXT PRIMARY KEY,platform TEXT NOT NULL,subject TEXT NOT NULL,previous_principal TEXT NOT NULL,previous_revoked INTEGER NOT NULL,principal TEXT NOT NULL REFERENCES echo_principals(identity),revoked INTEGER NOT NULL CHECK(revoked IN (0,1)),actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_active_context (session TEXT PRIMARY KEY, checkpoint TEXT NOT NULL REFERENCES echo_checkpoints(identity));

CREATE TABLE echo_attachments(attachment TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_sessions(session),path TEXT NOT NULL UNIQUE,name TEXT NOT NULL,media_type TEXT NOT NULL,digest TEXT NOT NULL,bytes INTEGER NOT NULL CHECK(bytes>=0),event TEXT NOT NULL UNIQUE REFERENCES echo_events(event));

CREATE TABLE echo_authority_events(identity TEXT PRIMARY KEY,principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_role TEXT NOT NULL,role TEXT NOT NULL,actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_cancel_requests(run TEXT PRIMARY KEY REFERENCES echo_runs(run),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_checkpoints (identity TEXT PRIMARY KEY, session TEXT NOT NULL, source_end TEXT NOT NULL REFERENCES echo_events(event), source_digest TEXT NOT NULL, previous TEXT NOT NULL, summary TEXT NOT NULL, structured_state TEXT NOT NULL, validation TEXT NOT NULL);

CREATE TABLE echo_device_link_events(identity TEXT PRIMARY KEY,device TEXT NOT NULL REFERENCES echo_devices(device),previous_principal TEXT NOT NULL REFERENCES echo_principals(identity),principal TEXT NOT NULL REFERENCES echo_principals(identity),actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_devices(device TEXT PRIMARY KEY,name TEXT NOT NULL,credential_hash TEXT NOT NULL UNIQUE,revoked INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_events (sequence INTEGER PRIMARY KEY AUTOINCREMENT, session TEXT NOT NULL, event TEXT NOT NULL UNIQUE, kind TEXT NOT NULL, payload TEXT NOT NULL, occurred TEXT NOT NULL, previous TEXT NOT NULL, digest TEXT NOT NULL);

CREATE TABLE echo_memory_claims(sequence INTEGER PRIMARY KEY AUTOINCREMENT,identity TEXT NOT NULL UNIQUE,principal TEXT NOT NULL REFERENCES echo_principals(identity),kind TEXT NOT NULL CHECK(kind IN ('semantic','episodic')),subject TEXT NOT NULL REFERENCES echo_memory_nodes(identity),predicate TEXT NOT NULL,object TEXT NOT NULL REFERENCES echo_memory_nodes(identity),evidence TEXT NOT NULL CHECK(evidence IN ('source-account','inference','tool-observation')),source_event TEXT NOT NULL REFERENCES echo_events(event),source_digest TEXT NOT NULL,event_time TEXT NOT NULL,valid_until TEXT NOT NULL,supersedes TEXT REFERENCES echo_memory_claims(identity),recorded_at TEXT NOT NULL,canonical TEXT NOT NULL);

CREATE TABLE echo_memory_nodes(identity TEXT PRIMARY KEY,principal TEXT NOT NULL REFERENCES echo_principals(identity),name TEXT NOT NULL,UNIQUE(principal,name));

CREATE TABLE echo_memory_visibility(sequence INTEGER PRIMARY KEY AUTOINCREMENT,identity TEXT NOT NULL UNIQUE,claim TEXT NOT NULL REFERENCES echo_memory_claims(identity),principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_revision INTEGER NOT NULL CHECK(previous_revision>=0),state TEXT NOT NULL CHECK(state IN ('visible','forgotten')),reason TEXT NOT NULL,run TEXT NOT NULL REFERENCES echo_runs(run),recorded_at TEXT NOT NULL);

CREATE TABLE echo_operations (identity TEXT NOT NULL, scope TEXT NOT NULL, principal TEXT NOT NULL, kind TEXT NOT NULL, request_digest TEXT NOT NULL, revision INTEGER NOT NULL CHECK(revision>=0), state TEXT NOT NULL CHECK(state IN ('queued','running','uncertain','succeeded','failed')), stage TEXT NOT NULL, cause TEXT NOT NULL, effects TEXT NOT NULL CHECK(effects IN ('none','unknown','confirmed')), diagnostic TEXT NOT NULL, recovery TEXT NOT NULL, result TEXT NOT NULL, occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')), PRIMARY KEY(identity,revision));

CREATE TABLE echo_pairing(invitation_hash TEXT PRIMARY KEY,consumed INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_principal_accounts(platform TEXT NOT NULL,subject TEXT NOT NULL,principal TEXT NOT NULL REFERENCES echo_principals(identity),revoked INTEGER NOT NULL DEFAULT 0 CHECK(revoked IN (0,1)),PRIMARY KEY(platform,subject));

CREATE TABLE echo_principal_devices(device TEXT PRIMARY KEY REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity));

CREATE TABLE echo_principal_revocations(identity TEXT PRIMARY KEY,principal TEXT NOT NULL REFERENCES echo_principals(identity),previous_revoked INTEGER NOT NULL,revoked INTEGER NOT NULL CHECK(revoked IN (0,1)),previous_role TEXT NOT NULL,role TEXT NOT NULL,actor TEXT NOT NULL,reason TEXT NOT NULL,occurred TEXT NOT NULL DEFAULT(strftime('%Y-%m-%dT%H:%M:%fZ','now')));

CREATE TABLE echo_principals(identity TEXT PRIMARY KEY,role TEXT NOT NULL DEFAULT 'user' CHECK(role IN ('user','admin','steward')),revoked INTEGER NOT NULL DEFAULT 0 CHECK(revoked IN (0,1)));

CREATE TABLE echo_process_launches(run TEXT NOT NULL,call_id TEXT NOT NULL,session TEXT NOT NULL,reservation TEXT NOT NULL UNIQUE REFERENCES echo_events(event),started TEXT UNIQUE REFERENCES echo_events(event),finished TEXT UNIQUE REFERENCES echo_events(event),PRIMARY KEY(run,call_id),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_provider_default(singleton INTEGER PRIMARY KEY CHECK(singleton=1),identity TEXT NOT NULL REFERENCES echo_provider_profiles(identity));

CREATE TABLE echo_provider_profiles(identity TEXT PRIMARY KEY,label TEXT NOT NULL,profile TEXT NOT NULL);

CREATE TABLE echo_provider_retirements(identity TEXT PRIMARY KEY REFERENCES echo_provider_profiles(identity),reason TEXT NOT NULL);

CREATE TABLE echo_run_authority(run TEXT PRIMARY KEY REFERENCES echo_runs(run),device TEXT NOT NULL REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity),role TEXT NOT NULL,role_revision INTEGER NOT NULL,revocation_revision INTEGER NOT NULL,link_revision INTEGER NOT NULL);

CREATE TABLE echo_runs (run TEXT PRIMARY KEY, session TEXT NOT NULL, objective_event TEXT NOT NULL REFERENCES echo_events(event), state TEXT NOT NULL CHECK(state IN ('ready','generating','tools','waiting','failed','cancelled','completed')), last_event TEXT NOT NULL REFERENCES echo_events(event));

CREATE TABLE echo_runtime_anchor(identity INTEGER PRIMARY KEY CHECK(identity=1),path TEXT NOT NULL);

CREATE TABLE echo_schema_migrations (version INTEGER PRIMARY KEY CHECK(version>0), name TEXT NOT NULL UNIQUE, digest TEXT NOT NULL, applied_at TEXT NOT NULL);

CREATE TABLE echo_session_owners(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_session_principals(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),principal TEXT NOT NULL REFERENCES echo_principals(identity),provenance TEXT NOT NULL CHECK(provenance IN ('legacy-device','authenticated-owner')));

CREATE TABLE echo_sessions(session TEXT PRIMARY KEY,title TEXT NOT NULL,created TEXT NOT NULL);

CREATE TABLE echo_tool_authority(run TEXT NOT NULL,call_id TEXT NOT NULL,device TEXT NOT NULL REFERENCES echo_devices(device),principal TEXT NOT NULL REFERENCES echo_principals(identity),role TEXT NOT NULL,role_revision INTEGER NOT NULL,revocation_revision INTEGER NOT NULL,link_revision INTEGER NOT NULL,PRIMARY KEY(run,call_id),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_tool_calls (run TEXT NOT NULL, call_id TEXT NOT NULL, session TEXT NOT NULL, request_event TEXT NOT NULL UNIQUE REFERENCES echo_events(event), state TEXT NOT NULL CHECK(state IN ('queued','running','uncertain','observed')), result_event TEXT UNIQUE REFERENCES echo_events(event), reconciliation_event TEXT REFERENCES echo_events(event), PRIMARY KEY(run,call_id));

CREATE TABLE echo_uploads(upload TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device),path TEXT NOT NULL UNIQUE,name TEXT NOT NULL,media_type TEXT NOT NULL,state TEXT NOT NULL CHECK(state IN ('prepared','receiving','failed')));

CREATE TABLE echo_web_workers(run TEXT PRIMARY KEY REFERENCES echo_runs(run),worker TEXT NOT NULL,status TEXT NOT NULL);

CREATE TABLE echo_worker_lock_protocol(worker TEXT PRIMARY KEY,run TEXT NOT NULL REFERENCES echo_runs(run));

CREATE TABLE echo_workspaces(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device),root TEXT NOT NULL,directory TEXT NOT NULL UNIQUE);

CREATE TRIGGER echo_account_links_no_delete BEFORE DELETE ON echo_account_link_events BEGIN SELECT RAISE(ABORT,'Account link history is immutable'); END;

CREATE TRIGGER echo_account_links_no_update BEFORE UPDATE ON echo_account_link_events BEGIN SELECT RAISE(ABORT,'Account link history is immutable'); END;

CREATE TRIGGER echo_attachment_no_delete BEFORE DELETE ON echo_attachments BEGIN SELECT RAISE(ABORT,'Attachment originals are immutable'); END;

CREATE TRIGGER echo_attachment_no_update BEFORE UPDATE ON echo_attachments BEGIN SELECT RAISE(ABORT,'Attachment originals are immutable'); END;

CREATE TRIGGER echo_authority_no_delete BEFORE DELETE ON echo_authority_events BEGIN SELECT RAISE(ABORT,'Authority history is immutable'); END;

CREATE TRIGGER echo_authority_no_update BEFORE UPDATE ON echo_authority_events BEGIN SELECT RAISE(ABORT,'Authority history is immutable'); END;

CREATE TRIGGER echo_checkpoints_no_delete BEFORE DELETE ON echo_checkpoints BEGIN SELECT RAISE(ABORT,'Saved checkpoints are immutable'); END;

CREATE TRIGGER echo_checkpoints_no_update BEFORE UPDATE ON echo_checkpoints BEGIN SELECT RAISE(ABORT,'Saved checkpoints are immutable'); END;

CREATE TRIGGER echo_device_links_no_delete BEFORE DELETE ON echo_device_link_events BEGIN SELECT RAISE(ABORT,'Device link history is immutable'); END;

CREATE TRIGGER echo_device_links_no_update BEFORE UPDATE ON echo_device_link_events BEGIN SELECT RAISE(ABORT,'Device link history is immutable'); END;

CREATE TRIGGER echo_device_principal AFTER INSERT ON echo_devices BEGIN INSERT INTO echo_principals(identity) VALUES('device:'||NEW.device); INSERT INTO echo_principal_devices(device,principal) VALUES(NEW.device,'device:'||NEW.device); END;

CREATE TRIGGER echo_events_no_delete BEFORE DELETE ON echo_events BEGIN SELECT RAISE(ABORT,'Original events are immutable'); END;

CREATE TRIGGER echo_events_no_update BEFORE UPDATE ON echo_events BEGIN SELECT RAISE(ABORT,'Original events are immutable'); END;

CREATE TRIGGER echo_memory_claims_no_delete BEFORE DELETE ON echo_memory_claims BEGIN SELECT RAISE(ABORT,'Memory assertion history is immutable'); END;

CREATE TRIGGER echo_memory_claims_no_update BEFORE UPDATE ON echo_memory_claims BEGIN SELECT RAISE(ABORT,'Memory assertion history is immutable'); END;

CREATE TRIGGER echo_memory_nodes_no_delete BEFORE DELETE ON echo_memory_nodes BEGIN SELECT RAISE(ABORT,'Memory node identity is immutable'); END;

CREATE TRIGGER echo_memory_nodes_no_update BEFORE UPDATE ON echo_memory_nodes BEGIN SELECT RAISE(ABORT,'Memory node identity is immutable'); END;

CREATE TRIGGER echo_memory_visibility_no_delete BEFORE DELETE ON echo_memory_visibility BEGIN SELECT RAISE(ABORT,'Memory lifecycle history is immutable'); END;

CREATE TRIGGER echo_memory_visibility_no_update BEFORE UPDATE ON echo_memory_visibility BEGIN SELECT RAISE(ABORT,'Memory lifecycle history is immutable'); END;

CREATE TRIGGER echo_migrations_no_delete BEFORE DELETE ON echo_schema_migrations BEGIN SELECT RAISE(ABORT,'Migration receipts are immutable'); END;

CREATE TRIGGER echo_migrations_no_update BEFORE UPDATE ON echo_schema_migrations BEGIN SELECT RAISE(ABORT,'Migration receipts are immutable'); END;

CREATE TRIGGER echo_operations_no_delete BEFORE DELETE ON echo_operations BEGIN SELECT RAISE(ABORT,'Operation receipts are immutable'); END;

CREATE TRIGGER echo_operations_no_update BEFORE UPDATE ON echo_operations BEGIN SELECT RAISE(ABORT,'Operation receipts are immutable'); END;

CREATE TRIGGER echo_process_finished_immutable BEFORE UPDATE OF finished ON echo_process_launches WHEN OLD.finished IS NOT NULL BEGIN SELECT RAISE(ABORT,'Process finish receipt is immutable'); END;

CREATE TRIGGER echo_process_identity_immutable BEFORE UPDATE OF run,call_id,session,reservation ON echo_process_launches BEGIN SELECT RAISE(ABORT,'Process launch identity is immutable'); END;

CREATE TRIGGER echo_process_no_delete BEFORE DELETE ON echo_process_launches BEGIN SELECT RAISE(ABORT,'Process launch reservations cannot be deleted'); END;

CREATE TRIGGER echo_process_started_immutable BEFORE UPDATE OF started ON echo_process_launches WHEN OLD.started IS NOT NULL BEGIN SELECT RAISE(ABORT,'Process start receipt is immutable'); END;

CREATE TRIGGER echo_provider_profiles_no_delete BEFORE DELETE ON echo_provider_profiles BEGIN SELECT RAISE(ABORT,'Provider versions are immutable'); END;

CREATE TRIGGER echo_provider_profiles_no_update BEFORE UPDATE ON echo_provider_profiles BEGIN SELECT RAISE(ABORT,'Provider versions are immutable'); END;

CREATE TRIGGER echo_revocations_no_delete BEFORE DELETE ON echo_principal_revocations BEGIN SELECT RAISE(ABORT,'Revocation history is immutable'); END;

CREATE TRIGGER echo_revocations_no_update BEFORE UPDATE ON echo_principal_revocations BEGIN SELECT RAISE(ABORT,'Revocation history is immutable'); END;

CREATE TRIGGER echo_run_authority_capture AFTER INSERT ON echo_runs BEGIN INSERT INTO echo_run_authority SELECT NEW.run,d.device,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events e WHERE e.principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations v WHERE v.principal=p.identity),(SELECT COUNT(*) FROM echo_device_link_events l WHERE l.device=d.device) FROM echo_session_owners o JOIN echo_devices d ON d.device=o.device JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE o.session=NEW.session AND d.revoked=0 AND p.revoked=0; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_run_authority WHERE run=NEW.run) THEN RAISE(ABORT,'Authenticated initiating authority required') END; END;

CREATE TRIGGER echo_run_authority_no_delete BEFORE DELETE ON echo_run_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_run_authority_no_update BEFORE UPDATE ON echo_run_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_run_identity_immutable BEFORE UPDATE OF run,session,objective_event ON echo_runs BEGIN SELECT RAISE(ABORT,'Run identity is immutable'); END;

CREATE TRIGGER echo_runtime_anchor_no_delete BEFORE DELETE ON echo_runtime_anchor BEGIN SELECT RAISE(ABORT,'Runtime anchor is immutable'); END;

CREATE TRIGGER echo_runtime_anchor_no_update BEFORE UPDATE ON echo_runtime_anchor BEGIN SELECT RAISE(ABORT,'Runtime anchor is immutable'); END;

CREATE TRIGGER echo_session_principal_capture AFTER INSERT ON echo_session_owners BEGIN INSERT INTO echo_session_principals SELECT NEW.session,p.identity,'authenticated-owner' FROM echo_devices d JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE d.device=NEW.device AND d.revoked=0 AND p.revoked=0; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_session_principals WHERE session=NEW.session) THEN RAISE(ABORT,'Authenticated session principal required') END; END;

CREATE TRIGGER echo_session_principal_no_delete BEFORE DELETE ON echo_session_principals BEGIN SELECT RAISE(ABORT,'Session principal is immutable'); END;

CREATE TRIGGER echo_session_principal_no_update BEFORE UPDATE ON echo_session_principals BEGIN SELECT RAISE(ABORT,'Session principal is immutable'); END;

CREATE TRIGGER echo_tool_authority_capture AFTER INSERT ON echo_tool_calls BEGIN INSERT INTO echo_tool_authority SELECT NEW.run,NEW.call_id,d.device,p.identity,p.role,(SELECT COUNT(*) FROM echo_authority_events e WHERE e.principal=p.identity),(SELECT COUNT(*) FROM echo_principal_revocations v WHERE v.principal=p.identity),(SELECT COUNT(*) FROM echo_device_link_events l WHERE l.device=d.device) FROM echo_session_owners o JOIN echo_devices d ON d.device=o.device JOIN echo_principal_devices b ON b.device=d.device JOIN echo_principals p ON p.identity=b.principal WHERE o.session=NEW.session AND d.revoked=0 AND p.revoked=0; SELECT CASE WHEN NOT EXISTS(SELECT 1 FROM echo_tool_authority WHERE run=NEW.run AND call_id=NEW.call_id) THEN RAISE(ABORT,'Authenticated initiating authority required') END; END;

CREATE TRIGGER echo_tool_authority_no_delete BEFORE DELETE ON echo_tool_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_tool_authority_no_update BEFORE UPDATE ON echo_tool_authority BEGIN SELECT RAISE(ABORT,'Initiating authority is immutable'); END;

CREATE TRIGGER echo_tool_identity_immutable BEFORE UPDATE OF run,call_id,session,request_event ON echo_tool_calls BEGIN SELECT RAISE(ABORT,'Tool identity is immutable'); END;

CREATE TRIGGER echo_tool_result_immutable BEFORE UPDATE ON echo_tool_calls WHEN OLD.state='observed' BEGIN SELECT RAISE(ABORT,'Observed tool result is immutable'); END;

CREATE TRIGGER echo_upload_identity_immutable BEFORE UPDATE OF upload,session,device,path,name,media_type ON echo_uploads BEGIN SELECT RAISE(ABORT,'Upload identity is immutable'); END;

CREATE TRIGGER echo_workspace_no_delete BEFORE DELETE ON echo_workspaces BEGIN SELECT RAISE(ABORT,'Workspace identity is immutable'); END;

CREATE TRIGGER echo_workspace_no_update BEFORE UPDATE ON echo_workspaces BEGIN SELECT RAISE(ABORT,'Workspace identity is immutable'); END;
