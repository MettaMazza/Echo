# Channel-public observations

`app/security/channel_public.ep` stores public-channel observations independently of private conversations, personal channel contexts and joined-group contexts. A registered namespace preserves the original platform/server/channel and derives a collision-resistant archive scope. Source event identity, sender, original text, occurrence time and archive digest remain available. Source edits require distinct platform event identities; conflicting replay never overwrites the original.

Only an active device belonging to the host-configured ECHO_SYSTEM_PRINCIPAL can register channels, record observations or persist permission decisions. These are internal connector operations, not public HTTP routes or model tools. The eventual connector must resolve actual platform permissions before supplying a decision; native tests do not establish Discord permission synchronisation.

Permission receipts distinguish viewing ongoing activity from reading historical messages. Reads use the latest receipt and active device/principal binding in the same SQL statement. Continuous view access retains its original visibility floor across permission refreshes. Revocation denies reads; re-entry without history permission starts a new floor. Idempotent replay of an earlier grant does not replace later revocation or re-entry. Each read advances a sequence cursor without an arbitrary total-history limit. Permission receipts themselves are not returned as public observations.

Namespace, observations and permissions use the existing immutable archive and durability mechanisms; no schema migration is introduced. The native test covers connector identity rejection, source replay/conflict, server/channel isolation, new-activity versus historical access, refresh, revocation/re-entry, stale grant replay, restart and device revocation. Evidence: evidence/phase2/channel-public-20261016/.

This component supplies Step 06 storage and scoped retrieval. Actual Discord permission ingestion and the channel-awareness consumer remain to be connected in their planned stages. It is not a live Discord acceptance claim.
