/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:09 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 18:02:55 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strncmp(cmd, "echo", 5) == 0 && cmd[4] == '\0')
		return (true);
	if (ft_strncmp(cmd, "cd", 3) == 0 && cmd[2] == '\0')
		return (true);
	if (ft_strncmp(cmd, "pwd", 4) == 0 && cmd[3] == '\0')
		return (true);
	if (ft_strncmp(cmd, "export", 7) == 0 && cmd[6] == '\0')
		return (true);
	if (ft_strncmp(cmd, "unset", 6) == 0 && cmd[5] == '\0')
		return (true);
	if (ft_strncmp(cmd, "env", 4) == 0 && cmd[3] == '\0')
		return (true);
	if (ft_strncmp(cmd, "exit", 5) == 0 && cmd[4] == '\0')
		return (true);
	return (false);
}

static int	execute_builtin_part1(char **args, t_data *data)
{
	int	result;

	if (ft_strncmp(args[0], "echo", 5) == 0 && args[0][4] == '\0')
	{
		result = builtin_echo(args);
		return (result);
	}
	if (ft_strncmp(args[0], "cd", 3) == 0 && args[0][2] == '\0')
	{
		result = builtin_cd(args, data);
		return (result);
	}
	if (ft_strncmp(args[0], "pwd", 4) == 0 && args[0][3] == '\0')
	{
		result = builtin_pwd();
		return (result);
	}
	return (ERROR);
}

static int	execute_builtin_part2(char **args, t_data *data)
{
	int	result;

	if (ft_strncmp(args[0], "export", 7) == 0 && args[0][6] == '\0')
	{
		result = builtin_export(args, data);
		return (result);
	}
	if (ft_strncmp(args[0], "unset", 6) == 0 && args[0][5] == '\0')
	{
		result = builtin_unset(args, data);
		return (result);
	}
	if (ft_strncmp(args[0], "env", 4) == 0 && args[0][3] == '\0')
	{
		result = builtin_env(data->envp);
		return (result);
	}
	if (ft_strncmp(args[0], "exit", 5) == 0 && args[0][4] == '\0')
	{
		result = builtin_exit(args, data);
		return (result);
	}
	return (ERROR);
}

int	execute_builtin(char **args, t_data *data)
{
	int	result;

	if (!args || !args[0])
		return (ERROR);
	result = execute_builtin_part1(args, data);
	if (result != ERROR)
		return (result);
	result = execute_builtin_part2(args, data);
	return (result);
}
