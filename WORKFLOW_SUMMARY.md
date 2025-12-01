# Minishell — Resumo de estado e ponto de retomada

Este ficheiro foi gerado para que possas copiar/colar ou enviar ao assistente no futuro como "estado para retomar". Contém os pontos essenciais do estado atual do projeto, como testar rapidamente e os próximos passos recomendados.

Resumo rápido
- Local do repositório (workspace): Minishell_github
- Build: `make` gera `./minishell` (PASS)
- Test runner: `tests/run_mandatory_tests.sh` produz logs em `tests/results/`
- Logs mais relevantes: `tests/results/*.out` (ex.: `pipes_simple.out`, `redirections.out`, `quotes.out`)

O que foi implementado
- Execução externa (absolute path e PATH lookup) — `src/executor/exec_helpers.c`
- Pipeline execution (fork/pipe/dup2/wait) — `src/executor/pipeline.c`
- Builtins implementados (echo, cd, pwd, env, export, unset, exit)
- Test runner and automated non-interactive test cases (`tests/run_mandatory_tests.sh`)

Problemas pendentes (alta prioridade)
1. Redirecionamentos: parser já reconhece tokens, mas o executor ainda não abre/dup2/redirect antes do exec (ver `tests/results/redirections.out`).
2. Parsing de aspas: as aspas ainda aparecem em `args` (ex.: `"hello"`), causar contagens/execs erradas; atualmente deixado adiado por ti (ver `tests/results/quotes.out`).

Outras tarefas/observações
- Validar sinais interativos (Ctrl-C, Ctrl-\, Ctrl-D) e histórico (readline) manualmente.
- O runner agora faz algumas asserções simples (SUB/NUM/PAT). Para CI mais estrito, adicionar checagens de exit code e resumo final.

Como reproduzir rapidamente
1. Build:
```bash
make
```
2. Executar a suíte de testes (não interativo):
```bash
./tests/run_mandatory_tests.sh
ls tests/results/*.out
```
3. Ver um log específico:
```bash
less tests/results/pipes_simple.out
```

Ponto de retomada para o assistente (quando quiseres que eu continue):
- Envia-me este ficheiro (`WORKFLOW_SUMMARY.md`) e eu retomarei a partir daqui. Inclui também o branch em que estás e quaisquer alterações locais não commitadas, se houver.

Próximos passos recomendados (ordem sugerida)
1. Implementar redirecionamentos no executor (open + dup2 no child) e adicionar testes automáticos cobrindo `>`, `<`, `>>` e heredoc.
2. Corrigir parsing de aspas (lexer + expander) para que as aspas não sejam passadas aos programs e as expansões ocorram corretamente.
3. Executar testes interativos e ajustar handlers de sinal se necessário.
4. Melhorar runner para gerar um resumo PASS/FAIL ao fim.

Se quiseres que eu comece pela etapa 1 (redirecionamentos), diz "começa redirs" e eu aplico as mudanças e corro os testes.
