# Stable shared recall prefix

21 October 2026. Candidate implementation; live acceptance failed and deployment was rolled back.

Request inspection distinguished tool-schema changes from shared recall changes.
Recent deployments changed the tool catalog, which occurs early in the model
prompt and legitimately invalidates most cached tokens. Separately, shared
recall reconstructed its entire automatic-memory message when claims changed.
That message precedes conversation history, causing avoidable recomputation.

The candidate uses the existing per-session snapshot and per-turn delta design
for the exact shared audience. It retains the first authorised base for the
current revision, appends each new claim identity once at its original turn, and
replays those additions from the archive. It does not cap history, shorten
reasoning, remove claims, change the provider capacity or substitute summaries.

Existing access, audience, runtime, visibility, supersession and expiry revision
checks still invalidate retained recall. Shared checkpoint changes now also
invalidate the base, so claims in earlier additions survive a history recap as
part of the new base. The snapshot format revision advances to v3; this causes
a one-time rebase for existing private and shared sessions. Private selection
and delta semantics otherwise remain unchanged.

The first native fixture passed in 10.304 seconds: stable base after a new
claim, exact nonduplicated addition, idempotent capture, reopen, forgetting,
source digests, delegated-child isolation and user/channel/server/DM/private
isolation. It is synthetic data, not model or Discord latency evidence.
Evidence: `evidence/phase2/shared-prefix-20261021/r1-1792595847077079000`.
The expanded shared fixture passed in 9.228 seconds, including a real checkpoint
switch with a labelled synthetic recap. The existing private-prefix regression
passed in 4.216 seconds. Their receipts are `r2-1792595958060426000` and
`private-1792595972630675000`. Tests retain their full diagnostic output, including
pre-existing duplicate semantic-unavailable diagnostic warnings during repeated
capture; the asserted snapshot and archive-integrity checks passed.

Metadata-only request comparison is retained privately as
`request-prefix-comparison.json`. No private prompts or source records are
published. Cold-cache performance, controlled multi-turn measurements and
live recall/correction still require qualification. This does not close Phase Two.

## Live failure and rollback

The stage was installed at idle after native tests. The first live record request
was rejected because the model supplied the wrong source digest; the tool
misclassified that failure as `not-authorized`. The source-integrity gate held.
A follow-up read the correct original digest, but its turn process later exited
with signal 11. The supervisor stopped and required explicit recovery rather
than replaying the request. The root cause has not yet been isolated.

The crashed labelled test was cancelled through its Discord button. Native
recovery verified the retained cancellation barrier and that no tool call was
in an uncertain running state, then closed the claimed job without inference
or delivery. The previous qualified agent was restored, with other components,
databases and resident Warm unchanged. Full logs and consistent database
snapshots are retained privately under this evidence directory.

Native passes do not qualify this candidate for production. No live cache
speedup is claimed. Investigate the crash and the inaccurate digest-failure
feedback before another promotion. The source remains a candidate in the
Phase Two development branch; the live runtime uses the prior qualified build.

## Investigation and digest feedback, 21 October

The captured incomplete stream was reconstructed 100 times in one native reader
(8.135 seconds) and 100 times each in four concurrent native readers (131.851
seconds). Both retained incomplete status without inference or tool execution;
neither reproduced the crash. Concurrent replay reached approximately 11.47 GB
RSS in an observed mid-run sample. These are diagnostic workloads, not live
latency measurements or proof that the crash is resolved. Socket/archive/monitor
reproduction remains pending.

The candidate now distinguishes an incorrect digest for an accessible original
from denied access. It checks access using the stored original before comparing
the caller's digest. Foreign and missing sources still receive the same
non-disclosing denial. An accessible mismatch returns `source-digest-mismatch`,
explicitly records no effects, and tells the model to read the original with
`memory_read` and use its exact returned digest. It does not imply a role or
system-wide tool restriction.

The native group-memory regression passed in 6.295 seconds. The queued tool
execution regression passed in 13.104 seconds: wrong digest produced the precise
feedback, corrected retry recorded exactly one scoped claim. Evidence directories
are `digest-1792596581793077000` and `digest-dispatch-1792596708880923000` under
the private evidence root above. This feedback change is not yet deployed;
production remains on the qualified rollback build.

The next native diagnostic replayed all 354 saved provider events through a local
HTTP socket, event decoder, durable archive and production Discord turn monitor.
Three independent trials passed, each preserving all 354 events and explicit
incomplete terminal evidence; no tool was dispatched. Total times were 34.735,
32.510 and 42.886 seconds on One Touch. A process sample showed archive fsync
work; these external-drive diagnostic timings are not production inference
latency. Evidence: `socket-1792597107574882000`. The crash is still not reproduced
or resolved. The first fixture build lacked the worker-schema import; it failed
before execution and the corrected build passed. This does not authorise
promotion of the shared-prefix candidate.
