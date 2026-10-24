> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Error feedback audit — 18 October 2026

Status: error-feedback changes deployed in `phase2-error-feedback-20261018-r2`. Live HTTP and real-model recovery checks passed. No claim that all latency is resolved or every individual source error branch was exercised. The user subsequently authorised verification and relaunch idle.

## Observed incident

The requested shift log exists as `stream-shift-log` in the same owner's persistent notes. The new session attempted a different name, `shift-earnings-log`. The exact lookup returned only `end`. An earlier listing attempt required a task checklist; after recording the checklist, the model retried the guessed lookup instead of listing notes. This was not evidence that cross-session notes had disappeared.

The read path now distinguishes a missing exact name from list exhaustion. It tells the caller to list notes, follow pagination, and use the returned name before creating a replacement. The tool description also states this discovery flow. Original notes and revisions are unchanged.

## Shared feedback changes

`app/feedback.ep` supplements original JSON receipts with an explanation, effects, recovery and reference. Existing specific effects and recovery remain authoritative. Unknown outcomes remain unknown; a failed response does not establish that a write did not happen. Successful process capture with a nonzero exit receives explicit unsuccessful-command guidance. Captured HTTP error pages receive transport guidance.

The enrichment is connected to tool dispatch/recovery, model and run failures, worker status and HTTP error responses. Browser error handling preserves structured responses, including service failures, and keeps the original code visible. Empty or malformed tool output retains its raw evidence in an invalid-result receipt. Access, integrity and conflict errors do not recommend bypasses or blind retries.

Inventory: 641 literal JSON status-return sites and 168 distinct codes in the source snapshot. This is an inventory, not 641 individually exercised tests. Dynamic/native errors and UI exceptions require boundary verification as well.

## Verification so far

- Native Ernos feedback fixture passed preservation of original fields, effects, custom recovery, idempotence, valid JSON with trailing whitespace, nonzero process exits, unknown errors, nested HTTP failure and lookup explanation.
- Six checks against the compiled Ernos browser functions passed: structured access denial, structured service failure, incomplete JSON, network failure normalization, existing active-session handling, and normal success. Controlled responses and DOM doubles were used; these are not live visual-browser tests.
- Cross-session note regression build succeeded. Its first execution stopped at the existing checklist prerequisite before writing a note (exit 5), so it did not validate continuity. The fixture now records its task before dispatch. The rerun exited 0 and reported PASS for cross-session read/edit/delete/conflict/isolation, session paging, reasoning lookup and workspace roundtrip. It also emitted an archive preparation diagnostic; this diagnostic still needs attribution before treating the run as clean.

## Live verification and release

- Final server SHA-256: `038673a9c69a39fe4d0a6bdaa48be909354ccc62980942899e06f60d1e84be61`.
- Live HTTP 403 access failures, 400 malformed JSON and 404 missing session returned explanation, effects, recovery and reference.
- Real-model run `41a50590-ad5d-4dcc-b1b4-d6273843f6e0` used the actual compiled browser submission functions, HTTP API, tools, model and archive. It encountered checklist-required, recorded its task, read the deliberately nonexistent note, received the exact-name guidance, listed the first page, completed its checklist and returned a final answer visible through the session API. No existing note body was read or edited by this test.
- Browser checks used DOM doubles; no visual-browser claim is made.
- A proposed recall-placement change was rejected after live comparison: the follow-up took 22.9 seconds versus a prior 14.4 seconds. Final release restores the previous recall placement. No memory/history or context limit was reduced.
- Final real two-turn check: replies visible in the session API after 22.1 and 14.5 seconds. Follow-up first visible text 13.9 seconds, fresh prompt processing 8.54 seconds for 5,507 tokens. This does not establish a latency improvement; latency remains an outstanding issue.
- Runtime evidence is retained under `work/echo-runtime/maintenance`: `error-feedback-live-http.json`, `error-feedback-live-tool.json`, `error-latency-before.json`, `error-latency-after.json`, `error-latency-final.json`, and `error-feedback-idle.json`. Local runtime paths are not a portable public test suite.

The final idle restart cancels any remaining diagnostic runs and checks both inference services for idle slots. The model stays loaded for manual testing. Original events and notes are preserved.
