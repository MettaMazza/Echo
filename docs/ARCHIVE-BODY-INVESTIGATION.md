> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# User-message archive investigation — 16 October 2026

## Finding

The reported event `73dc2e73-c83f-431e-969d-b4efe684e0dd:user`, sequence 303217, stores the 35-character caption “Here is a handover from the mobile ”. Its separate, immutable `turn_attachments` event at sequence 303220 binds the original handover file to this exact user event. The attachment contains 24,632 Unicode characters / 24,798 bytes. Its file SHA-256 matches the registered digest and the attachment manifest. This is the character count cited in the report, in a separate source, not evidence of a truncated caption. Archived tool results at sequences 303292 and 303985 reference that attachment.

No historical event was rewritten, no replacement was invented, and no derived memory was used for restoration. The evidence does not support applying a truncation patch or concatenating a file into the original caption.

## Submission and persistence path

`app/ui/client.ep` assigns the composer text to `payload.message` and selected file identities separately to `payload.attachments`. `app/web/http_api.ep:web_submit_json_owned` validates and decodes the complete message string and separate attachment list. `app/web/run_store.ep:web_submit_owned_provider` passes that message directly to `archive_append_pending` and appends its attachment manifest in the same transaction. There is no prefix slice in this path. Retry checks compare the complete original message and attachment selection.

`app/archive/event_store.ep` hashes length-prefixed session, event, kind, complete payload, timestamp and predecessor using SHA-256. This is an event-envelope digest, not simply SHA-256(message). The current writer reads the inserted event back and checks its full payload and digest before committing. `app/archive/turn_attachments.ep` supplies the immutable per-message file manifest; the original file has its own digest. A message inspection must include both the caption and that manifest to account for the complete submission.

## Regression

`tests/web/test_long_message_archive.ep` submits over 50 KB of multi-part Unicode text with quotes and newlines, a distinctive final marker, and a separately registered attachment through the submission JSON parser. It reopens the database, requires exact full-body equality and the tail, recomputes the complete event digest, checks the session chain and attachment binding, and verifies idempotent retry. Passed with the self-hosted Ernos compiler on 16 October 2026. This is an isolated submission-handler test, not a claim of a new physical-phone test.

## Audit and limits

A read-only SQLite snapshot checked all 217 user events present at audit time: zero event-digest mismatches and zero mismatches against their actual preceding session event. The target original attachment was independently read and hashed successfully. Private evidence is in `evidence/phase2/archive-body-20261016/audit.json`.

A matching digest proves integrity of the stored event; it cannot prove that text was never omitted before hashing. There is no independent historical transport capture for every submission, so this audit does not claim to rule out every possible pre-hash omission. No affected rows were established by these checks. Zero rows were restored.

## Recheck of the reported event

A fresh read-only audit on 16 October checked 227 user events, with zero event-envelope digest or predecessor mismatches. The original attachment was reread: all 24,798 bytes / 24,632 characters remain present and match its registered SHA-256 and byte count. The attachment itself does not begin with the caption quoted in the report; these are separate submission components. The current long-message regression was rebuilt successfully with the self-hosted compiler and passed again in a fresh isolated database, including full-body equality, final marker, digest, attachment binding and retry. No archive rows were changed or restored.
