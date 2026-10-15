# Ernos Language Reference Manual

**Version 1.0.0**

Ernos is a statically-typed, compiled programming language with plain English syntax, unification-based type inference, garbage-collected memory with ownership safety checks, and native code generation via C.

---

## Table of Contents

1. [File Structure & Indentation](#1-file-structure--indentation)
2. [Variables & Type Annotations](#2-variables--type-annotations)
3. [Operators](#3-operators)
4. [Control Flow](#4-control-flow)
5. [Functions](#5-functions)
6. [Structs & Methods](#6-structs--methods)
7. [Enums & Pattern Matching](#7-enums--pattern-matching)
8. [Concurrency](#8-concurrency)
9. [Ownership & Borrowing](#9-ownership--borrowing)
10. [Closures & Higher-Order Functions](#10-closures--higher-order-functions)
11. [Error Handling](#11-error-handling)
12. [Imports & Modules](#12-imports--modules)
13. [Standard Library](#13-standard-library)
14. [Compilation](#14-compilation)

---

## 1. File Structure & Indentation

Ernos uses Python-style indentation for block structure.
- Blocks introduced with `:` (colon)
- Consistent spaces for nesting (4 spaces recommended)
- No curly braces `{}` or semicolons `;`
- Comments start with `#`

```ernos
# This is a comment
define main:
    if 10 > 5:
        display "Inside block"
    display "Outside block"
    return 0
```

---

## 2. Variables & Type Annotations

### Declaration
```ernos
set x to 42                        # inferred as Int
set name to "Alice"                # inferred as Str
set pi to 3.14159                  # inferred as Float
set flag to true                   # inferred as Bool
```

### Explicit Type Annotations
```ernos
set x as Int to 42
set name as Str to "Alice"
set ratio as Float to 3.14
```

Type annotations are optional — the unification-based inference engine determines types automatically.

---

## 3. Operators

### Arithmetic
| Shorthand | English | Operation |
|:---------:|:--------|:----------|
| `+` | `plus` | Addition |
| `-` | `minus` | Subtraction |
| `*` | `multiplied by` | Multiplication |
| `/` | `divided by` | Division |
| `%` | `modulo` | Remainder |

### Comparison
| Shorthand | English | Comparison |
|:---------:|:--------|:-----------|
| `<` | `is less than` | Less Than |
| `>` | `is greater than` | Greater Than |
| `<=` | — | Less Than or Equal |
| `>=` | — | Greater Than or Equal |
| `==` | `equals` / `is equal to` | Equal To |
| `!=` | `is not equal to` | Not Equal To |

### Logical
| Shorthand | English | Operation |
|:---------:|:--------|:----------|
| `&&` | `and also` | Logical AND |
| `\|\|` | `or else` | Logical OR |
| `not` | `not` | Logical NOT |

> **Note:** The `and` keyword is context-sensitive — inside conditions it acts as logical AND, inside function call parentheses it separates arguments.

```ernos
set result to 10 + 5 * 2           # 20 (precedence enforced)
if score > 90 && passed == true:
    display "Excellence!"
```

---

## 4. Control Flow

### If / Else / Else If
```ernos
if score >= 90:
    display "Grade A"
else if score >= 80:
    display "Grade B"
else:
    display "Grade C"
```

### Repeat While (loops)
```ernos
set i to 0
repeat while i < 10:
    display i
    set i to i + 1
```

The `while` shorthand also works:
```ernos
while i < 10:
    set i to i + 1
```

### For Each
```ernos
set items to create_list()
set ok to append_list(items and 10)
set ok to append_list(items and 20)

for each item in items:
    display item
```

### Break / Continue
```ernos
set i to 0
repeat while i < 100:
    set i to i + 1
    if i == 50:
        break
    if i % 2 == 0:
        continue
    display i
```

### Pattern Matching
```ernos
check status:
    if Success with value:
        display value
    if Error with msg:
        display msg
```

---

## 5. Functions

### Basic Functions
```ernos
define greet:
    display "Hello!"
    return 0

define add with a as Int and b as Int returning Int:
    return a + b
```

### Calling Functions
Arguments are separated by `and`:
```ernos
set result to add(10 and 20)
```

### Type Inference on Untyped Functions
```ernos
define double with x:
    return x * 2       # infers x must be Int (from * operator)

define main:
    display double(21)        # ✓ works: 42
    display double("hello")   # ✗ REJECTED: expected Int, found Str
    return 0
```

### Async Functions
```ernos
async define fetch_data with url as Str returning Int:
    set data to ep_net_connect(url and 80)
    return data

define main:
    set result to await fetch_data("example.com")
    display result
    return 0
```

---

## 6. Structs & Methods

### Defining Structs
```ernos
define structure Point:
    field x as Int
    field y as Int

define structure User:
    field name as Str
    field age as Int
```

### Creating Instances
```ernos
set p to create Point:
    x is 10
    y is 20

set user to create User:
    name is "Alice"
    age is 30
```

### Methods
```ernos
define distance on Point with other as Point returning Int:
    set dx to self.x - other.x
    set dy to self.y - other.y
    return dx * dx + dy * dy

define main:
    set a to create Point:
        x is 0
        y is 0
    set b to create Point:
        x is 3
        y is 4
    display a.distance(b)    # 25
    return 0
```

---

## 7. Enums & Pattern Matching

```ernos
define choice Result:
    variant Ok with value as Int
    variant Error with message as Str

define main:
    set r to Ok with 42
    check r:
        if Ok with value:
            display value
        if Error with msg:
            display msg
    return 0
```

---

## 8. Concurrency

### Spawn Threads
```ernos
define worker with id as Int and ch:
    send id * 10 to ch
    return 0

define main:
    set ch to channel
    spawn worker(1 and ch)
    spawn worker(2 and ch)
    set a to receive from ch
    set b to receive from ch
    display a + b
    return 0
```

### Channels
```ernos
define producer with ch:
    send 42 to ch
    return 0

define main:
    set ch to create_channel()
    spawn producer(ch)
    set value to receive from ch
    display value    # 42
    return 0
```

> **Note:** Channel creation uses either `channel` (keyword) or `create_channel()` (function call). Send syntax is `send value to channel`. Receive syntax is `receive from channel`.

### Structured Concurrency (Task Groups)
Ernos supports structured concurrency using Task Groups, timeouts, cancellations, and cooperative async sleeping:

```ernos
import "stdlib/structured" as s

async define worker with id as Int and val as Int returning Int:
    set dummy to await sleep_ms(50)
    display f"Worker {id} done"
    return val

define main:
    set group to s_create_group()
    s_add_task(group and worker(1 and 100))
    s_add_task(group and worker(2 and 200))
    
    # Wait for all tasks to complete
    set results to s_wait_group(group)
    display get_list(results and 0)  # 100
    display get_list(results and 1)  # 200
    return 0
```

- `s_create_group()`: Returns a new task group handler.
- `s_add_task(group and fut)`: Spawns/adds an async future task to the group.
- `s_wait_group(group)`: Waits until all tasks in the group complete. If any task returns a negative error code (e.g. `-1`), all other running tasks in the group are cancelled, and a list of results is returned.
- `s_timeout(ms and fut)`: Returns the future's result if it finishes within `ms`, otherwise cancels the future and returns `-1`.
- `s_cancel(fut)`: Aborts execution of the task corresponding to `fut`.
- `sleep_ms(ms)`: Sleep for `ms` milliseconds. Inside the event loop (an `async` task) it is a non-blocking cooperative sleep that yields back to the scheduler. Outside the event loop (ordinary synchronous code) it blocks the calling thread for the full duration and returns an already-completed future, so `set ok to sleep_ms(800)` really waits 800 ms.

---

## 9. Ownership & Borrowing

Ernos enforces memory safety at compile time:

```ernos
# ✓ Valid: pass ownership
define consume with data:
    display data
    return 0

# ✗ REJECTED: cannot send borrowed reference to thread
define main:
    set x to create_list()
    spawn consume(borrow x)    # Compilation error!
    return 0
```

Rules:
- Each value has exactly one owner
- Borrowing creates a reference without transferring ownership
- Borrowed references cannot be sent to spawned threads
- The compiler rejects violations before generating code

---

## 10. Closures & Higher-Order Functions

```ernos
define apply with f and x as Int returning Int:
    return f(x)

define main:
    set doubler to given x:
        return x * 2
    display apply(doubler and 21)    # 42
    return 0
```

---

## 11. Error Handling

```ernos
set result to try risky_operation()
if result == 0:
    display "Operation failed safely"
else:
    display "Success"
```

---

## 12. Imports & Modules

### Basic Import
```ernos
import "string"
import "fs"
import "json"
```

Standard library modules are resolved by name — `import "string"` resolves to `stdlib/string.ep`. Relative paths also work: `import "mylib.ep"`.

### Namespace Import
```ernos
import "math" as m

define main:
    set result to m_absolute(-42)
    display result    # 42
    return 0
```

When using `import "module" as alias`, all functions from the module are available with the `alias_` prefix while also remaining available by their original names.

---

## 13. Standard Library

24 modules available: `string`, `collections`, `fs`, `net`, `http`, `json`, `csv`, `datetime`, `crypto`, `regex`, `sync`, `os`, `test`, `log`, `math`, `sort`, `sql`, `gui`, `hash`, `toml`, `static_server`, `websocket`, `select`, `structured`.

### Core Modules

| Module | Description |
|--------|-------------|
| `string` | String manipulation, builder, formatting |
| `collections` | HashMap, HashSet, Stack, Queue, PriorityQueue |
| `fs` | File I/O, directory operations, paths |
| `os` | Environment, process, system info |
| `datetime` | Timestamps, formatting, arithmetic |
| `math` | Arithmetic and mathematical functions |
| `json` | JSON parsing and generation |
| `csv` | CSV parsing and generation |
| `regex` | Pattern matching |
| `crypto` | Hashing, encoding, random |
| `sync` | Mutex, RWLock, Atomic, Barrier, Semaphore |
| `net` / `http` | TCP sockets, HTTP client and server |
| `test` | Assertions and test runner |
| `log` | Structured logging |
| `sort` | Sorting algorithms |
| `sql` | SQLite bindings |
| `gui` | GUI via raylib |
| `hash` | Hashing utilities |
| `toml` | TOML config parsing |
| `static_server` | Static file HTTP server |
| `websocket` | WebSocket protocol |
| `select` | I/O multiplexing |
| `structured` | Structured concurrency (task groups, timeouts) |

---

## 13.1. Bridge Libraries (FFI)

Pre-built bindings for popular C libraries using dynamic loading. Located in `stdlib/bridge/`:

```ernos
import "stdlib/bridge/sqlite"

define main:
    set db to sqlite_open("test.db")
    set _ to sqlite_exec(db and "CREATE TABLE users (name TEXT, age INT)")
    set _ to sqlite_close(db)
    return 0
```

Available bridges (29): `sqlite`, `curl`, `zlib`, `openssl`, `pcre`, `jansson`, `raylib`, `sdl2`, `ncurses`, `cairo`, `libpng`, `stb_image`, `stb_truetype`, `miniaudio`, `libsndfile`, `libsodium`, `expat`, `libgit2`, `libuv`, `lmdb`, `termbox2`, `portmidi`, `freetype`, `lua`, `mongoose`, `mosquitto`, `libnotify`, `libusb`, `chipmunk`.

### Dynamic Library Loading (Low-Level FFI)

```ernos
define main:
    set lib to ep_dlopen("libm.dylib")
    set abs_fn to ep_dlsym(lib and "abs")
    set result to ep_dlcall1(abs_fn and -42)
    display result    # 42
    set _ to ep_dlclose(lib)
    return 0
```

FFI Functions:
- `ep_dlopen(path)` — load shared library, returns handle
- `ep_dlsym(handle and name)` — find symbol, returns function pointer
- `ep_dlclose(handle)` — close library
- `ep_dlcall0(fn)` through `ep_dlcall10(fn and a1 ... and a10)` — call with 0-10 integer args
- `ep_dlcall_f0(fn)` through `ep_dlcall_f6(fn and f1 ... and f6)` — call with 0-6 float args

---

## 13.2. C Header Binding Generator

Generate ErnosPlain bindings from C headers:

```bash
ernos bind /usr/include/math.h -o math_bindings.ep
ernos bind mylib.h -o mylib.ep
```

Parses: function declarations, struct definitions, enums, typedefs, `#define` constants.

---

## 13.3. Cross-Language Transpilers

Translate source code from other languages into ErnosPlain:

```bash
ernos transpile script.py -o script.ep     # Python → ErnosPlain
ernos transpile program.c -o program.ep    # C → ErnosPlain
ernos transpile app.js -o app.ep           # JavaScript → ErnosPlain
```

Supported extensions: `.py`, `.c`, `.h`, `.js`, `.mjs`, `.go`, `.rs`, `.rb`, `.java`, `.ts`



## 14. Compilation

### Basic Usage
```bash
./target/release/ernos program.ep    # Compile with -O2
./program                            # Run the native binary
```

### Build Modes
```bash
ernos program.ep --release    # Compile with -O3 + LTO
ernos program.ep --debug      # Compile with -O0 + debug symbols
ernos check program.ep        # Full static validation, no binary
ernos format program.ep       # Auto-format source code
ernos --repl                  # Interactive REPL
ernos program.ep --native     # Native assembly frontend + C runtime
ernos program.ep --asan       # AddressSanitizer
```

The `--native` backend emits architecture-specific assembly for the supported
language subset, then compiles and links the shared C runtime. It therefore
requires the system assembler/linker and a C compiler (Clang or GCC).

### Cross-Platform
The generated C code compiles on any platform with a C compiler:
```bash
# macOS
clang program_compiled.c -O2 -o program -lpthread

# Linux
gcc program_compiled.c -O2 -o program -lpthread
```

---

## Type System Summary

| Type | Description | Example |
|------|-------------|---------|
| `Int` | 64-bit signed integer | `42` |
| `Float` | 64-bit double | `3.14` |
| `Bool` | Boolean | `true`, `false` |
| `Str` | String literal (immutable) | `"hello"` |
| `DynStr` | Heap-allocated string | `concat("a" and "b")` |
| `Any` | Top type (from container returns) | `get_list(l and 0)` |
| `List of T` | Dynamic array | `create_list()` |
| `StructName` | Named struct | `create Point: ...` |
| `EnumName` | Tagged union | `Ok with 42` |

---

## 15. Iterator Protocol

ErnosPlain supports custom iterators and iteration protocols using the `Iterator` trait and `IterResult` choice type defined in `collections`.

### Defining an Iterator

Any structure implementing the `Iterator` trait can be iterated over in a `for each` loop:

```ernos
import "collections"

define structure RangeIterator:
    field current as Int
    field limit as Int

implement Iterator for RangeIterator:
    define next returning IterResult:
        if self.current < self.limit:
            set val to self.current
            set self.current to self.current + 1
            return Next with val
        else:
            return Done

define main:
    set iter to create RangeIterator:
        current is 0
        limit is 5
    
    for each num in iter:
        display num    # Prints 0, 1, 2, 3, 4
    
    return 0
```

---

## 16. Doc Comments & API Documentation

ErnosPlain supports generating API documentation from triple-hash (`###`) doc comments:

### Writing Doc Comments

Place `###` doc comments immediately before the declaration of functions, structures, choices (enums), or traits:

```ernos
### Calculates the square of a given integer.
define square with n as Int returning Int:
    return n * n
```

### Generating Documentation

Use the `doc` CLI subcommand to automatically scan files and compile markdown API summaries:

```bash
ernos doc my_program.ep -o api_docs.md
```

---

<p align="center"><b>Ernos</b> — Code that reads like English. Runs like C.</p>

## Local Echo compiler override: import identity

The self-hosted compiler identifies imported source modules by their canonical filesystem path. Relative dot segments and symlink spellings of the same source file therefore do not cause duplicate function definitions. Imports still resolve relative to their importing source, with the existing standard-library lookup. An unreadable or unresolvable source produces a compiler error. This matches the reference compiler's canonical source identity.
