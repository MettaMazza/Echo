# Compaction repair cycle recovery — 29 October 2026

The Discord request `1565403527524913313` initially inherited a stopped autonomous run whose projection still said ready. That already-failed run was reconciled to failed through the native archive/state transition API, retaining its history. Once admitted, the new request processed 211,651 uncached input tokens (244,419 including cached tokens), generated three tool calls, and entered recap generation/review/repair. It stopped with `recap-no-progress` before a final answer.

The repeated recap was not proof of an incorrect citation. The review asked to move two citations to a message that did not contain the relevant corrections. The original cited messages did contain them. Repair preserved the correct citations, producing a previously seen candidate. The old loop treated the repetition as an unconditional terminal dead end.

The updated compaction loop gives each repeated candidate digest one independent review per invocation, retaining the normal structural, original-source, attachment, authority, provider-fit and checkpoint activation gates. Repeated failure after that adjudication still stops; repetition never becomes automatic approval. The independent review receives all frozen originals plus a host-selected exact lookup of candidate citations. It does not inherit an earlier review as authoritative.

Validation used the real retained stalled job. Exact source selection was checked against its 151 original records: the lookup selected 10 original records, included both disputed citations, and could not introduce an invented source ID. Both the recovery helper and Discord agent compiled. Native recovery produced an independent passing review and activated checkpoint `compaction:ee8908d1-0e46-4b02-9ac9-84c8bb1c5a08:checkpoint:2626f3f5-b0d1-47c1-aa53-5e69475c65f5`. Recovery required further reviewed edits; the first independent review was not an immediate pass.

Release `/Library/Application Support/EchoRSI/releases/compaction-cycle-20261029` passed protected integrity and fixed acceptance, deployed through the enrolled password-free Echo helper, and restarted the eight application services. The original request was resumed with its immutable identity and observed tool ledger intact. No session reset, history deletion, review-gate bypass or reasoning reduction was used. The resumed generation used approximately 34,740 context tokens.

Final verification: the original run completed and its final reply was sent as Discord message `1565423842812239975`. Web health remained ready.
