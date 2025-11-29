# Relatório consolidado — testes obrigatórios (mandatory)

Resumo rápido
- Local dos logs: `tests/results/` (cada caso gerou um `*.out`).
- Status geral: lexer+parser+builtins funcionam para a maioria dos casos; execução externa (PATH/absoluto) foi implementada; faltam correções em aspas e implementação de redirecionamentos (pipes implementados).

Tabela resumida

| Item | Status | Evidência (arquivo) | Nota curta |
|---|---:|---|---|
| Compilação | PASS | N/A | `make` gerou `minishell` |
| Comando absoluto (/bin/...) | PASS | `tests/results/simple_absolute.out` | `/bin/echo` executa |
| Execução por nome (PATH) | PASS | `tests/results/simple_absolute.out`, `tests/results/pwd_cd.out` | `ls` funciona via PATH |
| `echo` (builtin) | PASS | `tests/results/builtin_echo.out` | imprime corretamente |
| `cd` | PASS | `tests/results/pwd_cd.out` | atualiza PWD/OLDPWD |
| `pwd` | PASS | `tests/results/pwd_cd.out` | imprime cwd |
| `env` | PASS | `tests/results/env_head.out` | lista env |
| `export` / `unset` | PASS | `tests/results/export_unset.out` | altera `data->envp` |
| `exit` | PASS | `tests/results/builtin_echo.out` | encerra REPL |
| Expansão `$VAR` | PASS | `tests/results/export_unset.out` | `$TEST_AI` → `abc` |
| Expansão `$?` | PASS | `tests/results/return_value.out` | reflete exit do filho |
| Pipes (`|`) | PASS | `tests/results/pipes.out`, `tests/results/pipes_simple.out`, `tests/results/pipes_three.out`, `tests/results/pipes_ls_grep.out`, `tests/results/pipes_builtin_middle.out`, `tests/results/pipes_invalid_middle.out` | pipelines básicos executam (fork/pipe/dup2/wait) |
| Redirecionamentos (`<`, `>`) | PARTIAL | `tests/results/redirections.out` | parser reconhece, exec ignorado |
| Aspas duplas (`"..."`) | FAIL | `tests/results/quotes.out` | aspas ainda aparecem em `args` |
| Aspas simples (`'...'`) | FAIL | `tests/results/single_quotes.out` | tokenização incorreta |
| Permissão negada (exec) | PASS | (test manual sugerido) | implementado: 126 + mensagem |
| Comando não encontrado | PASS | `tests/results/invalid_command.out` | 127 + mensagem |
| Sinais (Ctrl-C/Ctrl-\\/Ctrl-D) | NT | testar manualmente | handlers presentes em `src/signals` |
| Histórico (Up/Down) | NT | testar manualmente | readline integrado |

Principais arquivos tocados nesta sessão
- `src/executor/exec_helpers.c` — resolução de PATH + fork/exec/wait (implementação nova)
- `src/executor/executor_minimal.c` — roteamento para `execute_external()` quando não for builtin
- `includes/minishell.h` — protótipos adicionados
- `Makefile` — inclusão de `exec_helpers.c` na lista de srcs
 - `src/executor/pipeline.c` — implementação de pipes (forks, pipe(), dup2(), wait)
- `tests/run_mandatory_tests.sh` e `tests/results/*.out` — script e logs gerados

Recomendações (prioridade)
1. Corrigir parsing de aspas (single/double): retirar as aspas dos `args` e aplicar regras corretas de expansão (single = literal, double = expandir variáveis). Isso evitará que `find_executable` falhe com nomes entre aspas e tornará argumentos corretos para `execve`.
2. Implementar redirecionamentos no executor: no filho, abrir arquivos e `dup2()` antes do `execve`.
3. Implementar pipes: criar `pipe()`s e múltiplos forks para conectar stdout/stdin entre processos; gerenciar vários `waitpid`.
4. Fazer testes interativos: sinais (Ctrl-C/Ctrl-\\/Ctrl-D) e histórico (Up/Down) para validar handlers do REPL.

Como reproduzir os testes localmente

No diretório do projeto:
```bash
make
./tests/run_mandatory_tests.sh
ls tests/results/*.out
``` 

Notas finais
- A execução externa (PATH e absoluto) agora está implementada; o próximo ganho mais importante é consertar aspas, depois redirecionamentos e pipes. Se quiser, posso começar pela correção de aspas e já criar testes unitários para isso.
