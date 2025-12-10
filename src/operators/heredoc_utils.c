/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:43 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/10 11:52:51 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_delimiter(char *delim, bool *quoted)
{
	size_t	len;
	char	*result;

	*quoted = false;
	if (!delim)
		return (NULL);
	len = ft_strlen(delim);
	if (len >= 2)
	{
		if ((delim[0] == '\'' && delim[len - 1] == '\'')
			|| (delim[0] == '"' && delim[len - 1] == '"'))
		{
			*quoted = true;
			result = ft_substr(delim, 1, len - 2);
			return (result);
		}
	}
	result = ft_strdup(delim);
	return (result);
}

int	create_heredoc_pipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		print_error("pipe", NULL, strerror(errno));
		return (-1);
	}
	return (0);
}

int	process_expanded_line(char *line, int fd, t_data *data)
{
	char	*expanded;

	expanded = expand_variables(line, data);
	free(line);
	if (!expanded)
		return (-1);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (0);
}

void	process_quoted_line(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	cleanup_heredoc_resources(int *pipefd, char *delim)
{
	if (delim)
		free(delim);
	if (pipefd[0] > 0)
		close(pipefd[0]);
	if (pipefd[1] > 0)
		close(pipefd[1]);
	reset_signal();
}
