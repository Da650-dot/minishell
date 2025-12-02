#include "minishell.h"

#include "minishell.h"

int	count_cmds(t_pipeline *pl)
{
    int n = 0;
    while (pl)
    {
        n++;
        pl = pl->next;
    }
    return (n);
}

void	close_fds(int *fds, int count)
{
    int i = 0;
    while (i < count)
    {
        if (fds[i] > 2)
            close(fds[i]);
        i++;
    }
}

void	prepare_pipeline_heredocs(t_pipeline *pipeline, t_data *data)
{
    t_pipeline *it = pipeline;
    while (it)
    {
        if (it->cmd && it->cmd->heredoc_delim)
        {
            if (prepare_heredoc(it->cmd, data) == -1)
            {
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
                return;
            }
        }
        it = it->next;
    }
}

void	handle_single_command(t_pipeline *pipeline, t_data *data)
{
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
}
