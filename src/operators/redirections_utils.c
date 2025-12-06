/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:49:03 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/03 16:49:06 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_stdio(t_redirect_save *save)
{
	if (!save)
		return (-1);
	save->stdin_backup = dup(STDIN_FILENO);
	save->stdout_backup = dup(STDOUT_FILENO);
	if (save->stdin_backup == -1 || save->stdout_backup == -1)
	{
		if (save->stdin_backup != -1)
			close(save->stdin_backup);
		if (save->stdout_backup != -1)
			close(save->stdout_backup);
		return (-1);
	}
	return (0);
}

int	restore_stdio(t_redirect_save *save)
{
	if (!save)
		return (-1);
	if (save->stdin_backup != -1)
	{
		dup2(save->stdin_backup, STDIN_FILENO);
		close(save->stdin_backup);
	}
	if (save->stdout_backup != -1)
	{
		dup2(save->stdout_backup, STDOUT_FILENO);
		close(save->stdout_backup);
	}
	return (0);
}
