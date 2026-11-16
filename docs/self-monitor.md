# Echo runtime self-monitor

The self-monitor makes a separate, background model call to review recorded execution and produce actionable findings for Echo. It does **not** submit RSI candidates, edit code, deploy changes, send messages, or resume conversations. Echo decides whether a finding is relevant and any subsequent change uses the existing RSI authority, consent and test gates.

Each finding records an observation, a separately labelled causal hypothesis, severity, confidence, exact evidence IDs, a recommendation and a behavioural acceptance test. A review with no findings is valid. Captured reasoning is treated as emitted text, not proof of an internal cause.

## Evidence and privacy

The worker reads bounded pages of settled runs from the existing web and Discord archives. These records include the originating request, captured reasoning/content, tool calls and results, provider dispatch/stream events, failures and cancellations. Consecutive streamed text deltas are assembled without another model call; the original range endpoints and counts remain attached. Full originals remain in the existing archive. Large payloads and incomplete pages are explicitly marked; duplicated model request bodies are omitted.

The native `self_monitor_bridge` derives access from the initiating authenticated run using the existing history and scratchpad gates. Scope is checked before review and again before publication. Findings are saved as `self-monitor:<digest>` notes in the existing scoped scratchpad. Private owners can see their findings across their own sessions; shared notes remain within their exact existing audience. Admin status does not select another person's private memory. Unscoped daemon logs are not pooled across users.

Echo receives a short tail indicator listing the latest three accessible notes. The full report is read through the existing scratchpad tool when relevant. The monitor does not overwrite Echo's `me` note or alter the tool schema or stable prompt prefix.

## Scheduling and storage

The worker checks once per minute and starts a review only while configured model endpoints and conversation/tool workers are idle. Reviews use the engine's existing `echo_background` foreground-priority scheduling and do not change foreground reasoning settings. Monitor requests set `cache_prompt=false` to avoid publishing each review into the persistent prompt cache; foreground session caching remains enabled. Maintenance defers monitoring. There is one worker lock; a restart resumes its cursor without replaying historical requests. Initial activation starts with future completions.

The model has no execution tools. Output is structurally validated, citations must belong to the supplied evidence, duplicate publication is create-only, and credential checks run before publication. Recent same-scope finding summaries are supplied to discourage repeated reports.

Evidence pages are bounded; model responses are capped at 24 KB for publication. At most 16 new reports are saved daily, approximately 384 KiB of report text. Coordinator state has an 8 MiB SQLite page cap, at most 128 queued reviews and seven days of daily counters. Temporary report files are private and removed after publication; a capacity guard stops further creation if stale files accumulate. Current status replaces one private `status.json`; private error logs rotate at 64 KiB with one backup. No session backups or full-log copies are made. Monitor state stays at `/Volumes/One Touch/Echo-private/self-monitor`.

## Operation

The installed job is `local.echo.self-monitor`, managed through the existing scoped Echo administration helper. Its root-owned configuration is `/Library/Application Support/EchoRSI/monitor/config.json`. Application indicators use `self_monitor` in the existing QoL configuration. The service uses Echo's existing protected launcher with a pinned canonical interpreter, permanently drops to the Echo user, and runs the pinned release's native bridge and Python review worker, not a downloaded classifier or another loaded model.

Source: `app/self_monitor_bridge.ep`, `app/monitor/context.ep`, `tools/monitor/`, and `tools/setup/self_monitor_installation.py`. Tests: `tests/monitor/test_review.py`, native scope/publication and context fixtures, and synthetic model reviews. These tests do not send Discord messages or replay user requests.
