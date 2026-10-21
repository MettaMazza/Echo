# Historical reply controls — 21 October 2026

Older delivered replies predate the immutable per-chunk final bindings used by
branch, edit, regenerate and retry. A host maintenance command now restores
those bindings from retained Speak action provenance. It performs no network,
model, message-edit or delivery operation.

The repair requires a confirmed sent Speak anchor, its archived final in the
same run/session, the exact original intake and channel, canonical contiguous
chunk identities, confirmed delivery for every chunk, and concatenated text
exactly equal to the archived final. It accepts the original first-Speak layout
and the later final-chunk layout without rechunking either. Incomplete,
conflicting or mismatched evidence is rejected; no newest-answer guess is used.
The complete group is checked before bindings are appended in one journal
transaction. Existing bindings retain their body hashes and cannot be replaced.
Current access and source ownership remain enforced by the normal controls.

`app/discord_backfill_finals.ep` accepts existing journal/archive paths, bot ID
and `--apply`. It reports each anchor's outcome. A successful process exit is
not a claim that every anchor qualified: inspect the individual results.

Native expanded acceptance passed in 12.242 s, binary
`096e4ac431691c6ce5ea3c10055b3625d3d569f37dd9a861d36cf23285bf56d3`.
Coverage includes Unicode multipart replies, confirmed delivery, incomplete
chunk rejection without partial bindings, mismatched text rejection, exact
lookup/channel filtering, replay, reopen, later finals and revoked authority.
Private receipt: `final-backfill-20261021/expanded-1792609300958997000`.

On an archive copy, all 133 historical anchors qualified, adding 178 chunk
bindings. All 2,373 outbox rows and 154 action rows remained equal to their
originals. The live application, preceded by database backups, produced the
same 20-to-198 binding count and preserved every original outbox, action and
binding row. CLI SHA-256:
`33ec7a7d0e29ef87e6a1a9c40e529b834fc6260bf230ca162f0e0ac540cf88e4`.
Private live evidence: `final-backfill-20261021/live-1792609335269399000`.

Actual Discord `/branch` interaction `1562541712176455750` used the repaired
historical reply `1562497673829814293`, returned `branch-created` and visibly
confirmed the new branch. The follow-up context check is in progress. This
closes neither the remaining Discord UI work nor Phase Two as a whole.
