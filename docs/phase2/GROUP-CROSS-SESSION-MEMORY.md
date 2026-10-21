# Shared memory across new conversations

21 October 2026. Native and actual-model qualification passed; not deployed yet.

Starting a new conversation in the same audience must preserve provenance-backed
memory without copying the old transcript into the new conversation. Previously,
claims were scoped to the audience but their source reader required the current
session. Extraction also considered only the current session's compacted records.
These two restrictions prevented automatic cross-session shared recall.

The memory reader now requires an exact group and audience-revision match, plus
the current run's authenticated authority and the source-history permission check.
The ordinary session/history/workspace gate is unchanged. Cross-session child
delegation sources and child readers remain isolated. Private history and other
user, channel, server and DM audiences do not become accessible. Source digests,
source authorship, correction lineage and visibility remain authoritative.

Extraction considers accepted prior-session originals in the same audience;
current-session originals still require an activated compaction boundary. Durable
index receipts prevent duplicate extraction. Automatic context, graph and timeline
selection use the same source-session rule. Reads recheck current authority.

Qualification exposed a second problem: an unindexed original could reintroduce
a forgotten claim through pending-source fallback. Originals containing a forgotten,
superseded or expired claim are now withheld from whole-source automatic reuse and
extraction. Other still-visible derived claims remain eligible. Explicit authorised
history access retains the original. This deliberately withholds the whole original
because a full quotation can repeat the withdrawn material. No source is deleted.

## Source inspection

- Echo: `app/memory/group_knowledge.ep`, `group_ingestion.ep`, `group_context.ep`,
  `group_episodes.ep`, private/shared automatic context, group authority/history,
  new-session selection and delegation provenance.
- HIVE commit `99961051e95805bddc1288b4f4ab2bc03b9a493f`,
  `src/models/scope.rs`: exact public user/channel and private-user boundaries.
- ErnosDecent commit `4ef202c5a920f17e5d2f6bde406b62e8185b1531`,
  `decent_agent/memory.ep`: distinct memory tiers, graph and explicit lifecycle
  tools. Its truncation and ring-buffer limits are not copied into Echo.

## Acceptance

The native scope/lifecycle fixture and both same-session and cross-session
actual-model extraction/recall variants passed (five tests). The existing model tests accept an optional fifth argument to run
in a newly selected conversation. Their original four-argument path still tests
same-session recall and compaction-boundary extraction. A seeded compaction
boundary is not a claim of model-generated compaction acceptance.

Canonical test artifacts are retained on One Touch under
`evidence/phase2/group-cross-session-20261021`. Failed trials remain retained.
The first extraction attempt exposed a SQL syntax error, corrected before any
deployment. The next native lifecycle trial exposed the pending-original forgetting
problem above. Successful intermediate extraction is not final acceptance of the
subsequent lifecycle change.

Live Discord new-session, repeated-turn and restart acceptance, deployment and
latency qualification remain pending. First-use extraction may add model work;
small fixture timings do not establish latency for a large historical backlog.
This change does not implement the separate reviewed Host–Discord continuity
bridge, the remaining memory UI, or complete Phase Two.

The existing `memory_search` and `memory_read` dispatcher is explicitly private
principal-only (`app/tools/memory_tools.ep`, `app/memory/archive_recall.ep`). This
repair does not silently widen that dispatcher. Dedicated audience-scoped original
search/read integration remains required; the claim graph and episodic timeline
are the model tools covered by this acceptance. Original conversations can still
be explicitly selected through the authorised session-history workflow.
