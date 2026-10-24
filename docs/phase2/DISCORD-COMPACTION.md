> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

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

The Discord activity worker is deployed in discord-parallel-20261021-133748; its binary SHA-256 is 3c5cce64c7d28661e96b2baa7bd869b03b5984a80c6ad0c2913ae61bcbc0e256. All six connector processes were observed running after deployment. The Host server remains staged. Native web trace and Discord page regressions pass, including source/run isolation and persistent page retries. An actual compaction session and remaining animated-status acceptance are still required; ordinary tool-thread delivery does not prove compaction acceptance.
