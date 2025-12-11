/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:26 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/11 16:37:26 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resolve_command_path(char *cmd, t_data *data)
{
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	result = find_executable(cmd, data->envp);
	return (result);
}

static void	print_command_error(char *cmd_name)
{
	struct stat	statbuf;

	if (stat(cmd_name, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			print_error(cmd_name, NULL, "Is a directory");
			return ;
		}
	}
	print_error(cmd_name, NULL, "command not found");
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
		print_command_error(cmd->args[0]);
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
