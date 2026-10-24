> **1.9 documentation:** [Current update](releases/1.9/README.md) · [feature status](releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Admin contexts

Group creation offers **Who can join → Invited admins only**. The browser submits an access mode; the server derives role from the authenticated principal and creates a reserved admin-scope origin. Neither client role labels nor group names grant access.

Admin contexts retain explicit membership: an unrelated admin cannot read them. Creation and invitation/join require current admin or steward roles. History, workspace and run scope expressions repeat the admin-role requirement, so demotion and principal/device revocation remove access. Browser listings hide contexts from demoted users; state/open endpoints deny access. Existing frozen run/tool authority still prevents later promotion from elevating an old queued action.

Sources: app/security/admin_silo.ep, groups.ep, group_gate.ep, app/web/groups.ep and app/ui/client.ep. No new schema migration. Native tests cover positive shared-admin access, guest denial, demotion across history/run/workspace, remaining-admin access and restart. Existing browser-group controls regression passes. Deployed HTTP acceptance covers guest creation denial, admin creation/list/open, and demotion list/open/state denial. Evidence: evidence/phase2/admin-silo-20261016/live-http.json.

The UI was emitted and syntax-checked; physical-browser visual acceptance is not claimed by the HTTP test. Channel-public and Echo-system routing remain separate Step 06 work.
