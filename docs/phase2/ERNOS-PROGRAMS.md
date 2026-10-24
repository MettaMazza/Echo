> **1.9 documentation:** [Current update](../releases/1.9/README.md) · [feature status](../releases/1.9/FEATURES.md). Dated acceptance and older configuration statements below are historical; they are not fresh 1.9 verification.

# Local Ernos compilation and execution

Step 08 computation work uses the existing durable execution guardian. `workspace_execute` with `program: "ernos"` invokes the configured self-hosted compiler. A later invocation runs the resulting workspace binary. Compiler stdout/stderr, raw wait status, output events and process receipts are retained; `workspace_output_next` reads decoded output. Process capture being complete does not prove compilation succeeded: inspect the compiler success line, exit status and resulting program.

## Runtime correction

The execution release previously advertised the `ernos` alias without containing `epc` or `stdlib`. The operator-selected `ECHO_ERNOS_RUNTIME` now identifies a content-hashed installed copy of the existing compiler and standard library. `ECHO_ERNOS_COMPILER_SHA256` pins the executable. The guardian receives only these explicit host settings, not model-controlled environment variables. Application releases can change without changing the toolchain location or existing standard-library links. Source/compiler code is unchanged.

`workspace/ernos_runtime.ep` checks the installed compiler/library, verifies the compiler digest and creates the workspace `stdlib` link only if absent. An existing link must target the configured library exactly. Conflicting user files/directories/links are preserved and return `ernos-stdlib-conflict`; they are never overwritten. The toolchain is read-only inside the sandbox.

The native compiler calls Darwin `realpath` on relative source/import paths. Under the ordinary execution sandbox this fails because cwd resolution reads ancestor directories. Only the pinned compiler receives ancestor-directory read access. Its accepted arguments are a relative source path, `check` plus a relative source path, or `test` plus a relative source path. Sources must be regular scoped `.ep` files. Basenames use letters, digits or underscore because this compiler builds an internal linker command from that basename. Invalid flags, parent traversal and unsafe basename characters are rejected before launch. This is an integration restriction of the existing compiler command interface, not a token/context cap or a compiler modification.

Generated binaries and arbitrary workspace commands keep the ordinary sandbox: no ancestor-directory enumeration, outside file contents/metadata/writes, network access or toolchain writes. Compilation does not execute the generated program. Full admin host execution retains its separate explicit policy.

## Evidence and scope

`tests/workspace/test_ernos_runtime.ep` checks configuration, repeat preparation, conflicting-file preservation, malformed arguments and a mismatched compiler digest. `tests/workspace/test_sandbox.ep` checks outside metadata/content/write denial, symlink escape denial, loopback denial, descendant inheritance and toolchain write denial under the ordinary execution profile. `tests/tools/test_live_ernos_program.ep` exercises actual local-model source creation, compilation with the math standard library, execution and readback of the computed result. Evidence is retained under `evidence/phase2/ernos-runtime-20261016/`.

This repairs the existing advertised compiler path. It does not count generic execution as completion of the whole planned computation family or the explicit build/test/project workflows. Optional native dependencies required by other standard-library modules must still be installed and exposed through their appropriate dependency configuration; the math-library acceptance does not prove every optional dependency is installed. Portable installer acceptance remains under its original plan step.

## Deployed verification

Release `phase2-ernos-runtime-20261016-224606` runs the rebuilt server and execution guardian with the pinned toolchain. Local HTTP and LAN HTTPS status both returned 200; the existing model process remained running. Compiler and deployed executable hashes match. `evidence/phase2/ernos-runtime-20261016/deployment-verification.json` records these checks. This host-side LAN check does not claim a physical-phone test.

## Explicit build interface

`workspace_build` accepts `path` and `mode`: `compile`, `check`, or `test-harness`. The immutable archived request maps directly to the pinned compiler argument list. Unknown modes, extra fields and wrong types are rejected. Source scope, filename validation, compiler pinning, cancellation, authority monitoring and complete output capture use the same guardian as workspace execution. The caller cannot supply a replacement compiler, environment or shell expression.

The tool intentionally distinguishes building a test harness from running tests. `test-harness` invokes the compiler's `test` command; run the produced executable through `workspace_execute` and inspect its output and exit receipt. Compiler process capture completion alone does not establish a successful build; read the diagnostics through `workspace_output_next`: check mode reports `Check passed: no errors.`, while compilation/test-harness generation reports `Self-hosted compilation successful!`. Compilation can replace compiler output files in the workspace. This interface is not a claim that the remaining data-transform, database, plotting or code/Git workflows are complete.

Build-interface acceptance passed with the actual primary model: source creation, workspace_build compilation, output readback, program execution and result readback. Typed argument rejection checks pass. Evidence: `evidence/phase2/build-interface-20261016/`. Release `phase2-build-interface-20261016-225305` is deployed; HTTP and LAN HTTPS status both returned 200 and model process 13836 remained running.

### Check/test diagnostic acceptance

`test_build_modes.ep` drives the real authenticated dispatcher and installed guardian without model inference. It checks valid source, a rejected duplicate definition, generation and execution of a passing test harness and a deliberately failing harness, and refusal of an injected compiler override before process launch. It retains compiler diagnostics and raw process exits instead of inferring success from capture completion. An initial fixture incorrectly assumed the installed compiler rejects an undefined identifier in check mode; that diagnostic assumption failed and is retained in private evidence. The compiler was not changed. Check mode reports what this installed compiler checks; it does not replace compilation or execution tests.

Check/test acceptance passed; evidence is retained in `evidence/phase2/build-modes-20261016/`. Corrected mode-specific diagnostic guidance is deployed in `phase2-build-modes-20261016-225654`; both service endpoints responded 200 and the model remained running.
