# Current status — 15 October 2026

This is a dated snapshot, not a permanent health assertion. Runtime observations and checks are recorded in `forensics/release-verification.json`; file identities are in `forensics/source-manifest.json`. Older plans, logs and handovers retain their original claims and must be read chronologically.

## Working foundation

Echo runs locally with a multi-turn ReAct loop, durable submissions and tool observations, streaming replies, cancellation, conversation history, branches, revisions, regeneration, export and device ownership. Local llama.cpp and MLX adapters use provider/model metadata and exact preflight counting. Identity/system architecture text is included in the assembled context; a live `system_inspect` tool reports provider health and capabilities. The application is authored in Ernos, including its browser client.

The browser works through a local HTTPS gateway on the same network. Each paired browser owns its sessions. Phone scrolling now follows accepted messages and incoming replies; scrolling away permits reading older text. Each assistant message retains its own expandable Thinking panel above the response, exposing archived model requests, emitted reasoning, tool calls and results. Speak beside the message actions generates an inline, saved Kokoro audio attachment. This is observed output, not a reconstructed private mental process.

Conversation originals remain in SQLite while reviewed, source-linked recaps reduce active context. Exact history rereading, recap inspection/rebuild, local workspace commands, text/PDF attachments, image operations, local transcription and speech synthesis have implementations and retained test evidence.

## Identity restoration

The private Echo identity has NOT been imported. ErnosDecent selects `config/personas/echo.txt` or `config/personal_identity.txt`, which are absent from the public reference checkout. The generic ErnOS persona was not substituted. The host can supply the original through `ECHO_IDENTITY_FILE`; wording is prepended intact, with technical architecture guidance in a separate block. A labelled fixture passed verbatim preservation through compaction. That does not prove that the missing real identity was installed. The complete My Brain continuity collection and SFT research records are not connected to this installation.

## Incident and recovery

On 15 October the native server exited with signal 11. The gateway returned HTTP 502 and the old browser attempted to decode the empty response as JSON. One durable accepted run was recovered and completed once. A later browser lock came from an unresolved local submission record when the user entered different text.

The browser now reports unavailable/incomplete responses explicitly, keeps the submission identity, reconciles it with the server, and preserves a newer draft. Draft text is stored per session in browser local storage. The associated browser fixture tests gateway errors, truncated JSON, the original submission identity and draft retention. The later crash investigation reproduced native memory races with AddressSanitizer. Echo now isolates HTTP requests and model workers into separate processes, and retains temporary application values explicitly. Both sanitizer and normal builds passed 1,200 overlapping authenticated reads; concurrent real chats and Kokoro speech passed. See `Crash repair 15 October 2026.txt` and the named evidence logs for the exact scope. The compiler/runtime sources were not changed for this repair.

## Retained performance and acceptance evidence

The recorded full-context text test used 260,094 input tokens and 392,796 ms initial ingestion. Changed follow-ups took 2,129 and 4,286 ms. These are direct measured workloads, not a guarantee for every integrated conversation. The user accepted a one-off import wait. A separate real two-provider test preserved corrections through three turns and a reopen. A long llama output completed; the retained long MLX output attempt ended incomplete and is not counted as a pass.

LAN endpoint tests recorded real completion, pairing, other-device denial and foreign-origin rejection. They ran from the Mac against its LAN interface; browser phone-width testing is not equivalent to an independent physical-phone test. The user subsequently supplied actual mobile UI/error screenshots. Backup/independent restoration and native intervention records apply to their stated snapshots and workloads.

## Phase Two and deferred work

Training, model weight updates, connected research interfaces, vector steering in the agent UI, projects, semantic/cross-session memory and broader restoration are Phase Two work. Host-side native activation/intervention commands exist separately; their presence does not mean a model-callable research panel exists. Microphone permission/real recording acceptance was explicitly deferred by the user. Echo has a local Ernos runtime supervisor and per-worker exit/recovery records. Reliable reboot startup and a portable installer remain unqualified. Extensive endurance, simultaneous full-context users and a full hardware/network fault matrix are not claimed passed.

## Latest speech and failure checks

Formatted-message Kokoro synthesis passed with original archive integrity, foreign-device rejection and idempotent audio reuse. The speech text filter has an Ernos regression fixture. Another 1,200 overlapping authenticated reads passed with durable diagnostics enabled. Deliberately killed request and model-worker processes produced signal records; the listener survived and interrupted generation returned to a retryable saved state. Evidence: `speech-plain-integration.log`, `speech-diagnostics-stress.log`, `speech-diagnostics-fault-checks.json`. Source indexes have been regenerated from every current application/test file.

The service owner was also fault-tested: a SIGKILL listener exit was durably recorded and a replacement listener returned HTTP 200 (`speech-diagnostics-supervisor.json`). Publication has replaced the existing private GitHub repository contents in logical commits while preserving its earlier history. Public visibility is still pending; the connector has no visibility-setting operation and the browser session is signed out.
