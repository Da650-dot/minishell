/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@example.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/11/29 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

/* Função 1/5 - Processa uma linha do input */
static int handle_heredoc_line(char *delim, bool quoted,
                              int write_fd, t_data *data)
{
    char *line;

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
    else
    {
        process_quoted_line(line, write_fd);
        return (0);
    }
}

/* Função 2/5 - Loop de leitura */
static int read_heredoc_lines(char *delim, bool quoted,
                             int write_fd, t_data *data)
{
    int line_result;

    while (1)
    {
        line_result = handle_heredoc_line(delim, quoted,
                                         write_fd, data);
        if (line_result == 1)
            break;
        if (line_result == -1)
            return (-1);
        if (was_interrupted())
            return (2);
    }
    return (0);
}

/* Função 3/5 - Executa o heredoc */
static int execute_heredoc(t_cmd *cmd, char *delim,
                          bool quoted, t_data *data)
{
    int pipefd[2];
    int read_result;

    if (create_heredoc_pipe(pipefd) == -1)
        return (-1);
    setup_signals_heredoc();
    read_result = read_heredoc_lines(delim, quoted,
                                    pipefd[1], data);
    if (read_result != 0)
    {
        cleanup_heredoc_resources(pipefd, delim);
        if (read_result == 2)
            data->exit_status = 130;
        return (-1);
    }
    reset_signal();
    free(delim);
    close(pipefd[1]);
    cmd->heredoc_fd = pipefd[0];
    return (0);
}

/* Função 4/5 - Verifica necessidade */
static int should_prepare_heredoc(t_cmd *cmd)
{
    return (cmd && cmd->heredoc_delim);
}

/* Função 5/5 - Função principal */
int prepare_heredoc(t_cmd *cmd, t_data *data)
{
    char *delim;
    bool quoted;
    int result;

    if (!should_prepare_heredoc(cmd))
        return (0);
    delim = extract_delimiter(cmd->heredoc_delim, &quoted);
    if (!delim)
        return (-1);
    result = execute_heredoc(cmd, delim, quoted, data);
    if (result == -1)
    {
        free(delim);
        return (-1);
    }
    return (0);
}
