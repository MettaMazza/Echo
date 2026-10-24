> **1.9 documentation:** [Current update](../1.9/README.md) · [feature status](../1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# System and change audit

## Scope and method

This record covers the current first-party application and tests, configuration examples, engine patches and maintained documentation. The generated references read every source file and identify its content by SHA-256. Definition and import extraction is lexical navigation; it does not prove semantics or test coverage. Narrative findings are grounded in the named implementations and dated repair records. Upstream compiler/engine/model source is identified through the existing build documentation and patches, not represented as newly audited first-party code.

The local baseline is `da4ec08`; the previously published baseline is `591c89a`. They belong to different histories. The change manifest records both comparisons. Historical status paragraphs remain evidence of what was known then, not competing descriptions of the current release.

## Message and execution lifecycle

1. The browser submits a message under its paired device identity. `app/web/devices.ep`, `device_links.ep` and security modules resolve the owning principal and session access. A linked device shares the authorized conversation library; linking is not public access to unrelated principals.
2. Routes validate the request and persist submission/run state. `app/web/run_store.ep`, `worker.ep` and archive modules retain run identity, cancellation, errors and observations. Retry must reconcile an existing operation, not silently repeat an uncertain effect.
3. `app/context/session_messages.ep` assembles original turns, recap material and authorized recall. Provider preparation and exact preflight counting govern admission. The context is not arbitrarily shortened to make benchmarks look better.
4. `app/agent/runner.ep`, generation and run-state modules execute the model/tool loop. Tool authority is checked before dispatch. Task checklists constrain multi-step work and delegated jobs; direct read-only parent conversation lookups do not require an invented project checklist.
5. Provider streams are archived and exposed to the browser. Tool results and final messages have durable provenance. The session API is the final-delivery check; a worker being marked ready alone is insufficient.
6. Continuity extraction and indexed recall preserve the distinction between original history and derived assertions. Compaction rebases active context while retaining originals for scoped rereading.

## Subsystem map

| Source area | Responsibility and important boundary |
| --- | --- |
| `app/agent` | Durable inference/tool loop, authority dispatch, task recovery and guidance handling. Model intent does not bypass authorization. |
| `app/archive` | SQLite event bodies, digests, schema evolution, scope associations and recoverable operation state. Schema reference publishes structure only, never private rows. |
| `app/context` | History assembly, source-linked recaps, typed review findings and compaction resume. Positive review observations must not become endless rewrite requests. |
| `app/memory` | Automatic episodic/semantic/procedural records, selection, source corrections, knowledge relationships and stable recall snapshots. Derived state can be invalidated without erasing original history. |
| `app/security` | Principal/capability resolution, audience versions, queued authority and revocation. Caller-supplied role labels are not authority. |
| `app/web` and `app/ui` | Authenticated HTTP operations, submission workers, linked-device synchronization, chat controls, activity/artifact/audio rendering and user-visible recovery. |
| `app/jobs` | Delegation, task checklist, progress and parent result continuation. Child completion must return to the parent and produce a user-visible answer. |
| `app/tools` and `app/workspace` | Tool contracts/discovery, local documents/data, scoped files, admin execution and Ernos authoring. Guest workspaces and authorized host access have different boundaries. |
| `app/providers` | Local llama.cpp/MLX adaptation, request preparation, stream decoding, preflight and pending guidance. These do not establish completed remote API integration. |
| `app/media` | Local speech/image supporting workflows. Text/image default inference is unified; Kokoro and transcription remain explicit local media services. |
| `app/projects` | Persistent project/work/research organization associated with authorized ownership. |
| `app/rights` and `app/lifecycle` | Protected action policy, manifests, candidate/controller records, durable ledgers and installation/recovery foundations. Root controller activation is a distinct deployment boundary. |
| `app/research` | Native activation/vector/research primitives. Their presence is not primary-model training acceptance. |
| `app/network` and `app/transport` | Local network/transport support. Folder names must not be mistaken for a completed Discord bridge. |
| Host commands | Native administration, migration, diagnostics and lifecycle entry points. Consult each entry's arguments and imports before running it. |

## Device identity, linked conversations and browser behavior

`app/web/device_links.ep` and corresponding archive/device code implement linking and conversation merging. Linked clients can see the same authorized history and receive updates without manually refreshing. Revoked credentials must fail visibly; an authorization failure is not evidence that the underlying archive vanished. The UI distinguishes identity/access state from a transport fetch failure. Browser storage/origin and server credential state remain relevant when diagnosing apparent unlinking.

The client implements Enter to send, Shift+Enter for a newline, message queueing, guidance delivery state, sidebar dismissal and selecting a conversation. Guidance uses the existing run, not a fabricated stop-and-new-conversation workflow. Repeated delivery and clearing accepted guidance are tracked separately from ordinary queued messages. Provider interruption can require generation to resume; this is distinct from changing the model's internal activations.

Audio playback is persistent across chat updates and only one playback is active at a time. Sending a message should not reset an unrelated audio player. Attachments and generated artifacts are explicit chat records; `attach_file` is the bridge from a workspace output to a user-visible downloadable item. Per-message Thinking and agent panels persist with the relevant reply. Sources use a disclosure control rather than a long permanently expanded list.

See [device linking/audio](../../DEVICE-LINKING-AND-AUDIO.md), [queue/guidance](../../MESSAGE-QUEUE.md), [chat artifacts](../../CHAT-ARTIFACTS.md) and [reasoning retrieval](../../REASONING-RETRIEVAL.md).

## Delegation and task completion

Jobs carry their task scope and checklist. Agents should return when that checklist is complete instead of pursuing unrelated interests. Progress/context are readable while a job is active. `app/jobs/delegation_return.ep` coordinates return to a parent, including deferral while another parent conversation run owns the session. Results must trigger a parent continuation, not merely be placed in history waiting for the next human prompt.

Parent task tracking uses the same general mechanism for substantive multi-step work. A casual greeting or direct read-only history/scratchpad lookup is not forced through task creation. Execution tools distinguish captured output from command success; decoded output and final exit status matter. Generalized behavior is implemented in dispatch/checklist/execution code, not a special case for one user's finance task.

## Memory, provenance and correction precedence

The archive is canonical history. Episodic, semantic and procedural records are derived, scoped material with source links. `app/memory/corrections.ep` handles correction precedence and invalidation of derivatives. Superseded claims must not be resurrected by automatic extraction from the same older source. The raw original remains available as history, not as an unqualified current fact.

Recall selection combines indexed candidates with scope filtering. Stable private-session snapshots retain a base under the applicable revision; newly selected exact quotations become deltas at their original turn position. Changing access, visibility, correction, runtime or compaction state invalidates the relevant base and deltas. Group context is not casually reused under private-session rules. Completed quotation indexing is cached by model fingerprint and quotation; failure is not marked complete.

Scratchpads provide durable revisioned notes with explicit list/read/edit/delete behavior. List pagination and exact names are part of the contract; `end` is a pagination result, not an assumed filename. Session index and message paging provide a table of contents for original history. Memory graph results and paginated recall must be interpreted according to their separate contracts rather than assuming equal row counts.

See [stable recall repair](../../phase2/STABLE-SESSION-RECALL-2026-10-18.md), [continuity tools](../../CONTINUITY-TOOLS.md), and the correction/selection entries in the generated application and test references.

## Compaction and context performance

Typed recap review findings distinguish supported defects from positive observations or unsupported requests. This prevents repeated rewrites merely because a reviewer describes something already correct. Original source references remain available. Compaction is resumable and user-visible; an animation is communication, not proof of progress. Inspect persisted state and provider timing when diagnosing a stall.

The engine patch set addresses device time sharing, fair prefill, interactive reservation/priority, longest-prefix reuse, published prefix cache, checkpoints and recurrent batching. These are patches against the pinned engine, not generic guarantees for all providers. Application scheduling and actual inference slot scheduling are separate layers. Warm/cold prefix behavior and generated reasoning both affect time to first visible text.

The latest measured repair reduced repeated follow-up prefill from 9,641 fresh tokens/14.18 seconds to 1,315/2.28 seconds. It did not reduce the supported model context or remove original memory. See the verification record for cold-cache and test-method limitations.

## Errors, side effects and recovery

`app/feedback.ep` supplies shared recovery guidance across HTTP, worker, model and tool paths. Original error identity and effect state remain available. A useful failure tells the user what happened, whether work is saved, and how to recover; it must not convert an uncertain side effect into a safe-to-repeat claim.

Compiler errors are one subset. Execution, missing sources, permissions, interrupted model streams, cancellation and unavailable services also require clear outcomes. Ernos authoring is the default, with syntax/build feedback and explicit exceptions for capabilities requiring another runtime. Missing original records must be reported, not reconstructed from derived memory and presented as originals.

The controller ledger change uses the proper native file-creation binding and rejects unsafe ownership/link/sidecar conditions. Primitive privileged tests have recorded results; the complete protected root service is not activated merely because those tests passed.

## Attachment visibility and delayed result delivery reconciliation

Current routes expose `/api/attachment-visibility`; the client presents **Forget automatic use** and **Restore automatic use**, saves retry identity, handles revision conflicts and keeps explicit rereading available. This supersedes the older Step 10 statement that only the backend existed. Trash, automatic visibility and permanent erasure are distinct operations. Existing messages and separately shared copies are not silently removed.

The later actual two-child test in [checklist/cache evidence](../../phase2/CHECKLIST-DEPLOYMENT-CACHE-2026-10-17.md) records both child completions prompting new parent replies without another user message, despite an intervening greeting. Compiled browser functions consumed the live responses and rendered all three parent finals while preserving the draft and existing message node. This was a DOM harness, not a physical-browser claim. The historical cancelled run in the earlier delegation document is no longer the latest result.

## Data and operational boundaries

Public source excludes live conversations, model weights, credentials, private identity material, runtime configuration and local evidence payloads. Environment documentation lists names and use sites, not values. SQLite documentation is schema only. External storage holds large assets/source while the active Mac runtime and database use internal storage; consult current deployment configuration rather than copying machine-specific paths into a portable setup.

A backup is not proven restorable by its existence alone. Retained restore tests are dated evidence; final Phase Two reboot/offline/full-load/failure acceptance remains a separate plan step. Diagnostics under tests may open archives, create indexes or execute commands when explicitly enabled. They are not all harmless unit tests: inspect the generated arguments/imports and implementation before invoking them against real data.
