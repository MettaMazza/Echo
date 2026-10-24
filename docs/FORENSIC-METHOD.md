> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

## 1.9 forensic coverage

`tools/document_release.py` reads every publishable first-party source/fixture byte, records SHA-256 and lexical imports/definitions, maps test references and compares changed files against the exact last pushed community tree. It includes Ernos, native C bridges and Python test/documentation harnesses. Generated manifests exclude themselves from recursive hashing. The Git commit/tree records the complete release. This is exhaustive file accounting, not a claim that every code branch was manually audited or every fixture executed. Measured acceptance is separately linked in [verification](releases/1.9/VERIFICATION.md).

# Forensic method and limits

The scope is the complete first-party application and test tree, retained compiler integration, deployment description, database schema and local evidence catalogue. Third-party source trees and weights are dependencies, not silently included in the first-party file count.

Every `.ep` file under app and tests is read in full to produce byte length, line count, SHA-256, import edges, definition signatures and source comments. Application/test references are lexical. This produces complete navigation and file identity; it is not a proof of correctness, semantic test coverage or a claim of manual line-by-line review. The subsystem narrative is based on the actual entry points and control flow.

The local evidence catalogue records file paths, lengths, timestamps and SHA-256 values without publishing original payloads. Digests identify exact observed bytes but do not prove that a test passed. Files marked unstable changed during hashing. Live logs may change after any snapshot. The catalogue is not a signed external chain of custody. Original evidence remains on the host.

Database schema is exported without rows, credentials or user content. Integrity checks and aggregate run counts are time-stamped observations. Model health probes and browser checks are observations at a particular time; they are not guarantees of later uptime.

Older handovers contain superseded scope gates, process IDs and incomplete-status statements. Current status plus the owner's later explicit scope correction govern the handover. Failed attempts remain failures in the record. Fixture, native unit/integration, direct-provider and browser/live-user observations are labelled separately. Source build success does not substitute for real hardware/model behavior.

When publishing, reviewed summaries and source manifests are committed; raw archives, logs, screenshots of private conversations and credentials are not. The repository retains historical commits from its previous contents because the owner explicitly selected that option. Making it public also exposes that history; the previous content requires review, not just the new tree.
