/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:50:46 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/10 13:57:43 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **args, t_data *data)
{
	int	i;

	if (!data || !data->envp)
		return (ERROR);
	if (args[1])
	{
		print_error("env", NULL, "too many arguments");
		return (ERROR);
	}
	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
		{
			ft_putstr_fd(data->envp[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		i++;
	}
	return (SUCCESS);
}
