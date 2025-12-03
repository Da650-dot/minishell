/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:53:59 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:54:00 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_cmd_args(t_cmd *cmd, t_data *data)
{
	char	**expanded;

	if (!cmd || !cmd->args)
		return ;
	expanded = expand_args(cmd->args, data);
	if (expanded)
	{
		free_array(cmd->args);
		cmd->args = expanded;
	}
}

void	expand_all_pipeline_args(t_pipeline *pipeline, t_data *data)
{
	t_pipeline	*current;

	current = pipeline;
	while (current)
	{
		expand_cmd_args(current->cmd, data);
		current = current->next;
	}
}
