> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

## Current 1.9 audience model

Public Discord memory may cross speakers and public channels after live view/history and imported-ancestry checks. Public does not mean “an admin can read it.” Restricted rooms and private threads remain restricted. A DM/web owner can access explicitly linked private history through `ECHO_PRIVATE_MEMORY_LINKS`; admin status alone never establishes ownership. Public/group runs cannot open that bridge. `/testing` is Discord admission state only, never an agent prompt directive. Brave credentials are restricted to the fixed API endpoint. Voice recording requires visible notice, participant consent and current per-participant authority, with revocation and epoch changes rechecked. Do not publish identity links, prompts, recordings, tokens or archives.

> **Echo 1.75 update:** [Release record](releases/1.75/README.md) and [remaining work](releases/1.75/HANDOVER.md) supersede older model/voice/watch status below. Older qualification records remain historical.

# Security and data boundaries

Device credentials authenticate paired browser profiles; session ownership is checked by server routes. Local Host/Origin checks reject foreign origins while permitting the configured loopback and HTTPS gateway origins. LAN availability is not anonymity or strong separation between people sharing a browser. There is no claim of a completed independent security audit.

The archive contains user messages, original provider requests/results, tool arguments/results and execution output. These are sensitive even when test messages also exist. Attachments and exports may contain private user material. The Thinking API is scoped to the authenticated device's run and must not expose another device's records. Trace text is rendered with textContent; tool data is not interpreted as HTML.

Model tools work within the trusted run/session context. Workspace paths reject traversal/symlink escapes; execution uses the session sandbox and explicit argv. It has no inherited private environment or network access. The model cannot gain arbitrary host access by naming a different session or path. `system_inspect` reports selected runtime facts, not secrets or every environment variable.

Identity and architecture files are host-controlled. Original prompt wording is separate from runtime technical guidance. They are read during context construction, so operators must not edit them during an in-flight checkpoint review. No versioned identity-management UI is claimed.

Local storage holds browser credentials, pending request identities and unsent drafts. Anyone with that browser profile can access them. Draft persistence improves recovery but is not encrypted secret storage. Revocation is a server decision; clearing browser data loses that profile's local state.

Raw service/gateway logs must be treated as private. During the 15 October incident the gateway's error record included a custom credential header. Those logs are excluded from publication; public reports include only reviewed metadata and hashes. Do not assume a proxy automatically redacts every custom authentication header.

Publication uses an allowlist: application/tests, compiler integration source, documentation, source manifests and placeholder configuration. Excluded: original conversation databases, WAL/SHM, attachments, exports, private identity, active profiles, TLS material, model weights, generated native binaries and raw logs. Test credential literals are synthetic fixtures and must never be reused in deployment.
