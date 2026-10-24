> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord announcement publication — 22 October 2026

Status: native checks and actual three-turn unsupported-channel/cleanup/ordinary-follow-up acceptance passed; deployed idle. Positive announcement publication remains unperformed because the authorised server lacks a suitable announcement channel.

The explicit `discord_post crosspost` operation publishes an actor-owned bot tool post in the current announcement channel to all followers, potentially in other servers. It is not a normal reply or automatic forwarding. The operation requires a configured admin and current requester/bot permissions, an inspected message hash, unchanged message identity/content, announcement channel type, current source and cancellation checks. Ordinary channels and DMs are refused. Status returns the message hash and publication flag. An already-published response avoids another mutation; durable call intent prevents blind replay of an uncertain call.

Echo's Garden currently reports no Community feature. Successful announcement-channel publication therefore needs a suitable authorised channel; no server settings were changed to manufacture a test. Native checks and an actual unsupported-channel refusal can be independently completed. This external prerequisite must not be reported as a successful publication or a whole Step 15 pass.

Protocol reference: [Discord Crosspost Message](https://docs.discord.com/developers/resources/message#crosspost-message). Own-message publication requires SEND_MESSAGES; publishing another author's message additionally requires MANAGE_MESSAGES. Echo conservatively checks both requester and bot for the latter because the requester is not the bot author.

## Qualification

Native identity, unchanged hash, announcement-only and already-published tests passed. The initial build rejected a reserved parameter name; it was renamed and the failed log retained. All five importing workers built with unchanged application sources; generated roots were inspected. The connector was deployed with no active work, preserving the previous release and Warm profile.

Actual turn 1562677955187179592 created one disposable owned post, inspected its hash, received `announcement-channel-required`, and verified unchanged `published: 0`. No publication intent was created. Turn 1562677998778581073 removed only that post; independent REST inspection returned 404/code10008. Turn 1562678023356940411 retained birch-publish-306, calculated 43 and identified echoskeys correctly. All final replies, source Thinking threads and Speak controls survived Discord reload.

The cold first-output observation was 150.957 seconds after the schema change; the following user turns measured 2.535 and 4.418 seconds. The cold delay is unresolved, not a performance pass. Canonical records: `evidence/phase2/discord-crosspost-20261022/`, including native/build/deployment, generated roots, live turns and negative-acceptance receipts.
