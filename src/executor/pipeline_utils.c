/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:52:20 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/05 06:31:30 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_fds(t_exec_ctx *ctx)
{
	int	i;

	if (ctx->in_fd != STDIN_FILENO)
		dup2(ctx->in_fd, STDIN_FILENO);
	if (ctx->out_fd != STDOUT_FILENO)
		dup2(ctx->out_fd, STDOUT_FILENO);
	i = 0;
	while (i < ctx->pipefds_len)
	{
		if (ctx->pipefds && ctx->pipefds[i] > 2)
			close(ctx->pipefds[i]);
		i++;
	}
}

void	child_exec_cmd(t_cmd *cmd, t_data *data, t_exec_ctx *ctx)
{
	setup_signals_default();
	setup_child_fds(ctx);
	if (is_builtin(cmd->args[0]))
		execute_child_builtin(cmd, data);
	else
		execute_child_external(cmd, data);
}
