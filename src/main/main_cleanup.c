/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:16:42 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 17:16:43 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_data(t_data *data)
{
	if (data->envp)
		free_array(data->envp);
	if (data->tokens)
		free_tokens(data->tokens);
	if (data->pipeline)
		free_pipeline(data->pipeline);
	rl_clear_history();
}

void	init_data(t_data *data, char **envp)
{
	data->envp = dup_envp(envp);
	data->exit_status = 0;
	data->tokens = NULL;
	data->pipeline = NULL;
}