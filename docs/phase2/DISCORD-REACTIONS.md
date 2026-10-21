# Discord conversational reactions

Phase Two section 4 requires Echo to be able to choose reactions during its ordinary tool loop. The existing queue/steering marker transport was host-owned UI machinery, not a model-callable reaction tool.

The candidate `discord_reaction` tool provides `discover`, `inspect`, `add` and `remove`. It is advertised only in the isolated Discord archive. The authenticated original request fixes the channel, message, initiating user and guild; model arguments cannot supply a destination, user identity, token or REST route. Add/remove affect only Echo's own reaction. Message-wide reaction moderation and reactions to other permitted messages remain part of the wider community catalogue.

`discover` queries current guild and application emoji, filters unavailable/role-restricted guild entries against the bot's current membership, and omits emoji creator account information. Unicode is passed literally and URL-encoded from UTF-8; Discord validates whether the glyph is a supported emoji. Custom emoji must match the newly observed usable catalogue. Remote names are data, never authority.

The controller reuses current run/call authority, admitted job lineage, source-change detection, live channel membership/permission checks, fixed Discord-origin TLS transport and shared rate deadlines. Adding requires Add Reactions permission for the requester and bot; this implementation conservatively requires it even when Discord could allow adding an already-existing reaction without that bit. Own-removal does not confer moderation access. Exact mutations are PUT/DELETE of `@me`, recorded by the ordinary tool ledger; an unconfirmed transfer reports uncertainty and directs inspection. No compulsory reaction or expressive/identity prompt changes are added.

Implementation: `app/discord/reaction_options.ep`, `reaction_tool.ep`, `app/tools/discord_reaction_schema.ep`; common access checks accept an optional operation-specific bit using the same fetched snapshots. Successful HTTP 204 observations now preserve their private response receipt identifier.

## Qualification

Candidate only until the evidence below is updated with actual deployment and Discord results. Native argument/catalogue tests passed (including encoded path punctuation, duplicate/unknown fields, missing role evidence, disallowed/unavailable custom emoji and stripped creator metadata). Existing permission arithmetic/overwrite tests passed. The first isolated options build missed its explicit `provider_quote` import; the import was corrected and the failed build log retained. Full queued-dispatch binding test and the final production worker build passed. The candidate was deployed at an idle boundary; real Discord acceptance is running.

Actual Discord reaction visibility, follow-up inspection, removal, repeat/reload and model-driven emoji discovery remain pending. No live acceptance or completed full community catalogue is claimed by compilation.

References checked 21 October 2026: [Discord reaction endpoints](https://docs.discord.com/developers/resources/message#create-reaction) and [guild/application emoji](https://docs.discord.com/developers/resources/emoji). Local implementation does not expose arbitrary REST requests as a shortcut around the catalogue or current authority.
