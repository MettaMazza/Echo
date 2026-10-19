# Echo 1.75

**Installed and validated on the configured host; relaunched idle.** Echo uses Qwen3-Omni Thinking with the selected Warm voice. Source and acceptance records distinguish tested scope from retained failures and remaining Phase Two work.

Echo is a local conversational agent built in Ernos, with a durable conversation archive, native tools and a browser interface for Mac and linked devices.

This release adds native audio and combined audio/video input through Qwen3-Omni, a private video library, in-chat/fullscreen watching, and the host-enrolled Echo voice. It also carries the multi-turn interim-message persistence fixes, mid-task questions, free/task autonomy and source-authority protections.

## What you can do

- Talk with Echo across multiple turns, attach audio/images/video, and inspect original tool results and model traces.
- Watch supported episodes or movies in the chat. Choose continuous playback or checkpoints; both participants can pause. Whoever pauses speaks first. Echo can end watching while retaining the original media.
- See playback position separately from the range actually processed by Echo. Analysis uses bounded native audio/video segments, not a transcript-only substitute.
- Use a host-enrolled private voice for spoken replies. The default example is **Echo Warm hybrid, pitch +1.25 semitones**. The current host selection uses two user-supplied private recordings blended with a synthetic Warm character; its Desktop audition is verified. The active helper anchors to that accepted voice, generates sentence by sentence and applies controlled pacing/pauses rather than higher sampling randomness. Two native long-speech checks passed within their documented scope; browser Speak/reload and native cache tests passed; actual selected-WAV and fresh generated-speech Thinking multi-turn listening checks passed in the isolated candidate. Personal recordings and conditioning must be installed locally; they are not distributed here.
- Use `/auto` for free autonomy or `/auto <task>` for bounded work. The existing authority and source-isolation rules continue to apply.

The required release checkpoint is **Qwen3-Omni Thinking**, with explicit reasoning output and native multimodal input. The initial Instruct installation did not meet the reasoning requirement. Thinking artifacts are downloaded and hash-verified. Native four-turn reasoning/tools, selected-voice multi-turn browser listening, fresh speech, cancellation recovery and bounded combined audio/video checks passed in the isolated candidate. Instruct is retired and its service is stopped. The live application is on Thinking, and the final four-turn audio/speech/reload checks passed before the idle restart. The activity panel displays actual returned reasoning and tool events, never invented traces. See [upstream model variants](https://huggingface.co/Qwen/Qwen3-Omni-30B-A3B-Thinking).

## Tested scope and limits

The native model context is **65,536 tokens**. Movie duration is not bounded by a single context window: recent source-bound observations are carried between short analysis segments. This is not every-frame perception or perfect recall. Playback may buffer while Echo catches up. Current browser playback loads a whole verified media blob, so file size, memory, codec support and inference speed are practical limits.

Current **Thinking** acceptance includes a 24-second synthetic film processed through four bounded native audio/video segments, actual processed-through coverage, human pause/cancellation, reload position recovery and end with original retention. Earlier Instruct checks remain historical evidence only. Native tests cover ownership, cancellation, no lookahead, coverage gaps and checkpoint recovery. **A full feature-film soak has not been performed.** Models can misdescribe timing or claim an action without performing it; the durable tool/player state is authoritative.

This is a source release for a configured Mac installation, not a portable one-click installer. [Build and test](docs/BUILD-AND-TEST.md), [1.75 operations and acceptance](docs/releases/1.75/README.md), [architecture](docs/ARCHITECTURE.md), [security](docs/SECURITY-AND-DATA.md), and [complete source reference](docs/reference/APPLICATION.md) describe the implementation and its boundaries. Generated source indexes are lexical documentation, not a claim of complete test coverage.

## Continuing development

[Phase Two](phase2.md) remains open. Remaining Discord functionality and server setup, wider integration qualification, the protected improvement/research lifecycle and the final joint prompt review are **not declared complete by 1.75**. See [handover](docs/releases/1.75/HANDOVER.md). Unfinished guardian experiments are retained in the separate local development history and are excluded from this release.

Models, credentials, live identities, recordings, voice embeddings, conversations, private media and raw private evidence are excluded. Echo is maintained by [MettaMazza](https://github.com/MettaMazza). No project-wide software licence has been selected; upstream dependencies retain their own terms.
