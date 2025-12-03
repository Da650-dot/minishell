/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:11:49 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 17:11:50 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_append_char(char *str, char c)
{
	char	*new_str;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	return (new_str);
}

char	*handle_quotes(char *input, int *i, char quote_char)
{
	int		start;
	int		len;
	char	*result;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] != quote_char)
		return (print_error("syntax error", "unclosed quotes", NULL), NULL);
	(*i)++;
	len = *i - start;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, &input[start], len + 1);
	return (result);
}

static char	*handle_word_char(char *input, int *i, char *word)
{
	char	*temp;
	char	*quoted;

	if (input[*i] == '\'' || input[*i] == '\"')
	{
		quoted = handle_quotes(input, i, input[*i]);
		if (!quoted)
			return (free(word), NULL);
		temp = word;
		word = ft_strjoin(word, quoted);
		free(temp);
		free(quoted);
	}
	else
	{
		temp = word;
		word = ft_append_char(word, input[*i]);
		free(temp);
		(*i)++;
	}
	return (word);
}

char	*extract_word(char *input, int *i)
{
	char	*word;

	word = ft_strdup("");
	while (input[*i] && !ft_strchr(" |<>", input[*i]))
	{
		word = handle_word_char(input, i, word);
		if (!word)
			return (NULL);
	}
	return (word);
}

t_token	*process_word(char *input, int *i)
{
	t_token	*new;
	char	*word;

	word = extract_word(input, i);
	if (!word)
		return (NULL);
	if (word[0] == '\0')
	{
		free(word);
		return (NULL);
	}
	new = new_token(TOKEN_WORD, word);
	if (!new)
		free(word);
	return (new);
}
