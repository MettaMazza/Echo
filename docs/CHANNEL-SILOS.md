> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# User-in-channel contexts

`app/security/channel_silo.ep` implements the Step 06 personal channel route. Trusted adapters supply the authenticated device and verified platform, server and channel. The principal comes from the device binding; callers do not supply a user identity. A length-prefixed namespace hash selects a stable context for that principal in that channel.

The route creates a fixed, one-principal audience using the existing group storage, then admits the message through the existing transaction-bound group run path. It does not add private-session ownership or import private memory. Existing group archive, workspace, attachment and compaction boundaries therefore apply. A joined conversation must be created separately; personal channel audiences cannot be expanded by invitations.

Exact submission retries retain the original run and segment, including after reopening the archive. User, channel, server and platform differences select different identities. Existing device/principal revocation checks still apply before admission.

Native integration verification: `tests/security/test_channel_silo.ep` creates real private and channel conversations for two principals and two channels, checks assembled model context, rejects cross-user/channel access and audience expansion, and reopens the archive for exact retry. The existing group routing regression also passes.

This is implemented connector-facing storage and routing. Discord event ingestion is not yet wired to it. Channel-public, admin and Echo-system routing remain separate Step 06 work; this does not claim completion of that step.
