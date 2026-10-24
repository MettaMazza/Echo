> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# General task and retained-document navigation repair — 18 October 2026

## Observed failures

Read-only archive inspection confirmed runs `29949592-6ccd-4bbb-9830-b1933e0cc38a` and `014f0d15-5d10-44af-93a6-515920d07485`. Two news searches were rejected before a checklist existed. After authenticated guidance during the document task, three read-only calls were rejected because checklist state was `revise`. A later checklist update was rejected for changing requirement wording, without returning the canonical requirements. Search challenges came from the external provider; an unrelated signed-proxy URL returned HTTP 403. Those external failures are not repaired or disguised as successful fetches.

## General changes

- Read-only capabilities can gather evidence without first creating or resaving a checklist. This covers public web reads, retained attachments, workspace reads, memory, session records, discovery and scoped status. Permission checks still precede this exception. Completed tasks still return their result; mutation/delegation bookkeeping remains enforced. No task-title, user-name, domain or paper-specific exceptions.
- `attachment_read_text` accepts `mode=read|search|headings`. Search takes a nonempty literal `query`; headings finds Markdown ATX heading candidates. Both use the existing trusted attachment scope, integrity verification and PDF page extraction. They scan locally and return a caller-sized passage, its exact Unicode offset, source event, digest and continuation cursor. This is deterministic source navigation, not semantic summarization or an assessment of the source's claims. Heading candidates inside code fences are not parsed as a Markdown AST. PDF searches cover the selected page; callers traverse pages.
- Search and heading cursors advance one Unicode scalar beyond the matched start, allowing overlapping matches without skipping later headings inside a returned excerpt. Read cursors advance by characters returned. There is no source-length or model-context ceiling added. Search currently materializes verified text in native process memory; very large files remain subject to machine resources.
- Checklist conflicts return the actual saved checklist and state that no update occurred. Requirements remain protected against silent removal. Tool guidance distinguishes preparing a final answer from already delivering it.
- Search challenge messages and effects explicitly state that the challenge response was retained and no search results were obtained and suggest accessible sources/APIs/feeds, rather than generic uncertain-change repair. The challenge is not bypassed.
- Runtime routing explains document navigation, caller-selected read size and continuity across compaction. It does not alter identity, philosophy or conclusions about any paper.

## Verification and deployment

Native checks passed on the rebuilt checklist, attachment and feedback test executables. Feedback tests preserve real partial-write effects, unknown outcomes and original receipts while giving search challenges an accurate read-only outcome and recovery. Checklist tests cover read-only parent and delegated work during missing/revised checklists, mutation exclusion, exact requirement preservation and recovery payloads. Attachment tests cover Unicode offsets and continuation, heading navigation, end-of-search, foreign-session exclusion and changed-file rejection. The attachment fixture was corrected to install its required session-principal schema; the earlier incomplete-fixture executions are not acceptance evidence.

The isolated browser candidate initially used an invalid HTTP public-origin argument and lacked the explicit principal/delegation migrations. These test setup errors were corrected; the interrupted first test is not a pass. Browser acceptance passed: run `8074be87-9fe9-4dfb-a0e1-fb4dedf066bf` correctly returned both markers, the count 47 and the following heading from a 1,020,160-byte fixture, using three targeted reads totaling 2,294 characters and no checklist gate. The final feedback-only refinement passed its native test after the browser candidate was built. Production navigation release is deployed with server SHA-256 `d62597b024573e5b4d0f30cad33609231c3a8b5f7a51e93265398cad923a36a0`; production reload showed Local service connected. Pending Step11 trace/HUD work is excluded from this bugfix release, which is built from the deployed video baseline plus this change. Existing conversations and workers are preserved.

## General operating rules derived from this review

1. Scale task bookkeeping to the work. Evidence gathering may proceed without a plan; changes and delegated assignments retain their requirements and authority checks.
2. Navigate sources by identity, headings, literal matches and explicit cursors. Chunk size is a choice for the current context, not a permanent reading ceiling. Preserve read coverage and source references across interruptions.
3. Classify failures by cause and effects. A provider challenge, permission denial, missing resource and incomplete execution need different recovery instructions. Preserve canonical source identity; do not infer that an unrelated redirected or signed URL is the intended source.
4. Make recovery actionable. A rejected update should return the current canonical state and identify the constraint rather than require the model to reconstruct it from memory.
5. Distinguish prepared, executed, verified and delivered work. A planned final response is not a delivered response. A published PASS receipt and matching hash establish what artifact was retrieved; they do not show that this reader independently reran its verifier or validated every scientific interpretation.
6. Keep evaluations proportional to evidence and independent of presumed user preferences. Correct a criticism when the evidence warrants it; do not convert a partial review or absence of a discovered defect into universal confirmation. This review makes no determination about the paper's substantive claims.

Rules 5–6 include review guidance beyond the narrow tool/schema changes. Broader persona and epistemic prompt revision remains with the planned joint prompt review; no paper-specific conclusion or philosophical position is installed by this repair.
