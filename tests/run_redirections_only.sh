#!/usr/bin/env bash
set -eu

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BIN="$ROOT/minishell"
OUTDIR="$ROOT/tests/results"

mkdir -p "$OUTDIR"

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
  echo "(see output capture)" >> "$outfile"

  cleaned_out="$outfile.clean"
  sed -r 's/\x1B\[[0-9;]*[A-Za-z]//g' "$outfile" > "$cleaned_out" 2>/dev/null || cp "$outfile" "$cleaned_out"

  if [ -n "$expected" ]; then
    echo "--- ASSERTION ---" >> "$outfile"
    case "$expected" in
      NUM:*)
        exp_val="${expected#NUM:}"
        found=$(grep -E '^[0-9]+$' "$cleaned_out" | tail -n1 || true)
        if [ "$found" = "$exp_val" ]; then
          echo "ASSERT PASS: numeric == $exp_val" >> "$outfile"
          echo "[PASS] $name"
        else
          echo "ASSERT FAIL: expected numeric $exp_val, found: '${found:-<none>}'" >> "$outfile"
          echo "[FAIL] $name"
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

# Redirection-only tests
run_test "redirs_overwrite" $'echo first > out_red.txt\necho second > out_red.txt\ncat out_red.txt\nexit 0' "SUB:second"
run_test "redirs_append" $'echo a > out_append.txt\necho b >> out_append.txt\ncat out_append.txt\nexit 0' "SUB:b"
run_test "redirs_input" $'echo abc > in_red.txt\nwc -c < in_red.txt\nexit 0' "NUM:4"
run_test "heredoc_basic" $'cat <<EOF\nhello\nEOF\nexit 0' "SUB:hello"
run_test "heredoc_quoted" $'export X=world\ncat <<\'EOF\'\n$X\nEOF\nexit 0' "SUB:\$X"
run_test "heredoc_pipe" $'cat <<EOF | wc -c\nhello\nEOF\nexit 0' "NUM:6"

echo "Redirections-only tests written to $OUTDIR"
