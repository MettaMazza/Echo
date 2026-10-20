# Echo 1.75 release record

**20 October update:** r35 preserves reasoning, memory and native audio. Five consecutive warm browser turns had a 17.7-second median (12.3–54.1 seconds), versus the historical short-turn median of 13.3 seconds. Latency remains inconsistent; the tested Q8 candidate was not promoted. See [measurements and limits](latency-oct20.md). Earlier deployment statements below are historical.

**Latest r29 deployment / r27 session replay:** see [r27 session replay](session-replay-r27.md). The owner-history replay exposed an audio-capability denial and high end-to-end latency after the earlier r24 checks. r27 preserves native reasoning and Warm, corrects capability/presence context, and separates interactive/background model work. Measured replies still take tens of seconds; the earlier first-output figures are not complete-turn latency.

Release source is based on the isolated fec6ab7 runtime and a5ca473 browser overlay, plus the committed changes in this release. Publication preserves GitHub main ancestry; local experimental development has a separate history. The release does not deploy the known-crashing newer guardian.

## Architecture additions

`media/video_request.ep` and `media/video_tool.ep` prepare the same bounded time range as video and mono audio. `agent/run_messages.ep` supplies both native inputs. `interaction/watch_analysis.ep` advances coverage only after an actual matching request and successful publication. `web/watch_schedule.ep` schedules played ranges, retains recent observations, recovers checkpoint admission and rejects pause/end races. `interaction/watch_party.ep` owns durable revisioned controls. `media/video_library.ep` checks explicit device grants and denies connector-origin sessions. `ui/watch_party.ep` provides the inline/fullscreen viewer and actual progress display.

`echo_speak.ep` is an Ernos native helper using the installed upstream MLX Audio/Qwen TTS library through the existing Python C API dependency bridge. It loads a host-selected saved speaker vector, uses the accepted unpitched Warm audition and its matching transcript as acoustic context, retains the mixed speaker vector, and applies the selected pitch once after synthesis. The host must install a UTF-8 transcript beside the reference using the reference filename plus `.txt`. Without this sidecar the helper falls back to speaker-only generation; that fallback is not the qualified Warm setup. Sentence-wise generation resets seed105 at temperature0.3. Deterministic rules slow and slightly strengthen short statements, give questions/exclamations a longer pause, and mildly accelerate longer statements. These are phrase-level pacing/gain controls, not semantic word stress or unrestricted emotional acting. Sentence splitting is punctuation-based; unusual abbreviations or very long unpunctuated input need further qualification. This is experimental vector interpolation, not biological inheritance or a guarantee of instructed emotion control. The reference and vector remain private. Qwen Base does not implement arbitrary VoiceDesign instructions for clones.

## Build and configuration

Compile `app/server.ep` and `app/echo_speak.ep` with the pinned self-hosted Ernos compiler documented in the existing build guide. Emit the browser bundle from concatenated `app/ui/client.ep` and `app/ui/watch_party.ep`; omitting the latter omits the viewer. Keep helper processes isolated from web workers.

Configure `ECHO_VOICE_PROFILE` using `config/examples/voice-profile.json`; install its private conditioning and reference locally. `ECHO_VIDEO_LIBRARY_ROOT` points to a private catalog with schema `echo-private-video-library/1`; `ECHO_VIDEO_LIBRARY_DEVICES` is a JSON array of explicitly authorised local paired device IDs. Never include Discord identities. The synthetic test library is separate from production media.

Keep Omni's context at 65536. Preserve the prior provider, service profile, executable, UI and database backup for rollback. Promote only when idle. The old Qwen3.8 provider remains available separately; do not confuse an already-open browser's stale model selection with the newly selected default.

## Prompt migration correction

The live inherited identity contained contradictory instructions requiring literal Thought/Action reply_request output. The obsolete formatting block was removed; native tool schemas and ordinary final text are the only current transport. Do not merely append another overriding instruction while leaving the contradictory block intact. Preserve identity/values separately from transport edits. Three consecutive historical live Omni Instruct replies and reload persistence were checked after this repair.

## Evidence boundaries

The acceptance manifest records observed checks and known failures. Earlier errors are retained: repeated provider closings, a model claiming end after a status-only call, UUID transcription mistakes, unsupported browser codec, and the first TTS helper's missing WAV dependency. None is erased by a later successful test. No full-film soak, perfect temporal understanding, portable installation or completed Discord parity is claimed.

The requested **19 October morning, 06:00–12:00 Europe/London** baseline was recovered directly from the retained archive: 18 top-level runs and 40 timed requests, including 10 warm requests. Warm median dispatch-to-first-output was 1.864 seconds and generation 31.613 tokens/s. The earlier comparison used 18 October and is retained as historical evidence. Sixteen earlier r24 warm Thinking requests measured median first output2.3735s and decode49.8534tokens/s. Different prompts, context and contention prevent an unqualified speedup claim. See [baseline receipt](morning-baseline.json).

## Deployment status

Current deployment is r29 on the same verified Thinking checkpoint, with two 65,536-token slots and interactive priority. Warm is unchanged. The later owner-session regression, failed intermediate trials, final live checks and remaining latency are recorded in [session replay](session-replay-r27.md). The application was restarted idle against its original archive.

The earlier r24 application was promoted locally to the verified Thinking checkpoint, with Qwen3.8 retained for rollback and Instruct retired/stopped. Warm controlled-prosody conditioning, acoustic anchor and +1.25 pitch remain active. Candidate acceptance covers actual selected-WAV listening across multiple turns and reattachment/reload, independent fresh generated speech, tool receipts, cancellation recovery and bounded audiovisual watching. Final live-browser checks passed across four turns: selected WAV transcription/acoustics, ending follow-up, reattachment with changed instruction, unrelated arithmetic, Warm Speak and reload persistence. The app was then restarted with zero active runs and left on a new empty conversation. No private recordings or vectors are distributed. See the acceptance manifest for exact scope and retained failures.

## Reasoning and activity

The initial checkpoint was Qwen3-Omni-30B-A3B-Instruct (non-thinking), which did not satisfy the required reasoning mode. It has been retired and replaced with the separately pinned, verified Thinking checkpoint. The earlier Instruct ordinary-text runs had request/final events without reasoning deltas. Current Thinking reasoning and native tool call/result events were observed in the live activity panel. A request for progress messages is not proof the model produced them: the fifth release test requested two interim updates but returned only a final reply, so it is not counted as an interim-persistence pass.

## Voice profile changes

Saved-message speech jobs bind the installed voice-profile digest. Repeated playback within one profile reuses its saved audio; changing profiles creates a separate job without erasing old audio. Execution rejects a profile changed after admission. The historical regression test ran both real engines against one immutable message and checked repeat reuse, switch-back and foreign-device denial.

## Long-speech correction

Two native passages of approximately 70 and 72 seconds were checked with local ASR and waveform inspection. Both had no clipping; the second matched the script after punctuation/case/spelling normalization, and focused ASR resolved two inflection differences in the first. This does not guarantee future utterances are error-free. The earlier single-utterance acoustic-anchor attempt inserted repeated text and was rolled back. Higher-temperature auditions changed words and were not deployed. A fresh browser Speak check used the controlled-prosody helper successfully; its audio card persisted after reload. Fresh Warm browser synthesis/repeat cache and independent Thinking listening/follow-up passed; the native profile-switch test retains its original scope.
