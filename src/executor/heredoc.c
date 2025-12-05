/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:58 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:52:00 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Função 1/5 - Processa uma linha do input */
static int	handle_heredoc_line(char *delim, bool quoted,
			int write_fd, t_data *data)
{
	char	*line;

	line = readline("> ");
	if (!line)
		return (1);
	if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
	{
		free(line);
		return (1);
	}
	if (!quoted)
		return (process_expanded_line(line, write_fd, data));
	process_quoted_line(line, write_fd);
	return (0);
}

/* Função 2/5 - Loop de leitura */
static int	read_heredoc_lines(char *delim, bool quoted,
			int write_fd, t_data *data)
{
	int	line_result;

	while (1)
	{
		line_result = handle_heredoc_line(delim, quoted,
				write_fd, data);
		if (line_result == 1)
			break ;
		if (line_result == -1)
			return (-1);
	}
	return (0);
}

/* Função 3/5 - Executa o heredoc */
static int	execute_heredoc(t_cmd *cmd, char *delim,
			bool quoted, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (create_heredoc_pipe(pipefd) == -1)
		return (free(delim), -1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (free(delim), -1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		setup_signals_heredoc();
		if (read_heredoc_lines(delim, quoted, pipefd[1], data) != 0)
			_exit(130);
		_exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
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

/* Função 4/5 - Verifica necessidade */
static int	should_prepare_heredoc(t_cmd *cmd)
{
	return (cmd && cmd->heredoc_delim);
}

/* Função 5/5 - Função principal */
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
