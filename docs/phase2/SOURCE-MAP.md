> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Phase Two source integration map

## Existing Echo foundation

Baseline source: 070d52b. The private baseline identity record hashes the running server, browser bundle, compiler and active service configuration. Native archive snapshot and clean internal restoration passed; all restored table rows matched the snapshot. This preserves the existing device/ownership structure before principal migration. Attachment/configuration bundle verification remains a distinct gate.

## ErnosDecent reference

Revision 4ef202c5a920f17e5d2f6bde406b62e8185b1531, read-only reference checkout.

- `decent_agent/rights.ep` pins the charter and owns hash-bound protected-change manifests, inspection/consent, human/Observer decisions, recovery bundles and rollback verification. Its storage paths and SQL helpers are specific to ErnosDecent. Echo will adapt its storage/dispatch boundary, not alter charter prose independently. Full gate porting and adversarial verification remain step 07.
- `decent_agent/live_learning.ep` validates transaction IDs and manifest hashes, then separates training permission from activation permission. Existing binary training artifacts are produced through its external bridge. Echo needs an Ernos-owned controller around compatible upstream training libraries, with separate candidate and activation journals.
- `decent_agent/self_extensions.ep` supplies a registry/schema/dispatcher pattern, with calls entering the core controller. Existing extensions are bounded built-in routines, not proof of a complete general make-tool workflow. Echo must add persistent versions, discover/compose checks, capabilities and deployment qualification.

## HIVE routing actually inspected

Revision 99961051e95805bddc1288b4f4ab2bc03b9a493f:

- `src/models/scope.rs` (Git blob 69c59740c40468b7bb1998d82ce7035ce7f37235) requires exact user/channel matches for public silos and exact user matches for private silos. Cross-public/private reads are rejected. Scope keys supply serialization identities.
- `src/platforms/discord/message.rs` (Git blob 2fd26934cf30b71d950e83c07a11ece2d9c2a5a1) decodes mentions, configured channels, admin-restricted DMs and own/bot messages; builds authenticated author/channel/message events; enriches with referenced messages/attachments and emits progress telemetry. Mentioning another user does not construct a joined group in this file. Echo must implement that requirement explicitly.
- Echo must not inherit HIVE's special commands, six-digit linking assumption, bot-message buffering policy or DM restriction without matching the reviewed Echo policy. The source is a behavior reference, not a dependency rewrite into Rust.

## Migration contract

`app/lifecycle/migrations.ep` takes trusted host-authored SQL and applies it under a writer transaction with a version/name/SHA-256 receipt. DDL failures roll back; exact repeats are accepted; changed receipts and gaps are rejected. `migration_compatible` refuses newer schemas for older supported versions. It is wired into every server executable role before archive writes. The schema probe uses the normal archive busy wait so transient writer locks do not immediately become incompatibility failures. Schema 8 is deployed with a verified cutover backup and unchanged pre-existing records; incompatible rollback still requires explicit restoration.

The generic migration routine is never a model tool or HTTP endpoint. Caller SQL must not contain its own transaction control or nontransactional operations. All shipped migrations will be fixed reviewed statements, with backup restoration for incompatible rollback.

## Durable operations and principal store

`app/lifecycle/operations.ep` is schema migration 1. Each operation receipt includes scope, principal, request digest, revision, state, stage, cause, known/unknown effects, diagnostic reference, recovery instruction and result reference. State transitions append immutable receipts under a writer lock. Terminal states cannot redispatch, and uncertain states cannot return to running. External connectors must validate result evidence and recheck current capabilities before execution; this journal alone grants no authority.

`app/security/principals.ep` is schema migration 2 after the existing web device schema. Existing devices and future pairings receive distinct user principals; no display name or network fact grants admin. Host role mutations atomically append an authority receipt. Admin and steward roles remain distinct. This module is connected to production authentication and tool filtering through immutable run/tool authority snapshots and explicit capability policy.

`app/principal_admin.ep` is a host-local operator command. On a verified disposable/restorable archive, run `principal_admin DATABASE inspect-device DEVICE` or `principal_admin DATABASE role-device DEVICE ROLE UNIQUE_CHANGE_ID REASON`. Use the separate explicit `principal_admin DATABASE migrate` command to apply foundation migrations first. Inspect/grant do not apply them. It is not an automatic bootstrap installer or a public endpoint. Production migrations require the matching executable, verified backup, idle cutover and compatibility gate.

Startup guard: `app/lifecycle/startup.ep` inspects migration receipts before `archive_open`; `app/server.ep` applies it to every executable role. `tests/lifecycle/test_startup.ep` verifies missing-file noncreation, future-version rejection, preserved data and absence of implicit archive tables. Offline deployment still requires stopping old processes; the check is not a live migration lock.

`app/security/revocation.ep` adds migration 3, host-only audited principal revocation/restoration and authenticated-platform account lookup. `tests/security/test_revocation.ep` covers rollback, denial, replay and restoration; its Discord binding is explicitly a database fixture, not live Discord acceptance. `app/principal_admin.ep` exposes host revoke-principal/restore-principal commands. Production dispatch must re-resolve current credentials and principal authority before side effects.

Principal enforcement now enters `web/devices.ep`, `workspace/sessions.ep`, `agent/tool_dispatch.ep` and image/speech authority helpers. Pre-migration archives retain existing Phase One sandbox rules; this fallback does not grant admin. Migrated archives deny missing or revoked principals. The HTTP fixture contains no model runs; its 200/403 checks qualify authentication, not Discord or full privileged tooling.

`app/security/accounts.ep` adds migration 4 and host-only transactional account link/unlink with immutable receipts. The trusted connector must supply an authenticated platform subject; this storage primitive does not verify a Discord token or event. `tests/security/test_accounts.ep` qualifies identity conflicts, stale commands, rollback and revocation. Session ownership is deliberately unchanged by this authority association.

`principal_account_enroll` in `security/accounts.ep` is a trusted-adapter entry point that creates only ordinary-user identity, using a length-prefixed platform/subject hash and an atomic first-link receipt. It never accepts a requested role. Existing revoked bindings resolve to denial rather than re-enrolment. Adapter authentication must precede this call; browser-provided subject claims are not trusted.

`security/device_links.ep` adds immutable device-link receipts and compares the expected current principal before changing it. Host unlink restores the original device principal only if active. `tests/security/test_device_links.ep` verifies authority changes without ownership changes, rollback, stale replay and credential revocation. Queued action authority binding remains a separate required dispatch contract.

`security/run_authority.ep` adds migration 6 and an AFTER INSERT trigger to capture the owned run’s initiating device/principal/role plus immutable audit counts. Unowned new runs fail rather than borrowing current authority. `run_authority_allows` rechecks the device binding and revisions; pre-migration compatibility permits only the old ordinary-user sandbox, never admin. Dispatcher and execution/media helpers apply it to model runs. Direct Speak uses the additional frozen tool-request binding below.

`security/tool_authority.ep` adds migration 7: immutable `(run, call_id)` authority snapshots captured atomically with tool queue insertion. Direct Speak is checked through `speech_tool_authority` before launch, during monitoring and before attachment publication. Historical requests are not backfilled with assumed authority. Model speech additionally retains the run-level check, so a later tool snapshot cannot elevate an older run. Tests: `tests/security/test_direct_speech_authority.ep` and real Kokoro `tests/media/test_message_speech.ep --principals`. This is not yet generic connector/group authorization.

`security/tool_policy.ep` explicitly classifies the existing callable tools into session read/recap, attachment read, workspace read/create/isolated execution, local media and scoped system inspection. Unknown names and capabilities do not acquire permission automatically. The dispatcher checks both initiating run and queued tool authority; process and image boundaries also enforce the policy. Unknown tool results are archived with no effects and an instruction to choose an authorised registered tool; the agent can continue. Revoked authority cancels the run. This establishes the current catalogue boundary, not privileged host tools, dynamic-tool manifests, connector policies or completed step 05 isolation acceptance.

`tools/host_text.ep` implements the first admin host-read capability using descriptor-based regular-file reads rooted at `/`. The tool parses only path/offset/characters from its saved request. `session_request_body_owned` advertises the schema from frozen run authority and is used by the normal generation loop and compaction acceptance preflight. `tool_policy` classifies host.read as admin; the dispatcher and tool both enforce it. `tests/security/test_host_text.ep` uses real HTTP generation fixtures and actual file reads to qualify guest, admin, revoked, promoted-after-queue and symlink cases. A subsequent real local-model HTTP run called it and returned exact fixture content; the tested executable is now deployed.

`run_authority_observation` in security/run_authority.ep exposes only the requesting run’s effective role and authority status. context/system_awareness.ep includes it in system_inspect, while echo_system_prompt remains unchanged. This observation does not authorise an operation; dispatch continues to use the frozen authority checks.

`memory/session_ownership.ep` defines migration 8 for immutable session-principal ownership and provenance. Legacy session ownership remains with the device’s original principal, even if its current authority link points elsewhere. New ownership insertion atomically captures the authenticated principal. `session_principal_allows` does not bypass ownership for admins. Schema 8 is deployed. Source-principal predicates are integrated into HTTP, attachment, workspace and queued execution paths; memory_search/memory_read retrieve original cross-session sources for the initiating private principal. Live two-chat recall and cross-user denial are verified.

## ErnosDecent memory reference

Inspected `decent_agent/memory.ep` (blob `e5c650126f2134cbc66b322f22633553904d4007`) and `decent_agent/test_memory_tools.ep` (blob `5e66d0d6075ae13563629355f30617f488eec606`) at the pinned ErnosDecent revision above. The implementation provides scratchpad, lessons, timeline, reasoning buffers, synaptic nodes/edges, JSON serialization/reload and embedding-based lesson recall. Its timeline currently trims entries to 8,192 characters and retains 200 entries; Echo’s explicit no-pruning requirement takes precedence. Echo retains complete original events and builds private, source-linked derived records. This inspection does not establish equivalence between Echo’s new graph and ErnosDecent’s synaptic/embedding implementation.

`memory/knowledge.ep` adds migration 9 and source-authorised immutable semantic/episodic assertions over stable principal-scoped nodes. `tools/knowledge_tools.ep` provides memory_record/memory_graph through saved ledger requests; schemas are advertised only when the graph table exists and the initiating principal is authorised. Native, streamed-dispatch and real-model record/recall tests passed; schema 9 is deployed with schema 10.

`memory/knowledge_lifecycle.ep` adds migration 10, immutable visibility decisions and explicit revision checks. `knowledge_visible_sql` applies the latest decision within graph retrieval. `memory_status` and `memory_set_visibility` are exposed only for archives carrying this schema; source events are preserved. Native and streamed-dispatch tests include backup/restore and stale replay. Schema 10 is deployed; the real model and live production graph smoke passed.

## Charter policy verification

`rights/charter.ep` verifies the exact canonical v2 charter hash (`7acfd345d50ad3d459061e7c76b94ff331d2102e3829de885a53df8e4722810c`) before parsing that same byte string. Categories come from the original consent/consultation arrays. Tests reject missing/altered policy and unknown categories. This helper does not authorise execution, derive executable target categories or replace the pending manifest/decision/recovery controller. The source’s 85% compaction clause remains unchanged and explicitly conflicts with Echo’s no-artificial-limit requirement pending the joint review.

## Observer audit protocol

Inspected the complete `decent_agent/observer.ep`, `observer_rules.ep` and `observer_parser.ep` at ErnosDecent revision `4ef202c5a920f17e5d2f6bde406b62e8185b1531`; source blob hashes are recorded in FOUNDATION-VERIFICATION.json. Original source is retained privately. The Observer makes policy judgments through a separate model call. Its parser accepts only an explicit top-level JSON verdict with exact aliases; prose containing verdict words never grants permission. Deployment audits fail closed, while the original nonblocking lookback requires a parsed explicit denial before reporting drift. The source test bypass is not a production authorization mechanism.

`rights/observer_parser.ep` adapts that protocol to Echo's existing JSON parser, additionally rejecting duplicate/conflicting verdict aliases and requiring a nonblank textual reason for deployment evidence. Native tests cover all 91 verdict-key/value combinations and malformed/ambiguous cases. This is a parser only: separate inference, archived audit binding and deployment gating remain pending. Original policy prose has not been installed or changed; final adaptations remain subject to Maria's joint review.
