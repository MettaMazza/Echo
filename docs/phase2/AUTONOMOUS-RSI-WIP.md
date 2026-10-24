# Autonomous RSI prototype — preserved, not deployed

Preserved on 24 October 2026 when Maria requested wrapping up Echo and preparing an OpenCode rebuild. This branch is unfinished implementation, not a production-ready RSI system.

The prototype adds pinned qualification plans, sandboxed execution of installed checks, candidate/result hash binding, controller-owned qualification receipts, and a machine-qualification path through the existing protected activation gate. Echo review/consent, inventory, preimage and recovery checks remain. A controller entry point attempts baseline health, qualification, activation, post-activation health and rollback verification without routine human approval.

An intermediate native fixture passed with synthetic Echo consent, an actual sandboxed evaluator, zero human stewardship records, transactional exchange and rollback. Separate plan and runner fixtures distinguished passing and failing checks. Subsequent edits added unique attempt identities, output-directory separation and baseline/recovery health handling; these final edits have not received complete qualification. Do not treat the earlier passing fixture as qualification of this branch head.

Still missing: the authenticated Discord/web request bridge, candidate source/build staging, installed controller configuration and service, job draining and service restart integration, crash/retry/reconciliation coverage for the entire workflow, independently pinned application acceptance suites and actual deployment/workflow verification. The root-owned controller is not installed. Merely calling the prototype controller does not provide complete recursive self-improvement.

Carry the tested requirements and useful primitives into the rebuild; do not enable this prototype in production or infer capabilities from the presence of files. Production continues to use the separately verified repair branch.
