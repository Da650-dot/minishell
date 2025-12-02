Manual tests helper

Arquivos criados:
- `tests/manual_tests.md` — descrição dos casos de teste e comandos (explicados e prontos para copiar/colar).
- `tests/manual_run_all.sh` — script não-interativo que executa uma seleção dos testes e salva saídas em `tests/manual_results/`.

Como usar:
1. Assegure que o binário `minishell` esteja no diretório do repositório raiz (ex.: `./minishell`).
2. Torne o runner executável (se ainda não estiver):

```sh
chmod +x tests/manual_run_all.sh
```

3. Execute o runner (gera `tests/manual_results/*.out`):

```sh
./tests/manual_run_all.sh
```

4. Inspecione os arquivos em `tests/manual_results/` para comparar a saída atual com o comportamento esperado presente em `tests/manual_tests.md`.

Testes interativos:
- Para sinais (Ctrl-C, Ctrl-D) e interação com o prompt, abra `./minishell` manualmente e siga as instruções no `tests/manual_tests.md`.

Observações:
- O runner usa `printf ... | ./minishell`, o que é suficiente para a maioria dos casos não-interativos (incluindo heredoc simples). Testes que requerem entrada direta do terminal (por exemplo: comportamento do readline com histórico) devem ser feitos manualmente.
- Os arquivos temporários usados ficam em `/tmp/` e podem ser limpos entre execuções.
