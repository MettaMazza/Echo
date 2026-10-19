# Thinking audio acceptance

**Latest r29 deployment / r27 session replay:** see [r27 session replay](session-replay-r27.md). The owner-history replay exposed an audio-capability denial and high end-to-end latency after the earlier r24 checks. r27 preserves native reasoning and Warm, corrects capability/presence context, and separates interactive/background model work. Measured replies still take tens of seconds; the earlier first-output figures are not complete-turn latency.

**Current status: passed within the recorded scope.** The selected-WAV sequence, post-reload listening, two cancellation recoveries, fresh generated-Warm listening and final live four-turn/reload checks are recorded in acceptance.json. Historical failures below are retained, not current pending work.

Run with the actual selected private Echo voice WAV, not a greeting or substitute fixture. Record the exact private audio digest, loaded Thinking model and request provenance privately; publish only non-sensitive outcomes. Keep the reference, audio, embeddings and conversation records out of GitHub and Discord.

1. Attach the current voice WAV in a fresh browser conversation. Ask what it says and for an acoustic description. Verify native waveform reaches the exact loaded Thinking provider; compare transcription to the private audition script. Report errors, do not coach the answer.
2. Without attaching again, ask a specific follow-up about words and sound. Verify access to the prior audio or an actual attachment-listen tool and an evidence-grounded reply.
3. Ask a different follow-up and change the requested response format. Verify the reply responds to the new instruction rather than repeating its previous text.
4. Reattach the same actual WAV with a new request. Verify the bytes and native input again, and verify a relevant response.
5. Reload the browser, reopen the conversation, and ask another audio follow-up. Verify source recovery, persistence and original-waveform access.
6. Ask an unrelated short question afterward. Verify normal conversation resumes without repeating an audio refusal or previous answer.
7. Exercise cancellation and a subsequent reply. Check visible reasoning/tool activity where actually emitted, saved messages, cache behaviour and latency across the sequence.

Failure regression: the earlier Instruct run returned the same audio-denial sentence on five consecutive turns even though all five answering requests included native audio. Presence of waveform bytes alone is not a listening pass. Successful UI playback alone is not a listening pass. A transcript alone is not native hearing. Do not force a gender description, emotional preference or approval. Capture exact errors and observed limitations.

Only mark the sequence passed after observing each result. A model download, HTTP success, single greeting, compilation or one correct answer is insufficient. Keep the working prior model available until Thinking passes the complete release acceptance gates.

### Cancellation recovery finding (19 October, later r22 check)

After a live audio-analysis turn was stopped, the next exact-sentence request reached the provider in the correct order, but the model answered the stopped analysis request. The archived cancellation event had been omitted from rendered conversation context. This remains a failed end-to-end acceptance check; correct message delivery alone is insufficient. A general context change now retains an application cancellation notice without removing earlier messages. Its native regression test passes for notice ordering, valid JSON, retained earlier request, and no false cancellation label on the new active request. Browser verification of the change is pending. No Thinking promotion or final release tag is claimed.

The r23 browser candidate then returned the requested new sentence correctly. A second active generation was stopped, followed by an unrelated arithmetic request; the answer was correct. The archived next request contains the cancellation notice. Fresh browser Speak used the current Warm controlled-prosody profile, produced 4.613 seconds without clipping, and repeat Speak reused the saved job. Independent listening of that generated WAV in a separate conversation is still under test. These observations do not imply correctness for every future model response.
