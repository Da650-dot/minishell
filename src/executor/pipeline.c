/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copilot <copilot@example.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/11/29 00:00:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static int	count_cmds(t_pipeline *pl)
{
    int n = 0;
    while (pl)
    {
        n++;
        pl = pl->next;
    }
    return (n);
}

static void	close_fds(int *fds, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (fds[i] > 2)
            close(fds[i]);
        i++;
    }
}

static void	child_exec_cmd(t_cmd *cmd, t_data *data, int in_fd, int out_fd, int *pipefds, int pipefds_len)
{
    int i;

    /* Setup child signal behaviour for exec */
    setup_signals_default();
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
    }
    if (out_fd != STDOUT_FILENO)
    {
        dup2(out_fd, STDOUT_FILENO);
    }
    /* close all pipe fds inherited */
    i = 0;
    while (i < pipefds_len)
    {
        if (pipefds && pipefds[i] > 2)
            close(pipefds[i]);
        i++;
    }

    /* Apply redirections in the child (if any) */
    if (cmd->input_file || cmd->output_file || cmd->heredoc_delim)
    {
        if (apply_redirections(cmd) == -1)
            _exit(1);
    }

    if (!cmd->args || !cmd->args[0])
        _exit(0);

    if (is_builtin(cmd->args[0]))
    {
        /* run builtin in child for pipeline (won't affect parent env) */
        int ret = execute_builtin(cmd->args, data);
        _exit(ret);
    }
    else
    {
        char *path = resolve_command_path(cmd->args[0], data);
        if (!path)
        {
            print_error(cmd->args[0], NULL, "command not found");
            _exit(127);
        }
        execve(path, cmd->args, data->envp);
        /* execve failed */
        if (errno == EACCES)
            print_error(cmd->args[0], NULL, "Permission denied");
        else
            print_error(cmd->args[0], NULL, strerror(errno));
        free(path);
        _exit((errno == EACCES) ? 126 : 127);
    }
}

void	execute_pipeline(t_pipeline *pipeline, t_data *data)
{
    int n;
    int *pipefds = NULL;
    int pipefds_len;
    int i;
    pid_t *pids;
    t_pipeline *cur;

    n = count_cmds(pipeline);
    if (n <= 0)
        return;
    /* Prepare heredocs for all commands in parent before forking children */
    {
        t_pipeline *it = pipeline;
        while (it)
        {
            if (it->cmd && it->cmd->heredoc_delim)
            {
                if (prepare_heredoc(it->cmd, data) == -1)
                {
                    /* cleanup any heredoc fds already created */
                    t_pipeline *c = pipeline;
                    while (c)
                    {
                        if (c->cmd && c->cmd->heredoc_fd >= 0)
                        {
                            close(c->cmd->heredoc_fd);
                            c->cmd->heredoc_fd = -1;
                        }
                        c = c->next;
                    }
                    /* interrupted or error: set exit and abort pipeline */
                    return;
                }
            }
            it = it->next;
        }
    }
    if (n == 1)
    {
        /* Single command pipeline: parent should handle builtin vs external */
        t_cmd *cmd = pipeline->cmd;
        if (!cmd || !cmd->args || !cmd->args[0])
        {
            data->exit_status = 0;
            return;
        }
        if (is_builtin(cmd->args[0]))
        {
            if (cmd->input_file || cmd->output_file || cmd->heredoc_delim)
            {
                t_redirect_save save;
                if (save_stdio(&save) == -1)
                {
                    print_error("dup", NULL, "failed to backup stdio");
                    data->exit_status = 1;
                    return;
                }
                if (apply_redirections(cmd) == -1)
                {
                    restore_stdio(&save);
                    data->exit_status = 1;
                    return;
                }
                data->exit_status = execute_builtin(cmd->args, data);
                restore_stdio(&save);
            }
            else
                data->exit_status = execute_builtin(cmd->args, data);
        }
        else
            data->exit_status = execute_external(cmd, data);
        return;
    }

    pipefds_len = 2 * (n - 1);
    pipefds = malloc(sizeof(int) * pipefds_len);
    if (!pipefds)
    {
        print_error("malloc", NULL, "pipe fds alloc failed");
        data->exit_status = 1;
        return;
    }
    i = 0;
    while (i < n - 1)
    {
        if (pipe(pipefds + i * 2) == -1)
        {
            print_error("pipe", NULL, strerror(errno));
            free(pipefds);
            data->exit_status = 1;
            return;
        }
        i++;
    }

    pids = malloc(sizeof(pid_t) * n);
    if (!pids)
    {
        print_error("malloc", NULL, "pids alloc failed");
        close_fds(pipefds, pipefds_len);
        free(pipefds);
        data->exit_status = 1;
        return;
    }

    i = 0;
    cur = pipeline;
    while (cur)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            print_error("fork", NULL, strerror(errno));
            /* cleanup */
            close_fds(pipefds, pipefds_len);
            free(pipefds);
            free(pids);
            data->exit_status = 1;
            return;
        }
        if (pid == 0)
        {
            int in_fd = (i == 0) ? STDIN_FILENO : pipefds[(i - 1) * 2];
            int out_fd = (i == n - 1) ? STDOUT_FILENO : pipefds[i * 2 + 1];
            child_exec_cmd(cur->cmd, data, in_fd, out_fd, pipefds, pipefds_len);
            /* child_exec_cmd should not return */
            _exit(1);
        }
        pids[i] = pid;
        cur = cur->next;
        i++;
    }

    /* Parent: close all pipe fds */
    close_fds(pipefds, pipefds_len);

    /* Wait for all children; use last child's status for pipeline exit code */
    i = 0;
    int status;
    int last_status = 0;
    while (i < n)
    {
        if (waitpid(pids[i], &status, 0) == -1)
        {
            print_error("waitpid", NULL, strerror(errno));
        }
        else
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
        i++;
    }

    data->exit_status = last_status;
    free(pipefds);
    free(pids);
}
