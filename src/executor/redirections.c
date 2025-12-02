/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@example.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/11/29 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int	apply_redirections(t_cmd *cmd)
{
    int fd;

    if (!cmd)
        return (0);
    if (cmd->input_file)
    {
        fd = open(cmd->input_file, O_RDONLY);
        if (fd == -1)
        {
            print_error(cmd->input_file, NULL, strerror(errno));
            return (-1);
        }
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            print_error("dup2", NULL, strerror(errno));
            close(fd);
            return (-1);
        }
        close(fd);
    }
    if (cmd->heredoc_fd >= 0)
    {
        if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
        {
            print_error("dup2", NULL, strerror(errno));
            close(cmd->heredoc_fd);
            return (-1);
        }
        close(cmd->heredoc_fd);
        cmd->heredoc_fd = -1;
    }
    if (cmd->output_file)
    {
        int flags = O_CREAT | O_WRONLY | (cmd->append ? O_APPEND : O_TRUNC);
        fd = open(cmd->output_file, flags, 0644);
        if (fd == -1)
        {
            print_error(cmd->output_file, NULL, strerror(errno));
            return (-1);
        }
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            print_error("dup2", NULL, strerror(errno));
            close(fd);
            return (-1);
        }
        close(fd);
    }
    /* heredoc is not handled here (will be implemented separately) */
    return (0);
}

int	save_stdio(t_redirect_save *save)
{
    if (!save)
        return (-1);
    save->stdin_backup = dup(STDIN_FILENO);
    save->stdout_backup = dup(STDOUT_FILENO);
    if (save->stdin_backup == -1 || save->stdout_backup == -1)
    {
        if (save->stdin_backup != -1)
            close(save->stdin_backup);
        if (save->stdout_backup != -1)
            close(save->stdout_backup);
        return (-1);
    }
    return (0);
}

int	restore_stdio(t_redirect_save *save)
{
    if (!save)
        return (-1);
    if (save->stdin_backup != -1)
    {
        dup2(save->stdin_backup, STDIN_FILENO);
        close(save->stdin_backup);
    }
    if (save->stdout_backup != -1)
    {
        dup2(save->stdout_backup, STDOUT_FILENO);
        close(save->stdout_backup);
    }
    return (0);
}
