#include "minishell.h"

static bool	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static int	get_exit_code(char *arg)
{
	long	num;
	int		sign;
	int		i;

	num = 0;
	sign = 1;
	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	while (arg[i])
	{
		num = num * 10 + (arg[i] - '0');
		i++;
	}
	return ((int)((sign * num) % 256));
}

int	builtin_exit(char **args, t_data *data)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args[1])
	{
		cleanup_data(data);
		exit(data->exit_status);
	}
	if (!is_numeric(args[1]))
	{
		print_error("exit", args[1], "numeric argument required");
		cleanup_data(data);
		exit(2);
	}
	if (args[2])
	{
		print_error("exit", NULL, "too many arguments");
		return (ERROR);
	}
	exit_code = get_exit_code(args[1]);
	cleanup_data(data);
	exit(exit_code);
}