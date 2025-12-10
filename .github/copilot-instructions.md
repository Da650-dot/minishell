# Minishell - AI Coding Agent Instructions

## Project Overview
A minimal bash-like shell implementation in C with readline support. Built as a learning project following strict coding standards (42 Norminette-style).

## Architecture Pipeline

The shell follows a **4-stage pipeline architecture**:

```
User Input → Lexer → Parser → Expansion → Executor
```

### 1. Lexer (`src/lexer/`)
- **Input**: Raw string from readline
- **Output**: Linked list of `t_token` structures
- Tokenizes input into: `TOKEN_WORD`, `TOKEN_PIPE`, `TOKEN_REDIR_IN/OUT`, `TOKEN_APPEND`, `TOKEN_HEREDOC`
- Handles quote preservation (quotes removed later in expansion)
- Entry point: `tokenize()` in `lexer.c`

### 2. Parser (`src/parser/`)
- **Input**: `t_token` linked list
- **Output**: `t_pipeline` linked list (commands separated by pipes)
- Each pipeline node contains a `t_cmd` with:
  - `char **args` - command arguments
  - `t_redir *redirects` - linked list of redirections
  - `int heredoc_fd` - file descriptor for heredoc input
- Entry point: `parse_tokens()` in `parser.c`

### 3. Expansion (`src/expansion/`)
- Expands environment variables (`$VAR`, `$?`) **after parsing**
- Quote handling: Single quotes block expansion, double quotes allow it
- Expansion happens in two places:
  - Command arguments via `expand_args()` before execution
  - Redirection filenames via `expand_redir_filename()` when applying redirects
- Critical: `$?` expands to `data->exit_status`

### 4. Executor (`src/executor/`, `src/operators/`)
- **Single commands**: Execute directly in main process (builtins) or fork (externals)
- **Pipelines**: Fork chain with pipe file descriptors passed between processes
- Heredocs processed **before** forking via `prepare_pipeline_heredocs()`
- Execution flow: `process_pipeline()` → `execute_pipeline()` → `handle_single_command()` or `execute_multi_pipeline()`

## Critical Data Structures

```c
typedef struct s_data {
    char **envp;           // Environment (dynamically managed)
    int exit_status;       // Last command exit status ($?)
    t_token *tokens;       // Temp storage during parsing
    t_pipeline *pipeline;  // Temp storage during parsing
} t_data;

typedef struct s_pipeline {
    t_cmd *cmd;
    struct s_pipeline *next;  // Next command in pipe chain
} t_pipeline;

typedef struct s_cmd {
    char **args;          // NULL-terminated array
    t_redir *redirects;   // Linked list of I/O redirections
    int heredoc_fd;       // -1 or pipe read end
} t_cmd;
```

## Signal Handling (`src/signals/`)

**Three distinct signal contexts**:
1. `setup_signals_interactive()` - REPL loop (SIGINT displays new prompt, SIGQUIT ignored)
2. `setup_signals_execution()` - Parent during child execution (both ignored)
3. `setup_signals_default()` - Child processes (default handlers)
4. `setup_signals_heredoc()` - Heredoc input (SIGINT terminates heredoc)

Global variable `g_signal` tracks interruptions. Check with `was_interrupted()` after blocking calls.

## Built-in Commands (`src/builtins/`)

Seven builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

**Critical distinction**:
- Builtins in **single commands**: Execute in main process (env changes persist)
- Builtins in **pipelines**: Fork and execute in child (env changes lost)

Check: `is_builtin()` → Execute: `execute_builtin()`

## Environment Management

- `t_data.envp` is **dynamically allocated** - use `add_env_var()` / `remove_env_var()`
- Always update `envp` pointer after modifications (reallocations happen)
- Example: `data->envp = add_env_var(data->envp, "PATH=/bin")`

## File Descriptor Management

**Critical cleanup pattern**:
1. Heredocs create pipe fds stored in `cmd->heredoc_fd`
2. Redirections use `dup2()` to redirect stdio, then close original fd
3. Pipeline uses `create_pipes()` to allocate fd array: `int **pipefds`
4. Always call `close_fds()` after pipeline execution
5. Backup/restore stdio in parent: `save_stdio()` / `restore_stdio()`

## Build & Test Workflow

```bash
make              # Build minishell
make re           # Clean rebuild
make run          # Run with valgrind + readline suppressions
```

**Valgrind setup**: Uses `readline.sup` to suppress readline-specific leaks

**Compilation flags**: `-Wall -Wextra -Werror -g` + `-lreadline`

## Code Organization Conventions

### Directory Structure Logic
- `src/main/` - Entry point, REPL loop, data initialization
- `src/utils/` - Generic helpers (env_utils, prompt, token_utils)
- `src/operators/` - Complex operators (pipelines, redirections, heredocs)
- `obj/` - Mirrors `src/` structure for object files

### Norminette-Style Patterns
- **25-line function limit**: Complex functions split into static helpers
- **5-function-per-file soft limit**: Related logic grouped in same file
- **No variable declarations mid-function**: All at top
- Example: `pipeline.c` has minimal dispatcher, logic in `pipeline_execute.c`, `pipeline_fork.c`, etc.

### Memory Management
- **No memory leaks policy**: Every malloc has corresponding free path
- Cleanup functions: `free_tokens()`, `free_pipeline()`, `free_cmd()`, `free_array()`
- `cleanup_data()` called before exit in main

## Common Pitfalls

1. **Quote removal timing**: Quotes preserved in tokens, removed during expansion with `remove_quotes()`
2. **Heredoc expansion**: Only if delimiter is unquoted - check `extract_delimiter()` logic
3. **Exit status**: Update `data->exit_status` after every command execution
4. **Builtin fork logic**: Check `handle_single_command()` for single vs pipeline distinction
5. **Redirection order**: Process left-to-right (last redirect wins for same fd)

## Testing Approach

Focus areas:
- Quote handling edge cases: `echo "hello'world'"`, `export VAR='$USER'`
- Pipeline fd leaks: Run `valgrind --track-fds=yes`
- Signal interruption: Ctrl+C during heredoc, during command execution
- Builtin env persistence: `export` in single vs pipeline context
- Empty/whitespace input handling in REPL

## Key Files to Read First

1. `includes/minishell.h` - All structures and function signatures
2. `src/main/main.c` - REPL loop and execution flow
3. `src/executor/executor_minimal.c` - Pipeline processing entry point
4. `src/operators/pipeline.c` - Single vs multi-command dispatch logic
