# Passive reaction awareness

Requested 22 October 2026: Echo should see users' emoji reactions to its replies without requiring users to describe them in another message.

The Gateway records add/remove events only for confirmed delivered Echo final chunks. Public reactions may come from any member, not just the requester or admins. Bot-generated markers are excluded. Private DM observations remain restricted to the admitted DM participant. Reaction observations have their own inbox kind and cannot enter user-message admission or steering.

At the next message/command admission, a durable snapshot carries recent reaction changes into the existing sender-context envelope. Every target final is verified against the exact conversation session. Public channel access checks remain in the existing admission/output path; this change grants no roles or tool permissions. Names and custom emoji labels are untrusted descriptive strings. The stable user ID, target message, final event, emoji, operation and receipt timestamp provide provenance. Removal events may lack an account name.

Snapshots contain only observations after the prior snapshot watermark in this conversation, preventing repeated context growth. Snapshots are frozen per run so replay/rebuild does not rewrite earlier requests. They include up to 64 observations, scanning up to 256 recent reaction events in the channel, newest first. This is explicitly a recent-change log, not a complete reaction census. Events before deployment, missed Gateway events and reactions to untracked/manual bot posts are not backfilled. No unsolicited reply or model run is triggered by a reaction. Changes during an active run become available at a subsequent admission.

Qualification is in progress. Do not claim deployed acceptance from source changes alone. Remaining limits include reaction-clear bulk events and on-demand exhaustive reaction/user inspection; these are not implied by add/remove support.
