/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:20:19 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 17:20:20 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_args(t_cmd *cmd)
{
	int	i;

	if (!cmd || !cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
}

static void	free_cmd_redirects(t_cmd *cmd)
{
	t_redir	*temp;

	while (cmd->redirects)
	{
		temp = cmd->redirects->next;
		free(cmd->redirects->file);
		free(cmd->redirects);
		cmd->redirects = temp;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_cmd_args(cmd);
	free_cmd_redirects(cmd);
	free(cmd->heredoc_delim);
	if (cmd->heredoc_fd >= 0)
		close(cmd->heredoc_fd);
	free(cmd);
}

void	free_pipeline(t_pipeline *pipeline)
{
	t_pipeline	*temp;

	while (pipeline)
	{
		temp = pipeline->next;
		free_cmd(pipeline->cmd);
		free(pipeline);
		pipeline = temp;
	}
}