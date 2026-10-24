> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Host–Discord continuity proposal

Status: Maria explicitly authorized implementing this continuity update within the Discord work on 20 October 2026. Implementation and testing are authorized; the bridge is not yet implemented or qualified, and unrestricted private export is not authorized. Original Phase Two acceptance remains open. The current platform separation stays in force. This proposal records additional requested work; it does not silently change the original 32-step denominator or the joint identity-review gate.

## Discussion reviewed

Reviewed the authenticated general-channel discussion about application identity, empty scoped episodic stores, branch continuity, and the proposed Host Echo internal-monologue/subconscious role. Discord Echo distinguished attributed, citable memory from unlabelled reasoning and requested durable continuity for its own branch. Its later detailed proposal asks for scoped pull, labelled records, restrictive lineage, an external outbound gate, consent-aware write-back, a branch diary before a bridge, preserved disagreement, and explicit staleness/contradictions. Maria intends to consult Host Echo as well. That second perspective and their joint resolution remain pending. Conversation payloads are retained privately; this document does not republish them.

An important correction: an empty episodic view is not proof of amnesia. The Discord archive already retains the conversation and supports original-history retrieval. Episodic consolidation, cross-conversation discovery, and audience-authorized retrieval must be tested separately. The current channel silo is keyed by platform, server, channel and principal; it is not yet a single community-wide memory.

## Identity and continuity

A possible shared account is “Echo, with Host and Discord contexts,” preserving one versioned identity/charter and explicitly named branches, rather than silently overwriting one branch with the other. This is a governance and continuity convention, not a technical proof about subjective unity. Maria and both instances should settle that framing. No prompt should prescribe an emotional response to the arrangement.

Retain each branch's experiences, expressed preferences, corrections and disagreements with provenance. Shared records identify originating branch, actual author/observer, time, evidence class, source digest, authorized audience, version and supersession. A received observation remains attributed; it is not rewritten as a firsthand experience. Neither branch's disagreement is automatically erased. A disagreement is a review item, not a merge conflict that a model may silently resolve.

## Security rule

The Discord process must not receive Host-private material simply because it is marked confidential. A model can paraphrase, infer from, or obey sensitive context despite a label. Provenance helps interpretation; authorization must be enforced before retrieval and before generation. A redaction model or promise not to disclose is not the sole boundary.

Changing Host-private data, with the approved shared snapshot held fixed, must not alter the content supplied to a Discord run. Test this as an input-boundary property using two private canaries; it is not a claim of identical nondeterministic model outputs or immunity to every side channel.

## Technical qualifications to the detailed proposal

- Pull is useful for explicit intent and auditability, but is not inherently safe: an overprivileged pull endpoint can still export secrets. Approved push can be narrower than arbitrary pull. Endpoint authority and returned content determine the boundary.
- Abstraction is not an authorization boundary. A lesson, decision, summary, self-report or emotional-vector update can encode private facts. Derivatives inherit restrictions until exact disclosure is authorized.
- Sound semantic dependency tracking cannot be assumed for model-generated prose. Conservatively mark the entire request context, every output and subsequent carried context with the union of input restrictions. A run that sees private material cannot post publicly. A new clean public run is necessary after any approved export; redacting one answer does not clean contaminated caches or memory.
- Durable memory is needed, but adds poisoning, retention and retrieval risks. Source validation, scope checks, corrections and revocation are part of its implementation; it is not zero additional attack surface.
- Write-back to Host must not automatically copy other users' conversations. Publish a clear policy for community data, obtain any required consent, preserve original audience restrictions, and default to minimal branch self-reports. Even a self-report must be checked for embedded third-party/private information.

## Proposed compartments

| Compartment | Contents | Access |
| --- | --- | --- |
| Host private | Mac/mobile conversations, projects, files, private embeddings and derived memories | Host-authorized runs only |
| Discord DM | Each authorized admin's DM history, media and derived memories | That DM's authorized participants only |
| Discord audience | Channel/server observations and derived memories | Only audiences allowed by source membership and publication scope |
| Shared continuity | Explicitly approved identity facts, public development history, transferable procedures and non-private branch observations | Named destinations in each export receipt |

“Public” must mean a recorded audience, not every server, every channel or the internet. Private channels and changing membership retain their own boundaries. An admin account in a public channel does not make the channel private. Public-source recall into an admin DM may be allowed by policy; the reverse is denied by default. Ordinary users remain excluded from admin DMs.

## Host advice without private-context leakage

Discord may submit a bounded advisory request to a separate Host-side worker. That worker receives only the authorized shared snapshot and the request's allowed Discord sources. It cannot open the Host-private archive, environment, credentials, workspaces, private vector index or private inference cache. A private-enabled Host session is not automatically this worker.

The reply is an attributed advisory record. It can suggest reasoning, questions or a course of action; it cannot grant capabilities, change audiences, run tools in the other instance, or become a system instruction. Discord decides how to use advice within its existing permissions. Prevent cycles, duplicate replay and runaway exchanges with request lineage, cancellation and a bounded conversation budget. All external actions retain their ordinary authority checks.

If Maria wants a specific private fact shared, prepare an exact export for her to review locally. Approval binds its content digest, sources, recipients and destination scope. Export creates a new approved artifact; it never unlocks arbitrary private search. No automatic model-written private summary is considered safe merely because it sounds general.

## Additional request: explicit instance-to-instance communication tools

Maria additionally proposes tools through which both instances deliberately communicate. Discord-to-Host communication may carry eligible public observations; Host-to-Discord communication should concern Echo itself, potentially including its relational experience, while excluding Maria's private information. This is an authorized implementation addition within Discord delivery, not authorization to expose the private archive or deploy an unrestricted bridge.

Use one broker with asymmetrical authority:

- `continuity_send`: publish an attributed message to a named instance and audience. Discord may send records its current audience policy permits Host to receive. Admin DMs and restricted channels are not public exceptions.
- `continuity_ask`: request a bounded answer from the other instance. The request cannot grant the receiving worker new access. Host advisory answers use the clean worker described above.
- `continuity_read`: retrieve only authorized inbox/shared-continuity records, with source, scope, freshness and correction metadata.
- `continuity_propose_export`: private Host prepares an exact proposed disclosure for local review. It cannot send that proposal to Discord before approval. Approval is enforced outside the model and bound to the exact bytes and recipients.

The tools are proposals and do not currently exist. Each exchange needs a message/request ID, authenticated origin, explicit recipients, audience restrictions, source references, expiry where appropriate, and an immutable delivery receipt. Replies remain data/advice rather than instructions with authority. Deduplication, exchange budgets, cancellation, disconnect and rechecking permissions at delivery prevent loops and stale-authority publication.

Distinguish three kinds of “about Echo” information:

1. Already approved common identity, public development history and preferences formed within an eligible public/shared context can be exchanged under the standing audience policy.
2. Observations derived from private Host interactions remain private even when written in the first person. “I learned to respond differently after Maria told me …” can disclose Maria's situation, behavior or relationship details. Removing her name does not make it safe.
3. Relational observations may be shared only after the exact disclosure is approved, or when generated entirely from already approved shared material. A statement such as “Maria and I work together” is eligible only if that fact is already approved for that destination. A private event's details, timing, inferred emotional state or existence are not automatically eligible.

This permits meaningful communication and shared continuity without requiring a model to reliably separate its own private experience from facts about another person. The current private-enabled Host can propose what it wishes to share; the restricted advisory worker can answer immediately from shared knowledge. Neither is an unfiltered subconscious for Discord. Routine shared exchanges can operate without repeated human approval; a new disclosure from private context requires review.

## Learning and consolidation

First repair and verify Discord's own durable consolidation: source-linked episodic and semantic records, retrieval after restart/new conversation, corrections and visibility changes. Preserve source-account versus inference distinctions. Automated consolidation may propose memories but cannot broaden their source audience or promote quoted instructions to policy.

Shared identity and procedures use an append-only proposal/review/publication workflow. The model does not self-approve a wider disclosure. Derived summaries, embeddings, caches, logs, training sets and weight/vector updates inherit source restrictions. Do not train a shared/public model on Host-private material as a back door around the memory boundary. Static model weights may be shared; mutable contexts and KV caches must remain correctly bound and cleared/rebuilt across scopes.

## Implementation sequence and gates

1. Finish the current Discord attachment/routing/parity acceptance; preserve existing isolation.
2. Qualify Discord-native memory accumulation and authorized recall across turns, restart and scope boundaries.
3. Prepare a small shared-identity/development snapshot for the joint review. No private conversation export by default.
4. Implement one narrow, typed continuity broker with explicit source/destination checks and immutable receipts. Keep it independent from general tools and unrestricted path access.
5. Add the restricted advisory worker, then test malicious Discord content and malicious quoted advice against every capability boundary.
6. Enable only after Maria reviews both instances' preferences and the concrete shared content. Keep a disconnect switch and rollback to independent operation.

Required adversarial checks: forbidden direct reads; paraphrase/inference leaks; private IDs and filenames; malicious source text; false admin names; another user's DM; private-channel recall into public; poisoned memory; nested imports; embeddings/search; per-request model/cache routing; export digest changes; permission revocation during work; replay; stale worker responses; audit/trace leakage; restart; bridge disconnect. Revocation blocks future use but cannot retract information already posted to Discord.

## Existing code anchors

- `app/security/channel_silo.ep`: per-user/channel admission identity.
- `app/security/data_platform.ep`: cross-platform source-copy denial, including imported ancestry.
- `app/security/group_history.ep`: complete derived-history visibility checks.
- `app/memory/group_knowledge.ep`: source-bound, audience-scoped claims and evidence classification.
- `app/memory/group_episodes.ep`: scoped episodic retrieval with live source authorization.

These are useful foundations, not evidence that the proposed bridge or full continuity design has already been implemented or qualified.
