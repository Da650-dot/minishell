/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:21 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 16:18:22 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	find_env_index(char **envp, char *key)
{
	int	i;
	int	key_len;

	if (!envp || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0
			&& (envp[i][key_len] == '=' || envp[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static char	**create_new_envp(char **envp, int count, int index)
{
	char	**new_envp;
	int		i;
	int		j;

	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (envp);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (i != index)
		{
			new_envp[j] = envp[i];
			j++;
		}
		else
			free(envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

char	**remove_env_var(char **envp, char *key)
{
	char	**new_envp;
	int		count;
	int		index;

	index = find_env_index(envp, key);
	if (index == -1)
		return (envp);
	count = 0;
	while (envp[count])
		count++;
	new_envp = create_new_envp(envp, count, index);
	free(envp);
	return (new_envp);
}
