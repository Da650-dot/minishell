/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 11:24:53 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/06 13:28:14 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipeline(t_pipeline *pipeline, t_data *data)
{
	int	n;

	n = count_cmds(pipeline);
	if (n <= 0)
		return ;
	if (prepare_pipeline_heredocs(pipeline, data) == -1)
	{
		g_signal = 0;
		setup_signals_interactive();
		return ;
	}
	if (n == 1)
		handle_single_command(pipeline, data);
	else
		execute_multi_pipeline(pipeline, data, n);
}
