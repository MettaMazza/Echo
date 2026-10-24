> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Scoped continuity tools — 16 October 2026

## Greeting investigation

The reported greeting’s retained run entered automatic memory extraction and ended with an agent_cancelled event before ordinary reply generation. The archive does not establish a crash or the cancellation trigger. The operator subsequently reported that they might have pressed Cancel and confirmed that a new session worked. Delay itself is acceptable to the operator. A proposed extraction optimisation was reverted before deployment; memory extraction behavior and waiting policy are unchanged by this release. No cause has been invented to fill the missing evidence.

## Primitives

- memory_sessions: one owned private session per cursor call, with title, creation time, message count and next_after. Start at zero and continue to end.
- memory_messages: one original user/Echo message identity per cursor call within an owned session. memory_read retrieves exact text; reasoning_read retrieves captured reasoning for Echo response events.
- reasoning_read: Unicode-offset reads of the original reasoning_content, with the event and archive digest. Empty historical captures are explicitly reported as absent. No retrospective reasoning is fabricated. Existing stream capture remains in place; current real replies already contain reasoning.
- scratchpad: private working notes scoped to the authenticated principal, shared across that owner's sessions. List, read, write/replace and delete are supported. Writes and deletion require the exact previous_revision; new notes use an empty previous revision. Revisions are append-only archive events; deletion tombstones the current note, retaining audit history. This is mutable working knowledge, not a fact assertion or authority source. No new schema migration or unrestricted filesystem access is needed.

## Related corrections

memory_graph remains cursor-based, but now returns next_search explicitly and explains that a single result is one page, not the entire graph. workspace_output_next returns decoded UTF-8 text and next_after. Original bytes remain archived; binary or a chunk splitting a UTF-8 sequence is explicitly returned as hex rather than corrupted text. This release does not coalesce binary stream chunks.

The sandbox previously granted file-read-data on path ancestors of the workspace/runtime. That allowed listing shared ancestor directories while preventing child-file reads. Ancestors now permit only metadata traversal; directory contents outside allowed roots are denied. Session-index tools provide the missing private archive discovery surface instead. This permission change and the scoped index ship together.

Workspace create/read round-trip tests pass; the current tools already support that operation. Generated-image reply inclusion was corrected in the preceding artifact release. This release retains that fix; it does not claim the older missing image display was imaginary.

## Verification

Native dispatcher tests cover session listing/message paging, streamed reasoning capture and read-back, foreign-principal denial, cross-session scratchpad read/edit/delete, stale-revision conflict, UTF-8 output decoding, binary handling, and workspace text create/read. Sandbox process checks deny enumeration of both a sibling workspace parent and the actual runtime data directory while permitting own-workspace shell write/read. Production server and guardian are built from the updated Ernos source; the inference model is not restarted.

## Separate concurrency failure found during acceptance

An automated live test was cancelled without calling the cancellation endpoint; echo_cancel_requests contained no request for it. Service diagnostics include SQLite busy errors. The old cancellation monitor treated node_run_allows=false as a genuine denial even when the underlying schema lookup failed. This is a demonstrated code-path defect, not proof of what triggered the operator's earlier cancelled greeting.

Node checks now preserve a read-error state, which pauses agent work rather than asserting revocation. The monitor retries transient SQLITE_BUSY/SQLITE_LOCKED reads instead of turning them into cancellation. Other monitor failures still return an explicit monitor error. On this Mac, the active archive is on internal APFS, so the deployment selects ECHO_ARCHIVE_JOURNAL=WAL with synchronous=FULL to allow readers alongside the event writer. Default portable storage continues to use DELETE journals. Backups use SQLite's backup API; copying the main file alone is not a valid live WAL backup. No context, response-length or inference-time limits were introduced.

## Final live acceptance and deployment

The final server and guardian passed an isolated HTTP acceptance run against the running local model. Echo invoked session listing and message paging, wrote and read a scratchpad note, created and read a workspace text file, executed a command, retrieved decoded output and attached the file. The download succeeded for its owner and returned 404 for another principal. Temporary acceptance credentials were revoked. This test used an isolated archive with the final binaries; it was not a test against a user's conversation.

A separate exclusive-writer test held a database lock beyond the SQLite busy timeout. The node check reported read-error rather than denial, the cancellation monitor did not cancel, and observation resumed after unlock.

The same final server and guardian were deployed after the active user reply finished. Localhost and LAN health checks passed. The production archive remains schema 10 on internal storage; the inference process was not restarted.
