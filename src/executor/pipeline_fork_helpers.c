/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_fork_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 06:47:17 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/05 06:47:19 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipes(int **pipefds, int n)
{
	int	i;
	int	pipefds_len;

	pipefds_len = 2 * (n - 1);
	*pipefds = malloc(sizeof(int) * pipefds_len);
	if (!*pipefds)
		return (0);
	i = 0;
	while (i < n - 1)
	{
		if (pipe(*pipefds + i * 2) == -1)
		{
			free(*pipefds);
			return (0);
		}
		i++;
	}
	return (pipefds_len);
}

int	wait_all(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) != -1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}
