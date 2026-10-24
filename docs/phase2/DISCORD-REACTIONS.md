> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord conversational reactions

Phase Two section 4 requires Echo to be able to choose reactions during its ordinary tool loop. The existing queue/steering marker transport was host-owned UI machinery, not a model-callable reaction tool.

The candidate `discord_reaction` tool provides `discover`, `inspect`, `add` and `remove`. It is advertised only in the isolated Discord archive. The authenticated original request fixes the channel, message, initiating user and guild; model arguments cannot supply a destination, user identity, token or REST route. Add/remove affect only Echo's own reaction. Message-wide reaction moderation and reactions to other permitted messages remain part of the wider community catalogue.

`discover` queries current guild and application emoji, filters unavailable/role-restricted guild entries against the bot's current membership, and omits emoji creator account information. Unicode is passed literally and URL-encoded from UTF-8; Discord validates whether the glyph is a supported emoji. Custom emoji must match the newly observed usable catalogue. Remote names are data, never authority.

The controller reuses current run/call authority, admitted job lineage, source-change detection, live channel membership/permission checks, fixed Discord-origin TLS transport and shared rate deadlines. Adding requires Add Reactions permission for the requester and bot; this implementation conservatively requires it even when Discord could allow adding an already-existing reaction without that bit. Own-removal does not confer moderation access. Exact mutations are PUT/DELETE of `@me`, recorded by the ordinary tool ledger; an unconfirmed transfer reports uncertainty and directs inspection. No compulsory reaction or expressive/identity prompt changes are added.

Implementation: `app/discord/reaction_options.ep`, `reaction_tool.ep`, `app/tools/discord_reaction_schema.ep`; common access checks accept an optional operation-specific bit using the same fetched snapshots. Successful HTTP 204 observations now preserve their private response receipt identifier.

## Qualification

Native argument/catalogue tests passed (including encoded path punctuation, duplicate/unknown fields, missing role evidence, disallowed/unavailable custom emoji and stripped creator metadata). Existing permission arithmetic/overwrite tests passed. The first isolated options build missed its explicit `provider_quote` import; the import was corrected and the failed build log retained. Full queued-dispatch binding test and the final production worker build passed. The candidate was deployed at an idle boundary; real Discord acceptance is running.

Actual Qwen3.8 Discord acceptance passed on three consecutive turns: add/inspect (`me: true`), add/remove/inspect (empty reactions), and custom emoji discovery (observed empty catalogue) with correct prior marker and arithmetic. The original reaction, replies, Thinking threads and Speak controls survived Discord reload. The Thinking thread visibly contained recorded reasoning, exact tool calls/results and completion. Native role-limited/nonempty custom catalogues are tested; the live server/application returned zero usable custom emoji, so nonempty custom-emoji use is not claimed as live-tested. The connector was idle at the check.

The first request rebuilt 84,392 prompt tokens after the changed tool schema, with 126.521 seconds from dispatch to first output. The next two user turns took 1.682 and 2.732 seconds; subsequent tool-loop requests took 0.818–0.967 seconds and reused 84,332–86,439 cached tokens. Decode was approximately 28.74–29.99 tokens/second. These are observed per-request timings, not whole-turn latency or a controlled reproduction of the historical 1.864-second/31.613-token-per-second morning baseline. Full latency/endurance acceptance remains open.

The third turn's first model-emitted thought referred to an unrelated software-engineer/no-task situation. The phrase was absent from the inspected current request. It was preserved verbatim, not rewritten by the activity renderer; the actual discovery call, subsequent reasoning, marker recall and answer were correct. Its cause is not established, and these results do not establish universally correct semantic reasoning.

Private evidence: `evidence/phase2/discord-reactions-20261021/`, including source hashes, retained failed/passed builds, native dispatch archives, idle deployment/rollback receipts and `live-acceptance.json`. Agent binary SHA256 `7e2f176488b2e233ae756a6a22a6a4b5171c66651353c84560deeb3ebea41967`. This closes the documented current-request own-reaction slice, not the full Discord community catalogue or Phase Two.

References checked 21 October 2026: [Discord reaction endpoints](https://docs.discord.com/developers/resources/message#create-reaction) and [guild/application emoji](https://docs.discord.com/developers/resources/emoji). Local implementation does not expose arbitrary REST requests as a shortcut around the catalogue or current authority.
