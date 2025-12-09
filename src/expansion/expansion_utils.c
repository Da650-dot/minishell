/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:04:27 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 18:03:03 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;
	char	*result;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		result = ft_strdup("?");
		return (result);
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
	{
		result = ft_strdup("$");
		return (result);
	}
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, &str[start], len + 1);
	return (name);
}

char	*append_str(char *result, char *to_append)
{
	char	*temp;

	if (!to_append)
		return (result);
	temp = ft_strjoin(result, to_append);
	free(result);
	return (temp);
}

static char	*get_var_value(char *var_name, t_data *data)
{
	char	*var_value;
	char	*result;

	if (ft_strncmp(var_name, "?", 2) == 0 && var_name[1] == '\0')
	{
		result = ft_itoa(data->exit_status);
		return (result);
	}
	var_value = get_env(var_name, data->envp);
	if (!var_value)
	{
		if (ft_strncmp(var_name, "$", 2) == 0 && var_name[1] == '\0')
		{
			result = ft_strdup("$");
			return (result);
		}
		result = ft_strdup("");
		return (result);
	}
	result = ft_strdup(var_value);
	return (result);
}

char	*process_dollar(char *str, int *i, t_data *data)
{
	char	*var_name;
	char	*result;

	(*i)++;
	if (!str[*i])
	{
		result = ft_strdup("$");
		return (result);
	}
	var_name = get_var_name(str, i);
	if (!var_name)
		return (NULL);
	result = get_var_value(var_name, data);
	free(var_name);
	return (result);
}
