# Next-session handoff — latency and optimisation

Updated 16 November 2026. Start from installed release **`echo-self-monitor-20261116-findings`** and the reconciled local `release/echo-1.9` checkout. Confirm current Git and installed identities at session start; do not use October pending-work notes as present status.

## Completed starting point

Feature restoration, bounded persistent inference caching, all four Echo QoL requests and the findings-only self-monitor are deployed. The final recorded check had 14 services healthy/running and idle. See [current build](../CURRENT-BUILD.md) and [exact runtime source mapping](../releases/1.9/runtime-source-manifest.json). Do not rebuild these features or replay historical tests merely to recreate prior evidence.

Four local edits excluded from deployment were preserved in the task workspace `runtime-publication-20261116/undeployed` and `undeployed.patch`: Discord queue controls, Discord activity, Discord access, and tool-registration build. Published versions match installed source. Review those preserved proposals independently before adopting them; do not silently reintroduce them as runtime fixes.

## Next sessions

1. Observe a complete real request through delivered response. Record first-token and total time, queue wait, prompt preparation, cache restore/prefill, generation, tool execution, tool-result-to-next-model-request gaps, compaction and delivery. Inspect existing logs and monitor findings before adding instrumentation.
2. Use the same base model as a control on the same machine. Compare equivalent inputs and settings; distinguish warm, cold and `/new` cases. Do not blame context length without measurements or lower reasoning to manufacture a speedup.
3. Fix the largest demonstrated source of avoidable delay with existing systems. Avoid extra model calls, duplicate memory systems or keyword-routing substitutes. Preserve full useful context and task-appropriate reasoning, including substantive compaction.
4. Validate the changed path with the original case and a small set of varied tasks. Compare end-to-end results and privacy/correctness, not only first-token speed. Deploy only the qualified change, then leave the full stack running and idle.

## Boundaries

Do not unmount the external drive or move data off it. Keep caches, receipts, logs and monitor state bounded. Preserve public-community versus private DM/web access and explicit account ownership. The monitor produces findings only; it does not automatically submit RSI changes. Keep cancelled/held requests held and never resume old test spam. Do not interrupt a live user conversation for diagnostics.

This publication changed repository source/documentation only; it did not restart production. Existing evidence is dated, and no broad latency improvement is claimed from synthetic controls. Source publication excludes models, credentials, conversations, private prompts and live configuration.

## Publication method

Use the connected GitHub Git-object tools when shell Git credentials are unavailable. Publish the exact local source tree as a fast-forward from the current remote release, verify the tree hash, and update the default landing-page branch. Retain the original local development commits; connector publication must not discard or rewrite them. A shell authentication failure alone does not establish that GitHub publication is blocked. No production restart is needed for source/documentation publication.
