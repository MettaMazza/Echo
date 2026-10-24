> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord reviewed message sharing

The native `/share` handler uses the existing memory source-copy writer. It is an
explicit human control, absent from model tool discovery. `preview` takes an
original event ID (`source`) and a destination conversation ID (`destination`).
The private receipt contains the exact selected text, source digest, original
authorship, destination and recipient audience, plus a review digest. Nothing is
copied during preview. `confirm` requires the same source and destination plus
that digest in `review`. A changed selection or audience requires a fresh review.

Only the selected text is copied. Other messages and attachments remain at their
source. The new record retains original provenance and authorship; an assistant
claim does not become a user fact by being copied. Repeating confirmation of the
same actor/selection/review produces the same copy, not another event.

Current scope is conversations available to the authenticated account in the
command's exact Discord guild and channel. Explicit shared audiences work through
current membership checks; admin status is not a membership bypass. Host/private
sessions, other users' silos, other channels and DMs are denied. Cross-channel
review with fresh source/destination access checks remains required for broader
sharing parity; this stage does not silently broaden access. An audience change,
revocation or source mismatch prevents delivery and confirmation.

The native controller checks authority and copies under one transaction. Command
execution also verifies authenticated intake and current Discord access. Replies
are private attachments on the original interaction, with a frozen response and
SHA-256; uncertain uploads are inspected before retry. The selected copy is
independent once confirmed, as in the browser. This feature is not the separate
Host/Discord Echo-continuity bridge.

The handler is deployed and registered. Native controller and command tests passed
exact Unicode copy, source retention, repeat confirmation, frozen interaction
retry, wrong digest, substituted actor, foreign/private sources, changed audience,
revocation and archive reopen. The existing browser HTTP sharing regression passed.
The live Discord preview and explicit confirmation passed, with zero copies before
confirmation and exactly one afterward. The downloaded preview matched its SHA-256;
actual copied bytes and original digest matched the source. No public response or
model turn was created by this human control.

Remaining acceptance includes live second-account/revocation and transport-fault
exercises, richer message selectors, and the cross-channel scope described above.
Discord's native JSON preview has the same Unicode display limitation documented
for project receipts; downloaded UTF-8 data remains correct. This is not a full
Phase Two completion claim. See the release receipt for exact coverage.

## Cross-channel extension deployed and live-tested

The source now supports a reviewed destination in another Discord channel within
the same allowed guild. The source must still belong to the command channel.
Destination namespace and current audience membership are derived from the stored
conversation, never trusted from a supplied channel ID. The delivery controller
checks current Discord access in both channels before preparing a copy, and again
before sending the private receipt. Same-channel requests reuse that one channel
check rather than performing duplicate permission fetches. Host/DM/private and
foreign-account destinations remain denied.

Native cross-channel and existing share-command tests passed, including a reviewed
destination swap refusal. After the UI connection recovered, the updated actions
worker was deployed at idle with all other qualified components retained. Actual
Discord preview and confirmation passed between the testing and general channel
conversations of the ordinary test account. Preview created zero copies;
confirmation created exactly one with the original text, source digest and audience.
Both receipts appeared privately in the originating channel. Earlier same-channel
scope statements above describe the initial stage, superseded by this extension.
Live second-account/revocation, transport faults and richer selectors remain open.
See the separate cross-channel receipt for exact coverage.
