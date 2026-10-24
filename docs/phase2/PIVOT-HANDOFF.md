# Echo 1.9 maintenance snapshot and OpenCode rebuild handoff

Updated 24 October 2026. Existing Echo development is being wrapped up for an OpenCode-based rebuild evaluation. This is a preserved, deployed maintenance snapshot, not a declaration of completed Phase Two or the V1 production milestone.

## Deployed repairs

- Memory recall preserves actionable identifiers, source-event links and evidence receipts through encoding. User-message evidence is supplied with the correct kind and digest; recovery schemas retain validation.
- Public Discord admins can inspect published application source and execute scoped system tools. The execution guardian receives the actual source/privacy policy. Public execution remains isolated from private databases, credentials, private host roots and private local services. Explicit owner-linked DM/web recall remains separate from public community recall.
- Long Chatterbox Nano speech is split at bounded sentence/word boundaries and assembled into a complete WAV. CPU eight-thread operation and the installed Echo reference are retained. Text is not shortened to fit the model's generation ceiling.
- Startup prefix warming is enabled. Highest configured reasoning remains unchanged; no response-length cap was introduced.

The deployed source corresponds to [repair commit 85cd292](https://github.com/MettaMazza/Echo/commit/85cd292eafc3d29ea2a2261707967344b6880088), equivalent tree to local qualification commit `a8a4017`. Production web, six Discord workers and the repaired resident speech service were restarted/selected. Readiness checks returned HTTP 200 for web and all three model services, a fresh Discord heartbeat acknowledgement, zero active conversation/tool jobs, zero active inference slots and a responsive idle speech listener. The isolated test web server and retired speech service were stopped. Private configuration, databases, build/source hashes, deployment receipts and rollback material are retained on the operator's host, not published here.

## Verification and measured limits

Native tests cover public-admin source access, guest/revoked access denial, source symlink/hardlink protection, actual tool-to-guardian execution, private/credential/child-shell denial, workspace writes, source-write and private-loopback denial, command cancellation/revocation, explicit private identity linking, DM/web recall and public/foreign-user denial. Final deployment binaries were compiled from the frozen repair source and checked against its hashes.

| Workload | Observed result |
|---|---|
| Real-model fresh greeting | 30.27 seconds |
| Memory save | 16.18 seconds |
| Source investigation | 125.24 seconds, compared with the recorded 443.57-second incident |
| Arithmetic | 8.10 seconds |
| Short Nano speech | 2.24 seconds generation / 4.96 seconds audio |
| Long Nano speech | 26.05 seconds generation / 63.68 seconds audio |
| Longer varied Nano speech | 42.35 seconds generation / 107.44 seconds audio |

These are individual measured runs, not statistical guarantees. Independent transcription recovered all sections of the three speech samples; this is not a claim of human listening acceptance. The real-model conversation checks used an isolated web session. Native Discord transport/security tests passed, but the computer-use Discord acceptance attempt timed out. Full Discord GUI and live voice acceptance remain open. Complex source investigation still takes minutes; latency is not universally resolved.

## Preserved unfinished work

The [autonomous RSI prototype branch](https://github.com/MettaMazza/Echo/tree/wip/autonomous-rsi-20261024) contains the controller, qualification and deployment/rollback prototype plus its explicit WIP notes. It is **not deployed or production-qualified**. An intermediate inert qualification test passed; final prototype edits have not completed qualification. It must not be represented as an end-to-end autonomous self-improvement system.

The [existing Phase Two remaining-work inventory](../releases/1.9/REMAINING.md) remains the acceptance boundary. Generated-tool activation, the complete protected autonomous research/improvement lifecycle and full live voice acceptance are not closed by this handoff. No new completion percentage is inferred from commit counts.

## OpenCode rebuild requirements

1. Preserve Echo's identity, original event/memory records, provenance, private account links and media assets. Keep secrets and private records outside Git and migration logs.
2. Preserve attributed public community recall across users/rooms. Private DM/web history is available only to its explicitly linked owner. Public admin system authority must never imply access to private memory or credentials.
3. Keep authenticated admin entry from Discord and web. Retain regular-user restrictions, cancellation, follow-up steering, attachments and context/session continuity.
4. Benchmark the same tasks, model, highest reasoning setting and context against this snapshot. Do not obtain faster numbers by lowering reasoning, silently discarding context or truncating output.
5. Implement autonomous improvement as candidate isolation, independent fixed acceptance checks, Echo's protected review/consent, verified deployment, health checks and automatic rollback. Routine human approval is not the target design; authorization and privacy boundaries still apply.
6. Reuse proven memory, community and media interfaces where useful. Evaluate OpenCode as the agent engine before porting its entire implementation into Ernos; preserve the ability to take upstream fixes.
7. Keep the deployed Echo snapshot available until the replacement passes privacy, tool-authority, memory, latency, long-speech and Discord/web workflow parity tests.

No OpenCode production migration has started. This snapshot is the rollback baseline and the starting point for that rebuild.
