#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include "../libft/libft.h"

# define SUCCESS 0
# define ERROR 1

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	**args;
	char	*input_file;
	char	*output_file;
	bool	append;
	char	*heredoc_delim;
}	t_cmd;

typedef struct s_pipeline
{
	t_cmd				*cmd;
	struct s_pipeline	*next;
}	t_pipeline;

typedef struct s_data
{
	char		**envp;
	int			exit_status;
	t_token		*tokens;
	t_pipeline	*pipeline;
}	t_data;

extern volatile sig_atomic_t	g_signal;

/* ======================= MAIN & INIT ======================= */
void		init_data(t_data *data, char **envp);
void		cleanup_data(t_data *data);
int			main(int argc, char **argv, char **envp);

/* ======================= SIGNALS ======================= */
void		setup_signals_interactive(void);
void		setup_signals_execution(void);
void		setup_signals_default(void);
void		setup_signals_heredoc(void);
void		reset_signal(void);
bool		was_interrupted(void);

/* ======================= PROMPT ======================= */
char		*build_prompt(t_data *data);
char		*build_simple_prompt(void);
void		print_welcome_banner(void);
void		print_simple_banner(void);
void		print_exit_message(void);
void		clear_screen(void);

/* ======================= LEXER ======================= */
t_token		*new_token(t_token_type type, char *value); 
t_token		*tokenize(char *input);
void		add_token(t_token **head, t_token *new_token);
void		free_tokens(t_token *tokens);
void		print_tokens(t_token *tokens);
void		test_lexer(void);
char		*extract_word(char *input, int *i);
t_token		*handle_redirect(char *input, int *i);
char		*ft_append_char(char *str, char c);
t_token		*process_word(char *input, int *i);  // ← CORRIGIDO: adicionei ;
bool		add_token_safe(t_token **tokens, t_token *new);

/* ======================= PARSER ======================= */
t_pipeline	*parse_tokens(t_token *tokens);
t_pipeline	*parse_single_cmd(t_token *tokens);
void		free_pipeline(t_pipeline *pipeline);
void		free_cmd(t_cmd *cmd);
void		print_pipeline(t_pipeline *pipeline);
void		print_cmd(t_cmd *cmd, int cmd_num);
void		test_parser(void);
t_cmd		*new_cmd(void);
t_pipeline	*new_pipeline_node(t_cmd *cmd);
void		add_pipeline_node(t_pipeline **head, t_pipeline *new);
int			count_words_until_pipe(t_token *tokens);
bool		has_redirects_in_tokens(t_token *tokens);
bool		fill_args(t_cmd *cmd, t_token *tokens, int word_count);
bool		handle_redirect_token(t_cmd *cmd, t_token **token);

/* ======================= UTILS ======================= */
char		**dup_envp(char **envp);
void		free_array(char **arr);
char		*get_env(char *key, char **envp);
void		print_error(char *cmd, char *arg, char *msg);
bool		is_only_spaces(char *str);
int			handle_args(int argc, char **argv);

/* ======================= TOKEN UTILS ======================= */
bool		is_operator(t_token *token);
bool		is_redirect(t_token *token);
bool		validate_tokens(t_token *tokens);
int			count_tokens(t_token *tokens);

#endif
