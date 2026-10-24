# Verification and limits — 1.9

This publication separates fresh release checks from retained functional evidence. It does not restart production, certify every fixture, or declare Phase Two complete.

## Fresh publication checks

- Exact equality of the last pushed community tree and the local committed baseline established before switching to remote ancestry.
- Complete publishable source-byte inventory and changed-file before/after SHA-256 ledger, including Ernos, C and Python fixtures/support.
- Native voice callback C syntax check with Clang.
- Fresh isolated native regression results are recorded in [native-verification.json](native-verification.json), including compilation markers, new executable hashes and actual exit status.
- Relative documentation links, JSON, manifest identities, Git whitespace and a known-secret/signature publication scan are checked before push; [publication checks](publication-checks.json) records outcomes and limits.

## Retained functional evidence

[Retained evidence summaries](retained-verification.json) contain receipt hashes and selected nonprivate fields. The original local receipts and archives are not published.

- Checklist delta/atomic-final delivery had compiled application workers and positive/negative qualification. The retained build receipt lists exact executable hashes; publication does not imply those binaries were rebuilt here.
- Private memory qualification recorded an authorized original read and denied unauthorized linked-archive reads.
- Nano CPU8 reference synthesis measured approximately 2.59 seconds for the short fixture and 12.01 seconds for the medium fixture. These are generation timings, not guaranteed Discord end-to-end latency.
- The response-channel correction was observed in actual web requests with highest reasoning preserved; duplicate complete-answer drafting was absent in the reviewed run, while analysis/outlining remained.
- That research workflow still took **345.95 seconds**; a calculation workflow took **12.155 seconds**. Different tasks and tool paths cannot establish a universal speedup. Research latency remains unresolved.
- Voice membership qualification does not establish completed live playback/recording/transcription acceptance.

## Explicit exclusions

No full fresh code-path audit, complete suite, full feature-film soak, final release concurrency/failure matrix or Nano reboot/login acceptance is claimed. Source inventory is exhaustive file accounting, not semantic proof. Full research/training/promotion, Gmail, generated-tool activation and external-provider requirements remain visible in [remaining work](REMAINING.md). Private host identity/prompt and configuration updates are described but not exported.
