# Scoped original recall and shared-context latency

21 October 2026. Implementation under qualification; not deployed.

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
argument rejection and replay denial. Actual-model tool selection, repeated
Discord turns, restart and controlled latency acceptance remain required before
deployment. This does not complete
Phase Two or the separately reviewed Host–Discord continuity bridge.
