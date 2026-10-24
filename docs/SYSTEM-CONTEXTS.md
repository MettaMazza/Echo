> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Echo system contexts

`app/security/system_silo.ep` routes Echo-owned records into dedicated contexts using the host-configured `ECHO_SYSTEM_PRINCIPAL`. An authenticated device must resolve to that exact active principal. A human admin role, device display name or supplied message does not select the system identity. Missing configuration denies admission.

Context identities include both the principal and context name using length-prefixed fields. Each context has its own durable group-backed session, run provenance and existing workspace/attachment/compaction boundaries. It receives no private human-session ownership or history. The audience is fixed to the service principal and cannot be expanded by invitations. Existing device/principal revocation checks apply on submission and reads. Changing the configured service identity does not transfer older records.

The host configuration selects identity; it does not grant a role. Provisioning and role assignment must use the existing audited host principal commands. The routing function is not a model tool or public HTTP endpoint.

Native integration: tests/security/test_system_silo.ep verifies human-admin impersonation denial, two independent assembled contexts, exclusion of private human history, fixed audience, exact retry after reopening, and revoked service denial. Evidence: evidence/phase2/system-silo-20261016/.

Production service enrolment and autonomous job integration are not enabled by this component commit. Step 06 remains open, including channel-public routing and the remaining integrated context checks.
