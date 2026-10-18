# Documentation index

Start with the [Echo 1.5 forensic release](releases/1.5/README.md): current system audit, original 32-step status, verification and Phase Three sketch.


- [Current status and open issues](STATUS.md)
- [Architecture and execution flow](ARCHITECTURE.md)
- [Build, verification and test reproduction](BUILD-AND-TEST.md)
- [Running, pairing, backup and recovery](OPERATIONS.md)
- [Security and data boundaries](SECURITY-AND-DATA.md)
- [Compiler, dependencies and provenance](PROVENANCE.md)
- [HTTP API and model tools](API-AND-TOOLS.md)
- [Forensic method and evidence limits](FORENSIC-METHOD.md)
- [Complete application reference](reference/APPLICATION.md)
- [Complete test/fixture reference](reference/TESTS.md)
- [Observed database definitions](reference/DATABASE.sql)
- [Source hashes, imports, definitions and test references](forensics/source-manifest.json)
- [Local evidence catalogue](forensics/local-evidence-index.json)
- [Database integrity observation](forensics/database-status.json)
- [Release and incident verification](forensics/release-verification.json)

The generated reference covers every application and test source file, including command-line entry points, browser modules, diagnostic programs and fixtures. A listed test is not automatically a passing test. Original runtime records remain local and are identified by path/hash in the catalogue.

## Current implementation notes

- [Unified multimodal model](UNIFIED-MULTIMODAL.md)
- [Prompt inheritance and technical adaptations](PROMPT-MIGRATION.md)
- [Attachments and repair history](ATTACHMENT-REPAIR.md)
- [Kokoro speech and persistent trace panels](KOKORO.md)
- [Maintainer handover and documentation updates](MAINTAINING.md)
- [Publication and retained-history review](PUBLICATION.md)
- [Dependency-ordered Phase Two plan](../phase2.md)

- [Supporting configuration, compiler and planning inventory](forensics/repository-manifest.json)
- [Exact model tool schemas](reference/TOOL-SCHEMAS.json)

## Phase Two implementation

- [Progress and next actions](phase2/PROGRESS.md)
- [Requirement and acceptance ledger](phase2/REQUIREMENTS.json)
- [Inspected source integrations](phase2/SOURCE-MAP.md)

- [Phase Two foundation verification](phase2/FOUNDATION-VERIFICATION.json)

- [Tool discovery contracts and dependency reporting](phase2/TOOL-CONTRACTS.md)
- [Scoped workspace files](phase2/WORKSPACE-FILES.md)
- [CSV and TSV tables](phase2/DELIMITED-TABLES.md)
- [Exact JSON selection](phase2/JSON-SELECTION.md)
- [Archive creation and extraction](phase2/ARCHIVE-INSPECTION.md)
- [Workbook reading, creation and editing](phase2/WORKBOOKS.md)

- [Delegated tasks, authority and verification](phase2/DELEGATION.md)
- [Local Ernos compilation and execution](phase2/ERNOS-PROGRAMS.md)

- [Workspace plots](phase2/PLOTS.md)
- [Local workspace databases](phase2/LOCAL-DATABASES.md)
- [Admin Code/Git workflow](phase2/GIT-TOOLS.md)

Project continuity backend and current integration status: [phase2/PROJECTS.md](phase2/PROJECTS.md).
Step 08 reconciliation: [phase2/STEP-08-EVIDENCE.md](phase2/STEP-08-EVIDENCE.md).
