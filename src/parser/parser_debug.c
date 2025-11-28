#include "minishell.h"

void	print_cmd(t_cmd *cmd, int cmd_num)
{
	int	i;

	printf("\n  ---- Comando %d ----\n", cmd_num);
	printf("  Args: [");
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("\"%s\"", cmd->args[i]);
			if (cmd->args[i + 1])
				printf(", ");
			i++;
		}
	}
	printf("]\n");
	if (cmd->input_file)
		printf("  Input:  < %s\n", cmd->input_file);
	if (cmd->output_file)
	{
		if (cmd->append)
			printf("  Output: >> %s\n", cmd->output_file);
		else
			printf("  Output: > %s\n", cmd->output_file);
	}
	if (cmd->heredoc_delim)
		printf("  Heredoc: << %s\n", cmd->heredoc_delim);
}

void	print_pipeline(t_pipeline *pipeline)
{
	t_pipeline	*current;
	int			i;

	printf("\n========== PIPELINE ==========\n");
	current = pipeline;
	i = 1;
	while (current)
	{
		print_cmd(current->cmd, i);
		if (current->next)
			printf("  └─> PIPE |\n");
		current = current->next;
		i++;
	}
	printf("==============================\n\n");
}

void	test_parser(void)
{
	t_token		*tokens;
	t_pipeline	*pipeline;
	
	printf("\n========== TESTE PARSER ==========\n");
	tokens = tokenize("ls -la");
	pipeline = parse_tokens(tokens);
	print_pipeline(pipeline);
	free_tokens(tokens);
	free_pipeline(pipeline);
	printf("==================================\n\n");
}
