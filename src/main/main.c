/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:18:01 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/05 07:18:50 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_input(t_data *data, char *input)
{
	t_token		*tokens;
	t_pipeline	*pipeline;

	tokens = tokenize(input);
	if (!tokens)
		return ;
	pipeline = parse_tokens(tokens);
	free_tokens(tokens);
	if (!pipeline)
	{
		data->exit_status = 2;
		return ;
	}
	print_pipeline(pipeline);
	process_pipeline(pipeline, data);
	free_pipeline(pipeline);
}

static char	*get_user_input(t_data *data)
{
	char	*input;
	char	*prompt;

	reset_signal();
	setup_signals_interactive();
	prompt = build_prompt(data);
	input = readline(prompt);
	free(prompt);
	return (input);
}

static bool	handle_user_input(t_data *data, char *input)
{
	if (!input)
	{
		print_exit_message();
		return (false);
	}
	if (was_interrupted())
	{
		free(input);
		return (true);
	}
	if (input[0] == '\0' || is_only_spaces(input))
	{
		free(input);
		return (true);
	}
	add_history(input);
	process_input(data, input);
	free(input);
	return (true);
}

static void	repl_loop(t_data *data)
{
	char	*input;

	while (1)
	{
		input = get_user_input(data);
		if (!handle_user_input(data, input))
			break ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (handle_args(argc, argv))
		return (0);
	print_welcome_banner();
	init_data(&data, envp);
	setup_signals_interactive();
	repl_loop(&data);
	cleanup_data(&data);
	return (data.exit_status);
}
