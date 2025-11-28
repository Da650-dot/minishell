#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = false;
	cmd->heredoc_delim = NULL;
	return (cmd);
}

t_pipeline	*new_pipeline_node(t_cmd *cmd)
{
	t_pipeline	*node;

	node = malloc(sizeof(t_pipeline));
	if (!node)
		return (NULL);
	node->cmd = cmd;
	node->next = NULL;
	return (node);
}

void	add_pipeline_node(t_pipeline **head, t_pipeline *new)
{
	t_pipeline	*current;

	if (!*head)
		*head = new;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

int	count_words_until_pipe(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			count++; 
		}
		current = current->next;
	}
	return (count);
}

bool	has_redirects_in_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (is_redirect(current))
			return (true);
		current = current->next;
	}
	return (false);
}
