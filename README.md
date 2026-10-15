# Echo

A locally hosted, multi-turn AI agent written in plain-English Ernos. The current installation runs on an Apple Silicon Mac, with llama.cpp and MLX inference adapters, a browser UI, a durable conversation archive, source-linked compaction and session-scoped tools.

Start with [current status](docs/STATUS.md), [architecture](docs/ARCHITECTURE.md), [build and test](docs/BUILD-AND-TEST.md), and [operations](docs/OPERATIONS.md). The [documentation index](docs/README.md) includes a complete per-file/function reference and forensic evidence inventory.

This repository contains the application and tests, compiler integration overlay, configuration examples and documentation. It does not contain model weights, private identity material, conversations, device credentials, TLS keys or the machine's active environment. Configuration examples require real local paths; this is not a one-command portable installer.

The original Phase One plan is preserved in `plans/`. Later user scope decisions take precedence: training and advanced research features belong to Phase Two; microphone permission testing was explicitly deferred. See the current status rather than treating old chronological handovers as current blockers.

Application source is in `app/`; tests and fixtures are in `tests/`. Native executables and browser JavaScript are generated from Ernos. Dependency implementations remain upstream dependencies.
