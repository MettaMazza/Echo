# Discord attachment controls — implementation in progress

The Discord transport must use the existing archive lifecycle and visibility writers. Native uploads remain native; these controls manage retained attachments after upload.

- **List** shows attachments for an authorised Discord conversation, including current lifecycle and visibility revisions.
- **Trash / restore selection** controls future attachment selection. Existing messages, original bytes, project references and recall remain preserved.
- **Exclude / restore context** controls future use of the attachment and identified source-derived context. Original bytes and receipts remain available for explicit inspection and restoration. This does not erase backups, external copies or weights.

The connector supplies the authenticated actor, guild and channel. A known attachment ID, linked account or admin role does not grant access to Host files or another channel. Namespace checking and mutation occur in the same transaction. Every mutation requires the current revision and a stable interaction identity; retries reuse the original immutable receipt.

Current source: `app/discord/attachment_control.ep`. Browser entry points retain their transaction wrappers and share pending writers with Discord. Discord command registration, private response delivery and actual UI acceptance remain pending; this document does not claim the controls are available in Discord yet.

Planned command surface: `/attachments list session:` and explicit `trash`, `restore-selection`, `exclude-context`, `restore-context` actions with attachment and revision options. Results must clearly distinguish selection from context visibility and retain an inspectable receipt. Delivery rechecks current source membership before returning file metadata.
