# Step 10 — media, attachment lifecycle and usable UI

## Current reconciliation — 18 October 2026

This section supersedes the historical status statements below, using the original three implementation bullets and exit condition. Step 10 is complete on Codex’s side, with physical microphone permission explicitly owner-deferred under Maria’s instruction. The video gap passed and is deployed; no new plan steps are introduced.

| Original requirement | Current implementation and evidence |
| --- | --- |
| Unified text/image/tools, thumbnails and attachment lifecycle | Primary-model image/text/tool evidence is retained. Reversible trash/restore and automatic-use forgetting are deployed; explicit rereading remains allowed and original history is preserved. The 18 October production-browser check verified forgetting, reload persistence and restoration. See `ATTACHMENT-LIFECYCLE.md` and private `evidence/phase2/resume-step10-20261018/browser-forgetting.json`. |
| Recording, transcription, speech/playback and declared video | Actual local Kokoro/Whisper and browser playback/download passed; synthetic capture and interruption recovery evidence is retained. Physical microphone permission remains explicitly owner-deferred, not a pass. Local video sampling, same-model image inspection and original playback passed and are deployed; evidence and sampling boundaries are recorded in `VIDEO-2026-10-18.md`. |
| Themes, keyboard, responsive/history UI, projects/research/work panels and compaction display | Existing browser records below cover 240-message history, Enter/Shift+Enter, theme, 390px panels, project conflict recovery, research sources, work state, compaction stages and runtime reduced motion. The real three-agent parent-return browser test passed on 18 October; see `DELEGATION-PROTOCOL-2026-10-18.md`. |

The final physical-device, simultaneous-workload and endurance matrix stays at original Step 27. Step 10 does not claim native whole-video/audio model understanding or a latency pass. The actual service profile is authoritative for the current deployment; historical release names below are not current deployment claims.

## Historical evidence and observations

> 18 October reconciliation: the current [1.5 audit](../releases/1.5/SYSTEM-AUDIT.md) supersedes older outstanding-work statements below. Attachment visibility controls now exist in routes and the browser. Later actual two-child parent-return evidence is recorded in [CHECKLIST-DEPLOYMENT-CACHE-2026-10-17.md](CHECKLIST-DEPLOYMENT-CACHE-2026-10-17.md). Earlier cancelled tests remain historical, not the latest acceptance result.


Current reconciliation: 17 October 2026. This follows the three implementation bullets and exit condition in `phase2.md`; it does not create a new task count. Step 10 remains in progress. Current deployed release is `phase2-agents-20261017-063428` (service profile and release manifest verified on 17 October).

## Unified media and attachment lifecycle

Unified image/text/tool behaviour has actual primary-model evidence in `evidence/phase2/group-media-verification.json`: red/blue image interpretation together with a source document marker read through the actual tool loop. The evidence records original image bytes in both provider requests, private/group access rejection and persisted source receipts. Later changes must be evaluated against the affected source rather than treating this historical result as current whole-system acceptance. The reconciliation JSON records which of its source hashes still match. Thumbnail and pending-selection implementation remains in `app/ui/client.ep`; earlier browser evidence is described in `docs/ATTACHMENT-REPAIR.md`.

**Attachment lifecycle update:** archive-aware reversible trash/restore is now deployed and documented in `ATTACHMENT-LIFECYCLE.md`. The prior picker-only hiding remains explicitly distinguishable with a local restore control. D19 still requires scoped forgetting and its protected erasure implications. Existing readers include attachment listing, text/image tools, model image construction, project file references, compaction source verification and backup/restoration. A lifecycle change must be coherent across these consumers; merely renaming the picker button cannot close D19.

The pinned ErnosDecent charter (`identity/ernosdecent-source/config/rights/charter_v2.json`) classifies memory erasure and session deletion as protected core changes. Reversible selection/visibility and actual byte/history erasure must remain distinguishable. The plan does not authorize treating hiding as erasure or silently destroying canonical history.

## Recording, speech and modality acceptance

`evidence/phase2/shared-media-verification.json` records real local synthesis and transcription through scripted tool calls, private/shared ownership checks, and actual generated image publication. Those fixtures establish their described runtime paths, not physical microphone capture or every browser playback situation. Preserve the current TTS layout while completing functional acceptance.

Physical microphone permission/recording remains explicitly user-deferred under Maria's instruction to close genuine human-only dependencies on Codex's side; it is not a claimed pass. Declared audio/video qualification, sampling disclosure and the final device matrix remain subject to their original Step 10/27 requirements. Existing recovery and playback evidence is reconciled below. No unsupported modality should be advertised as observed.

## UI, projects and compaction

Reviewed message sharing is deployed and documented in `MEMORY-SHARING.md`. Real browser evidence covers exact preview, cancellation, confirmation, duplicate-free repeat, and private/shared destination display. Project controls are deployed and documented in `PROJECTS.md`: create/edit/select, task state, cross-conversation reuse, browser reload and a digest-bound uploaded file reference passed. Distinct linked-device and fault-injected browser acceptance remain broader UI acceptance, not conclusions of those single-browser fixtures.

Compaction animation and explanation are already implemented in `app/ui/client.ep` and `app/ui/layout.ep`, with a polite stage live region and reduced-motion CSS. `compaction-browser-verification.json` records desktop and 390px browser checks, saved stage restoration, draft preservation, failure/reconnect behaviour and stopping a paused run. This is explicitly a seeded-stage browser test. `compaction-real-resumed-verification.json` separately records the original real model run resuming to completion, a saved checkpoint, recalled marker and preserved original. `compaction-ui-live-deployment.json` records deployment and explicitly does not claim performance acceptance. Later combined real compaction/restoration evidence is described in `MEMORY-RECOVERY.md`.

Reuse those results for the behaviour they actually cover. Do not repeat the long real-model compaction merely to demonstrate that the spinner exists. The browser checks recorded below cover runtime reduced motion, keyboard/history, responsive layouts and research/work panels. Simultaneous real conversations and the final physical-device matrix retain their original acceptance scope. Discord progress belongs to the later connector steps.

Reviewed evidence hashes and current group-media source comparisons are retained in `evidence/phase2/step-10-evidence-reconciliation-20261017.json`. This document records implemented behaviour and exact remaining scope; it is not a Step 10 completion certificate.

Project stale-edit browser acceptance now verifies two simultaneous editor tabs, visible rejection, retained fields, and reloading the winning revision. The retry-control correction and deployment are recorded in `project-conflict-20261017/result.json`.

Capability disclosure correction: the backend `research` field describes configured web search/fetch/navigation, not steering, activation inspection or training. Its UI label now says **Web research**. The isolated production-renderer browser fixture preserves supported/unsupported/unknown modality distinctions; it does not qualify those modalities. Evidence: `provider-label-20261017/result.json`. Video processing has no attachment route in the inspected application and is not accepted as delivered.

Retained web-research results now have a per-message disclosure, separate from sources actually read. Implementation, component checks and authenticated desktop conversation/open/download acceptance are documented in `RESEARCH-PANEL.md`.

Current desktop history/keyboard acceptance (`history-keyboard-20261017/result.json`) loaded 240 archived Unicode messages exactly once in chronological order, using initial 20 and user-selected 100-message batches, then hid the exhausted older control. Shift+Enter preserved a newline; Enter created exactly one canonical event and displayed message and cleared the draft. Dark theme selection applied. This is synthetic archived history and keyboard submission acceptance, not model response, physical-phone or endurance acceptance.

The read-only Work sidebar panel is deployed with authenticated run paging, schedule selection, current-state refresh and provenance details. `WORK-PANEL.md` records passed browser/HTTP checks plus populated schedule and private/shared delegation isolation, including membership departure.

Phone-width panel acceptance (`panels-phone-20261017/result.json`) used the authenticated UI at 390×844: Work and Projects stayed within the viewport with matching scroll/client widths, loaded their scoped records, and closed with Escape returning focus to the invoking button. The viewport override was restored and the isolated server stopped. This is responsive-browser evidence, not a physical-phone test.

## Reused recording and playback evidence (17 October)

Recording recovery is implemented and has browser evidence: `audio-store-reload-002.txt` preserves sealed and unfinished recordings across reload, rejects foreign reads/listing and prevents append after sealing. `audio-recorder-browser.txt` verifies AudioWorklet capture through committed chunks, sealing, WAV decoding and reopening with 48,128 exact synthetic samples. `audio-upload-before-body.txt` and `audio-upload-after-body.txt` verify recovery before upload delivery and after delivery before local acknowledgement, retaining the original upload identity and samples. `audio-ui-recovery.txt` records the saved attachment and retained recovery copy. These results qualify synthetic capture and interruption recovery, not physical microphone permission.

`phase2/audio-reasoning-20261016-152656/verification.json` confirms native inline audio controls, playback continuing after message submission, and the first player pausing when another starts. `kokoro-acceptance/verification.json` records actual local Kokoro synthesis and Whisper transcription, attachment ownership and browser playback; the installed Fable voice is `bm_fable` (the requested `am_fable` name was not an available voice).

`phase2/direct-shared-speech-verification.json` supersedes the older shared-media report's outstanding direct-Speak item: actual local synthesis, browser playback/reload/download, repeat-click idempotence, caller/source distinction, outsider and departed-member denial passed. Current `shared_authority.ep`, `message_speech.ep` and `group_speech.ep` hashes match that evidence. This is not a claim that every subsequent UI or authority dependency is unchanged.

Existing `speech-real-stop-001.log`, `speech-real-revoke-001.log`, `image-logs-stop-test.log` and `image-logs-revoke-test.log` record helper termination/reaping, retained output streams and cancellation before another model turn. They do not by themselves qualify every current shared-membership transition. Evidence hashes and these scope limits are retained in `evidence/phase2/media-recovery-reconciliation-20261017.json`. No model or production service was restarted for this reconciliation.

Compaction scope regression: identical activity text from a foreign-session observation could retain the previous spinner. The renderer now clears its activity identity before hiding it. The Ernos browser fixture failed before and passed after the fix; the UI-only release above serves HTTP/LAN 200 with unchanged server/model binaries. Evidence: `compaction-scope-20261017/result.json` and `browser.png`. This component check does not replace simultaneous real-model or runtime reduced-motion acceptance.

Runtime reduced-motion acceptance now passed with the actual macOS Accessibility preference toggled off/on/off: the production CSS spinner changed from `echo-recap-spin` to `none`, the explanation and polite stage region stayed visible, and animation resumed after restoring the original setting. This used a synthetic pending-stage component fixture, not another long model compaction. Evidence: `compaction-scope-20261017/reduced-motion.json` and `reduced-motion.png`. The preference and Settings page were restored, and the fixture tab/server closed.


Options redesign was introduced in `phase2-options-20261017-055323` and is retained in the current release. `docs/UI-SETTINGS.md` records categorized settings, mobile layout, dismissal and linking-shortcut checks. Attachment visibility revisions, stale recap/fragment refusal, and explicit rereading are now deployed; native regression evidence is recorded in `ATTACHMENT-LIFECYCLE.md` and `evidence/phase2/explicit-reread-20261017/`. This supersedes the earlier undeployed status.

## Latest source reconciliation and manual-test handover

The current `app/web/attachment_visibility.ep` contains the scoped reversible policy writer. `app/web/routes.ep` and `app/ui/client.ep` do not yet expose its exclude/restore operation. The attachment picker currently explains trash/restore and explicitly distinguishes forgetting. Therefore the remaining attachment work within the first original Step 10 bullet is the end-user forgetting control and its complete consumer/workflow qualification, not another rewrite of the already deployed reread or compaction protections. No new plan step or acceptance denominator is introduced here.

Inline delegated panels and automatic parent result collection are deployed and documented in `../DELEGATED-AGENT-RESULTS.md`. The actual model admitted a child; the user then requested all runs stop for manual testing. The child and test parent reached cancelled state and all recorded running workers stopped. Browser acceptance verified the cancelled child panel and nested trace control, then New chat opened an empty conversation and closed the sidebar. Successful actual-model child-result-to-parent-response acceptance remains unproven; synthetic native acceptance is distinguished in that document.

Maria's manual-test handover leaves the UI/server and local model available, with no agent-started inference or test workers. Further source/documentation review does not authorize cancelling any new run Maria starts during testing.
