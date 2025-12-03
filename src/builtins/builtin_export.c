/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:50:57 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:50:58 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_export_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

static bool	export_single_var(char *arg, t_data *data)
{
	char	*equals;

	if (!is_valid_identifier(arg))
	{
		print_error("export", arg, "not a valid identifier");
		return (false);
	}
	equals = ft_strchr(arg, '=');
	if (!equals)
		return (true);
	data->envp = add_env_var(data->envp, arg);
	return (true);
}

int	builtin_export(char **args, t_data *data)
{
	int		i;
	bool	has_error;

	if (!args[1])
	{
		print_export_env(data->envp);
		return (SUCCESS);
	}
	has_error = false;
	i = 1;
	while (args[i])
	{
		if (!export_single_var(args[i], data))
			has_error = true;
		i++;
	}
	if (has_error)
		return (ERROR);
	return (SUCCESS);
}
