/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:52:17 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:52:18 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipes(int **pipefds, int n)
{
	int	i;
	int	pipefds_len;

	pipefds_len = 2 * (n - 1);
	*pipefds = malloc(sizeof(int) * pipefds_len);
	if (!*pipefds)
		return (0);
	i = 0;
	while (i < n - 1)
	{
		if (pipe(*pipefds + i * 2) == -1)
		{
			free(*pipefds);
			return (0);
		}
		i++;
	}
	return (pipefds_len);
}

static void	get_pipe_fds(int i, t_exec_ctx *ctx)
{
	if (i == 0)
		ctx->in_fd = STDIN_FILENO;
	else
		ctx->in_fd = ctx->pipefds[(i - 1) * 2];
	if (i == ctx->n - 1)
		ctx->out_fd = STDOUT_FILENO;
	else
		ctx->out_fd = ctx->pipefds[i * 2 + 1];
}

static int	fork_single_cmd(t_exec_ctx *ctx, int i, pid_t *pid)
{
	t_pipeline	*cur;
	int			j;

	cur = ctx->pipeline;
	j = 0;
	while (j < i && cur)
	{
		cur = cur->next;
		j++;
	}
	get_pipe_fds(i, ctx);
	*pid = fork();
	if (*pid == -1)
		return (0);
	if (*pid == 0)
		child_exec_cmd(cur->cmd, ctx->data, ctx);
	return (1);
}

static int	fork_pipeline(t_exec_ctx *ctx, pid_t **pids)
{
	int	i;

	*pids = malloc(sizeof(pid_t) * ctx->n);
	if (!*pids)
		return (0);
	i = 0;
	while (i < ctx->n)
	{
		if (!fork_single_cmd(ctx, i, &(*pids)[i]))
		{
			free(*pids);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	wait_all(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) != -1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

void	execute_multi_pipeline(t_pipeline *pipeline, t_data *data, int n)
{
	int			pipefds_len;
	pid_t		*pids;
	t_exec_ctx	ctx;

	ctx.data = data;
	ctx.pipeline = pipeline;
	ctx.n = n;
	pipefds_len = create_pipes(&ctx.pipefds, n);
	if (!pipefds_len)
	{
		data->exit_status = 1;
		return ;
	}
	ctx.pipefds_len = pipefds_len;
	if (!fork_pipeline(&ctx, &pids))
	{
		close_fds(ctx.pipefds, pipefds_len);
		free(ctx.pipefds);
		data->exit_status = 1;
		return ;
	}
	close_fds(ctx.pipefds, pipefds_len);
	data->exit_status = wait_all(pids, n);
	free(ctx.pipefds);
	free(pids);
}
