# Discord conversation revisions — implementation in progress

The existing browser branch operation requires private ownership and cannot be
used as a shared Discord-session writer. `app/security/group_branches.ep` adds
an audience-bound branch operation and resumable edit/regenerate submission.
It does not copy or rewrite original messages. A branch records the original
session, event, sequence and digest, mode, replacement and requesting device.
The group and membership revision remain unchanged. Source readers follow the
same immutable history boundary used by browser branches.

The operation checks the authenticated current platform/server/channel audience
inside its write transaction. Edit/regenerate additionally requires the original
speaker, a terminal source run and no unobserved tool calls. A continue branch
requires a completed answer boundary. The branch, frozen audience, origin and
selection commit together. Duplicate operation identities return the saved
branch without selecting it again over a newer choice. Changed inputs conflict.
Run submission is separately durable with the same derived identity so a retry
after interruption cannot create a duplicate generation.

Shared attachment lookup now follows only verified branch ancestors with the
same group and membership revision, bounded by their saved event sequence.
Listing, selected-turn manifests and text reads retain original identity/digest;
later parent files stay outside the branch. Private source sessions are excluded.

## Evidence and limits

Native run `group-branches-20261021/r3-1792599511802814000` passed in 8.437 s,
binary SHA-256 `3720fde4a58587a4c46a5e1a10769f4110459ce8ae902936e62b81d5e26dad94`.
It covers original history, later-message/file exclusion, attachment inheritance,
member/speaker distinctions, wrong channel and Host denial, replacement conflicts,
duplicate revision submission, reopen, changed-audience rejection and archive
integrity. The original remains retrievable even when omitted from regenerated
conversation replay; retaining evidence is distinct from repeating the old prompt.

Earlier failures are retained: the first run found an application SQL parameter
count mismatch and rolled back; the second found a test expectation that wrongly
required the original prompt to become unreadable. Neither is counted as a pass.
An older private-browser fixture also failed on its legacy database layout;
the same failure reproduced on the previously qualified baseline, so this is not a new branch-change regression. The failure remains retained.

The candidate now exposes `/branch message:` through immutable registration,
authenticated command binding, acknowledgement and the executor's fresh Discord
access check. The message ID resolves to a delivered final in the exact channel;
the group writer checks current audience rights and records a separate selected
conversation. Confirmation stays in the command response; the next ordinary
channel message continues the branch. The original conversation is preserved.
The command is registered and the corrected candidate is deployed. Native and
multi-turn live acceptance, including an idle restart, passed as recorded below. Edit/regenerate commands and uncertain-work retry
are still separate unfinished controls.
The live memory-cache repair is separate and remains unchanged.

The expanded current-schema fixture passed in 11.908 s, including positive private
browser branching with original history and no later-parent or shared-source
leakage. Evidence `r4-1792599605512192000`, binary
`204ce8828599f725b4f0fa78fc2d338fd5d794858ff225d1eae3e1d15cf56bf0`.
The separate legacy baseline failed in 6.819 s with the same prepare diagnostic
as the modified build (`legacy-baseline-1792599605512239000`). Timings were
collected concurrently and are test durations, not performance comparisons.

## Reply-to-event binding

The candidate delivery path now records an immutable `discord_final_bindings`
row for every reply chunk, not only the final chunk carrying Speak. The row
binds the outbox identity, accepted run, exact final event and body digest.
Resolution requires a confirmed sent message in the exact channel and an
unchanged body. Conflicting rebindings are rejected; interrupted enqueue/bind
can be repaired by the same validated output replay. This lookup supplies
provenance only: command execution must still recheck current Discord access
and the source group's authority before branching. Existing old replies are
not blindly inferred or backfilled from their visible text.

Native transport fixture exercises all four chunks, pending/wrong-channel
denial, conflicting run binding, a later final, journal reopen, unchanged
Speak placement and revoked output authority. Native qualification passed in 10.019 s: receipt
`group-branches-20261021/final-binding-r2-1792600025168968000`, binary
SHA-256 `c69a6181cad4d941e9ebdca36da252d1e926a7533b292171f77d5010abc05757`.
The earlier 9.840 s pass is retained; the final rebuild uses explicit rooted
argument/result lists. These are synthetic transport tests, not live Discord
branch acceptance. This delivery change is now deployed with the corrected branch candidate.

## Command qualification

`tests/discord/test_command_branch.ep` passed the corrected command fixture in
12.561 s, receipt `group-branches-20261021/command-r2-1792600371464660000`,
binary SHA-256 `e6aa3bac29d5cd9a1111cc8e613514ab4f9e48796b2a1ea706b975cb29156fd0`.
It checks an authenticated registered command, required options, a genuine
pending acknowledgement, unsent versus confirmed delivery, wrong channel,
another account's denial, branch selection for the next normal message, and
replay after both databases reopen. Both original and branch archives verify.
The first run passed but used an invalid pending-state fixture; it is retained
and does not establish the acknowledgement case. These tests do not exercise
Discord's live HTTP permissions or the visible slash-command interface.

The extended definition matcher also passed (0.153 s), receipt
`group-branches-20261021/registration-1792600428025184000`, binary SHA-256
`f3626b731bd9ce0c7afdcdd2d622e4f45ac8f08089e9b86a065be8bfae6cf21b`.
This checks fixed definition compatibility and rejects altered option types;
it is not proof of live registration.

## Live attempt and rollback — 21 October

The native-qualified command was built, idle-deployed and registered. An actual
Discord `/branch` interaction completed and selected a distinct session; a normal
follow-up correctly repeated the seed answer. However, inspection of **every**
model request found the automatic background memory-extraction request included
a later parent message outside the saved boundary. The final answering request
excluded it. This is a failed branch acceptance, not a pass based on the correct
visible answer. Evidence: `group-branches-20261021/live-branch-boundary-failure.json`.

The connector was idle-rolled back to the qualified rooted-memory build
`3f21215f2e562cf211ed148db512ca32bac8d46679cb0ab757738dd2252d5784`.
The registered command may remain visible but is not handled by that older
executor. Its failed test branch and original history remain preserved. No Host
or other-user data was involved. The pending repair bounds automatic memory
extraction and source-backed recall by the branch ancestor endpoint, while
keeping explicit authorized original-history retrieval separate. It must pass
native and actual model/Discord checks before redeployment.

The seed turn used 38.778 s prompt processing (25,302 new tokens); the next warm
parent turn used 1.273 s (578 new tokens). The branched turn also ran a separate
10.234 s prefill / 27.580 s decode extraction request, then a 0.994 s prefill
answering request. These are observed component timings, not whole-message
latency or proof that the morning baseline has been restored.

### Boundary repair qualification

Automatic source selection, extraction reads and recalled claims now use the
verified recursive branch ancestry boundary, including omitted edit/regenerate
user events. Explicit audience-authorized original retrieval remains separate.
The final expanded native fixture passes both positive prior-source/claim access
and negative later-parent extraction/recall checks: `memory-boundary-r4-1792601342448747000`,
9.061 s, SHA-256 `93a58806bc0eac5a54990ef8fa473cfbce4252be41150ccfbcc6bcad63aecbea`.
The cross-conversation regression also passes: `cross-memory-1792601287980217000`,
7.805 s, SHA-256 `53d4a1f7310cedb89324994b83ffa80d95cc7f422fe83ace07ad7e534fcd191b`.
Earlier r2 stopped on an invalid test evidence-category argument; r3 passed the
negative checks before the positive cases were added. All evidence is retained.
The repaired agent was subsequently idle-deployed and tested with fresh markers;
see the following acceptance record.

## Corrected live boundary acceptance — 21 October

The corrected native agent (`6972eb09d08ed8f4fd58aa50ed17d20ea3eb5211fa47a57cb712a51ff63751b8`)
was idle-deployed with the qualified command workers. A fresh seed, later parent
message, actual `/branch message:` interaction and ordinary follow-up exercised
a nested branch. The visible answer repeated the correct seed. Both actual
model requests—background extraction and final answering—contained the seed
and excluded the later-parent marker. The completed run and inspected requests
are recorded in `group-branches-20261021/live-corrected-boundary.json`.

An idle connector restart preserved the same binary hashes and resident voice
profile. Discord reload preserved the earlier answers, Thinking threads and
Speak controls. The post-restart follow-up used the same selected branch, correctly recalled
the seed and answered an unrelated arithmetic question. Every actual request
excluded the later-parent marker. Its completed reply, Thinking thread and Speak
control were verified in the reloaded Discord UI.
The first failed live attempt remains recorded above. This does not establish
edit/regenerate, old-message backfill or complete Discord UI parity.

## Edit/regenerate command preparation

The candidate adds separate immutable registration bindings for `/edit message: text:`
and `/regenerate message:`. Parsing accepts only the required named string options,
rejects duplicate names and authority/destination substitutions, and preserves the
replacement text without truncation. Registration definitions do not activate commands.

`revision_target.ep` resolves either an admitted original user message or a confirmed
delivered reply through its immutable final binding. It checks the retained transport
source, original speaker, terminal run with no unresolved tool calls, and current
channel audience. The existing transactional revision writer rechecks those boundaries
before creating a new branch and retaining original attachments. Original messages
and answers remain intact.

The candidate execution path is now connected to public command anchors, queue
reservation, durable run-origin receipts and background dispatch. Production builds
and actual Discord edit/regenerate acceptance remain required. These commands have
not been registered on the server or deployed.

Native qualification `revision-target-r2-1792602321722191000` passed in 11.895 s,
binary SHA-256 `ab84f9ee73e9a68bc1d1fb6488ef6d38b8c1ab5d921e41e4b54c7bec1a586a79`.
It exercises direct and delivered-reply source equivalence, wrong actor/channel
and unknown-source denial, authenticated registration lookup, reordered valid
options, duplicate/unknown/wrong-type/empty replacement rejection, immutable
original history, revision creation and exact replay after database reopen.
The initial run stopped at revision submission because the fixture supplied a
plain label as structured environment data; the corrected fixture uses the
normal channel-environment construction. That failure remains preserved as
`revision-target-1792602207907640000`, not counted as acceptance.

## Revision admission and public prompt delivery candidate

The connector chooses the authenticated interaction-derived run identity through
`group_revision_submit_run`; the existing generic wrapper retains its previous
identity scheme. Revision admission reserves the conversation before creating
the branch/run. The admitted command records its exact intake hash and public
anchor separately from the human actor. The standard worker lineage resolver
accepts ordinary-user revisions while retaining administrator authority for `/auto`.

Edited text is posted without truncation as immutable, sentence-bounded public
chunks attached to the command anchor. Admission waits for confirmed sent receipts
for every chunk. Repeated attempts reuse the same outbox identities and cannot
append another copy. The original prompt and earlier replies remain preserved.
Delivery and Thinking-thread paths check command-specific eligibility rather than
assuming every interaction is administrator-only.

Native receipt `revision-start-r2-1792602810589864000` passed in 15.921 s, binary
SHA-256 `21e4ff73a37729c0745db1d3cbc3048c1e785eec0d7807b6861cccf2387ab2b4`.
It covers busy-conversation admission blocking, an ordinary-user regeneration,
exact original prompt, typed actor/anchor provenance, active replay, cancellation
and database reopen without reacquiring the released reservation, webhook expiry,
authority revocation, and a lossless three-chunk edit followed by revision admission.
The first run incorrectly required an already-applied migration to report a fresh
application; that fixture assertion was corrected and its failed receipt retained.
These are native synthetic transport receipts, not live Discord acceptance.

The existing autonomy-admission regression also passed: receipt
`revision-auto-regression-1792602882243607000`, 16.431 s, SHA-256
`f58911be067cad6fd40319f0e72dd15e1540f12f05b2d797d61198ec8631b56a`.
It retains the current admin gate, busy reservation, sender binding, cancellation
and replay behavior. Callback visibility/recovery regression passed in 5.713 s:
`revision-visibility-regression-1792602966181078000`, SHA-256
`b08522957b78362af4e514c9f7e173d08eacd62d63cb1ae86612bd66d6d8ddcb`.
This preserves public `/auto`, private unrelated controls, exact response binding
and restart compatibility; actual revision ACK visibility still requires live
Discord acceptance. No live deployment or command registration is claimed here.
