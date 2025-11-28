#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd->input_file);
	free(cmd->output_file);
	free(cmd->heredoc_delim);
	free(cmd);
}

void	free_pipeline(t_pipeline *pipeline)
{
	t_pipeline	*temp;

	while (pipeline)
	{
		temp = pipeline->next;
		free_cmd(pipeline->cmd);
		free(pipeline);
		pipeline = temp;
	}
}
