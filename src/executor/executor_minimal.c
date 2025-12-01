#include "minishell.h"

/**
 * @brief Expande variáveis em todos os comandos da pipeline
 * @note Percorre toda a pipeline e expande $VAR e $?
 */
static void	expand_pipeline_vars(t_pipeline *pipeline, t_data *data)
{
	t_pipeline	*current;
	char		**expanded;

	current = pipeline;
	while (current)
	{
		if (current->cmd->args)
		{
			expanded = expand_args(current->cmd->args, data);
			if (expanded)
			{
				free_array(current->cmd->args);
				current->cmd->args = expanded;
			}
		}
		current = current->next;
	}
}

/**
 * @brief Executor MÍNIMO - apenas para built-ins
 * @note Ignora pipes e redirecionamentos
 * @note Executa APENAS o primeiro comando se for built-in
 */
static void	execute_simple_builtin(t_pipeline *pipeline, t_data *data)
{
	t_cmd	*cmd;

	if (!pipeline || !pipeline->cmd)
		return;
	cmd = pipeline->cmd;
	if (!cmd->args || !cmd->args[0])
	{
		data->exit_status = 0;
		return;
	}
	/* Delegate to execute_pipeline which handles single and multi-command cases */
	execute_pipeline(pipeline, data);
}

/**
 * @brief Processa a pipeline completa
 * @note 1. Expande variáveis
 * @note 2. Executa built-in (se for o caso)
 */
void	process_pipeline(t_pipeline *pipeline, t_data *data)
{
	expand_pipeline_vars(pipeline, data);
	print_pipeline(pipeline);
	execute_simple_builtin(pipeline, data);
}

