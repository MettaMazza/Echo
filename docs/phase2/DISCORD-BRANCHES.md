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

This backend is not yet exposed as a Discord command or button and is not
installed in the live connector. Command binding, response placement, execution,
current authority checks, actual multi-turn UI acceptance and retry recovery must
be completed before marking Discord branch/regenerate/edit parity delivered.
The live memory-cache repair is separate and remains unchanged.

The expanded current-schema fixture passed in 11.908 s, including positive private
browser branching with original history and no later-parent or shared-source
leakage. Evidence `r4-1792599605512192000`, binary
`204ce8828599f725b4f0fa78fc2d338fd5d794858ff225d1eae3e1d15cf56bf0`.
The separate legacy baseline failed in 6.819 s with the same prepare diagnostic
as the modified build (`legacy-baseline-1792599605512239000`). Timings were
collected concurrently and are test durations, not performance comparisons.
