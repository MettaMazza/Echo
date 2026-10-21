# Shared session and captured-reasoning reads

21 October 2026. Native shared and private regressions passed; live deployment acceptance
pending.

The previous stage supplied original search/read within an exact shared audience.
Session enumeration and captured-reasoning retrieval still rejected every shared
run. The candidate now advertises `memory_sessions`, `memory_messages` and
`reasoning_read` in that same audience, alongside its existing shared scratchpad.
Private continuity/import semantics are not advertised as shared capabilities.

Private ownership SQL remains unchanged. Shared SQL uses the initiating run,
exact group and audience revision, and existing delegated-session isolation.
Before returning a result, the reader rechecks current authority and audience,
then verifies source-history access and source digest. A named event, run or
request does not grant access. Session listing uses the same audience gate and
checks source-history access before returning a session's metadata. Reasoning
comes only from archived model-emitted fields; missing capture stays explicit.
Unicode offsets preserve exact slices. No private Host context is imported.

The native queued-dispatch test passed in 19.964 seconds. It covers cross-session
session/message enumeration, captured Unicode reasoning, schema exposure,
private/other-user denial, reopen and membership invalidation. Its reasoning is
explicitly synthetic streaming-fixture content. It does not constitute an
actual-model or live Discord result. Evidence is retained on One Touch under
`evidence/phase2/group-continuity-20261021/1792594697971517000`.

This is a read-only continuity extension. It does not implement the separately
reviewed Host–Discord exchange broker, approve new disclosures, finish the
remaining memory UI, or close Phase Two.

The unchanged private-continuity regression passed in 15.535 seconds, including
reasoning retrieval, original session/message access and scratchpad lifecycle.
Its workspace round-trip required a canonical Mac execution directory. Earlier
runs lacked workspace configuration, encountered unsupported atomic publication
on One Touch, or used a symlinked temporary path. Those failures remain retained;
no workspace safety check was relaxed. Final evidence and workspace outputs are
archived in `private-r4-1792594876379664000` under the evidence directory.
