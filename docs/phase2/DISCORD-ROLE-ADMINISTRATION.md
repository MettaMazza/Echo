# Discord role administration — qualification, 22 October 2026

This implements the original Step 15 role catalogue. The configured-admin tool is
registered and deployed for live qualification. It does not close Step 15 or
Phase Two. Discord role creation/assignment never changes Echo's admin-ID list.

The controller supports list, inspect, create, edit, reorder, delete, assign and
remove. Mutations use current guild membership, requester and bot permissions,
role hierarchy, managed/everyone-role refusal, audit reasons and durable intents.
Except creation, mutations require the freshly observed whole-role-list hash.
New roles default explicitly to zero permissions. Permission additions must be
held by both requester and bot. Uncertain external effects are retained, not
blindly replayed. Host catalogue exclusion and immutable Discord request binding
are enforced independently of model-visible schema filtering.

Discord can return equal numeric role positions. The comparator uses its
smaller-ID-first ordering, comparing decimal strings without floating-point or
signed-integer overflow. Reordering checks the proposed position against both
applicable hierarchies. Native tests include equal/older/newer roles, 20-digit
IDs, managed roles, exact permission subsets and repeated allocation pressure.

## Evidence and current limits

Canonical evidence: `evidence/phase2/discord-role-admin-20261022/`.

- Native options, hierarchy and captured-binding tests passed. The binding test
  covers Host exclusion, guest refusal, forged arguments and revoked authority.
- Six workers rebuilt successfully; source snapshots and generated memory roots
  were recorded. The hierarchy/cancellation repair was deployed at idle.
- The original live create succeeded, but a blanket Gateway audience barrier
  cancelled its task. Its mutation receipt remains; creation was not repeated.
  A queued conditional follow-up correctly performed only inspection.
- After repair, another live turn created the missing zero-permission role,
  renamed/recoloured it, reinspected it and completed. Independent Discord reads
  confirmed the exact ID, permissions, name and colour.
- Reordering succeeded, then unknown masks on pre-existing roles conservatively
  cancelled that turn. No member assignment had occurred. A subsequent request
  completed only assignment/removal. Independent member reads captured the role
  appearing and then disappearing; the final member role list is empty again.
  Cleanup and final reload acceptance are still in progress at this checkpoint.

## Gateway classification

Authenticated role events retain role IDs and permission masks. Classification,
prior-mask observation and dispatch cursor commit atomically. A new zero-role or
an update with an observed unchanged mask is metadata; changed masks, unknown or
malformed updates and every deletion remain audience barriers. Role hierarchy
and permissions are still checked afresh by the mutation controller.

A further native-qualified Gateway change seeds the session's masks from the
actual `GUILD_CREATE` role snapshot. Its projection excludes names and member
payloads. Guild reappearance itself remains a barrier, and a fresh session cannot
reuse a previous session's masks. Resumed sessions retain their own observations;
missing observations remain conservative. This additional Gateway binary was
deployed at idle; the other five qualified worker binaries were retained. Deletion remains conservative because
removing even a zero-permission role can change channel-overwrite effects.

Tests cover actual Gateway envelopes, permission changes, malformed/unknown
updates, replay, transactional rollback, new-session separation and guild-snapshot
projection. The initial SQL binding-count failure is retained alongside the
corrected passing result. These results do not constitute the broader Step 16
multi-user revocation/isolation acceptance.

References: [Guild roles](https://docs.discord.com/developers/resources/guild#create-guild-role),
[Discord permission hierarchy](https://github.com/discord/discord-api-docs/blob/main/developers/topics/permissions.mdx),
[discord.js role ordering](https://github.com/discordjs/discord.js/blob/main/packages/discord.js/src/structures/Role.js).
