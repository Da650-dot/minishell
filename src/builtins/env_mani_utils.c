/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_mani_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:12 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 18:03:00 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**add_new_env_var(char **envp, char *new_var)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (envp);
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[count] = ft_strdup(new_var);
	new_envp[count + 1] = NULL;
	free(envp);
	return (new_envp);
}

char	**add_env_var(char **envp, char *new_var)
{
	char	*key;
	char	*equals;
	int		index;
	char	**result;

	equals = ft_strchr(new_var, '=');
	if (equals)
		key = ft_substr(new_var, 0, equals - new_var);
	else
		key = ft_strdup(new_var);
	if (!key)
		return (envp);
	index = find_env_index(envp, key);
	free(key);
	if (index != -1)
	{
		free(envp[index]);
		envp[index] = ft_strdup(new_var);
		return (envp);
	}
	result = add_new_env_var(envp, new_var);
	return (result);
}
