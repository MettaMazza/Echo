> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Interactive inference reservation — 17 October 2026

The previous cache/scheduling repair did not establish that ordinary fresh conversations were responsive. Its 14/18-second browser checks used exact-marker replies without tools. They missed contention for admission to the four inference slots.

## Observed failure

The reported fresh conversation dispatched its first main request 1.315 seconds after submission. The inference response headers arrived at 109.528 seconds and its first model token at 141.697 seconds. Thus about 108 seconds elapsed before admission, followed by 32.187 seconds of prompt processing. Provider timings show 169,567 cached tokens reused and 2,211 new tokens processed. The problem was not a completely uncached new session.

Three old workloads still occupied inference slots. Automatic memory extraction, launched alongside the new conversation, occupied the remaining capacity before the user request. The three obsolete jobs and their old parent were subsequently cancelled through the authenticated application endpoint; archived work was retained.

## Change

The local inference server recognizes `echo_background` scheduling metadata. With multiple configured slots, background requests cannot consume the last idle slot. Interactive requests can use it. Deferred interactive requests are admitted before deferred background requests. Interactive prompt preparation also takes precedence over background prompt preparation; already-generating requests retain their measured GPU-time share. The limit derives from the configured slot count; the installed four-slot service allows at most three simultaneous background requests. Single-slot configurations remain functional.

The Ernos provider adapter labels delegated-session requests and automatic-memory extraction as background. This includes compaction requests within delegated sessions. Ordinary interactive conversation requests remain unchanged. This metadata is added only for the local llama.cpp adapter. Context capacity, generation limits, prompts, memory content and model weights are unchanged.

Apply `llama-b11429-interactive-reservation.patch` after the existing longest-prefix and single-recurrent-prefill patches. The server schema, task parameters, slot admission and deferred queue are changed together.

The greeting also triggered `system_inspect`, whose result repeated all 69,265 characters of the system message verbatim. The request composer now replaces only this exact duplicate with an explicit reference and SHA-256 digest of the system text already in that request. The canonical tool event is unchanged. Different historical architecture text, missing current system text and all dynamic status fields remain intact. No unique information is removed.

## Verification

- Existing Ernos compiler built the application; no compiler changes.
- `tests/context/test_system_observation_dedup.ep` passed: exact duplicate projection, dynamic fields retained, changed or absent current system prompt leaves original observation unchanged.
- `tests/providers/test_inference_admission.ep` passed: ordinary user request unchanged, delegate and memory requests classified as background.
- Actual loaded model: three background streams ran concurrently. A fourth background request stayed queued. An interactive request used the available slot, produced its first token after 0.225 seconds and completed after 0.603 seconds. No background request needed to finish first. Test connections were closed afterward.
- A reservation-only browser check still took 102.969 seconds to its first model token: 7,511 new prompt tokens competed with agents loading context. That candidate was not accepted as the latency fix. The final patch therefore also prioritizes interactive prompt preparation.
- On the final engine, three long background prompts ran while a fourth remained queued. An interactive request containing roughly 3,000 new prompt tokens reached its first token in 7.563 seconds and completed without a background request finishing.
- Full recalled-context browser verification is recorded below when completed; the short admission test does not establish full-context chat latency.

Local evidence is retained under the runtime `maintenance` directory: `reservation-admission-evidence.json`, `reservation-admission-test.log`, `reservation-warm-evidence.json`, and `reservation-browser-evidence.json`. These are private operational evidence, not conversation data published in the repository.

## Final browser observations and handback

Both ordinary messages completed through the deployed LAN HTTPS browser, with no page errors and valid canonical final-event digests. The greeting used the exact user example, “Hello Echo, good morning, how are you?” The follow-up asked what to work on together. These were not forced-marker replies.

| Message | First model token (including reasoning) | First visible answer text | Complete |
| --- | ---: | ---: | ---: |
| Fresh greeting | 12.076 s | 71.168 s | 99.234 s |
| Follow-up | 17.926 s | 194.456 s | 219.515 s |

The reservation and prompt-priority defects are repaired. These figures do **not** establish that all user-visible response latency is resolved. The later delays include model-generated reasoning and multiple tool steps; the follow-up also delegated work despite being phrased as a question about possible work. That behavior is recorded, not counted as a performance success. No reasoning/output caps or identity-prompt changes were introduced to make the test pass faster.

The live `system_inspect` canonical receipt retained its complete architecture text; the subsequent model request contained the 1,461-character dynamic observation/reference instead of the duplicate system text. The full system message remained present.

Two full research agents launched by the earlier smoke-test conversation were cancelled: that objective explicitly deferred full work until confirmation. All diagnostic runs, their spawned work and temporary browser access were stopped/revoked. Final checks found zero active runs and all four inference slots idle. Echo and its inference/gateway services remain running. Development is paused at this deployed point.
