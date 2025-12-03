/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:54:29 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:55:36 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_quote_state(char c, bool in_double, bool *in_single, bool *skip)
{
	if (c == '\'' && !in_double)
	{
		*in_single = !(*in_single);
		*skip = true;
	}
}

static void	copy_without_quotes(char *str, char *result)
{
	int		i;
	int		j;
	bool	in_single;
	bool	in_double;
	bool	skip;

	i = 0;
	j = 0;
	in_single = false;
	in_double = false;
	while (str[i])
	{
		skip = false;
		update_quote_state(str[i], in_double, &in_single, &skip);
		if (str[i] == '"' && !in_single)
		{
			in_double = !in_double;
			skip = true;
		}
		if (!skip)
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
}

char	*remove_quotes(char *str)
{
	char	*result;
	char	*final;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	copy_without_quotes(str, result);
	final = ft_strdup(result);
	free(result);
	return (final);
}
