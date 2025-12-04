/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:27:06 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/04 11:27:09 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_redirect_left(char *input, int *i)
{
	t_token_type	type;

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
	return (new_token(type, NULL));
}

static t_token	*handle_redirect_right(char *input, int *i)
{
	t_token_type	type;

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
	return (new_token(type, NULL));
}

t_token	*handle_redirect(char *input, int *i)
{
	if (input[*i] == '<')
		return (handle_redirect_left(input, i));
	else
		return (handle_redirect_right(input, i));
}

bool	add_token_safe(t_token **tokens, t_token *new)
{
	if (!new)
		return (false);
	add_token(tokens, new);
	return (true);
}
