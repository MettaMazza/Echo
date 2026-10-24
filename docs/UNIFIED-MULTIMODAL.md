> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

> **Echo 1.75 update:** [Release record](releases/1.75/README.md) and [remaining work](releases/1.75/HANDOVER.md) supersede older model/voice/watch status below. Older qualification records remain historical.

# Unified local model correction — 16 October 2026

The downloaded Qwen 3.5 MLX checkpoint includes 333 vision-tower tensors and processor files. Its serving process was mlx_lm.server, which exposes text-only inference. This was a serving/integration mistake, not evidence that the checkpoint lacks vision. The subsequent automatic switch to another model for image messages was a workaround and has been removed for new runs.

The default is now the already installed Qwen 3.8 local llama.cpp profile. Its loaded projector handles images in the same model conversation as text and tools. The former text-only MLX profile is retired from new selections; its immutable profile and historical records remain for provenance. Browsers refresh into the active default when their saved selection has been retired. Existing in-flight runs are not silently rebound. A previously recorded historical routing event can still be replayed for that run; new image content cannot create one.

The host launch arguments, ECHO_PROVIDER_PROFILE and registry default all point to the same local model/endpoint. The loaded context is 262144 tokens. Provider metadata reports vision and video enabled, audio disabled. Video metadata is not claimed as a fresh end-to-end video test. Whisper transcription and Kokoro speech remain explicit local audio services; this checkpoint is not described as a native audio model.

Validation: self-hosted server/provider administration/native image test builds passed. Browser reload showed Qwen 3.8 local llama.cpp. A fresh live API conversation performed a text-only marker turn, an image turn requiring both pixel recognition and retained marker, then a multi-tool history_search/history_read follow-up. It correctly recalled CEDAR_8621 and red-left/blue-right. Every actual model request across these turns used one model identity, and no run_vision_profile event was created. Evidence: unified-model-live.json; unified-native-image.log; unified-model-build.log. These short-turn tests do not replace earlier full-context performance evidence.

During deployment a launch-profile/argument mismatch was caught by the server's validation; the mismatch was corrected and the supervised service restored before acceptance. The retained service logs record the failed start and recovery.

Phase 2 implementation has not begun.
