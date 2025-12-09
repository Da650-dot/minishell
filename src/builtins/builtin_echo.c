/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:50:21 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 14:34:18 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_only_chars(char *s, const char *chars)
{
	int	i;

	if (!s)
		return (false);
	i = 1;
	if (s[0] != '-')
		return (false);
	while (s[i])
	{
		if (!ft_strchr(chars, s[i]))
			return (false);
		i++;
	}
	return (true);
}

static int	parse_flags(char **args, bool *newline, bool *escape)
{
	int	i;

	*newline = true;
	*escape = false;
	i = 1;
	while (args[i] && is_only_chars(args[i], "ne"))
	{
		if (ft_strchr(args[i], 'n'))
			*newline = false;
		if (ft_strchr(args[i], 'e'))
			*escape = true;
		i++;
	}
	return (i);
}

static void	print_with_escapes(const char *s)
{
	while (*s)
	{
		if (*s == '\\' && *(s + 1))
		{
			s++;
			if (*s == 'n')
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (*s == 't')
				ft_putchar_fd('\t', STDOUT_FILENO);
			else if (*s == 'r')
				ft_putchar_fd('\r', STDOUT_FILENO);
			else if (*s == '\\')
				ft_putchar_fd('\\', STDOUT_FILENO);
			else
			{
				ft_putchar_fd('\\', STDOUT_FILENO);
				ft_putchar_fd(*s, STDOUT_FILENO);
			}
		}
		else
			ft_putchar_fd(*s, STDOUT_FILENO);
		s++;
	}
}

int	builtin_echo(char **args)
{
	int		i;
	bool	newline;
	bool	escape;

	if (!args)
		return (ERROR);
	i = parse_flags(args, &newline, &escape);
	while (args[i])
	{
		if (escape)
			print_with_escapes(args[i]);
		else
			ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
