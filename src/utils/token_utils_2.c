/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:29:52 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/11 15:13:19 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	validate_operator_rules(t_token *current, t_token *tokens)
{
	if (current == tokens && is_operator(current))
	{
		print_error("syntax error", "unexpected token", "|");
		return (false);
	}
	if (is_operator(current) && !current->next)
	{
		print_error("syntax error", "unexpected end of input after pipe", NULL);
		return (false);
	}
	if (is_operator(current) && current->next && is_operator(current->next))
	{
		print_error("syntax error", "unexpected token", "|");
		return (false);
	}
	return (true);
}

static bool	validate_redirect_rules(t_token *current)
{
	if (is_redirect(current)
		&& (!current->next || current->next->type != TOKEN_WORD))
	{
		print_error("syntax error", "expected filename after redirect", NULL);
		return (false);
	}
	return (true);
}

static bool	validate_token_rules(t_token *current, t_token *tokens)
{
	if (!validate_operator_rules(current, tokens))
		return (false);
	if (!validate_redirect_rules(current))
		return (false);
	return (true);
}

bool	validate_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (!validate_token_rules(current, tokens))
			return (false);
		current = current->next;
	}
	return (true);
}
