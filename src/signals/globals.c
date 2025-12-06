/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 07:30:00 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/06 14:55:20 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	on_heredoc(int set)
{
	static int	heredoc_flag = 0;

	if (set == -1)
		return (heredoc_flag);
	if (set == 1)
		heredoc_flag = 1;
	else if (set == 0)
		heredoc_flag = 0;
	return (heredoc_flag);
}

int	is_fork(int set)
{
	static int	fork_flag = 0;

	if (set == -1)
		return (fork_flag);
	if (set == 1)
		fork_flag = 1;
	else if (set == 0)
		fork_flag = 0;
	return (fork_flag);
}

int	is_after_prompt(int set)
{
	static int	prompt_flag = 0;

	if (set == -1)
		return (prompt_flag);
	if (set == 1)
		prompt_flag = 1;
	else if (set == 0)
		prompt_flag = 0;
	return (prompt_flag);
}
