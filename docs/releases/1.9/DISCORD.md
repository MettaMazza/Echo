# Echo’s Garden and Discord controls

[Join Echo’s Garden](https://discord.gg/9QU2rXwGVF) · [Welcome and rules](https://discord.com/channels/1559928442403819711/1559928443817431091)

The Discord invite API was checked on 24 October 2026: the invite resolves to Echo’s Garden, opens welcome-and-rules and reports no expiry. Administrators or Discord can revoke it; membership/permissions still govern channel access.

Start with `/new` to open a fresh conversation while keeping authorized memory. `/history`, `/search` and `/resume` manage saved conversations. `/branch`, `/edit`, `/editfile`, `/regenerate`, `/retry` and `/cancel` act on their source-bound requests. `/attachments`, `/project`, `/schedules`, `/group`, `/share`, `/export` and `/hud` expose the corresponding workflows. `/auto` and `/autooff` are admin controls. Registration and permissions on a particular server determine availability; [definitions](../../../app/discord/command_registration.ep) are authoritative.

Admin `/testing` toggles Discord-only admission. Only configured admins can communicate while it is enabled. It does not insert a testing instruction into Echo’s prompt or restrict the web interface. Rejected and previously excluded queued messages are not admitted merely by turning the toggle off.

Public recall can cross users and public rooms with explicit attribution. Current visibility checks and imported-source ancestry apply. A public run cannot use the private linked DM/web archive, even when its sender is an admin. Private linking is a separate host configuration for the verified owner, not a role privilege.

Speak buttons and generated image/file replies bind to retained source artifacts. Discord voice-channel membership is separate from completed playback/recording/transcription acceptance; consult [feature status](FEATURES.md) before relying on those workflows.
