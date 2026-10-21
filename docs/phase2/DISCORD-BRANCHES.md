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

## Revision stage deployed for live acceptance — 21 October

The seven production builds from `18baad74a62a6024257d663bd7ecaa216ddd7b5a`
completed. Six worker binaries were installed only after the journal, active-run
and background-worker idle checks passed. The resident Warm service and active
voice profile were preserved. Native registration then successfully installed
`/edit` and `/regenerate`; the first registration attempt failed before network
inspection because its private output directory did not yet exist. That failure
is retained alongside the successful retry.

An ordinary-account `/edit` of a completed human prompt produced its public
command anchor, replacement prompt, Thinking thread and exact requested answer
in the channel. The original remained visible. Both the memory extraction and
answering request excluded the later-parent marker; only the answering request
contained the replacement prompt. Private receipt: `revision-live-edit.json`.

The attempted 4,340-character slash-option paste was rejected by the Discord
client before submission; the shorter live prompt succeeded. The native
three-chunk fixture remains valid, but does not prove a usable long-edit UI.
That input path requires further work. The first edited answer also incurred
38.952 seconds of model prefill for 25,845 new tokens; this is not a latency pass.
Regeneration from the edited answer and the subsequent normal follow-up also
completed with the correct revised context and main-channel replies. All inspected
revision requests excluded the later-parent marker. Private combined evidence is
`revision-live-sequence.json`. The regeneration inserted a 44,428-character shared
recall block and took 44.298 seconds of prefill; branch recall/cache behavior needs
repair. Post-restart conversation continuity passed; cancellation on command anchors
remains open. This stage is not full UI parity or
Phase Two completion.

The normal follow-up after regeneration retained the selected revision and
returned both its result sentence and the correct new arithmetic answer. Its
provider prefill was 1.331 seconds for 627 new tokens with 70,511 cached tokens,
32.15 decoded tokens/second. This is one observed warm turn, not a completed
controlled benchmark. An idle restart preserved the exact six worker hashes;
Discord reload retained the original, edit, regeneration, follow-up and controls.

The subsequent post-restart normal message returned the revised result sentence
and the correct new sum. The selected revision session and request provenance
survived restart; the response and Speak control were verified in Discord.

## Branch recall-prefix repair candidate

The observed revision prefill gap comes from snapshot reuse being limited to one
session. The candidate extends shared-memory snapshot lookup to verified branch
ancestry. Every reused record must equal a record in the current, freshly filtered
recall view. It retains the existing visibility, corrections, expiry, runtime and
checkpoint revisions. Private-memory lookup is unchanged.

Ancestor additions are replayed only against the same base and saved boundary.
Additions from omitted turns are supplied again on the current turn rather than
silently discarded. Deduplication requires a terminal source turn that the normal
history renderer can actually replay. Neither same-audience sibling snapshots nor
unbounded parent history qualify as prefix sources.

The expanded native fixture covers inherited base, relocated omitted-turn additions,
excluded edited-out source, nested revision replay, reopen, forgetting, checkpoint
rebase and the existing audience/delegation/private isolation cases. The initial
expanded run passed in 10.070 seconds; the separate private stable-recall regression
passed in 5.260 seconds. The replay-eligibility test passed in 10.680 seconds. Final candidate ordering
prefers the nearest verified ancestor, retaining that parent's prefix rather than
switching unnecessarily to a more distant ancestor. Final native receipt
`branch-prefix-final-1792604247603968000` passed in 10.028 seconds, SHA-256
`aef058517f299e58c8d04d740e7dfd2d8554ed329f71e09677eef403378cf42c`.
The fixtures retain diagnostic duplicate-event probes in their complete logs;
only the asserted, terminal successful outcomes are counted here.
No production deployment or live latency improvement is claimed for this candidate.

### Live candidate failure and rollback

The native-qualified candidate was installed idle and one regeneration completed
with a visible correct reply. However, receipt validation found its snapshot had
an empty value after `records:` and was not valid JSON. That invalidates the
apparent 2.898-second prefill result: no latency success is claimed. The subsequent
normal follow-up produced a valid snapshot, but does not erase the earlier failure.
The cause remains unresolved. Private archive/journal copies and exact requests
are retained under `branch-prefix-live-failure`.

The previously qualified revision worker was restored at idle, preserving Warm
and all archived originals. The candidate source remains published for diagnosis,
not claimed as the deployed repair. Native synthetic passes did not cover this
live failure; the next investigation uses the saved archive and validates every
snapshot rather than only final answers and timing receipts.

Saved-archive diagnostic `test_saved_group_records.ep` then performed twenty
fresh shared-record reads for the failed run. All returned valid arrays in
1.457 seconds. This narrows the investigation but does not reproduce or resolve
the capture-time empty value. The diagnostic did not invoke a model or transport.

## Snapshot validation repair candidate — 21 October

The saved live archive contains two malformed snapshots with an empty raw
`records` value, including one predating the rejected branch-prefix deployment.
The source of the intermittent empty read is not established. The capture path
previously read shared memory twice and could serialize a failed read as JSON.
The repair uses one validated shared selection for both rendered content and
record provenance, rolls back on invalid records, checks existing receipt shape,
and guards historical JSON queries. Snapshot revision v4 retains older immutable
receipts while constructing fresh context; it causes an initial cache rebase.
No memory wording, voice profile, model selection or compiler was changed.

Native branch regression, including a deliberately malformed historical receipt,
passed in 11.004 s (`test_branch_context_prefix-v4-1792605689613656000`, binary
`f431c650ea5e664e48f868cfa7273cdd029774eb146391eba4004725443a444d`).
Private recall regression passed in 4.366 s
(`test_stable_session_recall-v4-1792605700897489000`, binary
`8a9213dc9d2b599acc6fb513d845077b60d455038b09881957f80defc2e58de4`).
Expected missing-event and duplicate-event diagnostics remain in the raw logs.

`test_saved_group_capture.ep` ran on a disposable private copy of the failed
archive, for both affected runs. Both captures and idempotent recaptures passed
(0.425 s and 0.278 s). Independent JSON parsing confirmed 50 records and 44,428
characters of corresponding recalled content in each new snapshot. Receipt:
`group-branches-20261021/saved-capture-v4-final-1792605722546121000`, binary
`bce4b70571ccade4ac1be19801a92276600a5981552e10d027d85e72153392c2`.
The two original malformed receipts remain unchanged. This is saved-state and
native evidence, not live Discord or latency acceptance. Production remains on
the previous revision worker pending candidate build and live qualification.

### Live snapshot repair acceptance

Source `d71c44e702ff8146e6c0330985ecdb0d4cb6eb9a` built in 74.259 s;
agent SHA-256 `b77791bcce3f5bde2b7dcd1fb25a8b604bef29dcf6a1fb46ff9596a3cdc8eef9`.
The idle-gated stage preserved the other five connector binaries and resident
Warm. Actual Discord regeneration, normal memory follow-up and nested regeneration
completed with correct marker and arithmetic replies, visible Thinking threads
and Speak controls; all persisted after Discord reload. All five model requests,
including extraction, excluded the future-parent marker. All three new snapshots
were valid with identical 44,428-character bases; record counts were 50/51/51
and per-turn delta lengths 0/1,324/1,324. This establishes actual branch reuse
with preserved recall for these turns.

Answering requests: prefill 1.028/2.019/1.029 s, first output from archived model
request 1.233/2.218/1.234 s, decode 31.80/31.87/31.86 tokens/s. The historical
morning median was 1.864 s first output and 31.613 tokens/s; these three different
requests are not a controlled equivalent workload or full performance acceptance.
The earlier 38–44 s branch-prefill behavior was absent in this sequence.
Private evidence: `group-branches-20261021/snapshot-v4-live-sequence.json`.

The first distinct slash interaction timed out during HTTP acknowledgement
(curl 28, HTTP 0) and admitted no run. Discord displayed “The application did not
respond”. Its uncertain acknowledgement receipt is preserved separately in
`snapshot-v4-ack-timeout.json`; it is not counted as a successful interaction.
A later distinct interaction succeeded. Acknowledgement reliability and the
remaining command Cancel, long-edit input, retry, broader UI and full Phase Two
requirements remain open. Production is on this validated snapshot stage; no
claim of complete Phase Two or complete latency/endurance qualification is made.

## Cancel controls for command runs — candidate

The existing status reply and immutable Cancel binding now support authenticated
command sources as well as ordinary messages. A command control replies to its
public bot-authored anchor; the human actor remains the original command user.
Click resolution checks the exact delivered control, channel, audience and actor,
then refreshes channel access and command-specific authority. Cancellation records
an immutable source receipt; the running agent's existing source monitor observes
it. Completed command work cannot be mistaken for an ordinary queued message.
Controls disable after completion without editing the immutable command anchor.
Command status refresh also continues ordinary queue work after a settled update.

Extended revision-admission fixture passed in 17.775 s, including current-command
control selection, actor/channel rejection, exact source cancellation, idempotent
replay, monitor barrier, terminal control position, anchor-reference validation
and removal from refresh selection. Receipt:
`group-branches-20261021/test_revision_start-command-cancel-1792606466867332000`,
binary `95b119328e6109f0feacd5fd535734ef7552c769b178f62a2eedb00d0de4fc9e`.
The ordinary Cancel regression passed in 2.561 s, receipt
`test_cancel_button-command-cancel-1792606338191420000`, binary
`50800a2e2d9b3ec0dfe2a211e1d2ef57707d9ae7ae24ef60b8274b1bf60698c4`.
No live command-button click or deployment is claimed yet. Build dependency
inspection identifies agent, actions, activity and queue-controls as affected;
Gateway and acknowledgement workers retain their qualified binaries.

## Command Cancel — deployed live acceptance, 21 October

The four affected workers were built from `9c4031c8` and idle-deployed as
`command-cancel-stage-20261021`; Gateway/acknowledgement and resident Warm were
preserved. The actual ordinary-account `/edit` long-output request entered
`generating`. Its public source-bound Cancel button was clicked; the run became
`cancelled`, the button disabled, and Discord displayed the cancellation receipt.
No final reply was delivered for that cancelled run. A subsequent normal message
completed with the exact requested response and retained Thinking/Speak controls.
Private evidence: `group-branches-20261021/command-cancel-live.json`.
This closes the live active-command cancellation case; second-actor, revoked
permission and real-admin autonomy exercises are not claimed by this sequence.

### Acknowledgement timing investigation

A later regeneration received no confirmed callback (`curl 28`, HTTP 0) and
admitted no model run; Discord showed its application-timeout notice. This
failure is retained, not treated as successful command completion. Five
read-only public API probes took 0.123–0.237 s; the native authenticated REST
probe took 0.308 s with 8 ms setup. These later probes do not establish the
cause of the earlier callback failure.

The acknowledgement worker now retains immutable timing receipts after the
transfer, including intake/preparation timestamps, remaining deadline, transport
setup, DNS, connect, TLS and first-byte durations. No token, URL or conversation
text enters these receipts. Callback state remains authoritative; uncertainty
still uses read-only reconciliation and never resends a callback. Native
expiry/reopen/immutable-receipt regression passed in 2.287 s; binary
`30487a4b2713dca45a6327f1041a591a04d43ca3644e4039bc49606cdac72afe`.
Only the acknowledgement binary changed in the idle deployment; its SHA-256 is
`59ec9146efbc59ae51d16e368b9e4254b242ddd77ca5dd0a56b32c20ee1ea126`.

The instrumented regeneration and subsequent `/history` both completed visibly.
Acknowledgement preparation took 96/90 ms and network transfer 691/771 ms.
The completed regeneration's Cancel disabled and its final reply retained Speak
and the attached Thinking thread. Private receipt: `ack-timing-live.json`.
The earlier timeout remains unresolved; these successes do not prove its cause
or close acknowledgement reliability.
