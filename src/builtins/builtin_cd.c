/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:50:15 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:50:35 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_target_dir(char **args, t_data *data)
{
	char	*target;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		target = get_env("HOME", data->envp);
		if (!target)
		{
			print_error("cd", NULL, "HOME not set");
			return (NULL);
		}
		return (target);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		target = get_env("OLDPWD", data->envp);
		if (!target)
		{
			print_error("cd", NULL, "OLDPWD not set");
			return (NULL);
		}
		ft_putstr_fd(target, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (target);
	}
	return (args[1]);
}

static bool	update_pwd_vars(t_data *data, char *old_pwd)
{
	char	*new_pwd;
	char	*temp;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (false);
	temp = ft_strjoin("OLDPWD=", old_pwd);
	if (!temp)
	{
		free(new_pwd);
		return (false);
	}
	data->envp = add_env_var(data->envp, temp);
	free(temp);
	temp = ft_strjoin("PWD=", new_pwd);
	if (!temp)
	{
		free(new_pwd);
		return (false);
	}
	data->envp = add_env_var(data->envp, temp);
	free(temp);
	free(new_pwd);
	return (true);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*target;
	char	*old_pwd;

	if (args[1] && args[2])
	{
		print_error("cd", NULL, "too many arguments");
		return (ERROR);
	}
	target = get_target_dir(args, data);
	if (!target)
		return (ERROR);
	old_pwd = getcwd(NULL, 0);
	if (chdir(target) == -1)
	{
		print_error("cd", target, "No such file or directory");
		free(old_pwd);
		return (ERROR);
	}
	if (!update_pwd_vars(data, old_pwd))
	{
		free(old_pwd);
		return (ERROR);
	}
	free(old_pwd);
	return (SUCCESS);
}
