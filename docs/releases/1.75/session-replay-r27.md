> **1.9 documentation:** [Current update](../1.9/README.md) · [feature status](../1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Session replay repair — r27

The later owner-session failure reopened the r24 audio-capability and latency acceptance. Earlier isolated successes did not cover this sequence with the owner's accumulated context. r24's blanket completion wording must not be read as universal reliability.

## Reproduction and diagnosis

The original five-turn conversation was replayed through the actual native Echo web UI against a private backup of the owner archive. The production archive was stopped idle and was never replaced by the test archive. The original invitation had no waveform yet: the correct response was to invite attachment, not deny all audio capability.

The original greeting took 112.346 seconds from archived submission to final reply. Its foreground request reused only three prompt tokens and processed 43,624 new tokens; the single model slot also served background memory extraction. Previously reported warm dispatch-to-first-output timings excluded memory preparation, waiting and completion. Those figures are not end-to-end user latency.

## Changes

- Verified audio capability is now a stable prefix on text and audio requests, including invitations. Unknown or false capability remains unchanged. Original identity and historical messages are preserved.
- A current-input receipt at the end of each conversational request states whether actual nonempty native waveform parts are present. It does not create an attachment or turn historical speech into an instruction. Memory-extraction and compaction requests are excluded.
- Current-turn guidance asks for proportionate deliberation without imposing a reasoning-token cap or disabling reasoning.
- The same verified Thinking model uses two slots with existing interactive priority. Background memory work can be suspended while interactive processing runs. Each slot retains the model's native 65,536-token context. The total two-slot allocation is not a larger per-conversation context. Instruct stays retired.

## Observed results and limits

The r27 replay returned an upload invitation instead of the text-only denial. The actual selected Warm waveform was supplied and its words were transcribed correctly. A subsequent ending question was answered correctly. Reattachment and an unrelated arithmetic question received new, relevant answers; the model combined a short introductory phrase with the following clause using a colon, so the requested first-two-sentence boundary was not strictly preserved. Exact punctuation is not an audio measurement.

The first greeting took 50.535 seconds; subsequent original conversational turns took 33.342, 67.908, 24.641 and 31.566 seconds. The actual audio transcription/description took 57.147 seconds, compared with 211.464 seconds in the earlier failed-performance r26 replay. The ending follow-up took 24.239 seconds; reattachment 36.920; unrelated arithmetic 28.898. These are single observed runs, not controlled statistical benchmarks. Later-turn context and sampling differ. There is still substantial context preparation and model deliberation; this repair does not promise instant responses or eliminate all latency.

The model also incorrectly agreed that its replay greeting had said “good morning” and introduced unsolicited ethics commentary. Those conversational-quality limitations remain recorded. No canned answers, coached acoustic preference or forced gender description were used.

Unit tests cover stable prefix, presence versus absence, explicit unsupported capability, idempotence, preserved waveform data, malformed/ambiguous JSON and large native-media bodies. The application and native test compiled successfully. The exact current Warm WAV hash was verified privately on every audio-bearing request. Public documentation excludes recordings, vectors, transcripts and private request bodies.

Deployment and final reload/cancellation checks are recorded in `session-replay-r27.json`; do not infer them merely from compilation. Wider Phase Two/Discord work and full-film soak remain outside this repair.

## Live deployment check

The same build was installed against the original production archive. A no-attachment invitation correctly requested an upload (49.614 seconds including the first new production prefix load). The exact selected Warm recording was then transcribed with all words intact and an acoustic description (63.607 seconds). The original archive, selected Warm profile and device permissions were preserved; the copied replay archive was not promoted.

An additional explicit attachment-list request printed a JSON tool envelope in the final reply instead of invoking the native tool channel. This is a failed tool-selection observation, not an executed call or a transport success. Existing native-tool instructions were present. Do not infer general tool reliability from the audio repair; the recovery outcome is recorded separately.

## r28 and r29 follow-up

r28 retains the same capability/cache repair and makes explicit that native audio presence does not waive user-requested tool actions. Native provider regression tests passed again. The repeated live attachment-list request then emitted an actual native tool call and received its durable result. The earlier printed-JSON attempt and verbal non-execution are retained failures, not retroactively counted as successes. Its post-tool final reply nevertheless repeated an earlier audio answer. r29 removes the competing imperative from the media receipt: waveform presence is source metadata, not a request to transcribe or describe it. The final r29 tool/audio follow-up and idle state are recorded in the accompanying receipt. Tool completion and model interpretation remain distinct.

Final r29 live sequence: targeted native lookup plus correct filename final 65.957 seconds (including a 15,827-token first-load prefill); audio ending follow-up 23.394 seconds; unrelated arithmetic 19.578 seconds. Reload preserved all replies. The application was restarted idle with the same Warm profile and original archive. These are observed full-turn timings, not a universal latency guarantee.
