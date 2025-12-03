/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_minimal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:37 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:51:38 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_pipeline_vars(t_pipeline *pipeline, t_data *data)
{
	t_pipeline	*current;
	char		**expanded;

	current = pipeline;
	while (current)
	{
		if (current->cmd->args)
		{
			expanded = expand_args(current->cmd->args, data);
			if (expanded)
			{
				free_array(current->cmd->args);
				current->cmd->args = expanded;
			}
		}
		current = current->next;
	}
}

static void	execute_simple_builtin(t_pipeline *pipeline, t_data *data)
{
	t_cmd	*cmd;

	if (!pipeline || !pipeline->cmd)
		return;
	cmd = pipeline->cmd;
	if (!cmd->args || !cmd->args[0])
	{
		data->exit_status = 0;
		return;
	}
	execute_pipeline(pipeline, data);
}

void	process_pipeline(t_pipeline *pipeline, t_data *data)
{
	expand_pipeline_vars(pipeline, data);
	print_pipeline(pipeline);
	execute_simple_builtin(pipeline, data);
}
