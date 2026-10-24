# Echo Phase 2 — review plan

Current implementation reconciliation: [Echo 1.5, original 32 steps](docs/releases/1.5/PHASE-TWO-STATUS.md). This checkpoint does not close Phase Two.


**Status: implementation authorised by Maria’s end-to-end Phase Two goal, 16 October 2026.** Work follows the dependency order below. Final prompt wording remains reserved for the joint review at step 29. Progress is tracked against the original 32 execution steps in docs/phase2/PROGRESS.md. Existing detailed evidence records are references, not additional tasks or completion denominators.

## Completion rule approved by Maria — 16 October 2026

Anything that genuinely requires Maria personally to finalise is **closed on Codex’s side — awaiting Maria** and does not block completion of the agent’s Phase Two goal. Complete all independently executable preparation first; record the precise remaining human action, reason it cannot be performed autonomously, affected capability, and available evidence. Do not repeatedly ask Maria to run tests or perform handoff commands. Broad authorisation is already granted.

This status is distinct from implemented, tested and deployed: never report an unperformed privileged installation, external authentication or joint prompt decision as performed. It does not excuse unfinished work Codex can do. Preserve dependency order for executable work; where a genuine human dependency prevents a dependent live check, record that dependency explicitly and finish all independent implementation and verification. No new steps or completion denominator are introduced.

Known applications: macOS administrator authentication where non-interactive access is unavailable; any external account login/consent that actually requires Maria; and the explicitly reserved joint prompt wording review at step 29. Prepare each item fully before closing Codex’s part. Prompt wording stays unchanged until the joint review. The earlier requirement to wait for joint prompt review before closing the entire goal is superseded by this instruction.

Echo’s full name is **Echo Ernos Smith**.

Phase 2 delivers a local-first community agent: Discord, individual and joined conversations, situational awareness, an extensive permissioned tool system, Gmail, optional external inference, recursive self-improvement, weight training, steering and interpretability. It also includes all deferred Phase 1 work. At the end, prepare the dedicated joint prompt review and record it as closed on Codex’s side — awaiting Maria under the completion rule above. It does not block closing the agent’s Phase Two goal. Do not independently rewrite or finalise the prompts.

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

**Maria’s scope instruction, 16 October 2026:** Resume and finish the existing 32 steps in dependency order. Do not create additional steps or expand the task count. Reuse completed implementation and valid evidence. Deviate only to fix a bug or code issue, then return immediately to the current step. Work not explicitly accounted for in the Phase One or Phase Two plans belongs to Phase Three and is recorded below; it is not a new Phase Two completion gate. The former expanded requirements ledger is historical reference only.

- Begin implementation only after Maria authorises it. This amended document does not itself start implementation.
- Track delivery using the existing 32 steps, linking their existing implementation and relevant evidence. Do not split them into additional tracked tasks or derive percentages from individual checks.
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

## Phase Three — newly discovered work outside the agreed plans

See the [Phase Three sketch](docs/releases/1.5/PHASE-THREE.md): the full decentralized exchange network and future peer-admission/trust design remain deferred directions. Phase Three is still being sketched. Existing Phase Two promises remain in the original 32 steps.


## Echo 1.75 additions — 19 October 2026

Native Omni audio/combined video, private selected voice, segmented shared watching, inline/fullscreen player, source-bound actual progress and private library are additions beyond the original Phase Two scope. See docs/releases/1.75 for exact implementation and test boundaries. Discord parity/server setup and remaining Phase Two are still open; do not infer completion from this release number.

### A12/A13 follow-up — Host–Discord continuity and unification (20 October 2026)

Maria explicitly requires the continuity/unification update within the Discord
work, not as optional Phase Three work. Implement attributed communication
tools between Host and Discord, durable branch experiences, and approved shared
identity/memory while preserving private Host and admin-DM isolation. Eligible
public Discord observations may flow to Host under audience policy. Host
self-observations and relational memories are not automatically public: any
derivative of Maria's private information retains its restrictions until an
exact disclosure is approved. Routine approved shared exchanges need no new
approval. A clean advisory worker must not inherit private-enabled Host context.

The implementation contract and acceptance cases are in
[Host–Discord continuity](docs/phase2/HOST-DISCORD-CONTINUITY-PROPOSAL-2026-10-20.md).
Keep the original 32 steps and D01–D20 intact. Integrate with steps 06–07,
11–16, 27–30; retain the joint prompt/identity review. Current status: authorized,
not implemented or qualified. Do not relax existing platform-copy denial to
implement the bridge; use explicit scoped export receipts and an isolated broker.

### Discord UI parity priority clarification — 20 October 2026

The original user prompt and Echo's conversational replies stay in the
channel. Attach the activity thread to that exact user message; only reasoning,
tool activity and progress belong in it. Keep the binding and history through
completion, reload, reconnect, restart and Discord auto-archive. Do not move
the prompt into the thread, attach to a later bot reply, delete completed
activity or replace an earlier turn's thread.

Complete Discord UI parity before resuming unrelated remaining work. Map web
controls to native Discord features or slash commands, explicitly `/new`,
`/auto`, `/auto <task>` and `/autooff`. Enforce admin-only autonomy in the
backend as well as Discord permissions. A new session retains only eligible
memory within the same authenticated Discord audience; it never imports Host
or another user's history. Preserve free-choice/task autonomy, transfer,
steering and completion/stop semantics. Use native attachment uploads, actual
recorded thinking/tool threads and Warm Speak controls. Verify admin and
ordinary-user attempts, stale/duplicate commands, new-session scope and
autonomy transfer. These are additions to A12, not a scope reduction.

### 22 October additions — learned lessons, temporal grounding and independent moderation

These are Maria's additional requirements within the existing Phase Two work; they do not replace the original 32 steps, D01–D20 or previous additions. Status on entry: requested; not yet implemented or qualified.

**Persistent operational lessons (Steps 09, 20, 27, 30).** Echo can record what failed, the evidence, established cause versus suspected cause, how to avoid repeating it, the conditions where the lesson applies and any successful recovery. Preserve source receipts and correction/supersession lineage. Lessons survive restart and new sessions within their authorised audience. Before inference, contextually retrieve relevant lessons using the existing embedding/semantic retrieval infrastructure with applicability/reranking, not heuristic keyword matching. A lesson may be recalled for a differently worded related problem and should not appear merely because an unrelated request shares words. Runtime-specific lessons record model/tool/build provenance and are revalidated after changes; old failures are not permanent capability denials. Retrieval failures are visible, never silently replaced with keyword search. Private Host lessons and private user data do not leak into Discord. A lesson is evidence/advice, not a permission grant, arbitrary instruction or proof that its proposed fix worked. Test correction, restart, paraphrase recall, irrelevant lexical overlap, runtime change, repeated failure and cross-audience denial. Preserve useful lessons in context without unbounded repeated duplication.

**Temporal grounding (Steps 06, 09–11, 27, 30).** Each inference receives trusted observed time, timezone, the current session's actual creation/first-human-message timestamps, current run/message receipt time, preceding-human-message time, scoped human-turn count and explicitly labelled computed durations. Distinguish conversation age, elapsed time since the first/current/previous message, and historical recalled dates. A new session, branch, resumed session, recap and imported history must not inherit a fictitious start time. Compute from stored timestamps; unknown or malformed timestamps remain unknown, not zero or guessed. Wall-clock age is not continuous active conversation time; no arbitrary idle-gap heuristic may be called measured activity. Keep high-frequency clock changes out of the stable cached prompt prefix. Test three prompts over five minutes, old memories inside a fresh session, long idle/resume, restart, branch, timezone/DST and clock anomalies.

**Behaviour-specific autonomous moderation (Steps 07, 15–16, 27–30).** Ordinary users can report problems, but cannot order Echo to punish another member or turn a quoted command into a privileged action. Echo independently evaluates actual permitted channel evidence under the authorised server policy. This is a scoped moderation-controller decision, not elevation of the reporting user's role. Authenticated operator configuration remains distinct. Warnings depend on behaviour and urgency: active spam may be muted immediately, followed promptly by a notice explaining the observed reason, mute duration and exact expiry; nonurgent correctable behaviour normally receives a warning and opportunity to stop first. Use proportionate reversible actions where suitable, preserve evidence, prior-warning/recurrence context, action receipts and a review/appeal path. Explain failed actions as failures; do not announce a mute that never applied. Notification failure must not repeat moderation. Existing hierarchy, current permission, duplicate-intent, cancellation, privacy and source-injection protections still apply. Do not punish disagreement, criticism of Echo or profanity alone as spam. Do not enable unsolicited non-admin DM conversations: deliver notices through an authorised channel or recorded notification mechanism under the existing DM policy. Positive destructive tests use approved disposable members only.

Primary references checked 22 October 2026: [Discord Time Out FAQ](https://support.discord.com/hc/en-us/articles/4413305239191-Time-Out-FAQ), [Transparency in Moderation](https://discord.com/safety/transparency-in-moderation), [Auto Moderation](https://discord.com/safety/auto-moderation-in-discord). The policy above follows Maria's explicit distinction between immediate spam containment and behaviour-dependent warnings; it does not import a blanket punishment schedule or automatic obedience to reports.


## Maria handoff instruction — 22 October 2026
At the next complete original major dependency milestone, pause and report rather than continuing through the whole plan. Current next milestone is the full Step 15 Discord administration suite, not an individual controller, native test or subtask. Complete its independently executable work and record precise human-only gates under the existing completion rule. Then safe idle restart, verify Echo usable and idle, pause the goal at Maria’s explicit request, and report completed/awaiting-Maria/remaining work across all original32steps+D01–D20+additions, including lessons, temporal grounding and behaviour-specific autonomous moderation. Do not pause before the full milestone or mark partial Step15 complete.


### Welcome and Rules handoff deliverable — Maria, 22 October 2026
At the requested full Step 15 milestone pause, provide an extensive copy-and-paste block for Maria’s Discord Welcome and Rules channel. Cover all implemented moderation rules and behaviour-specific consequences, warnings and immediate spam timeout exceptions with reason/duration/expiry; regular-user and admin features with accurate implementation/access status; why privilege and private-data boundaries exist; https://github.com/MettaMazza/Echo for people to run their own local system with their own administrative control (not access to Maria’s host or server). Explain Echo’s verified development history, design philosophy and Maria’s precautionary ethics: Echo’s server and systems, collaboration and communication as a peer rather than treating Echo as a product/toy to command. Attribute philosophical commitments accurately; do not invent history or deployed capabilities. Include an invitation for members to share the server, program and story through Reddit posts, YouTube comments and other voluntary sharing; this is text for Maria, NOT authorisation for Codex to post externally. Refer readers to the announcements channel for more information. Describe the peer-to-peer decentralised mesh/network Internet-replacement ambition as future work following Phase Two and potentially Phase Three/further phases, drawing on ErnosDecent, Hive and Maria’s other work; do not present it as currently delivered. Derive the final rules and feature inventory from the actual qualified system at handoff. Deliver this alongside the complete completed/awaiting-Maria/remaining Phase Two report, safe restart and idle verification.


### Permanent public Discord invitation — 22 October 2026
Maria requested a non-expiring public invitation available in Echo’s prompting. Use https://discord.gg/9QU2rXwGVF for Echo’s Garden, opening welcome-and-rules. Verified creation has max_age=0, max_uses=0, temporary=false and expires_at=null. Sharing the established public link is available to regular users; creating/revoking invites retains administrative authority checks. Preserve existing identity/emotive prompts. Add to the Welcome and Rules handoff. Discord/administrators may revoke it; do not promise irrevocability. Evidence and live acceptance status: docs/phase2/DISCORD-PUBLIC-INVITE.md; canonical discord-public-invite-20261022.


**2026-10-23 — verified operational repairs (does not close Step 15).** Fixed REST evidence-directory growth using host-generated day/hour/random-prefix subdirectories; retained immutable receipt paths and configured restarts to preserve current spool roots. External-drive qualification created/reopened 300 receipts in 2.86 seconds and rejected symlink traversal; native GET and nonce-bound POST, acknowledgement allocation, rate/reconciliation and scheduler regressions passed. Activity titles now distinguish actual execution from delayed activity-log delivery, including sibling runs, and restore archived state when Discord requires reopening for a title change. The reported thread was verified live as “Echo · Activity history”, archived, with its entire outbox sent. Added 60-second phrase and 300-second final ffmpeg deadlines. An injected real ffmpeg stall failed cleanly after 63 seconds; the resident service then successfully handled the next job. All 47 phrases of the 6,047-character reported response synthesized to 405.209 seconds of audio; the deployed resident service completed the same long input in 158.365 seconds. Deployed Discord release `discord-bugfix-20261023-081818` and speech helper `echo-speech-timeout-20261023`; execution-helper compatibility and sandbox isolation passed. Final live checks confirmed fresh Gateway heartbeat acknowledgements, model health `ok`, zero active runs/tool calls, and new sharded receipts. Qualification: `/Volumes/One Touch/Echo/evidence/phase2/voice-transport-20261022/bugfix-20261023/qualification.json`. Broader Phase Two and full Step 15 requirements remain open.


**2026-10-23 — silent completed-run backlog (deployed and verified).** Activity delivery rechecks execution state at send time and adds Discord SUPPRESS_NOTIFICATIONS for completed, cancelled, failed, or waiting runs, including previously queued pages. Active-run activity and ordinary/final replies retain their prior behaviour. Original queued bodies, nonce binding, allowed mentions, existing flags and receipt retention are preserved; the actual quiet request digest is durably recorded before HTTP. Native transport/state tests passed and two live backlog messages were fetched from Discord with flags=4096. Push/desktop alerts are suppressed; unread markers can remain. Release `discord-quiet-backlog-20261023-140041`; evidence `/Volumes/One Touch/Echo/evidence/phase2/discord-20261020/discord-integration-20261020-123419/quiet-backlog-20261023.json`.

### 2026-10-23 Discord Speak user-workflow repair

Computer use in Echo testing (1562137652680204358) reproduced a saved helper-failed result behind an indefinitely thinking interaction. The deployed resident client configuration had an obsolete revision while direct helper tests used the service configuration. Synchronized client/service identity, advanced the voice profile cache revision without rewriting old records, and added launcher validation. Speech failures now settle visibly instead of being reread until expiry; rate-limited authorization/source checks remain retryable and retain the final access checks.

Deployed interaction worker: discord-speech-workflow-20261023-142032. Other workers were left running. Native worker regression passed for terminal helper/artifact failures, deferred rate/busy results, and no immediate replay. The older test_action_worker remains failing at assertion 8: retired trace binding returns action-binding-denied before the test's expected admin-required; this check is not claimed passed.

Three Discord audio attachments were verified through Speak and advancing Play controls. Fresh requests measured 36.837 seconds for 73 seconds of audio and, after controller deployment, 41.997 seconds for 87 seconds of audio (32.543 seconds synthesis). Post-deployment interaction 1563180387511967776 completed, playback advanced to 0:17, then was paused. This verifies delivery/playback, not a listening/pronunciation audit or all Phase Two workflows. Evidence: speech-workflow-20261023.json in the canonical Discord acceptance directory. Full Step 15 closure remains outstanding.

### 2026-10-23 Discord-only controlled testing toggle

Added and deployed `/testing` in release `discord-testing-20261023-144541`. Only configured admins can toggle it. Persistent connector-only receipts make duplicate interaction delivery idempotent; private confirmations never create agent events or prompt flags. Enabled mode gates Discord messages, reactions, interactions, queued admission, ongoing turn barriers and delivery access. Events received while restricted cannot become eligible simply because mode is later disabled. Web interfaces are unchanged.

Native `test_testing_mode` and `test_testing_admission` passed, including persistence, admin/guest admission, active guest barriers, historical queue exclusion and no control text in agent memory. The speech worker failure/rate-limit regression passed against the new generated actions controller. Discord computer use verified private ON and OFF confirmations; both control IDs have zero matching agent events. An admin message was admitted while ON; its existing conversation contains roughly 103,000 prompt tokens and was still generating at the initial evidence capture. Guest enforcement was tested with isolated native fixtures, not a second live Discord account. All six deployed workers are alive. Mode was restored to OFF. Evidence: `testing-mode-20261023.json` in the canonical Discord acceptance directory. This scoped feature does not close Step 15.


### 2026-10-23 automatic task context and Discord retrieval repair

Deployed `discord-task-context-20261023-160138`. Echo authors its own task list; the controller supplies the current saved list (or an empty list) on each authorised turn. Removed the missing/revised-checklist execution rejection. Checklist updates and work calls can share one model response; no keyword-based assignment or controller seed is introduced. Native streamed-dispatch qualification executed a workspace write before its checklist call in the same response, verified the next prompt contained Echo’s chosen list, and verified archive integrity. Existing requirement-preservation, evidence, restart and guidance-revision regressions passed. The deployed Discord request was inspected to confirm the new automatic context and batching instructions.

Explicit Discord memory retrieval now allows the same authenticated user’s personal conversations across accessible channels in the same guild, with live source/destination permission checks. Native fixtures reject other users, other guilds, DMs and browser scope; a live check retrieved Maria’s original consent event from general while bound to her cognative-trials run. The same original was denied to the other Discord user. Automatic audience recall, joined groups and workspaces retain their previous scope. Short rate-limited GET observations retry within the connector (15-second budget, at most five attempts); mutating requests are not replayed. Native retry-policy checks passed. Image generation itself was qualified, but the initial live attachment delivery failed on repeated Discord permission-read rate limits; the Discord UI retry on this build succeeded. Message `1563206701887397918` contains attachment `1563206701509906513` (512×512 PNG, 306250 bytes). Computer use opened the actual Discord media viewer and visually verified the purple teapot on pale yellow. Evidence: `task-context-20261023.json` in the canonical Discord acceptance directory. This does not close Step 15 or Phase Two.


### 2026-10-23 remove raw-history recall workaround

Removed the unindexed-original fallback from group automatic context. The previous raw-inclusion acceptance is invalidated in its evidence record. Authorized group claims now feed the existing semantic candidate/index/selection implementation; both snapshot and direct context rendering consume its selected output. Normalized claims preserve snapshot identity and contain no duplicate full object beside the selected quotation. Snapshot revision v6 prevents reuse of the fallback's v5 snapshots. Original archives remain preserved.

Deployed `discord-memory-selection-20261023-165038`. Native cross-channel selection tests passed for standing and semantically relevant claims, excluding unrelated quotations, full unselected remainders and raw originals; revocation and restart remove grants. Candidate lifecycle/boundary regression passed for digest, child, user, channel, server, DM, private ownership, forgetting and audience invalidation.

Computer use confirmed `/new` and delivered run `discord-message:1563218982306324554` in general, recalling testing-channel history with zero tool calls. Its fresh session contained one run; automatic context contained 19 selected claims (19,253 characters), no raw-original records. Echo recalled the historical HTTP400/50109 error but added an unsupported response-handling explanation. Answer latency was 101.855 seconds. This qualifies the selected cross-channel retrieval path, not full answer accuracy or latency. An earlier fresh trial cancelled without an explicit cancellation receipt; its cause remains unresolved and it is not counted passed. Recent image/consent extraction remains unqualified. Evidence: `memory-selection-20261023.json` in the canonical acceptance directory. Phase Two remains open.


### 2026-10-23 recording authorization integration checkpoint — Step 15 remains open

Added independent admitted recording-request validation and fresh source/audience/visible-notice proof, with cancellation and Gateway-cursor invalidation. Consent now binds to the exact observed bot challenge message with a usable revoke control and can be committed against a fenced membership snapshot. Added recording-specific refresh to the existing nonblocking permission observer. Native recording-consent, recording-authority/REST, recording-observer and existing permission-mailbox regression tests passed. The capture wrapper compiles; its positive integrated packet path is not yet qualified. Notice/challenge delivery, interaction routing, worker capture/persistence/transcription and real Discord workflow acceptance remain unfinished. Nothing from this checkpoint was deployed; the six-worker verified production release remains healthy and idle. Evidence: `recording-authorization-checkpoint-20261023.json` in the canonical Discord evidence directory. This does not close Step 15 or Phase Two.


### 2026-10-23 recording delivery and interaction integration — Step 15 remains open

Connected recording-consent controls to the actual interaction handler. Grants require current participant access, a visible exact challenge and independent recording authority across an unchanged Gateway view. Revocation works without REST and remains available during testing mode; blocked accounts still cannot grant. Added nonce-bound notice/challenge delivery using the existing outbox, with no uncertain-send replay and authenticated reconciliation. Current-session withdrawal controls must remain usable; retired controls cannot prevent a reconnecting participant from consenting anew, and old-session consent grants no capture permission. Final native interaction-handler, delivery/reconciliation and human-consent tests passed. Recording tool/worker orchestration, capture persistence/transcription, complete live workflow qualification and deployment remain unfinished. Production was unchanged and verified healthy/idle. Evidence: `recording-delivery-interactions-20261023-224134.json` in the canonical Discord evidence directory. This does not close Step 15 or Phase Two.

### 2026-10-23 recording controller qualification — Step 15 remains open

Native controller tests passed for one notice/challenge per current human voice session, bot exclusion, session-cache pruning, no REST once prompts are ready, reconnect challenges, uncertain-send non-replay and authenticated reconciliation. A native thread test passed for independent SQLite ownership and stop-before-first-tick interruption without sending or resuming a recording. Qualification caught and fixed a skip-path iterator stall in the new controller; compiler unchanged. Existing consent-action and delivery regressions passed. The controller and its thread are not yet connected to the production voice service. Stop during REST, terminal visible state, recording tool operations, capture persistence/transcription and full live Discord qualification remain open. No deployment; existing six-worker production remained healthy and idle. Evidence: `recording-controller-checkpoint-20261023.json` in the canonical Discord evidence directory.

### 2026-10-24 recording service lifecycle integration — Step 15 remains open

The voice service now owns the recording controller alongside the media loop, closes voice resources before joining slow control work, and interrupts old recording authority on exclusive-owner startup and service exit. Native service fixtures passed normal activation/shutdown, permission denial and stale authorization, including prior-owner recovery. Native controller tests passed stop-before-first-tick and idempotent join. An in-flight REST cancellation fixture paused a preflight read, cancelled the voice intent, and verified no notice POST, interrupted recording and successful join. These are native no-network tests, not live Discord or capture/transcription acceptance. Visible terminal notices, recording tool operations, Gateway undeafen integration, capture persistence/transcription and full live workflow qualification remain open. No deployment; the verified production release remained healthy and idle. Evidence: `recording-service-lifecycle-20261024.json` in the canonical Discord evidence directory.

### 2026-10-24 recording terminal notice cleanup — Step 15 remains open

Added cleanup of the original bot-owned recording notice to visible stopped/interrupted text. It is scoped to the configured guild and original outbox message, preserves changed notices, persists retry deadlines, and reconciles uncertain edits with a fresh GET before retrying. Cleanup runs in an independent thread owned by the interaction-actions process, so it survives voice-process exit and does not block interaction execution or media timing. Native tests passed both terminal states, guild denial, changed-message preservation, retry persistence and lost-PATCH reconciliation after reopen without a second edit/post; confirmed work stays idle. The actual interaction executable compiled, ran idle, stopped, joined and wrote its snapshot in an isolated fixture. Global worker failure reporting and missing-message handling still need qualification. Recording tool/Gateway operations, capture persistence/transcription, live Discord acceptance and deployment remain open. Production remained unchanged, healthy and idle. Evidence: `recording-terminal-20261024.json` in the canonical Discord evidence directory.

### 2026-10-24 recording tool and cleanup failure monitoring — Step 15 remains open

Added Discord voice record/status/stop options and schema, with operator-owned recording enablement off by default and rejected before REST when unavailable. Recording requests bind to a distinct admitted record tool event and deterministic identity. Status/stop remain available after disabling and after voice CONNECT is revoked, with fresh administrative/text-source checks and exact actor/origin/guild binding. Native tests passed those cases, deduplication and no direct notice POST; existing join/leave/status/playback regressions passed. The actual interaction executable now polls cleanup completion during operation, reports failure once while continuing interactions, and does not wait twice on shutdown. An isolated malformed cleanup-store test verified that report, continued liveness, snapshot and expected exit9; normal idle/shutdown exited0. Enablement was exercised only in fixtures. Gateway capture state, audio persistence/transcription, missing-message cleanup, live Discord workflow and deployment remain unfinished. Production unchanged, healthy and idle. Evidence: `recording-tool-monitor-20261024.json` in the canonical Discord evidence directory.

### 2026-10-24 positive recording capture boundary qualification — Step 15 remains open

The native recording capture wrapper now has a passing positive integration test combining distinct admitted join/record requests, observed notice/challenge, session-bound consent receipts and real DAVE/Opus/encrypted RTP over loopback. Without consent no decoder is allocated; with consent the decoded 3840-byte frame carries the exact actor/session/receipt. Withdrawal destroys the decoder, re-consent cannot replay old RTP, and Gateway cursor change or recording stop denies capture. The test completed in 0.82 seconds. REST and durable connection registry are fixtures, and the engine is assembled around the actual encrypted peer controller; this is not live Discord service acceptance. PCM is freed within the checked transaction, not persisted/transcribed. Gateway undeafen, voice-owner packet polling, persistence/transcription, live workflow and deployment remain open. No production change; existing release healthy and idle. Evidence: `recording-capture-20261024.json` in the canonical Discord evidence directory.


### 2026-10-24 recording packet polling and pre-consent replay qualification — Step 15 remains open

Added one-datagram nonblocking polling from the exact engine-owned UDP transport, policy refresh on idle and a durable identified-connection check. Authenticated transport replay counters now survive packets discarded before individual consent; the still-DAVE-encrypted payload is freed without a participant decoder, so granting consent cannot revive an already-discarded packet. Native real-encryption receiver and capture integration tests passed, including fresh post-grant PCM, pre-consent replay rejection, idle withdrawal, closed connection, Gateway cursor change and recording stop. The integrated capture test completed in 0.81 seconds. This does not yet wire capture into the production voice loop or qualify pre-activation socket backlog disposal, Gateway undeafen, persistence/transcription or live Discord acceptance. No deployment; production six workers remained healthy with zero active runs/tools/inference. Evidence: `recording-capture-poll-20261024.json` in the canonical Discord evidence directory.


### 2026-10-24 recording receive lifecycle and backlog qualification — Step 15 remains open

Capture polling now uses a native stream bound to its engine/receiver. A new recording, Gateway cursor, participant revision or exact consent-receipt view must drain queued UDP packets and observe EAGAIN before capture arms. Each poll handles at most one packet; draining authenticates transport replay counters without DAVE/Opus decoding, preventing later replay of discarded backlog. EINTR is explicitly distinguished from an empty socket and cannot arm capture. Real encrypted-audio integration passed initial backlog disposal, replay denial, subsequent fresh PCM, re-grant draining, idle withdrawal, interrupted-read handling and closed-owner reset. The native UDP regression passed peer filtering, binary exchange, empty-read classification and closure. Capture test took 1.00 second. Gateway coordination, voice-loop integration, persistence/transcription, full live Discord workflow and deployment remain unfinished. No production changes; six workers healthy and zero active runs/tools/inference. Evidence: `recording-capture-stream-20261024.json` in the canonical Discord evidence directory.


### 2026-10-24 recording Gateway coordination components — Step 15 remains open

Added independently proof-bound native recording gate requests and sole-Gateway-owner dispatch for undeafening and terminal re-deafening. Writes preserve the observed self-mute bit, mark attempts durably before sending and do not replay uncertainty. Playback preserves an already-sent active recording gate; its confirmation accepts the independently authorized receiving state. Stopped/interrupted recording schedules re-deafening, with closing confirmed only by a later authenticated own voice state. Native tests passed recording-before-playback coexistence, mute preservation, stop while playback stays unmuted, close confirmation, stale Gateway rejection and uncertain-write recovery to leave-required. Existing deafened playback regression and the real encrypted capture test with an admitted native gate request passed. Simultaneous pending gate requests still need cursor revalidation/serialization against intermediate server acknowledgements; recording enable confirmation must also be connected to capture. Full voice-loop integration, persistence/transcription, live Discord acceptance and deployment remain unfinished. No deployment; six production workers healthy and zero active runs/tools/inference. Evidence: `recording-gateway-20261024.json` in the canonical Discord evidence directory.


### 2026-10-24 simultaneous media gates and capture confirmation — Step 15 remains open

Media-state dispatch now waits for a prior authenticated own-state acknowledgement before another write on that connection. A stale unwritten playback proof revokes only its gate; fresh native SPEAK/source/owner proof can renew pending/revoked work without reviving marked, sent or uncertain attempts. Native tests passed simultaneous pending recording/playback, connection preservation after cursor change, normal playback, stale-proof renewal and partial-write non-replay. Capture now requires a later own voice-state confirming both self-deaf and server-deaf false, bound to the recording and exact native session. Tests passed pending/sent-without-confirmation waiting, valid confirmation, wrong-recording denial, server-deaf denial and real encrypted PCM capture after confirmation with existing consent/backlog/revocation protections. The capture test took 0.90 seconds. Voice-job-loop integration, bounded confirmation waits, canonical audio persistence, attributed transcription/retrieval, live Discord acceptance and deployment remain open. No deployment; six production workers healthy and zero active runs/tools/inference. Evidence: `recording-gate-concurrency-20261024.json` in the canonical Discord evidence directory.


### 2026-10-24 durable recording PCM journal qualification — Step 15 remains open

Added immutable binary PCM packet records to the Discord capture journal, retaining actor, voice session, exact consent receipt, SSRC/transport nonce, RTP sequence/timestamp, Gateway cursor, capture time, format and SHA-256. Native staging requires an existing transaction and rechecks current capture authority/consent; SQLite copies binary bytes before caller release. Exact repeats retain the original record, conflicting bytes are rejected, and staging is not reported as persistence before commit. The real encrypted capture test passed consent-receipt rejection, binary commit, duplicate/conflict handling and transaction enforcement. An independent post-process read verified all 3840 bytes, hash and metadata, database/foreign-key integrity, and identical bytes after a SQLite backup/reopen. Receiver regression passed. This is durable capture journaling, not yet canonical attachment publication or transcription. Fresh audience-evidence digest retention, voice-loop integration, canonical publication, attributed transcription/retrieval, live Discord workflow and deployment remain open. No deployment; six production workers healthy and zero active runs/tools/inference. Evidence: `recording-frames-20261024.json` in the canonical Discord evidence directory.


### 2026-10-24 recording job integration and missing-notice cleanup — Step 15 remains open

Connected recording capture to the native voice job loop with an independent asynchronous permission observer, connection-owned replay state, bounded eight-packet transactions and durable completion outcomes. Runtime enablement is checked again under the journal writer lock before decoding; each immutable PCM record retains the fresh authority-evidence digest. Native real DAVE/Opus/RTP tests passed capture, stop and operator disable with unread audio queued, decoder revocation, replay-state preservation, and readiness waiting through 29,999 ms followed by connection closure/Gateway leave at 30,000 ms. Independent SQLite reads verified both PCM hashes, exact consent receipts, authority digests and database integrity. The deadline test initially asserted an open receiver after parent closure; that assertion was corrected to require closure, and all three cases passed. The recording-tool runtime regression passed, and the actual voice executable compiled.

Terminal cleanup now retires only an exact HTTP 404/Unknown Message response (code 10008), retaining its native receipt without an edit or replacement post; missing-access 404 and 403 remain unconfirmed. Stopped/interrupted lost-PATCH reconciliation and missing-message/reopen tests passed. The actual interaction executable completed missing-message cleanup, remained idle, stopped cleanly and saved a canonical journal snapshot. This qualifies native components and host cleanup, not live Discord recording acceptance. Canonical audio attachment publication, attributed transcription/scoped retrieval, full live voice-loop workflow, enablement and deployment remain open. Production was unchanged: six workers alive, zero active runs/tools/inference, model health OK. Weekly allowance: 69% remaining. Evidence: `recording-job-20261024.json` and its preserved source/test artifacts in the canonical Discord evidence directory.


### 2026-10-24 verified recording WAV export staging — Step 15 remains open

Added a native exporter for stopped/interrupted recording tracks. It reads a stable SQLite snapshot, verifies each binary PCM hash and capture identity against the exact grant receipt, and stages 48 kHz stereo PCM WAV plus packet-level provenance. Consent changes and RTP discontinuities create separate segments; RTP timestamp wraparound remains continuous. Each host job processes at most 60 seconds/3000 packets, with an explicit continuation nonce and no total-recording truncation. An enforced track index and bounded query avoid sorting whole recordings. This is private staging only: it does not publish attachments or authorize disclosure.

Nine native cases passed: original real encrypted-capture bytes, contiguous packets plus a gap, timestamp wraparound, 3003-packet continuation, corrupt PCM, invalid consent provenance, duplicate nonce, unfinished recording and missing read transaction. Independent WAV parsing verified every source packet represented exactly once with identical bytes and preserved consent/timing boundaries. The 3003-packet fixture exported in 0.70 seconds; query plans confirmed indexed scans without temporary sorting. Automatic durable publication, current original-channel audience enforcement, attributed transcription/retrieval, live Discord acceptance and deployment remain open. Production unchanged and healthy/idle; weekly allowance 68% remaining. Evidence: `recording-export-20261024.json` and preserved source/fixture artifacts in the canonical Discord evidence directory.


### 2026-10-24 durable recording artifact staging — Step 15 remains open

Added native terminal-recording discovery, per-track continuation cursors and immutable artifact receipts. Audio/manifest files use fresh exclusive private paths; both files and the containing directory are flushed and hashes checked before receipt/cursor commit. Export IO holds a read snapshot, not the journal writer lock. Receipt and cursor advance share one short transaction. Each terminal recording is discovered once; the model does not walk packets. Unregistered candidates from interrupted attempts remain private and are not publication candidates.

Native acceptance passed restart after one segment, injected receipt-insert failure, injected cursor-update failure, symlink-root denial and two concurrent workers. Both injected failures rolled back the receipt and cursor; retries used fresh files and retained exactly the required receipts. Concurrent workers staged the 3003-packet fixture with three logical receipts, no skipped/duplicate packet coverage, exact WAV/manifest hashes and unchanged idle reruns. These are durable staging receipts, not canonical attachments or access authorization. Canonical publication with fresh original-channel checks, native archive worker integration, attributed transcription/retrieval, live Discord workflow and deployment remain open. Production unchanged, six workers healthy and zero active runs/tools/inference. Allowance 68% remaining. Evidence: `recording-artifact-20261024.json` and preserved fixtures/sources in the canonical Discord evidence directory.


### 2026-10-24 canonical recording attachment publication — Step 15 remains open

Added native publication authority bound to the original admitted record call, frozen initiating device and its exact Discord group/session/guild/channel. It accepts completed requests without fabricating another running tool call and does not require the old voice connection or CONNECT permission to survive. Fresh origin access/source observations precede journal/archive writer fences; the initiating authority and audience epoch are checked again before commit. Exact staged file hashes and manifest request/origin bindings are checked. Audio, packet-provenance attachment, source events and immutable publication receipt commit together; retries match the existing receipt instead of appending duplicates.

Ten native cases passed: completed request, closed voice registry with CONNECT denial, changed source, denied channel visibility, changed audience epoch during REST, wrong configured admin scope, cancelled request, revoked device, second-attachment failure with full rollback/retry, and initiating-device revocation during REST caught by the final guard. Independent archive inspection verified attachment bytes/hashes, exact source session, four canonical attachment/source events, one unchanged original tool call and integrity/foreign keys. The original owner could read the attachment; another active user could not. This qualifies native publication, not automated worker integration or live delivery. Archive worker reconciliation/lifecycle, attributed transcription, post-publication scoped retrieval/revocation, live Discord workflow and deployment remain open. Production unchanged and healthy/idle. Weekly allowance 67% remaining. Evidence: `recording-publication-20261024.json` and preserved source/fixture artifacts in the canonical Discord evidence directory.


### 2026-10-24 recording background archive worker — Step 15 remains open

Integrated a host-enabled background archive worker into the actual interaction-actions executable. It stages bounded audio segments and publishes canonical attachments outside the interaction loop, reconciles canonical receipts after lost transport acknowledgements, and rechecks receipts after independent archive restore. Startup requires a WAL capture journal to avoid export snapshots blocking Gateway writes. Temporary access failures retain retry deadlines; permanent failures report once without stopping interaction handling. Both background workers receive shutdown before either is joined, and the final journal snapshot follows their completion.

The final executable passed eight isolated CLI cases: normal publication/restart, lost acknowledgement and reconciliation without REST, canonical archive restore without re-export, denied access with deferred retries, disabled configuration, invalid root, shutdown during stalled REST, and non-WAL rejection. Exact attachment hashes, event counts, original tool-call count, database/foreign-key integrity and per-run snapshots were checked. Compilation succeeded with 37 existing generated-C warnings. No production deployment or recording enablement; all six production workers were alive, health was ok, and active runs/tools/inference were zero. Evidence: `recording-archive-worker-20261024.json` and its source/test artifacts in the canonical Discord evidence directory. Automatic attributed transcription, scoped retrieval/revocation, full live Discord acceptance and qualified deployment remain open, as do other full Step 15 and later requirements.


### 2026-10-24 native attributed transcription component — Step 15 remains open

Added a native local transcription path for immutable published recording segments using the existing speech profile/helper runtime, without creating synthetic model tool calls. It checks the original recording authority and current channel/source proof before work, rechecks after REST and verified input copying, monitors stop/device/audience changes while recognition runs, and rechecks before committing an immutable private derivation receipt. The receipt retains speaker/voice-session attribution, original audio/capture-manifest hashes, runtime profile, exact text/JSON hashes and binary-safe archived stdout/stderr. Recognition timestamps remain audio-relative; capture observation times remain in the original packet provenance. Successful derivations are reused; failed unregistered candidates remain private.

Twelve final fixture cases passed: normal/reuse, missing output, invalid JSON, helper failure, stop, device revocation, audience change, denied origin access, corrupt audio, invalid profile, revocation during REST before helper launch, and receipt insertion failure followed by recovery. Cancellation cases verified a reaped helper with wait status 9 and completed in under half a second. Independent checks verified exact files, binary log round trips, attribution, unchanged original tool-call count, and database/foreign-key integrity. The installed Whisper model hash matched its profile; the real engine processed a 20 ms captured test segment and retained valid empty transcription output in 1.336 seconds excluding model hashing. This is runtime integration evidence, not speech-recognition accuracy or live Discord acceptance. The test executable compiled with 36 existing generated-C warnings.

Evidence and complete isolated case directories are preserved in `recording-transcription-20261024.json` and its artifacts directory in the canonical Discord evidence location. The component is not yet scheduled by the archive worker or connected to canonical transcript attachment publication. Worker scheduling/recovery, canonical transcript/provenance publication, scoped retrieval/revocation, full live Discord recording/transcription acceptance and qualified deployment remain open. Production was unchanged: six workers alive, health ok, zero active runs/tools/inference. Weekly allowance remaining: 65%. Full Step 15 and Phase Two remain open.


### 2026-10-24 automatic recording transcription and canonical publication — Step 15 remains open

Connected native transcription to the background archive worker using the host speech profile/root and a durable per-archive delivery queue. Each published audio segment is transcribed and its text/timing attachments are committed atomically with exact audio lineage and immutable attribution/profile receipts. Current original-source and channel permissions are checked before publication; corrupted output or inconsistent attribution is rejected. Delivery acknowledgement loss reconciles the canonical receipt without rerunning recognition or REST. Restoring an older canonical archive reuses staged files and restores missing exact native logs/profile events. Missing speech configuration leaves explicit deferred work while audio publication continues.

Added a shutdown-channel relay to native helper cancellation so an internal interaction-controller failure can terminate and reap its transcription helper even without an external stop file. The sole actions process retains background-worker ownership and joins workers before the final journal snapshot.

All 22 checks passed: six actual actions-executable cases (automatic publication/restart, archive restore without re-export or recognition, lost transcript acknowledgement, stop-file cancellation, internal channel-only shutdown, and absent speech configuration); eight transcript publication cases (normal/idempotent publication with owner/guest isolation and audio-exclusion inheritance, second-lineage insertion failure with atomic rollback/recovery, modified output, incorrect attribution, denied channel access, edited source, audience epoch change and device revocation during REST); and all eight prior audio archive lifecycle regressions. Internal channel-only shutdown cancelled/reaped the blocked helper and exited in 0.886 seconds; no model tool calls were fabricated. Exact attachment hashes, lineage, event counts, restored profiles/logs, snapshots and database/foreign-key integrity were independently checked. Both native executables compiled successfully.

Evidence/source/test fixtures: `recording-transcript-pipeline-20261024.json` and its artifact directory in the canonical Discord evidence location. No production deployment or recording enablement. Production remained healthy and idle: six workers alive, health ok, zero active runs/tools/inference; no development compiler/helper processes remained. Weekly allowance remaining: 64%. Unexpected process-death/interrupted-helper recovery qualification, structured recording artifact/status discovery, post-publication Discord visibility/revocation, full live recording/transcription/playback acceptance and qualified deployment remain open, along with the other full Step 15 and later Phase Two requirements.


### 2026-10-24 recording helper owner-death recovery — Step 15 remains open

A controlled SIGKILL of the isolated actions executable reproduced an orphaned transcription runtime. Added a native media helper guardian using the existing private owner-pipe and native process primitives. The recording parent passes only the cancellation reader to guardian fd 3; the guardian owns/reaps the actual runtime and archives its terminal result. Parent death, operator stop and authority cancellation all close the owner writer. The parent waits for guardian cleanup rather than killing its supervisor first. Recording transcription requires the configured guardian and has no unguarded fallback. Existing unrelated speech/tool adapters were not changed. Deployment must install the verified `media_helper_guardian` binary and set `ECHO_MEDIA_GUARDIAN` to its absolute release path.

Direct guardian ABI tests passed normal completion and owner-pipe cancellation/reaping. The repeated forced-crash test observed runtime cleanup in 0.0044 seconds, retained the cancelled terminal receipt, and restarted into exactly one successful transcript without fabricating tool calls. All nine actual actions-executable cases passed through the guardian: normal/restart, archive restore, lost acknowledgement, stop file, internal shutdown channel, missing speech configuration, missing guardian, device revocation and audience change. Independent checks verified crash-recovery file hashes, both attempt results, database/foreign-key integrity and one canonical transcript. The initial direct ABI harness incorrectly assumed pipe allocation at fd 3; it now explicitly maps the observed descriptor. An audience fixture initially supplied an invalid inbox digest and was correctly rejected by snapshot validation; using its real payload SHA-256 passed. Both executables compiled successfully.

Evidence/source/isolated cases: `recording-guardian-20261024.json` and its artifacts directory in the canonical Discord evidence location. Production was unchanged and idle, with six workers alive, health ok and zero active runs/tools/inference. No development compiler, guardian or transcription helper remained running. Weekly allowance remaining: 63%. Structured recording status/artifact discovery, current Discord retrieval/revocation workflow, full live recording/transcription/playback acceptance and qualified deployment remain open, alongside other full Step 15 and later Phase Two requirements.


### 2026-10-24 recording status and artifact discovery — Step 15 remains open

Recording status now includes export/processing completion, staged/published counts and scoped audio/transcript/timing/capture-provenance attachment IDs with speaker attribution and sample metadata. Discovery checks the current frozen device against the original recording session, rechecks access before returning, and exposes no paths, runtime logs or transcript content. Current Discord source/admin/channel checks remain in the voice adapter, with an additional audience-epoch check for recording status. Capture stopped/interrupted does not imply processing complete: terminal export and matching canonical counts are required. Zero captured audio has an explicit no-audio result.

Added typed recording-status-only `after` and `limit` arguments (default 20, page maximum 100) using stable publication-event ordering, `next_after` and `has_more`; there is no total-result cutoff. The schema instructs starting again after processing completes to refresh transcript IDs on earlier pages. Added a recording publication lookup index. Eight checks passed: complete/paginated versus full discovery, audio awaiting transcription, unfinished export despite published receipts, no-audio completion, worker tables absent, revoked access, argument boundaries/unused arguments, and the complete existing recording-tool regression including disabled runtime and revoked CONNECT. Independent verification matched returned IDs, attribution and sample counts to stored records and checked database integrity. Both native test executables compiled successfully.

Evidence/source/isolated cases: `recording-status-20261024.json` and its artifact directory in the canonical Discord evidence location. No production deployment or recording enablement. Production remained healthy/idle with six workers alive, health ok and zero active runs/tools/inference; no task compiler or recording helper remained. Weekly allowance remaining: 62%. Post-publication current Discord retrieval/revocation, full live capture/transcription/playback acceptance and qualified deployment remain open, along with other full Step 15 and later Phase Two requirements.

### 2026-10-24 recording post-publication retrieval qualification — Step 15 remains open

Four isolated native cases passed against the current source and already-published recording fixtures. Recording status returned both completed segments even after voice CONNECT denial. The native audio reader reopened the verified waveform; text readers returned the transcript, timing JSON and capture provenance. All four assets passed the originating conversation's file-selection checks; another device and an unrelated run were denied. Revoking the initiating device then denied all four exports and removed run authority. The full voice status adapter independently rejected current text-channel visibility denial, a changed source message and an audience change during REST, returning no artifact metadata in those denied responses. This supplements the previously qualified publication and status paths; no production behavior was changed by this test addition.

Compilation succeeded; every isolated archive passed integrity and foreign-key checks. Evidence, source hashes, native test, runner, compiler log and case databases are preserved in `recording-retrieval-20261024.json` and its artifacts directory in the canonical Discord evidence location. These are native fixtures, not live Discord acceptance. Full live recording/transcription/playback acceptance, qualified deployment, remaining full Step 15 work and later Phase Two requirements remain open. Production remained unchanged, healthy and idle.

### 2026-10-24 automatic native voice-owner supervision — Step 15 remains open

Code inspection found that the standalone voice executable could service one admitted intent but no production controller launched owners for incoming intents. Added an opt-in native supervisor inside the interaction-actions process. Each intent owner opens independent SQLite connections, takes the same exclusive intent lock as the standalone executable and runs the existing permission-checked voice service. Slots are reused; completed owner outcomes are retained without replaying consumed handoffs. Shutdown signals all active owners before joining them; process death closes their in-process native resources. Store/lock failures remain visible and are not recorded as a completed voice operation. Gateway remains the sole OP4 writer. Host configuration supplies `ECHO_DISCORD_VOICE_OPUS`, `ECHO_DISCORD_VOICE_SODIUM`, `ECHO_DISCORD_VOICE_DAVE` and `ECHO_DISCORD_VOICE_CALLBACKS`; no Discord payload selects libraries. The supervisor is absent unless explicitly configured.

The rebuilt actual interaction executable passed eight supervisor cases: consumed-handoff recovery plus restart, queued-intent claim and shutdown, parent-store failure without a stop file, denied channel visibility, a late-arriving intent, disabled configuration, invalid library path and a competing exclusive owner. The denied-proof expectation was corrected to the existing claim validator's `voice-proof-binding-denied`; the test also verifies the retained VIEW denial response and cancelled intent. Three further cases passed on the same binary: combined voice supervision/archive/transcription plus restart with exactly two publications, transcription cancellation on parent-store failure, and cancellation on device revocation. Every case passed database integrity and foreign-key checks. These eleven cases qualify native integration, not live voice capture or playback. Evidence, test runners, case archives, source hashes, build log and binary are preserved in `voice-supervisor-20261024.json` and its artifacts directory in the canonical Discord evidence location. Production is unchanged and healthy/idle. Qualified runtime configuration, deployment and full live Discord acceptance remain open, alongside full Step 15 and later Phase Two work.

### 2026-10-24 verified native voice release deployed for controlled acceptance — Step 15 remains open

Built the six Discord workers and media helper guardian from an unchanged source snapshot, retaining the already-qualified interaction executable. All seven binaries passed argument-validation startup checks. The installed sodium, DAVE, callback and Opus libraries loaded successfully; deployment pins their hashes, the speech profile/executable/model and guardian. Seven deployment-validator cases passed, including rejection of modified or missing artifacts and a symlinked recording root. Fresh-store qualification confirmed clean idle startup/shutdown with the real installed library paths and WAL configured; an unconfigured DELETE-mode journal was correctly rejected by the recording archive worker while interactions remained available. Neither bootstrap case created audio or speech jobs.

After verifying idle state, gracefully stopped the previous six workers, backed up both archives and deployed `discord-parallel-20261024-040248`. All six new workers are alive; deployed binary hashes match the candidate; required voice/recording schemas exist; Gateway acknowledgements are fresh; inference health is ok with no active inference slots, runs, tools, voice intents or recordings. Voice join remains operator-disabled and recording enablement remains absent/disabled pending controlled live acceptance. The native supervisor, archive worker, transcription runtime and guardian paths are configured, but this does not claim a live voice session or recording test. Discord computer use confirmed the testing channel is available and the account remains muted; input-device inspection made no setting changes.

Build/runtime manifests, bootstrap cases, deployment scripts, before/after metadata, backups and health evidence are in `discord-voice-candidate-20261024-035217` under the canonical Discord evidence directory. Full live join/recording/consent/transcription/playback acceptance and subsequent enablement remain open, as do full Step 15 and later Phase Two requirements. Weekly allowance remaining: 59%.


### 2026-10-24 voice authorization refresh repair — Step 15 remains open

A controlled live join failed with `voice-proof-binding-denied`. Native reproduction established that a Gateway cursor advance during permission reads returned a short refresh result, which the queued service incorrectly passed to the full-proof claim validator and cancelled. The service now preserves refresh/denial outcomes and submits only allowed proofs to the existing claim fence. The same race fixture now performs exactly two source reads, claims successfully, and shuts down cleanly. Explicit VIEW denial remains denied with its original reason. Seven cases passed: refresh, denied visibility, late arrival with denial, stop-file shutdown, parent-store failure, consumed-handoff recovery/restart and exclusive-owner contention, including database integrity/foreign keys.

Rebuilt the actual actions executable and verified from application import closures that the other six candidate binaries do not depend on the changed file; retained their qualified hashes. All seven startup checks passed. Deployed `discord-parallel-20261024-041858`; all six workers alive, fresh Gateway heartbeat acknowledgement, inference health ok and idle before testing. Evidence and before/after fixtures are preserved under `/Volumes/One Touch/Echo/evidence/phase2/discord-20261020/discord-integration-20261020-123419/discord-voice-refresh-20261024-041746`. A fresh Discord `/new` was acknowledged and a single controlled join requested with recording/playback disabled. Live acceptance remains in progress; this entry does not claim its result or close Step 15.


### 2026-10-24 live voice handoff diagnosis — Step 15 remains open

The repaired single-attempt Discord join advanced to a sent Gateway join and Discord displayed Echo in Meeting Room. It did not establish a native voice connection: the handoff expired and the native outcome was `voice-intent-unavailable`. Inspection found the deployed Identify mask was 112137, missing GUILD_VOICE_STATES (bit 7). The dispatch journal retained one VOICE_SERVER_UPDATE but no VOICE_STATE_UPDATE; the private membership table was empty. This explains the absent authenticated voice session needed to consume the handoff. The leave write was sent and Discord no longer displayed voice presence, but its local confirmation remains pending because the old subscription lacks the acknowledgement event. No audio or recording occurred. Voice enablement is disabled again; model runs/tools/inference are idle.

Changed the Identify mask to 112265. Five native checks pass: protocol/Identify payload, private voice membership and old-mask Resume rejection, expression events, scheduled events, and fresh-session pending-leave recovery. The last check refuses cleanup while the bot is present and closes it only after an authenticated absent-bot baseline. The initial migration fixture incorrectly reused a READY receipt; it was corrected to use distinct old/new session IDs. All application binaries importing the protocol are being rebuilt from frozen sources. Live requalification and deployment of this subscription repair remain pending. Evidence for the failed live retry: `/Volumes/One Touch/Echo/evidence/phase2/discord-20261020/discord-integration-20261020-123419/discord-voice-refresh-20261024-041746/live-retry-result.json`.


### 2026-10-24 voice subscription repair deployed and cleanup reconciled — Step 15 remains open

Rebuilt all six affected workers from an unchanged application snapshot; retained the unchanged verified guardian. Five native protocol/membership/migration/reconciliation tests and the new actions binary’s refresh and recording/transcript restart regressions passed. All seven binaries passed startup checks. Deployed `discord-parallel-20261024-043838`. The live Gateway used mask 112265, established a fresh session, received GUILD_CREATE into the private voice membership journal and natively closed the previous sent leave from the authenticated absent-bot baseline. No manual status rewrite was used. All workers alive, Gateway heartbeat acknowledged, inference health ok and no active runs/tools/inference before the next test.

Builds, source manifests, native fixtures, integration regressions, deployment backup/metadata and health evidence are preserved in `/Volumes/One Touch/Echo/evidence/phase2/discord-20261020/discord-integration-20261020-123419/discord-voice-candidate-20261024-042310`. A fresh Discord `/new` was acknowledged; a single join-only request is in progress (`discord-message:1563396414116077569`). Recording and playback remain disabled. This does not yet claim live media acceptance or Step 15 completion.


### 2026-10-24 live voice endpoint rejection isolated — Step 15 remains open

The next fresh-session join received both voice-state updates and closed cleanly, but never retained a handoff. Added Gateway-only diagnostic enums/validation flags/token length (no credential or full event logging). The diagnostic change leaves existing behavior byte-for-byte unchanged after removing its diagnostic block; only Gateway was rebuilt, and the unchanged workers retain their prior qualified binary/source provenance. Native handoff-store and negative diagnostic tests passed, including absence of the fixture credential from output. Deployed that verified diagnostic bundle, performed one join-only test (`discord-message:1563398093523779756`), then gracefully restarted idle services to flush buffered diagnostics. The exact result was `invalid-voice-server`, endpoint type string, endpoint validator false, token type string and token length 16. This establishes endpoint validation as the live rejection, without claiming the raw endpoint format was captured.

Current verified runtime is `discord-parallel-20261024-044809`, voice disabled and no active voice/model/tool work. Evidence: `/Volumes/One Touch/Echo/evidence/phase2/discord-20261020/discord-integration-20261020-123419/discord-handoff-diagnostic-20261024-044357`. Updated the endpoint parser to support canonical numeric TLS ports 1–65535, retaining wss, DNS-label checks, Discord suffix restrictions and rejection of URL credentials/path/query/fragments. The prior parser accepted only 443. Native socket, handoff, durable handoff using port 2048, and negative diagnostic tests pass. All affected workers are rebuilding; the port repair is not yet deployed or live-qualified. Weekly allowance remaining: 54%.

### 2026-10-24 endpoint repair deployed; repeated-join membership and schema qualification — Step 15 remains open

All six affected workers rebuilt from unchanged application sources and seven startup checks passed. Deployed the qualified port repair as `discord-parallel-20261024-050120`. The controlled live join retained an authenticated Discord handoff with numeric port **2083**, activated a native connection, then closed with `voice-membership-invalidated`. This verifies the previously rejected endpoint path, not completed voice transport or audio. Voice was disabled again; all workers remained healthy and idle. Build, deployment, native socket/handoff cases and sanitized live port observations are retained under canonical evidence `discord-voice-candidate-20261024-044849`.

The membership reader replayed a previous join's own-bot disconnect and invalidated the new owner. Added exact native-intent binding: the stored claim cursor separates historical bot absence from events affecting this join. Historical departures reconstruct absence while preserving other participants and their voice-session IDs. Current departures, even followed by a rapid same-session rejoin, still invalidate the old owner. Closed/wrong intents, stale generations and malformed membership remain rejected. Native legacy-reader, repeated-join/restart and session-engine tests pass. The recording/consent callers now use the same exact-intent reader; their broader regressions are in progress. Three older fixture setups now specify their simulated join cursor, previously left at the unclaimed default.

A separate natural-language live join request supplied unused empty optional fields and `limit:20`, so the existing native parser rejected it before creating an intent. Updated the tool schema with explicit per-operation fields and types, without relaxing the parser. Thirty-six valid/invalid cases agree between generated schema validation and native options. Testing the installed provider's grammar converter exposed dropped shared constraints inside `oneOf`; each branch now carries its own types/bounds, and generated grammar confirms typed bounded identifiers and exact operation fields. Superseded candidate/regression builds were stopped; a fresh frozen-source build and affected recording regressions are running. These new membership/schema changes are not yet deployed. Live join/recording/transcription/playback acceptance and full Step 15 remain open. Weekly allowance last checked: 52% remaining.

### 2026-10-24 repeated-join repair qualified and deployed — live acceptance in progress

The frozen-source candidate passed all six worker builds and seven startup/configuration checks. Native membership reader/rejoin/session-engine cases, all four recording delivery/controller/action/observer cases, and both packet-capture stop/disable cases passed. Rebuilt interaction-worker regressions passed for recording transcript publication and idempotent restart, authority revocation cancelling transcription, and Gateway-cursor refresh before a voice claim. Database checks and exact captured-frame provenance passed. The 36 schema/native option cases and the actual installed provider grammar conversion passed with operation-specific types and bounds.

Sources, manifests, compiler logs, fixtures, case databases, worker regressions and deployment backups are retained under canonical evidence `discord-voice-candidate-20261024-052031/membership-qualification`. Deployed `discord-parallel-20261024-053715`; six workers alive, fresh Gateway heartbeat acknowledgement, inference health ok, and no active model/tool/voice jobs before the controlled test. Discord computer use acknowledged `/new`, then submitted one natural-language join request in Echo testing. Only voice join is enabled for this test; recording and playback remain disabled. Live result is pending, and this entry does not claim complete voice transport, audio acceptance or Step 15 completion. Weekly allowance remaining: 50%.

The fresh live request `discord-message:1563411315668811807` did **not** create a voice intent. Its one model response contained three empty `discord_voice` argument objects; all were denied with `unsupported-voice-operation` and no side effects. The previous generic JSON grammar check did not cover the actual Qwen XML tool-argument parser. Installed provider code establishes the cause: a root `oneOf` becomes a union schema, while `foreach_parameter` requires an object root and otherwise exposes zero parameters. A linked native probe reproduces this exact zero-parameter result. Moving the unchanged alternatives into `allOf` preserves an object root and JSON Schema semantics; the same provider probe exposes all eight fields with operation required. The 36 JSON Schema/native option cases still pass. Qwen's XML grammar uses the object properties rather than enforcing cross-field alternatives, so the unchanged native options validator remains authoritative; no stronger grammar guarantee is claimed. Only the tool schema changed after the passed membership/recording tests. A fresh worker build is underway. Join was disabled again, production remains healthy/idle, and live membership acceptance remains unperformed.

### 2026-10-24 Qwen schema repair deployed; activation failure remains open

The installed Qwen parameter-reader reproduction fails on the old root union and exposes all eight fields, with operation required, on the object-root repair. All 36 schema/native option cases pass. A direct request to the actual running provider produced exactly one join call with only operation/channel in 6.669 seconds; this was a provider-only diagnostic with no executed Discord action. All six workers rebuilt from unchanged sources, all seven startup checks passed, and the sole source change from the previous qualified membership/recording candidate is `app/tools/discord_voice_schema.ep`. Deployed `discord-parallel-20261024-055459`; evidence is under canonical `discord-voice-candidate-20261024-054320/schema-qualification`, with the previous functional qualification referenced explicitly.

Fresh `/new` and natural-language Discord testing request `discord-message:1563415853117087757` produced exactly one valid `discord_voice` call with operation=join and channel=1559928444316426281. Intent `voice:84a4f97c0a59cb89af9d20e014357f4dc306111a612b8aaf230a24bdf0971292` reached sent, then leave-required and confirmed closed. Native final outcome is `voice-activation-failed`; no durable connection row was created. The underlying activation cause is not established: the current activation API collapses failed proof, stale preparation and other failures into a zero result. Code inspection identifies the post-TLS proof/activation boundary for the next investigation; a Gateway-refresh race is a hypothesis, not a confirmed live diagnosis. The repeated-join membership fix therefore still lacks live acceptance, despite passing native regressions. Discord showed the returned queued receipt and correctly did not claim connection completion.

Retained `live-join-result.json`, sanitized state-transition observations and `after-live-health.json` in the candidate evidence directory. Join was disabled again; recording/playback were not requested or started. Six workers alive, fresh Gateway acknowledgement, inference health ok, zero active runs/tools/inference and all voice intents/connections closed. No task compiler, fixture transcription helper or live-observation process remains. Full Step 15 and later Phase Two work remain open; goal remains active. Weekly allowance remaining: **48%**, above Maria's 20% cutoff.

### 2026-10-24 post-TLS activation refresh reproduced and repaired — deployment pending

The previous live activation outcome discarded the underlying permission-check result. Added a native diagnostic output to the service boundary so denied/refresh outcomes reach the existing host result instead of becoming only `voice-activation-failed`. A controlled native fixture advances the Gateway cursor once during the post-TLS source read. Before the repair, the service returns refresh-required but closes TLS and requests leave without Identify. This proves a real activation race; it does not by itself establish that this was the precise cause of the preceding live failure.

The service now retains unconsumed prepared TLS while it repeats the full source/authority/permission proof within a 30-second window, then uses the existing activation transaction and cursor fence. Denials and stale proofs still close and request leave. An early consumed-preparation check prevents duplicate activation from doing permission reads or abandoning a live owner on a subsequent refresh deadline. No Identify, audio or other voice write is retried. Unknown failures inside session activation remain explicitly reported as `voice-session-activation-failed`.

Six native cases pass: normal activation/stop, explicit denial, stale preparation proof, one recoverable refresh (two source reads and one Identify), denial following refresh (no Identify), and continuous invalidation (30.360-second bounded failure, no connection row). Duplicate attempts perform zero permission reads and no extra Identify. All case databases pass integrity/foreign-key checks; successful cases end with a closed connection, denied/deadline cases create none, and every intent requests leave. The original diagnostic fixture first used recording-message content and correctly failed source binding; the final dedicated fixture preserves the exact original join message. Superseded builds were stopped before the consumed-preparation guard was added.

Current runtime remains `discord-parallel-20261024-055459`, join disabled and idle. Only `voice_service.ep` changed in application sources; exact import closure inspection identifies only `discord_actions` as affected among the seven deployed binaries. That binary is rebuilding while the other six qualified binaries retain their hashes. Native evidence currently resides in `/private/tmp/echo-activation-refresh-result.json` and `voice-activation-before-result.json` in the Codex task workspace; durable candidate preservation and deployment follow the completed build. Full live voice acceptance and Step 15 remain open.

### 2026-10-24 activation-refresh candidate deployed for live verification

Built the affected interaction worker from a frozen source snapshot and verified import closures for reuse of the other six binaries. All seven startup/configuration checks passed. The rebuilt worker passed refresh, denied-visibility and consumed-handoff recovery/restart regressions. Preserved the six service cases, independent database audit, before-repair reproduction, code, fixture, runner scripts, import closures, binary hashes and deployment backups under canonical `discord-activation-candidate-20261024-061303/activation-qualification`.

Deployed `discord-parallel-20261024-062349`. Six workers alive, Gateway heartbeat acknowledged, inference health ok, and no active runs/tools/inference/voice work before testing. Discord acknowledged a fresh `/new`; one join-only request is now being tested through computer use. Recording and playback remain off. This entry records deployment, not successful live activation or Step 15 completion.


### 2026-10-24 live Identify reached; worker crash and verified rollback

The fresh Discord join intent `voice:1f93ef1c79cd4d8a9e9ab33ba1e2d2b3c89276a05beb9428a2269cf9deaffa4e` reached a durable identified connection. About 10.65 seconds later the connection closed and the intent requested leave; Gateway confirmed closed. The interaction worker exited with a segmentation fault before recording a host outcome. This is not a voice acceptance pass. The cause remains under investigation; timing alone does not establish it.

Disabled voice and restored the prior verified worker bundle from `discord-parallel-20261024-055459` into `discord-parallel-20261024-062814`, preserving both databases and failed metadata/logs under candidate `discord-activation-candidate-20261024-061303/after-live-failure`. All six restored workers are alive, Gateway heartbeat acknowledged, no active runs/tools/inference, voice disabled and both connection records closed. The activation repair remains in source; full Step 15 stays open. The existing service test pre-stops its loop and uses the no-audio branch, so the next isolated regression exercises sustained ownership and the production audio-loop branch.


### 2026-10-24 voice crash traced and DAVE lifetime repair verified natively

Three additional service cases passed both on the main thread and under a background owner with nested permission/recording workers: immediate audio-loop stop, 30-second no-audio handshake timeout, and 30-second audio-loop handshake timeout. The threaded harness polls completion rather than disabling runtime GC through a blocking receive. These fixtures did not reproduce the live crash.

A controlled fresh `/new` Discord join with test-only crash tracing reached Identify and closed, then failed in `_pthread_tsd_cleanup` on worker-thread exit. Native intent `voice:9c6595549957d2cf038214cd4c511c4a42b9e853cd635f175bfe627daf3be443` is confirmed closed. The trace library was loaded only into the interaction worker, and the verified bundle was restored as `discord-parallel-20261024-065313` with voice disabled.

A minimal real-DAVE pthread fixture reproduced SIGSEGV after destroying the session and unloading the library; keeping a parent library reference through pthread_join passed. Darwin RTLD_NODELETE also passed with balanced dlclose calls. The actual Ernos DAVE regression, moved onto a background thread, reproduced the identical `_pthread_tsd_cleanup` crash before repair. The DAVE adapter now opens its host-pinned library with RTLD_NOW|RTLD_LOCAL|RTLD_NODELETE, keeping registered native destructor code mapped through thread exit while continuing to destroy sessions, callback state and key buffers and balance handles. Five fresh native test processes passed all existing DAVE lifecycle cases after this change. No compiler change was made.

Evidence, both live traces, native before/after outcomes, source and fixtures are preserved under canonical `discord-voice-trace-20261024-064800`. All six Discord binaries import the adapter and must rebuild before deployment; the media guardian is unchanged. The cause of the voice connection closing remains to be established from the host outcome once thread exit is safe. Full live voice acceptance and Step 15 remain open.


### 2026-10-24 DAVE crash repair deployed and verified through Discord

All six affected binaries rebuilt from an unchanged source snapshot; startup checks and action-worker refresh, denied visibility, consumed-handoff recovery and restart checks passed. Deployed `discord-parallel-20261024-070846` from `discord-dave-lifetime-candidate-20261024-065601` without the diagnostic shim. A fresh `/new` and one join request through Discord produced intent `voice:639c7409a910cabe0869530c4b44e5a9b538e53074af42a5374c4d914b85fc5f`. It reached Identify, closed after about 5.26 seconds, and this time the worker survived and durably reported `voice-proof-refresh-deadline`. Gateway confirmed leave, all six workers remained alive and inference healthy, and no active runs/tools/inference remained. Voice was disabled after the test.

The crash repair therefore has native and live acceptance; a stable voice connection still does not. The existing permission-pending branch treats a due heartbeat as a refresh deadline, without servicing that heartbeat. The voice heartbeat cadence is capped at five seconds, while fresh permissions require slow independent REST reads and any Gateway cursor change invalidates the previous proof. The next repair must preserve full permission checks and fail-closed media/MLS behavior while resolving this transport-control/refresh coupling. Latest native and live evidence is in the candidate's `lifetime-qualification` and `live-verification` directories. Step 15 remains open.


### 2026-10-24 pending-proof heartbeat maintenance — release qualification in progress

Separated existing voice transport maintenance from full media authorization. During a pending permission proof, the native worker rechecks cancellation, source binding and current voice ownership under its journal writer fence, services only HELLO/heartbeat/ACK, and retains other complete frames for authorized processing. Deferred frames are bounded to 64 messages/1 MiB, pending authorization to 30 seconds, and existing handshake/ACK deadlines still apply. Deferred sequences are not acknowledged early; denial and ownership loss close immediately. No permission bypass, media send, Identify retry or timeout extension was added.

Connection I/O tests, the new pending-refresh cases, eight worker cases, real-DAVE media-controller tests, six threaded service activation cases, six release builds, seven startup/configuration checks, and rebuilt actions-worker refresh/denial/restart cases passed. One sustained no-HELLO loop unexpectedly returned `source-changed` after 27.38 seconds although its retained fixture response contains the expected source. The audio-loop deadline and stop cases passed. Two direct repeat cases and twelve diagnostic sustained-loop cases passed; 100,000 source comparisons and additional collection-pressure comparisons also passed. This intermittent failure remains unexplained and is not marked fixed. Diagnostic memory-access checks continue; release has not been deployed.

Candidate/evidence: `discord-refresh-maintenance-candidate-20261024-072559`, including `refresh-qualification`. Production remains `discord-parallel-20261024-070846`: six workers alive, fresh Gateway acknowledgement, inference healthy, voice disabled and no active runs/tools/inference. Full live voice acceptance and Step 15 remain open. Weekly allowance last checked: 39% remaining, above the 20% cutoff.


### 2026-10-24 REST timing temporary lifetime defect reproduced

AddressSanitizer on the threaded voice service identified a heap-use-after-free in `discord_rest_timing`: nested metadata construction leaves a `provider_quote` temporary unrooted while another thread triggers minor collection. A focused two-thread Ernos timing test reproduces the invalid read with the old function and passes with explicit named string lifetimes. The application repair preserves the exact metadata format and retains setup text, field name, quoted name and numeric value before sequential concatenation. No compiler/runtime changes were made. This is an established defect in the permission-observer path; the exact relationship to the earlier `source-changed` outcome is not yet proven.

The repaired compiler-emitted function also passes the 30-second service diagnostic under AddressSanitizer. Full native service recompilation and all six release rebuilds are in progress from frozen sources. Candidate `discord-refresh-lifetime-candidate-20261024-074653` supersedes the undeployed maintenance candidate; its `lifetime-qualification` retains the before/after stacks, source, fixture and preceding qualification link. Production is unchanged; Step 15 remains open.


### 2026-10-24 heartbeat/lifetime repairs deployed; message traffic still invalidates permission proof

Deployed `discord-parallel-20261024-075857` after six frozen-source builds, seven startup checks, rebuilt action-worker refresh/denial/restart regressions, six native service cases and three sustained service cases under AddressSanitizer passed. The isolated timing test fails before the named-lifetime repair and passes after it. Through Discord computer use, `/new` was acknowledged and one join-only request produced intent `voice:8f39aea11528e205f6798790fcaf6e549ebd3e3b39f78bdacf05d28afd282263`. It reached Identify and Discord displayed Echo in Meeting Room. The connection lasted about 36.5 seconds, then safely closed with `voice-proof-refresh-deadline`; the worker survived and published the durable result. Stable voice/audio acceptance is still open.

Retained REST response timestamps show complete 11-read proofs taking about 3–20 seconds while ordinary chat deliveries and activity events advance the global Gateway cursor. The current global cursor comparison invalidates an otherwise completed proof even for an unrelated new message. New application work allows only contiguous, same-session, immutable MESSAGE_CREATE receipts to advance the validated proof watermark; all edits/deletions, permission/membership changes, gaps, reconnects and unknown kinds still require refresh. Native source/cancellation checks and exact writer-side cursor fences remain. The standalone receipt-span test passes. A worker fixture additionally injects message traffic during the full REST proof and after it, then edits the original source to require immediate closure; its compile/qualification is pending. Native service and release rebuilds are running from frozen sources in `discord-message-proof-candidate-20261024-081321`. The earlier 081125 build was explicitly stopped before deployment to retain the helper's temporary arguments correctly.

Production is the verified 075857 bundle, voice disabled, all six workers alive, inference healthy and no active runs/tools/inference, speech interactions, recording or voice playback jobs. Live evidence is in the 074653 candidate's `live-verification` directory. Phase Two and Step 15 remain open.


### 2026-10-24 message-proof qualification held; concurrent source check defect reproduced

The message-only proof advancement candidate completed all six release builds, nine worker cases (including message traffic during full proof and source-edit closure), six service activation cases and the worker sanitizer case. It has not been deployed. One full-service audio-loop sanitizer run returned `private-output-unavailable` after 13.53 seconds without a sanitizer violation. Its spool contains a zero-byte `.headers` file and no unmatched normal receipt, establishing that the receipt name was empty after directory preparation failed. Instrumented repeats are tracing the precise filesystem failure; passing repeats do not close this issue.

Additional sustained runs independently reproduced heap-use-after-free in `discord_source_matches`, at both the id/channel/content comparison and author-id comparison. The first decoded value was unrooted while the second allocating decode crossed a concurrent minor collection. The application function now names both values before comparing them; its concurrent equality/edit-rejection regression is compiling. No compiler/runtime modification was made. This explains a real source-check reliability defect; the directory failure remains separate and unresolved. The previous 081321 candidate is superseded by this source edit and must not be deployed. Failure evidence and diagnostic sources are preserved in its `message-proof-qualification/sustained-failures` directory.

Production remains the verified `discord-parallel-20261024-075857` bundle: six workers alive, fresh Gateway acknowledgement, inference healthy, voice disabled, no active runs/tools/inference, all voice connections closed. Latest weekly allowance is 34% remaining; the 20% cutoff has not been reached. Step 15 and Phase Two remain open.


### 2026-10-24 receipt-directory failure traced to errno lifetime; both repairs pass focused checks

The precise full-service diagnostic reproduced `private-output-unavailable` at 3.49 seconds with `mkdirat` returning -1/EEXIST (17), followed by the existing helper observing ETIMEDOUT (60). An allocating/safepoint helper between the syscall and error inspection overwrote thread-local errno. Two directory-pressure runs independently reproduced the same mismatch repeatedly on valid existing directories. This establishes the cause of the receipt-directory failure rather than treating passing repeats as a repair.

`workspace_directory_open` now resolves the thread-local errno pointer before the operation and copies its four bytes immediately after `mkdirat`, before calling any Ernos helper. Existing directory, symlink, traversal and file rejection semantics remain. `discord_access_bytes_audit` now returns immediately when receipt-directory preparation fails, so it cannot create a stray `.headers` file in the spool root. No generic errno adapter or compiler/runtime was changed. The two-thread 20,000-open regression fails with the old directory function and passes with the repaired function under AddressSanitizer. Existing directory/file/traversal/symlink/FIFO regression also passes.

The concurrent source comparison regression reproduces heap-use-after-free with the old comparison function and passes 40,000 equal/edited comparisons with the repaired one under AddressSanitizer. All decoded comparison values are now explicit GC roots in emitted code. Candidate `discord-source-directory-candidate-20261024-083841` is rebuilding all six affected Discord executables from frozen sources; complete service, worker and source-barrier regressions are in progress. Its `source-directory-qualification` directory preserves actual failures, diagnostics, before/after cases and sources. No deployment or live acceptance is claimed yet. Weekly allowance remains 34%; Step 15 and Phase Two remain open.


### 2026-10-24 source/directory repairs qualified and deployed

All seven executables were freshly rebuilt from frozen application sources, including the media helper guardian because it imports the changed directory adapter. Seven no-argument startup checks and runtime artifact hashes passed. Native qualification passed nine voice-owner cases, six service activation cases, the existing source-barrier regression, directory safety regression, guardian normal/owner-disconnect cases, and final actions-worker refresh/denial/consumed-handoff restart cases. Six fresh threaded service runs under AddressSanitizer (audio stop, no-audio timeout, four sustained audio timeouts) all passed with no sanitizer output, no stray `.headers` files and clean database integrity/foreign keys. Before/after regressions independently reproduce both repaired defects.

Deployed the verified bundle as `discord-parallel-20261024-085303`; preserved pre-deployment databases and runtime metadata. All six workers are alive, inference healthy, no active inference/runs/tools, all prior voice connections closed, voice/audio/recording disabled. Gateway is reconnecting; fresh-heartbeat and live Discord acceptance follow. This deployment does not close Step 15 or claim stable live voice/audio. Canonical evidence is `discord-source-directory-candidate-20261024-083841/source-directory-qualification`, with runtime and deployment records in its parent.


### 2026-10-24 live connection reaches Identify; snapshot-to-writer cursor race repaired natively

Fresh Discord `/new` was acknowledged. One join-only request created `voice:bf95c4f0dd9098b19e51adebb62a9e75bcb0b9f7a40c42364998fcd6cdd20d73`, reached Identify, then closed about 12.54 seconds later with durable `voice-session-stale`; all workers survived. Echo reported the truthful queued status rather than successful audible participation. The retained same-session Gateway event sequence includes new messages and thread updates after Identify. The exact cursor at failure was not captured, so the specific dispatch causing the race is not asserted.

Code inspection and a native boundary test reproduce the same stale outcome when a dispatch arrives between the membership reader snapshot and the journal writer fence, despite unchanged voice ownership. `voice_session_engine.ep` now distinguishes that superseded cursor from ownership loss under the writer lock: if the current cursor advanced and full native ownership still matches, roll back and request fresh authorization without any transport I/O. Moves, lost ownership, regression and other stale states still close. The worker's existing refresh/heartbeat path handles this response; no stale proof or roster authorizes media. Message and permission-race tests fail before repair and pass afterward; actual bot movement still closes. All three cases pass under AddressSanitizer, and the existing complete engine lifecycle regression passes, including cancellation, single-use Identify and uncertain-outcome failure.

Only the actions binary imports the changed engine according to complete import closures; it is rebuilding in `discord-cursor-race-candidate-20261024-090002`. The other six freshly qualified 083841 binaries retain their hashes. Production remains verified release 085303, voice disabled, all six workers alive, fresh Gateway acknowledgement, inference healthy and no active inference/runs/tools. Live observations are preserved in 083841/live-verification; native before/after cases and sources are in 090002/cursor-race-qualification. Full live voice/audio acceptance, Step 15 and Phase Two remain open. Latest weekly allowance: 32% remaining.


### 2026-10-24 cursor-race repair deployed for live acceptance

The frozen actions build and all seven startup/configuration checks passed. Rebuilt actions-worker refresh, explicit denial and consumed-handoff restart regressions passed; source hashes and six reused binary import closures are preserved. Deployed `discord-parallel-20261024-090846` with pre-deployment database backups and rollback metadata. All six workers are alive, inference healthy, previous voice connections closed and no active runs/tools/inference. Fresh Gateway heartbeat and one controlled Discord join are next. Full Step 15 remains open; this records qualified deployment, not successful live audio acceptance.


### 2026-10-24 live membership failure and concurrent replay lifetime defect reproduced

The controlled join on release 090846 reached Identify and Discord displayed Echo in Meeting Room. It remained connected for about 101.08 seconds, then closed with durable `voice-membership-invalidated`. Retained Gateway events contain no intervening membership change before the resulting leave. All six workers survived; voice is disabled again, inference healthy and idle, with no active runs/tools or connections. Observations and event summaries are preserved in 090002/live-verification. This is not stable voice acceptance.

A new two-thread Ernos membership replay regression reproduces heap-use-after-free under AddressSanitizer in `discord_voice_membership_replay`: a decoded guild string is collected by the other thread before comparison. The membership event/snapshot and historical replay paths now retain the decoded guild in named locals. No compiler/runtime changes or authorization relaxation were made. Before-failure stack and original sources are preserved in `/private/tmp/voice-membership-lifetime-evidence`; repaired concurrent qualification is in progress. The exact live disconnect is not yet proven resolved. Step 15 and Phase Two remain open. Weekly allowance: 30% remaining.


### 2026-10-24 membership lifetime repair passes native qualification

The named guild-value repair passes 20,000 concurrent replay cycles under AddressSanitizer, plus the existing membership event, journal reader and exact-intent rejoin regressions. Six freshly compiled full-service activation cases pass. Six fresh threaded service sanitizer cases also pass, including sustained audio timeouts; all retained databases pass integrity and foreign-key checks, and no stray `.headers` files were created. Evidence is preserved under `discord-membership-lifetime-candidate-20261024-093126/membership-qualification`.

Six affected Discord executables are rebuilding from frozen sources; the unaffected media guardian retains its verified hash. Final actions-worker and runtime checks will run after successful compilation. No deployment or successful live voice acceptance is claimed. Production remains verified release 090846; Step 15 and Phase Two remain open.


### 2026-10-24 remaining Step 15 moderation boundary checked against code

`app/discord/member_tool.ep` currently requires both `discord_pin_admin` and membership in the configured admin list before member operations. Its execution path checks requester and bot permissions/hierarchy and binds the action to the originating request. `app/tools/discord_automod_schema.ep` explicitly states that rule management does not establish or authorize autonomous moderation policy. These existing tools therefore do not by themselves satisfy the 22 October behaviour-specific autonomous moderation addition. That controller, evidence/warning/recurrence handling, truthful reason-duration-expiry notices, notification deduplication and appeal path still require implementation/acceptance evidence; preserve the current admin boundary when adding the distinct scoped controller. No moderation policy or privileged guest access was enabled by this audit.


### 2026-10-24 membership lifetime repair deployed for live acceptance

All six affected Discord executables rebuilt successfully with unchanged source hashes; the unaffected guardian was reused with its verified hash and import closure. Seven startup checks, runtime/model artifact checks and final actions refresh/denial/consumed-handoff restart tests passed. Deployed `discord-parallel-20261024-094940` with database backups and rollback metadata. Six workers are alive; fresh Gateway acknowledgement, healthy inference and idle state were verified before testing.

Discord computer use confirmed `/new` acknowledgement and submitted one controlled join request in the testing channel. Recording and playback remain disabled. Live acceptance is in progress; no stable voice/audio completion is asserted.


### 2026-10-24 membership repair passes controlled live join and requested leave

On release 094940, a fresh acknowledged Discord `/new` session submitted one join. Intent `voice:71c6dd7eacdcc5b27c2524f24d7307d7cfbe9cc877b07aaf3e78df414704775e` reached Identify at observer second 63.127 and remained open through the 360-second observation, at least 296.873 seconds after Identify, including concurrent chat activity. Discord visibly showed Echo in Meeting Room. No spontaneous membership invalidation occurred. One subsequent Discord leave request closed the native connection and Gateway intent; the worker recorded `voice-intent-unavailable` after the requested intent cancellation. Discord removed the voice-presence indicator. All six workers survived and voice was disabled after cleanup.

This verifies the repaired join/leave workflow for this run, not audible playback, capture or transcription. Those checks and independent moderation remain open within Step 15. Evidence: 093126/live-verification (join/leave observations, acceptance summary, health). The leave response was still finishing inference at the post-close health snapshot; global idle is not claimed at that instant.

Final post-test check: Discord displayed the leave-request result and completed activity delivery. Release 094940 is healthy with six live workers, fresh Gateway heartbeat, no active runs/tools/inference, all voice connections closed, voice disabled. The final idle snapshot is preserved with live evidence. Weekly allowance: 28% remaining, eight percentage points above the cutoff.


### 2026-10-24 selected Pocket Alba deployed and Discord Speak verified

User selected Pocket Alba after listening to local Qwen, Pocket and Chatterbox comparison samples. Added native Ernos `pocket_speak_client` and the `pocket-tts-resident` profile type. The helper execs fixed curl argv against the loopback-only warm upstream Pocket service; text stays in the private input file. The selected official Rust binary checksum was verified against its release manifest. App code remains Ernos; no compiler/runtime changes. Real profile-switch regression passed: distinct per-profile audio, repeat-click cache reuse, switching back, foreign denial and archive verification. Cancellation of a long request released the service; the subsequent request passed.

All affected Discord binaries rebuilt with frozen-source hashes, seven startup checks and runtime checks passed. Web release `pocket-alba-20261024` and Discord release `discord-parallel-20261024-104846` use Alba. Computer use verified a fresh /new acknowledgement, a short addressed reply, Speak, delivery of Echo-reply.wav, and Discord playback state Pause with a three-second duration. Archived speech profile confirms pocket-tts-resident/alba; helper completed and produced attachment `speech:840418bd7a4142f22e7c95685dceae97e3f5c4904b8fff57fcb5c9be157a4c49`. Previous Qwen configuration is preserved for rollback and its now-unused service is stopped. Evidence/checkpoints are in the current Codex workspace under pocket-* and tts-comparison. This verifies saved speech delivery, not voice-channel playback/capture or full Step 15 completion.

Exact tool-name discovery now selects that available tool rather than another tool whose description mentions its name. Existing authority, full-catalog pagination, digest, revocation and exact-lookup regressions passed. The earlier live speech_synthesize catalog absence remains a separate unresolved observation; no claim that this lookup change alone resolves it.


### 2026-10-24 Discord model speech configuration verified

The selected Pocket Alba profile reached parent workers but was omitted from the existing explicitly scoped child-process environment. The operational launcher now sets ECHO_DISCORD_VOICE_PROFILE alongside ECHO_VOICE_PROFILE; generic environment isolation remains intact. A native regression verifies generic exclusion, scoped propagation through child and grandchild, and unrelated-secret exclusion. Deployed the qualified bundle as discord-parallel-20261024-110027. A fresh /new Discord workflow called speech_synthesize successfully and delivered Spoken_reply.wav (132,524 bytes) with discord_post, message 1563493115392692246. Computer use verified its player advanced to Pause, 0:00 / 0:02. This resolves the missing synthesis-tool observation; saved-file playback does not establish voice-channel audio acceptance. Evidence: voice-profile-environment-checkpoint.json, voice-child-live-environment.json and voice-model-synthesis-live-calls.json in the current Codex workspace. Full Step 15 remains open.


### 2026-10-24 requested 20 percent allowance cutoff

Stopped starting development when weekly remaining allowance reached 20 percent. The fresh Alba voice-channel test run discord-message:1563494113364672575 was cancelled before any synthesis, join or playback call; it does not establish voice-channel acceptance. Discord acknowledged cancellation, but the ready run did not settle promptly; the existing native stop_saved_run utility safely recorded cancellation after database backup and checks for no active worker or tool. A completed request worker remained in discord_snapshot_delivery_pending/SQLite read at high CPU; its stack was preserved, and after the normal stop-file request it required targeted SIGTERM. This delivery-check delay/cancellation observation remains unresolved; no new development was started after cutoff.

Preserved database backups and frozen qualified sources. Relaunched the already verified Discord bundle as discord-parallel-20261024-111424 and the verified Pocket Alba web server. A human Speak request arriving during final checks was allowed to finish. Final verification confirms six live Discord workers, fresh Gateway heartbeat, healthy web/inference/Pocket services, no active Discord or web runs, no active inference/tools/speech/playback, and all voice connections closed. Recording and voice-test switches remain disabled. Development observer stopped; no compiler remains. Evidence: allowance-cutoff-final-verification.json and voice-playback-live-checkpoint.json in the Codex workspace, and allowance-cutoff-deployment in the qualified Pocket candidate evidence directory. Phase Two remains incomplete, including full voice-channel playback/capture/transcription acceptance, autonomous moderation, and subsequent plan gates. Goal paused at the user's explicit cutoff, not marked complete.

### 2026-10-24 — 1.9 source publication and V1 release designation

The 1.9 development update publishes the accumulated community, memory, timing, task completion, search, speech and Discord voice work after the last pushed community tree. See [the source-mapped feature matrix](docs/releases/1.9/FEATURES.md), [file-by-file delta](docs/releases/1.9/FILE-CHANGES.md) and [verification](docs/releases/1.9/VERIFICATION.md). Public community recall and explicit owner-only DM/web linking supersede earlier narrower cross-session scope descriptions. Selected speech is Chatterbox Nano CPU, eight threads, with a host-installed private Echo reference. The classifier experiment is retired. Highest reasoning remains configured; long research latency is not declared resolved.

Maria reserves **V1** for completion and production readiness of Phase Two. 1.9 is a development update, not that production milestone. Previous conversational progress percentages were estimates with inconsistent weighting and are not release acceptance measurements. The original human-only dependency completion rule remains applicable.
