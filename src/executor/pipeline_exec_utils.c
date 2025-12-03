/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:52:05 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:52:06 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmds(t_pipeline *pl)
{
	int	n;

	n = 0;
	while (pl)
	{
		n++;
		pl = pl->next;
	}
	return (n);
}

void	close_fds(int *fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (fds[i] > 2)
			close(fds[i]);
		i++;
	}
}

static void	cleanup_pipeline_heredocs(t_pipeline *pipeline)
{
	t_pipeline	*c;

	c = pipeline;
	while (c)
	{
		if (c->cmd && c->cmd->heredoc_fd >= 0)
		{
			close(c->cmd->heredoc_fd);
			c->cmd->heredoc_fd = -1;
		}
		c = c->next;
	}
}

static int	process_cmd_heredocs(t_cmd *cmd, t_data *data)
{
	t_redir	*redir;

	if (cmd->heredoc_delim)
	{
		if (prepare_heredoc(cmd, data) == -1)
			return (-1);
	}
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (prepare_heredoc(cmd, data) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

void	prepare_pipeline_heredocs(t_pipeline *pipeline, t_data *data)
{
	t_pipeline	*it;

	it = pipeline;
	while (it)
	{
		if (it->cmd && process_cmd_heredocs(it->cmd, data) == -1)
		{
			cleanup_pipeline_heredocs(pipeline);
			return ;
		}
		it = it->next;
	}
}
