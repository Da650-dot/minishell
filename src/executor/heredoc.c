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

static char *strip_quotes_if_needed(char *delim, bool *quoted)
{
    size_t len;
    char *res;

    *quoted = false;
    if (!delim)
        return (NULL);
    len = ft_strlen(delim);
    if (len >= 2 && ((delim[0] == '\'' && delim[len - 1] == '\'') || (delim[0] == '"' && delim[len - 1] == '"')))
    {
        *quoted = true;
        res = ft_substr(delim, 1, len - 2);
        return (res);
    }
    return (ft_strdup(delim));
}

int	prepare_heredoc(t_cmd *cmd, t_data *data)
{
    int pipefd[2];
    char *line;
    char *delim;
    bool quoted;

    if (!cmd || !cmd->heredoc_delim)
        return (0);
    if (pipe(pipefd) == -1)
    {
        print_error("pipe", NULL, strerror(errno));
        return (-1);
    }
    delim = strip_quotes_if_needed(cmd->heredoc_delim, &quoted);
    setup_signals_heredoc();
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            /* EOF reached */
            break;
        }
        if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
        {
            free(line);
            break;
        }
        if (!quoted)
        {
            char *expanded = expand_variables(line, data);
            free(line);
            if (!expanded)
            {
                free(delim);
                close(pipefd[0]);
                close(pipefd[1]);
                reset_signal();
                return (-1);
            }
            write(pipefd[1], expanded, ft_strlen(expanded));
            write(pipefd[1], "\n", 1);
            free(expanded);
        }
        else
        {
            write(pipefd[1], line, ft_strlen(line));
            write(pipefd[1], "\n", 1);
            free(line);
        }
        if (was_interrupted())
        {
            free(delim);
            close(pipefd[0]);
            close(pipefd[1]);
            reset_signal();
            data->exit_status = 130;
            return (-1);
        }
    }
    reset_signal();
    free(delim);
    close(pipefd[1]);
    cmd->heredoc_fd = pipefd[0];
    return (0);
}
