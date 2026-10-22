# Discord scheduled events — implementation in progress

This is part of original Step 15, not a replacement milestone. Scheduled-event tooling is not registered or deployed yet. Native guard qualification alone does not establish working event administration.

## Authority and lifecycle foundation

`app/discord/scheduled_event_guard.ep` evaluates event permissions for one authenticated identity at a time. The controller must evaluate both requester and bot, separately from the configured-admin grant, source-message binding, cancellation, and current visibility checks. Creator identity is compared independently for each: bot ownership must never confer requester ownership. External events use guild permissions; voice and stage events use the target channel's effective permissions and require the correct channel type and guild. Ended/cancelled events cannot be reopened; identical statuses must not be replayed as mutations.

`tests/discord/test_scheduled_event_guard.ep` exercises event-type permissions, individual missing permissions, creator/noncreator differences, member/guild/channel bindings and all status pairs including invalid values. Native execution passed on 22 October 2026: 20 permission cases, all 36 tested status pairs, administrator-role scope mismatches, explicit member overwrite denial and malformed operation/type checks. Generated GC roots were inspected. Receipt: canonical `discord-scheduled-events-20261022/native-guard.json`; executable SHA-256 `d292b46e03a4ace4a9e544befa9864c8df13cc6dd8e400b809a1c5ba230f3708`. This is not controller or model-driven acceptance.

Reference: [Discord scheduled events](https://docs.discord.com/developers/resources/guild-scheduled-event), inspected 22 October 2026. Also re-read existing Echo invitation/member/role guards, original ErnosDecent rights source and HIVE scoped-context source at the revisions in SOURCE-MAP.md. Their authority/provenance patterns inform the boundary; their prompts and runtime-specific paths are not copied.

## Remaining controller and acceptance requirements

- Bound list/inspect/create/edit/start/end/cancel/delete and paginated interested-user inspection. Event IDs remain inside the authenticated guild; list results must exclude channels the requester cannot currently view.
- Validate create and merged edit payloads against event type, required fields, explicit timestamps and temporal ordering. Support documented recurrence, image and nullable edit semantics without silently ignoring fields. Numeric recurrence enum takes precedence over contradictory prose examples in the upstream documentation.
- Inspect fresh current state before mutation; hash-bind edits/deletion, reread permissions and source at the mutation boundary, use durable duplicate-effect claims, and preserve uncertain REST outcomes without blind retries.
- Handle event Gateway metadata as events, not user conversation. Subscription identities remain scoped to authorised inspection; do not expose full member profiles in public traces.
- Native argument/response/authority/revocation/race tests; actual disposable event lifecycles through Echo over multiple turns; independent REST verification, cleanup, thread/reply persistence and restart checks. No existing community event is a test fixture.
- Separate unsupported configuration or external permission gates from implementation gaps. Neither a native build nor a direct manual REST request is model-driven acceptance.

All of these remain required before scheduled events can be called complete. Full Step 15 also retains the other administration and moderation work in phase2.md.

## Timestamp and recurrence validation

`scheduled_event_time.ep` parses explicit RFC3339 timestamps without depending on host locale or timezone. It validates Gregorian dates, leap years, clock bounds, timezone offsets and fractional seconds (truncated to milliseconds); missing or unknown offsets are rejected. The controller still must compare times against the current clock and validate merged start/end values. Dates before 1970 and leap-second notation are outside this administrative timestamp format.

`scheduled_event_recurrence.ep` validates Discord's numeric frequency enum, supported intervals, weekday sets, monthly nth-weekday and annual month/day patterns, mutual exclusion and unsupported fields. Omitted/null optional selectors remain distinct from malformed values. Timestamp and recurrence native tests passed. These validators do not yet expose remote event operations.

`scheduled_event_options.ep` parses the nine planned operations with operation-specific arguments, mandatory mutation reasons and fresh state hashes, exclusive user-list cursors, explicit nullable edits, and bounded image-data syntax. Its native tests passed. Image syntax validation is not a claim that the encoded bytes are a valid or Discord-accepted image; live cover-image acceptance remains required.

`scheduled_event_effective.ep` merges supplied changes with freshly observed state for cross-field checks but sends only the explicit patch. It checks future changed start times, end-after-start, external location/end requirements and channel/metadata transitions; creator metadata never becomes part of the outgoing patch. Its native test passed. Source hashes, generated-root review and executable results for all four validators are retained in canonical `discord-scheduled-events-20261022/native-validation.json`. All four builds and native runs passed; no remote event was created or changed.
