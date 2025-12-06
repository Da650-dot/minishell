/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:26 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/06 19:17:05 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_command_path(char *cmd, t_data *data)
{
	if (!cmd || !cmd[0])
		return (NULL);
	return (find_executable(cmd, data->envp));
}

int	execute_external(t_cmd *cmd, t_data *data)
{
	t_exec_args	exec_args;
	char		*path;
	int			exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (127);
	path = resolve_command_path(cmd->args[0], data);
	if (!path)
	{
		print_error(cmd->args[0], NULL, "command not found");
		return (127);
	}
	exec_args.path = path;
	exec_args.args = cmd->args;
	exec_args.envp = data->envp;
	exec_args.cmd = cmd;
	exec_args.data = data;
	exit_code = spawn_and_exec(&exec_args);
	free(path);
	return (exit_code);
}
