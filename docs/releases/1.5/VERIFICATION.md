# Verification, limits and publication

## Evidence levels

Source presence establishes implementation location. Compilation establishes that the selected source builds under the recorded toolchain. Native fixtures establish their exercised assertions. Live HTTP/model/archive/session checks establish the recorded path. Physical-browser/device testing is a separate claim. None of these alone establishes every future workload or completion of Phase Two.

The exhaustive source references are lexical inventories, not an assertion that every branch was manually proven. The test reference includes fixtures and diagnostics; 469 files does not mean 469 tests were rerun for this documentation release.

## Current deployment and retained measurements

Application release: `phase2-stable-session-recall-20261018-r2`.

Server SHA-256: `e61a7faea3a2a68910a61344e5f7b57234d22c1061fc12b2f42d61aedcb19d1b`.

Browser bundle SHA-256: `6be11274b8013e795e818251a46b5c70ec815c6e17454b85cbf9b53162ede70b`.

| Recorded workload | Result |
| --- | --- |
| Prior actual new-session greeting | First visible text 21.0s; final 22.6s |
| Prior follow-up | First visible text 20.7s; final 24.5s |
| Prior follow-up fresh prefill | 9,641 tokens; 14.18s |
| Final verified follow-up prefill | 1,315 tokens; 2.28s |
| Final verified follow-up | First visible text 11.66s; final 16.97s |
| Final warm-cache new session | First visible text 7.69s; final 9.14s |
| First request after tool-schema change | Cold prefix rebuild; final 47.94s |
| Final preparation stages | 3.06–3.80s combined indexing/context capture |

These are different dated workloads, not a controlled universal speed guarantee. Reasoning and output length contribute to final time. The earlier 14-second preparation delay was not reproduced or precisely attributed; the evidence does not prove every future delay eliminated.

The live verification used compiled browser submission functions, actual HTTP service, inference, memory, tool/archive observations and session API final replies, with DOM doubles. It was not a new visual phone-browser test. The native stable-recall fixture passed stable base/delta/invalidation/reopen and read-versus-write checks while emitting optional-fixture archive diagnostics; retain that caveat.

The final idle receipt recorded HTTP 200, no active runs, four idle inference slots, an idle embedding slot and no development/test workloads. That is an observation at the receipt time, not a promise that the user has not started work since. This documentation release leaves the running system untouched.

## Evidence preservation

The machine-local maintenance receipts include `user-new-session-times-20261018.json`, `stable-session-final-live-times.json`, `stable-session-final-proof.json`, `stable-session-warm-new-session.json`, `stable-session-idle.json` and the error-feedback HTTP/tool/browser results. Public verification metadata records hashes and scope, not the private conversation payloads. Dated repair documents preserve earlier measurements and limitations. Older release-verification metadata is retained as historical evidence.

## Publication safety

Review the candidate tree for credentials, private keys, runtime state, conversation exports and accidental model/binary assets. Publish examples and schema, not live configuration or database rows. Private identity originals and tokens remain excluded. The repository is public; public availability does not itself grant a license where no project license has been chosen.

The local and GitHub histories have different roots. Publication preserves remote ancestry and applies the reviewed logical changes on top of the existing remote head. The final ref update uses an expected-head lease without force. Compare complete Git tree identities after fetching the result; local and published commit IDs need not match when their parents differ. No claim is made that all historical local commits are published individually.

Documentation checks cover source identities, JSON parsing, local links, patch whitespace and publication-tree equality. They do not rerun expensive inference or restart the application for a documentation-only change. A publication receipt with exact commit/tree IDs is retained locally after success.
