# Attachment repair — 16 October 2026

The temporary image-routing workaround described below was subsequently replaced by one multimodal default model. See [UNIFIED-MULTIMODAL.md](UNIFIED-MULTIMODAL.md). Thumbnail and removal changes remain active.

The failing image submissions reached mlx_lm.server._tokenize, which rejects image content with “Only text content type is supported.” Retry reused the same text-only provider, so it could not resolve the failure.

Echo now checks typed image content before entering the agent loop. For an MLX text-only selection, a host-configured local llama.cpp vision profile is checked against live provider modality metadata and recorded in a separate immutable run_vision_profile event. The selected profile remains intact. Retry uses the recorded route; no cloud fallback or hardcoded model identity is introduced. The active route appears in the thinking trace and system inspection. An absent route gives a specific actionable status. Unsupported content is rejected before the MLX tokenizer helper.

Images now have authenticated, hash-verified thumbnails in saved messages and the attachment picker. Newly uploaded files are selected automatically. A visible composer strip shows pending files even with Files & voice collapsed. Remove deselects and removes a file from that browser's picker; previously sent archive originals remain intact. This is attachment selection/removal, not permanent erasure of historical files. Open and Download controls use compact labels.

Validation: self-hosted server and live retry test builds passed; emitted browser JavaScript passed syntax checking. A real image uploaded through the HTTP API, using the default MLX selection, completed through the local vision route with “Left: red. Right: blue.” The native test_live_image_route_retry test paused without a vision profile, then completed the SAME run after configuration repair, preserved the original binding and verified its archive. A mobile-width browser check verified thumbnails decoded to 128 by 96 pixels, remained visible with the picker collapsed, and disappeared from pending selection after Remove. Historical image thumbnails also rendered. No compiler/runtime modifications.

Private evidence: attachment-fix-live.json, attachment-fix-build.log, image-route-retry.log, image-route-retry-build.log. Host configuration: ECHO_VISION_PROFILE points to environments/vision-profile.json. Restart the supervisor after changing environment bindings. The browser needs a refresh to load the revised JavaScript.
