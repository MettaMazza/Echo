# Discord emoji and sticker administration — in progress

Original Step 15 requires guild emoji/sticker administration alongside the
already implemented conversational reaction tools. This work does not replace
reaction discovery or confer administration on ordinary users.

The current candidate `app/discord/expression_guard.ep` implements guild-level
create/manage and creator-ownership decisions using the existing member and
permission parser. The controller must evaluate requester and bot separately;
bot ownership does not grant requester ownership. Managed emoji cannot be
mutated through this path. Unknown creator identity cannot satisfy the
create-permission ownership exception. Source visibility, current configured
admin authority, resource/guild binding and mutation receipts remain controller
responsibilities. The helper is not yet registered or deployed.

Implementation scope: guild list/inspect/create/edit/delete for emoji and
stickers, role restrictions where supported, retained scoped attachment input,
size/format validation, multipart sticker upload, hash-bound changes, audit
reasons, fresh authority, cancellation, uncertain-effect reconciliation and
Gateway invalidation. Never replay uncertain creation. Projection must exclude
unnecessary creator profiles. Existing user resources are not test fixtures.

Qualification will use native permission/payload/isolation tests and real
disposable image/sticker lifecycle calls, independent readback and cleanup.
`test_expression_guard.ep` passes its 15 native permission/binding cases.
The executed binary SHA-256 is
`15a9e1468f4f1800c8de8d23f1e2b331c724fe4bd0ac8eb4c2ee506d5c1953ab`;
the private receipt is `discord-expressions-20261022/native-guard.json`.
Generated C review confirmed that all seven reference arguments and the parsed
fields are rooted across collection, with eight roots popped at cleanup.
This qualifies the permission helper only; uploads, controller integration and
live emoji/sticker acceptance remain unfinished.

References inspected 22 October: [emoji API](https://docs.discord.com/developers/resources/emoji),
[sticker API](https://docs.discord.com/developers/resources/sticker), and
[permissions](https://docs.discord.com/developers/topics/permissions).
Creation uses CREATE_GUILD_EXPRESSIONS (bit 43); managing another creator's
resource needs MANAGE_GUILD_EXPRESSIONS (bit 30). Retained own-resource creation
permission is evaluated independently for each actor. Emoji rate limits require
special care; ordinary returned quota is not a promise that another request
will succeed.

Source review reused Echo member/event/invite boundaries, re-read the pinned
ErnosDecent rights source and HIVE scope blob
`69c59740c40468b7bb1998d82ce7035ce7f37235` (revision in SOURCE-MAP.md).
These inform authority/provenance handling; no foreign prompts are installed.
