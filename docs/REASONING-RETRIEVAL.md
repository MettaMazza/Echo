> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Reasoning retrieval

`reasoning_read` reads the provider-emitted reasoning retained on published Echo response events. It does not reconstruct or invent missing reasoning. Scope remains the authenticated run’s private principal.

The `event` argument accepts an exact published response event ID, its model-request ID, or a run ID. A run selects its latest published final response, falling back to its latest tool response if no final response exists. A mistaken `<run>:response` is resolved through the actual run rather than treated as a nonexistent response event. Exact events take precedence; the returned `event` and `digest` identify the original source selected.

`memory_messages` also returns `reasoning_event`, `run`, and `reasoning_characters`, so Echo can discover the correct source instead of constructing an ID. Results distinguish `capture_status=captured`, a found response with no emitted/retained reasoning, and `status=not-found` with discovery guidance. An unmatched ID no longer returns the ambiguous `end`.

The reported failure was replayed from the archived calls: one used a run UUID plus `:response`; another used a model-request UUID. The actual final response records contain 121 and 1012 reasoning characters respectively. These were lookup failures, not absent capture or denied access. Original archived events were not modified.

Implementation: `app/tools/continuity_tools.ep`. Regression coverage: `tests/security/test_continuity_tools.ep` (exact event, request/run aliases, missing identifier, other-principal isolation), and `tests/security/test_reasoning_existing.ep` (read-only replay against an existing authorised run without printing trace text).
