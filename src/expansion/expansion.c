/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:10:10 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/04 11:26:44 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quotes(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '\"' && !(*in_single))
		*in_double = !(*in_double);
}

static char	*handle_expansion(char *str, int *i, t_data *data, bool in_single)
{
	char	*temp;
	char	c[2];

	if (str[*i] == '$' && str[*i + 1] && !in_single)
	{
		temp = process_dollar(str, i, data);
		return (temp);
	}
	else
	{
		c[0] = str[*i];
		c[1] = '\0';
		(*i)++;
		return (ft_strdup(c));
	}
}

char	*expand_variables_raw(char *str, t_data *data)
{
	char	*result;
	char	*temp;
	int		i;
	bool	in_single_quotes;
	bool	in_double_quotes;

	result = ft_strdup("");
	i = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (str[i])
	{
		update_quotes(str[i], &in_single_quotes, &in_double_quotes);
		temp = handle_expansion(str, &i, data, in_single_quotes);
		result = append_str(result, temp);
		free(temp);
	}
	return (result);
}

char	**expand_args(char **args, t_data *data)
{
	char	**expanded;
	int		i;
	int		count;

	if (!args)
		return (NULL);
	count = 0;
	while (args[count])
		count++;
	expanded = malloc(sizeof(char *) * (count + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < count)
	{
		expanded[i] = expand_variables(args[i], data);
		if (!expanded[i])
		{
			free_array(expanded);
			return (NULL);
		}
		i++;
	}
	expanded[count] = NULL;
	return (expanded);
}

char	*expand_variables(char *str, t_data *data)
{
	char	*expanded;
	char	*final;

	expanded = expand_variables_raw(str, data);
	final = remove_quotes(expanded);
	free(expanded);
	return (final);
}
