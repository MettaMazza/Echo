> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Persistent projects

Project chat tools and automatic project context are deployed in `phase2-projects-20261016-235537`, schema 26. This completes the project-tool stage of Step 09, not the remaining memory/continuity acceptance or project browser UI.

## Storage, scope and provenance

`app/projects/store.ep` writes exact manifest text and its SHA256 into `project_revision` events in the existing immutable archive. Each revision records its initiating run, original scope and preceding revision event. A writer transaction validates the manifest and source references, checks the expected revision and appends the new event atomically. Identical event retries are idempotent; conflicting event reuse and stale edits fail. Original revisions remain in the same archive and backups; there is no separate mutable project database or schema migration.

`scope.ep` resolves the frozen initiating run's private principal or exact group/audience revision. Cross-session private access uses the existing linked-memory union. A shared run cannot select the sender's private projects, and project updates retain their original scope. Linked-device changes and audience-transition project cases remain part of Step 09's broader integrated acceptance.

`manifest.ep` requires name, instructions, tasks and files with exact field/type validation. Task IDs are unique and statuses are todo/doing/done/blocked; these are authored planning states, not independent proof of execution. Files reference registered attachments by identity, label and exact source SHA256. Each reference must already belong to the current private source scope or shared audience. References do not copy files or grant permission to their bytes. Original attachment readers still validate actual bytes. Unknown properties, duplicate JSON keys, malformed task states and unavailable/foreign file references are refused.

## Chat workflow

The native `project` tool supports create, update, read, list and select. Create returns stable project/revision IDs derived from the archived run/call. Update requires the current revision ID and a complete replacement manifest. List pages latest scoped revisions using an archive-sequence cursor; concurrent edits can change ordering, so restart listing to refresh. Select records the active project for this conversation; an empty project string clears selection. To resume in another conversation, discover/read and select the same project.

`context.ep` resolves the current selected revision for every constructed model request, including the shared context-building path used around compaction. It provides the exact project instructions, tasks, file references, revision event and manifest digest as user-level project context. It cannot override system authority. Missing authorization or unavailable source references do not inject stale project contents. Actual compaction and independent database restoration now pass in MEMORY-RECOVERY.md, including selected project instructions/tasks. That recovery project has no files; file-reference validation is covered by the separate project-tool fixture. Linked-device/browser project acceptance remains in Step 10.

## Evidence

Inspected reference: ErnosDecent `decent_agent/workspace.ep` and `workspace_links.ep`. Echo keeps project selection separate from access authority, using archived revisions rather than its mutable line-file format. No compiler or identity prompt was changed.

`test_project_store.ep` passes exact Unicode/digest preservation, second-session read/update, retry/conflict handling, stale-write rejection, foreign-principal and private/group isolation, reopen, archive chains and revocation. `test_project_tools.ep` passes actual streamed dispatch, referenced attachment validation, listing/selection in a second session, persistent automatic context, invalid task state/foreign-file denial and revocation. `test_live_project.ep` passed with the actual primary model: create a project, reopen SQLite, start a separate conversation, discover/select the project, include its automatic context in a subsequent archived model request and return its source-only verification marker. Both original session chains verify.

Private evidence is in `evidence/phase2/project-store-20261016/` and `project-tools-20261016/`. The initial native fixture omitted attachment schema setup; a later scripted fixture reused a provider request ID across sessions. Both fixture errors were corrected, not treated as product acceptance or used to restart the actual-model test. Successful archives independently pass SQLite quick_check and foreign_key_check.

Deployment replaced only the server; other helpers and the inference process were retained. Both local HTTP and LAN HTTPS status endpoints returned 200. This is host-side connectivity evidence, not a new physical-phone test. The project UI and episodic/semantic/procedural memory integration remain at their original steps.

## Browser management — 17 October

`app/web/projects.ep` adds an authenticated-device operation path for create/update/read/list/select. The browser path resolves the selected conversation under its actual credential and current linked-account/group permissions; it does not manufacture a model turn or reuse another run's authority. Human revision records retain actor/device provenance with an empty model-run field. Common scope, file-reference and manifest checks are shared with the existing model tool. Writes are transactional, preserve previous revisions and reject stale updates; retry identities are bound to device, session and exact payload. `/api/projects` is wired through the existing origin, credential and restored-archive write guards.

The native `tests/web/test_projects.ep` passed cross-session model-context consumption, retries, stale changes, foreign/private/group separation, reopen/archive integrity and revocation. The existing streamed project-tool regression also passed with file references and denial checks after sharing the validators. Evidence: `evidence/phase2/project-browser-20261017/native-results.json`. The browser controls and real HTTP execution subsequently passed and are deployed in `phase2-project-ui-20261017-021508`. Distinct linked-device browser acceptance remains part of the wider UI acceptance.


The sidebar **Projects** button opens a labelled editor. Users can create or select a project, edit its instructions and tasks, choose uploaded file references, save an immutable revision, select it for the current conversation, or clear selection. Previously referenced files remain selectable when editing from another conversation. Removing a reference does not delete its original. Interrupted writes retain their exact operation payload and identity locally, with a retry button; stale-revision rejection preserves fields for review instead of overwriting another edit. The editor reports that project instructions take effect when the next model request is built.

Actual browser/HTTP acceptance covered creation, task editing, selecting the same project in two conversations, reload persistence, and saving a registered attachment whose digest matches the original bytes. Three revisions and exactly the original two fixture runs remained: editing did not create artificial model turns. The first browser attempt caught the missing outer server route and returned HTTP 405; that route was corrected and rebuilt before passing acceptance. Native tests cover exact retry and stale-edit rejection. Browser network fault injection and distinct linked-device acceptance are not claimed by this fixture. Evidence and screenshot: `evidence/phase2/project-browser-20261017/`. Local and LAN health passed after deployment; only server/client changed and the model process was preserved.

## Concurrent browser edits — 17 October 2026

Two tabs loaded the same project revision. Saving in the first tab succeeded; a stale save in the second was refused, preserving its typed fields and the first saved revision. Reopening loaded the saved version. Fixed the misleading retry button remaining visible after a definitive rejection cleared the pending operation. Evidence: `evidence/phase2/project-conflict-20261017/result.json` and `conflict.png`. This verifies two tabs of one disposable account, not physical linked-device acceptance. The UI-only release retains all executable and model bytes.
