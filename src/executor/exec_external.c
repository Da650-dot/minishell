/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:26 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/05 07:11:48 by jgiancol         ###   ########.fr       */
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
	char	*path;
	int		exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (127);
	path = resolve_command_path(cmd->args[0], data);
	if (!path)
	{
		print_error(cmd->args[0], NULL, "command not found");
		return (127);
	}
	exit_code = spawn_and_exec(path, cmd->args, data->envp, cmd);
	free(path);
	return (exit_code);
}
