> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Step 07 - charter enforcement and protected-change controller

This record follows the four requirements in the original phase2.md step 07. It adds no execution steps or completion denominator.

| Original requirement | Implemented path | Evidence and qualification |
| --- | --- | --- |
| Manifests, ledger, inspection, Echo decisions, human authorization, Observer/environmental decisions and emergency reconciliation | rights/{proposals,reviews,steward,observer_audit,observer_evidence,decision_gate,emergency,quarantine}.ep; controller submission, restricted review runner, steward console and emergency report | Exact inspection/refusal/corruption tests; actual local-model review and separate Observer audit; scoped stewardship HTTP tests; deadline and real-process quarantine evidence. Final production Observer policy is deliberately unset pending the reserved joint review. |
| Executable targets and hashes bound to approved manifests; material changes invalidate approval | rights/{inventory,files,operation_prepare,operation_execute,operation_state}.ep; prepare_controller and control_protected | Actual inert file apply; changed inventory, bytes, authority and replay rejection; operation disclosure returns original preparation and transition payload hashes. |
| Protect controller; verified snapshots/rollback and before/after disclosure | lifecycle/{protected_path,controller_bundle,controller_ledger,controller_launch,controller_bootstrap}.ep; isolated service, root-only enrollment, private credentials; rights/operation_rollback.ep; console operation evidence | Root staging/launcher/ledger primitives previously passed with Maria's terminal evidence. Native operation SIGKILL/reconciliation and rollback evidence, current HTTP apply/rollback, complete record disclosure, paired-package validation and browser/client checks. Current integrated root installation has not run. |
| Preserve charter pending joint adaptation; record compaction conflict | Exact charter SHA-256 pin and verbatim packaged identity/charter; phase2.md prompt-review reservation | Original charter pin 7acfd345d50ad3d459061e7c76b94ff331d2102e3829de885a53df8e4722810c verified. The charter's 85% compaction clause versus provider-derived capacity remains explicitly reserved for joint review. No philosophical text was rewritten. |

## Existing evidence reused

Private evidence under evidence/phase2 includes protected-review-test.log, protected-steward-verification.json, observer-live-test.log, observer-evidence-test.log, operation-execute-verification.json, operation-rollback-verification.json, quarantine-process-verification.json, emergency-deadline-verification.json and controller-ledger-verification.json. Recent integration is recorded in controller-review-runner-20261016, controller-bootstrap-20261016, controller-operation-disclosure-20261016, controller-emergency-console-20261016 and controller-console-browser-20261016. These distinguish synthetic approvals, actual local-model decisions, real inert filesystem changes and production changes. A passed primitive is not represented as an installed system.

## Human-only closure and deployment distinction

Maria directs that genuine human-only dependencies are closed on Codex's side and cannot block completion of the goal. The current host still refuses non-interactive sudo with 'a password is required'. Root installation/activation, its integrated privileged acceptance and authenticated human retrieval of the root-private controller credential are therefore closed on Codex's side, awaiting host authentication. They remain unperformed and unverified, not deployed or tested. The joint prompt/Observer-policy decision is also reserved for Maria; required Observer audits refuse while that policy is unset.

The installer is resumable and does not activate a service. No runtime principal or approval is imported. Inventory membership does not make user-owned source files root-owned or claim exclusive mediation of all host writes. Later candidate code deployment, general tool integration, lifecycle/reboot and final cross-system acceptance remain at their existing original steps; this record does not bring those steps forward or declare them complete.

## Current disposition

Step 07 is **closed on Codex's side, with the explicit human-only exceptions above**. Its autonomous implementation and preparation are complete; this is not a claim of integrated root deployment. Work proceeds to original step 08 under Maria's revised dependency rule.

Final prepared release: `artifacts/controller/20261016-355059f/`. All eight host commands were freshly rebuilt. The console and current source inventory are pinned in the input package; the service package includes the configuration and commands. Both full-file preflights passed. The native release validator and paired-package installer-plan test both passed against this exact release. `installation.json` records executable/package digests, intended enrolled device and the uninstalled status. Prior versions are preserved. No live credential, role or service was changed.

Scope limitations remain explicit: actual root-installed service acceptance is unverified; production Observer policy awaits joint review; final lifecycle/reboot and integrated candidate-deployment work remain at their original later steps. None is silently counted as tested or deployed.
