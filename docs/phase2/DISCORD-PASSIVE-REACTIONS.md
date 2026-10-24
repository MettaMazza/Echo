> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Passive reaction awareness

Requested 22 October 2026: Echo should see users' emoji reactions to its replies without requiring users to describe them in another message.

The Gateway records add/remove events only for confirmed delivered Echo final chunks. Public reactions may come from any member, not just the requester or admins. Bot-generated markers are excluded. Private DM observations remain restricted to the admitted DM participant. Reaction observations have their own inbox kind and cannot enter user-message admission or steering.

At the next message/command admission, a durable snapshot carries recent reaction changes into the existing sender-context envelope. Every target final is verified against the exact conversation session. Public channel access checks remain in the existing admission/output path; this change grants no roles or tool permissions. Names and custom emoji labels are untrusted descriptive strings. The stable user ID, target message, final event, emoji, operation and receipt timestamp provide provenance. Removal events may lack an account name.

Snapshots contain only observations after the prior snapshot watermark in this conversation, preventing repeated context growth. Snapshots are frozen per run so replay/rebuild does not rewrite earlier requests. They include up to 64 observations, scanning up to 256 recent reaction events in the channel, newest first. This is explicitly a recent-change log, not a complete reaction census. Events before deployment, missed Gateway events and reactions to untracked/manual bot posts are not backfilled. No unsolicited reply or model run is triggered by a reaction. Changes during an active run become available at a subsequent admission.

Native qualification and three real Discord conversation turns passed on 22 October 2026. Six rebuilt workers were deployed at an idle boundary. Remaining limits include reaction-clear bulk events and on-demand exhaustive reaction/user inspection; these are not implied by add/remove support.

## Acceptance

Native tests cover third-party non-admin reactions, add/remove and replay deduplication, bot and wrong-channel/guild exclusion, custom emoji, mismatched account-label rejection, no privilege elevation, exact-session/DM isolation, historical snapshot stability and omission of already delivered observations on a later turn. Generated GC roots were reviewed.

A real user-account heart reaction on an existing Echo reply produced a Gateway observation without starting inference. The next actual model request contained its correct emoji, account label and target final; Echo identified it without tools. Removing that heart produced only REMOVE in the next snapshot, and Echo correctly described the change. Both replies persisted after Discord reload. A third ordinary turn retained the earlier conversation marker and returned the requested arithmetic; its new-reaction list was empty. The live account was the configured test administrator; non-admin coverage is native, not a separate live non-admin-account test.

All six workers were alive with zero active runs after acceptance. Existing models, voice, permissions and identity profiles were retained. Source and acceptance records are published on the Phase Two community branch, not represented as completion of Phase Two.

Observed first-output times were 216,914 ms, 3,617 ms and 2,769 ms. This is functional acceptance, not a controlled morning-baseline latency pass; the first request remains slow. Old user attribution wording is unchanged and snapshots are incremental to avoid adding repeated reaction history.
