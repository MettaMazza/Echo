> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Builtin tool discovery contracts

Step 08 implementation, 16 October 2026. `tool_search` returns the schema plus a versioned contract: capability, minimum role, operation effects, recovery guidance and runtime dependencies. These describe existing implementations; they grant no permissions. Invocation still checks current authority and dependencies.

Catalog cursors bind both schemas and contracts. A changed contract invalidates an old cursor and discovery receipt. Search is case insensitive. Results remain restricted to the same loaded builtins advertised for that run. Dormant candidates are not represented as active tools.

Evidence: `evidence/phase2/tool-contracts-20261016/verification.json`, source hashes, native build logs and isolated archive snapshots. Discovery and registry regression binaries returned 0; both archives pass SQLite quick_check. The registry deliberately corrupts a receipt and verifies rejection, so its integrity-error diagnostic is expected. The original evidence above predates later deployments. Current extension and deployment evidence follow; this document does not close the remaining step 08 families.


Workbook and archive dependency reporting (16 October): discovery now names macOS libarchive, libxml2, scoped descriptor access and hashing where applicable. Workbook records distinguish reading cached results, creating new files and editing explicit XML parts with a preserved source. Recovery records cover stale sources, exclusive destinations and uncertain publication. The authority-filtered discovery/streamed-dispatch/reopen regression passed both with and without a configured workspace; unconfigured tools remain absent. Evidence: tables-20261016/workbook-contract-final-result.json. The earlier failed fixture expected workspace tools without configuring a workspace; that test setup was corrected, not the visibility boundary. Deployed phase2-workbook-contract-20261016-220516; local and LAN status returned 200 (host HTTPS trust bypass); model process preserved.

Git, Ernos build, local SQL and plotting contract reconciliation: discovery now names each operation's actual runtime dependencies and recovery semantics. Git describes native hooks/helpers and uncertain remote effects; build distinguishes harness compilation from execution; SQL distinguishes an uncommitted rollback from earlier durable commits; plotting directs uncertain-publication inspection and explicit attachment publication. Schema and capability permissions remain unchanged. Catalogue digests include these contract bytes, so a changed description invalidates an earlier discovery cursor rather than mixing versions.

Deployed in `phase2-contracts-20261016-233838`. The native discovery/dispatch/reopen regression passed with workspace configuration present and absent; both status endpoints returned 200 and the model process was retained. Evidence: `evidence/phase2/final-tool-contracts-20261016/`.
