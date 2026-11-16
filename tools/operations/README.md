# Owner-authorized Echo deployment

`echo_owner_admin.py` mirrors the installed root-owned `/Library/PrivilegedHelperTools/local.echo.admin`. The existing sudo delegation remains restricted to that helper and the enrolled local owner.

Ordinary deployment uses `deploy`. Owner-authorized source upgrades can use `deploy-owner`, which updates only existing protected source entries present in the captured, hash-verified package. It retains the protected path set, fixed tests and installed evaluator. The acceptance specification and pipeline reference are updated together with a durable transaction receipt. Recovery keeps pins matching the active release or restores the exact original bytes. The RSI controller is reloaded before stable-service checks because it pins configuration for its process lifetime.

Copy `install_echo_package.py` to a prepared package as `install.py`, set `owner_source_pin_update: true` in its manifest when required, and run that coordinator. Do not run the helper deployment verb alone: the coordinator performs idle/health checks and acknowledges each installation phase. Authentication is not requested by this already-enrolled path.

Run `python3 tools/operations/test_owner_admin.py` for synthetic commit/rollback checks. The test does not modify installed services or policy. Installing/upgrading the helper itself remains an owner administration operation; this source file is not executed automatically by Echo or supplied as a privileged package installer.
