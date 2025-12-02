#!/usr/bin/env zsh
# Runner não-interativo para alguns testes manuais do minishell
# Gera arquivos em tests/manual_results/

set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR"
MINISHELL="$BUILD_DIR/minishell"
OUTDIR="$ROOT_DIR/tests/manual_results"
mkdir -p "$OUTDIR"

run_case() {
  name="$1"
  shift
  echo "=== Running: $name"
  # comandos são passados como múltiplos argumentos e juntados em linhas
  cmds=("$@")
  # garante que o shell processe corretamente novas linhas
  printf "%s\n" "${cmds[@]}" | "$MINISHELL" > "$OUTDIR/$name.out" 2>&1 || true
  echo "  -> saved: $OUTDIR/$name.out"
}

# 1) Builtins
run_case builtin_export "export ARG=hello" "echo \$ARG" "echo \"\$ARG\"" "echo '\$ARG'" "unset ARG" "echo \$ARG" "exit"

# 2) pwd/cd
run_case builtin_cd_pwd "pwd" "cd /" "pwd" "cd -" "pwd" "exit"

# 3) Pipes
run_case pipes_simple "printf 'a\nb\nc\n' | grep b" "exit"
run_case pipes_chain "printf '1 2 3 4\n' | tr ' ' '\\n' | wc -l" "exit"

# 4) Redirections
run_case redir_overwrite "echo hello > /tmp/minishell_test_1" "cat /tmp/minishell_test_1" "exit"
run_case redir_append "echo one > /tmp/minishell_test_2" "echo two >> /tmp/minishell_test_2" "cat /tmp/minishell_test_2" "exit"
run_case redir_input "printf 'infile line\n' > /tmp/minishell_in" "cat < /tmp/minishell_in" "exit"

# 5) Heredoc
run_case heredoc_expand "export X=42" "cat <<EOF" "\$X" "EOF" "exit"
run_case heredoc_noexpand "export Y=99" "cat <<'EOF'" "\$Y" "EOF" "exit"
run_case heredoc_pipe "cat <<EOF | wc -l" "line1" "line2" "EOF" "exit"

# 6) Expansion and quotes
run_case expansion_vars "export V=abc" "echo \$V" "echo \"\$V\"" "echo '\$V'" "exit"
run_case exit_status "false" "echo \$?" "exit"

# 7) Errors / permissions (non fatal to runner)
run_case missing_input "cat < /no/such/file" "exit"
run_case write_dir "echo hi > /" "exit"

# 8) Edge cases
run_case empty_line "" "exit"
run_case spaces_redir "echo hi> /tmp/minishell_test_3" "cat /tmp/minishell_test_3" "exit"
run_case multiple_redir "echo 1 > /tmp/a > /tmp/b" "cat /tmp/a" "cat /tmp/b" "exit"

# Summary
echo "Manual run completed. Results in: $OUTDIR"
ls -l "$OUTDIR"

# Exit with success (the cases themselves may record failures in their outputs)
exit 0
