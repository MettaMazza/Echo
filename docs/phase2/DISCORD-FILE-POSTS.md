# Discord retained-file delivery — 22 October 2026

Status: native qualification, idle deployment and four actual consecutive Discord acceptance turns passed within the scope below.

The `discord_post` reply accepts one retained attachment ID, optionally alongside text, embeds or a poll. The source must belong to the exact current Discord conversation and remain accessible, selectable and included. A host pathname, URL, private Host attachment or another conversation's file is not a source grant. `attach_file` first preserves a workspace file as an immutable attachment. File delivery never changes the existing host/private versus Discord boundary.

The file post records its source ID, digest, size and deterministic safe filename. Multipart encoding preserves binary NUL/high bytes and verifies the frozen buffer against the original digest before HTTP. The frozen payload is bound to the immutable outbox body and nonce. Current requester/bot ATTACH_FILES authority and the ordinary source/cancellation guards apply on dispatch and pending retry. An uncertain transfer is inspected, never blindly resent. Failed or excluded sources cannot silently degrade to an empty/text-only post.

The default Discord file allowance is 10 MiB; an authenticated interaction's advertised limit takes precedence. A rejected oversized file remains retained locally and the pending post is reported. This is a protocol allowance, not an archive size limit. Filename header characters are sanitized; original names remain in the attachment archive. Text edits retain existing remote attachments; deletion remains restricted to this requester's tool-owned posts in this channel.

## Required evidence

Native binary/header/digest/size tests, exact conversation and actor isolation, immutable mapping, exclusion/retry checks, original audio multipart regression, imported worker build/root inspection, real multi-turn creation/upload/status/follow-up and disposable cleanup, authenticated downloaded-byte hash comparison and UI/reload inspection. None is claimed passed until the matching receipt exists.

Protocol: [Discord message attachments and multipart uploads](https://docs.discord.com/developers/resources/message), [API file upload reference](https://docs.discord.com/developers/reference#uploading-files).

## Native qualification and deployment

Both the new retained-file regression and existing audio multipart regression passed. The new test includes another actor, another run/conversation, immutable mapping, binary NUL/high bytes, exact byte limit, malformed header characters, excluded-source retry and source-byte mutation. Initial fixture failures (symlinked temporary path, device/principal prefix and omitted isolated attachment schema) are preserved; the device resolver uses the existing `discord:` device identity. Five importing workers built successfully without application changes during their builds. Generated roots for source/preparation/multipart values were inspected. The deployment occurred with no active runs or interaction jobs; the prior build remains available. Canonical receipts: `evidence/phase2/discord-file-posts-20261022/`.

## Actual Discord acceptance

The initial two real turns exposed a missing dedicated attachment-storage setting in spawned Discord workers. `attach_file` correctly returned `not-configured`; no upload was claimed. The repair forwards only `ECHO_DISCORD_ATTACHMENT_ROOT`, never a generic Host root, and the launcher supplies the isolated Discord archive. The native child-process environment/isolation regression passed, all five importing workers were rebuilt, and the repaired deployment waited for idle. Earlier failures and build receipts are retained.

Four subsequent actual turns uploaded the existing 60-byte UTF-8 fixture once, read its owned post and retained text, removed only the disposable post, and answered a normal follow-up with the correct marker, arithmetic and authenticated username. An independent authenticated message read and credential-free CDN download matched the retained bytes and SHA-256 exactly. Independent cleanup read returned HTTP 404 / Discord code 10008. The original workspace file and retained attachment remained readable.

The file card survived reload before cleanup. Final replies and their source-attached Thinking threads survived reload after cleanup. A fresh Speak request generated public audio on the ordinary final reply, and that audio survived reload, checking the shared multipart path without changing Warm's profile. Canonical `acceptance.json`, `live-turns.json`, `receipt-comparisons.json`, `cleanup-verification.json` and `ui-checks.json` preserve the observations.

User-request first-output observations after repair were 3.469, 1.961, 2.469 and 3.430 seconds. These are functional-run observations, not the full controlled latency/endurance gate or a like-for-like comparison with the historical baseline. This bounded pass does not close the remaining Discord catalogue or Phase Two.
