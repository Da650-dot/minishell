/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:30:47 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 17:30:48 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == TOKEN_PIPE);
}

bool	is_redirect(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND
		|| token->type == TOKEN_HEREDOC);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = temp;
	}
}

void	print_token_type(t_token_type type)
{
	if (type == TOKEN_WORD)
		printf("WORD");
	else if (type == TOKEN_PIPE)
		printf("PIPE");
	else if (type == TOKEN_REDIR_IN)
		printf("REDIR_IN");
	else if (type == TOKEN_REDIR_OUT)
		printf("REDIR_OUT");
	else if (type == TOKEN_APPEND)
		printf("APPEND");
	else if (type == TOKEN_HEREDOC)
		printf("HEREDOC");
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("[");
		print_token_type(current->type);
		if (current->value)
			printf(":\"%s\"", current->value);
		printf("]");
		if (current->next)
			printf(" -> ");
		current = current->next;
	}
	printf("\n");
}
