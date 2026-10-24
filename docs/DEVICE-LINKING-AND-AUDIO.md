> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Linked browsers and uninterrupted audio

## Link phone and Mac

On the device whose account you want to link, open **Link devices**, expand **Want to bring these chats to another device instead?**, and create a code. On the other browser choose **Link devices**, paste the code into step 1, and click **Link this device & open my chats**. The device name is optional. Codes expire after ten minutes and work once.

When the joining browser already has an account, it supplies its existing credential alongside the one-use code. Redemption and merging its conversation library happen in one transaction. Both devices then list their combined private conversations and can open and continue either original conversation. Original session IDs, message bytes, attachment references, ownership, workspaces and provenance are retained, not copied or rewritten. Existing switch-only links upgrade from the previous credential retained in that browser when it next loads. Linked-account scope also applies to Echo’s session/message recall, reasoning retrieval, automatic memory context, memory graph reads, attachment listing and text retrieval. Original principal ownership and provenance are preserved; group membership and tool authority are not rewritten. New work in an original conversation retains its original principal and authority rules.

The visible page checks every three seconds for new conversations and turns. Returning to the tab checks immediately. Changes update the sidebar without replacing unchanged rows; an open conversation follows a new run and its ordinary progress stream. Draft text is retained. Browser drafts and unsent queues remain local; a backgrounded browser catches up when visible again. Simultaneous execution in one conversation remains serialized.

Each linked browser has an independently hashed credential. Unlink revokes that credential for future requests; it does not split the combined library or erase accepted work. Original device/principal revocation still applies. Linking intentionally shares access to the linked account, including its permissions. Never give the code to someone who should not have that access.

Schema23 holds link-code digests, expiration/consumption and linked credentials. Schema24 adds conversation-account membership and append-only merge receipts. Both credentials are verified; caller-supplied device IDs cannot authorize a merge. Private HTTP resource requests resolve their original owner only after proving active membership in the combined account. Existing per-session checks still apply. Group routes keep their normal identity rules.

Implementation: `app/web/device_links.ep`, `app/web/devices.ep`, `app/web/routes.ep`, `app/web/run_store.ep`, and `app/ui/client.ep`. Native fixtures cover atomic merge rollback, combined listings, unrelated-user denial, continuation, preserved original principals, revocation and reopen. `tests/web/linked_sync_browser.ep` uses the real browser watcher/queue with transport probes to verify automatic list/turn observation. The former cancellation-based steering behavior is superseded by `tests/web/test_midturn_guidance.ep` and MESSAGE-QUEUE.md.

## Audio

The original native audio player remains inside its message, with its seek/play/pause controls and Download audio button. There is no separate composer playback dock. The history renderer reuses connected message nodes and adds newly arrived attachments in place. Batch commits do not detach retained audio elements; a playing older message remains connected even if it falls outside the newest requested batch. This keeps playback running when a new message is submitted or a reply arrives. A capturing play listener pauses the previous audio element when another starts. Playback coordination is within the page, not across devices.

`tests/web/audio_playback_browser.ep` uses the actual message renderer, inline player and batch commit functions with local WAV bytes. Browser verification confirmed continued playback across a new-message commit and single-player exclusivity. Switching conversations or losing access still clears the relevant transcript.

### Clearer linking screen
The header now has a direct **Link devices** button. The first form accepts a code from another device, with numbered visible labels and an optional device name. Code creation is a separate expandable section. Linking displays progress and actionable code-expiry errors.
