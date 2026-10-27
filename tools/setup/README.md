# Echo setup toolkit

Double-click **Install-Echo.command** on macOS; Windows has **Install-Echo.ps1** and Linux has **install-echo.sh**. Python 3.10+ is recommended (the current macOS verification also ran on Python 3.9). `python3 echo_setup.py --scan` prints hardware and disk capacity without reading credentials.

Only the existing macOS production installation has end-to-end operational evidence. Windows/Linux entry points and shortcut formats are supplied, but native Echo ports and fresh-machine distribution bundles are not yet packaged. This is **not a completed universal one-click installer**. Echo currently has 134 application files using Darwin-specific APIs, including process, socket, filesystem, ACL and protected-controller code. Renaming a binary or substituting a C library would not safely port those contracts.

For an existing macOS EchoRSI installation, setup verifies every binary hash, all eight running/persistent services, maintenance state and HTTP readiness before creating `Desktop/Echo.webloc`. It does not modify credentials, service authority or durable conversation history. It opens the local app when checks pass. `--no-open` verifies and creates the shortcut without opening a browser.

The bundle installer accepts `--bundle PATH --destination PATH`. A manifest must bind the operating system/architecture, every file hash, executable paths and acceptance commands. Files are installed in a sibling staging directory; failed checks preserve the old installation. A passing package receipt means its checks passed, **not** that a live service was activated. Model downloads require HTTPS plus a pinned size and SHA-256; hardware selection reserves RAM and disk overhead. No model or credential is embedded.

Tests: `python3 tools/setup/test_setup.py`. They cover shortcut formats, capacity selection, path traversal, integrity/platform rejection and acceptance rollback. They do not substitute for native Windows/Linux acceptance, signed app packaging, fresh-machine dependency installation, model acquisition or Discord credential onboarding. Those remain required before publishing a universal ready-to-use installer.
