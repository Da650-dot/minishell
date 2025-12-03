/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:04 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:51:05 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	unset_single_var(char *arg, t_data *data)
{
	if (!is_valid_identifier(arg))
	{
		print_error("unset", arg, "not a valid identifier");
		return (false);
	}
	data->envp = remove_env_var(data->envp, arg);
	return (true);
}

int	builtin_unset(char **args, t_data *data)
{
	int		i;
	bool	has_error;

	if (!args[1])
		return (SUCCESS);
	has_error = false;
	i = 1;
	while (args[i])
	{
		if (!unset_single_var(args[i], data))
			has_error = true;
		i++;
	}
	if (has_error)
		return (ERROR);
	return (SUCCESS);
}
