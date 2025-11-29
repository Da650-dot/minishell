#!/usr/bin/env bash
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/minishell"
OUTDIR="$ROOT/tests/results"

mkdir -p "$OUTDIR"

run_test() {
  local name="$1"
  local here_doc="$2"
  local outfile="$OUTDIR/${name}.out"
  echo "==== Running: $name" | tee "$outfile"
  echo "--- COMMANDS ---" >> "$outfile"
  echo "$here_doc" >> "$outfile"
  echo "--- OUTPUT ---" >> "$outfile"
  (cd "$ROOT" && printf "%s\n" "$here_doc" | "$BIN" ) >> "$outfile" 2>&1 || true
  echo "--- EXIT STATUS ---" >> "$outfile"
  # can't capture exit status of minishell easily here because it's interactive; rely on printed output
  echo "(see output capture)" >> "$outfile"
  echo "Saved: $outfile"
}

echo "Tests results -> $OUTDIR"

run_test "simple_absolute" $'/bin/echo hello\nexit 0'
run_test "builtin_echo" $'echo world\nexit 0'
run_test "invalid_command" $'invalidcmd\nexit 0'
run_test "ls_nonexistent" $'/bin/ls /nonexistent\nexit 0'
run_test "return_value" $'/bin/false\necho $?\nexit 0'
run_test "env_head" $'env | head -n 5\nexit 0'
run_test "export_unset" $'export TEST_AI=abc\necho $TEST_AI\nunset TEST_AI\necho $TEST_AI\nexit 0'
run_test "pwd_cd" $'pwd\ncd ..\npwd\nexit 0'
run_test "path_unset" $'unset PATH\nls\necho $?\nexit 0'
run_test "quotes" $'echo "texto com espaço"\necho \"$USER\"\nexit 0'
run_test "redirections" $'echo teste > out.txt\ncat < out.txt\nexit 0'
run_test "pipes" $'echo foo | wc -c\nexit 0'
run_test "single_quotes" $"echo 'texto com $HOME e | pipe'\necho '\''\'\'\'empty\''\nexit 0"
run_test "error_cases" $'cd a b\nexport 1BAD=val\nexit 0'

echo "All tests written to $OUTDIR"
