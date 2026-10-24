> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord rich posts — 22 October 2026

Status: deployed and passed the bounded five-turn native/Discord acceptance on 22 October. This closes neither Step 15 nor Phase Two.

## Contract

`discord_post` retains its requester/channel-owned post IDs and durable delivery history. A new reply can contain text, text-rich embeds, a native poll, or a combination. Subsequent `status` reads the actual owned Discord message, including embeds and poll results. `edit` changes text only; poll-message editing is refused because Discord does not support it. `delete` remains limited to the requesting user's own tool-created post. Ordinary final replies and their Speak buttons use the existing delivery path.

Embeds support title, description, color, fields and a text footer. The validator enforces Discord's per-field limits, 10 embeds, 25 fields, and a combined 6,000 UTF-16 units. It rejects arbitrary URLs, components and unknown nested fields. Text keeps the existing 2,000-unit limit and disabled mentions. Poll input uses a question, 2–10 text answers, duration of 1–768 hours (default 24) and optional multiselect. The application builds the native poll payload; callers cannot inject results or API identifiers. Bots do not cast votes.

Every pending send, including an explicit retry, checks the original stored payload and current requester/bot channel permissions: EMBED_LINKS for embeds and SEND_POLLS for polls in guild channels. The normal delivery worker excludes tool posts, so it cannot bypass these checks. Existing source, cancellation and authority checks run before dispatch. Uncertain delivery is retained for inspection and is never blindly repeated. Rich content cannot select a different channel or requester.

## Qualification scope

- Native boundary/type/schema/mention and retained-allocation checks, plus ordinary post regression.
- Build and generated-root inspection of importing workers.
- Real labelled Discord embed/poll creation, actual status retrieval, unsupported edit refusal, cleanup of explicitly disposable tool posts, follow-up memory/identity and reload persistence.
- Retained request/response comparison, deployment provenance and measured timings.

The checks above passed; the retained evidence and limits are recorded below.

## Protocol sources

- [Discord messages](https://docs.discord.com/developers/resources/message): creation, rich embed limits, allowed mentions and message ownership.
- [Discord polls](https://docs.discord.com/developers/resources/poll): payload, duration, answer limits and poll editing restrictions.
- [Discord permissions](https://docs.discord.com/developers/topics/permissions): EMBED_LINKS and SEND_POLLS.

## First live attempt: interrupted before inference

The first actual fresh-session request (`1562662174256791643`) crashed before a model request or tool call. The native backtrace identifies `continuity_shared_schemas` calling `get_list`; generated code did not root its temporary `provider_items` list while building the shared tool catalogue. This is schema preparation, not lost memory or an API permission refusal. No rich post was created in this attempt.

The application now passes the list through an explicitly typed helper so its lifetime spans allocations. Schema text and audience rules are unchanged. The regression repeatedly compares all three shared history/reasoning schemas across actual garbage collections. The original crash, generated C, zero-request/zero-tool state and explicit exited-child recovery are retained under `evidence/phase2/discord-rich-posts-20261022/memory-crash`. The recovery queues the same original request through normal authority checks; no prompt resend, compiler modification or tool replay is used. The 1,000-iteration regression passed across actual collections; all five importing workers were rebuilt and deployed. The original request and four following turns completed after this repair.

## Actual Discord acceptance

Canonical evidence is `evidence/phase2/discord-rich-posts-20261022/`: `acceptance.json`, `live-turns.json`, `receipt-comparisons.json`, `independent-absence.json`, `ui-checks.json`, and retained build/native/crash records. Five consecutive runs completed: `1562662174256791643`, `1562662250559438879`, `1562664287183306754`, `1562665448997126195`, `1562665483252011041`.

The first two produced exactly one embed and one native poll, confirmed against the actual REST response and rendered Discord cards. The third read both and refused poll editing with `poll-message-not-editable` without replacing it. The fourth deleted only the two expressly disposable posts; independent authenticated reads returned HTTP 404 / code 10008 for both. Delivery history remains retained. The fifth recalled the earlier marker, calculated 43 and used the authenticated username, with no tool calls. Reload preserved normal replies, source-attached Thinking threads and Speak controls. The actual Thinking thread showed reasoning and tool call/result activity.

Native checks covered strict keys/types, delimiter-bearing unknown keys, Unicode limits, poll construction, disabled mentions, retained allocations, original-payload storage, ownership/channel checks and uncertain-send no-replay behaviour. The initially failed standalone schema test import and the original live crash are retained alongside successful repairs; no compiler/runtime modification was made.

## Timing limits

The cold first answer after the catalogue change took 98.790 seconds from dispatch to first output (65,421 new prompt tokens, no prefix cache). Later user-turn first outputs measured 6.442, 4.952, 3.527 and 4.212 seconds. Subsequent tool-cycle answers ranged from 1.155 to 5.106 seconds. Automatic-memory work and queue/recovery waits are separately recorded. These observations do not constitute a controlled comparison with the historical 1.864-second warm median / 31.613 tokens per second, and do not close the performance/endurance gate.
