> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Silo recap and recovery qualification

The existing context engine is shared by joined groups, user-in-channel contexts, admin contexts and Echo-system contexts. Their admission and audience checks differ; their durable run, recap and recovery paths must retain those boundaries.

On 16 October 2026, `tests/context/test_group_recap_rebuild.ep` was extended to route each of those scope types through its actual admission function. Four isolated database executions passed. Each also contained a separate private conversation, cancelled before the tested recap proceeded.

The fixture requests a recap through the recorded tool path, closes and reopens the database, generates and reviews a candidate, then deliberately fails the completion-receipt write after checkpoint activation. It reopens again and completes the same run using the existing checkpoint. Assertions require no private canary in assembled context or the tested archive, no private checkpoint, exactly one rebuild completion, valid archive chaining, and a completed-run reopen which needs no inference. The private run must remain cancelled. Each retained database passes SQLite quick_check.

The provider fixture supplies deterministic model judgments. These results establish native integration and fault recovery, not a new live-model or physical-device performance result. Previous live joined-group recap evidence remains separate. Public-channel observations are source records governed by their permission receipts, not an additional conversational model run.

Run the compiled fixture with a new database path:

    ECHO_SYSTEM_PRINCIPAL=device:other ./test_group_recap_rebuild DATABASE gap channel
    ECHO_SYSTEM_PRINCIPAL=device:other ./test_group_recap_rebuild DATABASE gap admin
    ECHO_SYSTEM_PRINCIPAL=device:other ./test_group_recap_rebuild DATABASE gap system
    ECHO_SYSTEM_PRINCIPAL=device:other ./test_group_recap_rebuild DATABASE gap group

Each invocation must use a different nonexistent database. Omitting `gap` retains normal completion coverage; the scope argument requires four process arguments including the executable. Existing two/three-argument group cases remain supported.

Private archives, build/runtime output and the summary are retained in `evidence/phase2/silo-recovery-20261016/`. No production conversations or model processes were changed by this qualification.

## Shared recap inspection

The saved-recap reader previously required a private-session owner even after the caller had authorized a shared audience. Shared sessions intentionally have no private owner, so reads returned no recap despite successful creation. The reader now selects checkpoints from the exact authorized shared session while retaining the existing history-disclosure checks. Web requests authorize the current device and session before reading; tool dispatch authorizes the frozen initiating run. Requested checkpoint identities never widen the session query.

The integration fixture also reads the generated recap through `web_compaction_read` for a member, rejects an outsider, and verifies that an exact checkpoint ID cannot expose it through a different private session. All four scope variants passed, as did the existing private Unicode/pagination/previous-checkpoint/revocation test. Evidence: evidence/phase2/shared-recap-reader-20261016/.
