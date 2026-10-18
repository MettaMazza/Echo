# Current status — Echo 1.5, 18 October 2026

The current consolidated record is [Echo 1.5](releases/1.5/README.md), including the [original 32-step reconciliation](releases/1.5/PHASE-TWO-STATUS.md). Current deployed release: `phase2-stable-session-recall-20261018-r2`. Phase Two remains incomplete; later foundations do not close their integrations.

The entries below are dated historical observations. Their older counts, remaining-work statements, pause state and deployment names are superseded by the consolidated record where they differ.

## Historical record

# Current status — 18 October 2026

18 October: Ernos Plain is now the explicit default for all Echo-authored code and tools, with documented exceptions, syntax support and actionable compiler-preflight recovery. Live author/build/run/error-recovery/attachment verification passed. See [Ernos authoring and feedback](phase2/ERNOS-AUTHORING-FEEDBACK-2026-10-18.md).

Latest targeted repair: `phase2-general-task-tracking-r2-20261018` gives parent Echo the same durable task checklist as delegated agents, resolves bare installed commands correctly, and directs execution output to the decoded reader. Native checks and a real multi-step model/tool/chat pipeline passed; ordinary chat remains tool-free. See [general task verification](phase2/GENERAL-TASK-TRACKING-2026-10-18.md).

Earlier targeted repair: `phase2-selected-recall-20261017` selects automatic private recall instead of injecting every eligible record. Live fresh greeting: 32,709 total / 7,852 memory tokens. Two live replies were verified through the session API; see [selective recall evidence and limitations](phase2/SELECTIVE-RECALL-2026-10-17.md). All original memories remain retrievable.

Previous targeted repair: `phase2-recap-review-20261017` is deployed. Typed review findings stop positive observations causing recap rewrite loops; compaction yields to interactive inference. The actual 62-record review passed in 124.369 seconds without a rewrite; real semantic defects remain rejected. Existing audit workers and archives were preserved. See [review repair evidence](phase2/COMPACTION-REVIEW-REPAIR-2026-10-17.md).

Previous targeted repair: `phase2-compact-recall-20261017` is deployed. Private cross-session recall now groups source metadata, preserves exact claim quotations and stops accumulating indexed originals in later snapshots. Live request memory is 83,650 tokens; total request is 109,078, admitted without compaction. Existing audit workers were preserved on their earlier executable. See [compact recall evidence](phase2/COMPACT-CROSS-SESSION-MEMORY-2026-10-17.md). General Phase Two development remains paused; this is not overall latency or Phase Two acceptance. Older status sections below are dated history.

16 October continuity tools: scoped session/message index, captured reasoning read-back, persistent revisioned scratchpad, explicit graph continuation and decoded execution output. Sandbox ancestor-directory listing is denied. Read-lock cancellation regression is covered separately; recall extraction was not shortened. See CONTINUITY-TOOLS.md.

16 October: generated images now appear on replies, attach_file publishes workspace files to chat, and recall/compaction self-labels use Echo. See CHAT-ARTIFACTS.md and phase2/JEV-RESEARCH.md. Jev was reviewed and excluded because no open local model release was found.

This is a dated snapshot, not a permanent health assertion. Runtime observations and checks are recorded in `forensics/release-verification.json`; file identities are in `forensics/source-manifest.json`. Older plans, logs and handovers retain their original claims and must be read chronologically.

## Phase Two authority deployment — 16 October

The authority foundation through schema 7 is now deployed with the browser identity readout. The upgrade preserved every pre-existing table row, assigned no implicit admins, and followed a fresh bundle backup plus independent restore. The live local model completed a fresh paired-user run with four successful history-search calls; one run-authority record and four immutable tool-authority records were verified. The test credential was revoked afterwards. Service health and archive quick-check passed.

This deploys principal/account/device association, revocation, queued authority, the current tool policy and identity visibility. The subsequent host-read release adds admin-only absolute-path text reading, tested through the real local model. That acceptance run took 105 seconds wall time. Its archived timings total 28.07 seconds of inference; another request occupied the single execution slot for 75.78 seconds between its turns. The follow-up reused 16,035 cached tokens. Concurrent-user latency acceptance remains open. It does not complete the privileged host-tool suite, Discord, cross-session memory, rights, training or the other Phase Two requirements. See `phase2/PROGRESS.md` and `phase2/FOUNDATION-VERIFICATION.json` for scope and evidence.

## Working foundation

Echo runs locally with a multi-turn ReAct loop, durable submissions and tool observations, streaming replies, cancellation, conversation history, branches, revisions, regeneration, export and device ownership. Local llama.cpp and MLX adapters use provider/model metadata and exact preflight counting. Identity/system architecture text is included in the assembled context; a live `system_inspect` tool reports provider health and capabilities. The application is authored in Ernos, including its browser client.

The browser works through a local HTTPS gateway on the same network. Each paired browser owns its sessions. Phone scrolling now follows accepted messages and incoming replies; scrolling away permits reading older text. Each assistant message retains its own expandable Thinking panel above the response, exposing archived model requests, emitted reasoning, tool calls and results. Speak beside the message actions generates an inline, saved Kokoro audio attachment. This is observed output, not a reconstructed private mental process.

Conversation originals remain in SQLite while reviewed, source-linked recaps reduce active context. Exact history rereading, recap inspection/rebuild, local workspace commands, text/PDF attachments, image operations, local transcription and speech synthesis have implementations and retained test evidence.

## Identity restoration

The ErnosDecent kernel, personas, epistemic and philosophical frameworks, runtime policies and rights charter are now installed verbatim with a separate technical adaptation block. The Codex-addressed handover is excluded from the live Echo prompt bundle; its original remains private audit material. The actual live provider request and compaction/reopen preservation passed verification. The separate gitignored private identity file remains absent; the complete My Brain collection and SFT archive are not connected. See [PROMPT-MIGRATION.md](PROMPT-MIGRATION.md) for exact scope and evidence.

## Incident and recovery

On 15 October the native server exited with signal 11. The gateway returned HTTP 502 and the old browser attempted to decode the empty response as JSON. One durable accepted run was recovered and completed once. A later browser lock came from an unresolved local submission record when the user entered different text.

The browser now reports unavailable/incomplete responses explicitly, keeps the submission identity, reconciles it with the server, and preserves a newer draft. Draft text is stored per session in browser local storage. The associated browser fixture tests gateway errors, truncated JSON, the original submission identity and draft retention. The later crash investigation reproduced native memory races with AddressSanitizer. Echo now isolates HTTP requests and model workers into separate processes, and retains temporary application values explicitly. Both sanitizer and normal builds passed 1,200 overlapping authenticated reads; concurrent real chats and Kokoro speech passed. See `Crash repair 15 October 2026.txt` and the named evidence logs for the exact scope. The compiler/runtime sources were not changed for this repair.

## Retained performance and acceptance evidence

The recorded full-context text test used 260,094 input tokens and 392,796 ms initial ingestion. Changed follow-ups took 2,129 and 4,286 ms. These are direct measured workloads, not a guarantee for every integrated conversation. The user accepted a one-off import wait. A separate real two-provider test preserved corrections through three turns and a reopen. A long llama output completed; the retained long MLX output attempt ended incomplete and is not counted as a pass.

LAN endpoint tests recorded real completion, pairing, other-device denial and foreign-origin rejection. They ran from the Mac against its LAN interface; browser phone-width testing is not equivalent to an independent physical-phone test. The user subsequently supplied actual mobile UI/error screenshots. Backup/independent restoration and native intervention records apply to their stated snapshots and workloads.

## Phase Two and deferred work

Training, model weight updates, connected research interfaces, vector steering in the agent UI, projects, complete semantic/procedural memory and broader restoration remain Phase Two work. Private cross-session source recall is deployed and verified; the private evidence graph and individual-assertion visibility lifecycle are deployed with real-model record/recall/forget acceptance. Automatic source-backed extraction and cross-session context recall are now deployed, including episodic records, semantic facts/preferences/corrections and procedural workflow records. Embeddings, validated workflow mastery and weight learning remain open. See [automatic memory](AUTOMATIC-MEMORY.md) for the deployed mechanism and its limits. Host-side native activation/intervention commands exist separately; their presence does not mean a model-callable research panel exists. Microphone permission/real recording acceptance was explicitly deferred by the user. Echo has a local Ernos runtime supervisor and per-worker exit/recovery records. Reliable reboot startup and a portable installer remain unqualified. Extensive endurance, simultaneous full-context users and a full hardware/network fault matrix are not claimed passed.

## Latest speech and failure checks

Formatted-message Kokoro synthesis passed with original archive integrity, foreign-device rejection and idempotent audio reuse. The speech text filter has an Ernos regression fixture. Another 1,200 overlapping authenticated reads passed with durable diagnostics enabled. Deliberately killed request and model-worker processes produced signal records; the listener survived and interrupted generation returned to a retryable saved state. Evidence: `speech-plain-integration.log`, `speech-diagnostics-stress.log`, `speech-diagnostics-fault-checks.json`. Source indexes have been regenerated from every current application/test file.

The service owner was also fault-tested: a SIGKILL listener exit was durably recorded and a replacement listener returned HTTP 200 (`speech-diagnostics-supervisor.json`). Publication replaced the existing GitHub repository contents in logical commits while preserving its earlier history. The repository is public, reconfirmed through GitHub on 16 October 2026.

## Unified multimodal default correction

The active default now uses the same local Qwen 3.8 llama.cpp model for text, images and tool loops. The text-only MLX endpoint is retired from new chat selections, and automatic image model switching has been removed. Actual three-turn text/image/history-tool acceptance confirmed one model identity throughout. See [UNIFIED-MULTIMODAL.md](UNIFIED-MULTIMODAL.md).

## Phase Two implementation started — 16 October 2026

Maria authorised the full end-to-end goal. The source integration map, requirement ledger, native backup/restore verification and separately tested transactional migration foundation are in [Phase Two progress](phase2/PROGRESS.md). Production is on schema 10 with immutable private session ownership, queued authority checks, explicit tool policy, admin host text reads cross-session source recall, private evidence-linked facts/episodes and auditable derived-memory withdrawal/restoration. The Codex-addressed handover was removed from the private prompt bundle, and the changing run identifier was removed from the stable system prefix; cache reuse was measured. Other final identity/philosophy changes still require the joint review at step 29. My Brain import was explicitly excluded; other continuity sources remain in scope. Phase Two is active, not complete.

## Guardian execution update — 16 October 2026

The current host now runs the guardian-backed server on schema 10. An authenticated production conversation exercised the local model, a real command, durable guardian/exit records and final reply; a second device was denied access. Temporary test credentials were revoked. A consistent pre-cutover backup and executable/prompt/browser hashes are retained privately. This update does not deploy the schema-15 emergency controls or establish completion of Phase Two.

Compaction indicator is deployed (16 October): animated preparation/review stages explain the delay and retained originals; worker failure stops the animation. Reduced-motion styling includes the spinner. Both loopback and LAN HTTPS served the verified browser build after a backed-up restart with unchanged archive rows. This deployment does not close the remaining compaction performance acceptance gate.


## Latest-source deployment — 16 October 2026

At Maria’s explicit request, rebuilt the current server, browser and audio/math assets, execution guardian, principal/device administration, service launcher, controller staging/launch commands and native research commands. Aligned server and administrative compatibility with schema22 and migrated through the existing complete migration chain. The copy migration and live migration each preserved every pre-existing table row; integrity and foreign-key checks passed. Original archive, identity, service profile, server and browser were retained before cutover.

The active service profile now selects the versioned internal release `20261016-123350`, including its matching guardian and UI assets. The local primary model and HTTPS gateway are running. Group conversation UI and the newer callable tools are now in the deployed build. Stored candidate-tool definitions remain candidates; this deployment does not invent unfinished tool activation, Discord, Gmail, training or protected-controller service integration. The separate privileged ledger remains a component, not an activated root service.

Private deployment receipts, build logs, source hashes and cutover backup: `evidence/phase2/release-20261016-123350/`. This supersedes earlier statements that production remains schema10 or that the current group interface is undeployed. Phase Two feature development has not otherwise resumed.


Composer update: Enter sends/queues and Shift+Enter inserts a newline. Typing remains available during replies. Per-device/session queued messages survive refresh and can be removed. Steer now cancels the current owned run and delivers the correction after cancellation; existing tool effects remain archived. Real-browser checks verified keyboard handling, refresh persistence, removal, automatic next-message delivery, retained unsent draft and correction delivery after cancellation. See MESSAGE-QUEUE.md and private evidence/phase2/queue-ui/verification.json. The bundle was updated without restarting active conversations.


Linked-browser/audio update (16 October): deployed schema23 with independent linked credentials for the same account, one-use expiring codes, browser unlink and current private-conversation following. Original rows were verified unchanged across migration. Audio now uses a stable player outside transcript redraw and pauses the preceding player on new playback. Native linking, live HTTP shared-session/revocation and real-browser audio continuity/exclusivity checks passed. See DEVICE-LINKING-AND-AUDIO.md.


## 16 October: linked conversation libraries and steering UI
Deployed schema24 and the updated browser. Explicitly linked existing accounts share their private conversation library without rewriting original session/principal records. Visible pages refresh the sidebar/new-turn state every three seconds and on focus. Native merge/isolation/reopen checks and live HTTP combined-history/rename checks passed. Browser watcher probes passed; live queued steering cancelled its original turn and submitted the correction once, with visible confirmation. The disposable follow-up was stopped after delivery verification. Queue cards, count, removal controls and dedicated steering status are styled in the composer. See DEVICE-LINKING-AND-AUDIO.md and MESSAGE-QUEUE.md for exact scope.


## 16 October: original audio UI restored and reasoning lookup corrected
Restored native audio controls inside each message. Connected message nodes survive transcript updates, preserving playback; starting another clip pauses the first. Browser verification passed. Reasoning capture was present: two failed requests used non-response IDs and received ambiguous end results. reasoning_read now resolves request/run IDs to original response events, reports missing IDs clearly, and memory_messages supplies reasoning_event and capture length. Native isolation/alias regression and read-only replay of both real failures passed. Deployed the new server and browser without a schema change; the already-running reply retained its original worker. New turns use the updated tool. See REASONING-RETRIEVAL.md.

## 16 October 2026 — same-run guidance and linked recall correction

Deployed `midturn-linked-recall-20261016-153834` (schema 24 unchanged). Steer now records guidance against the active run; it no longer cancels and submits another turn. An obsolete generation is interrupted at a stream event boundary and resumed with the original objective, completed observations and new guidance. Unlaunched tools are reconsidered; running tool effects are preserved. Received/applied guidance appears in Thinking. See MESSAGE-QUEUE.md for prefill timing and text-only guidance details.

Linked-account scope now reaches model-facing message/session/reasoning recall, automatic context, graph reads, attachment listing and text retrieval. A read-only check against the actual Mac run retrieved the phone conversation and its original handover attachment with verification. Original principals and tool authority remain intact.

Native streaming and tool-boundary tests passed, along with outsider isolation and browser submission checks. The live local-model test accepted guidance while generating and returned exactly `silly goose`: same run, two generation requests, one received guidance, one application, one final response, zero cancellation events. Evidence: `evidence/phase2/midturn-linked-recall-20261016-153834/`. Broad Phase Two remains paused; this entry covers these requested corrections only.


## 18 October — semantic automatic recall

Deployed `phase2-semantic-recall-r3-20261018`. Local embedding similarity replaces the broad keyword-admission rule for automatic private memory. Original memory remains accessible through tools; full model context is unchanged. The passing live two-turn test completed both visible replies: greeting in 7.32 seconds with a warm cache; broad follow-up in 62.71 seconds including three memory calls. Broad initial input fell from 95,672 to 33,838 tokens. The test exposed and corrected a passage-iteration crash before this release. See [semantic recall evidence](phase2/SEMANTIC-RECALL-2026-10-17.md) for exact scope, failures, verification and limitations. This entry does not close unrelated Phase Two work.

18 October 06:10 follow-up: r4 supersedes r3 after a fresh-session allocation crash in recall candidate construction. Named, retained parameter lists replace temporary list literals in the new recall path. Two post-deployment live turns completed and appeared in the session API. See the semantic recall evidence document for the incident and exact results.

## 18 October — correction precedence

Deployed `phase2-correction-precedence-r2-20261018`. Recorded corrections now quarantine all derivatives of corrected private-memory sources, invalidate frozen recall, block relearning from those originals, and accompany historical memory/tool reads. Transitive corrections preserve the latest correction. Five verified contaminated history messages were linked to the existing Corvus/Dewthread correction without changing original bytes or digests. Native regressions and the final deployed live original-read/model-reply/session-API check passed. See [correction precedence](phase2/CORRECTION-PRECEDENCE-2026-10-18.md) for scope and evidence.
