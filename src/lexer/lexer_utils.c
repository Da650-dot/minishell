#include "minishell.h"

void	test_lexer(void)
{
	char	*input;
	t_token	*tokens;

	printf("\n========== TESTE LEXER ==========\n");
	
	// Teste 1: Comando simples
	input = "ls -la";
	printf("Input: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	
	// Teste 2: Com redirecionamentos
	input = "cat < input.txt > output.txt";
	printf("\nInput: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	
	// Teste 3: Com pipes
	input = "ls | grep test | wc -l";
	printf("\nInput: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	
	// Teste 4: Com heredoc e append
	input = "cat << EOF >> file.txt";
	printf("\nInput: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	
	printf("==================================\n\n");
}
