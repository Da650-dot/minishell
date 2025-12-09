/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:22:38 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/09 18:04:46 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*new_redir(t_token_type type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

static void	add_redir(t_redir **head, t_redir *new)
{
	t_redir	*current;

	if (!*head)
		*head = new;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

static bool	process_heredoc(t_cmd *cmd, t_token *filename)
{
	if (cmd->heredoc_delim)
		free(cmd->heredoc_delim);
	cmd->heredoc_delim = ft_strdup(filename->value);
	return (true);
}

static bool	process_redirect(t_cmd *cmd, t_token *redirect, t_token *filename)
{
	t_redir	*new;

	new = new_redir(redirect->type, filename->value);
	if (!new)
		return (false);
	add_redir(&cmd->redirects, new);
	return (true);
}

bool	handle_redirect_token(t_cmd *cmd, t_token **token)
{
	t_token	*redirect;
	t_token	*filename;

	redirect = *token;
	filename = redirect->next;
	if (!filename || filename->type != TOKEN_WORD)
	{
		print_error("syntax error", "expected filename", NULL);
		return (false);
	}
	if (redirect->type == TOKEN_HEREDOC)
	{
		if (!process_heredoc(cmd, filename))
			return (false);
	}
	else
	{
		if (!process_redirect(cmd, redirect, filename))
			return (false);
	}
	*token = filename->next;
	return (true);
}
