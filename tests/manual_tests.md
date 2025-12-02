# Testes manuais para Minishell (Mandatory)

Este documento lista casos de teste manuais e comandos prontos para copiar/colar. Use-os para checar comportamento básico e bordas do `minishell`.

Formato por caso:
- Descrição
- Comando(s) a executar no shell (ou forma não-interativa)
- Resultado esperado (resumo)

---

## 1) Builtins básicos

1.1 Export / echo / unset
Descrição: exportar variável e imprimir com e sem aspas.
Comandos:
- export ARG=hello
- echo $ARG
- echo "$ARG"
- echo '$ARG'
- unset ARG
- echo $ARG
Expected: `hello` nas duas primeiras, `'$ARG'` literal na terceira, e nada após `unset`.

1.2 pwd / cd
Comandos:
- pwd
- cd /
- pwd
- cd -
Expected: caminho correto, cd muda diretório, `cd -` retorna ao anterior.

1.3 exit
Comandos:
- exit 42
Expected: o shell termina com status 42.

---

## 2) Execução de binários (PATH)

2.1 Comando externo simples
Comando: ls -1
Expected: lista de arquivos do diretório atual (mesmo comportamento do /bin/ls).

2.2 Execução por caminho absoluto/relativo
Comando:
- /bin/echo OK
- ./minishell (não tentar rodar dentro do mesmo processo, testar com outro binário se disponível)
Expected: execução por caminho funciona.

---

## 3) Pipes

3.1 Pipe simples
Comando:
- printf "a\nb\nc\n" | grep b
Expected: `b`

3.2 Pipeline com múltiplos estágios
Comandos:
- printf "1 2 3 4\n" | tr ' ' '\n' | wc -l
Expected: `4`

---

## 4) Redirecionamentos (>, >>, <)

4.1 Sobrescrever (>)
Comandos:
- echo hello > /tmp/minishell_test_1
- cat /tmp/minishell_test_1
Expected: arquivo contém `hello`.

4.2 Append (>>)
Comandos:
- echo one > /tmp/minishell_test_2
- echo two >> /tmp/minishell_test_2
- cat /tmp/minishell_test_2
Expected: `one` seguido de `two` em linhas separadas.

4.3 Input (<)
Comandos:
- printf "infile line\n" > /tmp/minishell_in
- cat < /tmp/minishell_in
Expected: imprime `infile line`.

---

## 5) Heredoc (<<)

5.1 Heredoc simples (expande variável)
Comandos:
- export X=42
- cat <<EOF
$X
EOF
Expected: a expansão do `$X` (42) e saída `42`.

5.2 Heredoc com delimitador entre aspas (não expande)
Comandos:
- export Y=99
- cat <<'EOF'
$Y
EOF
Expected: imprime literal `$Y` (sem expansion).

5.3 Heredoc usado em pipeline
Comandos:
- cat <<EOF | wc -l
line1
line2
EOF
Expected: `2`

---

## 6) Expansão e aspas

6.1 Variáveis e aspas
Comandos:
- export V=abc
- echo $V
- echo "$V"
- echo '$V'
Expected: `abc`, `abc`, literal `$V`.

6.2 Exit status ($?)
Comandos:
- false
- echo $?   # imediatamente após
Expected: `1` (ou o status do último comando não-zero)

6.3 Empty / unset variables
Comandos:
- unset U
- echo -n "$U"
Expected: vazio (nenhuma saída)

---

## 7) Signals (manual / interativo)

7.1 Ctrl-C (SIGINT) durante prompt
Passos (interativo):
- Abra `./minishell`
- Aguarde o prompt e pressione Ctrl-C
Expected: interrompe a input atual e mostra novo prompt, não fecha o shell.

7.2 Ctrl-D (EOF)
Passos (interativo):
- Abra `./minishell`
- No prompt, pressione Ctrl-D
Expected: encerra o shell (comportamento esperado do readloop).

7.3 Ctrl-C durante heredoc (interativo)
Passos:
- Abra `./minishell`
- Execute `cat <<EOF` e no prompt do heredoc pressione Ctrl-C
Expected: heredoc interrompido e shell retorna a prompt com status apropriado.

---

## 8) Permissões / erros

8.1 Arquivo não existente (input)
Comando:
- cat < /no/such/file
Expected: mensagem de erro e código de saída não-zero.

8.2 Tentativa de redirecionar para diretório
Comando:
- echo hi > /
Expected: erro de permissão/Is a directory

---

## 9) Casos de borda

9.1 Comando vazio (apenas enter)
Comando: pressione Enter no prompt
Expected: nada executado, novo prompt.

9.2 Espaços em torno de redirecionadores
Comando:
- echo hi> /tmp/minishell_test_3
- cat /tmp/minishell_test_3
Expected: `hi` (suportar sem espaço)

9.3 Múltiplos redirecionadores sequenciais
Comando:
- echo 1 > /tmp/a > /tmp/b
- cat /tmp/a
- cat /tmp/b
Expected: último redirecionador prevalece (arquivo `/tmp/b` contém `1`).

---

## Notas
- Alguns testes (sinais, interativos) requerem execução manual no prompt.
- Para execução não-interativa, use os exemplos do script `tests/manual_run_all.sh`.
- Ajuste caminhos temporários (/tmp/...) se necessário na sua máquina.
