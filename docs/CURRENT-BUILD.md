# Current deployed build — 16 November 2026

The installed release is **`echo-self-monitor-20261116-findings`**. This publication reconciles application source with that release; it does not rebuild or restart Echo. The [runtime source manifest](releases/1.9/runtime-source-manifest.json) records exact hashes and installed-to-repository mappings, including the monitor Python helpers installed under `app/monitor` from `tools/monitor`.

## What is deployed

- Browser and Discord conversations, public community recall, explicitly linked private DM/web continuity, source-linked memory, tasks, tools, search and local media.
- Restored optional native OpenAI Responses/Anthropic integration, private Gmail/Contacts workflows, generated-tool qualification and lifecycle, and the private research HUD. Credentials, personal authorization and account-specific live checks remain installation-specific, not missing source implementations.
- Callback-safe SQLite handling, durable journal-backed permissions/message reads, stable memory prefixes, foreground completion ordering, bounded storage retention and scoped unattended administration.
- Persistent primary/multimodal inference caching across server restarts, with bounded storage and compatibility checks. Full context capacity is retained.
- All four Echo-requested QoL features: scoped working note, recall awareness, change notices and disclaimer awareness.
- Findings-only self-monitor: a separate idle-time model call reviews scoped settled-run evidence. Echo can act on its findings through existing tools; the monitor does not submit RSI changes or deploy automatically.

See [features](releases/1.9/FEATURES.md), [restoration](feature-restoration-20261116.md), [persistent cache](persistent-session-cache-20261116.md), [QoL](echo-qol-20261116.md), and [self-monitor](self-monitor.md).

## Recorded deployment verification

The 16 November deployment check recorded **14 services running and idle**, ready web service, healthy primary/multimodal/embedding endpoints, no active inference slots, tools, claimed Discord jobs, sending messages or speech connections. Three deliberately held Discord requests stayed held; they were not replayed. The monitor had no pending reviews. These are dated observations, not a perpetual health guarantee.

Monitor validation included 14 Python tests, native scope/publication/context checks, rejection of invalid citations, revocation and deduplication checks, a clean review and an evidence-linked finding for a synthetic 40.8-second gap. Fixed release acceptance passed. Cache validation included process-restart reuse with a tiny isolated model; QoL acceptance covered scopes, bounds, flags and restart behavior. This is specific acceptance evidence, not a claim that every repository test ran during publication.

The synthetic foreground model control took 1.266 seconds alone and 1.085 seconds during monitor work. These controls are not Discord end-to-end latency measurements. No general latency resolution is claimed.

## Next work

The next sessions focus on **latency reduction and optimisation**. Measure complete real requests and isolate queueing, prompt preparation, prefill/cache restoration, generation, tool-result-to-next-request gaps, compaction and delivery. Preserve reasoning appropriate to the task, useful context, memory/tool behavior and privacy boundaries. Start with the [handoff](phase2/RESUME-HANDOFF.md).

Old October pending-work claims are historical. They do not reopen restored or completed implementations. V1 remains a separately named production release; this update is not a new V1 announcement.
