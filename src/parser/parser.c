/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:25:28 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 17:25:29 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipeline	*parse_tokens(t_token *tokens)
{
	t_pipeline	*pipeline;
	t_pipeline	*node;
	t_token		*current;

	if (!tokens || !validate_tokens(tokens))
		return (NULL);
	pipeline = NULL;
	current = tokens;
	while (current)
	{
		node = parse_single_cmd(current);
		if (!node)
		{
			free_pipeline(pipeline);
			return (NULL);
		}
		add_pipeline_node(&pipeline, node);
		while (current && current->type != TOKEN_PIPE)
			current = current->next;
		if (current && current->type == TOKEN_PIPE)
			current = current->next;
	}
	return (pipeline);
}

static t_pipeline	*create_pipeline_node(t_cmd *cmd)
{
	t_pipeline	*node;

	node = new_pipeline_node(cmd);
	if (!node)
	{
		free_cmd(cmd);
		return (NULL);
	}
	return (node);
}

t_pipeline	*parse_single_cmd(t_token *tokens)
{
	t_cmd		*cmd;
	t_pipeline	*node;
	int			word_count;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	word_count = count_words_until_pipe(tokens);
	if (word_count == 0 && !has_redirects_in_tokens(tokens))
	{
		print_error("syntax error", "empty command", NULL);
		free(cmd);
		return (NULL);
	}
	if (!fill_args(cmd, tokens, word_count))
	{
		free_cmd(cmd);
		return (NULL);
	}
	node = create_pipeline_node(cmd);
	return (node);
}
