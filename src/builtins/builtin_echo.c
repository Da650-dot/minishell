#include "minishell.h"

static bool	check_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static int	count_flags(char **args)
{
	int	i;

	i = 1;
	while (args[i] && check_n_flag(args[i]))
		i++;
	return (i);
}

int	builtin_echo(char **args)
{
	int		i;
	bool	newline;

	if (!args)
		return (ERROR);
	newline = true;
	i = count_flags(args);
	if (i > 1)
		newline = false;
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (SUCCESS);
}
