# Copilot instructions for this repository

Purpose: help AI coding agents become productive quickly in this minishell C project by summarizing the architecture, developer workflows, key conventions, and concrete code pointers.

- Quick start (build/run)
  - In repo root run: `make` (the project has a lowercase `makefile` that builds `libft` and the `minishell` binary).
  - Run the program: `./minishell`.
  - Clean: `make clean`, `make fclean`, full rebuild: `make re`.
  - Notes: the Makefile links against `libft` and `readline` (see `makefile` for flags: `-L./libft -lft -lreadline`).

- Big-picture architecture
  - Header: `includes/minishell.h` contains central types and prototypes (t_token, t_cmd, t_pipeline, t_data, global `g_signal`). Use it as the primary reference for data shapes.
  - REPL entrypoint: `src/main/main.c` — reads input (readline), adds history, and runs the pipeline: tokenize → parse → expand → execute.
  - Lexer: `src/lexer/*` (tokenize implementation lives across `lexer.c`, `lexer_words.c`, `lexer_redirect.c`). Tokens have types like `TOKEN_WORD`, `TOKEN_PIPE`, `TOKEN_REDIR_*`.
  - Parser: `src/parser/*` — turns token list into a `t_pipeline` (linked list of `t_cmd`). Key functions: `parse_tokens` and `parse_single_cmd`.
  - Executor: `src/executor/*` — currently there is a minimal executor (`executor_minimal.c`) that only handles built-ins and variable expansion; full external command exec, redirections and pipes are TODOs.
  - Builtins: `src/builtins/*` contains builtin implementations (echo, cd, env, export, unset, exit, pwd). `is_builtin()` and `execute_builtin()` are the integration points.
  - Signals: `src/signals/*` centralizes signal setup (`setup_signals_interactive`, `setup_signals_execution`, etc.). `g_signal` is a global `volatile sig_atomic_t` used to detect SIGINT.
  - Utilities: `src/utils/*`, `src/permissions/*` (helpers such as `find_executable`, permission checks), `libft/` contains partner library code (compiled as a separate target by the Makefile).

- Data flow and responsibilities (why things are split this way)
  - Main handles I/O and lifecycle (init/cleanup). Tokenization is isolated so parser only needs tokens. Parser builds an intermediate `t_pipeline` used by the executor. This separation allows independent testing and incremental implementation: you can implement executor pieces without changing lexer/parser.

- Project-specific conventions & patterns
  - Directory layout: keep related sources grouped under `src/<component>/` with matching header declarations in `includes/minishell.h`.
  - Memory and ownership: many functions return pointers and require manual free (there are `free_tokens`, `free_pipeline`, `free_cmd`, `free_array`). Follow existing free helpers when editing code.
  - Error/boolean patterns: functions commonly return `bool` for success, or `NULL` on allocation/failure. Observe existing callers' expectations (e.g., `parse_tokens` returns NULL on syntax errors and callers free prior state).
  - Debug helpers: `print_tokens` and `print_pipeline` exist and are used across the codebase for quick debugging — prefer using them for small changes rather than adding new printf noise.
  - Readline integration: `add_history` used in `main.c`; signal handling is carefully set before/after `readline` calls. Use provided `setup_signals_*` helpers when adding code that interacts with blocking syscalls or child processes.

- Integration points / external deps
  - `libft/` is built as part of the Makefile. Do not modify the Makefile unless you update the libft build step too.
  - `readline` is required at link time. Tests or CI must have `readline` available.
  - `permissions.c` provides `find_executable` and permission checks — use these when implementing external command execution.

- Concrete examples and pointers
  - To trace input handling: read `src/main/main.c` — `get_user_input()` → `tokenize()` → `parse_tokens()` → `process_pipeline()`.
  - To extend tokenization: look at `src/lexer/lexer_words.c` and `lexer_redirect.c`; new operators should be added as `t_token_type` values in `includes/minishell.h` and handled consistently by `tokenize()`.
  - To implement external commands: edit `src/executor/executor_minimal.c` and use `find_executable()` from `src/permissions/permissions.c`, fork+exec, and call `setup_signals_execution()` in the child/parent appropriately.

- Known quirks & gotchas observed in the codebase (detectable from sources)
  - Header duplications: some prototypes (e.g., `expand_variables`) appear declared more than once in `includes/minishell.h`. Be careful when changing signatures — update all declarations.
  - The Makefile filename is lowercase `makefile` (works with `make`, but CI scripts may assume `Makefile`).
  - Executor is intentionally minimal: pipes and redirections are not yet implemented — many code comments/warnings signal this; tests or feature PRs should account for that.

- Suggested short workflow for a small change (example: add support for simple external commands)
  1. Implement lookup/exec path in `src/executor/executor_minimal.c` using `find_executable()`.
 2. Compile locally with `make` and run `./minishell`.
 3. Use `print_pipeline` and `print_tokens` to validate parser output for complex inputs (pipes/redirects).
 4. Run under `valgrind --leak-check=full ./minishell` for memory checks.

If anything is missing or you want more examples (specific files or a proposed unit-test scaffold), tell me which part to expand and I'll iterate.
