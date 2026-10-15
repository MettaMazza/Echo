# Echo Phase 1 Implementation and Live Acceptance Plan

Prepared for Maria Smith and Echo on 14 October 2026. Revised to require plain-English Ernos implementation throughout.

Phase 1 delivers a complete, locally hosted multimodal AI application on Maria’s Mac Studio M3 Ultra with 512 GB unified memory. It includes a familiar modern chat interface, the strongest qualifying local model, responsive operation at that model’s full supported context, durable continuity across compaction, interchangeable providers, and working foundations for steering, interpretability, and weight training.

Everything in this plan belongs to Phase 1. The numbered stages are implementation work packages within that phase, not reasons to defer agreed features. Completion requires a working installation and recorded live end-to-end evidence. This document is a plan; implementation and acceptance tests remain to be performed.

## 1 Governing requirements

1. Run inference, storage, retrieval, embeddings, document processing, speech, application services, and UI assets locally. No silent cloud fallback, external authentication dependency, telemetry, remote fonts, or CDN requirement.
2. Select the strongest model that actually runs responsively at its full supported context on this machine. Evaluate model quality, quantisation quality, latency, context performance, and resource use together. Parameter count alone cannot determine intelligence.
3. Introduce no arbitrary application ceilings for context tokens, output tokens, message count, attachments, session duration, or tool-loop iterations. Every effective constraint must originate in the model, running provider, measured resources, protocol, or an explicit operator choice.
4. Discover capabilities and effective limits at runtime. Never silently substitute a guessed limit or claim an unloaded context capacity is available.
5. Preserve the complete session independently of the active prompt. Compaction continues the same conversation and never erases source history.
6. Default to accessible, locally controlled inference machinery suitable for steering and inspection. Use llama.cpp with Metal as the initial default candidate, with a native extension interface alongside the portable provider interface.
7. Support other providers through adapters. Advanced features are capability-negotiated: a chat-compatible endpoint does not imply access to model internals.
8. Keep the machine and drive organised, with isolated environments, pinned dependencies, explicit storage paths, and reproducible setup.
9. Report performance and model quality honestly. Do not describe a mock result, passing process exit, or generated success statement as proof of a working feature.
10. Write all project-authored executable source in plain-English Ernos (ErnosPlain), using the existing Ernos language and ErnosDecent ecosystem. This covers the UI, application services, provider adapters, continuity, tools, research controls, training orchestration, build orchestration and automated tests. Do not substitute React, TypeScript, JavaScript, Python or another implementation language based on assumptions about Ernos.
11. Establish compiler and library capabilities by inspecting and compiling real source and existing programs. Reproduce any suspected gap with a minimal example, investigate existing solutions, and implement required Ernos support where necessary. A gap is engineering work to resolve, not grounds to silently change languages.
12. Keep user agency and continuity central: model changes, training activation, deletion, and restoration must be visible, attributable, and recoverable where promised.

The earlier ErnosDecent project supplies useful context and design lessons. This is a new project. Phase 1 does not automatically import its code, identity records, database, constitutional system, cryptocurrency stack, or network services. Personal-history imports must preserve provenance and be explicitly selected. The implementation language is mandatory: plain-English Ernos/ErnosPlain. The inspected repository calls its application ErnosDecent and links its language toolchain as Ernos-Programming-Language; verify the current canonical compiler and terminology from those sources. Reuse suitable existing Ernos components after inspection and validation.

## 2 Project location and organisation

Canonical project home: `/Volumes/One Touch/Echo`.

Maria clarified the storage policy during implementation: One Touch is the durable project, model and archive store; computation runs on the Mac CPU, GPU and unified memory. Latency-sensitive active databases, build products and caches may use a clearly documented internal-SSD working directory. Internal copies must have a defined lifecycle and verified persistence back to One Touch. Avoid disk offloading or repeated external reads during active inference where resident memory can serve the workload. Measure cold loading separately from steady-state inference and monitor swap. External storage saves internal disk space, not the RAM required by a loaded model.

Desktop shortcut: `/Users/dolltrading/Desktop/Echo`, pointing to that folder.

Planned layout:

```text
Echo/
  plans/                 Requirements, implementation plan, decision records
  app/                   Version-controlled UI and application services
  engine/                Engine integration and narrowly scoped patches
  environments/          Isolated runtimes and dependency lockfiles
  models/                Weight files, manifests, tokenizer and template assets
  data/                  Personal runtime state and complete session archives
  artifacts/             User-created files and previews
  experiments/           Steering, activation and training experiments
  tests/                 Fixtures, automation and acceptance scenarios
  evidence/              Test reports, traces, timings, screenshots and hashes
  backups/               Verified snapshots and recovery manifests
  scripts/               Setup, start, stop, update, export and restore
```

Create these directories as their work begins. Keep credentials, personal transcripts, weights, runtime databases, and large experimental captures out of source control.

Before choosing active database/cache placement, inspect the drive’s filesystem, sustained throughput, free space, and disconnect behaviour. Test atomic replacement and database locking. A removable drive must not be assumed suitable for every hot write path. If measured performance or filesystem behaviour requires internal storage for a hot cache or database, document the exact path, make that placement explicit, and keep export/backup under Echo. Never silently duplicate hundreds of gigabytes of weights. A backup on the same physical drive protects against logical mistakes, not drive loss; record the available recovery coverage.

## 3 Stage A Hardware and performance contract

First inspect the canonical Ernos compiler, its self-hosting and self-compilation path, standard library, build instructions and representative existing applications. Build the toolchain and compile representative UI, storage, networking, native interop and test programs. Record actual results and resolve required toolchain work in Ernos.

Inspect actual hardware and software rather than relying on the handover: chip, RAM, available Metal memory, storage, macOS, developer tools, existing runtimes, and competing processes. Record a baseline before installing project dependencies.

Define measurable interactive performance targets from an initial benchmark, then agree and freeze those targets before final model selection. Targets are acceptance criteria, not hidden runtime restrictions. Record cold start, cold full-context prefill, warm follow-up latency, sustained generation rate, compaction latency, and recovery latency separately. Report median and tail latency together with workload and resource conditions.

Benchmark real full-context workloads with text, code, documents and images. Measure memory, cache footprint, swap growth, disk traffic, temperature-related slowdown, and responsiveness of the rest of the Mac. Account for operating-system and application memory from measurement, rather than allocating all 512 GB to weights.

A configuration qualifies only if it meets the recorded responsiveness targets at its full supported context with the required services active. Fitting weights in RAM alone is insufficient. If no candidate qualifies, record the measured bottleneck and revisit model/runtime selection; do not quietly reduce context or announce success.

Deliverables: hardware inventory, drive assessment, baseline measurements, and performance acceptance contract.

## 4 Stage B Model qualification

Recheck official model releases and engine support at implementation time. Initial research candidates include Qwen3.5-397B-A17B, Qwen3.8-Flash-Next, and Kimi K2.5. These are candidates, not a preselected winner or a claim that any is globally smartest.

For each plausible candidate, record official source, licence, exact revision, weight hashes, architecture, supported modalities, native context, separately documented extended context, tokenizer, chat template, quantisation, runtime support, and memory estimate. Distinguish native context from extensions requiring different scaling settings or validation.

Evaluate the highest-quality feasible representations and compare accuracy before accepting more aggressive quantisation. Use reasoning tasks with checkable answers, executable coding tasks, unseen images, document understanding, structured/tool calling, long-context retrieval at multiple positions, and continuity tasks. Repeat stochastic tests and retain failures. Compare quality and performance rather than treating benchmark marketing claims as local results.

Full-context qualification includes room for generation: record prompt, special tokens, tools, multimodal tokens, and maximum available output together. Do not imply the entire window can simultaneously be both input and output. Test cold and cached cases separately, including the maximum context configuration the provider genuinely supports for this model.

Optimise using supported Metal execution, attention implementations, prefix/KV reuse, batching, compatible cache quantisation, and speculative decoding where beneficial. Verify quality-sensitive changes and do not count speculative speedups unless acceptance rate and end-to-end latency improve. Measure concurrency before selecting scheduling behaviour.

Select a primary model and a second model for portability tests. Keep the registry open to replacements. Never switch a conversation’s model silently.

Deliverables: reproducible candidate scorecards, selection decision, exact model manifests, full-context performance evidence.

## 5 Stage C Application and provider architecture

Required stack: plain-English Ernos source throughout the UI and application, using the ErnosDecent ecosystem and existing compiler/standard-library facilities. Implement the local application service, durable storage, attachment handling, inference integration and processing workers in Ernos. Inspect existing UI, networking, storage, FFI and compilation examples before choosing the implementation route. Browser assets or native code emitted by the compiler are generated artifacts; maintain the authored behaviour in Ernos rather than hand-maintaining a second-language application.

```text
Local browser UI
  -> Local application service and durable job/event store
     -> Context and continuity manager
        -> Portable provider adapter
           -> Local inference runtime and weights
        -> Native research interface
           -> Steering, activation inspection and adapter controls
     -> Local files, projects, retrieval and embeddings
     -> Local document, speech and media workers
     -> Local tool execution workers
```

Own a provider-neutral conversation/event format containing text, files, image/audio references, model-exposed reasoning where available, tool calls/results, artifacts, errors, usage, and provenance. Do not fabricate access to hidden internal reasoning. Preserve originals when translating provider request formats.

Echo is a multi-turn ReAct agent. Each run repeatedly constructs context, requests the model's next action, persists tool calls, executes authorised tools, persists observations, and returns those observations to the model. It continues until the task is complete, cancelled, failed recoverably, or genuinely needs user input. There is no fixed turn or tool-iteration cap. Context compaction continues the same run, preserving unfinished requests and complete tool-call/result pairs.

Persist run and tool-call identities before execution. After interruption, distinguish queued work, confirmed results and actions whose outcome is uncertain. Never blindly repeat an uncertain side effect; reconcile it with the tool's actual state or an idempotency mechanism. Live acceptance must include a task requiring multiple model/tool cycles, continuation after compaction, cancellation during a tool, and restart without duplicate side effects.

The portable interface covers model listing/loading, capability discovery, tokenisation, request construction, streaming, cancellation, tools, modality transport, finish reasons, usage and health. The native research interface covers supported activation access, interventions, steering vectors, adapters and engine-level measurements.

Integrate llama.cpp first through Ernos-authored bindings or process/protocol control. The previously requested inference engine and any selected training engine are explicit external dependencies; their upstream implementation language does not authorise project logic in that language. Inventory these dependencies and their boundaries. Keep new integration logic and research controls in Ernos; investigate native interfaces before proposing an engine-source patch in another language. Add a second real runtime, such as MLX or LM Studio, to prove interchangeability. Future providers plug into the same contract. Unsupported capabilities must be explicit in the UI; no provider-name-specific assumptions in application logic. Hosted adapters, if added later, do not qualify as the local default or offline acceptance route.

Provide a reproducible Ernos build and self-compilation verification, pinning the bootstrap provenance and distinguishing any bootstrap dependency from the resulting self-hosted toolchain. Author test automation and setup/lifecycle orchestration in Ernos.

Use local service supervision, authenticated internal control where appropriate, loopback bindings, streamed job events, cancellation propagation and crash recovery. Resume durable jobs without duplicating side effects. Keep shell/code execution isolated to the intended workspace, with visible actions and appropriate approval for destructive or external operations.

Deliverables: adapter contract, local services, durable event schema, two working providers, native research interface.

## 6 Stage D Provider-driven limits and context

Discover the actual model context capacity and loaded runtime capacity separately, along with output caps, modality constraints, tokenizer/template, generation controls and runtime resources. Query native metadata beyond generic chat APIs when necessary. Fall back to local model/runtime configuration and controlled validation when metadata is missing. Unknown values stay unknown until resolved.

Request maximum supported context and verify it after load. Reject silent runtime reductions. Capture the requested value, effective value, source and reason for any difference. Distinguish genuine architecture/memory constraints from configurable provider defaults; configure restrictive defaults appropriately instead of inheriting them without inspection.

Calculate request size with the real tokenizer and chat template, including system instructions, tool schemas, history, attachments and output reserve. Use exact provider multimodal accounting where available; otherwise measure conservative estimates and reconcile them with actual usage. Do not silently truncate inputs when accounting is uncertain.

Use provider-supported output capacity within the actual remaining budget. Omission of a max-output field is acceptable only if verified not to introduce a smaller default. Discover separate output ceilings where they exist. Context size does not imply temperature or other sampling controls should be maximised.

Audit all layers: UI validators, upload parsers, reverse proxies, SDK defaults, request bodies, transport buffers, model launch flags, slots, output settings, tool-result handling, retries and timeouts. No fixed last-N-message pruning or arbitrary iteration cutoff. Use streaming, backpressure, durable queues and progress-aware stall detection. Operator cancellation and real resource exhaustion remain valid stop conditions and must preserve work.

Concurrency must not silently divide away the main conversation’s advertised context. Queue or schedule work according to measured resources; explain when training or a second model needs exclusive resources.

Deliverables: capability registry, inspectable effective-limit report, token accounting tests, cross-provider boundary tests, default-limit audit.

## 7 Stage E Complete session preservation and compaction

### Complete durable archive

Persist the session continuously, not only when context fills. Archive all user and model messages, tool requests/results, exposed reasoning channels, attachments, generated artifacts, corrections, message branches, model changes, errors and pending work. Give every event a stable ID, timestamp, parent/branch relationship and provenance. Store attachments by verified content identity and preserve original bytes. Save resumable partial streams distinctly from completed responses.

The transcript is canonical. Prompt context, retrieval indexes, summaries and inference caches are derived state. A cache loss must not lose history. Content hashes detect damage; atomic writes and tested backup/restore provide recovery.

### Transactional compaction

1. Compute the budget for the next request, tool result, output and compaction work from actual context capacity.
2. Start compaction before overflow, leaving enough measured space to complete it. Do not use an arbitrary fixed percentage.
3. Flush and verify the full archive and its attachments. Record a compaction checkpoint and source event range.
4. Generate a detailed continuity summary covering identity/relationship context actually supplied, user preferences, objectives, constraints, decisions and reasons, corrections, completed work, unresolved questions, failures, exact artifacts and locations, current workspace state, pending tool work and next actions.
5. Preserve critical instructions and exact values in structured state as well as prose; link summary claims to original event IDs. Keep quoted source material distinct from instructions to prevent archived tool output acquiring authority.
6. Validate coverage of outstanding requests, commitments, corrections and active work against the source. Record uncertainties rather than inventing missing details.
7. Build the replacement context with the detailed summary, relevant recent messages, required instructions, pending execution state, archive index and retrieval tools. Budget summary depth from the current workload, not a fixed token constant.
8. Atomically activate the new context only after persistence and validation pass. Retain the old checkpoint for recovery.
9. Continue the same session. Surface a compact UI marker with access to the summary and original transcript. Do not announce session termination merely because context changed.

If summarisation fails, preserve the original state and retry or expose a recoverable failure. Oversized single inputs and tool outputs require progressive processing with source references, never silent deletion. Respect complete tool-call/result pairs when reconstructing context.

### History tools

Provide `session.search`, `session.read`, `session.read_attachment`, `session.get_compaction`, and `session.rebuild_recap`. Support exact search, chronological access and local semantic retrieval. Return progressive ranges sized to the current budget while keeping the complete originals accessible. Every retrieved passage carries source IDs and dates.

Across repeated compactions, refresh summaries from original records and current structured state, not solely from an increasingly compressed prior summary. Allow the model and Maria to inspect and correct recaps. Persistent history is not an unlimited active context; the UI distinguishes both.

Deliverables: complete event archive, transactional compactor, detailed recap schema, rereading tools, continuity and recovery evidence.

## 8 Stage F Familiar modern chat UI

Build a polished local interface with sidebar navigation, new chat, searchable history, projects, settings and model selection. The central conversation supports streaming, Markdown, mathematical notation, tables, highlighted code, copy, export, edit-and-branch, regenerate, stop and resume where supported.

The composer supports multiline input, pasted images, drag-and-drop files, attachment previews and microphone input. Include document/image previews, downloadable artifacts, a preview/work panel, tool progress and results, errors, local provider status, model loading state and clear cancellation.

Implement light/dark themes, responsive layouts, keyboard navigation and accessible controls. Large histories should remain responsive using progressive rendering without dropping stored content.

Projects own local files, selected context and instructions. Cross-session memory and semantic retrieval retain provenance and corrections, and distinguish historical information from current-session facts. Never silently convert every personal conversation into training data.

Provide local document extraction and source-linked answers; local speech recognition and playback; image understanding; and a local image-generation route with inspectable generated outputs. Handle video/audio inputs through declared supported providers or explicit local preprocessing, recording sampling/transcription so reduced input is not presented as complete observation. Unsupported modalities remain clearly labelled. Native image understanding, image generation and speech are distinct services and need independent tests.

Deliverables: fully functioning UI and local processing routes, with browser-level evidence for every exposed feature.

## 9 Stage G Steering and interpretability

Keep llama.cpp integration directly accessible and versioned. Start with existing control-vector, adapter and evaluation callback facilities, then implement the smallest native extensions needed for the selected model architecture. Do not treat existing flags as proof that every layer/intervention is supported.

Record each run’s model revision, weights, quantisation, tokenizer, template, engine commit, adapter hash, steering vector, layer range, scaling, sampling settings and seed where supported. Activation captures also record tensor identity, dimensions, dtype and source token positions.

Deliver a working research panel/API that can capture selected real activations, apply a compatible steering vector to selected supported layers, compare baseline/intervention runs and remove the intervention. Model-specific layer maps belong in adapters, not general chat logic. Invalidate or rebuild affected caches when changing controls; verify there is no contamination of supposedly baseline runs.

Use selective capture to avoid unnecessary memory and disk overhead. Measure both normal-mode and instrumented performance. Explain what each measurement shows; activation dumps alone do not establish a causal account of reasoning.

Deliverables: real steering demonstration, activation capture and inspection, reproducible comparisons, clean restoration of baseline.

## 10 Stage H Local weight training and rollback

Implement training orchestration and worker control in Ernos, with MLX LoRA/QLoRA as an initial candidate external training engine. Confirm support for the chosen primary model, its architecture and conversion path. A successful smaller-model demonstration does not by itself qualify the primary model’s training route.

Freeze an explicitly selected dataset with source provenance, hold-out examples and replay/retention examples. Record model/tokenizer identity, training configuration, dependencies and input hashes. Preserve base weights and accepted adapters. Train an immutable candidate; evaluate before activation; keep training approval and activation decisions separate and visible.

Demonstrate the actual path from local training through export/conversion into the default inference runtime. Verify tensor mapping, tokenizer/template compatibility, output behaviour and the conversion’s effect. If direct adapter interchange is unsupported, qualify another documented path, such as a merged converted candidate, with storage and quality costs measured. Do not claim compatibility based only on matching filenames or model names.

Retain parent lineage so future accepted updates can accumulate without double-applying deltas. Record losses and held-out retention, text/vision/tool-use regressions, and live service health. Atomically activate only the evaluated candidate, show the active version, and prove rollback. Do not modify active weights while requests are using them. Schedule training within measured memory resources, checkpointing and pausing inference visibly if necessary.

Deliverables: real local training on the selected architecture, evaluated candidate served by llama.cpp or the qualified default engine, lineage records and verified rollback.

## 11 Stage I Offline operation and lifecycle

After deliberate acquisition of weights and dependencies, the full core application must cold-start and work with external networking blocked. Bundle UI assets and retain all required local models/processors. Monitor outgoing connection attempts during acceptance, including library update checks and telemetry.

Provide explicit setup/start/stop/update scripts, health checks, persistent logs, dependency locks and reversible application/database migrations. Separate secrets and personal data from code and logs. Sanitize rendered content and isolate artifact previews/code execution. Maria explicitly enabled access from other devices on the same Wi-Fi on 14 October 2026. Expose the authenticated UI gateway on the local network; keep inference, database administration and research control services on loopback. Do not configure public internet exposure or router port forwarding.

### Local network access and separate devices

Phones, tablets and computers on the home network must be able to open the same responsive Echo interface using a stable local address. Serve it over HTTPS with a documented local certificate trust setup. Test actual reachable devices: a Mac connected by Ethernet and a phone connected by Wi-Fi may share the same local network; guest-network client isolation may prevent access and must be reported clearly.

Pair each browser profile through a one-use invitation displayed on the host Mac. Pairing creates a persistent device identity with a user-readable name and a private credential. Store credential hashes on the server, keep credentials out of URLs and logs, and support listing and revoking paired devices from the host. The bootstrap endpoint must not distribute a shared access token. Treat browser storage being cleared as a new pairing, without exposing or deleting the previous device's history. Do not infer identity from IP addresses or browser fingerprints. People sharing one browser profile share its identity; separate profiles or explicit sign-in are required to separate those people.

Enforce ownership on the server for conversations, runs, archives, attachments, uploads, downloads, artifacts, searches, projects and memory. An unpaired or different device must not discover another device's content through guessed IDs, retries, stream subscriptions or error messages. Give existing unowned local records an explicit host-owned migration; never assign them to the first remote device that connects. Keep global model settings, device administration, training and research controls host-authorized. Any future sharing must be explicit.

Separate each device's selected conversation, drafts, submissions, cancellation and reconnect state. Bind retry identities to the owning device and commit ownership with the first message and run. Concurrent devices may queue for shared inference resources according to measured capacity, but must never mix histories or cancel each other's work. The agent's tools must inherit the authenticated session's access scope, including archive retrieval and filesystem workspaces; shared unrestricted host filesystem tools would defeat conversation isolation.

Implement and verify device ownership before opening the listener beyond loopback. Retain real two-device evidence for pairing, simultaneous independent turns, refresh and restart, cross-device access rejection, revocation, archive retrieval isolation and HTTPS access. This is application-level separation on a shared host; the Mac's administrator can still access locally stored data.

Test clean shutdown, abrupt process death, model-service restart, browser refresh, OS reboot, external-drive unavailability and restoration. Avoid stale locks, duplicated tool execution, phantom completed messages and automatic fallback to a different model/provider.

Provide export and verified restore into a clean data directory. Include model manifests and environment locks without duplicating weights unnecessarily. Clearly identify which files a backup contains and which would need to be reacquired.

Deliverables: reproducible offline installation, lifecycle scripts, backup/restore instructions and fault-recovery evidence.

## 12 Live end-to-end acceptance matrix

Every row is mandatory for Phase 1 unless a concrete scope change is explicitly agreed. Missing support remains an open requirement, not a silent pass. Tests must traverse the real browser, application, runtime and storage when those layers are involved. Unit and simulated-provider tests supplement this evidence.

| ID | Scenario | Required result and retained evidence |
|---|---|---|
| A01 | Clean setup and reboot | Reproducible startup with recorded versions and selected local model. |
| A02 | Full supported context | Effective context verified; near-boundary workload completes within agreed latency and resource targets. |
| A03 | Cold versus cached context | Separate prefill, first-token and sustained output measurements; no misleading cache-only headline. |
| A04 | No artificial caps | Audit and live tests expose no hidden UI, SDK, provider-default, output or iteration ceiling. |
| A05 | Output capacity | Long generation reaches natural completion or actual provider boundary with accurate finish reason. |
| A06 | Multimodal boundaries | Account for image/audio/tool tokens; preserve oversized sources through explicit progressive processing. |
| A07 | Standard chat | Stream, stop, edit, branch, regenerate, copy and export through the actual UI. |
| A08 | Coding and local tools | Generate code, execute controlled tests, inspect actual files/results, and cancel work correctly. |
| A09 | Documents and citations | Upload fresh documents, answer checkable questions and open exact cited source passages. |
| A10 | Images and generation | Understand unseen images; generate a local image, preview it and pass it back for inspection. |
| A11 | Voice and media | Local input/transcription/reply/playback; verify declared video/audio processing and disclose sampling. |
| A12 | Provider portability | Two real runtimes and models work without application source changes; unsupported research features are explicit. |
| A13 | Model/context switching | Recount and rebuild context using the new tokenizer/capacity while preserving originals. |
| A14 | First compaction | Entire session is verified on disk before replacement; detailed recap preserves active task state. |
| A15 | Repeated compaction | Seed early facts, corrections, relationships, decisions and unfinished work; recover them after multiple cycles. |
| A16 | Exact history rereading | Use tools to retrieve originals and answer exact questions beyond the summary, with event IDs. |
| A17 | Compaction interruption | Kill the process before/during/after commit; recover a consistent old or new context without lost events. |
| A18 | Persistence | Refresh, browser close, process restart and reboot retain messages, branches, files, projects, memory and paired-device ownership; reconnect and retry do not mix or duplicate work. |
| A19 | Steering | Apply a real compatible vector, retain controlled behavioural comparisons, remove it and restore baseline. |
| A20 | Interpretability | Capture and inspect real selected internal tensors with model/layer/token provenance. |
| A21 | Training | Train actual adapter weights, retain metrics and load the evaluated result into the default runtime. |
| A22 | Activation and rollback | Verify selected candidate identity, live inference, parent restoration and cache correctness. |
| A23 | Offline cold start | With networking blocked, run chat, documents, retrieval, speech, images and continuity successfully. |
| A24 | Failure recovery | Inject service death, interrupted streams, disk exhaustion, memory pressure and drive loss; preserve recoverable work. |
| A25 | Backup restoration | Restore to clean storage and verify transcript, attachment, artifact and configuration hashes. |
| A26 | Sustained workloads | Repeat mixed long-context tasks; measure memory growth, queue health and latency degradation. |
| A27 | Accessibility and UI performance | Keyboard operation, themes, responsive layout and large-history rendering work in the browser; a real phone/tablet and computer access the local HTTPS UI and run separate simultaneous conversations. |
| A28 | Provenance and privacy | No cloud fallback or transcript leakage; source claims remain traceable; archived content cannot silently become instructions. Verify one-use pairing, credential persistence and revocation, host-only administration, and server-side rejection of cross-device session/run/stream/archive/file/search/tool access, including guessed IDs and retry collisions. |
| A29 | Plain-English source and self-hosting | Audit all authored executable source for Ernos compliance; build and exercise the self-hosted compiler path and run Ernos-authored application and acceptance tests. Generated output and upstream dependency code are separately inventoried. |

Use deterministic fixtures for state integrity and repeated trials for model quality. Freeze test workloads and expected outcomes before evaluating candidates to avoid tailoring the test to a preferred model. No claim that finite tests prove all future behaviour; report the exact coverage and failures.

For each run retain date, hardware, commit, model and engine hashes, context configuration, quantisation, request/usage accounting, runtime measurements, expected versus observed result, UI screenshots/traces where useful, and failure logs. Protect personal content in reports.

## 13 Execution order and completion gate

Execute A and B first, then establish C and D with a thin working chat. Implement E before relying on long sessions. Build F while completing the native research path G and training path H. Validate I continuously and run the complete acceptance matrix against the final integrated build. Investigate any regression and rerun affected tests; do not repeat unrelated expensive tests without a reason.

Before calling Phase 1 complete, deliver:

- A working local application in the agreed Echo project structure with a desktop entry point.
- Primary model selection supported by full-context quality/performance measurements.
- A frozen performance contract met by the final build.
- Two demonstrated providers, with capabilities and every effective limit inspectable.
- Full session archives, detailed compaction, rereading tools and multi-compaction recovery proof.
- Complete functioning chat UI and the advertised local document/media/tool workflows.
- Working steering, internal activation capture, local training, activation and rollback.
- Offline cold-start, reboot recovery, drive-failure handling and verified backup restoration.
- Complete plain-English Ernos application and test source, verified compiler/self-compilation path, generated-artifact provenance, external dependency inventory, dependency locks, model manifests, setup/operation/recovery instructions and acceptance evidence.
- A final requirements-to-evidence report with every acceptance row resolved and no hidden outstanding Phase 1 feature.

Unknowns to resolve during implementation: the fastest qualifying primary model/quantisation, exact drive suitability, runtime support for that model’s full multimodal context, acceptable measured latency targets, and its training-to-inference conversion path. These are qualification work, not permission to quietly narrow the requirements.

## 14 Technical references

Sources used to establish the initial plan; pin exact revisions during implementation.

- [llama.cpp server and runtime controls](https://github.com/ggml-org/llama.cpp/blob/master/tools/server/README.md)
- [llama.cpp evaluation callback example](https://github.com/ggml-org/llama.cpp/blob/master/examples/eval-callback/README.md)
- [MLX local LoRA and QLoRA training](https://github.com/ml-explore/mlx-lm/blob/main/mlx_lm/LORA.md)
- [MLX VLM multimodal runtime](https://github.com/Blaizzy/mlx-vlm)
- [LM Studio native provider API](https://lmstudio.ai/docs/developer/rest)
- [Qwen3.5 397B candidate model](https://huggingface.co/Qwen/Qwen3.5-397B-A17B)
- [Qwen3.8 Flash Next candidate model](https://huggingface.co/Qwen/Qwen3.8-Flash-Next)
- [Kimi K2.5 candidate model](https://huggingface.co/moonshotai/Kimi-K2.5)

The requested archive-and-recap behaviour is specified directly here. It does not depend on an assertion about the private implementation of any commercial chat service.


### User clarification: full-context performance, 15 October 2026

A one-off multi-minute wait to import a huge new conversation is acceptable if ongoing chat is fast. Qualify initial ingestion separately from subsequent turns at full supported context. This does not permit shortening context or adding artificial limits, and a fast identical-prompt cache hit alone does not prove fast ongoing conversation.


### User scope correction and immediate handover, 15 October 2026

Maria explicitly clarified that Phase 1 is the usable local project, not a production-ready launch, and that advanced deferred features such as training must not hold it open. This supersedes the earlier blanket requirement that all advanced stages gate Phase 1. Immediate priority is working phone access and usable chat. The microphone permission check remains deferred. See evidence/current-code-audit/Current verification.txt for the actual verification results and the phone-access repair; it is not authorization to resume deferred work.
