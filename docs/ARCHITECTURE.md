# Architecture

## A message from browser to archive and back

`app/server.ep` binds the local HTTP listener, loads the compiled browser bundle, checks configured prompt files, opens the archive and performs startup recovery. `web/routes.ep` applies host/origin checks, credential resolution and ownership checks before dispatch. `web/run_store.ep` durably records the user's submission and selected provider. A submission ID is an idempotency key: replaying the same accepted request recovers it rather than creating a second turn.

`transport/isolated_process.ep` starts each HTTP connection in a fresh native process, passing only its socket and standard output/error. The listener remains a single-threaded owner. `web/worker.ep` claims a run and starts a separate model process under the existing lock protocol. HTTP requests and model workers do not share mutable Ernos heaps. `agent/runner.ep` repeatedly reads the durable state. In `ready` it reconstructs context and performs exact provider preflight; in `tools` it dispatches the next ledger entry. Completion, cancellation, unresolved effects or recoverable errors are explicit outcomes. There is no fixed model/tool-turn count. Native workers and provider helpers have distinct lifetimes; the tokenizer helper is an isolated process, not a Python interpreter shared across web workers.

`agent/generation.ep`, `checked_generation.ep` and provider stream modules preserve requests and incoming events before accepting an assistant response. Tool calls are matched to observations with durable IDs. The browser polls `/api/run`, incremental `/api/progress`, and paginated `/api/trace`; only committed records are displayed. Streamed partial content is not a completed answer.

## Subsystems

| Directory | Responsibility | Important boundary |
| --- | --- | --- |
| `archive` | Event hash chains, source windows, attachment digests, exports, backup/restore, filesystem publication and runtime locks | Original records are retained; restoration can require execution reconciliation |
| `agent` | Run state, provider binding, generation, accepted lineage, tool ledger, cancellation/recovery and loop | A run keeps its selected profile; uncertain effects are not blindly repeated |
| `context` | Exact budget use, checkpoint reconstruction, source-linked recaps, reviews, progressive merge, repair and activation | Compaction is explicit; failures do not silently discard original history |
| `providers` | llama.cpp and MLX discovery/preflight, JSON parsing, streamed responses, helper process and model registry | Metadata/capacity is provider-derived; unsupported modalities fail explicitly |
| `transport` | Local sockets, HTTP parsing, body streaming, SSE, cancellation-aware I/O and file responses | Framing and partial reads are validated separately from model semantics |
| `web` | Authentication, ownership, sessions, submissions, worker status, uploads, trace and history APIs | Session/device checks precede private data reads and execution |
| `tools` | History, attachments, compaction, workspace and execution output contracts | Model arguments do not choose arbitrary device/session identities |
| `workspace` | Session roots, safe relative paths, exclusive file creation and sandboxed command execution | No general host filesystem or network authority |
| `media` | Locally configured image generation, transcription and speech jobs | A configured runtime is not proof of a successful operation |
| `ui` | Responsive chat, drafts, history, rendering, media controls and Thinking panel | Model/tool text is rendered as source data, not executable HTML |
| `lifecycle` | Explicit executable/cwd/argv/environment launch profiles | Launcher is not a restart supervisor or portable installer |
| `research` | Host-native activation capture, interventions, vector validation and comparisons | Host CLI research is separate from model-callable agent tools |

Root-level `app/*.ep` files are host commands and qualification entry points. Their exact signatures, imports and definitions are indexed in the complete source reference.

## Context and identity

`context/session_messages.ep` reconstructs accepted ancestry inside a read transaction. A checkpoint contributes a source-labelled recap; later originals and the current objective remain available. `context/system_awareness.ep` reads only host-configured identity/architecture files, then adds the run's selected model/adapter. Identity is outside the compacted transcript. Its text is included in token counting. Host files must be kept stable during a running checkpoint review; configuration mutation is not a versioned identity migration mechanism.

`system_inspect` uses the trusted current run and returns a timestamped provider observation. It does not expose all environment variables or credentials. Null loaded-context metadata means unreported capacity, not an unloaded model. Current tool schemas determine executable abilities.

## Compaction and recovery

`context/compact.ep` acquires a session compaction lock, captures the source endpoint, verifies attachment references and finds/resumes the job. A candidate can be produced directly or from collected source fragments and merged notes. Structural provenance checks, coverage review, synthesis and repair precede checkpoint activation. Repeated identical candidates are detected as no progress. The runtime retains explicit failure statuses for oversized or unsuccessful work; not every extreme/repeated boundary case has live qualification.

`web/recovery.ep` proves that a lock-aware worker claim has no live owner before recovery. `agent/generation_recovery.ep` accepts complete saved streams and handles incomplete work explicitly. `agent/tool_recovery.ep` reconciles observations and uncertain effects. Restored archives have a separate historical-action execution hold. This is deliberate preservation of external-effect history, not a claim that all interrupted programs can safely restart.
