# Channel permission control — implementation and qualification

This extends original Step 15's channel administration tool with `permissions_set` and `permissions_delete`. It does not close Step 15 or the complete Phase Two catalogue.

The same authenticated run, current guild/channel, configured admin identity, tool authority, original-message binding and durable no-replay intent gates apply. Inputs name one role/member and a freshly inspected channel digest. Both requester and bot require current MANAGE_ROLES and channel visibility. Set operations check every requested permission against guild or verified parent permissions, including Discord's target MANAGE_ROLES overwrite exception. Roles/members and parent identity are checked from authenticated REST snapshots. Mask strings are canonical decimals with nonoverlapping allow/deny bits within the existing evaluator's supported published bits 0–52; larger values return an explicit unsupported-input result.

The metadata-audience proof compares validated VIEW_CHANNEL overwrite semantics rather than unrelated send/voice permissions. A send-message restriction therefore does not make an otherwise public channel impossible to inspect or restore. A changed visibility audience still withholds metadata. Permission mutations return acceptance and an independently fetched match/absence result; they do not return newly private channel metadata. Discord's 204 acceptance is distinguished from a subsequent read failure or changed state. Inspect-before-change is an application precondition, not an atomic Discord compare-and-swap guarantee.

No credentials, arbitrary URL, HTTP body, actor identity or authorization overrides are accepted. Audit reason is required. An uncertain mutation remains recorded and is never automatically repeated. Both successful and failed observations remain available to the normal recovery system.

Validation and live deployment remain pending until evidence is appended below. The currently deployed retained-parser stage is preserved until all candidate builds and native tests pass and an idle-only promotion is possible.

Primary references inspected 22 October 2026: [Discord channel permission endpoints](https://docs.discord.com/developers/resources/channel#edit-channel-permissions), [permission precedence and flags](https://docs.discord.com/developers/topics/permissions). Canonical private evidence directory: `evidence/phase2/discord-overwrites-20261022/`.

## Native qualification and candidate deployment — 22 October

Native tests passed strict masks and types, overlapping-mask refusal, visibility versus non-visibility changes, permission assignment through guild/parent scope, the documented target overwrite exception, wrong-identity and denied-role refusal, and 500 verification/audience iterations with actual garbage collection. Existing channel option, projection, private-audience and durable intent/reopen regressions also passed. All six worker builds passed; generated C retains the new lists, parsed snapshots and typed helper arguments through allocation. The compiler was unchanged; its 36 pre-existing group-schema comparison warnings remain recorded.

A read-only authenticated probe confirmed MANAGE_ROLES is already granted to the configured test requester and bot. No role or bot privilege was changed. The six-worker candidate was promoted only while idle, with previous binaries and configuration preserved. Actual multi-turn disposable-channel permission acceptance is now running; native results and a running test do not count as live completion. The first shell build command used the wrong working directory and could not find the compiler; the corrected invocation and its successful results are retained separately.
