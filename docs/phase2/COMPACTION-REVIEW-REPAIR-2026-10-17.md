# Compaction review repair — 17 October 2026

## Observed failure

Job `compaction:d622f007-6b8c-49b1-bbbe-4cb6789fb8e0` repeatedly rewrote its recap. Reviews could mark all 62 original records covered yet include positive commentary in `findings`. The old validator interpreted every nonempty finding as a repair requirement. The user turn was subsequently cancelled; this repair does not restart that cancelled turn.

## Implemented repair

Findings now explicitly distinguish supported observations from defects and unresolved uncertainty. Supported observations require an empty correction. Defects and uncertainty require a concrete correction and continue to prevent activation. Invalid findings fail validation. Source coverage, candidate digest, original evidence, attachment revision and checkpoint activation checks remain in place. The reviewer is instructed to emit concise covered verdicts and avoid redundant positive commentary.

Saved whole-source repair reports using the old untyped contract are reviewed again against the saved candidate instead of immediately causing another rewrite. Historical archive rows remain unchanged. Fragment and synthesis prompts and validators accept the same typed finding contract.

All local compaction requests now use background inference scheduling, including fragment and synthesis requests. Interactive inference can take priority. No model context, output, source evidence or memory limits were reduced.

## Verification

- Native `test_recap_findings`: passed positive/negative/malformed/legacy findings, omitted-source rejection, fragment review, background scheduling and idempotent request serialization.
- Replayed the actual 62-record sources and last complete candidate through the actual local model. Result: `review-pass`, all 62 covered, no findings, no rewrite. Wall time 124.369 seconds; 439 generated tokens. Engine timing: 41,963 cached tokens, 7,043 new prompt tokens, 74.095 seconds prompt processing and 48.351 seconds generation, alongside the two real agents.
- Actual-model semantic check with the new request format: faithful paraphrase accepted; invented deletion rejected. Evidence: `/private/tmp/echo-recap-semantics-v2-r2-20261017.sqlite`.
- Native automatic compaction fixtures passed normal activation and rejected-candidate repair, same-run continuation, archive verification, and durable reopen. Evidence: `/private/tmp/echo-review-v2-pass-r3-20261017.sqlite` and `/private/tmp/echo-review-v2-repair-r3-20261017.sqlite`. These use scripted model judgments and counts, not live-model latency evidence. Their old transport expectations were updated for the three-message cached-source format and runtime routing records.
- A direct live-engine interactive priority probe during review and agent work returned first content in 0.115 seconds and completed in 0.212 seconds. This is not full browser/chat latency acceptance.
- Live replay evidence: `/private/tmp/echo-recorded-review-v2-20261017.sqlite`. Runtime receipts are under `maintenance/recap-review-*`.

## Deployment and limits

Deployed `phase2-recap-review-20261017`, server SHA256 `64f876502f3a861618e63bd671c97141d76c2d3c64268649212bc109229aac6c`. Authenticated identity and session-list APIs returned valid JSON successfully. Engine remained running. Existing audit worker processes were preserved on their earlier executable; new work uses the new release. Original archives were not rewritten. Test inference has finished.

This fixes the evidenced false rewrite loop and compaction priority. Full-source processing still takes time; this is not a claim that every latency issue or Phase Two is complete. The cancelled real turn was not revived. No visual browser pass was performed.
