# Discord conversation recap visibility

The source-attached thinking thread uses the same archived trace projection as
the web UI. The candidate implementation adds committed compaction stages:
preparation, saved notes, merge progress, a draft awaiting review, recorded
reviews, combined findings, and the transactional attachment-verification
receipt accompanying recap activation.

Each stage is bound to the original run's compaction job and session. A job for
another run in the same session is excluded. Status text does not publish draft
recap content or invent a percentage. Review completion alone is not reported
as acceptance. The start and completion explanations state that original
messages remain saved. Existing trace pagination and durable thread delivery
carry these entries without replacing conversational replies.

This is candidate source, not deployed or live accepted. Native web trace and Discord page regressions pass, including source/run
isolation and persistent page retries. Deployment, an actual compaction session, and the remaining animated
status parity must be recorded before closing the compaction UI requirement.
