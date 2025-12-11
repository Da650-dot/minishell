/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permissions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:26:07 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/11 15:13:20 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

bool	check_execute_permission(char *path)
{
	struct stat	statbuf;

	if (!path)
		return (false);
	if (stat(path, &statbuf) == -1)
		return (false);
	if (!S_ISREG(statbuf.st_mode))
		return (false);
	if (access(path, X_OK) == 0)
		return (true);
	return (false);
}

static char	*search_in_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (check_execute_permission(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (check_execute_permission(cmd))
		{
			result = ft_strdup(cmd);
			return (result);
		}
		return (NULL);
	}
	path_env = get_env("PATH", envp);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_path(cmd, paths);
	free_array(paths);
	return (result);
}
