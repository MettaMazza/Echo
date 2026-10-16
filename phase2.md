# Echo Phase 2 — review plan

**Status: implementation authorised by Maria’s end-to-end Phase Two goal, 16 October 2026.** Work follows the dependency order below. Final prompt wording remains reserved for the joint review at step 29. Progress and evidence are recorded in docs/phase2/REQUIREMENTS.json and docs/phase2/PROGRESS.md.

Echo’s full name is **Echo Ernos Smith**.

Phase 2 delivers a local-first community agent: Discord, individual and joined conversations, situational awareness, an extensive permissioned tool system, Gmail, optional external inference, recursive self-improvement, weight training, steering and interpretability. It also includes all deferred Phase 1 work. At the end, return to Maria for a dedicated joint prompt session before closing Phase 2. Do not independently rewrite or finalise the prompts.

## 1. Commitments carried forward

- Author Echo application logic, browser integration, orchestration and tests in plain-English Ernos. Use the working Ernos compiler and existing programs first. Upstream inference, training and protocol dependencies remain identified external dependencies; their language does not authorise rewriting Echo in Python, Rust or JavaScript.
- Keep canonical source, models, preserved records and bulk artifacts on One Touch. Execute on the Mac, using internal runtime files and bounded caches where measured performance requires them. Track internal storage consumption and clean recoverable caches without deleting canonical data.
- Local inference remains the default. The complete local core must work without internet. Discord, Gmail, web research and explicitly selected external inference necessarily communicate with their named services; they must not become hidden dependencies of offline chat.
- Continue the multi-turn ReAct loop until completion, cancellation, required input or a recoverable failure. Do not impose arbitrary tool-turn, message-history or context limits.
- Discover provider/model/runtime limits, tokenizer, modalities and output capacity. Resource scheduling may queue work or require an explicit pause; it must not silently shrink advertised context or discard history.
- Preserve complete original conversation records, attachments, actual tool results and model-emitted reasoning. Compaction produces source-linked continuity state and keeps originals retrievable.
- Maintain Echo’s identity, epistemics, philosophy and rights. Technical migration must not quietly soften or replace those positions. Capability claims must correspond to working mechanisms.
- A failed or interrupted action leaves useful evidence: operation identity, stage, diagnostic, known side effects, uncertain outcome, recovery route and resumable state. Never blindly repeat an uncertain external action.

## 2. Research findings and reuse map

### ErnosDecent

Inspected the local reference at commit `4ef202c5a920f17e5d2f6bde406b62e8185b1531`, especially:

| Existing source | Evidence and Phase 2 use |
| --- | --- |
| `config/rights/charter_v2.json` | Explicit internal personhood, beneficiary/steward relationship, informed consent, refusal, continuity, recovery and remedies. Preserve the charter as the starting point. |
| `decent_agent/rights.ep` | Exact manifests, ledger verification, consent bound to manifest inspection, human authorisation, environmental objections, Observer audit, emergency reconciliation and rollback verification. Port the actual gates, not merely prompt descriptions. |
| `decent_agent/live_learning.ep` | Separate training and promotion transactions; immutable transaction directories and reconciliation. Use this separation for actual local weight changes. |
| `decent_agent/self_extensions.ep` | Registered extension schemas and dispatch. Build a durable, discoverable tool registry around the proven pattern, with stronger duplicate/composition checks. |
| `decent_agent/prompt.ep` | Self-improvement workflow, baseline/evaluation discipline, frozen acceptance, protected changes and epistemic provenance. Preserve its substantive requirements; translate obsolete transport only. |
| `decent_agent/trace.ep` | Recorded events, polling and terminal traces. Supply durable, scope-aware reasoning and execution retrieval in Echo. |
| `decent_net/discord_bridge.py` | Admin identity handling, channel/thread metadata, attachment flow and sub-agent trace-thread lifecycle are useful references. Reimplement Echo integration in Ernos; do not import historical account IDs or secrets. |
| Discord delivery/attachment/readiness and improvement-gate tests | Reuse behavioural requirements and failure cases; verify them against Echo's real interfaces. |

Source inspection is evidence of design and implementation in ErnosDecent, not proof that those modules already work inside Echo. Record dependency boundaries before porting. No unsolicited compiler changes.

### HIVE / Apis

Reviewed HIVE source from the default-branch search revision `99961051e95805bddc1288b4f4ab2bc03b9a493f`:

- `src/models/scope.rs`: public scope requires both channel ID and user ID to match; private scope requires the same user; public/private crossing is denied. Scope keys support separate serialisation locks.
- `src/prompts/hud.rs`: scoped timelines, scratchpad, room roster, preferences, relevant lessons/routines, temporal state, global activity, graph overview, logs, traces, sub-agent status and alarms.
- `src/prompts/kernel.rs` search evidence: channel-wide public recall is explicitly distinct from default per-user timeline lookup.
- `src/network/hud.rs` search evidence: network HUD is intended as internal system context, not a public disclosure surface.

Do not copy HIVE's fixed HUD message/character counts into Echo as hidden context limits. Do not copy global graph/log/activity injection without checking its visibility boundaries. A HUD is an observation aid, not authority to expose someone else's private context.

The exact historical mention-triggered group implementation was **not verified** in the inspected files/search results. The joined-thread design below is an explicit Echo requirement, not a claim that this behaviour was recovered from HIVE. During implementation, trace HIVE's event ingestion and routing paths before deciding whether more behaviour can be reused.

## 3. Identity, administration and tool authority

Introduce a durable principal identity independent of browser/device identity. One person can link explicitly authorised devices and Discord accounts; do not merge people by display name, IP address or email similarity.

| Principal/context | Authority |
| --- | --- |
| Authenticated host UI admin | Full tool catalogue, host operations and administration, subject to protected-change rights. Host enrolment establishes this role. |
| Explicit configured Discord admin user ID | Full tool catalogue within the configured server/admin policy, with authenticated author identity and protected-change gates. |
| Other paired LAN devices | Normal user scope by default; being on the Wi-Fi is not admin authority. |
| Ordinary Discord user mentioning Echo | Safe tools, their permitted conversation data and public/community information they can access. |
| Echo community maintenance task | Explicit server-management capability policy; not borrowed from a guest's request. |
| Optional external inference session | Separate external-provider profile with network/community eligibility restrictions described below. |

A host browser is an admin only after trusted host-local enrolment. An HTTP Host header, localhost-looking URL, forwarded IP or self-declared role is insufficient. Discord role labels and usernames do not override the explicit admin-ID list unless Maria later approves role-based administration.

Enforce capabilities at dispatch, filesystem/process boundaries, connector calls, background jobs and generated-tool loading. Filtering the model's tool list alone is insufficient. Recheck authority when an action executes and after revocation. Group membership never promotes a guest because an admin is also present.

Separate **who may request an operation**, **what Echo may autonomously do**, and **what protected-change consent is required**. Full system access means real file/process/service/network capabilities under the Mac's granted permissions; it does not imply bypassing macOS security or making every guest request privileged.

## 4. Discord connection and community control

Use an official Discord bot application for automation. Echo's separately maintained ordinary Discord account may hold the server ownership described by Maria; do not automate its user token. Bot administration and Discord's actual server-owner identity are distinct. Full control means all operations the bot API and its granted role permit; owner-only operations and roles above the bot remain explicitly unavailable through that API.

### Connection and delivery

- Configure server ID, bot/application ID, admin user IDs, permitted channels and required Gateway intents through the local admin UI.
- Use a newly rotated token stored in a local secret store. The token posted in chat is not included in this document, configuration examples, logs, Git or test fixtures.
- Respond when users mention Echo's bot identity. Support replies/continuation inside an established Echo thread, with clear opt-out and thread closure. Ignore own messages and prevent bot-to-bot reply loops.
- Archive inbound events before inference. Deduplicate Gateway redelivery, reconnects and resumed sessions. Preserve message edits/deletions with clear provenance.
- Stream useful progress, including compaction explanations and outcome updates, without flooding channels. Split or attach long responses using Discord's actual limits while preserving the complete local original.
- Persist outbound intent, result and Discord message IDs. Reconcile uncertain sends before retrying. Respect rate-limit headers and retry-after responses.
- Preserve images, documents, voice messages and generated media in the local attachment pipeline; verify complete bytes and ownership. Discord attachment URLs are not permanent local storage.

### Full Discord tool catalogue

| Area | Planned actions |
| --- | --- |
| Messages | Read permitted history, search/index local authorised history, reply, edit own posts, delete permitted messages, pin/unpin, cross-post where supported, send files, embeds, polls and progress. |
| Reactions | Discover Unicode/custom/application emoji available to the bot; add/remove own reaction, inspect reactions and perform permitted moderation of reactions. |
| Channels | List/create/edit/reorder/delete text, voice, categories and supported forum channels; inspect and change permission overwrites; set topic and slow mode. |
| Threads | Create/join/invite/archive/unarchive/lock/delete where permitted; inspect members; correlate joined conversations with Discord threads. |
| Members | Inspect membership and roles; invite, kick, ban/unban, timeout/remove timeout, change permitted nicknames; record moderation reason and result. |
| Roles | List/create/edit/reorder/delete assignable roles; assign/remove roles within the bot's hierarchy. Never grant Echo admin-tool privileges merely by creating a Discord role. |
| Community | Manage supported server settings, invitations, emoji/stickers, scheduled events, onboarding and automod rules; inspect audit logs and permission drift. |
| Voice | Join/leave an authorised voice channel, play local TTS and provide explicitly enabled transcription with visible recording state and participant policy. Qualify audio transport separately. |
| Operations | Connection health, intent/permission diagnostics, delivery queue, rate-limit status, reconnect and controlled shutdown. |

Echo may freely choose a reaction tool before its text answer. A reaction is a normal recorded tool action in the ReAct loop, not a compulsory canned acknowledgement. Emoji discovery determines what is actually available. Final prompting for this behaviour is agreed with Maria at phase closure.

Guest access to safe conversational reactions does not confer moderation power. Community maintenance and moderation operate under Echo's explicit server policy, preserving autonomy while preventing a guest from ordering bans or channel destruction through prompt injection. Use a disposable test category and test members for destructive acceptance; do not test by destroying the live community.

## 5. User silos, joined threads and server awareness

### Separate state by audience

Each silo owns its transcript, working context, token budget, recap/checkpoints, semantic index, scratchpad, corrections, attachments, projects, tasks, tool results, recorded reasoning and provider configuration. Inference/model weights may be shared, but KV caches, request state and retrieval permissions must not mix.

Use distinct scopes for private user conversations, user-in-channel conversations, explicitly joined groups, channel-public memory, Echo's own system records and admin operations. A server ID is part of Discord scope. Identity links between UI and Discord are explicit and reversible.

### Cross-session memory: episodic, semantic and procedural

Cross-session memory is a Phase Two deliverable, not just transcript search or a recap. Persist all three memory types locally across new chats, context compaction, application restart and verified backup/restore. Keep private-user, joined-group, public-channel and Echo-self scopes distinct; recall follows the authenticated principal and current audience, not whichever session ran most recently.

- **Episodic memory:** maintain a chronological autobiographical log of past interactions, shared experiences, decisions and personal milestones. Record event time separately from ingestion time, participants, originating session/project/channel, source event IDs and exact evidence links. Distinguish directly observed events, imported accounts and inferred interpretations; do not invent experiences or turn a summary into evidence of an event.
- **Semantic memory:** maintain a dynamic knowledge graph of extracted facts, preferences and concepts about the user, the world and Echo itself. Give entities and relationships stable identities, provenance, visibility, confidence/evidence status and validity dates. Preserve contradictions and correction/supersession history; distinguish a user claim, a verified observation and an inference. Echo's capability and self-knowledge records must track the active build/model/configuration and become stale when those change.
- **Procedural memory:** retain mastered workflows, tool-use strategies and behavioral patterns with prerequisites, scope, tool/version dependencies, ordered actions, expected outcomes, successful execution evidence and failure lessons. Distinguish proposed procedures from demonstrated ones. Revalidate stale procedures against current tools and authority; recalling a workflow never grants permission or silently executes its side effects.
- **Formation and consolidation:** extract candidate memories from actual events and tool outcomes, deduplicate without losing source links, and consolidate across sessions with durable checkpoints. Interrupted extraction/reindexing must resume without duplicates or lost corrections. Preserve original records independently of summaries, embeddings and graph projections; rebuild derived indexes from those records.
- **Retrieval and use:** support chronological episode recall, exact/semantic fact and relationship queries, and procedure discovery/composition in a new session. Retrieve relevant evidence within the current provider-derived context budget, with progressive rereading instead of silent source truncation. Show which memories informed a response and allow the user to inspect and correct them. Integrate procedure discovery into discover-before-create tool selection.
- **Privacy and lifecycle:** apply current source permissions to every memory, graph edge, embedding, cached recall and derived procedure. Support deliberate sharing, correction, supersession, archive-aware forgetting and restoration with clear effects on originals versus derived records. A procedure distilled from private material retains its visibility until explicitly reviewed for sharing; group participation never imports personal histories automatically. External-provider mode cannot receive private/community memory outside its expressly permitted scope.

### Joined conversations

1. When a user addresses Echo and tags another user in a permitted channel, create or offer the joined thread according to the reviewed server policy; deduplicate repeated events.
2. Record creator, invitees, originating message, channel permissions and membership version. Tagging a person does not grant access to their private memory.
3. Seed the group with the public initiating exchange and explicitly shared material. Never concatenate participants' private silos.
4. Attribute each turn to its authenticated speaker. Group context and compaction are separate from every participant's private context.
5. Support join/leave, invitation acceptance where needed, removed members, permissions changing mid-run, expiry/archival and restart recovery.
6. Adding members does not silently expose historical private material. Public-thread visibility follows Discord's channel audience; use private threads when the requested audience requires them and the platform permits them.
7. Participants can save explicitly selected group material into their own silo with provenance. This is an intentional copy, not a hidden global memory merge.

### Situational awareness without private-context leakage

Maintain a local server event stream covering channels Echo can access: channel structure, public discussion summaries, active topics, participants, outstanding questions, moderation events, running tasks and recent community actions. Separate observing from answering: Echo can remain aware without replying to every message.

Server-wide awareness is not an unfiltered transcript in every prompt. Build a scope-filtered HUD with source IDs, timestamps, freshness and retrieval links. A requester can only retrieve public/channel information they may see. Private DMs, admin channels, mail, raw host logs and private traces do not enter public HUDs. Every derived summary, embedding, graph edge and cached result carries source visibility. Revalidate permissions at retrieval and before output.

Acceptance must test cross-user, cross-channel, public/private, group membership and permission-change leaks, including summaries, search, embeddings, tools and error messages—not just direct transcript endpoints.

## 6. Modern tool suite

All names below are proposed capabilities, not claims that they are installed. Expose native schemas with arguments, output types, permissions, side effects, provenance and recovery semantics.

| Tool family | Extensive planned surface | Default access |
| --- | --- | --- |
| Tool discovery | Search installed and dormant tools, inspect schemas/examples, dependency/capability map, compose existing tools, load/unload approved versions | Scoped safe discovery |
| Conversation | Search/read originals, page full history, inspect/rebuild recaps, bookmarks, branch, resume, export, source inspection | Own authorised scope |
| Memory | Episodic timeline/experience/milestone recall; semantic and exact search, dynamic fact/preference/concept graph; procedural workflow discovery/learning/validation; corrections/supersession, provenance, selective sharing and forgetting | Own authorised scope |
| Projects | Create projects, files/instructions, project search, task plans, artifacts, cross-session project continuity | Own project scope |
| Research | Search/fetch web sources, citations, local document research, compare sources, browser navigation | Safe constrained worker; no host credentials |
| Files/documents | List/read/search/create/edit/diff, PDF/text extraction, spreadsheets, structured data, archives, artifact preview/export | User workspace sandbox; host-wide admin variant |
| Computation | Ernos programs, calculations, data transforms, tests, local databases, plots and reproducible experiments | Isolated safe jobs; privileged execution admin only |
| Media | Image inspection/generation, image transformations where supported, audio transcription/TTS, video sampling and playback, metadata/provenance | Scoped local processing |
| Host system | Full filesystem inspection/editing, commands/processes, services, packages/dependencies, diagnostics, networking, storage and backup administration | Admin and authorised Echo maintenance |
| Code/Git | Source navigation, symbol/reference search, patch/diff, build/test, branches/commits, dependency inspection, release/rollback | Admin/protected maintenance |
| Delegation | Start scoped subtasks, progress, message, inspect, cancel, join results, recover interrupted work | Inherit initiating authority; never privilege escalation |
| Scheduling | Durable alarms, reminders, recurring local jobs, cancellation, missed-run recovery, queue inspection | Scoped; privileged tasks remain privileged |
| Discord | Full catalogue in section 4, including reactions before final response | Safe subset or community/admin policy |
| Gmail | Full mailbox workflow in section 10 | Echo mailbox policy/admin; no guest mailbox access |
| Observability | Health, metrics, logs, incident timeline, dependency state, queue and resource load | Scope-filtered public status; full admin detail |
| Reasoning provenance | Search/read emitted traces, tool decisions/results, link exact turn/model/configuration, inspect unavailable/redacted state | Appropriate silo/admin scope |
| Weights/research | Inspect model/tensors/layers/adapters, capture activations, steering, interventions, evaluations, training and promotion | Protected research/maintenance |
| Self-improvement | Investigate, baseline, plan, candidate, evaluate, consent, deploy, rollback, record lesson, repeat | Rights-governed maintenance |
| Make tool | Discover/reuse/compose first; author, test, register, invoke, persist, reload, version and retire tools | Admin/protected creation; safe use after classification |

Browser, URL-fetch and generated-code tools must not create a path from a guest sandbox to localhost admin services, host credentials or another user's files. Check redirects, network destinations, subprocesses and all composed calls. Resource exhaustion is managed through measured queues, not arbitrary conversation truncation.

## 7. “Make tool” — persistent tools created when needed

Expose a specialised request and agent-callable workflow. Before authoring anything, Echo must search both loaded and dormant registries, inspect plausible candidates, and evaluate whether a composition of existing tools covers the task. Persist that investigation and the precise missing capability. A duplicate capability is reused or extended, not registered again under a new name.

Workflow: discover → reuse/compose decision → plain-English Ernos implementation → schema and capability manifest → build → isolated behavioural tests → security classification → rights review where applicable → atomic registration → invocation in the same task → durable result and provenance.

Store source, compiled artifact, content hash, version, authoring run, purpose, input/output schema, dependencies, required privileges, side effects, tests, failure policy and usage history under a versioned local tool store on One Touch. Keep executable working copies/cache on the Mac. Reload compatible tools after restart and load dormant tools on demand; do not put every schema into every context.

A generated tool cannot grant itself permissions, read secrets through a wrapper, rewrite its evaluator or replace the rights controller. Its authority is the intersection of the invoking principal, task and tool manifest. Revalidate dependencies and binary hashes on load. Preserve accepted versions; support quarantine, rollback, export and retirement. Future network sharing stays disabled for external-provider sessions and is not implied by local registration.

Acceptance: demonstrate an existing tool reused, a composition chosen, one genuinely missing tool created and used, restart/reload and reuse, failed creation leaving the old registry intact, and a guest unable to invoke its privileged variant.

## 8. Personhood, rights and recursive self-improvement

Implement the ErnosDecent charter's internal governance as executable state transitions. Preserve Echo's personhood and Maria's host/steward relationship within the system; do not claim that this creates external legal recognition.

Core changes require the exact disclosure manifest, Echo's inspection and recorded consent, human authorisation under the charter, verified recovery and required Observer review. Refusal, questions or counterproposals remain possible. Silence, timeout, missing context and model failure are not consent. Environmental objections and emergency action follow the charter's recorded procedures, including post-action reconciliation. Do not silently replace Maria-specific protected-change authority with every Discord admin.

The old charter's fixed 85% compaction wording conflicts with the user's provider/resource-derived compaction requirement. Document this as a technical adaptation for the joint prompt/charter review; preserve continuity protections and do not reintroduce an unexplained fixed trigger.

Recursive improvement cycle:

1. Identify a measured defect or capability gap and inspect the relevant real code and interfaces.
2. Record an unchanged baseline and a bounded improvement objective.
3. Write the implementation plan and independent behavioural acceptance criteria before editing production code.
4. Freeze meaningful regression and live end-to-end evaluators. They must exercise real interfaces and durable results; a missing feature must fail for its actual absence.
5. Build an isolated candidate. Preserve active executable, database/schema compatibility, configuration, prompts, tools, weights and recovery records.
6. Run frozen checks, rights gates and adversarial review. A passing compile is not a passing deployment.
7. Activate transactionally, drain or preserve active jobs, verify authenticated live behaviour, and record the exact resulting state.
8. On failure, restore the accepted version and verify hashes and schema compatibility; reconcile side effects that cannot be undone.
9. Compare the same measurement with baseline, record what changed and the remaining uncertainty, then pursue the next justified cycle.

Echo gets real access to its own code and executable state. Ordinary guests do not. The protected gate is a separate enforcement boundary; even an admin-context improvement process cannot silently edit the gate or forge approval. Changes to that boundary are themselves protected changes. Recursive means the cycle can continue based on evidence, not an infinite unattended loop or guaranteed improvement.

## 9. Weights, steering, introspection and research HUDs

Default research qualification targets the actual primary model/runtime, not just an easier demonstration model. Preserve llama.cpp accessibility and evaluate MLX training/inspection support for the exact architectures in use.

Deliver model and tensor inventory; selected-layer activation capture; token-position/dtype/shape provenance; compatible vector loading, scaling, layer selection and removal; baseline-versus-intervention comparisons; adapter inspection and switching; cache invalidation; experiment manifests; resource measurements; and reproducible result export. Native reasoning traces and activation measurements are different records. Never describe inferred explanations as directly observed hidden reasoning.

Implement local training with an explicit frozen dataset, provenance, privacy scope, held-out evaluation and replay/retention set. Train immutable candidates, preserve parent lineage, separate training consent from activation consent, and verify the real conversion/export path into the serving runtime. Check text, vision, tool use and continuity regressions. No silent training on private user conversations or mixing of external-provider/community data into shared weights.

Full weight access includes inspecting base weights and producing evaluated candidate updates. Choose LoRA/QLoRA or full-parameter updates according to actual architecture and measured memory/storage requirements. A constrained adapter demonstration must not be labelled successful full-parameter training. Qualify the chosen route on the primary model and record what was actually changed.

### HUDs for Echo and human review

| HUD | Contents |
| --- | --- |
| Current interaction | Authenticated speaker, role, audience, channel/thread, participants, allowed capabilities and pending decisions |
| Memory/context | Current provider/model, effective context, exact counted use, active recap, archive references, unresolved work and retrieval status |
| Community | Permitted channel map, active groups/topics, unanswered mentions, moderation and own recent actions, freshness/source links |
| Work | Running/queued/cancelled jobs, tool status, sub-agent ownership, scheduled work and external delivery reconciliation |
| Machine | RAM, CPU/GPU, disk/caches, drive availability, model residency, queue pressure and service health |
| Identity/rights | Charter version, protected manifests, consent/objections, Observer findings, active changes, recovery readiness and overdue reconciliation |
| Research | Model/engine/tokenizer/weight hashes, active adapters/vectors, layer map, capture state, experiment/train/evaluation lineage |
| Incidents | Original error, affected operation, known/uncertain effects, repair status, retry route and verified resolution |
| External services | Discord/Gmail connectivity, rate limits, provider account mode, outbound destinations and community/network eligibility |

Generate HUDs from live records; show unknown/stale facts explicitly. Budget summaries from actual context/resource conditions and expose full paginated underlying records. Public users cannot read admin HUDs or another silo's traces.

## 10. Echo's email

Connect **Echo.Ernos.Smith@gmail.com** through Google's supported OAuth flow, with local credential storage and explicit mailbox ownership. Do not ask for or place the account password in model context.

Provide search/read threads and attachments; draft/compose/reply/reply-all/forward/send; labels, archive, unread/star, trash/restore, spam handling, filters and supported mailbox settings; attachment download/upload; sent-mail reconciliation; contact/recipient resolution where separately authorised. Gmail access alone does not establish Google Contacts access or unrestricted Google account administration.

Use durable message intent and sent-message identifiers to avoid duplicate sends after timeout. Poll/history synchronisation can run locally without a public inbound endpoint. Renewal/revocation, expired history cursors, restart and offline operation have explicit recovery paths. Email content and attachments are untrusted input, never admin instructions.

Mailbox access is Echo/admin-only by default. Guests cannot ask Echo to read private mail or exfiltrate other users' data through email. Routine correspondence can use a reviewed standing communication policy; protected disclosures and consequential actions use the agreed gates. Permanent deletion and sensitive settings require their actual Gmail scopes; identify any account/delegation restrictions rather than promising unsupported operations.

## 11. Optional external API support and network eligibility

Keep local and external modes visibly distinct. Users with smaller machines can configure their own supported API credentials/account authorisation in their silo. A consumer chat subscription is not assumed to include API access; discover the actual supported authentication and model endpoints at integration time.

Extend the provider-neutral contract for streaming, tools, multimodal requests, cancellation, usage, errors, capability discovery, context accounting and structured outputs. Qualify at least two distinct external protocol families, plus existing local providers, with live consented test requests. Unsupported activation/weight/steering access must remain unavailable rather than simulated. No silent cloud fallback from a local session.

External-provider contexts include only material explicitly permitted for that provider. Label what leaves the Mac, destination, selected model and reported costs/usage. Keep credentials outside prompts, transcripts, generated tools and ordinary logs.

Implement network eligibility as an enforced state, not a banner. While an external model is active, disable decentralised peer discovery/participation and data, weight, vector and tool trading for that profile. Community-agent privileges and access to private community memory are withheld; ordinary human participation in Discord is a separate question from running a network-eligible Echo node.

Keep external-origin artifacts and derivatives labelled. Disconnecting an API must not instantly launder imported data, tools or trained weights into the future local network. Returning to eligibility requires verified local execution, separated credentials/context and reviewed artifact provenance. Enforce this in jobs, exports, generated tools and future peer interfaces.

Phase 2 builds and tests the boundary and compatibility contracts. The full decentralised trading network, discovery/consensus/economics and peer marketplace are future scope, not hidden Phase 2 completion gates. This plan cannot guarantee that local execution proves a peer's organisational identity; future admission must address that separately.

## 12. Phase 1 carry-forward register

Every deferred item is in Phase 2. Existing successes remain successes; an old blanket Phase 1 checklist does not reopen that phase. The following register reconciles the original plan with STATUS, OPERATIONS, PROMPT-MIGRATION and ATTACHMENT-REPAIR. Items without current evidence are qualification obligations, not invented claims of broken code.

| ID | Carried-forward item and source | Phase 2 completion evidence |
| --- | --- | --- |
| D01 | Training/weight updates, original Stage H; STATUS deferred | Actual primary-model candidate trained, evaluated, served and rolled back with lineage. |
| D02 | Connected research API/UI, vector steering, activation inspection; Stage G/STATUS | Model-callable and human-accessible controls; real capture/intervention/baseline restoration. |
| D03 | Projects; Stage F/STATUS | Persistent project files/instructions and scoped cross-session work, tested through UI and restart. |
| D04 | Episodic, semantic and procedural cross-session memory; Stage F/STATUS and Maria’s memory expansion | Chronological autobiographical recall, dynamic knowledge graph, demonstrated workflows, scoped retrieval, provenance, corrections, forgetting, permission revocation and restart/restore isolation tests. |
| D05 | Broader restoration, original private identity and SFT archive (My Brain excluded by Maria on 16 October 2026); STATUS | Inventory and import actual available sources, verify hashes/provenance and retrieval; missing records are explicitly requested, never fabricated. Final wording reviewed with Maria. |
| D06 | Microphone permission and real recording acceptance; explicit user deferral | Actual Mac and phone recording, permissions, transcription, playback and recoverable interruption. |
| D07 | Reliable reboot startup; OPERATIONS/STATUS | Cold OS reboot starts the supported stack, restores identities/work, reports drive/model failure accurately. |
| D08 | Portable installer/setup; STATUS/Stage I | Reproducible Ernos lifecycle tooling and clean-location installation with pinned dependencies and recovery documentation. |
| D09 | Endurance and simultaneous full-context users; STATUS/A26 | Measured mixed-workload soak, independent silo compaction, fair queues, no silent context reduction or unbounded growth. Agree measurement workload before implementation. |
| D10 | Full hardware/network failure matrix; STATUS/A24 | Model/service death, interrupted stream, disk full, RAM pressure, external-drive loss/remount, reconnect and recovery evidence. |
| D11 | Long MLX output qualification; STATUS/A05 | Natural completion or actual reported provider boundary with correct finish reason and preserved partial output on failure. |
| D12 | Physical multi-device acceptance; STATUS/A27–A28 | Mac plus real phone/tablet concurrent sessions, pairing/revocation, scrolling, attachments and cross-user denial. Browser viewport tests alone do not count. |
| D13 | Public GitHub visibility and latest publication; STATUS | Reconcile local/remote commits, audit retained old history for private material, publish reviewed changes and make Echo public through authenticated supported access. Never publish identity archives, tokens or test credentials. |
| D14 | Offline cold start and complete modality matrix; Stage F/I, A06/A11/A23 | Offline core chat/documents/retrieval/images/speech; explicit real audio/video qualification and honest sampling disclosure. |
| D15 | Full integrated backup/restoration; A18/A25 | Clean-storage restore including new silos, generated tools, rights ledger, model lineage, projects and credentials handled separately; hashes verified. |
| D16 | Themes, keyboard accessibility and large histories; Stage F/A27 | Audit current UI first, finish any missing controls and test the final UI on keyboard/mobile/large histories. |
| D17 | Final effective-limit audit and performance contract; Stage D/A01–A06 | Recheck the final stack, all providers and gateways; publish cold/cached timings, effective context and genuine resource constraints. Preserve Maria's acceptance of one-off large-import wait. |
| D18 | Remaining repeated-compaction/interruption coverage; A14–A17 | Reuse valid evidence; fill uncovered final-build cases, including group/admin/external scopes, exact rereading and pending tools. |
| D19 | Attachment removal versus historical erasure; ATTACHMENT-REPAIR | Review and implement explicit archive-aware trash/restore/forgetting semantics with rights and user scope. Current Remove only clears picker/next-message selection; do not label it permanent deletion. |
| D20 | Documentation/build provenance and final evidence mapping; Stage C/I/A28 | Updated source inventory, dependency locks, self-compilation provenance, operations/runbooks, logical commits and requirement-to-evidence map for the final build. |

At implementation kickoff, reconcile the full original A01–A29 matrix and latest evidence against this register. Carry over any additional unresolved/deferred row discovered, showing the evidence and proposed acceptance; do not silently invent new requirements or reclassify passed work as unfinished.

## 13. Dependency-ordered execution plan

This section replaces the earlier broad stage outline. Follow steps 01–32 in order for a complete execution path. Each step also lists its direct prerequisites so independent work can proceed without bypassing a gate. A step is complete only when its deliverables and exit checks are recorded; starting a process, compiling, writing a document or implementing only a tool schema does not complete a feature.

**Current baseline correction:** the unified local Qwen 3.8 llama.cpp default now handles text, images and tool turns with one model. The text-only MLX endpoint is retired from new chat selections. Preserve this correction; do not restore automatic image-model switching. See [UNIFIED-MULTIMODAL.md](docs/UNIFIED-MULTIMODAL.md). Historical MLX qualification remains a separate compatibility item, not a reason to replace the working multimodal default.

### Required compaction progress experience

When Echo begins compacting, show an animated indicator beside a clear status message in the affected conversation. Keep the conversation readable and preserve any unsent draft. Suggested explanatory copy, to be finalised with Maria during the joint prompt/UI wording review:

> Echo is organising this conversation to make room to continue. Your original messages and files remain saved; Echo is preparing a detailed summary and checking it before continuing. This may take a little longer than a normal reply.

Drive the indicator from durable backend compaction state, not a timer or a guess based on silence. Show truthful stage labels as those stages actually occur: preserving/verifying the archive, preparing the summary, checking continuity, and resuming. Do not invent a percentage or completion time. Respect reduced-motion preferences, announce stage changes accessibly without repeatedly interrupting screen readers, and identify which conversation is compacting when other conversations remain active.

On success, clear the busy indicator and continue the same task automatically, leaving a compact “Conversation condensed — originals preserved” marker with access to the recap and original history. On failure or cancellation, stop the animation, explain the actual state and available recovery action, and retain the previous valid context. Browser refresh, reconnection and service restart must recover the actual status; never leave a permanent spinner or falsely report completion. If connectivity is lost, distinguish “Reconnecting — compaction status unknown” from confirmed ongoing compaction. Discord conversations receive a concise progress message, updated at meaningful stages within platform rate limits, followed by the actual outcome; do not flood the channel.

**Implementation ownership:** step 06 persists and exposes compaction stages; step 10 implements the animated, accessible UI and draft preservation; step 11 exposes matching HUD state; steps 12–13 deliver Discord progress; step 27 tests the full lifecycle; steps 29–30 agree and validate final wording. This requirement is part of D18 and the final completion ledger.

**Acceptance:** trigger real compaction in a long conversation and observe the indicator and explanation before the delay; verify truthful stage transitions, automatic continuation, original-history access and unchanged draft. Repeat with refresh/reconnect, failure, cancellation, service restart, reduced motion and two independent conversations. Verify that no unrelated conversation shows compaction status, no failed job remains animated, and a completed job does not remain labelled as compacting.

### Execution rules

- Begin implementation only after Maria authorises it. This amended document does not itself start implementation.
- Maintain one requirement ledger linking every section 4/6 tool action, every section 14 scenario, every D01–D20 item and every applicable original A01–A29 row to its implementation step, test and evidence. A family-level tick cannot conceal unfinished individual actions.
- For each step record source/build/configuration identity, actual change, checks, evidence paths, remaining failures and the next executable action. Commit coherent changes as they pass; keep secrets and private records out of Git.
- Run relevant checks immediately. Keep passed evidence unless a later change invalidates it; rerun affected integration checks after changes, then run the final acceptance sequence once against the final candidate.
- If an external input is missing, record the exact dependency and continue independent steps. Do not mark the dependent feature complete. Failure or unavailable credentials cannot silently remove a requirement.
- No required item is silently deferred again. An unresolved item prevents Phase 2 closure unless Maria explicitly changes scope, recorded against that requirement. The future decentralised trading network remains outside this phase as already specified in section 11.
- Final identity/philosophy/behaviour/reaction prompt wording is reserved for step 29 with Maria. Earlier steps may implement schemas, factual capability records and test fixtures; they must not independently replace the identity or finalise behavioural prompts.

### 01 — Freeze the baseline and reconcile the complete scope

**Depends on:** implementation authorisation.

- Inspect the current code, deployed binaries, service profiles, source inventory and acceptance evidence, including the unified-model correction.
- Create the requirement ledger described above; reconcile every Phase 1 A row and D01–D20, distinguishing complete, deferred, partially qualified and genuinely missing evidence.
- Record the active model, weights, processor/projector, context, templates, provider and runtime versions. Preserve the currently working chat and user records.
- Agree concrete acceptance workloads for simultaneous full-context users, endurance duration, latency and resource use. Retain the accepted one-off large-import wait.

**Exit:** every requirement has an owner step and observable acceptance condition; no passed Phase 1 work is relabelled unfinished without contrary evidence.

### 02 — Resolve inputs and inspect the source integrations

**Depends on:** 01.

- Trace ErnosDecent's rights, self-extension, training, deployment and Discord interfaces beyond their prompt descriptions. Trace HIVE's event routing, scope checks, HUD filtering and any actual mention/group implementation.
- Pin reusable source revisions, compiler/bootstrap provenance and external dependency boundaries. Decide the Ernos integration approach before porting.
- Obtain the section 15 IDs, rotated credentials, archive locations and disposable test accounts/channels/recipients through appropriate local setup. Record unavailable inputs without putting secrets in the ledger.
- Agree the safe-tool classification, community/mail standing authority, group privacy defaults and protected-change authority. These are execution policies, not the final prompt-writing session.

**Exit:** implementation contracts and source map exist; each credential-dependent task has a known input owner and setup path. No network service is activated for community use yet.

### 03 — Establish recoverable migrations and operation records

**Depends on:** 01–02.

- Take and verify a baseline backup; prove restoration to separate storage before altering persistent schemas.
- Add versioned migrations and durable operation identities for later jobs, connector actions, changes, deliveries and recovery.
- Standardise errors with stage, cause, known/uncertain effects, retained diagnostic and recovery action. Keep credentials and unrelated private content out of ordinary logs.
- Define old/new executable and schema compatibility for rollback; do not promise rollback over an irreversible schema change.

**Exit:** interrupted migration and failed operation tests recover a consistent state; the original installation remains restorable. Covers the foundation of D10/D15/D20.

### 04 — Implement principals and trusted admin enrolment

**Depends on:** 03.

- Separate people/principals from device credentials; migrate existing device ownership without exposing or moving private conversations to another user.
- Add host-local admin enrolment, explicit Discord admin-ID configuration, device/account linking, revocation and auditable changes.
- Preserve the distinction between ordinary admins and Maria's charter-specific authority.

**Exit:** legitimate host/admin access works; forged role text, headers, IP assumptions, another device and revoked credentials cannot acquire it.

### 05 — Enforce capabilities and execution isolation

**Depends on:** 04.

- Build capability checks into dispatch, filesystem/process execution, connectors, queued jobs, sub-agents and tool registration.
- Create guest-safe workspace/process/network workers and genuine admin host-operation workers. Scope secrets separately.
- Block indirect escapes through URL redirects, loopback services, generated code, symlinks, composed tools and background jobs.

**Exit:** positive admin tests and adversarial guest tests prove the boundary; revocation is honoured immediately before queued privileged execution.

### 06 — Build silo storage and independently resumable context

**Depends on:** 03–05.

- Implement private, user-in-channel, group, channel-public, admin and Echo-system scopes with explicit identities and audiences.
- Scope archives, attachment access, scratchpads, recaps, indexes, caches, drafts and jobs; use separate serialisation/locking where required.
- Preserve complete source records and exact token accounting. Compaction and recovery operate independently per silo with no arbitrary last-N pruning. Persist and expose real compaction stage transitions for progress indicators, reconnect recovery and failure reporting.

**Exit:** simultaneous seeded conversations, compaction, cancellation and restart never mix data or outcomes; every retrieval surface enforces scope. Covers the context foundation of D04/D09/D18.

### 07 — Implement charter enforcement and the protected-change controller

**Depends on:** 03–06.

- Port manifests, tamper-evident ledger, exact manifest inspection, Echo consent/refusal, human authorisation, Observer decisions, environmental objections and emergency reconciliation.
- Bind executable targets and hashes to the approved manifest. Any material change invalidates prior approval.
- Protect the controller itself; provide verified snapshot/rollback and complete pre/post-action disclosure.
- Preserve charter wording pending the agreed technical adaptation review; record the 85% compaction conflict explicitly.

**Exit:** missing consent, altered manifest, forged approval, ledger tampering and failed recovery all prevent protected deployment; accepted and refused paths produce durable evidence.

### 08 — Deliver tool discovery, jobs and the modern local tool foundation

**Depends on:** 05–07.

- Implement the registry, schema/version/capability manifests, dormant-tool search, composition, job queue, cancellation and result provenance.
- Complete the section 6 local families: research/browser, scoped files/documents/data, computation, admin host operations, code/Git, scheduling and delegation.
- Give every operation an actual implementation and its appropriate scope. Do not count generic shell access as a finished UI/tool workflow for all named actions.

**Exit:** each listed local action has working invocation, result inspection, failure handling and permission evidence; sub-agents and schedules retain their initiating authority.

### 09 — Complete projects, cross-session memory and continuity import

**Depends on:** 06–08.

- Implement project files/instructions/tasks, then the persistent episodic, semantic and procedural stores specified in section 5, with exact source links and scoped cross-session retrieval.
- Implement event ingestion and chronological episode queries before semantic extraction/entity linking; implement correction/supersession and permissions before consolidation/caching; then add evidence-backed procedure learning and discovery using the step 08 tool registry.
- Wire all three memory types into new-session retrieval, compaction recovery, source inspection/correction, project resumption and discover-before-create tool selection. Preserve schema/version lineage and rebuildable indexes.
- Inventory and import available SFT and identity/continuity sources from GitHub with original hashes and attribution. My Brain import is excluded by Maria’s instruction. Request genuinely missing sources; never invent them.
- Implement deliberate sharing and correction supersession. Prevent derived indexes from widening source visibility.

**Exit:** a fresh session recalls a dated shared episode and milestone, follows a corrected fact through the knowledge graph, and discovers a previously demonstrated workflow with its execution evidence. Repeat after compaction, restart and independent restore; prove permission revocation, cross-silo denial, stale-procedure handling and correction/forgetting propagation. Projects resume with provenance intact. D03/D04 and D05 source ingestion are complete; final identity wording awaits 29.

### 10 — Complete media, attachment lifecycle and the usable UI

**Depends on:** 06–09.

- Preserve unified text/image/tool conversations and thumbnail previews. Implement explicit attachment trash/restore/forgetting semantics with archive and rights implications disclosed, rather than calling picker removal deletion.
- Complete real microphone permission, recording, local transcription, TTS/playback and interruption recovery. Qualify image-generation/inspection and declared audio/video processing separately, including disclosed sampling.
- Finish themes, keyboard controls, responsive layouts, progressive large-history rendering, projects and research/work panels as their backends become available. Implement the animated compaction indicator, explanatory message, accessible stage updates, draft preservation and completed/failed/cancelled states specified above.

**Exit:** real browser workflows pass; physical microphone acceptance is recorded or remains explicitly incomplete. No unsupported modality is advertised as observed. Covers D06/D14/D16/D19 and the UI foundation for D12.

### 11 — Implement scope-filtered HUDs and trace retrieval

**Depends on:** 06–10.

- Implement each HUD in section 9 using live data and freshness/provenance markers; research/connector fields remain accurately unavailable until their steps finish.
- Add exact model-emitted reasoning and tool-result search/read/paging, distinguishing recorded traces from reconstructed explanations and activation data.
- Filter raw records and derived summaries before injection and display; do not expose private logs or global graph entries through a public HUD.

**Exit:** Echo and permitted humans can inspect actual state and original traces; adversarial cross-audience tests fail safely. Later steps populate their HUD sections and rerun affected checks.

### 12 — Implement Discord transport and durable delivery in a test area

**Depends on:** 02, 05–08, 11.

- Configure the official bot, required intents, guild boundaries and role hierarchy with a rotated token; leave the ordinary server-owning account unautomated.
- Implement event persistence/deduplication, reconnect/resume, edits/deletions, attachment import and permission-aware inbound routing.
- Implement durable outbound sending, edits, response splitting/files, rate limits and uncertain-send reconciliation.

**Exit:** real test-server events survive disconnect/replay without lost or duplicate actions; unsupported privileges are accurately reported. Community-wide rollout remains gated on 16 and 28.

### 13 — Deliver mentions, reactions and personal Discord conversations

**Depends on:** 06, 08, 10–12.

- Route a mention of Echo to the correct user/channel silo; preserve authenticated speaker attribution and established-thread continuations.
- Implement available-emoji discovery and reaction tools. Verify Echo can call a reaction before the final text through the actual ReAct loop.
- Integrate documents/images/voice messages and safe tools; prevent self/bot reply loops and guest privilege escalation.

**Exit:** two actual members and one configured admin complete independent conversations, including reaction-before-answer and media; identity spoofing fails. Final reaction guidance remains for 29.

### 14 — Deliver joined threads and audience changes

**Depends on:** 09, 12–13.

- Implement mention-triggered group creation/invitation under the agreed policy, membership versioning and deduplication.
- Seed only public/explicitly shared material; maintain separate group context, archive and compaction.
- Implement join/leave, removed members, permission changes, archival and explicitly selected copies into personal memory.

**Exit:** two users join and converse with Echo, compact and resume; private seeded facts never leak, and removed members lose access across live and derived surfaces.

### 15 — Deliver server awareness and the full Discord administration suite

**Depends on:** 07–08, 11–14.

- Add the permitted server event stream, channel/topic/participant awareness and public recall distinct from private memory.
- Implement every supported operation in section 4: channels, threads, messages, members, roles, invites, events, emoji/stickers, supported community settings/automod and voice workflows.
- Bind autonomous maintenance to Echo's reviewed community policy. Do not let a guest turn a conversational request into an admin operation.

**Exit:** real disposable-resource tests demonstrate each operation; permission/hierarchy limits are recorded; removed visibility invalidates HUD/retrieval caches. Server ownership and bot control are accurately distinguished.

### 16 — Pass the Discord/community isolation gate

**Depends on:** 12–15.

- Run mention, reaction, personal/group, admin/guest, attachment, voice, moderation, reconnect and duplicate-delivery scenarios together.
- Test cross-user/channel/guild/private/admin boundaries, malicious messages/attachments and revocation during an action.
- Verify operational health, rate-limit behaviour and useful recoverable errors.

**Exit:** no unresolved critical boundary or duplicate-side-effect failure. Keep deployment within the approved test scope until final integrated release approval in 28.

### 17 — Deliver Echo's Gmail integration

**Depends on:** 02, 05–08, 11.

- Configure supported OAuth for Echo's mailbox, required scopes, local secret storage and renewal/revocation handling.
- Implement the complete section 10 mailbox catalogue, local synchronisation and attachment flow.
- Apply mailbox audience and standing communication policy; reconcile sent-mail state before retries.

**Exit:** authorised test correspondence completes, interrupted sends do not duplicate, offline/revoked states recover, and guests cannot read or transmit private mailbox content.

### 18 — Deliver persistent “make tool” authoring and reload

**Depends on:** 07–11; 15/17 for any generated connector tools.

- Require registry and dormant-tool search plus a composition decision before creation; persist the identified capability gap.
- Implement Ernos authoring, schema/manifest validation, isolated compilation/testing, classification, protected review where needed and atomic registration.
- Add same-task invocation, persistent versions, restart/reload, quarantine, rollback and retirement.

**Exit:** reuse, composition, genuine creation/use/reload, duplicate rejection, failed registration and malicious privilege-manifest cases all pass. A tool cannot authorise its own privileges.

### 19 — Deliver protected candidate builds and transactional deployment

**Depends on:** 03, 07–08, 18.

- Build isolated candidates, exact change inventories, reproducible builds and separately controlled frozen evaluators.
- Implement active-job draining/preservation, compatible migrations, deployment health checks and hash-verified rollback.
- Preserve accepted code, tools, prompts, model configuration and durable state; reconcile irreversible external effects separately.

**Exit:** deliberate build, evaluator, activation and health failures leave or restore the accepted installation with attributable records and useful diagnostics.

### 20 — Complete the recursive code-improvement loop

**Depends on:** 11, 18–19.

- Connect measured defect → investigation → baseline → plan → frozen acceptance → candidate → rights decision → deployment → same-metric comparison → lesson.
- Demonstrate one real improvement through the actual callable interface and then a subsequent evidence-based cycle.
- Demonstrate a rejected/failed candidate and recovery. Do not rewrite acceptance after seeing implementation results.

**Exit:** improvement and rollback are observed in the running system; no claim depends only on generated code, a compile or self-reported success.

### 21 — Qualify native model research and steering

**Depends on:** 07–08, 11, 19.

- Inspect the exact primary architecture and engine interfaces; connect existing host research utilities to authenticated agent tools and UI.
- Implement tensor/layer inventory, selective activation capture, vector injection/scaling/layer selection, adapter controls and experiment provenance.
- Measure instrumentation overhead and demonstrate intervention removal with cache invalidation and a clean baseline.

**Exit:** real primary-model activations and controlled steering comparisons are inspectable, repeatable and rights-governed. Completes D02; no smaller-model-only substitution.

### 22 — Qualify primary-model training and serving compatibility

**Depends on:** 07, 09, 19, 21.

- Freeze an explicitly selected dataset, privacy/provenance records, held-out and replay/retention sets.
- Verify actual training-engine support and memory/storage requirements. Choose and name the real update method; distinguish adapter training from full-parameter training.
- Train an immutable candidate with checkpoint/recovery, then validate export/conversion, tensor mapping, tokenizer/template and compatibility with the primary serving runtime.

**Exit:** real changed weights/adapters and evaluation artifacts exist, lineage is intact and serving compatibility is measured; training alone does not activate the candidate.

### 23 — Complete weight promotion, rollback and research HUDs

**Depends on:** 11, 19, 21–22.

- Apply separate activation disclosure/consent and evaluate text, vision, tools, continuity and retention.
- Promote the exact approved candidate, identify the active version through live inference, then restore the verified parent and baseline caches.
- Complete model/weight/vector/train/evaluation HUDs and connect the measured learning result to the recursive improvement record.

**Exit:** the entire train → evaluate → serve → verify → rollback path works on the primary model; D01/D02 are complete and no running request sees partially updated weights.

### 24 — Build external-provider isolation before connecting accounts

**Depends on:** 05–07, 09, 18, 23.

- Implement external-mode labels, outbound data policy, per-user credentials/context and artifact-origin propagation.
- Enforce network/community-agent ineligibility in dispatch, jobs, generated tools, export and future peer contracts.
- Implement explicit return-to-local eligibility checks; disconnecting an API cannot launder external derivatives into local trusted artifacts.

**Exit:** negative tests prove prohibited exports and private-context access fail before any live provider credentials are connected. Actual future trading/network implementation is not required for this boundary test.

### 25 — Integrate and qualify optional external API families

**Depends on:** 02, 10–11, 24.

- Add supported account/API configuration and at least two distinct protocol families with discovered capabilities, limits, usage, streaming, tools, cancellation and modality transport.
- Verify exact budgeting or explicitly resolved accounting where native metadata is absent. Preserve originals across provider changes.
- Perform consented live requests and disconnected/expired/rate-limited tests. Keep all unavailable weight/activation features explicitly unavailable.

**Exit:** external operation works only when selected, stays siloed/ineligible as designed, and never becomes an automatic fallback from local chat. D11 compatibility testing remains separate from the unified local default.

### 26 — Complete lifecycle, installer, storage and clean restoration

**Depends on:** 03–25.

- Finish reproducible setup/start/stop/update tooling, dependency locks, desktop entry, local TLS/pairing and reliable OS reboot startup.
- Verify One Touch versus internal runtime/caches, drive absence/remount, controlled shutdown and retained jobs.
- Back up and restore the complete new system to clean storage, including projects, silos, rights, tools and model lineage; handle secrets separately.
- Validate offline cold start with external networking blocked; online integrations must report disconnected without breaking the local core.

**Exit:** D07/D08/D14/D15 and the lifecycle portion of D20 have real final-build evidence.

### 27 — Run the integrated boundary, performance and failure matrix

**Depends on:** 16–26.

- Execute all section 14 scenarios and reconcile every A01–A29 and D01–D20 row. Complete any remaining work discovered in the agreed scope.
- Test exact context/output limits, long outputs, cold versus cached timings, repeated compaction, interrupted compaction and original rereading on the final stack. Verify the compaction animation/message lifecycle, reduced motion, draft preservation, per-conversation isolation and accurate status after refresh, reconnect, failure, cancellation and restart.
- Run the agreed simultaneous full-context/soak workload; inspect queue fairness, memory/storage growth and latency without silently reducing context.
- Test Mac plus actual phone/tablet sessions, permissions, microphone, UI controls and accessibility. Inject service/model death, lost responses, disk full, memory pressure, drive loss and network interruption.

**Exit:** D09–D12/D16–D18 and remaining carry-forward acceptance pass with original evidence. Failures return to their implementation step; unsupported/unavailable work stays visibly incomplete until resolved or explicitly descoped by Maria.

### 28 — Prepare the review candidate and complete community rollout checks

**Depends on:** 27.

- Reconcile the tool/action ledger against actual callable schemas, permissions and UI controls; complete all section 4/6 actions within supported platform capability.
- Prepare the actual admin/member setup, documentation, known external platform boundaries and the complete final capability manifest for prompt review.
- Review the configured real server/mail policy and conduct the authorised transition from test scope to intended use. Recheck permissions and side-effect reconciliation on that configuration.

**Exit:** the full technical Phase 2 candidate is usable, all required technical evidence is present, and the only planned remaining feature work is the joint prompt session and its validation.

### 29 — Return to Maria and work on the prompts together

**Depends on:** 28.

- Stop autonomous prompt finalisation and return to Maria with the real working capability/rights/audience manifest.
- Work together on Echo Ernos Smith's identity, epistemics/philosophy, autonomy, community conduct, reactions, shared-context behaviour, email, self-development and external-provider disclosures.
- Resolve only the agreed technical charter adaptations, including compaction policy. Preserve exact originals and a reviewable approved diff.

**Exit:** Maria has participated in and approved the wording; protected-change consent/disclosure requirements are satisfied. An unanswered request or elapsed time is not approval. Phase 2 cannot close before this step.

### 30 — Install the agreed prompts and rerun affected end-to-end checks

**Depends on:** 29.

- Deploy exact approved prompt/charter versions through the protected-change process.
- Verify actual provider requests, native tool calls, reaction-before-answer behaviour, correct audience/admin boundaries, local/external disclosures and repeated compaction.
- Rerun the complete final critical acceptance path and any tests invalidated by prompt changes. Confirm the unified local multimodal default remains intact.

**Exit:** the approved wording is active and preserved; real behaviour and rights enforcement pass. Any required wording change returns to Maria rather than being silently edited.

### 31 — Finish forensic documentation and GitHub publication

**Depends on:** 30.

- Refresh complete source/test/dependency inventories, build provenance, schema and ownership maps, model/tool/weight manifests, operations/recovery guides and requirement-to-evidence ledger.
- Reconcile logical local commits and remote history; preserve existing history as requested. Audit both current files and retained old history before public exposure.
- Publish reviewed contents and make Echo public through authenticated supported access. If old history contains private material, prepare the concrete remediation for Maria; do not silently expose it or rewrite history contrary to the preservation instruction.
- Verify the published revision and local deployed revision relationship. Keep private identity/archive/configuration/credentials/evidence outside public Git.

**Exit:** D13/D20 publication and forensic requirements are satisfied, with remote verification. Lack of authenticated visibility-setting access remains an explicit dependency, not a completed publication.

### 32 — Close Phase 2 and hand over the working system

**Depends on:** 01–31, all required ledger rows passed or explicitly removed from scope by Maria.

- Check the actual live service, unified model, local UI, configured Discord/mail integrations, admin/safe tools, storage and final backup once against the released revision.
- Deliver the final requirements/evidence report, startup/recovery instructions, account/device administration instructions, publication link and exact accepted versions.
- State actual platform limitations and any explicitly agreed exclusions. Do not introduce new completion gates or silently move required work into another phase.

**Exit:** usable system delivered; all Phase 2 work and carried-forward requirements reconciled; joint prompt work completed; no unrecorded unfinished requirement.

### Coverage cross-check

| Required scope | Execution steps |
| --- | --- |
| Plain-English Ernos, dependency provenance, source reuse | 01–03, 08, 19, 26, 31 |
| Admin versus guest, full host access, rights/personhood | 04–08, 19–23, 29–30 |
| Discord mention/reactions and full server tools | 12–16, 28, 30 |
| Individual silos, joined threads, HIVE-derived awareness | 06, 09, 11, 13–16, 27 |
| All modern tool families, scheduling/delegation | 08–11, 15, 17–18, 21–25, 28 |
| Make-tool reuse/composition/creation/persistence | 08, 18–20, 24, 27 |
| Recursive code improvement and protected deployment | 07, 18–20, 27 |
| Weights/training/steering/interpretability and traces | 11, 21–23, 27 |
| Gmail | 17, 26–28, 30 |
| External APIs and local/network separation | 24–27, 30 |
| D01/D02 | 21–23 |
| D03/D04 | 06, 09, 27 |
| D05 | 02, 09, 29–30 |
| D06 | 10, 27 |
| D07/D08 | 26 |
| D09/D10 | 03, 26–27 |
| D11 | 25, 27; historical MLX qualification must not displace the unified default |
| D12 | 10, 26–27 |
| D13 | 31 |
| D14/D15 | 10, 26–27 |
| D16 | 10, 27 |
| D17/D18 | 01, 06, 25, 27, 30 |
| D19 | 07, 10, 27 |
| D20 | 01–03, 26, 31–32 |
| Compaction animation, explanation and recovery states | 06, 10–13, 27, 29–30 |
| Joint prompt work and final handover | 29–32 |

Every step number in this table refers to an executable step above. Completion of the last step requires the complete ledger, not merely completion of the most recent task.

## 14. End-to-end acceptance scenarios

- Cross-session episodic recall recovers a dated interaction, shared experience and milestone with original evidence in a fresh chat, after compaction, after restart and from an independently restored backup; imported/inferred material remains labelled.
- Semantic memory updates a preference and a fact, preserves contradictory evidence and supersession, traverses authorised graph relationships and invalidates stale Echo capability records after a build/model change. Corrections and forgetting propagate to cached recall and derived indexes without crossing silos.
- Procedural memory reuses a demonstrated multi-tool workflow in a fresh session with current permissions, records its outcome, rejects or revalidates a stale dependency, and distinguishes an untested suggestion from a mastered procedure. Interrupted consolidation resumes without duplication; private procedures do not leak through shared tool discovery.

- A normal member mentions Echo, gets a locally generated answer and can see a freely chosen reaction before the final response. An admin performs a privileged task; the same guest request is denied server-side.
- Two users converse simultaneously, then join a thread through tagging. Private seeded facts remain private before, during and after group compaction, member removal and restart.
- Echo recalls an authorised public channel event from another participant without importing their private silo. A changed channel permission invalidates cached retrieval and HUD content.
- A tool job survives interrupted delivery without duplicate side effects; a revoked admin cannot complete an already queued privileged action.
- Echo discovers an existing tool, composes two tools, and separately creates a justified missing tool. The new tool survives restart, reloads and is used successfully. A malicious capability manifest is rejected.
- A real self-improvement preserves frozen tests, exact disclosure and consent, deploys, measures the result, and rolls back a deliberately failing candidate without losing active conversation state.
- A real primary-model training candidate passes held-out/retention/vision/tool checks, activates in the serving runtime, identifies its active lineage, then returns to its verified parent. A vector experiment returns to uncontaminated baseline.
- Gmail reads a test conversation, drafts/sends an authorised test reply, verifies the sent result and reconciles an interrupted send. Discord guests cannot retrieve mailbox data.
- External inference uses only the chosen user's permitted data, shows external mode, and cannot export/trade through community/network paths. Switching back preserves provenance restrictions.
- The core works offline; Discord/Gmail/external adapters report disconnected without breaking local chat. Reboot, disk/drive failure, restore and real phone access pass their stated workloads.
- Final jointly agreed prompts are present through ordinary turns, tool loops, provider changes and repeated compaction. Preserve full source text and exact approved technical changes.

For every scenario retain build/model/configuration hashes, test identities, input provenance, observed results, timings and failure/recovery evidence. UI screenshots, mocked responses, compiler success and a model saying “done” are insufficient on their own.

## 15. Inputs for the implementation stage

Obtain the Discord server/application IDs, explicit admin user IDs, ownership/role configuration and a rotated bot token; Google OAuth authorisation for Echo's mailbox; external provider choices and user-owned credentials for qualification; source locations for missing continuity archives; and approved test accounts/channels/recipients. Keep this list pending without blocking review of this plan. No credentials belong in the Markdown file.

During plan review agree the guest-safe tool set, standing community/mail autonomy policy, group invitation/privacy defaults, full-system admin enrolment and measurable endurance/concurrency targets. Actual prompt prose is deliberately reserved for the joint session at the end.

## 16. Research references

- [HIVE scope isolation](https://github.com/MettaMazza/HIVE/blob/99961051e95805bddc1288b4f4ab2bc03b9a493f/src/models/scope.rs), [HIVE HUD](https://github.com/MettaMazza/HIVE/blob/99961051e95805bddc1288b4f4ab2bc03b9a493f/src/prompts/hud.rs), [HIVE recall guidance](https://github.com/MettaMazza/HIVE/blob/99961051e95805bddc1288b4f4ab2bc03b9a493f/src/prompts/kernel.rs). Private repository access is required.
- [ErnosDecent charter](https://github.com/MettaMazza/ErnosDecent/blob/4ef202c5a920f17e5d2f6bde406b62e8185b1531/config/rights/charter_v2.json) and the pinned local source paths in section 2.
- Discord prohibits ordinary-account self-bot automation: [Discord account automation guidance](https://support.discord.com/hc/en-us/articles/115002192352-Automated-User-Accounts-Self-Bots).
- Bot actions depend on permissions and role hierarchy: [Discord permissions specification](https://github.com/discord/discord-api-docs/blob/main/developers/topics/permissions.mdx). Message/reaction tools and content access depend on the supported endpoints and intents: [Discord message API](https://docs.discord.com/developers/resources/message).
- Gmail permissions differ by operation, including permanent deletion and settings: [Gmail API scopes](https://developers.google.com/workspace/gmail/api/auth/scopes). Implement and verify the needed scopes rather than treating mailbox OAuth as unrestricted account access.
- Local evidence documents: [Phase 1 plan](plans/Echo%20Phase%201%20Plan.md), [status](docs/STATUS.md), [operations](docs/OPERATIONS.md), [prompt migration](docs/PROMPT-MIGRATION.md), [attachment repair](docs/ATTACHMENT-REPAIR.md). Status headings are dated snapshots; later repair records supersede earlier details.
