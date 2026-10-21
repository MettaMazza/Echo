# Scoped original recall and shared-context latency

21 October 2026. Deployed after native and actual-model qualification; extended live acceptance remains open.

The first live cross-session recall took 64.768 seconds from intake to final.
Saved provider timings show 55.923 seconds of prompt processing and 2.221
seconds decoding 73 tokens (32.425 tokens/second). The historical morning
baseline was 1.864 seconds to first output and 31.613 tokens/second decoding.
These are different workloads, not a controlled speed comparison. The measured
slow request copied the unindexed shared-history backlog into automatic context.
A separate concurrent extraction request processed 29,427 prompt tokens.

The candidate retains derived shared claims in automatic context and replaces
whole-backlog injection with access to original records through `memory_search`
and `memory_read`. Originals are neither deleted nor capped. Search captures a
fixed sequence boundary and returns provenance plus a continuation cursor. Read
uses Unicode offsets. The initiating run selects the private principal or exact
shared audience; model arguments cannot choose another scope. Source digests,
current authority, audience revision and delegated-session separation remain
checked. Explicit original reads carry claim lifecycle metadata; historical
quotation does not reinstate a forgotten or superseded automatic claim.

Shared tools are advertised separately from private continuity/import tools.
The first qualification attempt exposed missing shared-tool advertisement; this
was corrected. The next trial passed queued search and Unicode reading, then
exposed a fixture attempting a new run in an active session. Its later-source
check now uses the prior idle session in the same audience.

Canonical evidence: `evidence/phase2/group-original-recall-20261021` on One Touch.
Revised native shared-tool dispatch passed in 15.729 seconds; the private
regression passed in 16.898 seconds. These are whole fixture durations, not
model latency. Shared coverage includes Unicode reads, a fixed search boundary,
unindexed sources without bulk injection, private/other-user denial, database
reopen and audience invalidation. Private coverage includes continuation, scope
argument rejection and replay denial. Actual-model tool selection and repeated Discord turns are recorded below.
Broader controlled latency and live tool acceptance remain open. This does not complete
Phase Two or the separately reviewed Host–Discord continuity bridge.

## Actual-model qualification in progress

A real Qwen3.8 inner-loop test passed cross-conversation original search/read,
returned the dated marker and Unicode inscription, and re-read the source after
database reopen. Extraction was intentionally excluded from this fixture so it
could not provide the answer instead of original retrieval. The first trial took
254.079 seconds across both turns and repeated searches before reading. This is
correctness evidence, not acceptable latency. Search now states explicitly that
its results are metadata without message text, identifies `memory_read`, and
explains non-message sequence gaps. The revised trial passed both turns in 148.048 seconds; it used three searches
and one original read on the first turn and re-read on the second. These
whole-test observations are not a controlled throughput comparison. The qualified candidate was deployed while idle, preserving the other five
Discord workers and the resident Warm voice profile.

A separate actual Discord follow-up on the prior deployment visibly returned the
correct marker/date. Provider timings: 21,504 cached tokens, 17,292 newly processed
tokens, 25.280 seconds prompt processing, 0.998 seconds decoding, 33.059 decoded
tokens/second. The earlier and follow-up system messages were byte-identical;
the recall message changed after backlog indexing. These figures reinforce the
prompt-processing diagnosis but do not prove a new deployment fixes steady-state
latency. Canonical `live-prior-followup-metrics.json` retains the measurements.

## Idle deployment and live turns

Agent SHA-256 `a0a24612a359bce0f59ce8d156710c9db540fece692ff2a9fabac74669cf167c`.
The first post-restart recall returned the correct prior-conversation marker/date
and remained visible alongside earlier replies. Two subsequent independent turns
also returned correct answers. No concurrent qualification model request ran during
these measurements. Actual tool-dispatch through Discord also passed: one memory_search and one
memory_read retrieved the original prior-session marker message, with its exact
event ID and digest, followed by a channel-visible answer. Receipt:
`live-original-tools.json`.

| Turn | Intake to first model output | Intake to saved final | Cached / newly processed tokens |
| --- | ---: | ---: | ---: |
| First post-restart recall | 47.233 s | 48.764 s | 7,168 / 32,614 |
| Warm recall follow-up | 1.971 s | 3.339 s | 39,722 / 566 |
| Unrelated warm arithmetic | 1.996 s | 2.947 s | 40,228 / 573 |

Warm decode rates were 32.117 and 32.857 tokens/second. The historical morning
baseline (10 warm requests) was 1.864 seconds first output and 31.613 tokens/second.
Different prompts and cache states prevent treating this as a controlled baseline
comparison. Saved-final time excludes subsequent Discord delivery. All three
answers were separately observed in the Discord UI. The cold/rebuilt-cache delay
is unresolved; this is not a general latency or Phase Two completion claim.
Canonical evidence: `live-post-update-turns.json`, `before-deployment.json`,
`after-deployment.json` in the same evidence directory.
