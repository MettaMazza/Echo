> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Long replacement prompts — retained-file input deployed

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

Native pipeline acceptance and real Discord upload/edit/follow-up/reload checks
have now passed for complete input preservation. The qualified workers are
deployed and the command is registered. The exact-only answer format and cold
branch latency observations below remain distinct unresolved findings.

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

## Deployed and actual Discord acceptance — 21 October

Qualified source `21ef063` was built into the affected agent, actions, activity,
queue-control and acknowledgement workers; the unchanged Gateway binary was
reused. The workers were replaced only after the connector was idle, with the
previous registry preserved for rollback. The new fixed command was registered
through the native maintenance program after deployment. Voice, model selection
and prompts were unchanged.

The ordinary testing account uploaded `live-edit-input.txt` using Discord's
native file picker. Echo answered “File retained.” without executing the file's
instructions. The explicitly submitted `/editfile` then published all 5,473
characters in three confirmed public chunks, and the joined chunks and admitted
user event exactly matched the original. Both answering model requests and the
next follow-up retained the entire replacement in the user content array.
Source SHA-256:
`6d18b9accb6009e828d700f04a1aaa7787fb261b3c75f41595bb13110fe1371a`.

The edit reply included the final marker `amber-heron-247`; the follow-up answered
`amber-heron-247 and 23`. The original, public prompt, replies, Thinking threads
and Speak controls remained visible after client reload. **The edit reply also
included unrequested verification commentary, so this is not an exact-only
response-format pass.** No prompt change was made to hide that observation.

The first edited-branch answering request processed 28,242 new tokens and reused
43,739 cached tokens, with 43.369 s prefill and 32.22 tokens/s decoding. The
follow-up processed 744 new / 73,302 cached tokens, with 1.467 s prefill and
32.17 tokens/s decoding. These are provider timings, not complete user-perceived
latency, and the cold branch remains a performance issue. This is not the final
controlled baseline/endurance acceptance.

Private evidence: `revision-file-20261021/live-pipeline.json`,
`request-source-coverage.json`, `ui-acceptance.json`, `worker-build.json`,
`registration/result.json`, and before/after deployment registries.
The Discord client file chooser initially stalled; restarting only the client
recovered it before upload. No duplicate message or command was submitted.
