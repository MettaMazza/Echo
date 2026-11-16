# Native latency regressions

Build `app/discord_agent.ep` with the SQLite GC safepoint patch. The reproducible compiler update is in `tools/toolchain/build_sqlite_gc_compiler.py`; it verifies runtime source hashes before applying the patch.

Pass the resulting `discord_agent_compiled.c` to each test:

```sh
python3 tests/runtime/test_sqlite_gc.py "$generated"
python3 tests/runtime/test_latency_paths.py "$generated"
python3 tests/runtime/test_foreground_order.py "$generated"
python3 tests/runtime/test_receipts.py "$generated"
```

The tests compile the actual generated implementation with synthetic native fixtures. They do not contact Discord or need production credentials. Temporary build files are removed automatically.

- SQLite contention: fifty full collections while a writer waits; retained roots, parked-thread balance and allocating row callbacks remain correct.
- Directory traversal: existing paths perform no mkdir/fsync; newly created paths remain durable; links, traversal, failed flushes and concurrent creation are checked.
- Recall: every selected record and provenance field round-trips, while standing records retain their prefix when query-specific records change.
- Foreground release: completed output and a follow-up claim become available while automatic extraction is blocked; extraction still persists and joins.
- Permission receipts: the real transport uses libc memory streams and durable immutable SQLite evidence; the mock network checks fixed origin, TLS verification and redirect/proxy controls. Denial, invalid JSON, transport failure, rate limits and receipt-write failure do not grant access. Mutation/file receipts still perform both file flushes. Expected failure cases print database diagnostics.

Native tests are not an end-to-end latency claim. Completed Discord replies and model timings must be measured separately. Owner-authorized protected source upgrades use the scoped unattended deployment path in `tools/operations`; fixed acceptance tests and rollback checks remain enforced.
