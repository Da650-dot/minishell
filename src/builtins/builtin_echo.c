#include "minishell.h"

static bool	is_only_chars(char *s, const char *chars)
{
	int i;

	if (!s)
		return (false);
	i = 1; /* skip leading '-' */
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

/* parse flags -n and -e (supports combined like -ne or -en)
 * returns the index of the first non-flag argument and sets flags via pointers
 */
static int	parse_flags(char **args, bool *newline, bool *escape)
{
	int i;

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

/* simple escape processing for \n, \t, \\ and \r */
static void	print_with_escapes(const char *s)
{
	const char *p = s;

	while (*p)
	{
		if (*p == '\\' && *(p + 1))
		{
			p++;
			if (*p == 'n')
				ft_putchar_fd('\n', STDOUT_FILENO);
			else if (*p == 't')
				ft_putchar_fd('\t', STDOUT_FILENO);
			else if (*p == 'r')
				ft_putchar_fd('\r', STDOUT_FILENO);
			else if (*p == '\\')
				ft_putchar_fd('\\', STDOUT_FILENO);
			else
			{
				ft_putchar_fd('\\', STDOUT_FILENO);
				ft_putchar_fd(*p, STDOUT_FILENO);
			}
			p++;
		}
		else
		{
			ft_putchar_fd(*p, STDOUT_FILENO);
			p++;
		}
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
	if (i > 1)
		newline = false; /* redundant but preserves previous logic */
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
