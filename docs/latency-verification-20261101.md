# Latency verification — 1 November 2026

The deployed agent SHA-256 is `436580ab7be87e62b6aeeb83fcb63ca943cc6278305121519a048a7d8ad670c0`, release `latency-audit-journal-owner-20261101`.

Implemented changes: GC-safe SQLite waits, opening existing audit directories without redundant metadata writes, exact standing-memory prefix encoding, releasing completed foreground output before joining extraction, and durable journal receipts for permission reads instead of separate files. Selection, provenance, permission checks, TLS controls and highest foreground reasoning remain intact.

Completed real Discord measurements, verified in the native UI:

| Workflow | Before | Deployed |
|---|---:|---:|
| Ordinary follow-up: “And 20 plus 24?” after “What is 19 plus 24?” | 104.598 s | 28.013 s |
| Memory capture during that follow-up | 66.320 s | 8.200 s |
| First brief test after deployment, with cold prefix cache | 116.987 s | 105.304 s |

A fresh arithmetic request took 73.373 s while a separate live community request and memory jobs overlapped. Its 14,317-token prefill took 51.781 s. It is not an isolated comparison with the earlier 44.581 s arithmetic result. Cold-cache and concurrent-inference latency remain; these measurements do not establish that every request is below 30 seconds.

Native tests exercised retained GC roots and callback safety, exact recall round-trips, foreground release before extraction completion, and permission transport/durability failures. Three synthetic permission observations on the same external drive took 2,203 ms with file receipts versus 128 ms with journal receipts; this isolates receipt overhead and is not an end-to-end timing.

The initial candidate was rejected by the installed protected-source digest check. The enrolled owner's unattended deployment helper now supports source-version pin updates without changing the fixed acceptance tests, installed evaluator or protected path set. It journals the policy update, restores exact original bytes on rollback, and reloads the controller before health checks. The subsequent deployment passed acceptance and integrity checks without user attendance.

Final checks found all 13 expected services running, no active inference/tool/claimed Discord/speech work, and preserved cancelled/backlogged requests. Model and speech servers were not restarted. No external drive unmount or existing-data migration was performed.
