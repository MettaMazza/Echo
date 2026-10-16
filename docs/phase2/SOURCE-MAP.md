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

`app/lifecycle/migrations.ep` takes trusted host-authored SQL and applies it under a writer transaction with a version/name/SHA-256 receipt. DDL failures roll back; exact repeats are accepted; changed receipts and gaps are rejected. `migration_compatible` refuses newer schemas for older supported versions. It is presently a tested foundation, not yet wired into server startup. Rollback compatibility must be explicitly checked before the first production migration; old Phase One binaries do not magically gain this guard.

The generic migration routine is never a model tool or HTTP endpoint. Caller SQL must not contain its own transaction control or nontransactional operations. All shipped migrations will be fixed reviewed statements, with backup restoration for incompatible rollback.

## Durable operations and principal store

`app/lifecycle/operations.ep` is schema migration 1. Each operation receipt includes scope, principal, request digest, revision, state, stage, cause, known/unknown effects, diagnostic reference, recovery instruction and result reference. State transitions append immutable receipts under a writer lock. Terminal states cannot redispatch, and uncertain states cannot return to running. External connectors must validate result evidence and recheck current capabilities before execution; this journal alone grants no authority.

`app/security/principals.ep` is schema migration 2 after the existing web device schema. Existing devices and future pairings receive distinct user principals; no display name or network fact grants admin. Host role mutations atomically append an authority receipt. Admin and steward roles remain distinct. This module is not yet connected to production authentication or tool filtering.

`app/principal_admin.ep` is a host-local operator command. On a verified disposable/restorable archive, run `principal_admin DATABASE inspect-device DEVICE` or `principal_admin DATABASE role-device DEVICE ROLE UNIQUE_CHANGE_ID REASON`. Use the separate explicit `principal_admin DATABASE migrate` command to apply foundation migrations first. Inspect/grant do not apply them. It is not an automatic bootstrap installer or a public endpoint. Do not use it against production before the complete backup and compatibility gate.

Startup guard: `app/lifecycle/startup.ep` inspects migration receipts before `archive_open`; `app/server.ep` applies it to every executable role. `tests/lifecycle/test_startup.ep` verifies missing-file noncreation, future-version rejection, preserved data and absence of implicit archive tables. Offline deployment still requires stopping old processes; the check is not a live migration lock.
