/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:30:47 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/11 14:43:47 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_operator(t_token *token)
{
	bool	result;

	if (!token)
		return (false);
	result = (token->type == TOKEN_PIPE);
	return (result);
}

bool	is_redirect(t_token *token)
{
	bool	result;

	if (!token)
		return (false);
	result = (token->type == TOKEN_REDIR_IN
			|| token->type == TOKEN_REDIR_OUT
			|| token->type == TOKEN_APPEND
			|| token->type == TOKEN_HEREDOC);
	return (result);
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
