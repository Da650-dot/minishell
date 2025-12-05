/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 06:30:51 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/05 06:31:08 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child_builtin(t_cmd *cmd, t_data *data)
{
	int	ret;

	if (apply_redirections(cmd) == -1)
		_exit(1);
	if (!cmd->args || !cmd->args[0])
		_exit(0);
	ret = execute_builtin(cmd->args, data);
	_exit(ret);
}

void	execute_child_external(t_cmd *cmd, t_data *data)
{
	char	*path;

	if (apply_redirections(cmd) == -1)
		_exit(1);
	if (!cmd->args || !cmd->args[0])
		_exit(0);
	path = resolve_command_path(cmd->args[0], data);
	if (!path)
	{
		print_error(cmd->args[0], NULL, "command not found");
		_exit(127);
	}
	execve(path, cmd->args, data->envp);
	free(path);
	handle_exec_error(cmd->args[0]);
}

void	handle_exec_error(char *cmd_name)
{
	if (errno == EACCES)
	{
		print_error(cmd_name, NULL, "Permission denied");
		_exit(126);
	}
	print_error(cmd_name, NULL, strerror(errno));
	_exit(127);
}
