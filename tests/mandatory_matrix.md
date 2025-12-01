# Matriz rápida de status — testes obrigatórios

Esta tabela resume o estado dos testes ao momento da última execução automática. Os ficheiros de log estão em `tests/results/`.

| Teste | Status | Nota curta | Evidência (log) |
|---|---:|---|---|
| Compilação (make) | PASS | binário gerado | N/A |
| Comando absoluto (/bin/…) | FAIL | externos não executados | [simple_absolute.out](tests/results/simple_absolute.out) |
| Execução por nome (PATH) | FAIL | não pesquisado/executado | [pwd_cd.out](tests/results/pwd_cd.out) |
| echo (builtin) | PASS | imprime corretamente | [builtin_echo.out](tests/results/builtin_echo.out) |
| cd | PASS | altera PWD/OLDPWD | [pwd_cd.out](tests/results/pwd_cd.out) |
| pwd | PASS | imprime cwd | [pwd_cd.out](tests/results/pwd_cd.out) |
| env | PASS | lista env | [env_head.out](tests/results/env_head.out) |
| export / unset | PASS | modifica `data->envp` | [export_unset.out](tests/results/export_unset.out) |
| exit | PASS | encerra REPL | [builtin_echo.out](tests/results/builtin_echo.out) |
| Expansão `$VAR` | PASS | funciona para builtins | [export_unset.out](tests/results/export_unset.out) |
| Expansão `$?` | PARTIAL | funciona; externos → 127 | [return_value.out](tests/results/return_value.out) |
| Pipes (`|`) | PARTIAL | implementados parcialmente / testados | [pipes.out](tests/results/pipes.out), [pipes_simple.out](tests/results/pipes_simple.out), [pipes_three.out](tests/results/pipes_three.out) |
| Redirecionamentos (`<`, `>`, `>>`) | PARTIAL | basic input/output/append implemented; heredoc pending | [redirections.out](tests/results/redirections.out) |
| Aspas duplas (`"..."`) | FAIL | aspas preservadas nos args | [quotes.out](tests/results/quotes.out) |
| Aspas simples (`'...'`) | FAIL | tokenização incorreta | [single_quotes.out](tests/results/single_quotes.out) |
| Sinais (Ctrl-C / Ctrl-\ / Ctrl-D) | NT | testar manualmente | N/A |
| Histórico (Up/Down) | NT | testar interativo (readline) | N/A |

Notas:
- "PASS/FAIL/PARTIAL/NT" reflete o estado da execução automática e testes manuais reportados; revise os logs para detalhes.
- Alguns itens (ex.: execução via PATH) podem variar conforme `PATH` no ambiente de execução.
# Matriz de itens obrigatórios (mandatory)

Este arquivo mapeia cada item do enunciado (parte mandatory) para o estado atual no repositório e aponta para a evidência em `tests/results/`.

Legenda: PASS / PARTIAL / FAIL / NT = não testado (interativo)

- Compilação (`make`): PASS  — build local OK
- Comando absoluto (`/bin/echo`): FAIL  — ver `tests/results/simple_absolute.out`
- Builtin `echo`: PASS  — ver `tests/results/builtin_echo.out`
- `exit` builtin: PASS  — ver `tests/results/builtin_echo.out` (contém `exit` execução)
- Valor de retorno (`$?`): PARTIAL  — ver `tests/results/path_unset.out`
- Sinais (Ctrl-C / Ctrl-\\ / Ctrl-D): NT  — testar interativo
- Aspas duplas: FAIL  — ver `tests/results/quotes.out` (aspas preservadas nos args)
- Aspas simples: NT  — ver `tests/results/single_quotes.out` (novo teste)
- `env` builtin: PASS  — ver `tests/results/env_head.out`
- `export` / `unset`: PASS  — ver `tests/results/export_unset.out`
- `cd`: PASS  — ver `tests/results/pwd_cd.out`
- `pwd`: PASS  — ver `tests/results/pwd_cd.out`
- PATH / execução por nome: FAIL  — ver `tests/results/path_unset.out`
- Redirecionamentos (`>`, `<`, `>>`, `<<`): PARTIAL  — ver `tests/results/redirections.out` (parse OK, exec NÃO)
- Pipes (`|`): PARTIAL  — ver `tests/results/pipes.out` (parse OK, exec parcial)
- Histórico (Up/Down): NT  — testar interativo

Observações:

- As evidências são capturadas pelo script `tests/run_mandatory_tests.sh` em `tests/results/`.
- `FAIL` em comandos externos significa que o executor atual apenas executa builtins; implementar fork/exec e `find_executable` é necessário para mudar esse status.
- `PARTIAL` para pipes/redirs indica que lexer/parser já reconhecem os tokens (veja `print_pipeline`), mas a execução ainda não manipula descriptors e pipelines adequadamente.
