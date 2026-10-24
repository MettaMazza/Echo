> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Scoped delimited-table reader

Phase Two step 08, 16 October 2026. `workspace_read_table` reads UTF-8 CSV, TSV, or a single ASCII delimiter chosen by the caller. It uses the authenticated session workspace and the existing read capability. It preserves string cells, empty cells, quoted separators, escaped quotes, multiline values and Unicode without formula evaluation or inferred types/headers.

Arguments: `path`, `delimiter`, zero-based record `offset`, and positive page `rows`. Results include rows, total record count, next offset, end flag and SHA256 of the entire source. The entire file is validated even for a partial page; malformed input returns a reason and byte offset. Full-file reading uses memory proportional to the source. This implements delimited text, not XLSX workbooks or the whole document-tool family.

Source: `app/workspace/read_table.ep`; schema/dispatch: `app/tools/workspace_operations_schema.ep`, `workspace_tools.ep`, `attachment_tools.ep`, agent dispatch and tool policy. Tests: `tests/workspace/test_read_table.ep`. Native regression passed for exact cells, paging, malformed tails, Unicode, explicit full-source digest equality, unauthorized access and revoked-device rejection. Fixture ran on the Mac runtime filesystem; an earlier external-drive fixture failed workspace creation. No filesystem protections were weakened.

Private evidence: `evidence/phase2/tables-20261016/final-test.json` and build logs. Release validation is recorded separately after deployment.

Deployed release `phase2-tables-20261016-205222`. Local HTTP and LAN HTTPS returned 200 after service replacement; model inference and active conversation workers were not targeted. HTTPS used the existing certificate with trust verification bypassed. Model-driven chat invocation remains to be checked; successful HTTP health is not evidence of that interaction.

Actual local Qwen agent-loop acceptance subsequently passed (`tests/tools/test_live_table.ep`): the model invoked workspace_read_table, returned a Unicode marker present only in the file and the leading-zero code, completed the run, and reopened without repeating inference. Archive-chain verification and SQLite quick_check passed. Private evidence: live-result.json and live-fixture.txt in the same evidence directory.
