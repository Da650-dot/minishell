#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <test-name>"
  echo "Case files live in tests/manual_tests/cases/*.cmd"
  exit 2
fi

TEST_NAME="$1"
CASE_FILE="tests/manual_tests/cases/${TEST_NAME}.cmd"
OUT_DIR="tests/manual_tests/out"
OUT_FILE="${OUT_DIR}/${TEST_NAME}.out"

if [ ! -f "$CASE_FILE" ]; then
  echo "Case file not found: $CASE_FILE"
  exit 1
fi

mkdir -p "$OUT_DIR"

echo "Running manual test: $TEST_NAME"
# Use `script` to allocate a pty so readline-based shells behave correctly.
script -q /dev/null ./minishell > "$OUT_FILE" 2>&1 <<EOF
$(cat "$CASE_FILE")
EOF

echo "Saved output: $OUT_FILE"
