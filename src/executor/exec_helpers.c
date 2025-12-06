/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:33 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/06 19:16:46 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	map_exec_errno(int err)
{
	if (err == EACCES)
		return (126);
	return (127);
}

static int	execute_child_process(t_exec_args *args)
{
	setup_signals_default();
	if (args->cmd && apply_redirections(args->cmd, args->data) == -1)
		_exit(1);
	execve(args->path, args->args, args->envp);
	if (errno == EACCES)
		print_error(args->args[0], NULL, "Permission denied");
	else
		print_error(args->args[0], NULL, strerror(errno));
	_exit(map_exec_errno(errno));
}

static int	handle_parent_process(pid_t pid)
{
	int	status;

	setup_signals_execution();
	if (waitpid(pid, &status, 0) == -1)
	{
		print_error("waitpid", NULL, strerror(errno));
		setup_signals_interactive();
		return (1);
	}
	setup_signals_interactive();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static int	do_spawn_and_exec(t_exec_args *exec_args)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		print_error("fork", NULL, strerror(errno));
		return (1);
	}
	if (pid == 0)
		execute_child_process(exec_args);
	return (handle_parent_process(pid));
}

int	spawn_and_exec(t_exec_args *args)
{
	return (do_spawn_and_exec(args));
}
