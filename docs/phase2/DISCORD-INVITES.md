# Discord channel invitations — candidate qualification

Status: native and real three-turn invitation lifecycle acceptance passed, including reload and cleanup. This is one controller within original Step 15, not completion of that dependency.

The `discord_invites` tool provides list, inspect, create and delete for the authenticated current channel and guild. Both the frozen configured-admin identity and current requester/bot Discord permissions are required. A guest refusal explicitly identifies a requester-role restriction, not a general system failure. Invites for another channel or guild are rejected.

Create defaults to one use and one hour, always unique. Explicit zero allows unlimited uses or no expiry; bounds are Discord’s 100 uses and 604800 seconds. Mutations require an audit reason. Delete requires the hash from a current projected observation. A durable mutation claim precedes network transmission, so uncertain outcomes are retained and never blindly replayed. No target-user grants, application invitations or arbitrary routes are accepted by this controller. These advanced cases are not represented as delivered.

Public observations contain the invite code, current guild/channel and expiry/use metadata; they exclude inviter profiles and unrelated guild metadata. Invite codes themselves enable joining, so do not place an invitation intended for a restricted audience in a broader channel. Native tests exercise cross-guild/channel rejection, route confinement, malformed and unused fields, limits, duplicate codes and metadata projection.

Native binding tests cover host-runtime refusal, forged calls, ordinary-user denial, configured-admin binding and revocation. The execution helper is rebuilt with the same capability catalogue; actual native process execution, cancellation/reaping and revocation tests passed.

Evidence: `/Volumes/One Touch/Echo/evidence/phase2/discord-invite-admin-20261022/integration/`. Native receipts are evidence for their stated cases only. Actual runs 1562721286134702141, 1562721323107614751 and 1562721357110968321 completed. Five invitation results independently matched retained Discord responses and hashes: exactly one create, two inspections, one delete and one final list confirming absence. All three replies and Speak controls persisted after reload; the cleanup source message opened its thinking/tool thread. No pre-existing invitation was mutated. The normal follow-up preserved iris-731, arithmetic 43 and the authenticated account name.

Primary API references: [Channel invitations](https://docs.discord.com/developers/resources/channel#get-channel-invites), [Invite resource](https://docs.discord.com/developers/resources/invite).

## Separate timing finding

The associated temporal test exposed a distinct admission-versus-receipt bug: two queued messages arrived 8,056 ms apart, while their runs were admitted 143,899 ms apart. The first candidate called the latter a message gap, and Echo repeated it as though it measured sending time. The arithmetic was internally consistent but the meaning was wrong; the temporal requirement is not accepted on that evidence. A correction records immutable gateway receipt timestamps separately, labels admission times explicitly, leaves missing historical receipts unknown and forbids inferring unmeasured active/computing duration. Its dedicated native queue fixture, receipt provenance/idempotency test, rebuilt integration and real three-turn queued-message regression subsequently passed. The corrected second turn reported 9,291 ms between gateway receipts versus 163,772 ms between admissions; both match underlying records. Reload and ordinary continuity passed. See LESSONS-TIME-MODERATION-2026-10-22.md for remaining temporal scope.

Cold first-output latency was 147.682 seconds for the changed request/context; later model steps were approximately 1.9–7.8 seconds. These heterogeneous observations do not establish parity with the historical morning baseline.
