# Live latency and compaction repair, 17 October 2026

## Confirmed causes

The saved SFT and transcript agents repeatedly reviewed a rejected recap, then used a deterministic repair helper which appended review criticism without removing the rejected claims. The candidate digest changed, so digest-cycle detection did not stop the loop. There were 15 and 17 saved candidate cycles respectively; candidates had grown to 66,613 and 53,664 characters. This kept expensive background inference active without advancing the tasks.

`recap_repair_provider` now generates a complete replacement from the original sources, rejected candidate and review findings. The append-only helper and its misleading test were removed. Activation still requires the existing structural and independent semantic review. Original source records are never replaced by a recap.

The live recovery also demonstrated overlapping memory-extraction requests for the same owner. A native, nonblocking account/group scope lock now permits one extraction owner. Other conversations defer derived extraction and retain their pending original memory context. Kernel lock release allows recovery after a worker dies; pending receipts are reread under ownership. This does not serialize foreground model requests or impose a context or output limit.

## Storage and recovery

At inspection there were no live Echo app, inference or gateway processes; archived worker statuses were stale. Before restart, the archive was backed up and 85 original/tool/recap receipt identities and digests were recorded. All 85 matched after initial recovery.

The model plus vision projector occupy approximately 176.14 GiB. Cold reads from One Touch sustained roughly 110–125 MB/s. A persistent internal SSD runtime cache now holds verified copies, while the master model remains on One Touch. All six model shards matched their manifest SHA-256 values. The model reached healthy status in 15.09 seconds from the cache. The original model identifier remains the API alias, preserving existing provider bindings.

Four inference slots retain 262,144 tokens each; ordinary generation remains uncapped by this change. The RAM prefix cache and existing native scheduling patches remain enabled. The additional native patch is described below.

Starting every saved workload cold at once reproduced long queueing: two large recap reviews, an existing conversation, and a memory pass occupied all slots; another overlapping memory pass was next in line. Recovery therefore warms the full conversation request once before resuming the app workers. A one-token maintenance probe warms the complete input; it is not a user output limit or evidence of end-to-end reply latency. Full browser replies are measured separately below.

## Shared cache selection defect

The live browser test exposed another concrete cause. The engine checked its shared RAM cache only when replacing enough of a slot's existing prefix. An idle slot holding a short, fully matching prefix therefore bypassed a much longer cached prefix. The cache's own comparison also required both a greater retained fraction and a greater match length, so a 100%-matching short prefix could defeat a much longer partially matching entry.

`engine/patches/llama-b11429-longest-prefix-cache.patch` checks the shared cache for each cache-enabled completion assigned to an idle slot and compares actual common-prefix token counts. It leaves busy slots alone, uses the existing cache restore/clone/eviction paths, and does not change model weights, context size, or generation limits. Apply it after the existing prefix-publication patch and current scheduling patches. A regression is included in the vendor's existing completion test file.

On the actual local model, the same three-request regression (short prefix on slot zero, longer prefix on slot one, longer continuation on slot zero) measured:

| Engine | Reused tokens | Newly processed tokens | Request wall time |
|---|---:|---:|---:|
| Before | 837 | 6,508 | 13.09 s |
| Final replacement | 7,341 | 4 | 0.18 s |

This regression uses one output token to isolate cache behaviour; it is not the end-to-end UI acceptance result. The final engine is `llama-cache-selection-v2-20261017`; the app is `phase2-recap-rewrite-20261017`.

## Concurrent recurrent prompt batching

The first full browser run after the cache selection fix completed both exact marker replies but failed latency acceptance: 79.19 and 77.19 seconds. Both requests reused about 169,000 tokens. Native timings isolated 37.37 and 50.20 seconds of prompt work for only 697 and 699 new tokens, while large background prompts were active. These are failed performance measurements, not evidence that the fix was complete.

`engine/patches/llama-b11429-single-recurrent-prefill.patch` keeps unequal recurrent prompt sequences in separate batches and gives a continuation which fits the current prompt quantum a chance to finish before another long-prompt quantum. Long prompts retain rotating admission; generation retains measured device-time sharing. All four slots and their complete contexts remain available. Apply this patch after the longest-prefix cache patch. The replacement engine is `llama-single-prefill-20261017`.

The final browser test used the real LAN HTTPS endpoint, normal pairing/composer/Enter submission and the existing owner account. The original conversation and both saved agents were active. It rendered the exact requested replies and retained them as completed archive events:

| Live browser request | Before scheduler correction | Final build | First visible answer, final build |
|---|---:|---:|---:|
| New conversation | 79.19 s | 14.04 s | 13.04 s |
| Follow-up | 77.19 s | 18.04 s | 15.04 s |

The final request prompt work was 4.02 seconds for 953 new tokens and 8.65 seconds for 698 new tokens. Both retained the full approximately 170,000-token context. These are two observed short diagnostic replies under load, not a promise that arbitrary research tasks complete in these times.

Both completed event digests were independently recomputed using the archive's length-prefixed `echo-event-v1` contract and matched. All 85 preserved original/tool/recap receipts still matched. Browser JavaScript recorded no errors. Screenshots showed the final replies and a ready composer. All temporary verification browser credentials were revoked.

The original agents remain active; this verification does not claim that their large research tasks have finished. Their compaction repair implementation separately passed the real-model correction/review regression.

## Verification

- Real local-model recap regression passed: false deletion and completion claims were replaced, the exact source marker was retained, a real repair model request was archived, and independent source review passed.
- Extraction coordination regression passed: a concurrent same-owner extraction deferred before provider access, pending original text remained accessible, and ownership was reacquired after release.
- Server compiled successfully using the existing Ernos compiler. Existing compiler warnings remain; no compiler changes were made.
- Live browser and archive verification passed with the measurements above. Test browsers and maintenance drivers exited; the app, gateway, inference engine and preserved user work remain running.

Temporary maintenance evidence remains in the host runtime maintenance directory and contains private request/archive data. It is not intended for public Git history.
