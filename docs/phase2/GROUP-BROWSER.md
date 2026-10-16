# Shared browser HTTP integration

This is candidate implementation, not a deployed group interface. The production archive remains schema 10. A separately seeded schema-20 archive and isolated server are used for acceptance. The server source now accepts archive versions through 20; it does not migrate production automatically.

All endpoints use the existing bearer-device authentication and exact Origin/Host checks. Browser-created groups use the fixed `browser` / `local` namespace; the channel identifier equals the group identifier. Request data cannot select Discord or another connector's namespace. Membership grants no administrator role.

| Endpoint | Request | Behavior |
|---|---|---|
| `/api/group-create` | `group` | Create an idempotent group owned by the authenticated principal. |
| `/api/groups` | Empty object | List only this principal's joined groups and invitations, with current revision. |
| `/api/group-change` | `group`, `operation`, `member`, `state`, `previous` | Apply an exact-retry membership change against the current revision. An invitation does not grant access; the invited principal accepts their own join. Creator removal and self-departure retain immutable history. |
| `/api/group-open` | `group` | Resolve/create the segment for the current audience. |
| `/api/group-submit` | `group`, `submission`, `message`, optional `attachments` | Bind sender, selected sources and current audience; claim and launch the normal isolated worker. |

The existing session, status, progress, trace and compaction routes admit authenticated recorded group members. Read transactions keep the audience check and returned text in one database snapshot. Attachment routes use the independently tested group scope. Private ownership checks remain separate; branch/revise/export behavior is not implicitly widened by this change.

Only the exact initiating device may claim, retry or cancel a shared run. Being the creator or another member does not grant control over that run. The frozen initiating authority still governs every tool call. Privileged history cannot be repeated after the audience's authority changes.

A ready run with a saved cancellation request and no worker claim now becomes cancelled without starting inference. Running workers retain their normal cancellation monitor; this path does not declare an active process stopped.

The frontend group picker, invitation controls, speaker labels, historical segment navigation and membership-change experience remain to be built and tested. Discord delivery, shared export/revision semantics, repeated group compaction and physical-device acceptance remain separate requirements.
