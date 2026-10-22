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

## Scoped response projection

`scheduled_event_view.ep` adds a response boundary for the controller: bind the
event to the authenticated guild and its actual channel, evaluate current member
visibility, and project documented event fields without the full creator profile.
External-event location is projected separately. Recurrence responses allow
documented server-owned fields while rejecting unexpected nested profile data.
Interested-user pages are bound to the inspected event and return account IDs
and names, not full member objects; duplicate IDs and mismatched events fail.

The controller must supply fresh observations and evaluate requester and bot
separately. This helper does not fetch permissions, authorize mutations or grant
access by itself. Controller wiring is now implemented in the working candidate; live event acceptance remains open.
The native projection suite covers denied visibility, mismatched channel/guild/
member, profile omission, malformed and duplicate responses, recurrence nesting
and repeated allocating calls. Final native qualification is recorded separately
in `discord-scheduled-events-20261022/native-view.json`; no production deployment
is implied by that component receipt.

Final native build and execution passed on 22 October, including 200 repeated
allocating calls. Generated C confirms both iterable response lists are retained
as typed GC roots. An initial function-name typo and helper-extraction parse
failure are retained in the private build logs; neither build was deployed.

## Timestamp and recurrence validation

`scheduled_event_time.ep` parses explicit RFC3339 timestamps without depending on host locale or timezone. It validates Gregorian dates, leap years, clock bounds, timezone offsets and fractional seconds (truncated to milliseconds); missing or unknown offsets are rejected. The controller still must compare times against the current clock and validate merged start/end values. Dates before 1970 and leap-second notation are outside this administrative timestamp format.

`scheduled_event_recurrence.ep` validates Discord's numeric frequency enum, supported intervals, weekday sets, monthly nth-weekday and annual month/day patterns, mutual exclusion and unsupported fields. Omitted/null optional selectors remain distinct from malformed values. Timestamp and recurrence native tests passed. These validators do not yet expose remote event operations.

`scheduled_event_options.ep` parses the nine planned operations with operation-specific arguments, mandatory mutation reasons and fresh state hashes, exclusive user-list cursors, explicit nullable edits, and bounded image-data syntax. Its native tests passed. Image syntax validation is not a claim that the encoded bytes are a valid or Discord-accepted image; live cover-image acceptance remains required.

`scheduled_event_effective.ep` merges supplied changes with freshly observed state for cross-field checks but sends only the explicit patch. It checks future changed start times, end-after-start, external location/end requirements and channel/metadata transitions; creator metadata never becomes part of the outgoing patch. Its native test passed. Source hashes, generated-root review and executable results for all four validators are retained in canonical `discord-scheduled-events-20261022/native-validation.json`. All four builds and native runs passed; no remote event was created or changed.

## Controller integration — 22 October 2026, pending live qualification

The candidate now registers `discord_events` only in the bound Discord runtime,
with list, inspect, users, create, edit, start, end, cancel and delete operations.
The controller rechecks the actual admitted message, configured administrator,
requester and bot permissions, guild binding and channel audience before
publication or mutation. Mutations require a reason and, except creation, the
observed event hash. A durable claim prevents automatic mutation replay after
an ambiguous response. Full creator/member profiles are not returned.

Native binding and mutation-plan suites passed. They exercise host exclusion,
ordinary-user and revoked-admin refusals, actual admitted bindings, stale event
hashes, explicit outgoing patches, invalid lifecycle transitions, future dates,
independent creator permissions and stage/private-channel visibility. Receipts:
`discord-scheduled-events-20261022/native-binding-run.json` and
`discord-scheduled-events-20261022/native-plan.json`. Generated controller C was
reviewed for rooted lists across allocating calls.

Integrated worker and matching execution-helper builds passed. The matching helper passed actual execution, cancellation and revocation checks. These
results do not establish live Discord event lifecycle, cover-image acceptance,
recurrence acceptance, interested-user paging or restart/replay behavior. The candidate was installed after an atomic idle check for live qualification; it is not yet accepted as a complete scheduled-event feature.

## First live qualification — failed creation, retained evidence

The actual testing-channel request E22A reached Qwen3.8 and successfully called
`discord_events` list, returning no events. Create attempts omitted required
fields (particularly the start/end timestamps). The raw provider argument stream
matched the archived arguments, so this was not an application parser removing
fields. The controller rejected every attempted mutation before REST. The test
was cancelled with the real Discord Cancel button; the run became cancelled,
and independent authenticated REST readback confirmed zero scheduled events.
Private evidence: `integration/live-E22A-failure.json`. Creation and the remaining
lifecycle operations are **not accepted**.

The candidate feedback change names missing required argument paths and says a
retry must include the complete corrected call, retaining the user's values. Its
native regression test passed, including exact missing paths, complete-input
checks and valid JSON. Generated C retains the required-fields list as a GC
root. The subsequent E22B live retry created the requested external event with exact
name, description, location and start/end times, independently read back from
Discord. Its first call still omitted dates; the named-field feedback enabled
the next complete call to succeed. Private receipts are
`integration-feedback/E22B-create-turn.json` and
`integration-feedback/E22B-create-readback.json`. This improves
error recovery without weakening permission or input validation.

The initial cold request processed approximately 179,000 tokens. Archived request
sizing and source inspection identified all eligible shared claims being copied
into automatic context (bypassing private-memory semantic selection), plus three
large historical system-inspection results. Evidence is retained in
`integration/live-context-sizing.json`. No memory or reasoning was removed, and
no latency fix is claimed by this event-controller checkpoint.


## Follow-up rate-limit defect — 22 October

E22C attempted inspection before a description edit and interested-user read.
The final fresh-state GET repeatedly hit the same Discord rate bucket already
used by the initial GET. Repeating the entire tool call reproduced the failure.
The real Cancel control stopped the run; independent REST confirmed that the
fixture remained scheduled with its original description. No edit is accepted.
The failure and readback are retained in `integration-rate/E22C-failure.json`.

The candidate now shares one event-read bucket keyed by guild, waits for the
recorded server deadline, and retries only idempotent GETs. The wait checks
stop, admitted admin authority, cancellation and source/permission barriers at
100 ms intervals. Missing rate-state or a 429 without a future recorded deadline
returns an error. Mutation requests are never automatically replayed. Final
fresh authority observations occur after the potentially delayed event read.
Native deadline/cancellation/revocation qualification passed, and all five affected
worker builds passed with unchanged application sources. The initial test build
missed an import; its first runtime fixture lacked worker/job schemas. Both
failures are retained alongside the corrected passing test. The candidate was
deployed at verified idle for live acceptance; E22D is now exercising inspection,
description editing and interested-user retrieval. Those results remain pending.
Evidence: `integration-rate/native-wait.json`, `build-qualification.json` and
`after-deployment.json`.


The E22D real Discord follow-up completed. Three event calls inspected, edited
only the requested description using its fresh hash, and read the empty first
interested-user page with limit 1. Independent REST readback confirmed the
name, location, start/end, channel/type and status were unchanged. The final
reply and Speak control appeared in the channel, with the activity thread
attached to the initiating message. Evidence: `integration-rate/E22D-turn.json`
and `E22D-readback.json`. This proves the repaired workflow, not populated
subscriber pagination or the remaining lifecycle operations. E22E is exercising
start on the same disposable fixture; its result is not yet accepted.


## Gateway change markers — candidate

The Gateway previously used intent mask 46593, without scheduled-event bit 16.
The candidate requests 112129 and records that mask atomically with READY.
A retained session with an absent or different mask cannot resume as if it had
the new subscription; it requires a fresh Identify. This is the standard
[scheduled-event intent](https://docs.discord.com/developers/events/gateway#list-of-intents).

All five [scheduled-event change types](https://docs.discord.com/developers/events/gateway-events#guild-scheduled-event-create)
retain guild/event references in the durable journal, with replay deduplication.
Foreign guilds are excluded. Ambient names, descriptions, locations and subscriber
identities are omitted; retrieval still requires fresh scoped REST checks. These
markers neither grant visibility nor create conversational runs. They are a
change stream foundation, not a completed model-facing event-history HUD.

Native acceptance passed all five event types, exact minimal projection, foreign
guild exclusion, replay, reopen and absent/old intent-profile handling. Evidence:
`gateway/native.json`. Worker builds and real subscription acceptance are pending.

## 22 October — lifecycle and Gateway deployment update

E22E start and E22F end passed with independent status 2 and 3 readbacks.
E22G inspected then deleted the same disposable fixture; independent REST
returned 404 / Discord code 10070 (Unknown Guild Scheduled Event). Final
cleanup reply is visible in Discord. Its unsolicited language switch is retained
as a behavioural observation, not a tool failure.

All six Gateway worker builds passed with unchanged application sources.
The candidate was deployed at idle; fresh READY persisted intent mask 112129
and all six workers were observed alive. Native protocol, five event markers,
replay and intent-profile migration checks passed. E22H actual creation/cancel
acceptance is in progress; live marker delivery is not yet claimed.
Evidence: integration-rate/E22F-*, E22G-turn.json, E22G-delete-readback.json,
gateway/build-qualification.json and gateway/live-subscription.json.
