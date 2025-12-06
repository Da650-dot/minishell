# Copilot Instructions — Minishell

Purpose: make agents productive quickly on this minishell. Keep advice concrete and codebase-specific.

## Overview
- Shell flow: `readline` → `tokenize` → `parse` → `expand` → `execute` (pipes/redirs) → cleanup.
- Core structs in `includes/minishell.h`: `t_data` (envp, exit_status, tokens, pipeline), `t_pipeline` (linked `t_cmd`), `t_cmd` (args, redirects, heredoc fields). `g_signal` tracks interrupts.
- Primary files: `src/main/main.c` (REPL loop), `src/lexer/*.c` (tokens incl. quoted words, redirs), `src/parser/parser.c` (builds pipeline), `src/expansion/*.c` (quote/var handling), `src/operators/pipeline_execute.c` + `pipeline_fork.c` (dispatch + pipes), `src/operators/redirections.c`, `src/operators/heredoc*.c`, `src/executor/exec_external.c`, `src/builtins/builtin_utils.c`, `src/signals/signals.c`.

## Build & Run
- Build with `make` (root lowercase `Makefile`), uses `libft/` and links `-lreadline`. Rebuild: `make fclean && make`. Run `./minishell`.

## Tests
- Local scripts: `tests/run_mandatory_tests.sh`, `tests/run_redirections_only.sh`, `tests/manual_run_all.sh`, `tests/test_leak.sh` (valgrind if available).
- External suite: `minishell_tester/tester` (clone already present) supports `./tester`, or focused sets (`./tester builtins|pipes|redirects|extras|wildcards`). Manual scenarios under `minishell_tester/manual_tests/`.

## Execution model
- Builtins run in-process; externals fork/exec via `execute_external()` after PATH lookup in `src/permissions/permissions.c`.
- Pipelines wired in `pipeline_fork.c`; redirs applied per-command via `apply_redirections()`/`apply_single_redir_*()`; heredocs handled in `heredoc*.c` with temp files/fds.
- Signals: call `setup_signals_interactive()` before `readline()`, `setup_signals_execution()` before forking; `reset_signal()` after. Parent ignores SIGINT/SIGQUIT during execution so children handle them.

## Conventions & patterns
- Caller frees: most creators return owned allocations. Prefer helpers `free_tokens()`, `free_pipeline()`, `free_cmd()`, `free_array()`; avoid piecemeal frees unless unwinding failures.
- Error signaling: parsing returns `NULL` on syntax errors; many helpers return `bool`; always propagate `data->exit_status`.
- Keep prototypes in sync with `includes/minishell.h` (duplicate declarations are easy to miss). Update header immediately on signature changes.
- Redirections use `dup2`; close original fds after duplication to avoid leaks. Heredoc temp fds must be closed before returning to prompt.

## Debugging tips
- Trace path: `main` → `tokenize` → `parse_tokens` → `expand_pipeline_vars` → `pipeline_execute`. Use `print_tokens()` / `print_pipeline()` helpers.
- To capture behavior: `echo "cmd" | ./minishell > log 2>&1`; strip ANSI with `sed -r 's/\x1B\[[0-9;]*[A-Za-z]//g'` (tests use this).

## Common extension points
- Add builtin: update `is_builtin()` + `execute_builtin()` in `src/builtins/builtin_utils.c`, implement `src/builtins/builtin_<name>.c` returning exit code into `data->exit_status`.
- Adjust external execution: tweak PATH search in `permissions.c` or forking/dup in `exec_external.c` / `pipeline_fork.c`; keep signal setup consistent.

Questions or missing details? Ask for specific areas to clarify.
