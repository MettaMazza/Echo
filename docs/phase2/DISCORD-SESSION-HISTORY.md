# Discord conversation history and resume

Deployed and live-qualified in the testing connector, 21 October 2026. This closes the existing-session selection control, not all Discord parity.

`/history` lists up to ten saved conversation IDs for the authenticated user's current channel silo and membership revision. `after` requests the next page. `/resume session:` selects an existing conversation from that same audience. `/new` continues to create a fresh conversation while retaining the old archive.

Selections are append-only archive events. Retrying an older interaction does not override a later selection. Resuming does not copy memory or move messages across users, channels, guilds, host storage or membership revisions. Delegated child sessions cannot be selected. Existing runs retain their original session.

Native session tests passed retained history, selection and restart persistence, new-after-resume, replay ordering, denied foreign identities/targets, operation collisions and write contention. Discord command registration, binding and execution tests passed, followed by full worker builds. Actual Discord interactions: history 1562337319166419035, new 1562337451240980480, resume 1562337542810894386. Runs 1562337633022246945 and 1562337822566781021 completed in the original resumed session and correctly recalled the earlier tool lookups and the follow-up. History search/export and richer selectors remain separate open controls.
