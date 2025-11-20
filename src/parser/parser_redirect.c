#include "minishell.h"

static void	handle_redir_in(t_cmd *cmd, char *filename)
{
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(filename);
}

static void	handle_redir_out(t_cmd *cmd, char *filename, bool append)
{
	if (cmd->output_file)
		free(cmd->output_file);
	cmd->output_file = ft_strdup(filename);
	cmd->append = append;
}

static void	handle_heredoc(t_cmd *cmd, char *delimiter)
{
	if (cmd->heredoc_delim)
		free(cmd->heredoc_delim);
	cmd->heredoc_delim = ft_strdup(delimiter);
}

bool	handle_redirect_token(t_cmd *cmd, t_token **token)
{
	t_token	*redirect;
	t_token	*filename;

	redirect = *token;
	filename = redirect->next;
	if (!filename || filename->type != TOKEN_WORD)
		return (print_error("syntax error", "expected filename", NULL), false);
	if (redirect->type == TOKEN_REDIR_IN)
		handle_redir_in(cmd, filename->value);
	else if (redirect->type == TOKEN_REDIR_OUT)
		handle_redir_out(cmd, filename->value, false);
	else if (redirect->type == TOKEN_APPEND)
		handle_redir_out(cmd, filename->value, true);
	else if (redirect->type == TOKEN_HEREDOC)
		handle_heredoc(cmd, filename->value);
	*token = filename->next;
	return (true);
}
