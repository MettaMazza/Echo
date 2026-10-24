# Echo 1.9 — community, continuity and voice development update

1.9 publishes the accumulated source changes after the last pushed community revision, `72fcd9c4b297c0679743ec043547309d7e58d6d7` (22 October 2026). The corresponding local commit `5823c562391be5c36a170dc4c43d75659737bd82` has the identical tree, `70f1085f97d5fbd209a93d8dda0e5f510e354044`. Publication follows the remote ancestry without rewriting history. The older default-branch 1.75 landing page is not the last pushed community baseline.

**V1 remains the future production milestone: Phase Two complete and production-ready.** This update does not declare that milestone complete. No completion percentage is published: earlier conversational percentages used inconsistent weighting and are not forensic measurements.

- [Feature matrix and source map](FEATURES.md)
- [Changes grouped by behavior](CHANGES.md)
- [Every changed file](FILE-CHANGES.md) and [exact before/after hashes and changed definitions](change-manifest.json)
- [Verification, evidence and limitations](VERIFICATION.md)
- [Discord guide and public links](DISCORD.md)
- [Remaining V1 work](REMAINING.md)
- [Complete application reference](../../reference/APPLICATION.md), [tests](../../reference/TESTS.md), [source manifest](../../forensics/source-manifest.json)

The manifest includes application, test, support and documentation changes. Generated reference/manifests are excluded from recursive self-hashing and remain covered by Git history. Function changes are lexical; a listed test reference is not an executed pass. Local runtime/configuration changes are documented in the change narrative; private runtime artifacts are not committed.
