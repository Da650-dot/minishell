/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 16:50:06 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/04 11:26:22 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_single_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		print_error(file, NULL, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_error("dup2", NULL, strerror(errno));
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	apply_single_redir_out(char *file, bool append)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		print_error(file, NULL, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_error("dup2", NULL, strerror(errno));
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	apply_heredoc_redirection(t_cmd *cmd)
{
	if (cmd->heredoc_fd >= 0)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
		{
			print_error("dup2", NULL, strerror(errno));
			close(cmd->heredoc_fd);
			return (-1);
		}
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	return (0);
}

static int	apply_file_redirections(t_cmd *cmd)
{
	t_redir	*current;

	current = cmd->redirects;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
		{
			if (apply_single_redir_in(current->file) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_OUT)
		{
			if (apply_single_redir_out(current->file, false) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_APPEND)
		{
			if (apply_single_redir_out(current->file, true) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	apply_redirections(t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (apply_file_redirections(cmd) == -1)
		return (-1);
	if (apply_heredoc_redirection(cmd) == -1)
		return (-1);
	return (0);
}
