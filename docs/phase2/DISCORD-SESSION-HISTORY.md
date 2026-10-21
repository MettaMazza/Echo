# Discord conversation history and resume

Deployed and live-qualified in the testing connector, 21 October 2026. This closes the existing-session selection control, not all Discord parity.

`/history` lists up to ten saved conversation IDs for the authenticated user's current channel silo and membership revision. `after` requests the next page. `/resume session:` selects an existing conversation from that same audience. `/new` continues to create a fresh conversation while retaining the old archive.

Selections are append-only archive events. Retrying an older interaction does not override a later selection. Resuming does not copy memory or move messages across users, channels, guilds, host storage or membership revisions. Delegated child sessions cannot be selected. Existing runs retain their original session.

Native session tests passed retained history, selection and restart persistence, new-after-resume, replay ordering, denied foreign identities/targets, operation collisions and write contention. Discord command registration, binding and execution tests passed, followed by full worker builds. Actual Discord interactions: history 1562337319166419035, new 1562337451240980480, resume 1562337542810894386. Runs 1562337633022246945 and 1562337822566781021 completed in the original resumed session and correctly recalled the earlier tool lookups and the follow-up. Export acceptance is recorded below; richer selectors remain open.

## Conversation search

`/search query:` searches saved human messages and Echo final replies in the same authenticated account/channel/audience revision used by `/history`. It returns at most ten matching session IDs, which can be passed to `/resume`; refine the query when there are more matches. It does not alter the selected session or start a model run. Results use the private command response, with mentions disabled.

Matching is literal substring search with SQLite's ASCII case folding. Query text is parameter-bound; SQL punctuation and wildcard characters do not become executable queries. Blank queries and queries over 1,000 bytes are rejected. Search excludes tool results, reasoning, internal final-event metadata and delegated child sessions. It does not search Host archives or other accounts' channel silos. Current authority and the read use one database snapshot.

Native scope, final-content filtering, literal-query, argument/registration and dispatch checks passed. The command is deployed. Actual Discord interactions 1562344192653656114 (matching retained marker) and 1562344353593040936 (no match) completed with private responses confirmed in the UI. Do not infer full search/export parity from this bounded command.

## Verified conversation export

`/export session:` prepares the existing provenance-verifiable JSON archive for a session in the requesting account's current channel silo and audience revision. Every branch ancestor must remain in that same scope. Delegated children, Host sessions and foreign channel/account targets are denied. This reuses the web export format and verifier; the JSON contains retained archive records, not a lossy summary. Binary attachments are not embedded by that existing format.

Each authenticated interaction freezes its export bytes and SHA-256 in an immutable private journal record. Later messages cannot change a retry's snapshot. Delivery PATCHes the original private interaction response with a content-addressed JSON filename, preserving its private visibility and disabling mentions. Current channel access, database authority, original interaction, credential expiry and file size are checked before upload. An uncertain transfer is inspected before retrying the same original response.

Native dispatch, snapshot/retry and ancestor-boundary checks passed. The deployed command returned a private attachment in Discord. Downloading that attachment through the actual UI reproduced its advertised SHA-256, and the standalone native verifier accepted its contents and hash chain against the separately recorded head. This live archive contained one session-selection event; multi-event and retry checks used native fixtures. See `../releases/1.75/discord-conversation-export-2026-10-21.json`. Exports exceeding the current Discord attachment limit are explicitly rejected without truncation; multipart-volume export remains open, so this does not close all export parity.
