# Maintainer handover

Start with STATUS.md for current scope, ARCHITECTURE.md for execution boundaries, and API-AND-TOOLS.md for external contracts. APPLICATION.md and TESTS.md under reference map every first-party Ernos source file to its definitions, imports and SHA-256. TOOL-SCHEMAS.json preserves exact literal tool contracts; DATABASE.sql preserves schema without private rows. Config examples and the compiler overlay are catalogued in repository-manifest.json.

## Changing the system without losing its history

1. Read the affected implementation, its imported modules and corresponding fixtures before editing. Do not infer behavior from an old plan or test filename.
2. Preserve user originals, run/submission IDs and tool-effect records. Build against the pinned compiler and retained overlay. Do not alter the compiler as an incidental application fix.
3. Run validation appropriate to the changed behavior. Record source revision, exact command, expected outcome, actual outcome and evidence digest. Distinguish compilation, fixture execution, provider tests and real browser/device tests.
4. Generate source identities from every non-AppleDouble .ep file under app and tests. For each, record bytes, lines, SHA-256, imports and definition signatures. Update the application/test references and inventory totals together. This is lexical coverage, not semantic proof.
5. Update the architecture/API/status narrative when behavior changes. Refresh schema from a read-only SQLite connection; never publish database rows. Add evidence summaries and hashes to release-verification.json without publishing prompts, credentials or conversations.
6. Verify all relative Markdown links, documented route names, source hashes and tool-schema names against the source. Review the staged diff and every newly included asset for private content.
7. Build into the Mac runtime; wait for active runs to finish before deployment. The server loads browser JavaScript at startup. Restart coherently, verify served bundle digest, then exercise the changed behavior. Keep rollback files private.
8. Commit logical changes. Local and connector publication histories have different parents: compare tree identities, preserve remote ancestry, and use an expected-head lease. Never force-push to disguise divergence. Record the published commit and tree locally after success.

## Reporting a fault

Include the source revision, platform, provider/adapter, operation, observed status, reproduction steps and a redacted incident excerpt. Never post a browser token, invitation, model API key, full conversation export or private identity prompt. Relevant records are the saved run state, tool ledger, incident JSON-lines log, service stderr and macOS crash report. Keep uncertain actions unresolved until their effects are checked.

## Scope of this documentation

The first-party source inventory is complete navigation, not a claim that every branch was manually audited or every test was run. Dependency source and model weights remain upstream acquisitions. Model metadata, configuration and measured evidence are separate facts. Phase Two remains a plan; its features must not appear as implemented in the README until tested and documented.
