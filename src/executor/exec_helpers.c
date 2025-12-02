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
    if (!cmd || !cmd[0])
        return (NULL);
    return (find_executable(cmd, data->envp));
}

static int map_exec_errno(int err)
{
    if (err == EACCES)
        return (126);
    return (127);
}

int spawn_and_exec(char *path, char **args, char **envp, t_cmd *cmd, t_data *data)
{
    pid_t pid;
    int status;

    (void)data;
    pid = fork();
    if (pid == -1)
    {
        print_error("fork", NULL, strerror(errno));
        return (1);
    }
    if (pid == 0)
    {
        /* child */
        setup_signals_default();
        /* apply redirections (if any) before exec) */
        if (cmd)
        {
            if (apply_redirections(cmd) == -1)
                _exit(1);
        }
        execve(path, args, envp);
        /* execve returned -> error */
        if (errno == EACCES)
            print_error(args[0], NULL, "Permission denied");
        else
            print_error(args[0], NULL, strerror(errno));
        _exit(map_exec_errno(errno));
    }
    /* parent */
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
    char *path;
    int exit_code;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (127);
    path = resolve_command_path(cmd->args[0], data);
    if (!path)
    {
        print_error(cmd->args[0], NULL, "command not found");
        return (127);
    }
    exit_code = spawn_and_exec(path, cmd->args, data->envp, cmd, data);
    free(path);
    return (exit_code);
}
