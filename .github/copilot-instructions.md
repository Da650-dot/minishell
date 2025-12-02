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

If something’s missing from this guide (CI, test harness, or platform quirks), tell me which part to expand and I’ll update this file.

Please review and tell me if you want any sections expanded (build matrix, valgrind tips, or more code examples).
