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

The native `/project` interface is registered and deployed. Actual private Discord
list, create, read, use and clear interactions passed on 21 October 2026. A downloaded
read response matched its recorded SHA-256 and retained the Unicode name exactly.
Native tests additionally cover task/file edits, stale revisions, frozen retries,
actor substitution, retained originals and archive integrity. Live task/file editing
and concurrent transport fault acceptance are not claimed by these checks.

Controller evidence: `../releases/1.75/discord-project-control-2026-10-21.json`.
Command evidence: `../releases/1.75/discord-project-commands-2026-10-21.json`.

## Native Discord command

The `/project` handler is implemented with these native string fields. Registration and the live actions listed above passed. The conversation ID is supplied as `session`
from `/history`; no model turn is needed to operate the editor.

| Action | Fields and effect |
| --- | --- |
| `list` | Lists projects and the selected project for this audience. |
| `read` | `project`: returns the current complete manifest and revision. |
| `create` | `name`, optional `instructions`: creates an empty task/file plan. |
| `edit` | `project`, `revision`, and `name` and/or `instructions`: edits project details. |
| `use` | `project`: selects it for the conversation. |
| `clear` | Clears this conversation's selection; retains the project. |
| `task` | `project`, `revision`, `task`, `title`, `status`: adds or replaces one task. Status is todo, doing, done or blocked. |
| `remove-task` | `project`, `revision`, `task`: removes that task from a new revision. |
| `files` | Lists retained file IDs, names and hashes in this exact audience. |
| `file` | `project`, `revision`, `attachment`, optional `label`: adds or replaces a file reference. The controller obtains its actual hash. |
| `remove-file` | `project`, `revision`, `attachment`: removes a reference, preserving the uploaded original. |

Edits use an observed revision from `read`, preventing a stale editor from replacing
newer work. The full response is a private JSON attachment on the original command
response, with immutable response bytes and SHA-256. It is not posted to the public
channel. Uncertain uploads are inspected before retry. Over-limit responses are
explicitly rejected without truncation; any already saved edit remains retained.
The authenticated interaction supplies the operation identity, account and channel.
No raw manifest JSON is required to create projects, edit details, or manage tasks
and files.

Known display limitation: Discord’s native JSON attachment preview currently shows
non-ASCII text incorrectly despite an explicit UTF-8 content type. Stored and
downloaded bytes were verified correct. This preview issue remains open.
