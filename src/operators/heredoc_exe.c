/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_exe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 14:42:47 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/06 19:01:32 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	fork_heredoc_child(int *pipefd, char *delim, bool quoted,
		t_data *data)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		setup_signals_interactive();
		free(delim);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		setup_signals_heredoc();
		if (read_heredoc_lines(delim, quoted, pipefd[1], data) != 0)
			_exit(130);
		_exit(0);
	}
	return (pid);
}

static int	execute_heredoc(t_cmd *cmd, char *delim, bool quoted, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (create_heredoc_pipe(pipefd) == -1)
		return (free(delim), -1);
	pid = fork_heredoc_child(pipefd, delim, quoted, data);
	if (pid == -1)
		return (-1);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	g_signal = 0;
	setup_signals_interactive();
	free(delim);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		data->exit_status = 130;
		return (-1);
	}
	cmd->heredoc_fd = pipefd[0];
	return (0);
}

static int	should_prepare_heredoc(t_cmd *cmd)
{
	return (cmd && cmd->heredoc_delim);
}

int	prepare_heredoc(t_cmd *cmd, t_data *data)
{
	char	*delim;
	bool	quoted;

	if (!should_prepare_heredoc(cmd))
		return (0);
	delim = extract_delimiter(cmd->heredoc_delim, &quoted);
	if (!delim)
		return (-1);
	return (execute_heredoc(cmd, delim, quoted, data));
}
