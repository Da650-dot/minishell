/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:18:55 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 17:18:56 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_args_partial(t_cmd *cmd, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(cmd->args[j]);
		j++;
	}
	free(cmd->args);
	cmd->args = NULL;
}

static bool	process_token(t_cmd *cmd, t_token **current, int *i)
{
	if (is_redirect(*current))
	{
		if (!handle_redirect_token(cmd, current))
			return (false);
	}
	else if ((*current)->type == TOKEN_WORD && *i < cmd->word_count)
	{
		cmd->args[*i] = ft_strdup((*current)->value);
		if (!cmd->args[*i])
			return (false);
		(*i)++;
		*current = (*current)->next;
	}
	else
	{
		*current = (*current)->next;
	}
	return (true);
}

bool	fill_args(t_cmd *cmd, t_token *tokens, int word_count)
{
	t_token	*current;
	int		i;

	cmd->args = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->args)
		return (false);
	i = 0;
	while (i <= word_count)
		cmd->args[i++] = NULL;
	cmd->word_count = word_count;
	current = tokens;
	i = 0;
	while (current && current->type != TOKEN_PIPE)
	{
		if (!process_token(cmd, &current, &i))
			return (free_args_partial(cmd, i), false);
	}
	return (true);
}