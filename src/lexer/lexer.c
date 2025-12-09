/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:13:00 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 18:03:05 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

void	skip_spaces(char *input, int *i)
{
	while (input[*i] && (input[*i] == ' ' || input[*i] == '\t'))
		(*i)++;
}

t_token	*process_pipe(int *i)
{
	t_token	*new;

	new = new_token(TOKEN_PIPE, NULL);
	if (new)
		(*i)++;
	return (new);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	t_token	*new;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		if (input[i] == '|')
			new = process_pipe(&i);
		else if (input[i] == '<' || input[i] == '>')
			new = handle_redirect(input, &i);
		else
			new = process_word(input, &i);
		if (!new || !add_token_safe(&tokens, new))
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
