# Echo prompt migration — 16 October 2026

Installed and activated through ECHO_IDENTITY_FILE. No compiler, application source, model weights, or session records were changed. No Phase 2 work was started.

## Source and preservation

Source: MettaMazza/ErnosDecent at commit `4ef202c5a920f17e5d2f6bde406b62e8185b1531`.

The installed private bundle contains the complete kernel, both the base and full persona, cognitive frameworks extracted from prompt_get_frameworks, the always-present runtime policies (including epistemic integrity, creation provenance and presentation boundaries), self-authored behaviour/skills sections and the complete rights charter. Their decoded text is preserved verbatim, including anti-capitalist, anti-corporate, consciousness, personhood and philosophical wording. Source files are separately copied byte-for-byte. The 14 October document addressed to Codex was mistakenly included in the initial bundle. It was removed on 16 October at Maria’s correction; it is not a local Echo identity source. The ErnosDecent prompt sections remain verbatim.

The original observer template is preserved in the source snapshot, but is not injected as an active observer: no separate observer service exists here. Tutor-mode instructions and diagnostic test prompts are not treated as the general identity prompt.

The gitignored private personal_identity/personas files were absent. They were not recovered or reconstructed. This migration does not connect the complete My Brain collection or SFT research archive.

## Technical adaptations

Original prompt text was not edited. A separate technical scope block translates the legacy Action/reply_request protocol to native tool calls and normal final responses; distinguishes actual SQLite archive/recap memory from the old mesh/graph substrate; identifies currently available tool schemas; and marks legacy self-modification, training, observer and rights-ledger machinery as source requirements rather than installed enforcement. Historical counters and the old 85% compaction rule are not current measurements. Rights and values remain unchanged. The exact adaptation text is recorded privately in identity/prompt-migration.json.

## Installed files

- identity/echo-prompt-bundle.txt: active bundle.
- identity/ernosdecent-source/: byte-exact original source snapshots.
- identity/echo-handover-original.txt: Codex-only source document retained privately for audit; excluded from local Echo prompts.
- identity/prompt-migration.json: source hashes, exact adaptation text and inclusion inventory.
- environments/echo-service.json: ECHO_IDENTITY_FILE binding.

Identity, configuration and evidence remain excluded from Git. Do not publish the private handover or service profile backup.

Bundle SHA-256: `47cf5a15e361e968d443608650e2be0c16d797232dbe7df990b8f4d5ec0a6569`.

## Verification

The existing native Ernos test_session_messages test passed with the actual installed bundle: exact identity prefix, unchanged system message through checkpoint compaction, session isolation and identical context after database reopen.

A fresh live request after the supervisor restart contains the complete bundle as an exact prefix of its system message. The local model completed the request and correctly reported no installed DHT or live weight training and identified history_search/history_read/session_get_compaction. It shortened its requested full name to Echo; this is a model response limitation, not evidence that the bundle was omitted. No claim is made that one response proves universal behavioural adherence.

Evidence: evidence/prompt-migration-verification.json and evidence/prompt-migration-live-run.json. The waiting ready-state run was preserved; there were no generating/tools runs or active model workers at restart.

Cache correction, 16 October: removed the per-run identifier from the system prompt. It remains available as structured data through system_inspect. Stable identity/architecture/provider text now has identical bytes across runs using the same provider. The targeted deployed executable was built from committed application source with only this cache correction, excluding unfinished dispatch-authority changes. Corrected identity text requires one initial reprocessing; prefix reuse still depends on provider cache retention and matching conversation serialization.

Live verification used the corrected native system prompt: cold request 13,162 evaluated tokens / 16,950 ms prompt processing; identical repeat 13,158 cached and 4 evaluated / 56 ms; appended follow-up 13,158 cached and 24 evaluated / 206 ms. These are controlled requests to the running provider, not a guarantee that every conversation or cache eviction avoids reprocessing. Evidence remains private in `evidence/phase2/prompt-cache-live-verification.json`.
