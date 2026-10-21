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

This foundation is not yet connected to a usable Discord control. The remaining
work is a registered file-edit interaction, source selection/upload integration,
public complete replacement preview, existing revision admission, cancellation,
replay and real multi-turn Discord acceptance. No new command has been
registered or advertised as ready. Production is unchanged.

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
