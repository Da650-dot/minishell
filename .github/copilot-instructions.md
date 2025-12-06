# Copilot Instructions — Minishell

Purpose: enable AI coding agents to be immediately productive in this minishell implementation.

## Quick Start

- **Build:** `make` at repo root (lowercase `makefile`)
- **Run:** `./minishell`
- **Clean:** `make clean`, `make fclean`, `make re`

## Big Picture

Minishell is a POSIX shell replica with this data flow:

```
readline input → tokenize → parse → expand → execute
```

**Key components:**
- **Lexer** (`src/lexer/*`): Breaks input into tokens (words, pipes, redirects).
- **Parser** (`src/parser/*`): Builds `t_pipeline` (linked `t_cmd` nodes) from tokens.
- **Expansion** (`src/expansion/*`): Resolves variables (`$VAR`), quotes, and special chars within command args.
- **Executor** (`src/operators/pipeline_*.c`, `src/executor/*`): Runs builtins in-process; forks external commands.
- **Redirections** (`src/operators/redirections.c`): Applies `<`, `>`, `>>`, heredocs via `dup2`.
- **Signals** (`src/signals/*`): Manages SIGINT/SIGQUIT for interactive vs. execution contexts.

## Core Data Structures (from `includes/minishell.h`)

```c
t_pipeline → t_cmd  // Linked list of commands (|, <, >, >>)
t_cmd {
  char **args,        // Command + arguments
  t_redir *redirects, // Input/output redirections
  char *heredoc_delim,// Heredoc delimiter
  int heredoc_fd      // Heredoc temp file descriptor
}
t_data {
  char **envp,        // Environment variables
  int exit_status,    // Last exit code
  t_token *tokens,    // Parsed token list
  t_pipeline *pipeline// Execution graph
}
```

## Essential Files (In Reading Order)

1. **`includes/minishell.h`** — Type definitions, `g_signal` global, all function prototypes
2. **`src/main/main.c`** — REPL loop: calls `get_user_input()` → `tokenize()` → `parse_tokens()` → `process_pipeline()`
3. **`src/lexer/lexer.c`** — Token creation; `src/lexer/lexer_words.c` handles quoted strings
4. **`src/parser/parser.c`** — Main parse entry; calls `parse_single_cmd()` per command, links via pipes
5. **`src/operators/pipeline_execute.c`** — `handle_single_command()`: dispatches to builtin or external
6. **`src/operators/redirections.c`** — `apply_redirections()`, `apply_single_redir_in/out()`: file ops via `dup2`
7. **`src/expansion/expansion.c`** — Variable expansion; respects single/double quotes
8. **`src/builtins/builtin_utils.c`** — `is_builtin()` dispatch table; `execute_builtin()` router
9. **`src/signals/signals.c`** — `setup_signals_interactive()` vs `setup_signals_execution()`

## Critical Developer Workflows

### Building & Running
```bash
make                   # Compiles minishell + libft
./minishell            # Interactive REPL
make fclean && make    # Full rebuild (clean + compile)
```

### Testing & Debugging
```bash
tests/run_mandatory_tests.sh       # Baseline behavior suite
tests/run_redirections_only.sh     # Isolation: pipes, <, >, >>
tests/test_leak.sh                 # Valgrind memory checks (if available)
tests/manual_run_all.sh            # Runs multiple scenario sets
```

### Debugging Tips
- Use `print_tokens(tokens)` and `print_pipeline(pipeline)` (pre-built helpers)
- Trace with: main → `tokenize()` → `parse_tokens()` → `process_pipeline()` → `pipeline_execute()`
- Check `g_signal` for interrupt/signal state
- Pipe to file: `echo "cmd" | ./minishell > log 2>&1`

## Repository Conventions

### Memory Management
- **Caller-free model:** Many functions allocate; caller must free. Example: `tokenize()` returns a `t_token*` list; caller invokes `free_tokens()`.
- **Helper functions:** Use existing cleanup: `free_tokens()`, `free_pipeline()`, `free_cmd()`, `free_array()`.
- Avoid manual `free()` on individual nodes unless cleaning up failed allocations mid-function.

### Error Signaling
- **`NULL` return:** Indicates parse failure (e.g., `parse_tokens()` on syntax error).
- **`bool` return:** True = success, False = failure (e.g., `is_builtin()`).
- **Exit status:** Stored in `data->exit_status`; propagated by each executor step.

### Function Naming & Organization
- **`is_builtin()`** checks if a command is shell-native (echo, cd, pwd, export, unset, env, exit).
- **`execute_builtin()`** routes to the correct builtin implementation (`src/builtins/builtin_*.c`).
- **`execute_external()`** forks and execs non-builtins via `execve()`.

### Signal Handling (Critical!)
- **Interactive mode:** `setup_signals_interactive()` catches SIGINT (Ctrl+C) → newline + redisplay prompt.
- **Execution mode:** `setup_signals_execution()` ignores SIGINT/SIGQUIT in parent, allowing children to handle them.
- **Pattern:** Call before `readline()` (interactive) or before `fork()` (execution); call `reset_signal()` after to restore.

## Data Flow: Input → Execution

1. **readline input** (`main.c:get_user_input()`) + signal setup
2. **Tokenize** (`src/lexer/lexer.c:tokenize()`) → list of `t_token` (WORD, PIPE, REDIR_IN/OUT/APPEND, HEREDOC)
3. **Parse** (`src/parser/parser.c:parse_tokens()`) → validate tokens, build `t_pipeline` (cmd linked list)
4. **Expand** (`src/expansion/expansion.c:expand_*()`) → resolve `$VAR`, strip quotes in args
5. **Execute** (`src/operators/pipeline_execute.c:pipeline_execute()` or `handle_single_command()`):
   - If builtin: call `execute_builtin()` (in-process, may apply redirections first)
   - If external: `fork()` + setup child signals + `find_executable()` + `execve()`
6. **Cleanup** (`main.c:main_cleanup.c`) → free all structures

## Common Extension Points

### Adding a Builtin Command
1. Add case in `src/builtins/builtin_utils.c:is_builtin()` (name check)
2. Add case in `src/builtins/builtin_utils.c:execute_builtin()` (dispatch)
3. Implement logic in `src/builtins/builtin_*.c` with signature `int cmd_name(char **args, t_data *data)`
4. Return exit code in `data->exit_status`

### Modifying External Execution
- **Lookup:** `src/permissions/permissions.c:find_executable()` searches PATH
- **Exec:** `src/executor/exec_external.c:execute_external()` calls `fork()` → child sets up redirections + calls `execve()`
- **Signals:** Ensure child inherits `setup_signals_execution()` semantics

### Pipes & Redirections
- **Pipes:** `src/operators/pipeline_fork.c` creates `pipe()` array, forks children, wires stdout↔stdin
- **Redirections:** `src/operators/redirections.c:apply_redirections()` opens files + calls `dup2()` to rewire stdin/stdout
- **Heredocs:** Parsed as `TOKEN_HEREDOC` with delimiter; handled in `src/operators/heredoc.c`

## Build & Link Details

- **Makefile:** Compiles `src/**/*.c` → `obj/*/` → links with `libft.a` + `-lreadline`
- **libft:** Built separately; do not skip or modify link flags without updating Makefile
- **CI dependency:** `readline` must be installed (`apt install libreadline-dev` on Linux)
- **Flags:** `-Wall -Wextra -Werror -g` (debug symbols enabled by default)

## Gotchas & Pitfalls

- **Duplicate prototypes in `minishell.h`:** If changing function signature, update declaration immediately.
- **Signal toggling:** Always pair `setup_signals_interactive()` + `reset_signal()` around readline; do NOT forget in new blocking code.
- **File descriptor leaks:** Redirections use `dup2()` → original FD still open. Close explicitly if needed.
- **Heredoc cleanup:** Temp files created in `src/operators/heredoc.c` must be closed before next prompt.
- **Pipeline memory:** `parse_tokens()` returns `NULL` on error; check before using, else memory leaks.

## Testing Patterns

From `tests/run_mandatory_tests.sh`:
- Pipe input to minishell: `printf "%s\n" "cmd1\ncmd2" | ./minishell`
- Capture output with stdout + stderr: `2>&1`
- Strip ANSI escapes for assertion: `sed -r 's/\x1B\[[0-9;]*[A-Za-z]//g'`
- Compare output against expected; log to `tests/results/`

## Example: Trace a Simple Command

```
Input: "echo hello | cat"
  ↓ tokenize() → [WORD "echo", WORD "hello", PIPE, WORD "cat"]
  ↓ parse_tokens() → pipeline: cmd["echo", "hello"] → cmd["cat"]
  ↓ expand_pipeline_vars() → (no $VAR; unchanged)
  ↓ pipeline_execute() → fork + pipe setup → child1 runs "echo hello" → child2 reads pipe
  ↓ collect exit status
  ↓ free_pipeline(), return to REPL
```

---

**Questions?** Review the codebase sections listed above for the feature you're modifying; the pattern usually generalizes.
