# Echo 1.75 release record

Release source is based on the isolated fec6ab7 runtime and a5ca473 browser overlay, plus the committed changes in this release. Publication preserves GitHub main ancestry; local experimental development has a separate history. The release does not deploy the known-crashing newer guardian.

## Architecture additions

`media/video_request.ep` and `media/video_tool.ep` prepare the same bounded time range as video and mono audio. `agent/run_messages.ep` supplies both native inputs. `interaction/watch_analysis.ep` advances coverage only after an actual matching request and successful publication. `web/watch_schedule.ep` schedules played ranges, retains recent observations, recovers checkpoint admission and rejects pause/end races. `interaction/watch_party.ep` owns durable revisioned controls. `media/video_library.ep` checks explicit device grants and denies connector-origin sessions. `ui/watch_party.ep` provides the inline/fullscreen viewer and actual progress display.

`echo_speak.ep` is an Ernos native helper using the installed upstream MLX Audio/Qwen TTS library through the existing Python C API dependency bridge. It loads a host-selected saved speaker vector, uses speaker-only synthesis with no reference text/code replay, and applies the accepted pitch/tempo adjustment. This is experimental vector interpolation, not biological inheritance or a guarantee of instructed emotion control. The reference and vector remain private. Qwen Base does not implement arbitrary VoiceDesign instructions for clones.

## Build and configuration

Compile `app/server.ep` and `app/echo_speak.ep` with the pinned self-hosted Ernos compiler documented in the existing build guide. Emit the browser bundle from concatenated `app/ui/client.ep` and `app/ui/watch_party.ep`; omitting the latter omits the viewer. Keep helper processes isolated from web workers.

Configure `ECHO_VOICE_PROFILE` using `config/examples/voice-profile.json`; install its private conditioning and reference locally. `ECHO_VIDEO_LIBRARY_ROOT` points to a private catalog with schema `echo-private-video-library/1`; `ECHO_VIDEO_LIBRARY_DEVICES` is a JSON array of explicitly authorised local paired device IDs. Never include Discord identities. The synthetic test library is separate from production media.

Keep Omni's context at 65536. Preserve the prior provider, service profile, executable, UI and database backup for rollback. Promote only when idle. The old Qwen3.8 provider remains available separately; do not confuse an already-open browser's stale model selection with the newly selected default.

## Prompt migration correction

The live inherited identity contained contradictory instructions requiring literal Thought/Action reply_request output. The obsolete formatting block was removed; native tool schemas and ordinary final text are the only current transport. Do not merely append another overriding instruction while leaving the contradictory block intact. Preserve identity/values separately from transport edits. Three consecutive historical live Omni Instruct replies and reload persistence were checked after this repair.

## Evidence boundaries

The acceptance manifest records observed checks and known failures. Earlier errors are retained: repeated provider closings, a model claiming end after a status-only call, UUID transcription mistakes, unsupported browser codec, and the first TTS helper's missing WAV dependency. None is erased by a later successful test. No full-film soak, perfect temporal understanding, portable installation or completed Discord parity is claimed.

The requested **19 October morning, 06:00–12:00 Europe/London** baseline was recovered directly from the retained archive: 18 top-level runs and 40 timed requests, including 10 warm requests. Warm median dispatch-to-first-output was 1.864 seconds and generation 31.613 tokens/s. The earlier comparison used 18 October and is retained as historical evidence. The fresh Thinking comparison is pending; different prompts, context and contention prevent an unqualified speedup claim. See [baseline receipt](morning-baseline.json).

## Deployment status

Source and documentation are published; runtime qualification remains incomplete. The r14 application is installed locally with Qwen3.8 reasoning, voice-profile-aware speech caching, and explicit unsupported-media notices that keep text turns responsive. Three actual Qwen3.8 turns passed, including an attempted audio listen and follow-up. The currently selected voice is Warm hybrid at +1.25 semitones, explicitly selected from auditions based on two private recordings and a synthetic Warm character; native synthesis and the matching Desktop audition were verified. Fresh browser speech on this selected profile remains pending. Earlier browser speech tests remain evidence for their recorded versions, not fresh Thinking acceptance. A subsequent explicit request to activate the selected voice superseded the temporary audition hold. Final release remains pending replacement of the initial Instruct checkpoint with the required Thinking checkpoint and fresh acceptance. Preserve audition originals and reproducibility assets. No private recordings or vectors are part of the release.

## Reasoning and activity

The initial checkpoint was Qwen3-Omni-30B-A3B-Instruct (non-thinking), which did not satisfy the required reasoning mode. It is being replaced with the separately pinned Thinking checkpoint. Observed ordinary text runs have request/final events without reasoning deltas; actual native tool runs show call/result events in the browser activity panel. A request for progress messages is not proof the model produced them: the fifth release test requested two interim updates but returned only a final reply, so it is not counted as an interim-persistence pass.

## Voice profile changes

Saved-message speech jobs bind the installed voice-profile digest. Repeated playback within one profile reuses its saved audio; changing profiles creates a separate job without erasing old audio. Execution rejects a profile changed after admission. The regression test ran both real engines against one immutable message and checked repeat reuse, switch-back and foreign-device denial.
