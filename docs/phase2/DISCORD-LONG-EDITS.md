# Long replacement prompts — implementation in progress

The existing `/edit` text option remains constrained by Discord's slash text
field. Its complete-prompt publication and branch admission are already
implemented, but the client rejected the 4,340-character acceptance input.
This is an input-path gap; silently truncating the prompt is not a solution.

The new `app/discord/revision_file.ep` foundation reads a retained text
attachment as an explicitly selected replacement prompt. It verifies current
account and Discord guild/channel/session audience, selectable/included state,
media type, original SHA-256 and byte length. It reads the complete UTF-8 stream,
including final characters; binary, empty, changed or unavailable files return
an explicit failure. No PDF summary or excerpt substitutes for the original.
Host attachments cannot be imported by knowing an ID or holding admin status.

A preparation receipt freezes the full text, source event and digest against
one interaction, actor and audience. Replay cannot replace the file or actor;
current rights and original integrity are checked again. Ordinary attached
content remains source data. Only an authenticated, explicit replacement action
may promote its text to the new user prompt.

The `/editfile message: attachment:` implementation now resolves a retained text
attachment ID from `/attachments list`, freezes its complete text and uses the
existing public prompt publication, revision admission, queue and cancellation
paths. The raw registered command remains bound to its authenticated interaction;
normalising it internally to `edit` does not grant autonomy or admin authority.
Current file access and integrity are rechecked before admission. This is a
retained-upload selection, not a direct slash attachment option.

Native pipeline acceptance passed; worker builds, registration and real Discord
upload/multi-turn acceptance are still pending. Production is unchanged at this
checkpoint. No completed live acceptance is claimed.

Native final acceptance passed in 7.685 s, binary
`eec7ddd21b9477c19dcc2b1054b7fe2c7a3aeca203b91b6d93a938ab4e0339f9`.
The fixture preserved all 40,553 bytes / 34,553 Unicode characters,
including the final sentence. It covers Host/other-account/channel denial,
selection and independent context exclusion, non-text media rejection, changed
original detection, preparation replay/actor conflict, reopen and removed-member
denial. Private receipt:
`revision-file-20261021/final-1792609890082532000/result.json`.
An initial test syntax error was corrected before compilation; the earlier
reader-only pass remains retained separately. These are native foundation tests,
not actual slash-command or upload acceptance.

## Integrated native acceptance — 21 October

`test_revision_start.ep` passed in 26.153 s with binary
`759f5924eb09a039aa2060e7d73b76b8c4eec584e959dc803c06fb8d3fd51b60`.
A complete Unicode replacement larger than the slash text field is split into
public message chunks; their concatenation and the admitted user event must
both equal the original file. Registration/options, public callback visibility,
ordinary-user provenance, restart/replay, cancellation, changed-source rejection,
credential expiry and authority revocation are exercised. The existing revision
fixture also passed separately in 17.915 s before the expanded test.

The first integrated fixture failed at its run-origin assertion: full text had
already arrived intact, but the origin allowlist omitted the new raw command
name. The allowlist was corrected without weakening actor or audience checks;
the failed receipt remains preserved. Final private receipt:
`revision-file-20261021/pipeline-qualified-1792610563880643000/result.json`.
