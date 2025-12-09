/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:27:06 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/09 18:03:02 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_redirect_left(char *input, int *i)
{
	t_token_type	type;
	t_token			*token;

	if (input[*i + 1] == '<')
	{
		type = TOKEN_HEREDOC;
		*i += 2;
	}
	else
	{
		type = TOKEN_REDIR_IN;
		(*i)++;
	}
	token = new_token(type, NULL);
	return (token);
}

static t_token	*handle_redirect_right(char *input, int *i)
{
	t_token_type	type;
	t_token			*token;

	if (input[*i + 1] == '>')
	{
		type = TOKEN_APPEND;
		*i += 2;
	}
	else
	{
		type = TOKEN_REDIR_OUT;
		(*i)++;
	}
	token = new_token(type, NULL);
	return (token);
}

t_token	*handle_redirect(char *input, int *i)
{
	t_token	*result;

	if (input[*i] == '<')
		result = handle_redirect_left(input, i);
	else
		result = handle_redirect_right(input, i);
	return (result);
}

bool	add_token_safe(t_token **tokens, t_token *new)
{
	if (!new)
		return (false);
	add_token(tokens, new);
	return (true);
}
