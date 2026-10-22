# Discord channel invitations — candidate qualification

Status: native acceptance passed; worker build and real Discord acceptance pending. This is one controller within original Step 15, not completion of that dependency.

The `discord_invites` tool provides list, inspect, create and delete for the authenticated current channel and guild. Both the frozen configured-admin identity and current requester/bot Discord permissions are required. A guest refusal explicitly identifies a requester-role restriction, not a general system failure. Invites for another channel or guild are rejected.

Create defaults to one use and one hour, always unique. Explicit zero allows unlimited uses or no expiry; bounds are Discord’s 100 uses and 604800 seconds. Mutations require an audit reason. Delete requires the hash from a current projected observation. A durable mutation claim precedes network transmission, so uncertain outcomes are retained and never blindly replayed. No target-user grants, application invitations or arbitrary routes are accepted by this controller. These advanced cases are not represented as delivered.

Public observations contain the invite code, current guild/channel and expiry/use metadata; they exclude inviter profiles and unrelated guild metadata. Invite codes themselves enable joining, so do not place an invitation intended for a restricted audience in a broader channel. Native tests exercise cross-guild/channel rejection, route confinement, malformed and unused fields, limits, duplicate codes and metadata projection.

Native binding tests cover host-runtime refusal, forged calls, ordinary-user denial, configured-admin binding and revocation. The execution helper is rebuilt with the same capability catalogue; actual native process execution, cancellation/reaping and revocation tests passed.

Evidence: `/Volumes/One Touch/Echo/evidence/phase2/discord-invite-admin-20261022/integration/`. Native receipts are evidence for their stated cases only. Real create/inspect/list/delete, follow-up, reload and cleanup remain pending.

Primary API references: [Channel invitations](https://docs.discord.com/developers/resources/channel#get-channel-invites), [Invite resource](https://docs.discord.com/developers/resources/invite).
