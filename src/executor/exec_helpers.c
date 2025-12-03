/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:33 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:51:34 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	map_exec_errno(int err)
{
	if (err == EACCES)
		return (126);
	return (127);
}

static int	execute_child_process(char *path, char **args,
			char **envp, t_cmd *cmd)
{
	setup_signals_default();
	if (cmd && apply_redirections(cmd) == -1)
		_exit(1);
	execve(path, args, envp);
	if (errno == EACCES)
		print_error(args[0], NULL, "Permission denied");
	else
		print_error(args[0], NULL, strerror(errno));
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
		return (128 + WTERMSIG(status));
	return (1);
}

int	spawn_and_exec(char *path, char **args, char **envp,
		t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	(void)data;
	pid = fork();
	if (pid == -1)
	{
		print_error("fork", NULL, strerror(errno));
		return (1);
	}
	if (pid == 0)
		execute_child_process(path, args, envp, cmd);
	return (handle_parent_process(pid));
}
