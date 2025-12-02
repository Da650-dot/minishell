#include "minishell.h"

static void	setup_child_fds(t_exec_ctx *ctx)
{
	int	i;

	if (ctx->in_fd != STDIN_FILENO)
		dup2(ctx->in_fd, STDIN_FILENO);
	if (ctx->out_fd != STDOUT_FILENO)
		dup2(ctx->out_fd, STDOUT_FILENO);
	i = 0;
	while (i < ctx->pipefds_len)
	{
		if (ctx->pipefds && ctx->pipefds[i] > 2)
			close(ctx->pipefds[i]);
		i++;
	}
}

static void	execute_child_builtin(t_cmd *cmd, t_data *data)
{
	int	ret;

	if (apply_redirections(cmd) == -1)
		_exit(1);
	if (!cmd->args || !cmd->args[0])
		_exit(0);
	ret = execute_builtin(cmd->args, data);
	_exit(ret);
}

static void	handle_exec_error(char *cmd_name, char *path)
{
	if (errno == EACCES)
	{
		print_error(cmd_name, NULL, "Permission denied");
		free(path);
		_exit(126);
	}
	else
	{
		print_error(cmd_name, NULL, strerror(errno));
		free(path);
		_exit(127);
	}
}

static void	execute_child_external(t_cmd *cmd, t_data *data)
{
	char	*path;

	if (apply_redirections(cmd) == -1)
		_exit(1);
	if (!cmd->args || !cmd->args[0])
		_exit(0);
	path = resolve_command_path(cmd->args[0], data);
	if (!path)
	{
		print_error(cmd->args[0], NULL, "command not found");
		_exit(127);
	}
	execve(path, cmd->args, data->envp);
	handle_exec_error(cmd->args[0], path);
}

void	child_exec_cmd(t_cmd *cmd, t_data *data, t_exec_ctx *ctx)
{
	setup_signals_default();
	setup_child_fds(ctx);
	if (is_builtin(cmd->args[0]))
		execute_child_builtin(cmd, data);
	else
		execute_child_external(cmd, data);
}
