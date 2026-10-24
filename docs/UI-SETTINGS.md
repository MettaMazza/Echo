> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Options and settings

Updated 17 October 2026. Options opens a native modal dialog with Appearance, Conversations, and (when present) Devices & access categories. The desktop layout uses a category sidebar; phones use a horizontally scrollable category row. Each section scrolls independently of the conversation.

Existing controls are moved intact, retaining their IDs, values and event handlers. Appearance is saved by its existing handler; message batch size affects history loading only. Export, recap inspection, browser access refresh and device linking use the original implementations. Hidden actions produce no empty cards. Escape, close and backdrop dismissal restore focus to Options. The Link devices shortcut selects its category and focuses the code input.

Implementation: app/ui/client.ep, echo_settings_section, echo_settings_card and echo_settings_setup. Add categories through the section helper and existing controls through the card helper. This changes presentation only and adds no permissions, settings or backend features.

Deployment preserves the existing server and inference processes/configuration. Only the browser bundle changed, using the prior deployed client plus an isolated Ernos-generated presentation module. The source also invokes setup after layout construction for normal future full builds. Browser checks covered navigation, dismissal, device shortcut, and a 390 × 844 mobile layout. Evidence: evidence/ui/options-20261017/. Refresh an existing browser tab to load the new UI.

## Conversation sidebar dismissal

17 October 2026: `echo_sidebar_dismiss_setup` installs one menu toggle handler, outside-pointer dismissal, Escape dismissal, and a mobile close button. New chat and conversation selection close the sidebar in their existing navigation handlers. Live browser checks confirmed retapping the menu, tapping the composer outside the menu, and New chat all close it; New chat displayed an empty conversation and “Ready for a new conversation.” This is deployed in `phase2-agents-20261017-062146` (also present in the preceding sidebar release). Existing guards still prevent changing conversations during a running turn, upload, or unresolved submission; this change does not cancel background work or remove those guards.
