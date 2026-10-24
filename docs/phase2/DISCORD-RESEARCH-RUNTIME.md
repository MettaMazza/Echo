> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Discord public research runtime — 21 October 2026

The Discord worker launcher omitted ECHO_RESEARCH_RUNTIME and ECHO_RESEARCH_ROOT. The native schema builder consequently omitted web_fetch, web_search and web_follow even though the implementation existed. A real dictionary request exposed this deployment omission. It was not an admin-role denial.

The launcher-only live test failed: the isolated turn process also discarded research settings. app/discord/process.ep now maps explicit ECHO_DISCORD_RESEARCH_RUNTIME and ECHO_DISCORD_RESEARCH_ROOT to the tool settings, preserving the scoped keys for subsequent children and never falling back to generic host research settings. The actual subprocess regression covers both positive mapping and exclusion of generic host values when Discord settings are absent.

The launcher now explicitly supplies the qualified fetch_public helper and a dedicated Discord research store. Only the executable is shared in purpose with host research; the host research archive is not imported. The helper is rebuilt from app/fetch_public.ep. Its SHA-256 is 096ce588e7edfed9938e448f45b39ee61b006e7e35173abf163e477cc24a0cc4. The existing native URL, public-address, redirect, cancellation and scoped attachment checks remain active.

Direct helper acceptance: example.com returned complete; loopback returned non-public-destination. Actual Discord fetching and retained-document reading passed. The following search turn correctly recalled the marker and heading, then exposed a shared-handler authorization bug: it checked the hard-coded name web_fetch even for web_search/web_follow. The correction uses the exact name from the immutable ledger row, already restricted to these three operations. Native alias regression passed for all three names, including return to ready after argument errors. The repaired live retest searched successfully (10 results), read the search document, followed its wrapper, detected empty extracted content, fetched the decoded dictionary destination directly and read the actual entry. The marker and prior heading survived the restart. Both final reply parts were delivered and seen in Discord. Search-provider availability is independent of schema availability and must be reported from actual results. No universal research success is claimed.

Private regression evidence retains the disputed conversation with immutable event IDs and digests. Required behavioural checks are faithful attribution, no invented motives or certainty about unseen sources, corrections carried forward, and conclusions that remain responsive to evidence and correction. The current prompt already contains related guidance; this evidence does not establish that another prompt sentence will fix the behaviour. Final prompt revisions remain subject to joint operator review. No private conversation text is included here.

## Deployment contract

Set `ECHO_DISCORD_RESEARCH_RUNTIME` to the qualified directory containing `fetch_public`, and `ECHO_DISCORD_RESEARCH_ROOT` to a dedicated Discord-only research directory before launching `discord_agent`. The child maps those values to `ECHO_RESEARCH_RUNTIME` and `ECHO_RESEARCH_ROOT`. Setting only the generic keys in the controller is insufficient. Keep the scoped keys available for recursive child launches. Do not use the host research store as a fallback. The maintenance launcher now supplies both scoped keys explicitly.

The process test deliberately sets conflicting generic host values and proves that only the scoped values reach the actual child. It then removes the scoped values and proves that generic host values remain absent.

Acceptance limit: following the search wrapper did not itself retrieve the dictionary page; the model recovered by fetching the decoded destination. Do not report this as transparent redirect-wrapper support. The long-history cold/concurrent latency and behavioural prompt review remain open.
