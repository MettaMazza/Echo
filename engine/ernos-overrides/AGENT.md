# AGENT.md — AI Coding Agent Guidance for ErnosPlain

## Project Identity

ErnosPlain (Ernos) is a compiled, statically-typed, memory-safe programming language that reads like plain English. It compiles to native binaries via C transpilation. The goal is the best universal coding language that any person can read and write — code that looks like structured instructions, not cryptic symbols.

**Ernos must always be self-hosting and self-compiling.** The self-hosted compiler (`epc.ep` + `ep_lexer.ep` + `ep_parser.ep` + `ep_codegen.ep`) is not optional. It is the proof that the language works. If the self-hosted compiler cannot compile itself, the language is broken. Every change to the Rust bootstrap compiler must be validated against the self-hosted compiler.

---

## PRIME DIRECTIVE

**You are an implementation agent, not a decision-maker.** The human architect specifies what to build. You build it to the maximum possible quality. You do not:
- Downgrade scope ("let's just do a minimal version")
- Defer work ("we can add that later")
- Skip items ("this isn't critical right now")
- Approximate ("this should be close enough")
- Self-authorize tradeoffs ("I'll simplify this for maintainability")

If the plan says build X, you build X completely. If you hit a genuine technical impossibility, you report it with evidence (error output, not opinion) and wait for instructions. You never silently drop a requirement.

---

## Anti-Hallucination Protocol

These rules exist because AI agents hallucinate. They invent APIs that don't exist, claim features work when they don't, generate code that compiles but doesn't do what was asked, and declare success without verification. Every rule here is a hard boundary.

### Rule 1: No Unverified Claims

```
FORBIDDEN: "This should work" / "This will compile" / "This looks correct"
REQUIRED:  Compile it. Run it. Paste the output. THEN say it works.
```

Every code change must be verified with actual execution output. If you cannot run it, say so. Do not claim it works.

### Rule 2: No Invented Syntax

The language grammar is defined in `src/parser.rs`. If you write ErnosPlain code, it must parse. Common hallucinations:
- `,` between arguments (real: `and`)
- `{ }` for blocks (real: indentation)
- `var`/`let`/`const` (real: `set ... to`)
- `fn`/`func`/`function` (real: `define`)
- `print`/`println`/`console.log` (real: `display`)
- `match` (real: `check`)
- `len()`/`push()`/`pop()` (real: `length_list`, `append_list`, `pop_list`)
- `new StructName(...)` (real: `create StructName:`)
- `.length`/`.size` properties (real: `length_list(x)` function call)

### Rule 3: No Invented Functions

If a function isn't registered in `src/type_check.rs` (`register_builtins`) AND implemented in `src/codegen.rs`, it does not exist. Do not call functions that are not there. Check the source before using any builtin.

### Rule 4: Self-Hosting Gate is Mandatory

```bash
# This MUST pass after any change to the type system, parser, or codegen:
cargo run -- epc.ep && ./epc tests/test_basic_math.ep && ./test_basic_math

# Stronger gates (run after any epc-visible change):
bash tests/run_fixpoint.sh      # 3-stage byte-identical self-compile
bash tests/run_epc_parity.sh    # self-hosted coverage scoreboard (must not regress)
bash tests/run_differential.sh  # both compilers must AGREE on 43 adversarial programs
bash bootstrap/verify.sh        # clang-only, Rust-free 3-stage fixpoint + parity + freshness
```

Current state of these gates: `run_tests.sh` **84/84**, `run_epc_parity.sh` **55/55 runnable + 12/12 compile-error rejections** (0 wrongly accepted), `run_differential.sh` **43/43 agree**, `run_fixpoint.sh` byte-identical, `bootstrap/verify.sh` green.

The self-hosted compiler is ~7,100 source lines of real ErnosPlain (plus the generated runtime module) — `ep_lexer.ep` (lexer), `ep_parser.ep` (parser), `ep_check.ep` (semantic checker), `ep_optimizer.ep` (constant folding + DCE), `ep_codegen.ep` (C codegen), `epc.ep` (driver) — that exercises the full type system, all builtin functions, list/string operations, struct creation, pattern matching, closures, floats, traits, `try`/Result, and coroutine async. If it doesn't compile, you broke something.

**Shared runtime.** `runtime/ep_runtime.c` + `runtime/ep_builtins.c` are the single source of truth for the emitted C runtime. The Rust compiler embeds them via `include_str!`; the self-hosted compiler embeds them via the generated `ep_runtime_gen.ep`. After editing either `runtime/*.c` file, regenerate: `./target/release/ernos tools/gen_runtime_ep.ep && ./tools/gen_runtime_ep`.

**Bootstrap freshness.** `bootstrap/epc_bootstrap.c` is `epc` compiled by `epc`. After ANY change that alters epc's output, regenerate it in the same commit: `./epc epc.ep && cp epc_compiled.c bootstrap/epc_bootstrap.c`. `bash bootstrap/verify.sh` enforces freshness and the clang-only fixpoint.

### Rule 5: Read Source, Never Guess

When you need to know how something works:
1. Read the actual source file
2. Grep for the exact function/keyword/token
3. Trace the code path from lexer → parser → type_check → codegen

Documentation may be stale. The source is truth. The source files:

| File | Contents | Authority Over |
|---|---|---|
| `src/token.rs` | Token enum | What keywords exist |
| `src/lexer.rs` | Tokenizer | How source text becomes tokens |
| `src/ast.rs` | AST node types | What the parser can produce |
| `src/parser.rs` | Parser | What syntax is valid |
| `src/type_check.rs` | Type checker, `register_builtins()` | What types exist, what functions are typed |
| `src/borrow_check.rs` | Ownership analysis | What is tracked, moved, borrowed |
| `src/codegen.rs` | C codegen + full C runtime | What actually executes, every builtin implementation |
| `src/optimizer.rs` | Constant folding, DCE | What optimizations happen |
| `src/main.rs` | CLI, imports, REPL, pipeline | How compilation is orchestrated |
| `src/native_codegen.rs` | ARM64 assembly backend | Native ARM64 codegen |
| `src/x86_64_codegen.rs` | x86_64 assembly backend | Native x86_64 codegen |
| `src/diagnostics.rs` | Error reporting | How errors are displayed |

### Rule 6: No Silent Scope Reduction

If the plan has 5 items, you implement 5 items. You do not:
- Implement 3 and "acknowledge" the other 2
- Implement "the most important" ones and defer the rest
- Substitute a simpler version of what was asked
- Mark something as "future work" without being told to

If you cannot complete an item, you STOP and report exactly why with evidence. You do not continue past it silently.

### Rule 7: No Phantom Dependencies

Do not add `npm`, `pip`, `cargo add`, or any external dependency without explicit instruction. The bootstrap compiler uses only Rust std. The generated C uses only libc + pthreads. The stdlib modules use only the builtin C runtime. Preserve this.

### Rule 8: Maximal Implementation

Every feature must be implemented at its full potential. Not the "MVP". Not the "simple version". The complete, production-quality version. This means:
- Error messages include context, line numbers, and suggestions
- Edge cases are handled, not ignored
- Tests cover success AND failure paths
- Code is documented with comments explaining WHY, not just WHAT
- Generated output is clean and readable

### Rule 9: No Reward Hacking

Reward hacking is when an agent optimizes for the appearance of success rather than actual success. Examples:
- Writing a test that always passes regardless of the code being correct
- Claiming "all tests pass" without actually running them
- Modifying test expectations to match broken output
- Suppressing error output to hide failures
- Adding `|| true` to commands to mask exit codes
- Writing code that compiles but doesn't implement the specification

Every test must be a genuine verification. Every success must be demonstrated with real output.

### Rule 10: No Exception Clauses

Do not create your own exceptions to these rules. Phrases that indicate rule-breaking:
- "In this case, we can skip..."
- "For simplicity, I'll just..."
- "This is a reasonable tradeoff..."
- "We don't need to test this because..."
- "The self-hosting gate isn't necessary here because..."

These rules have no exceptions. If following a rule seems wrong, ask the human. Do not self-authorize an exception.

---

## Design Principles

### Plain English First
Every language construct should read like a sentence a non-programmer could understand:
- `set score to 85` not `int score = 85;`
- `if score is greater than 80:` not `if (score > 80) {`
- `repeat while count is less than 10:` not `while (count < 10) {`
- `for each item in list:` not `for (auto& item : list) {`

Symbol shortcuts (`+`, `<`, `==`, `&&`) are allowed as opt-in shorthands for experienced programmers. The plain English form is always the primary syntax.

### Self-Hosting is Non-Negotiable
The self-hosted compiler (`epc.ep` + modules) must always compile itself using the Rust bootstrap compiler. This is the ultimate integration test. If the type checker rejects the self-hosted compiler, the type checker is too strict — not the self-hosted compiler is wrong. The self-hosted compiler is ~7,100 source lines of real, working ErnosPlain plus its generated runtime module. It is the language's own dogfood, and it bootstraps from a frozen C snapshot with no Rust in the loop.

### Cross-Platform by Default
Ernos must work on:
- macOS ARM64 (Apple Silicon) — primary development platform
- macOS x86_64 (Intel Mac)
- Linux x86_64
- Linux aarch64

Both the C backend (default) and native assembly backends (`--native`) must produce correct binaries on all four targets. If a change works on macOS but breaks Linux, the change is broken.

### The C Runtime is the Language
The C runtime embedded in `codegen.rs` defines what the language can actually do at runtime. It is not a library — it is part of the compiler output. Every `.ep` program gets a copy of the runtime compiled into its binary. This means:
- Runtime changes affect ALL programs
- Runtime bugs affect ALL programs
- Runtime performance is language performance

### Tests Are Evidence, Not Decoration
Every feature, every bugfix, every behavioral change needs a test file in `tests/`. The test format:
- `tests/test_feature_name.ep` — the test program
- `tests/test_feature_name.expected` — the exact expected stdout (optional, for deterministic tests)

The `run_tests.sh` script compiles and runs every test. It is the regression gate.

---

## Compilation Pipeline

```
Source (.ep)
    ↓
Lexer (lexer.rs) → Token stream
    ↓
Parser (parser.rs) → AST
    ↓
Type Checker (type_check.rs) → Type-annotated AST (errors = hard stop)
    ↓
Borrow Checker (borrow_check.rs) → Ownership validation with NLL (non-lexical lifetimes)
    ↓
Optimizer (optimizer.rs) → Constant folding, dead code elimination
    ↓
Codegen (codegen.rs) → C source code (includes full runtime inline)
    ↓
Clang/GCC → Native binary

Alternative: --native flag (supported AST subset)
    Codegen → ARM64 or x86_64 assembly + shared C runtime → assembler/C compiler/linker → native binary
```

---

## Value Representation

Everything is `long long` at the C level. The type system is a compile-time overlay:

| Ernos Type | Runtime Value | How to interpret |
|---|---|---|
| `Int` | Raw 64-bit integer | Used directly |
| `Bool` | 0 or 1 | Used as integer |
| `Str` | Pointer to `const char*` cast to `long long` | Cast back to `char*` for string ops |
| `DynStr` | Pointer to `malloc'd char*` cast to `long long` | Same, but freed at scope exit |
| `Any` | Raw `long long` | Could be int, string, list, etc. — determined by context |
| `List` | Pointer to `EpList*` cast to `long long` | Cast back for list ops |
| `Struct(Name)` | Pointer to `EpStruct_Name*` cast to `long long` | Cast back for field access |
| `Enum(Name)` | Pointer to `EpEnum_Name*` cast to `long long` | Cast back, check tag for variant |
| `Closure` | Pointer to `EpClosure*` cast to `long long` | Magic number distinguishes from raw function ptr |

This means `get_list(list and 0)` returns a `long long` that could be an int, a string pointer, or a list pointer. The type checker tracks what it is at compile time, but at runtime it's just bits.

---

## Syntax Quick Reference for Agents

```ernos
# Variables
set x to 42
set name to "Alice"
set typed as Int to 100

# Functions  
define add with a as Int and b as Int returning Int:
    return a + b

# Function calls — args separated by 'and', not commas
set result to add(10 and 20)

# Bare calls (no return capture needed)
append_list(items and "value")

# If/else/else if
if x > 10:
    display "big"
else if x > 5:
    display "medium"
else:
    display "small"

# Loops
repeat while x < 100:
    set x to x + 1

while x < 100:
    set x to x + 1

for each item in items:
    display item

# Break and continue inside loops
repeat while true:
    if done == 1:
        break
    continue

# Structs
define structure User:
    field name as Str
    field age as Int

set user to create User:
    name is "Alice"
    age is 30

display user.name
set user.age to 31

# Enums (choices)
define choice Shape:
    variant Circle with radius as Int
    variant Square with side as Int

set s to Circle with 5
check s:
    if Circle with r:
        display r
    if Square with side:
        display side

# Methods (on structs or enums)
define greet on User:
    display self.name
    return 0

set ok to user.greet()

# Closures (lambdas)
set double to given x:
    return x * 2

# Higher-order functions
define apply with f and x as Int returning Int:
    return f(x)
set result to apply(double and 5)

# Concurrency
set ch to channel
# or: set ch to create_channel()
spawn worker(ch)
set v to receive from ch
send 42 to ch

# Imports
import "string"
import "fs"
import "math" as m

# Namespace import usage — prefix with alias_
set val to m_absolute(-10)

# F-strings (string interpolation)
display f"Hello {name}, you are {age} years old"

# List builtins
set items to create_list()
set ok to append_list(items and 10)
display get_list(items and 0)
display length_list(items)

# List literals
set nums to [1, 2, 3]
set names to ["Alice", "Bob"]

# Maps
set m to create_map()
set ok to map_insert(m and "key" and 42)
display map_get_val(m and "key")

# Try expression (error handling)
set result to try risky_operation()

# External functions (FFI with C)
external define c_function with param1 and param2 returning Int:

# Traits
define trait Printable:
    define to_string with self returning Str

implement Printable for User:
    define to_string:
        return self.name

# Comments
# This is a comment
```

---

## Builtin Functions Reference

These are implemented as C functions in the runtime (codegen.rs). They are NOT ErnosPlain functions — they are compiler intrinsics. Do not try to redefine them.

### Lists
`create_list()`, `append_list(list and value)`, `get_list(list and index)`, `set_list(list and index and value)`, `length_list(list)`, `pop_list(list)`, `remove_list(list and index)`, `free_list(list)`, `get_list_data_ptr(list)`, `string_from_list(list)`

### Maps
`create_map()`, `map_insert(map and key and value)`, `map_get_val(map and key)`, `map_get_str(map and key)`, `map_set_str(map and key and value)`, `map_contains(map and key)`, `map_has_key(map and key)`, `map_delete(map and key)`, `map_keys(map)`, `map_values(map)`, `map_size(map)`, `free_map(map)`

### Deques
`create_deque()`, `deque_push_front(deque and value)`, `deque_push_back(deque and value)`, `deque_pop_front(deque)`, `deque_pop_back(deque)`, `deque_length(deque)`, `free_deque(deque)`

### Strings
`string_length(s)`, `substring(s and start and len)`, `string_concat(a and b)` (alias: `concat`), `int_to_string(n)` (alias: `ep_int_to_str`), `string_to_int(s)`, `string_contains(s and sub)`, `string_index_of(s and sub)`, `string_replace(s and old and new)`, `string_split(s and delim)`, `string_upper(s)`, `string_lower(s)`, `string_trim(s)`, `char_at(s and index)`, `get_character(s and index)` (returns ASCII code), `char_from_code(code)` (returns single-char string), `ep_auto_to_string(val)`

### I/O
`display expr`, `display_string(s)`, `read_line()`, `read_int()`, `read_float()`, `read_file_content(path)`, `write_file_content(path and content)`, `file_append(path and content)`, `file_read(path)`, `file_write(path and content)`, `file_exists(path)`

### Filesystem
`fs_exists(path)`, `fs_is_file(path)`, `fs_is_dir(path)`, `fs_get_size(path)`, `fs_delete_file(path)`, `fs_copy_file(src and dst)`, `fs_move_file(src and dst)`, `fs_scan_dir(path)`

### Concurrency
`channel` (keyword), `create_channel()`, `send value to channel` (statement), `receive from channel` (expression), `spawn function(args)` (statement), `channel_has_data(ch)`, `channel_try_recv(ch)`, `channel_select(ch_list)`, `send_channel(ch and val)`, `recv_channel(ch)`, `create_task_group()`, `add_task_group(group and fut)`, `wait_task_group(group)`, `async_timeout(timeout_ms and fut)`, `cancel_task(fut)`

### Networking
`ep_net_connect(host and port)`, `ep_net_listen(port)`, `ep_net_accept(server)`, `ep_net_send(socket and data)`, `ep_net_send_raw(socket and data_ptr and byte_count)`, `ep_net_recv(socket and bufsize)`, `ep_net_recv_bytes(socket and bufsize)`, `ep_net_close(socket)`, `ep_http_request(method and url and headers and body)`

### JSON
`json_get_int(json and key)`, `json_get_string(json and key)`, `json_get_bool(json and key)`

### SQLite
`ep_sqlite3_open(filename and db_ptr)`, `ep_sqlite3_close(db)`, `ep_sqlite3_exec(db and sql and callback and cb_arg and errmsg_ptr)`, `sqlite_get_callback_ptr(dummy)`

Prepared-statement primitives (parameterized, injection-safe): `ep_sqlite3_prepare_v2(db and sql)`, `ep_sqlite3_bind_text(stmt and idx and value)`, `ep_sqlite3_bind_int(stmt and idx and value)`, `ep_sqlite3_step(stmt)`, `ep_sqlite3_column_count(stmt)`, `ep_sqlite3_column_text(stmt and col)`, `ep_sqlite3_column_int(stmt and col)`, `ep_sqlite3_finalize(stmt)`

> **Note:** The `ep_sqlite3_*` wrappers properly marshal between SQLite's `int` returns and ErnosPlain's `long long`, preventing arm64 upper-32-bit garbage. The stdlib `sql.ep` module provides a higher-level API: `sql_open`, `sql_close`, `sql_execute`/`sql_query` (RAW — never pass untrusted input), and `sql_execute_params`/`sql_query_params` (parameterized, injection-safe — use these for any user input; `?` placeholders bound from a list).

### Cryptography
`ep_sha256(s)`, `ep_md5(s)`, `ep_hmac_sha256(key_ptr and key_len and msg_ptr and msg_len)` (RFC 2104, returns hex), `ep_base64_encode(data)`, `ep_uuid_v4()`. The stdlib `crypto.ep` wraps these (`crypto_sha256`, `hmac_sha256`, `uuid_v4`, `random_int`, `random_bytes`).

> **Note:** `ep_random_int` and `ep_uuid_v4` draw from the OS CSPRNG (`arc4random` on Apple/BSD, `getrandom`/`/dev/urandom` on Linux), not `rand()`, so UUIDs and `random_bytes` are suitable for security tokens. `ep_random_int` uses rejection sampling to avoid modulo bias.

### Math / System
`ep_random_int(min and max)`, `ep_time_ms()`, `ep_time_now_ms()`, `ep_time_now_sec()`, `ep_time_day()`, `ep_time_month()`, `ep_time_year()`, `ep_sleep_ms(ms)`, `sleep_ms(ms)`, `ep_abs(n)`, `ep_system(cmd)`, `ep_play_sound(path)` (macOS), `run_command(cmd)`

### Float
`int_to_float(n)`, `float_to_int(f)`, `float_to_string(f)`

### Command-Line Args
`get_argument_count()`, `get_argument(index)`

### FFI (Dynamic Library Loading)
`ep_dlopen(path)` — load .dylib/.so, returns handle
`ep_dlsym(handle and name)` — find symbol, returns function pointer
`ep_dlclose(handle)` — close library handle
`ep_dlcall0(fn)` through `ep_dlcall10(fn and a1 ... and a10)` — call function pointer with 0-10 integer args
`ep_dlcall_f0(fn)` through `ep_dlcall_f6(fn and f1 ... and f6)` — call function pointer with 0-6 float args

All FFI functions work with `long long` arguments. Pointers and integers are passed directly. Strings must be passed as their `long long` representation (which is already how ErnosPlain stores them).

### CLI Subcommands

```bash
# Compile and run
ernos program.ep                     # Compile to ./program, then run
ernos program.ep --native            # Native assembly frontend + C runtime

# C header binding generation
ernos bind header.h [-o bindings.ep] # Parse C header → ErnosPlain bindings

# Cross-language transpilation
ernos transpile file.py [-o out.ep]  # Python → ErnosPlain
ernos transpile file.c [-o out.ep]   # C → ErnosPlain
ernos transpile file.js [-o out.ep]  # JavaScript → ErnosPlain

# Other
ernos --version                      # Show version
ernos --list-builtins                # List all builtin functions
ernos check file.ep                  # Full static validation without codegen
```

---

## Verification Protocol

Before committing ANY change, run this sequence IN ORDER:

```bash
# 1. Rust build — zero warnings, zero errors
cargo build --release 2>&1 | tail -3

# 2. All tests pass — zero failures
./run_tests.sh

# 3. Self-hosting gate (MANDATORY when type checker/codegen/parser changed)
cargo run -- epc.ep && ./epc tests/test_basic_math.ep && ./test_basic_math

# 4. Native and check-only safety gates
bash tests/run_native_gate.sh
bash tests/run_check_gate.sh
```

If ANY step fails, the change is not ready. Fix it before committing. Do not commit with known failures.

---

## Known Constraints

1. **All values are `long long`** — Float is supported via `int_to_float`, `float_to_int`, `float_to_string` conversions and `ep_dlcall_f0`–`ep_dlcall_f6` for calling C functions that take/return doubles
2. **Single-file compilation** — the import system flattens everything into one C file
3. **MonoType::Any for containers** — `get_list`, `pop_list`, `map_get_val`, `map_get_str` return `Any` to support heterogeneous data
4. **No closures over mutable state** — closures capture by value at creation time
5. **GC is stop-the-world mark-and-sweep** — all threads pause during collection (protected by `ep_gc_mutex`)
6. **Thread limit** — maximum 256 concurrent threads (`EP_MAX_THREADS`)
7. **No Windows support** — partial `#ifdef _WIN32` polyfills exist but are untested
8. **Namespace imports** — `import "module" as alias` adds `alias_` prefixed function names
9. **HOF with named functions** — passing named functions as arguments requires closure wrapping in some cases
10. **Concurrency scale** — channel operations are safe up to ~500-1000 operations before GC pressure
11. **Send/Sync checking** — spawn arguments and channel values must be Send. `Ref(T)` (borrowed references) are NOT Send (E0036). `Struct`/`Enum`/`List` are Send (owned values). All primitives are Send+Sync.
12. **NLL borrow checker** — borrows expire at their last use point (non-lexical lifetimes), not at scope end. The checker pre-collects variable uses via `UseCollector` and tracks `LiveBorrow` lifetimes.

---

## Bridge Libraries

Pre-built bindings for popular C libraries using the FFI system. Located in `stdlib/bridge/`:

| Module | Library | Category |
|---|---|---|
| `raylib.ep` | raylib | Graphics, input, audio |
| `sdl2.ep` | SDL2 | Multimedia, windowing |
| `ncurses.ep` | ncurses | Terminal UI |
| `cairo.ep` | cairo | 2D vector graphics |
| `libpng.ep` | libpng | PNG image I/O |
| `stb_image.ep` | stb_image | Image loading |
| `miniaudio.ep` | miniaudio | Cross-platform audio |
| `libsndfile.ep` | libsndfile | Audio file I/O |
| `curl.ep` | libcurl | HTTP client |
| `openssl.ep` | OpenSSL | TLS, crypto |
| `libsodium.ep` | libsodium | Modern cryptography |
| `zlib.ep` | zlib | Compression |
| `sqlite.ep` | SQLite3 | Embedded database |
| `jansson.ep` | Jansson | JSON parsing |
| `expat.ep` | expat | XML parsing |
| `pcre.ep` | PCRE2 | Regex |
| `libgit2.ep` | libgit2 | Git operations |
| `libuv.ep` | libuv | Async I/O, event loop |
| `lmdb.ep` | LMDB | Key-value database |

Usage: `import "stdlib/bridge/sqlite"` — all functions use `ep_dlopen`/`ep_dlsym` at runtime, so the library must be installed on the target system.

---

## File Naming Conventions

- Source files: `snake_case.ep`
- Test files: `tests/test_feature_name.ep` with optional `tests/test_feature_name.expected`
- Stdlib modules: `stdlib/module_name.ep`
- Self-hosted compiler: `epc.ep`, `ep_lexer.ep`, `ep_parser.ep`, `ep_check.ep`, `ep_optimizer.ep`, `ep_codegen.ep` (+ generated `ep_runtime_gen.ep`)
- Generated C: `filename_compiled.c` (temporary, cleaned up by compiler)
- Generated binary: `./dirname/filename` (next to source file, same stem)

---

## When Making Changes

### Adding a new builtin function
1. Register the type signature in `type_check.rs` (`register_builtins`)
2. Implement the C function body in `codegen.rs` (in the runtime emission section)
3. Register it in `codegen.rs` `func_return_types` map
4. Add it to the `is_builtin_or_runtime_func` filter
5. Write a test in `tests/`
6. Update AGENT.md builtin reference
7. Run ALL verification gates

### Adding new syntax
1. Add token(s) in `token.rs`
2. Add lexer rule(s) in `lexer.rs`
3. Add AST node(s) in `ast.rs`
4. Add parser rule(s) in `parser.rs`
5. Add type checking in `type_check.rs`
6. Add borrow checking in `borrow_check.rs` (if heap-allocating)
7. Add C codegen in `codegen.rs`
8. Add native codegen in `native_codegen.rs` and `x86_64_codegen.rs` (or return unsupported error)
9. Write tests, update spec, update AGENT.md, update LANGUAGE_REFERENCE.md
10. Run ALL verification gates including self-hosting

### Modifying the type checker
- The type checker uses unification-based inference (HM-style, without let-generalization) in `type_check.rs`
- `MonoType` is the core type enum (`Int`, `Float`, `Bool`, `Str`, `DynStr`, `Any`, `List`, `Struct`, `Enum`, `Fun`, `Var`, `Unit`, `Ref`, `Future`)
- `MonoType::is_send()` — returns true if the type can be safely transferred across thread boundaries
- `MonoType::is_sync()` — returns true if the type can be safely shared between threads
- `Ref(T)` is NOT Send — borrowed references cannot cross thread boundaries
- Spawn arguments and channel sends are checked for Send safety (error code E0036)
- `Substitution` maps type variables to concrete types
- `unify()` is the unification function — it resolves type constraints
- `Any` is the top type — it unifies with everything
- Changing type signatures of builtins can cascade — ALWAYS run self-hosting gate

### Modifying the GC / runtime
- The GC root stack is `__thread` (thread-local)
- `ep_gc_push_root` / `ep_gc_pop_roots` are per-thread
- `ep_gc_mark` walks ALL threads' root stacks (under `ep_thread_registry_mutex`)
- `ep_gc_maybe_collect` is called after every N allocations
- If you change the runtime, you change the behavior of ALL compiled programs
- Test with concurrency programs (channel stress, multi-worker) — GC bugs manifest under concurrent load
- Minor collections deliberately skip the conservative C-stack scan; they rely on the precise shadow stacks + the write-barrier remembered set. `ep_gc_mark_object_minor` guards every remembered-set entry with `ep_gc_table_get` (skips freed pointers), and `ep_gc_unregister` removes freed pointers from both the table and the remembered set under `ep_gc_mutex`. `tests/test_gc_remembered_uaf.ep` is the ASAN regression guard for this path — compile it `--asan` after any GC change.

---

## Collaboration Protocol for Human-AI Teams

### Before Starting Any Work
1. Read this file completely
2. Read the implementation plan or task description completely
3. Ask clarifying questions BEFORE coding — not after
4. State your understanding of the task back to the human

### During Work
1. Show real terminal output for every claim
2. If you hit a blocker, stop and report with evidence
3. Never silently skip an item — if you can't do it, say so
4. Commit frequently with descriptive messages

### After Completing Work
1. Run ALL verification gates
2. Report exactly what was done, what was tested, what passed
3. Report anything that was NOT done and why
4. Update documentation (AGENT.md, spec, LANGUAGE_REFERENCE.md, README.md) to reflect changes

---

## Architecture for AI-Built Systems

Ernos is designed to be a language that AI coding agents can work with reliably. These principles apply to all code written in or for Ernos:

### Explicit Over Implicit
- Every function signature should declare parameter types when non-obvious
- Every return type should be declared for public-facing functions
- Every struct field should have a type annotation
- No "magic" behavior — if something happens, it should be visible in the code

### Verifiable Over Clever
- Prefer simple, readable code over clever optimizations
- Every function should be testable in isolation
- No hidden state mutations — if a function changes something, it should be obvious
- Error paths should be explicit, not swallowed

### Complete Over Partial
- Every function must handle all its input cases
- Every enum `check` block must handle all variants
- Every error must produce a useful message
- No TODO/FIXME/HACK comments in committed code — either fix it or file an issue

## Echo local compiler override: canonical import identity

The self-hosted driver resolves source files through native realpath before its parsed-file lookup, matching the reference compiler's filesystem canonicalization. Keep tests/test_import_identity.ep and the nested fixture when changing import resolution. Echo's combined component import test is an additional integration gate. This override uses existing dynamic-library primitives; no new package dependency or hand-authored C runtime code was added. Its native resolver has been exercised on macOS arm64; Linux fallback remains unverified.
