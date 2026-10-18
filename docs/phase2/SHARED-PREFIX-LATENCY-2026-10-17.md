# Fresh-session and concurrent-agent cache reuse

The fresh run reported after the memory-snapshot fix reached its first model request within about one second. Its initial inference nevertheless processed 155,481 uncached tokens for 260.464 seconds. Later tool-step prompts reused the cache, but generated output also consumed time. The large delegated tasks were intentional.

Two additional cache problems were identified:

1. Unique child routing metadata preceded the shared recalled memory. A child therefore diverged from its parent before the large reusable prefix. The routing record now follows shared memory and still precedes the delegated objective. Memory content is not removed or shortened.
2. The llama.cpp server only saved prompt state into its cross-slot RAM cache when replacing enough of a slot's previous prompt. A busy parent could therefore have a warm context that new agents on other slots could not restore. The vendor patch publishes completed prompt state using the existing prompt-cache implementation before the slot is reused. It preserves the configured context capacity, scheduler, checkpoint mechanism and cache budget.

The application change is in `app/context/session_messages.ep`. The vendor change is `engine/patches/llama-b11429-publish-prefix-cache.patch`, applied after the existing fair-prefill, periodic-checkpoint and recurrent-batch patches. It changes the existing completion callback and cache restore operation, using native cache allocation, serialization, eviction and prompt cloning. Cache restores retain their entry for other slots instead of moving and erasing it. The configured RAM budget still controls eviction.

The routing regression covers both private and group delegation, routing order, source isolation for groups and later authority revocation. The older delegation fixture assumed private children could not automatically recall their own owner's earlier messages; that assumption no longer matches the cross-session memory design. Its unrelated private-memory assertion was not used as evidence for this cache fix.

Sequential replay of the recorded parent and child request shapes after ordering the common prefix reused 160,801, 151,513 and 155,609 tokens respectively. Diagnostic request durations were 0.91, 13.80 and 8.00 seconds. These requests asked for one output token to measure prompt processing; this does not change ordinary output limits and is not a measurement of full response generation.

The first concurrent experiment caught destructive cache reads: one child restored the entry and the next child missed it. That candidate was stopped and not deployed. The corrected candidate restored the shared prefix into both children while their parent was active; the children returned distinct exact marker answers in 1.83 and 1.94 seconds, reusing 6,404 tokens each. This small correctness check is supplemented by the full-context production measurement, not substituted for it.

## Full-context production verification

The deployed prompt and all 54 tool definitions were used. After the one-time restart warm-up (161,916 tokens), parent and both children ran concurrently on distinct slots.

| Request | Cached tokens | Newly processed tokens | Prompt processing | Diagnostic wall time |
|---|---:|---:|---:|---:|
| warm-parent | 161,912 | 4 | 0.071 s | 31.34 s |
| child-one | 153,049 | 4,975 | 17.395 s | 19.25 s |
| child-two | 153,049 | 6,346 | 23.942 s | 28.78 s |

The parent generated 64 diagnostic tokens; each child generated one. The parent prompt processed in 71 ms, but its 64-token generation took about 30 seconds while the children prefetched their remaining context. Shared hardware contention is visible in this measurement; these results do not establish identical generation speed under load and at idle. Context and ordinary output limits were not reduced.

After the child requests completed, the same full-context parent generated 64 diagnostic tokens in 3.83 seconds total, with 96.5 ms prompt processing.

Deployed app: `phase2-shared-prefix-20261017-091954`. Deployed inference: `llama-shared-prefix-v2-20261017`. The production cache is left warm. Broader development remains paused.
