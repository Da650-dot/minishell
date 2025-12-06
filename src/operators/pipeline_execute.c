/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:52:10 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/06 17:15:37 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin_with_redirections(t_cmd *cmd, t_data *data)
{
	t_redirect_save	save;
	int				exit_status;

	if (save_stdio(&save) == -1)
	{
		print_error("dup", NULL, "failed to backup stdio");
		return (1);
	}
	if (apply_redirections(cmd, data) == -1)
	{
		restore_stdio(&save);
		return (1);
	}
	exit_status = execute_builtin(cmd->args, data);
	restore_stdio(&save);
	return (exit_status);
}

static bool	has_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return (false);
	if (cmd->heredoc_delim)
		return (true);
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_IN
			|| redir->type == TOKEN_REDIR_OUT
			|| redir->type == TOKEN_APPEND)
			return (true);
		redir = redir->next;
	}
	return (false);
}

void	handle_single_command(t_pipeline *pipeline, t_data *data)
{
	t_cmd	*cmd;

	cmd = pipeline->cmd;
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		data->exit_status = 0;
		return ;
	}
	if (is_builtin(cmd->args[0]))
	{
		if (has_redirections(cmd))
			data->exit_status
				= execute_builtin_with_redirections(cmd, data);
		else
			data->exit_status = execute_builtin(cmd->args, data);
	}
	else
		data->exit_status = execute_external(cmd, data);
}
