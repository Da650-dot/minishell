#!/usr/bin/env bash
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/minishell"
OUTDIR="$ROOT/tests/results"

mkdir -p "$OUTDIR"

FAIL_COUNT=0

run_test() {
  local name="$1"
  local here_doc="$2"
  local expected="${3:-}"
  local outfile="$OUTDIR/${name}.out"
  echo "==== Running: $name" | tee "$outfile"
  echo "--- COMMANDS ---" >> "$outfile"
  echo "$here_doc" >> "$outfile"
  echo "--- OUTPUT ---" >> "$outfile"
  (cd "$ROOT" && printf "%s\n" "$here_doc" | "$BIN" ) >> "$outfile" 2>&1 || true
  echo "--- EXIT STATUS ---" >> "$outfile"
  # can't capture exit status of minishell easily here because it's interactive; rely on printed output
  echo "(see output capture)" >> "$outfile"

  # Prepare a cleaned copy without ANSI escapes to make assertions robust
  cleaned_out="$outfile.clean"
  # strip common ANSI escape sequences
  sed -r 's/\x1B\[[0-9;]*[A-Za-z]//g' "$outfile" > "$cleaned_out" 2>/dev/null || cp "$outfile" "$cleaned_out"

  # Simple assertions: support prefixes NUM:<n> (last numeric-only line equals n)
  # and SUB:<substring> (outfile contains substring), PAT:<grep regex>
  if [ -n "$expected" ]; then
    echo "--- ASSERTION ---" >> "$outfile"
    case "$expected" in
      NUM:*)
        exp_val="${expected#NUM:}"
        # find last numeric-only line in cleaned output
        found=$(grep -E '^[0-9]+$' "$cleaned_out" | tail -n1 || true)
        if [ "$found" = "$exp_val" ]; then
          echo "ASSERT PASS: numeric == $exp_val" >> "$outfile"
          echo "[PASS] $name"
        else
          echo "ASSERT FAIL: expected numeric $exp_val, found: '${found:-<none>}'" >> "$outfile"
          echo "[FAIL] $name"
          FAIL_COUNT=$((FAIL_COUNT+1))
        fi
        ;;
      SUB:*)
        exp_sub="${expected#SUB:}"
        if grep -F -q -- "${exp_sub}" "$cleaned_out"; then
          echo "ASSERT PASS: contains '$exp_sub'" >> "$outfile"
          echo "[PASS] $name"
        else
          echo "ASSERT FAIL: missing '$exp_sub'" >> "$outfile"
          echo "[FAIL] $name"
          FAIL_COUNT=$((FAIL_COUNT+1))
        fi
        ;;
      PAT:*)
        exp_pat="${expected#PAT:}"
        if grep -q -E "$exp_pat" "$cleaned_out"; then
          echo "ASSERT PASS: pattern '$exp_pat' matched" >> "$outfile"
          echo "[PASS] $name"
        else
          echo "ASSERT FAIL: pattern '$exp_pat' not matched" >> "$outfile"
          echo "[FAIL] $name"
          FAIL_COUNT=$((FAIL_COUNT+1))
        fi
        ;;
      *)
        echo "Unknown assertion type: $expected" >> "$outfile"
        echo "[WARN] $name (no assertion)"
        ;;
    esac
  else
    echo "[INFO] $name (no assertion)" >> "$outfile"
  fi
  rm -f "$cleaned_out"

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
run_test "redirections_overwrite" $'echo first > out_rw.txt\necho second > out_rw.txt\ncat out_rw.txt\nexit 0' "SUB:second"
run_test "redirections_append" $'echo line1 > out_ap.txt\necho line2 >> out_ap.txt\ncat out_ap.txt\nexit 0' "SUB:line2"
run_test "redirections_stderr" $'/bin/ls /nonexistent 2> err.txt\ncat err.txt\nexit 0' "SUB:No such"
run_test "redirections_input_wc" $'echo abc > in_wc.txt\nwc -c < in_wc.txt\nexit 0' "NUM:4"
run_test "pipes" $'echo foo | wc -c\nexit 0'
run_test "pipes" $'echo foo | wc -c\nexit 0' "NUM:4"
run_test "single_quotes" $"echo 'texto com $HOME e | pipe'\necho '\''\'\'\'empty\''\nexit 0"
run_test "error_cases" $'cd a b\nexport 1BAD=val\nexit 0'

# Additional pipeline-focused tests
run_test "pipes_simple" $'echo a | wc -c\nexit 0'
run_test "pipes_simple" $'echo a | wc -c\nexit 0' "NUM:2"
run_test "pipes_ls_grep" $'ls | grep src\nexit 0'
run_test "pipes_ls_grep" $'ls | grep src\nexit 0' "SUB:src"
run_test "pipes_three" $'echo x | wc -c | cat -e\nexit 0'
run_test "pipes_three" $'echo x | wc -c | cat -e\nexit 0' "SUB:2\$"
run_test "pipes_builtin_middle" $'echo hello | pwd\necho $?\nexit 0'
run_test "pipes_builtin_middle" $'echo hello | pwd\necho $?\nexit 0' "SUB:Minishell_github"
run_test "pipes_invalid_middle" $'echo hi | invalidcmd | wc -c\necho $?\nexit 0'
run_test "pipes_invalid_middle" $'echo hi | invalidcmd | wc -c\necho $?\nexit 0' "SUB:command not found"

# Pipeline exit status semantics: shell's $? should reflect last command in the pipeline
run_test "pipeline_status_true_false" $'true | false\necho $?\nexit 0' "NUM:1"
run_test "pipeline_status_false_true" $'false | true\necho $?\nexit 0' "NUM:0"

# Nested/escaped quotes sanity check
run_test "nested_quotes" $'echo "\'inner\'"\nexit 0' "SUB:'inner'"

echo "All tests written to $OUTDIR"

# Heredoc tests
run_test "heredoc_basic" "cat <<EOF
hello
EOF
exit 0" "SUB:hello"

run_test "heredoc_expand" "export X=world
cat <<EOF
a \$X
EOF
exit 0" "SUB:world"

run_test "heredoc_quoted" "export X=world
cat <<'EOF'
a \$X
EOF
exit 0" "SUB:\$X"

run_test "heredoc_pipe" "cat <<EOF | wc -c
hello
EOF
exit 0" "NUM:6"

echo "Heredoc tests written to $OUTDIR"

if [ "$FAIL_COUNT" -ne 0 ]; then
  echo "\nFAILED: $FAIL_COUNT test(s) failed." >&2
  exit 1
fi
