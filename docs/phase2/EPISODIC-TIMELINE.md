# Chronological episodic recall — Step 09

The timeline reads the existing immutable knowledge claims and their original archive events. It does not introduce a second memory store or replace source text with summaries. Deployed in release `phase2-episodes-20261017-001039`; both local HTTP and LAN HTTPS status endpoints returned 200. The model process remained unchanged. This is host-side LAN verification, not a new physical-phone test.

`app/memory/episodes.ep` implements private and linked-principal chronological retrieval. A reported event date takes precedence for ordering. Otherwise the source-recording timestamp supplies an explicitly labelled ordering time, without being represented as the date of the experience. Wholly unknown dates sort last. The result preserves reported, source-recorded and ingestion times separately, with source event ID, exact digest, originating session and principal, evidence classification, project selection at source time and correction links.

`memory_timeline` accepts a cursor, caller-selected positive page size and optional category. Its first page freezes the maximum authorized claim sequence. Later backdated imports therefore cannot shift subsequent pages; restarting traversal includes them. Each page resolves current source ownership, linked-principal scope and forgetting visibility. The cursor is bound to principal and category. Superseded claims remain visible as historical records with explicit correction links. A cursor is a position, not an authorization credential.

The immutable tool ledger supplies arguments to `knowledge_tool_execute`; `memory.private-read` is checked before and after retrieval. The schema is advertised only when the knowledge store and private principal are available. No artificial maximum page size is imposed.

## Evidence

`tests/memory/test_episodes.ep` compiled with the existing self-hosted compiler and passed against `evidence/phase2/episodes-20261017/chronology-01.sqlite`. It exercises deliberately nonchronological insertion, reported versus source dates, wholly unknown dates, project provenance, frozen-boundary paging, later backfill, cross-principal cursor rejection, forgetting, correction linkage, database reopen, archive-chain verification and principal revocation.

`tests/memory/test_live_episodes.ep` exercises the actual local model and dispatcher in an isolated archive. It passed against `live-02.sqlite`: Echo called `memory_timeline`, reread the original source, and returned the exact milestone marker, source event ID, reported event date and distinct recording timestamp. Both archive chains, SQLite quick-check and foreign-key checks passed. The first fixture attempt exited during redundant schema setup; its database and diagnosis are retained as `live-01-result.json`.

## Remaining Step 09 work

This component does not close Step 09. Participant/channel enrichment, shared/self memory scopes, semantic consolidation, evidence-backed procedural discovery, continuity imports and the complete compaction/independent-restoration acceptance remain governed by the existing Step 09 plan. No new plan steps are added here.

## Automatic event-date ingestion

The deployed server asks extraction for a normalized event date and an exact temporal phrase inside the source quotation. The validator checks that the date is calendar-valid and the phrase is present in the retained quote. Normalization remains model-derived: syntactic validation does not independently prove a natural-language interpretation. Ambiguous, relative or absent dates are requested as empty; invalid dates or absent temporal evidence leave the claim undated without losing its original text. Original generation events retain the extraction output for inspection.

Existing records and indexing receipts are preserved; this change applies to newly ingested source events and does not silently rewrite earlier claims. Explicit category-as-kind aliases are normalized to semantic or episodic types, including the observed `commitment` alias. Source ownership and exact quotations remain required.

The final native date-ingestion regression passed, including category aliases. The existing automatic-memory regression passed for compatibility. The corrected live test passed: an ordinary submitted account containing “3 February 2026” was automatically stored with event date `2026-02-03`, then retrieved by the actual model in a new session with exact source provenance. Evidence is in `evidence/phase2/episode-extraction-20261017/`. Deployed as `phase2-episode-dates-20261017-001615`; both status endpoints returned 200 and the model process remained unchanged. The first live fixture preserved its failure: a manually appended event was outside the eligible objective-event surface, and extraction returned the unhandled `commitment` kind. The corrected fixture submits the dated account through the ordinary user-message path.
