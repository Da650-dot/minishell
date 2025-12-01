#include "minishell.h"
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Scaffolding helpers for external command execution.
 * These stubs prepare function boundaries for implementing absolute-path
 * execution and PATH lookup + fork/exec. They do not change runtime
 * behavior of the current program until `execute_external` is wired into
 * the executor.
 */

char *resolve_command_path(char *cmd, t_data *data)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (!(path_env = get_env("PATH", data->envp)))
		return (NULL);
	if (!(paths = ft_split(path_env, ':')))
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			continue;
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (full_path && access(full_path, F_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_array(paths);
	return (NULL);
}

int spawn_and_exec(char *path, char **args, char **envp, t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	(void)data;
	pid = fork();
	if (pid == -1)
		return (print_error("fork", NULL, strerror(errno)), 1);
	
	if (pid == 0)
	{
		setup_signals_default();
		if (cmd && apply_redirections(cmd) == -1)
			_exit(1);
		execve(path, args, envp);
		if (errno == EACCES)
			print_error(args[0], NULL, "Permission denied");
		else
			print_error(args[0], NULL, strerror(errno));
		_exit((errno == EACCES) ? 126 : 127);
	}
	
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

int execute_external(t_cmd *cmd, t_data *data)
{
	char	*path;
	int		exit_code;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (127);
	
	path = resolve_command_path(cmd->args[0], data);
	if (!path)
		return (print_error(cmd->args[0], NULL, "command not found"), 127);
	
	exit_code = spawn_and_exec(path, cmd->args, data->envp, cmd, data);
	free(path);
	return (exit_code);
}