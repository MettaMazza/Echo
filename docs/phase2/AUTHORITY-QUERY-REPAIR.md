> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Retained authority query construction

Native Git acceptance exposed an intermittent SQLite syntax error during frozen-authority revalidation. The guardian correctly denied execution when the query failed, but this cancelled legitimate work. A captured failed query began its condition with `WHERE ) OR OR …`: the private-owner and group-scope fragments were missing. This was not a recorded privilege revocation.

Both run and tool authority now evaluate scope helpers into named retained strings before assembling the query in sequential statements. Previously nested allocating calls could leave intermediate strings unretained while a subsequent helper allocated. The generated native code roots the named strings; no compiler changes or permission relaxation were made.

Verification: two concurrent workers each completed 1,000 run/tool authority checks against the original failed live-run database. Three fresh native Git workflows completed all 14 operations, including local bare-remote push/fetch and rollback. Admin execution passed; guest, revoked and later-promoted initiating authority were denied. These checks do not substitute for the separate actual-model Git acceptance.

The Git workflow helper also rejects a terminal non-ready run immediately after tool execution; it no longer starts another mock-provider exchange after cancellation.

Private evidence, including failed archives, captured SQL and native results, is retained under `evidence/phase2/git-tools-20261016/`. The repeated-query regression is `tests/security/test_authority_query_repetition.ep`, taking an isolated archive, run ID and call ID whose frozen admin authority remains valid.

The subsequent actual-model Git acceptance passed and the repair was deployed with `phase2-git-20261016-233505`; see [GIT-TOOLS.md](GIT-TOOLS.md).
