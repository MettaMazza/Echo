CREATE INDEX echo_events_session ON echo_events(session,sequence);

CREATE TABLE echo_active_context (session TEXT PRIMARY KEY, checkpoint TEXT NOT NULL REFERENCES echo_checkpoints(identity));

CREATE TABLE echo_attachments(attachment TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_sessions(session),path TEXT NOT NULL UNIQUE,name TEXT NOT NULL,media_type TEXT NOT NULL,digest TEXT NOT NULL,bytes INTEGER NOT NULL CHECK(bytes>=0),event TEXT NOT NULL UNIQUE REFERENCES echo_events(event));

CREATE TABLE echo_cancel_requests(run TEXT PRIMARY KEY REFERENCES echo_runs(run),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_checkpoints (identity TEXT PRIMARY KEY, session TEXT NOT NULL, source_end TEXT NOT NULL REFERENCES echo_events(event), source_digest TEXT NOT NULL, previous TEXT NOT NULL, summary TEXT NOT NULL, structured_state TEXT NOT NULL, validation TEXT NOT NULL);

CREATE TABLE echo_devices(device TEXT PRIMARY KEY,name TEXT NOT NULL,credential_hash TEXT NOT NULL UNIQUE,revoked INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_events (sequence INTEGER PRIMARY KEY AUTOINCREMENT, session TEXT NOT NULL, event TEXT NOT NULL UNIQUE, kind TEXT NOT NULL, payload TEXT NOT NULL, occurred TEXT NOT NULL, previous TEXT NOT NULL, digest TEXT NOT NULL);

CREATE TABLE echo_pairing(invitation_hash TEXT PRIMARY KEY,consumed INTEGER NOT NULL DEFAULT 0);

CREATE TABLE echo_process_launches(run TEXT NOT NULL,call_id TEXT NOT NULL,session TEXT NOT NULL,reservation TEXT NOT NULL UNIQUE REFERENCES echo_events(event),started TEXT UNIQUE REFERENCES echo_events(event),finished TEXT UNIQUE REFERENCES echo_events(event),PRIMARY KEY(run,call_id),FOREIGN KEY(run,call_id) REFERENCES echo_tool_calls(run,call_id));

CREATE TABLE echo_provider_default(singleton INTEGER PRIMARY KEY CHECK(singleton=1),identity TEXT NOT NULL REFERENCES echo_provider_profiles(identity));

CREATE TABLE echo_provider_profiles(identity TEXT PRIMARY KEY,label TEXT NOT NULL,profile TEXT NOT NULL);

CREATE TABLE echo_runs (run TEXT PRIMARY KEY, session TEXT NOT NULL, objective_event TEXT NOT NULL REFERENCES echo_events(event), state TEXT NOT NULL CHECK(state IN ('ready','generating','tools','waiting','failed','cancelled','completed')), last_event TEXT NOT NULL REFERENCES echo_events(event));

CREATE TABLE echo_runtime_anchor(identity INTEGER PRIMARY KEY CHECK(identity=1),path TEXT NOT NULL);

CREATE TABLE echo_session_owners(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device));

CREATE TABLE echo_sessions(session TEXT PRIMARY KEY,title TEXT NOT NULL,created TEXT NOT NULL);

CREATE TABLE echo_tool_calls (run TEXT NOT NULL, call_id TEXT NOT NULL, session TEXT NOT NULL, request_event TEXT NOT NULL UNIQUE REFERENCES echo_events(event), state TEXT NOT NULL CHECK(state IN ('queued','running','uncertain','observed')), result_event TEXT UNIQUE REFERENCES echo_events(event), reconciliation_event TEXT REFERENCES echo_events(event), PRIMARY KEY(run,call_id));

CREATE TABLE echo_uploads(upload TEXT PRIMARY KEY,session TEXT NOT NULL REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device),path TEXT NOT NULL UNIQUE,name TEXT NOT NULL,media_type TEXT NOT NULL,state TEXT NOT NULL CHECK(state IN ('prepared','receiving','failed')));

CREATE TABLE echo_web_workers(run TEXT PRIMARY KEY REFERENCES echo_runs(run),worker TEXT NOT NULL,status TEXT NOT NULL);

CREATE TABLE echo_worker_lock_protocol(worker TEXT PRIMARY KEY,run TEXT NOT NULL REFERENCES echo_runs(run));

CREATE TABLE echo_workspaces(session TEXT PRIMARY KEY REFERENCES echo_sessions(session),device TEXT NOT NULL REFERENCES echo_devices(device),root TEXT NOT NULL,directory TEXT NOT NULL UNIQUE);

CREATE TRIGGER echo_attachment_no_delete BEFORE DELETE ON echo_attachments BEGIN SELECT RAISE(ABORT,'Attachment originals are immutable'); END;

CREATE TRIGGER echo_attachment_no_update BEFORE UPDATE ON echo_attachments BEGIN SELECT RAISE(ABORT,'Attachment originals are immutable'); END;

CREATE TRIGGER echo_checkpoints_no_delete BEFORE DELETE ON echo_checkpoints BEGIN SELECT RAISE(ABORT,'Saved checkpoints are immutable'); END;

CREATE TRIGGER echo_checkpoints_no_update BEFORE UPDATE ON echo_checkpoints BEGIN SELECT RAISE(ABORT,'Saved checkpoints are immutable'); END;

CREATE TRIGGER echo_events_no_delete BEFORE DELETE ON echo_events BEGIN SELECT RAISE(ABORT,'Original events are immutable'); END;

CREATE TRIGGER echo_events_no_update BEFORE UPDATE ON echo_events BEGIN SELECT RAISE(ABORT,'Original events are immutable'); END;

CREATE TRIGGER echo_process_finished_immutable BEFORE UPDATE OF finished ON echo_process_launches WHEN OLD.finished IS NOT NULL BEGIN SELECT RAISE(ABORT,'Process finish receipt is immutable'); END;

CREATE TRIGGER echo_process_identity_immutable BEFORE UPDATE OF run,call_id,session,reservation ON echo_process_launches BEGIN SELECT RAISE(ABORT,'Process launch identity is immutable'); END;

CREATE TRIGGER echo_process_no_delete BEFORE DELETE ON echo_process_launches BEGIN SELECT RAISE(ABORT,'Process launch reservations cannot be deleted'); END;

CREATE TRIGGER echo_process_started_immutable BEFORE UPDATE OF started ON echo_process_launches WHEN OLD.started IS NOT NULL BEGIN SELECT RAISE(ABORT,'Process start receipt is immutable'); END;

CREATE TRIGGER echo_provider_profiles_no_delete BEFORE DELETE ON echo_provider_profiles BEGIN SELECT RAISE(ABORT,'Provider versions are immutable'); END;

CREATE TRIGGER echo_provider_profiles_no_update BEFORE UPDATE ON echo_provider_profiles BEGIN SELECT RAISE(ABORT,'Provider versions are immutable'); END;

CREATE TRIGGER echo_run_identity_immutable BEFORE UPDATE OF run,session,objective_event ON echo_runs BEGIN SELECT RAISE(ABORT,'Run identity is immutable'); END;

CREATE TRIGGER echo_runtime_anchor_no_delete BEFORE DELETE ON echo_runtime_anchor BEGIN SELECT RAISE(ABORT,'Runtime anchor is immutable'); END;

CREATE TRIGGER echo_runtime_anchor_no_update BEFORE UPDATE ON echo_runtime_anchor BEGIN SELECT RAISE(ABORT,'Runtime anchor is immutable'); END;

CREATE TRIGGER echo_tool_identity_immutable BEFORE UPDATE OF run,call_id,session,request_event ON echo_tool_calls BEGIN SELECT RAISE(ABORT,'Tool identity is immutable'); END;

CREATE TRIGGER echo_tool_result_immutable BEFORE UPDATE ON echo_tool_calls WHEN OLD.state='observed' BEGIN SELECT RAISE(ABORT,'Observed tool result is immutable'); END;

CREATE TRIGGER echo_upload_identity_immutable BEFORE UPDATE OF upload,session,device,path,name,media_type ON echo_uploads BEGIN SELECT RAISE(ABORT,'Upload identity is immutable'); END;

CREATE TRIGGER echo_workspace_no_delete BEFORE DELETE ON echo_workspaces BEGIN SELECT RAISE(ABORT,'Workspace identity is immutable'); END;

CREATE TRIGGER echo_workspace_no_update BEFORE UPDATE ON echo_workspaces BEGIN SELECT RAISE(ABORT,'Workspace identity is immutable'); END;
