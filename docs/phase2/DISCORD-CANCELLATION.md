# Discord source-bound cancellation

Cancel button deployed and live-qualified on 21 October 2026. Slash-command
registration/interaction acceptance remains separate and pending.

The Cancel button is also implemented on the request's durable queue/active
status reply. Its immutable identifier binds to the original message, sender,
channel and guild, and the button is disabled once that request is terminal.
Authenticated component interactions recheck current channel access before
requesting cancellation. The button does not pick the latest run. Native tests
passed queue-position-one availability, foreign sender/channel/message denial,
restart/replay and terminal button state. Actual Discord active cancellation
stopped its exact run and disabled the button. A second queued request was
cancelled without interrupting the first active request; both confirmations and
terminal states were observed. Originals remained saved.

New completed responses put Speak on their last text chunk. It still speaks the
whole accepted response. Retry recovery preserves previously published legacy
first-chunk bindings rather than duplicating or silently moving existing buttons.
The four-chunk Unicode, retry, reopen and resumed-final fixture passed; live
multi-chunk acceptance passed: a two-chunk generated story retained the complete
final sentence, both chunk boundaries ended in punctuation, the first chunk had
no Speak action, and the second had exactly one. Long speech delivery through
that final action passed: its input contained all 2,426 characters of the
accepted final, and the shared 158-second audio attachment appeared on the last
chunk. This does not assert a new word-by-word transcription audit of that story.

`/cancel message:<original message ID>` identifies the user's original channel
message, rather than whichever run happens to be current when execution occurs.
The authenticated interaction supplies actor/channel/guild. The command checks
fresh Discord channel access and current local principal availability. The local
operation checks that all three match the retained original source.

The journal writer serializes cancellation with ordinary dequeue and steering.
A queued source receives a terminal cancelled job without being submitted to the
model. A claimed/background source receives a durable cancellation receipt; the
existing source monitor observes it and closes the provider cancellation pipe.
The response reports a request to stop, not a claim that in-flight work has
already stopped. Previously completed outputs and originals are preserved.

Repeated requests do not retarget a newer run. A steering source is rejected
with directions to cancel the original active request. Completed source jobs
are reported as already finished. The cancellation receipt survives restart.

Initial native controller tests passed queue removal, exact active-source
barrier, foreign actor/channel/guild rejection, restart replay, and rejection
of steering and completed source jobs. Command definition/binding, a blocked native socket read through the production
turn monitor, and existing source-guard regressions now pass. Current-authority,
interaction replay/failure for the slash command remains required. Button
active/queued cancellation and idle deployment have now passed. This does not
claim complete UI parity or completion of Phase Two.
