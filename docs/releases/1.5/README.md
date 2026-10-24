> **1.9 documentation:** [Current update](../1.9/README.md) · [feature status](../1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Echo 1.5 — forensic release record

18 October 2026. This is an intermediate release of the working local system. Phase Two is not complete. The original 32-step plan remains the scope; source counts and test cases are not additional roadmap tasks.

## Read this record

- [System and change audit](SYSTEM-AUDIT.md): execution, persistence, access boundaries, implementation changes and recovery.
- [Original 32-step status](PHASE-TWO-STATUS.md): delivered work, incomplete integrations and deployment distinctions.
- [Verification and publication](VERIFICATION.md): measured evidence, limits and publication method.
- [Phase Three sketch](PHASE-THREE.md): existing deferrals, without moving Phase Two commitments.
- [Complete evidence-document index](EVIDENCE-INDEX.md): maintained Phase Two narratives and their historical scope.
- [Per-file change manifest](change-manifest.json): changes against the local and published baselines.
- [Application reference](../../reference/APPLICATION.md) and [test reference](../../reference/TESTS.md): every first-party Ernos source file, definitions, imports, hashes and lexical references.
- [Source manifest](../../forensics/source-manifest.json), [supporting repository manifest](../../forensics/repository-manifest.json), [readable tool catalog](../../reference/TOOLS.md), [exact tool contracts](../../reference/TOOL-SCHEMAS.json), [HTTP routes](../../reference/HTTP-ROUTES.json), [environment names](../../reference/ENVIRONMENT.json) and [database schema](../../reference/DATABASE.sql).

## Release boundary

The inventory contains 871 Ernos source files: 402 application files and 469 tests/fixtures, 64,648 lines and 2,021 definition signatures. It extracts 61 distinct tool names and 39 literal HTTP route paths. These are source inventories, not a claim that every tool is enabled for every user or that every branch has been exercised.

The current deployed application is `phase2-stable-session-recall-20261018-r2`. Documentation work does not restart it, clear its cache, terminate user work or run inference workloads. Live evidence cited here was collected during the corresponding repair; publication checks are separate.

The application and browser source are Ernos Plain. Generated JavaScript, native binaries, upstream inference engines, media dependencies and maintenance utilities are distinct layers. The presence of a maintenance script is not evidence that Echo's application has been rewritten in that language.

## What is delivered

Local text/image inference; durable multi-turn tools; conversation history, branches and export; device pairing and linked conversation libraries; streaming and same-run guidance; queued messages; per-reply activity and agent panels; delegated jobs with parent continuation; persistent source-grounded memory, corrections and scratchpads; task tracking; modern local tools and Ernos authoring; scoped authority; local speech and artifacts; protected-change foundations; recoverable operation records; detailed errors and operational diagnostics.

The remaining substantial Phase Two work is Discord, Gmail, persistent generated-tool integration, the complete protected self-improvement and model-research/training lifecycle, optional external providers, final lifecycle/integrated acceptance, joint prompt review and final closure. Some of these have foundations already implemented. A foundation is recorded as such rather than as a completed integration.
