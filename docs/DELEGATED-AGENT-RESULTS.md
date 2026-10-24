> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Delegated agent results

> 18 October reconciliation: the current [1.5 audit](releases/1.5/SYSTEM-AUDIT.md) supersedes older outstanding-work statements below. Attachment visibility controls now exist in routes and the browser. Later actual two-child parent-return evidence is recorded in [CHECKLIST-DEPLOYMENT-CACHE-2026-10-17.md](phase2/CHECKLIST-DEPLOYMENT-CACHE-2026-10-17.md). Earlier cancelled tests remain historical, not the latest acceptance result.


Updated 17 October 2026.

Delegated runs retain separate archive sessions and their inherited authority. They are omitted from the ordinary conversation list. The parent conversation exposes child run identities through the existing scoped work-status endpoint, including the parent objective event for inline placement. No archive session is deleted or merged.

The parent worker waits without model calls while admitted children are active. After all direct children have reached completed, failed, cancelled or waiting state, `delegation_collect` appends an immutable `delegation_results` event containing each full final payload, source event and digest. Authority-revoked results are withheld. This event enters parent request context and recap sources. The parent then generates its own response. Results are evidence, not executable instructions.

Receipt identity depends on the child identities, current heads and authority state. Reopening the archive cannot redeliver an identical result set. A later changed set creates a distinct receipt. A parent that already completed can transition back to ready only with a delegation-results event; the scheduler may then reclaim its completed worker record using the existing compare-and-swap. Running workers are not reclaimed.

The UI adds expandable Agent panels inside the original conversation, with paginated full child messages and the existing trace viewer. It preserves a fallback panel while the parent message has not yet loaded. Source is `app/ui/client.ep`; the deployed extension was compiled from plain-English Ernos.

## Evidence and status

Native `tests/jobs/test_delegation_return.ep` passed against `/tmp/echo-delegation-return-20261017-b.sqlite`: two-child waiting, complete Unicode results in parent context and compaction sources, early-completed parent wake, archive verification, durable reopen without duplicate delivery, and omission of child sessions from the chat list. Child completion was synthetic; this is not actual-model acceptance.

The server and browser extension are deployed in `phase2-agents-20261017-063428`. Production service profile and release manifest remain authoritative. Existing inference and active child workers were preserved during deployment.

Actual-model acceptance run `4a8a4799-a35d-4b92-8a20-f3fe81967cd4` admitted a real child with the requested instruction. Maria requested cancellation for manual testing before a completed child result returned. Parent and child workers stopped, with cancelled state visible in the UI. This run does not prove the full successful model handoff. Native result-collection acceptance above remains valid.

The live browser now verifies an expandable Agent panel inside the original conversation, containing the exact child instruction and nested trace control. Fixed a DOM construction failure caused by an undeclared callback variable named `status` colliding with the browser's `window.status` property. Callback locals are explicitly scoped, and panel-loading failures are now visible instead of silently swallowed.

UI paging correction: `/api/work` requires a numeric `after` value. The initial panel implementation sent a string; it now sends a number for both the first and subsequent pages. Deployed 06:30; browser reload showed no console errors. Successful actual-model result handoff remains pending; cancelled-child panel rendering is verified.
