# Copilot instructions for this repository

Purpose: give AI coding agents a compact, actionable guide to be productive in this minishell C project.

Quick start
- Build: run `make` at repo root (note: the makefile is lowercase `makefile`).
- Run: `./minishell`.
- Clean: `make clean` / `make fclean` / `make re`.

Key files (what to open first)
- `includes/minishell.h` — canonical data shapes and global (`g_signal`).
- `src/main/main.c` — REPL: input → tokenize → parse → expand → execute.
- `src/lexer/*` — tokenizer (words, redirects, utils).
- `src/parser/*` — builds `t_pipeline` and `t_cmd` structures.
- `src/executor/executor_minimal.c` — minimal executor (builtins only; where to add fork/exec).
- `src/builtins/*` — builtin implementations and integration points (`is_builtin`, `execute_builtin`).
- `src/signals/*` — use `setup_signals_interactive` / `setup_signals_execution` when adding blocking/child code.
- `src/permissions/permissions.c` — `find_executable()`; use for external command lookup.

Data flow & intent (why files are split)
- Main handles lifecycle and I/O. Tokenizer produces a token list; parser converts tokens → `t_pipeline` (linked commands); executor consumes the pipeline. This separation lets you implement executor features without touching lexer/parser.

Project conventions (be concrete)
- Memory: many functions allocate and expect caller frees — prefer existing helpers: `free_tokens`, `free_pipeline`, `free_cmd`, `free_array`.
- Error patterns: functions often return `NULL` on failure or `bool` for success; follow caller expectations (e.g., `parse_tokens` returns `NULL` on syntax errors).
- Debugging: use `print_tokens` and `print_pipeline` (already present) rather than adding ad-hoc prints.
- Readline & history: `add_history` used in `main.c`; signal setup is toggled around readline — mirror that when adding blocking syscalls.

Integration notes & gotchas
- `libft/` is built by the makefile. Avoid changing link steps unless you update the makefile too.
- Link-time dependency: `readline` (CI must have it).
- Executor is intentionally minimal: pipes & redirections are not fully implemented — check `src/executor/*` and `src/redirections/*` before assuming behavior.
- Watch for duplicate prototypes in `includes/minishell.h` if you change signatures.

Concrete examples (copy/paste actions)
- Trace input: open `src/main/main.c` → call chain: `get_user_input()` → `tokenize()` → `parse_tokens()` → `process_pipeline()`.
- Add an external exec: modify `src/executor/executor_minimal.c`, use `find_executable()` and `fork()`+`execve()`; call `setup_signals_execution()` in parent/child appropriately.


---

## UPDATED (merged guidance)

The original block above is preserved for history. The section below replaces it as the authoritative, up-to-date guidance for AI coding agents working on this repository.

Purpose: provide concise, repository-specific guidance so AI coding agents can be productive immediately working on this minishell C project.

Quick start
- Build: run `make` at the repository root. (Note: the included makefile is lowercase `makefile`.)
- Run: `./minishell` from the repo root after `make` completes.
- Clean: `make clean`, `make fclean`, or `make re`.

High-level architecture
- `src/main/main.c`: REPL and top-level lifecycle (readline, history, and orchestration).
- Lexing -> Parsing -> Expansion -> Execution: tokens are produced in `src/lexer/*`, parsed into `t_pipeline`/`t_cmd` in `src/parser/*`, expanded in `src/expansion/*`, and executed in `src/executor/*`.
- Builtins live in `src/builtins/*` and are executed directly by the executor path (see `is_builtin` / `execute_builtin`).
- Signals and blocking behavior are controlled in `src/signals/*` (use `setup_signals_interactive` vs `setup_signals_execution`).

Key files to open first
- `includes/minishell.h` — canonical types, global state (`g_signal`), and many prototypes.
- `src/main/main.c` and `src/main/main_utils.c` — show how readline, `add_history`, and signal toggles are used.
- `src/lexer/*` — tokenization rules (words, redirects, utilities).
- `src/parser/*` — how tokens are converted into `t_pipeline` and `t_cmd`.
- `src/expansion/*` — variable expansion and quote handling.
- `src/executor/*` (esp. `executor_minimal.c`, `pipeline_execute.c`, `pipeline_fork.c`) — where external exec, forking, pipes and redirections are implemented or extended.
- `src/permissions/permissions.c` — `find_executable()` helper for locating external programs.

Critical developer workflows
- Build and run: `make && ./minishell`.
- Tests and quick checks: use the scripts in `tests/`:
	- `tests/run_mandatory_tests.sh` — runs mandatory behavior checks.
	- `tests/run_redirections_only.sh` — focused redirections tests.
	- `tests/test_leak.sh` — runs Valgrind/leak checks (useful for memory work).
	- `tests/manual_run_all.sh` — convenience script to run several scenarios.

Repository-specific conventions
- Memory management: many functions allocate and expect the caller to free. Prefer helpers in the codebase: `free_tokens`, `free_pipeline`, `free_cmd`, `free_array`.
- Error signaling: functions often return `NULL` on failure or `bool` for success — match the expected convention used by callers (e.g., `parse_tokens()` returns `NULL` on syntax errors).
- Debugging helpers: use existing `print_tokens` and `print_pipeline` instead of adding new ad-hoc prints.
- Readline/signal pattern: `add_history()` is called in `main.c`; code switches signal handling around readline and blocking syscalls — mirror this when adding children/exec/fork.

Integration points and extension notes
- To add external execution: prefer `find_executable()` in `src/permissions/permissions.c`, then `fork()` + `execve()` from `src/executor/*` (example starting point: `executor_minimal.c`). Ensure you call `setup_signals_execution()` for correct signal semantics in parent/child.
- Pipes, forks and redirections: `pipeline_fork.c`, `pipeline_execute.c`, and `redirections.c` contain the plumbing. Read these before implementing changes to avoid breaking descriptor handling.
- Builtins: defined in `src/builtins/*` and invoked by the executor path. To add or modify a builtin, update `is_builtin` / `execute_builtin` logic.

CI/build gotchas and external deps
- `readline` is a link-time dependency — CI must have it installed.
- `libft/` is built by the makefile; do not change link rules unless you also update `makefile`.
- Watch for duplicate prototypes in `includes/minishell.h` when changing function signatures.

Concrete copy/paste examples
- Trace the input path: `get_user_input()` (in `src/main/main.c`) -> `tokenize()` (`src/lexer/`) -> `parse_tokens()` (`src/parser/`) -> `process_pipeline()`/`pipeline_execute()` (`src/executor/`).
- Add external exec: modify `src/executor/executor_minimal.c`. Use `find_executable()` then `fork()` + `execve()`; call `setup_signals_execution()` in parent/child as appropriate.

Debugging & testing tips
- Use `tests/test_leak.sh` and `valgrind` for memory regression checks.
- Use `print_tokens` / `print_pipeline` to inspect intermediate data structures.
- Run focused scripts in `tests/` during iteration to avoid long runs.

If something’s missing (CI details, platform quirks, or deeper examples), tell me which area to expand and I’ll update this file with more targeted instructions.

Please review and tell me if you want any sections expanded (build matrix, valgrind tips, or more code examples).
