Manual heredoc tests for the minishell project

These steps let you manually exercise the heredoc implementation in the built `minishell` binary.

Quick build

```bash
# from repository root
make re
```

Simple heredoc (basic)

```bash
# Start shell interactively
./minishell
# Then type in the shell:
cat <<EOF
hello
EOF
# You should see 'hello' printed
```

Non-interactive quick-run (same thing piped):

```bash
printf 'cat <<EOF
hello
EOF
exit
' | ./minishell
```

Heredoc with expansion (delimiter NOT quoted):

```bash
# inside minishell or via piped commands
export X=world
cat <<EOF
hello $X
EOF
# Should print: hello world
```

Heredoc with quoted delimiter (no expansion):

```bash
export X=world
cat <<'EOF'
hello $X
EOF
# Should print: hello $X  (the literal $X, not expanded)
```

Heredoc piped to another command

```bash
cat <<EOF | wc -c
hello
EOF
# Should print a numeric count for 'hello\n' (6)
```

SIGINT during heredoc

- Start `./minishell`, run `cat <<EOF` and while typing the heredoc lines press Ctrl-C. The heredoc should be aborted and the shell should return to prompt without exiting the parent shell.

Notes and caveats

- The heredoc reader currently:
  - Reads lines with `readline("> ")` until the delimiter is matched exactly.
  - If the delimiter is quoted (single or double quotes), the heredoc lines are NOT expanded.
  - If the delimiter is not quoted, lines are fed through the shell expander (`expand_variables`) before being stored.
  - Heredoc content is written into a pipe; the read-end is used as `stdin` for the command.
- Edge cases not fully implemented yet:
  - `<<-` (strip leading tabs) is not supported.
  - Multiple heredocs in a single command are accepted but ordering/precedence of multiple redirections may need careful verification.

If you want, I can add automated tests for more heredoc edge cases and run valgrind to check for leaks.