> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Ernos authoring and actionable feedback — 18 October 2026

Echo's live architecture guidance and portable example now make Ernos Plain the default for generated programs, tools, scripts, calculations, workflows and self-modifications, for parent and delegated runs. Another language requires a demonstrated capability gap or unavoidable integration requirement, with the reason stated. Convenience and assumed limitations are insufficient. Existing dependencies need not be rewritten.

The guidance includes a compact syntax reference drawn from the existing Ernos application: entry points, variables, loops, functions, imports, display and build/run operations. This is general language support, not a task-specific solution. The original identity bundle is unchanged.

Feedback guidance distinguishes accepted, queued, running, completed, cancelled and failed actions. Errors should explain the observed cause, effects and supported next step, preserve unknown outcomes, inspect diagnostics and verify recovery rather than invent success.

`app/workspace/ernos_runtime.ep` supplies status-specific recovery for invalid compiler arguments or filenames, missing source, conflicting stdlib entries, compiler integrity mismatch and missing runtime files. `app/tools/execution_tool.ep` includes that recovery, the compiler-preflight stage and compiler-not-started effect in its receipt. Configuration/integrity failures do not recommend bypassing safeguards or silently switching languages. Both server and execute_guardian were rebuilt with the unchanged existing compiler.

The first live authoring probe chose Ernos but guessed invalid syntax. That failed probe remains archived as run `155ee503-f87b-4b03-9197-cd8902af151e`; it was stopped after discovering the need for accessible syntax guidance. Final verification uses a fresh run with the complete updated guidance and a deliberately invalid compiler request followed by actual build/run recovery. Run `10bbf2c7-6e87-4bf2-b473-bdbeeacf05c6` passed: the invalid request returned compiler-preflight, compiler-not-started and exact recovery guidance; Echo authored .ep source, compiled it successfully, ran the executable with wait_status 0, read actual output 210, attached the source and delivered its final response through the session API. The source downloaded with SHA-256 b67ad5eda81ed3985b066fe010c7719f8ab31f32129826acd8091f175f3c31f4. No Python execution was used by Echo. The test supplied an objective, not implementation code. It used compiled client functions with DOM doubles, live HTTP/model/tools and archive verification; not a visual browser check. Total observed duration was 447.922 seconds, including initial context processing and competing work; this is not latency acceptance.

Runtime evidence: maintenance/ernos-feedback-live-results.json and maintenance/ernos-feedback-assertions.json. Release: phase2-ernos-authoring-feedback-20261018.

This change establishes general feedback guidance and repairs compiler-preflight receipts. It does not claim that every error path throughout the entire application has been audited.
