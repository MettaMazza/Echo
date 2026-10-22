# Discord member administration — 22 October 2026

Original Phase Two Step 15, not a separate completion denominator. This stage
adds configured-admin member inspection, ban-state inspection, kick, ban/unban,
timeout/removal and permitted nickname changes. Invites belong to the remaining
invite controller. This document does not close Step 15 or the integrated Step 16
gate.

## Authority and effects

`discord_members` is catalogued only for Discord-bound runs. An immutable actual
call and the current configured admin-ID list are required at execution. A guest
receives `admin-required`, `restriction_scope: requester`, and
`system_failure: false`; denial is not described as missing system tooling.
The current source channel and guild allowlist remain mandatory. Guild-only
moderation rights are computed from guild roles, not channel overwrites.

Both requester and bot need the operation permission. Ban inspection accepts Ban Members or View Audit Log independently for each; plain member inspection has no extra guild-wide View Channel requirement beyond current source access. Highest-role comparisons
include managed roles, compare older IDs first at equal positions, protect the
owner and prevent self/equal/higher moderation. Timeout also rejects targets
with Administrator permission. Own nickname changes use their applicable
Change Nickname permission; changing another member uses Manage Nicknames.
Changing Discord membership or nickname never grants Echo administrator rights.

Every mutation requires an audit reason and a fresh observed hash. The hash
binds the guild, target identity, current full role snapshot, target membership,
and an explicit ban-observed flag plus (for ban/unban) ban state. Read `inspect_ban` before ban/unban, `inspect`
before other changes. State and requester/bot authority are re-read before
mutating; revocation, cancellation and source changes stop execution. A durable
intent is recorded before the network request, preventing automatic replay of
an uncertain mutation. Retained REST status/body receipts remain authoritative.

A ban defaults to **zero** historical-message deletion. An explicit
`delete_message_seconds` accepts 0–604800. Timeout accepts 1–2419200 seconds;
its absolute expiration is constructed after network rechecks. Reset nickname
with explicit null. Nickname input is conservatively bounded to 32 UTF-16 units,
with valid UTF-8 and no blank names. Unknown and unused fields are rejected.

Individual-member inspection does not require privileged member-list access.
Only an exact Discord Unknown Member/Unknown Ban response is interpreted as
absence. Permission failures, rate limits, other 404s and network failures are
not absence. No membership listing or privileged-intent access is silently
claimed or enabled. Full private REST snapshots are retained; normal results
project identity, username, nickname, roles, join time, timeout and ban boolean.
They do not return ban reasons, unrelated profile fields, or Host/private data.

## Implementation and verification

Application: `app/discord/member_guard.ep`, `member_options.ep`,
`member_observation.ep`, `member_tool.ep`; schema
`app/tools/discord_member_schema.ep`; explicit dispatcher, tool-contract,
capability and Discord catalogue registration. First-party code/tests are Ernos;
no compiler changes.

Canonical evidence: `evidence/phase2/discord-member-admin-20261022/`.
Native hierarchy, typed arguments, operation permissions/projection and
captured-runtime binding tests passed. The binding test covers Host exclusion,
guest refusal, forged calls and revoked admin authority. Extended destructive
boundary tests also passed. Five affected workers built from an unchanged
source snapshot and were deployed at idle; the qualified Gateway is retained.
Seven actual Discord turns completed across three worker revisions. Own and bot member results, ban state, fresh distinct observation hashes, invalid-ID refusal, ordinary marker/arithmetic/username recall and post-restart persistence were checked against retained REST bytes. Final Discord reload preserved replies, source-attached Thinking threads and final Speak controls. Native fixtures do not constitute live destructive moderation acceptance.

The first failed `/new` timed out before an HTTP response and was retained as uncertain, not replayed. A separate explicit command succeeded. Two ban-read rate deadlines recovered through read-only retries; Unknown User HTTP 404/code 10013 was retained as unavailable, never recast as absent membership. Independent observation hashes matched seven results. The initial cold first output was 98.724 seconds; the final revision produced first outputs of 1.471 and 3.185 seconds on its two user turns. These heterogeneous observations are not a controlled comparison with the historical 1.864-second baseline.

Final native permission checks also cover audit-only ban inspection, missing owner metadata and exact absence-code parsing. Hash-mode regression distinguishes an unqueried ban from a confirmed absent ban. The same member is reused only within one freshly collected authority bundle, never from a prior turn. Five final workers rebuilt from frozen source and were deployed only while idle; Gateway, models, Warm voice and grants were preserved.

Positive kick/ban/timeout acceptance needs a specifically approved disposable
member. Existing community members and the server owner are not destructive
test fixtures. No member was kicked, banned or timed out during development.
This is a precise live-test dependency, not a reason to leave independent
implementation or read-only/revocation testing unfinished.

Primary API reference checked 22 October 2026:
[Discord Guild resource](https://docs.discord.com/developers/resources/guild),
Get/Modify/Remove Guild Member and Get/Create/Remove Guild Ban;
[permissions](https://docs.discord.com/developers/topics/permissions).
