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

void	execute_pipeline(t_pipeline *pipeline, t_data *data)
{
    int n = count_cmds(pipeline);
    if (n <= 0)
        return;
    prepare_pipeline_heredocs(pipeline, data);
    if (n == 1)
        handle_single_command(pipeline, data);
    else
        execute_multi_pipeline(pipeline, data, n);
}
