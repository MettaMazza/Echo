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
the private evidence root above. At that test stage the feedback change was not deployed. The independent
qualification and subsequent deployment are recorded below.

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


## Independent digest fix deployed

The digest correction was isolated from the failed shared-prefix candidate:
application and tests from qualified base `1da7c3a`, with only the two feedback
source files and two corresponding tests from `760efad`. The exact-base queued
fixture passed in 12.513 seconds (`digest-base-1792597377349746000`). The resulting
agent SHA-256 is
`7a056b5188034f8640db120206a51fd3d09993969c7e1e69f9a239422bbe577b`.
It was installed at idle, preserving the other five connector components,
provider configuration, existing databases and resident Warm service.

Actual Discord acceptance then passed over three turns. A deliberately incorrect
digest returned `source-digest-mismatch` with no effects; the model read the
original, retried with its exact digest, and recorded exactly one scoped claim.
Its visible reply accurately distinguished integrity from account authority.
The next turn recalled the marker and date correctly; an unrelated arithmetic
turn returned the correct number. Earlier replies and message controls remained
visible. Private evidence is `digest-live-acceptance.json` and
`digest-live-followups.json`, with before/after deployment receipts alongside.

The first recall after writing the claim took 31.625 seconds to first model output
and 34.173 seconds to saved final, including 31.219 seconds of prompt processing
for 20,369 new tokens. Request comparison found the first changed message at
index 1, the shared recall context. The subsequent warm arithmetic turn took
1.487 seconds to first model output and 2.528 seconds to saved final; decode was
31.214 tokens/second. Historical morning observations were 1.864 seconds and
31.613 tokens/second, respectively. These differing prompts are not a controlled
benchmark. The shared-prefix/cache problem remains open, and this independent
feedback deployment does not qualify the failed prefix candidate or Phase Two.


## Rooted argument lifetime regression — 21 October 2026

A small concurrent collection fixture demonstrated that a temporary inline argument list can lose its lifetime protection while another argument yields. Three unprotected runs produced two corrupted-list failures and one segmentation fault. Three equivalent runs using a named list and named intermediate arguments passed. The compiler was inspected, not modified. Application snapshot queries now retain these arguments explicitly. This is a confirmed failure mechanism, not a conclusive attribution of the earlier live crash.

The rooted shared-memory regression passed in 11.876 seconds. The correct private stable-session regression passed in 4.744 seconds, including forgetting, reopen and write gating. An initially selected older private test failed at assertion 24 because it expected unindexed original text to be copied automatically; that result is retained and is not counted as a pass. Existing archive diagnostic output is retained with the successful test assertions.

An isolated three-turn actual Qwen3.8 test passed in 94.315 seconds overall: one original read, one memory record, two checklist calls, then recall and an unrelated arithmetic question. All three runs completed, with production Discord monitoring and concurrent memory preparation, stable-base assertions and archive integrity checks. This total is a fixture duration, not a user-response latency measurement. The candidate agent was built but has not been promoted; actual Discord cache/latency acceptance remains pending.

Private evidence: shared-prefix-20261021/argument-roots, group_context_prefix-rooted-1792598350609972000, private-rooted-1792598655820081000 and live-prefix-rooted-1792598385607926000. Voice conditioning, conversation payloads and databases are not published.


## Live deployment and three-turn acceptance

Agent `3f21215f2e562cf211ed148db512ca32bac8d46679cb0ab757738dd2252d5784` was deployed only while idle, retaining the other five connector workers and resident Warm profile. Actual Discord requests recorded a fictional marker with an original-source read and exact digest, recalled its marker/date correctly, then answered an unrelated arithmetic question correctly. All completed; replies, Thinking threads and final-chunk Speak controls were visible. No crash occurred in this sequence.

The three stored shared bases are byte-identical (32,344 characters). The follow-up carries a 1,404-character addition separately. Its first provider output took 3.865 s and saved final 6.454 s; 59,938 tokens were reused and 2,014 processed. Before this change, the same class of new-memory follow-up took 31.625 s first output and rebuilt 20,369 prompt tokens. These are actual sequential observations with different marker prompts, not a controlled benchmark. The next ordinary turn took 1.508 s first output and 2.826 s to saved final, with 61,892 cached / 604 new tokens and 31.041 generated tokens/s. Historical morning observations were 1.864 s and 31.613 tokens/s on different prompts.

The requested record workflow took 57.666 s over five model calls and four tools; that is not represented as a single-response latency. Reasoning and original tool receipts remain recorded. This qualifies the observed shared-base invalidation repair, not all cold starts, audio switching, endurance or Phase Two. Earlier failed deployment and uncertain root attribution remain in the record. Private receipts: `rooted-prefix-before-deployment.json`, `rooted-prefix-after-deployment.json`, `rooted-prefix-live-acceptance.json`, `rooted-prefix-live-turns.json`.
