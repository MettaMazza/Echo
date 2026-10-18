# Message composer, queue and mid-turn guidance

Enter sends; Shift+Enter inserts a newline. IME composition and held-key repeats do not submit. During a running reply the composer remains editable. Queue/Enter saves a separate next-turn message locally, with its stable submission identity, conversation and attachment references. Queue dispatch requires this conversation to remain open; ordinary queued messages wait for explicit Send next after a stopped or failed run.

Steer now sends text to `/api/steer` against the existing run ID. It does not call Stop, create a new run, or replace the original objective. The server archives `guidance_received` idempotently, checks permission to control that run, and rejects a new guidance request after the run has finished. The UI distinguishes sending, received and applied; guidance remains visible in that run's Thinking trace. Unknown network outcomes preserve the request identity for retry.

The worker incorporates pending guidance as `user_guidance` in the accepted parent chain. During streamed inference, it checks at event boundaries, closes the obsolete generation stream and records `agent_recovered`, then regenerates within the same run using the objective, completed tool observations and guidance. Providers do not mutate a prompt already executing; this is a generation restart inside the ongoing task. A blocked prefill/read reaches this check when the provider emits its next event. Publication rechecks under the writer lock so late guidance cannot be silently overtaken by a final response.

Already executing tools finish with their real outcomes. Unlaunched calls from the old batch receive explicit not-executed observations and are reconsidered after guidance. Nothing claims to undo completed effects. Text guidance is supported; attachments remain a separate queued message. Ordinary Stop continues to cancel the run.

Implementation: `app/ui/client.ep`, `app/web/cancellation.ep`, `app/web/routes.ep`, `app/providers/guidance_pending.ep`, `app/providers/archived_stream.ep`, and the agent generation, runner, message and tool dispatch modules. No schema change. `tests/web/test_midturn_guidance.ep` exercises incoming guidance after a generation request starts, idempotency, outsider denial, same-run continuation and final completion without cancellation.

Repeated-tap repair: while the guidance request is in flight, Steer now is disabled and labelled Sending guidance. After acknowledgement the cleared composer keeps it disabled until new text is entered. The saved receipt distinguishes received/waiting from applied, including after refresh; an unconfirmed request offers Retry guidance with the same original identity. A ten-tap browser regression recorded one request, and the retry regression preserved exact identity and text.


### Confirmed steering leaves the queue — 16 October 2026

Queued text has its own Steer now action. Acknowledgement removes the exact queued submission, preserving other queued messages and a new composer draft. Archived application receipts perform the same cleanup if the HTTP acknowledgement was lost. Retries retain the original guidance identity. The tooltip describes same-turn guidance, not stopping and starting another turn.

The streaming guidance lookup previously scanned the complete event archive before each provider event. A persistent event kind/sequence index now supports both intake and application lookup. On the live archive, ten-query averages improved from 109.33 ms to 0.015 ms per check. This measures database overhead, not total model response latency. The index is also created by archive initialization for new installations.

Verification: emitted browser regression covers two queued messages, exact confirmed removal, unrelated draft preservation, repeated taps and lost acknowledgements; native streaming regression applies two successive guidance messages to the same run and preserves tool interruption behavior. Evidence: evidence/phase2/steering-queue-latency-20261016/.
