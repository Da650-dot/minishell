/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:13:07 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/04 11:27:51 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_lexer(void)
{
	char	*input;
	t_token	*tokens;

	printf("\n========== TESTE LEXER ==========\n");
	input = "ls -la";
	printf("Input: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	input = "cat < input.txt > output.txt";
	printf("\nInput: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	input = "ls | grep test | wc -l";
	printf("\nInput: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	input = "cat << EOF >> file.txt";
	printf("\nInput: '%s'\n", input);
	tokens = tokenize(input);
	print_tokens(tokens);
	free_tokens(tokens);
	printf("==================================\n\n");
}
