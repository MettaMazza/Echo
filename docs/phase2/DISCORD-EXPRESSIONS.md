> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

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

Metadata validation candidate `expression_changes.ep` now separates emoji
name/role changes from sticker name/description/tag changes. Sticker creation
allows an empty description; modification uses explicit null to clear it.
Unknown/duplicate fields, invalid role IDs and duplicate roles are rejected.
The 17-case native metadata regression passes (`native-changes.json`).
Generated C inspection confirms the retained JSON, strings and both input
references are rooted across collection. No controller deployment is claimed. File upload validation, fresh role/guild checks, controller wiring
and actual resource acceptance remain. Official API pages were rechecked on
22 October before implementing these payload distinctions.

`expression_options.ep` adds flat operation arguments and constructs metadata
patches without admitting arbitrary REST paths or upload destinations. Mutation
reasons are mandatory; edit/delete require a complete observed SHA-256. Read
operations reject mutation fields, and creation requires a retained attachment
identifier. The identifier still requires scoped controller resolution before
any use. The separate 13-case native operation test passes (`native-options.json`).

The public projection component passes five native resource/guild binding and
creator-profile exclusion cases (`native-view.json`). Its generated C retains
all ten reference roots; the operation parser retains all fourteen roots.
Sticker multipart assembly reuses the
existing digest-checked frozen-file reader and creates the sticker-specific
name/description/tags/file form. Its native binary/form test passes (`native-multipart.json`), including exact
sticker field names and absence of message-upload fields. Dimensions,
format validation, scoped attachment resolution and controller integration
remain required before any live upload.

The new `expression_access.ep` candidate binds routes to validated guild and
resource IDs, checks requester and bot ownership independently, rejects managed
emoji mutations and verifies requested role IDs against fresh guild roles. Its seven
native tests pass (`native-access.json`). Runtime admin/source/cancellation checks and the
HTTP controller still need integration; these helpers alone grant no access.

Scoped upload preparation now reuses `discord_post_file_source` and verifies
the retained digest before encoding emoji data or freezing sticker multipart
bytes. It enforces the documented byte limits and the Lottie guild-feature
restriction. Discord remains authoritative for image decoding, dimensions and
animation limits; HTTP rejection must retain the original receipt without
automatic mutation retry or substituted content. The native upload test passed (`native-upload.json`), covering scoped emoji/sticker preparation, cross-conversation, exclusion and changed-byte denial. No live upload is claimed.

The HTTP controller candidate compiled and its projection/forged-request test passed (`native-controller.json`). It rechecks configured admin, source visibility, independent requester/bot permissions, cancellation and scoped attachment eligibility before a durable mutation claim. Frozen sticker buffers remain owned until the transfer returns. Schema, discovery and dispatch wiring are now present in candidate source; the registered binding regression passed (`native-binding-initial.json`). Generated C review then found the collection item list lacked a reference root; explicit list initialisation did not produce a root either, so collection projection now traverses token indices directly. The final registered regression passed (`native-binding.json`), including multi-item projection; generated C confirms the seven retained collection references and the added schema reference. These changes are not deployed, and live resource lifecycle acceptance remains pending.

Gateway expression change markers now retain only the allowed guild identifier, never entire emoji/sticker collections or creator profiles. The candidate intent mask is 112137, adding the standard GUILD_EXPRESSIONS bit 3 to the previous mask. Native marker, duplicate/reopen/foreign-guild tests and the existing protocol/scheduled-event regressions pass (`native-gateway.json`). A saved connection with the old mask cannot resume. [Discord Gateway documentation](https://docs.discord.com/developers/events/gateway) was rechecked on 22 October. Live READY and expression lifecycle checks remain pending; production has not changed.

All component and registered native checks above pass. Six Discord workers and the matching execution helper are building from the same candidate source. Deployment, actual Gateway READY with mask 112137, and disposable-resource acceptance are still pending.
