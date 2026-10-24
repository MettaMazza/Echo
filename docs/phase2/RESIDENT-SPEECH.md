> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Resident speech worker

Status on 21 October 2026: native service and real synthesis acceptance passed; Discord now uses the supervised resident Warm worker. Host routing still uses its previously qualified helper and is not yet promoted.

The local service loads the selected TTS model and conditioning once, then serializes jobs through a private socketpair to the native Ernos worker. The acoustic reference, sentence seed, temperature, punctuation prosody and total pitch stay unchanged. Per-job Python dependency references are released after each job; the model remains owned by the worker. Python is used through the existing dependency bridge, not as application-server source.

`app/echo_speech_service.ep` owns worker startup, requests and reaping. `app/echo_speak_client.ep` is the short-lived helper invoked by the existing speech tool. `app/media/resident_protocol.ep` implements framing and rooted job validation. The `qwen3-tts-resident` profile supplies an absolute private `resident_config` path. Its local token and pinned worker-profile digest are never accepted from model text or published.

The service listens only on loopback. It accepts pre-created job directories under configured speech roots; traversal, nested destinations, control characters and wrong credentials are rejected. It does not accept replacement voice assets, models, commands or output destinations. The existing speech tool retains authorization checks before and after generation, private job creation, provenance, cache identity and final attachment validation.

A cancelled helper closes its connection. The service cancels and reaps the owned worker, then reloads on a subsequent request. It does not replay the interrupted job. Normal successful requests retain the worker. macOS supervises the service; initial Documents access required Maria's approval. Automatic restart was tested after that approval. Restart recovery includes a cold load and is not a warm-latency measurement.

The LaunchAgent must use `ProcessType=Interactive`: this socket service directly
serves user speech requests. Its first supervised configuration used Background,
which macOS documents as applying CPU and I/O resource limits. Matched testing
exposed that slowdown; the service was restarted while idle with Interactive
scheduling. The same short live script then took 2.835, 2.599 and 2.596 seconds,
and all three waveforms matched the earlier live output byte-for-byte. Preserve
this setting in installation and restart orchestration.

The same 447-word live story then generated in 64.425 seconds versus the earlier
151.360-second background-scheduled job. Both produced the identical 158.006-second
waveform. This is generation wall time, not first-audio streaming or Discord
delivery latency; long replies still require substantial synthesis time.

## Recorded acceptance

- Three fresh-process short trials: 4.554, 4.537 and 4.459 seconds wall time.
- Private-stdin resident jobs: 2.794, 2.610 and 2.565 seconds, after an initial 1.707-second load.
- Three local service/client short requests: 2.895, 2.483 and 2.519 seconds wall time.
- Longer six-sentence passage: 16.980 seconds fresh, then 12.216 and 11.979 seconds through the service.
- All matching short outputs were byte-identical; matching long outputs were also byte-identical. These are repeated-script checks, not a claim of accuracy for every future utterance.
- Invalid token, outside-root and traversal requests were rejected. Cancelling a long request produced no final WAV; the next request succeeded with the expected short-output hash.
- Native profile-switch acceptance passed: distinct profile jobs, cache reuse, switch-back recovery, foreign-device denial and archive verification.
- Native shared-message acceptance passed: one generation, both-member delivery, outsider/departure denial and archive integrity.
- Native process regression passed: explicit environment, descriptor isolation, exact exit status, output integrity, cancellation and reaping.
- The supervised service restarted from a new PID and generated successfully. The first post-restart request took 10.416 seconds, including remaining cold startup.

Live Discord short Speak generated a shared attachment and played to completion. Hiding and restoring it reused the original cached waveform without rewriting its output file. The same resident worker PID remained alive throughout. The real Discord request took longer than the isolated service benchmark: input-to-WAV file timestamps spanned about 6.9 seconds, and cached restoration took about 7.7 seconds from interaction receipt through channel delivery. These measurements include different work and must not be presented as the isolated 2.5-second result. Network authorization, disk I/O and upload overhead remain relevant.

The worker's observed RSS was approximately 4.8 GB after repeated short/long jobs; this is not an endurance result. Host cutover, extended memory soak and final installer/reboot acceptance remain open. These measurements do not close Phase Two's wider performance requirements or the separate conversational-model latency work.

Private artifacts contain the waveform, reference/profile information and full process receipts. Public receipts intentionally omit those assets and local authentication tokens.
