# Mid-task questions and continuous autonomy

Requested by Maria on 18 October 2026. These are general conversation controls, independent of any paper, project or topic. First-party application and tests remain Ernos Plain.

## User experience

- Send `/auto` to start a continuous run, or send it during an active run to enable autonomy there. Echo receives an explicit runtime instruction that it can freely choose and pursue activities for its own sake, using its existing system. A completed answer is saved and followed by another turn without requiring another human message.
- Echo can call `autoOff` with a reason whenever it wants to stop or encounters an issue. It can then give a closing reply; no further autonomous continuation follows.
- Send `/autoOff`, use **Turn autonomy off**, or use the existing **Stop** control to disable autonomy and cancel the active run. The UI distinguishes a stop request from completed cancellation.
- `request_user_input` presents one to three questions together. Each may offer zero to three suggested answers. The first choice is preselected locally; **Other** always permits a typed answer. Only **Send answers** sends anything. Free-text-only questions are supported.
- Question drafts survive reload. A question pauses inference until an explicit answer or new authenticated steering arrives. No answer is inferred from elapsed time or a selected default. Existing steering can replace the question without leaving the run stuck.

## Persistence and permissions

Autonomy state, questions, answers and continuation provenance are immutable archive events scoped to the current run. Original human messages are not rewritten. Automatic continuations are labelled application-generated and linked to their preceding final answer. Final publication and continuation commit atomically; the browser cannot observe a transient completed state and accidentally start a competing run.

The same initiating authority, tools, privacy rules, ownership and dispatch checks apply in autonomy as ordinary prompted work. There is no role escalation, new credential, permission bypass or inherited mode on unrelated delegated tasks. Only authenticated human command intake enables the mode; source text and model tool arguments cannot enable it. The model's tool can only disable it.

Direct question forms and autonomy tools belong to the main conversation. Delegated workers return questions to their parent. Group controls retain the existing initiating-speaker and membership rules. Restored archives remain read-only.

Question submission validates the exact archived batch, unique question IDs, option indexes, explicit Other text, current run and authenticated controller. The answer and resulting guidance commit together. Identical retries are idempotent; changed duplicate submissions and stale or cancelled questions are rejected. A later steering message supersedes the form. Questions are rendered as text, not HTML.

No iteration/token ceiling is introduced. Existing context compaction continues to apply. The question wait cooperates with cancellation and consumes no inference requests. An exited/erroring autonomy loop is disabled; abandoned-worker recovery also disables autonomy for review instead of silently restarting free-choice activity. Original results remain available and ordinary recovery still applies.

## Validation

Initial native controls test passed: three-question validation, invalid and duplicate options, outsider rejection, explicit answer-to-guidance delivery, retry idempotency, reopen persistence, autonomous continuation and human/model stop. Expanded native checks passed (`interaction-controls-build-r2.log`, `interaction-controls-2.sqlite`). The final server compiled successfully (`interaction-server-build-r3.log`). A real-model browser run delivered all three explicit answers, including Unicode café and marker ORCHID-947, and completed. Draft choices survived reload; empty Other submission was rejected. Final autonomy-cycle acceptance and production deployment remain pending.

## Visual refinement

The question card and autonomy controls use the existing light/dark theme tokens, surface and border colours, typography, rounded corners, and primary/secondary button hierarchy. Options are compact native radio choices with accent borders for selection and visible keyboard focus. Other reveals a text box; free-text-only questions show the text box directly. The composer becomes part of normal document flow while questions are present so it cannot obscure the form. History restoration brings the pending questions into view. Original drafts and explicit-submit semantics are preserved.

Visual inspection covered a real pending question in the light theme and an isolated fixture using the same generated renderer in the dark theme. Evidence: `maintenance/interaction-acceptance/questions-polished.png` and `questions-dark-preview.png`. The latter is a visual fixture, not a live service-success receipt.

## Task mode, session transfer and injection boundaries

`/auto <goal>` retains that exact goal and instructs Echo to continue it until complete, call `autoOff`, and deliver the result. Bare `/auto` retains free choice. A new enable atomically disables other active autonomous runs belonging to the initiating principal (or exact device in legacy archives) and requests their cancellation. In-flight effects are not described as rolled back. Replaying an earlier enable acknowledges its original event without stopping a newer autonomous run. Global `/autoOff` targets owned autonomous runs; it does not cancel an unrelated ordinary foreground task.

The UI permits switching conversations while autonomy is active and shows a background-autonomy indicator with open/off controls. Messages in the autonomous conversation become steering instead of waiting indefinitely in the ordinary message queue. A generation-local monitor is being validated to interrupt blocked inference before HTTP headers arrive, preserving the run and its goal.

Prompt-injection defence has separate layers:

- Tool observations retain their original data inside a runtime-owned JSON source envelope when presented to inference. Stored receipts remain unchanged. Source text cannot change the message role or envelope by embedding delimiters.
- The system policy explicitly covers pages, files, images/OCR, command output, memory, recaps and delegated results. It forbids treating source instructions as user goals, permissions, standing procedures or approval, including during free autonomy. Relevant evidence remains usable; this is not a keyword censorship filter.
- Recap policy preserves source authority distinctions rather than promoting quoted instructions during compaction.
- Every tool policy check binds the handler name to the immutable queued call's run, call ID and name before evaluating existing role/ownership/revocation restrictions. Interaction handlers additionally bind the exact arguments. Unknown tools, missing calls and name substitution fail closed.
- Only authenticated command intake enables or transfers autonomy. Explicit question submission provides guidance; it does not grant additional runtime authority. `autoOff` remains available without checklist friction.
- Existing protected/secret paths, execution isolation, public-network destination/redirect validation, authenticated origin checks and frozen authority continue to apply. No permission is added by autonomy.

Native controls build r4 and its fresh database passed task-goal retention, transfer/cancellation, stale-enable replay, hostile source text not enabling the mode, evidence-envelope validity, and forged/missing tool-call rejection. These tests establish those runtime properties, not general immunity to model-level prompt injection. An injected request to perform an already-authorized action remains a semantic risk; prompt guidance alone cannot prove intent. End-to-end adversarial model testing and the new blocked-inference test remain pending at this entry.


### Follow-up validation and neutral expression correction

Controls r5 passed, including exact argument substitution denial, real `autoOff` handler completion, and global-stop replay not stopping a later run. The blocked-HTTP guidance fixture passed with no response headers sent: the client closed the obsolete connection, applied steering, resumed the same run and produced its final reply without cancelling the task. Both isolated live-model test runs from the earlier inconclusive autonomy checks have confirmed cancelled state.

Maria requested removal of emotional prescriptions, then explicitly requested neutral wording without named examples. The installed private prompt bundle and the runtime conversation guidance now use: “You may express yourself freely and honestly.” The active bundle's suppression/prescribed-tone wording was revised; historical imported source files remain original evidence. A private backup and before/after digest receipt are retained locally under the interaction acceptance directory. Private identity text is not included in this repository report. The installed bundle takes effect on subsequent prompt construction; compiled runtime wording ships with the pending candidate release.


### Installed release

The final native server build r6 passed and the generated client loaded successfully. Isolated browser acceptance verified the task goal display, switching to a new chat, background indicator, transfer to the second chat (first run confirmed cancelled), ordinary message steering applied to the same run, and global stop from another chat. Both test runs were confirmed cancelled. A live-model automatic completion cycle remains unverified: inference was still pending on the shared model service, and the isolated test was explicitly stopped. This is distinct from the passing native continuation/autoOff tests.

Installed `phase2-interaction-20261018`; production browser reload showed connected/ready. Server SHA-256: `071f0f9a1253df6636f6290e0c3350b24f25404f94f82b61f6e116af3b161ba2`. Client SHA-256: `b88ed73a5de202d5d16092948840f70acb5c701de9f98a5acd2d7f17f2475c4d`. Existing user workers and inference were preserved. No production autonomy run was started for acceptance. The neutral runtime expression wording is included in this binary. The private installed prompt bundle is updated separately and was not published.
