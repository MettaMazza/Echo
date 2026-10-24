> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Scoped run inspection and cancellation

Current deployment: these tools are included in the deployed schema-26 server (`phase2-git-20261016-233505`). Earlier statements below about non-deployment describe their original qualification date; they are superseded by the research release and subsequent release manifests. Current configured runtime dependencies and source hashes were checked during the Step 08 reconciliation.

Step 08 implementation, 16 October 2026. `job_control` lists or inspects existing runs and requests cancellation. It reuses Echo's existing run ledger, worker claims and cancellation monitor; it does not create a second job state machine. List cursors use persisted row identities and return original objective/last-event references for result inspection.

Authority comes from the initiating run and tool call. Private results require the same principal and current device access; shared calls are limited to their own conversation. Arguments cannot select a principal or device. Cancellation additionally uses the existing sender control check. Request receipts and the durable cancellation row commit together. Repeating an unresolved request preserves the original receipt; terminal jobs report their actual state.

A stop acknowledgement is not completion or rollback. Active workers retain control of shutdown. The existing idle reconciler now settles ready, waiting and failed runs only when no worker is running and no tool outcome remains unresolved. It records an actual cancellation event. Tool results explicitly instruct callers to inspect the resulting state.

Native streaming/dispatch tests pass private and group isolation, stop requests, active-worker preservation, idle transitions, duplicate receipt handling, argument rejection, reopen and revocation. Existing idle cancellation regression passes. The complete server builds with the unchanged Ernos compiler. Evidence and exact working-source hashes: `evidence/phase2/job-control-20261016/verification.json`. No production deployment. Scheduling, delegated launch and the remaining step 08 tool families are still outstanding; this is not a claim that the whole step is complete.
