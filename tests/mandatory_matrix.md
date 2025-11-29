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
