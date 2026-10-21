# Discord project controls

The scoped project controller is implemented in `app/discord/project_control.ep`.
It reuses the browser project writer, manifests, immutable revisions and
selection context. It does not create a parallel project database or start a
model turn to carry out a human project edit.

The authenticated Discord device, guild and channel are supplied by the
connector. Under the same database transaction as the project operation, the
controller requires membership in the session's exact current channel audience.
Both personal channel silos and explicitly joined shared groups use this check;
being an administrator does not substitute for membership.
It excludes delegated child sessions, old membership revisions, private
sessions and sessions from other accounts, guilds or channels. JSON cannot
supply an audience or a different authority. Existing project scope and file
digest checks still apply.

Supported controller operations match the browser: create, read, list, update
and select. Selecting an empty project clears the conversation selection.
Updates require the observed revision; retries retain their original identity
and do not duplicate or reorder events. Selecting a project in another
conversation within the same audience makes its current manifest available to
the next model request. Tasks are authored planning state, not proof that work
has run.

This is not yet a deployed Discord project interface. Native command/form
bindings, private response rendering, project/task/file editing controls and
actual multi-turn Discord acceptance remain required. No project command is
registered before its complete handler is qualified. The live connector and
Host service remain unchanged by this source addition.

Native tests passed create/read/list/update/select, retry and stale-revision behavior, next-run project context, account/channel/guild/private isolation, explicitly joined shared groups, removed-member and obsolete-audience denial, archive reopen and revocation. The receipt is `../releases/1.75/discord-project-control-2026-10-21.json`. No live Discord interface acceptance is claimed.
