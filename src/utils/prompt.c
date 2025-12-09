/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:31:46 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/09 18:02:58 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*simplify_home_path(char *cwd, char *home)
{
	char	*simplified;
	int		home_len;

	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0)
	{
		if (cwd[home_len] == '\0')
			simplified = ft_strdup("~");
		else
			simplified = ft_strjoin("~", &cwd[home_len]);
		free(cwd);
		return (simplified);
	}
	return (cwd);
}

static char	*get_current_dir(char **envp)
{
	char	*cwd;
	char	*home;
	char	*result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		result = ft_strdup("?");
		return (result);
	}
	home = get_env("HOME", envp);
	if (!home)
		return (cwd);
	return (simplify_home_path(cwd, home));
}

static char	*append_to_prompt(char *prompt, char *to_append)
{
	char	*temp;

	temp = ft_strjoin(prompt, to_append);
	free(prompt);
	return (temp);
}

char	*build_prompt(t_data *data)
{
	char	*prompt;
	char	*cwd;

	(void)data;
	cwd = get_current_dir(data->envp);
	prompt = ft_strdup(cwd);
	free(cwd);
	if (prompt[0] == '\0')
		return (prompt);
	prompt = append_to_prompt(prompt, "$ ");
	return (prompt);
}

char	*build_simple_prompt(void)
{
	char	*result;

	result = ft_strdup("minishell> ");
	return (result);
}
