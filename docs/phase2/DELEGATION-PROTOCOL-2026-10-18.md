# Delegation protocol and parent-return repair — 18 October 2026

Status: repair deployed and real-browser three-agent acceptance passed. Phase Two returns to its original Step 10 boundary.

## Observed failure

A completed parent checklist selected `tool_choice: none`. The local provider returned three Qwen XML tool calls as ordinary content with `finish_reason: stop` and an empty structured call list. The application accepted that content as a final answer although three delegated workers were still active. The original response and provider events remain in the immutable archive. This is a protocol/completion defect, not evidence that the child assignments finished.

The same control path required another tool call whenever a parent checklist was pending, including when the remaining work was delegated. That could prevent a natural interim reply and encourage resolving pending requirements merely to return to the user.

The live browser test additionally exposed a feedback classifier defect: successful `materialized` delegation receipts were enriched with an error-like message saying attention was required. The classifier now accepts `materialized`, `already-materialized` and applied guidance without that false failure; task admission remains explicitly distinct from completion. The native feedback regression passed.

Two original children also paged unrelated attachment listings despite receiving an exact attachment identity. A direct read of that identity succeeded; access was not missing. Routing now describes direct source verification through the read receipt.

The browser test also exposed a session-selection defect after intervening chat: the session index selected the newest user objective even when an older parent resumed to deliver agent results. The browser therefore kept tracking a completed chat while the parent was generating. Session selection now prioritises active work, returning to archive order when all runs are terminal. The overlap regression checks selection during chat, resumed parent synthesis and completion.

## Changes

- Resolved checklists keep the provider's structured tool parser enabled (`auto`). Dispatch still rejects extra work after resolution with a correlated tool observation requesting the final report.
- A parent with active children can give an interim reply while its checklist stays pending. When no children remain active, pending requirements still require resolution before final reporting.
- The delegation tool contract explains automatic parent wake, scoped attachment identities and when status inspection is useful; it no longer implies repeated polling is necessary for delivery.
- A final response beginning with Qwen tool protocol is archived as an explicit recovery event, not accepted as a completed reply or executed from text. One recovery attempt is allowed; a repeated malformed response fails with the cause and recovery guidance. Normal structured tools still pass through their durable ledger and authority checks.
- The browser withholds a partial tool-protocol prefix from the visible reply draft while preserving original provider traces. Ordinary text continues streaming.
- Completed child results continue to use the existing durable collection, parent wake and browser conversation revision mechanisms. No child result is fabricated and no original history is rewritten.

## Verification

Native Ernos tests cover resolved and pending checklist request modes, requirements/evidence preservation, and parent guidance; an updated assertion explicitly checks the new interim-reply mode while a child is active. The first rerun retained an obsolete assertion expecting no explicit tool mode before a parent checklist and failed at that assertion; corrected to the intended `auto` contract, the fixture passed.

The native overlap regression passed with active chat → resumed parent → latest completed chat selection, once-only result delivery and durable reopen.

The native malformed-response test passed: raw protocol retained as recovery evidence, no executable calls manufactured, no false final, repeated malformed output stops visibly. The compiled browser draft fixture passed across split protocol tokens, ordinary Unicode text, generation replacement and idle clearing.

Live acceptance uses the actual browser upload and Send controls, two synthetic warehouse source documents and three real local-model delegates. It checks source reads, correction precedence, arithmetic, independent worker results, parent synthesis and visible delivery. The real run completed: all three children read both originals, returned terminal reports, and were collected exactly once. The arithmetic agent also wrote, compiled and executed an Ernos program, read its output, recovered from a rejected checklist wording change, and returned verified totals. An intervening ordinary chat did not prevent later results from waking the original parent. The browser showed all three completed agent dropdowns, switched to active Queue/Stop controls during automatic parent synthesis, and displayed the consolidated report without another prompt or refresh. The final answer included 903 total units, 846 ready, 57 held, 23 ready batches, B19, violet, the P2 dispatch block and both source marker words. No protocol-shaped final was published.

Machine-readable synthetic evidence: [`delegation-browser-20261018.json`](DELEGATION-BROWSER-20261018.json). The full run took 2795.441 seconds under concurrent original research load, including an Ernos compile and a recovered checklist rejection. This is functional evidence, **not an idle latency benchmark or a claim that latency is solved**. The original research assignments were retained and are not claimed complete.

## Deployment boundary

The release was built from the committed Echo 1.5 source plus this repair. Unfinished video work is excluded. The inference service was retained. Existing original workers remained alive at initial deployment. The second deployment moved all three original workers onto the repaired executable only after freezing them and checking that no tool was running. Native lock-verified recovery retained their original partial provider streams and all completed work; each received a recovery event and resumed generating under a fresh worker claim. The browser acceptance workers were preserved.

## Source map

- `app/context/session_messages.ep`: request tool mode, active-child waiting and source-ID routing.
- `app/agent/generation.ep` and `app/agent/run_messages.ep`: malformed protocol retention, bounded recovery and resumed context.
- `app/web/run_store.ep` and `tests/jobs/test_delegation_return_overlap.ep`: active parent tracking across intervening chat and completion.
- `app/ui/client.ep`: partial protocol draft suppression without changing ordinary reply or audio rendering.
- `app/tools/delegation_schema.ep`, `app/tools/delegation_tool.ep` and `app/feedback.ep`: accurate admission, monitoring and automatic-return descriptions.
- `app/maintenance/protocol_repair.ep` and `recover_tool_protocol.ep`: host-only repair for an exact legacy malformed final. It refuses changed/active/cancelled/unauthorised targets, retains original events and claims a new worker. It is not an advertised model tool.
- `tests/jobs/test_task_checklist.ep`, `tests/tools/test_feedback.ep`, `tests/agent/test_unparsed_tool_response.ep` and `tests/agent/test_legacy_protocol_recovery.ep`: native regression coverage.

## Current deployed build

Release: `phase2-delegation-protocol-20261018-r3`. The third deployment replaced only the listener/scheduler, retaining all live worker owners and inference streams.

Server SHA-256: `a1dcbddb167925f5ebda0f37c91c817617258d226a5c354b07d1449fdbd8edbe`.

Client SHA-256: `1be02b4171577c685940e9362743473db3686d21b855723b174f8b21091b1f73`.

The original malformed parent was reopened by an explicit repair event, and returned an ordinary text final under the repaired build. Its research children continue; this does not claim their research is complete.
