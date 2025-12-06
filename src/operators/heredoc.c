/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:51:58 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/06 14:47:51 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_line(char *delim, bool quoted,
			int write_fd, t_data *data)
{
	char	*line;

	line = readline("> ");
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document delimited by ", 2);
		ft_putstr_fd("end-of-file (wanted `", 2);
		ft_putstr_fd(delim, 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
	{
		free(line);
		return (1);
	}
	if (!quoted)
		return (process_expanded_line(line, write_fd, data));
	process_quoted_line(line, write_fd);
	return (0);
}

int	read_heredoc_lines(char *delim, bool quoted,
			int write_fd, t_data *data)
{
	int	line_result;

	while (1)
	{
		line_result = handle_heredoc_line(delim, quoted,
				write_fd, data);
		if (line_result == 1)
			break ;
		if (line_result == -1)
			return (-1);
	}
	return (0);
}
