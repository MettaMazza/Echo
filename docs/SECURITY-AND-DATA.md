> **Echo 1.75 update:** [Release record](releases/1.75/README.md) and [remaining work](releases/1.75/HANDOVER.md) supersede older model/voice/watch status below. Older qualification records remain historical.

# Security and data boundaries

Device credentials authenticate paired browser profiles; session ownership is checked by server routes. Local Host/Origin checks reject foreign origins while permitting the configured loopback and HTTPS gateway origins. LAN availability is not anonymity or strong separation between people sharing a browser. There is no claim of a completed independent security audit.

The archive contains user messages, original provider requests/results, tool arguments/results and execution output. These are sensitive even when test messages also exist. Attachments and exports may contain private user material. The Thinking API is scoped to the authenticated device's run and must not expose another device's records. Trace text is rendered with textContent; tool data is not interpreted as HTML.

Model tools work within the trusted run/session context. Workspace paths reject traversal/symlink escapes; execution uses the session sandbox and explicit argv. It has no inherited private environment or network access. The model cannot gain arbitrary host access by naming a different session or path. `system_inspect` reports selected runtime facts, not secrets or every environment variable.

Identity and architecture files are host-controlled. Original prompt wording is separate from runtime technical guidance. They are read during context construction, so operators must not edit them during an in-flight checkpoint review. No versioned identity-management UI is claimed.

Local storage holds browser credentials, pending request identities and unsent drafts. Anyone with that browser profile can access them. Draft persistence improves recovery but is not encrypted secret storage. Revocation is a server decision; clearing browser data loses that profile's local state.

Raw service/gateway logs must be treated as private. During the 15 October incident the gateway's error record included a custom credential header. Those logs are excluded from publication; public reports include only reviewed metadata and hashes. Do not assume a proxy automatically redacts every custom authentication header.

Publication uses an allowlist: application/tests, compiler integration source, documentation, source manifests and placeholder configuration. Excluded: original conversation databases, WAL/SHM, attachments, exports, private identity, active profiles, TLS material, model weights, generated native binaries and raw logs. Test credential literals are synthetic fixtures and must never be reused in deployment.
