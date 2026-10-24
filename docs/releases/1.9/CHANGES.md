# Changes since the last GitHub community push

Baseline and method: [release record](README.md). Every changed file has a hash and definition delta in [change-manifest.json](change-manifest.json). The feature groups below explain the user-visible behavior; they do not replace that exhaustive file ledger.

## Conversation ownership, completion and activity

`app/agent/runner.ep` records a durable `foreground_complete` handoff before joining background memory extraction. `app/discord/foreground_release.ep`, terminal claims, worker delivery and turn guards distinguish completed foreground work from a still-running extraction process. Follow-ups need not remain blocked merely because memory work is finishing. Activity status, delivery and archive changes quiet terminal traces and preserve real execution state instead of treating backlog delivery as active inference.

Task checklists support delta updates and an optional model-authored final response. `app/jobs/task_checklist.ep`, `app/agent/tool_dispatch.ep` and `run_state.ep` permit atomic completion only for a sole resolved checklist call with the required provenance and guidance/cancellation guards. This removes an otherwise redundant model pass without replacing the model’s answer. User guidance now explicitly permits revising, replacing or ending the active task.

## Time and memory

Temporal code separates archived input admission times from the fresh current clock and elapsed-session observations. Context changes preserve stable prefixes without making an old timestamp appear current.

Community retrieval and automatic memory cross public Discord sessions and speakers only after live source visibility, destination audience and imported ancestry checks. Restricted rooms and private threads are not made public by admin visibility. Explicit host-installed private links allow an authenticated private owner to read permitted DM/web archives; public/group runs cannot open that bridge. The shipped code does not hardcode the operator’s private account linkage.

Continuity tools accept boundaries, batches and text budgets, retaining cursors when a message is only partly read. Recall encoding factors repeated metadata while preserving original identifiers, Unicode text, nulls, absent fields and provenance. Context snapshots, scoped recall and permission snapshot reuse reduce repeated work; failures are not cached as successful permission observations.

## Discord controls and administration

`/testing` is connector-only admin admission state. Replay-safe toggles and receipt-time checks prevent excluded queued inputs from becoming eligible after reopening. Toggle state and rejected content do not enter agent prompts or passive context. The web UI is unaffected.

New scoped audit-log, AutoMod, onboarding and guild-setting tools include option validation, role/permission checks, schema discovery and dispatch bindings. These extend the existing channel/thread/role/member/invite/event/expression tools already present in the pushed baseline. Native protocol/allocation fixes address access, callbacks, REST timing, directory errno handling, Gateway events and source-bound controls.

## Attachments and speech

Attachment representations preserve original versus derived media provenance. Image generation returns files through the same attachment delivery path; Discord messages and web attachment routes use authorized retained-file access. Speak controls validate source ownership and origin rather than treating a legitimate reply as inaccessible solely because of a mismatched binding.

The new Ernos `app/chatterbox_speak.ep` loads the installed Chatterbox dependency with `device=cpu`, `nano=true`, eight PyTorch threads and a host-supplied reference. Resident requests stay within private job directories. `app/pocket_speak_client.ep` remains as an optional retained transport, not the selected host default. Media helper guardians and guarded provider helpers preserve subprocess timeout/cancellation ownership.

## Discord voice implementation

Voice additions include Gateway intents and membership snapshots, session ownership, heartbeat/reconnect, UDP/RTP, Opus, DAVE/MLS encryption bindings, current permission observation, playback queues and confirmation gates. Native callback state is isolated in `engine/discord_voice/voice_callbacks.c` so library callbacks do not retain pointers into moving Ernos objects; logger ownership is reference-counted.

Recording adds visible notice and participant consent, epoch-bound authority, revocation, PCM journals, durable export, canonical audio/transcript attachments, transcription queues, owner-death recovery and scoped status/retrieval. Source and fixture coverage is extensive. Full live playback/capture/transcription acceptance and deployment qualification are not inferred from component tests or successful channel membership.

## Search and latency

Search supports a configured Brave credential on its fixed HTTPS endpoint, with retained raw responses and deterministic readable result documents. The API key is not forwarded to arbitrary sites or redirects. Web fetches expose usable previews and preserve source originals. Startup prefix warmup is a cache operation, not a conversation turn; its discarded token does not cap user generation.

The configured runtime’s batch tuning and stable prompt prefixes reduce some input overhead. Checklist final delivery removes an avoidable extra pass. A response-channel instruction was installed in private architecture files to avoid drafting a complete answer in reasoning and repeating it as final output; analysis and outlining remain permitted. Highest reasoning remains configured. A reviewed final research workflow still took 345.95 seconds, so overall research latency is not declared resolved.

## Host changes that are not public source files

The host selected Chatterbox Nano CPU/8-thread Echo-reference speech, installed a Brave key in private secret storage, linked only the explicitly authorized private owner’s accounts, retired the classifier experiment, synchronized approved web/Discord identity versions, and installed response-channel guidance. Active primary inference retained highest reasoning with unlimited generation; experimental speculative decoding and KV settings were reverted. Private prompts, keys, account maps, recordings, databases and binaries are excluded. Public examples describe interfaces without exporting these assets.

## Publication and documentation

The landing page, feature matrix, operations/status/security guides and current documentation index now describe 1.9. Historical evidence stays dated and linked rather than silently rewritten as new acceptance. Source, test, tool-schema, environment-name and route indexes are regenerated. The local compiled regression executable is excluded; its test source is included. The final development update is not a production V1 release.
