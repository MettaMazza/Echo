> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

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
  Both disposable roles were then deleted (actual HTTP 204 receipts), with the
  conservative deletion barrier terminating each mutation turn. A fresh final
  read completed and confirmed both absent. Independent reads confirmed the
  original two roles and member role list; only the bot role’s numeric position
  retained a gap (1 to 3), with relative hierarchy and all other fields unchanged.
  Final recall returned marker 518, arithmetic 43 and the authenticated username.
  Replies, attached Thinking threads and the final Speak button persisted after
  actual Discord reload. The connector was left with zero active runs.

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


Eight actual turns and eight applied mutations are retained, including the
original failed/cancelled turns and both cleanup receipts. The model’s creation/
edit summary counted four calls although five role calls occurred; the actual
receipts are authoritative. No fixture was recreated, no completed mutation was
blindly retried, and no unrelated server resource was removed. Canonical records
include `acceptance.json`, `receipt-comparisons.json`,
`independent-observations.jsonl`, `cleanup-verification.json` and `idle-state.json`.

The observed median first output across these heterogeneous role turns was
3.442 seconds; cold and subsequent tool requests are mixed. This is not a
controlled comparison to the 1.864-second morning baseline and does not close
Phase Two latency or endurance acceptance. Conservative cancellation on deletion
and unknown prior masks remains explicit; no seamless-resumption claim is made.
