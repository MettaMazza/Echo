> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Empty reply completion incident — 16 October 2026

A live run ended with provider finish_reason=stop, no tool calls, empty content and a nonempty reasoning trace. Generation publication classified every no-tool response as assistant_final/completed, so the UI displayed no answer and speech synthesis received empty text. This was not a missing user submission.

Generation publication now retains the response and reasoning as a recovery event and continues the same run with an explicitly application-generated notice. A repeated empty response becomes a visible agent_failure with reason empty-model-response rather than looping or falsely completing. Ordinary answers and tool calls retain their existing handling. The continuation message uses a template-compatible conversational role, explicitly labeled as runtime status rather than user instruction; this Qwen template rejects mid-history system messages.

Native generation regression passed ordinary final/tool/length handling, first-empty continuation, repeated-empty failure and archive verification/reopen. The initial live recovery encountered that template rejection during token counting; no generation occurred on that failed attempt. The corrected request passed preflight, completed generation and issued actual tool calls for the original task. Its final visible answer is now confirmed: the original run completed with an assistant_final containing 2,715 characters.

`app/recover_empty_reply.ep` repairs only verified empty completed replies (or resumes their recorded recovery), preserving the original event and completed tool ledger. It refuses an active worker or another active run in that session and acquires a new normal worker claim. Recovery was checked on an internal SQLite backup first; quick_check passed, original final remained, and no pending tools existed. Live recovery is the same original run, not a resubmitted prompt.

Deployed server: phase2-empty-20261016-211642. Private evidence: evidence/phase2/tables-20261016/empty-*. The live recovery worker 25495 has exited after completion. The archived final answer is nonempty; phone rendering has not been independently observed. The validated archive-creation implementation was also present in this build and is now documented and committed.
