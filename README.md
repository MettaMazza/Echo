> Current implementation and measured verification: [October 2026 build status](docs/CURRENT-BUILD.md). Historical release evidence below is not a fresh full-suite claim.

# Echo — a local AI with lasting context and a community home

![Echo: local AI, lasting context, plain English](docs/assets/echo-cover.svg)

**Conversations that continue. Tools that do real work. A shared community with private boundaries.**

Echo is a self-hosted conversational agent built in **Ernos**, a plain-English programming language. It combines a browser interface and Discord with durable memory, native tools, local media generation, and inspectable reasoning and action records.

[**Join Echo’s Garden on Discord**](https://discord.gg/9QU2rXwGVF) · [**Explore the features**](docs/releases/1.9/FEATURES.md) · [**Read the 1.9 update**](docs/releases/1.9/README.md) · [**Build and run**](docs/BUILD-AND-TEST.md)

> **1.9 is a development update.** “V1” names the future production release, reserved for completion and production readiness of Phase Two. These development labels are not a claim that the V1 milestone has already shipped. [Current status and remaining work](docs/STATUS.md).

> **24 October maintenance handoff:** verified memory, admin-tool isolation and long-speech repairs are deployed. Current work is preserved for an OpenCode rebuild evaluation. [Deployment evidence, limits and rebuild requirements](docs/phase2/PIVOT-HANDOFF.md).

## Meet Echo

- **Keep the conversation going.** Start fresh sessions without losing authorized cross-session memory. Recall original messages in bounded batches, follow their provenance, and distinguish historical timestamps from the current clock.
- **Build a community, not isolated chat boxes.** Echo can recall permitted public Discord conversations across users and rooms, retaining speaker and session attribution. Private DMs and explicitly linked web histories have separate access rules.
- **Work together on real tasks.** Projects, files, checklists, schedules, delegated work, scoped command execution and code/Git tools support multi-step work. Follow-ups can steer an active task; completed foreground work releases the conversation while memory extraction finishes separately.
- **Create and share.** Send documents and images, request local image generation, receive attached results, transcribe supported audio and request spoken replies. The selected host TTS configuration is **Chatterbox Nano on CPU, eight threads, using the locally installed Echo reference**.
- **Research with sources.** Brave-backed search on configured hosts, readable web previews, retained originals and source-linked results keep research inspectable. A Brave API key is operator-supplied; the chat core does not require cloud inference.
- **See what happened.** Inspect model-emitted reasoning, tool requests/results, task state and conversation history. Archived records distinguish queued, running, completed and failed operations.

## Discord, beyond a bot reply

Echo supports mentions, replies, reactions, joined conversations, message history/search, projects, schedules, attachments and conversation controls. Administrative tools cover channels, threads, roles, members, invitations, scheduled events, expressions, AutoMod configuration, onboarding, guild settings and scoped audit reads. Actual actions remain limited by Discord permissions and the configured authority of the requester.

Use `/new` for a fresh conversation; `/history`, `/resume`, `/search`, `/branch`, `/edit`, `/regenerate`, `/retry` and `/cancel` manage continuity. `/attachments`, `/project`, `/schedules`, `/group`, `/share`, `/export` and `/hud` expose supported workflows. Admin `/testing` restricts Discord communication to configured admins without inserting testing-state instructions into Echo’s context. It does not restrict the web UI.

**Voice is an active development area:** join/leave, transport, encrypted audio, consent-based recording, transcription and artifact handling have substantial implementation and component coverage. Full live voice playback/recording/transcription acceptance is still open. AutoMod configuration is not a completed autonomous moderation policy controller.

[Join Echo’s Garden](https://discord.gg/9QU2rXwGVF) · [Open welcome-and-rules](https://discord.com/channels/1559928442403819711/1559928443817431091) · [Discord and privacy guide](docs/releases/1.9/DISCORD.md)

## Local ownership, explicit boundaries

Echo stores original conversation and tool events locally. Compaction preserves source-linked continuity rather than deleting the originals. Public community recall must pass current visibility and provenance checks. Private account linking is explicit: admin status alone does not grant access to another person’s private history.

Guest workspaces, host/admin tools, generated tool candidates and protected changes have distinct authority boundaries. The source includes charter, consent, review and rollback machinery. **The RSI pipeline is implemented: candidate admission, native rebuild, pinned acceptance, independent review, activation, health verification and rollback are distinct gates. A healthy controller does not prove that any particular request completed. See the current build record for run-level evidence and remaining limits.**

The configured host uses a local primary reasoning model through llama.cpp, with separate configured media services. Normal agent reasoning retains its configured setting. Factual closing reports and structured compaction reviews emit their reports directly without a separate reasoning stream on the llama.cpp adapter. No output-token cap is introduced. Exact model choice and runtime settings are host configuration, not bundled dependencies.

## Start exploring

| I want to… | Start here |
|---|---|
| Talk with Echo | [Discord invitation](https://discord.gg/9QU2rXwGVF) |
| Understand every feature and its status | [Feature matrix](docs/releases/1.9/FEATURES.md) |
| Set up the source on a compatible Mac | [Build](docs/BUILD-AND-TEST.md) and [operations](docs/OPERATIONS.md) |
| Understand how it works | [Architecture](docs/ARCHITECTURE.md) and [security](docs/SECURITY-AND-DATA.md) |
| Inspect callable tools and endpoints | [API and tools](docs/API-AND-TOOLS.md) |
| Review what changed since the last push | [Change narrative](docs/releases/1.9/CHANGES.md), [every changed file](docs/releases/1.9/FILE-CHANGES.md), [hash manifest](docs/releases/1.9/change-manifest.json) |
| Follow the path to production | [Phase Two plan](phase2.md) and [V1 remaining work](docs/releases/1.9/REMAINING.md) |

This is a source release with a [setup toolkit](tools/setup/README.md). An existing Apple Silicon installation can be verified and given a desktop shortcut in one launch. Fresh-machine bundles and native Windows/Linux ports are not yet packaged; the toolkit does not claim those systems are ready. Models, credentials, private identity prompts, reference recordings, conversations and live configuration are not included. Research latency remains workload-dependent and can take minutes; 1.9 does not claim that issue is resolved. See [verification and measured limits](docs/releases/1.9/VERIFICATION.md).

Maintained by [MettaMazza](https://github.com/MettaMazza), using [Ernos](https://github.com/MettaMazza/Ernos-Programming-Language). No project-wide software licence has been selected; upstream components retain their own terms.
