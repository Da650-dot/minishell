/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:19:07 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/06 17:23:52 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _DEFAULT_SOURCE
# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

extern volatile sig_atomic_t	g_signal;

typedef struct s_pipeline		t_pipeline;
typedef struct s_data			t_data;

typedef struct s_exec_ctx
{
	int							in_fd;
	int							out_fd;
	int							*pipefds;
	int							pipefds_len;
	t_data						*data;
	t_pipeline					*pipeline;
	int							n;
}								t_exec_ctx;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

typedef struct s_redir
{
	t_token_type				type;
	char						*file;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						**args;
	t_redir						*redirects;
	char						*heredoc_delim;
	int							word_count;
	int							heredoc_fd;
}								t_cmd;

typedef struct s_redirect_save
{
	int							stdin_backup;
	int							stdout_backup;
}								t_redirect_save;

typedef struct s_pipeline
{
	t_cmd						*cmd;
	struct s_pipeline			*next;
}								t_pipeline;

typedef struct s_data
{
	char						**envp;
	int							exit_status;
	t_token						*tokens;
	t_pipeline					*pipeline;
}								t_data;

/* ======================= MAIN & INIT ======================= */
void							init_data(t_data *data, char **envp);
void							cleanup_data(t_data *data);
int								main(int argc, char **argv, char **envp);

/* ======================= SIGNALS ======================= */
void							setup_signals_interactive(void);
void							setup_signals_execution(void);
void							setup_signals_default(void);
void							setup_signals_heredoc(void);
void							reset_signal(void);
bool							was_interrupted(void);

/* ======================= PROMPT ======================= */
char							*build_prompt(t_data *data);
char							*build_simple_prompt(void);

/* ======================= LEXER ======================= */
t_token							*new_token(t_token_type type, char *value);
t_token							*tokenize(char *input);
void							add_token(t_token **head, t_token *new_token);
void							free_tokens(t_token *tokens);
void							print_tokens(t_token *tokens);
char							*extract_word(char *input, int *i);
t_token							*handle_redirect(char *input, int *i);
char							*ft_append_char(char *str, char c);
t_token							*process_word(char *input, int *i);
bool							add_token_safe(t_token **tokens, t_token *new);

/* ======================= PARSER ======================= */
t_pipeline						*parse_tokens(t_token *tokens);
t_pipeline						*parse_single_cmd(t_token *tokens);
t_pipeline						*new_pipeline_node(t_cmd *cmd);
t_cmd							*new_cmd(void);
void							free_pipeline(t_pipeline *pipeline);
void							free_cmd(t_cmd *cmd);
void							add_pipeline_node(t_pipeline **head,
									t_pipeline *new);
int								count_words_until_pipe(t_token *tokens);
bool							has_redirects_in_tokens(t_token *tokens);
bool							fill_args(t_cmd *cmd, t_token *tokens,
									int word_count);
bool							handle_redirect_token(t_cmd *cmd,
									t_token **token);

/* ======================= UTILS ======================= */
char							**dup_envp(char **envp);
void							free_array(char **arr);
char							*get_env(char *key, char **envp);
void							print_error(char *cmd, char *arg, char *msg);
bool							is_only_spaces(char *str);
int								handle_args(int argc, char **argv);

/* ======================= BUILT-INS ======================= */
int								builtin_echo(char **args);
int								builtin_cd(char **args, t_data *data);
int								builtin_pwd(void);
int								builtin_export(char **args, t_data *data);
int								builtin_unset(char **args, t_data *data);
int								builtin_env(char **envp);
int								builtin_exit(char **args, t_data *data);
bool							is_builtin(char *cmd);
int								execute_builtin(char **args, t_data *data);

/* ======================= ENV MANIPULATION ======================= */
char							**add_env_var(char **envp, char *new_var);
char							**remove_env_var(char **envp, char *key);
bool							is_valid_identifier(char *str);
int								find_env_index(char **envp, char *key);

/* ======================= TOKEN UTILS ======================= */
bool							is_operator(t_token *token);
bool							is_redirect(t_token *token);
bool							validate_tokens(t_token *tokens);
int								count_tokens(t_token *tokens);

/* ======================= PERMISSIONS ======================= */
bool							check_execute_permission(char *path);
bool							check_read_permission(char *path);
bool							check_write_permission(char *path);
char							*find_executable(char *cmd, char **envp);

/* ======================= EXPANSION ======================= */
char							*expand_variables(char *str, t_data *data);
char							**expand_args(char **args, t_data *data);
char							*process_dollar(char *str, int *i,
									t_data *data);
char							*append_str(char *result, char *to_append);
char							*remove_quotes(char *str);

/* ======================= EXECUTOR MINIMAL ======================= */
void							process_pipeline(t_pipeline *pipeline,
									t_data *data);
void							execute_pipeline(t_pipeline *pipeline,
									t_data *data);

/* ======================= EXECUTOR HELPERS (scaffolding) =========== */
char							*resolve_command_path(char *cmd, t_data *data);
int								spawn_and_exec(char *path, char **args,
									char **envp, t_cmd *cmd, t_data *data);
int								execute_external(t_cmd *cmd, t_data *data);

/* Redirections helpers */
int								apply_redirections(t_cmd *cmd, t_data *data);
int								save_stdio(t_redirect_save *save);
int								restore_stdio(t_redirect_save *save);
int								prepare_heredoc(t_cmd *cmd, t_data *data);

/* Funções públicas do heredoc */
int								handle_heredoc_line(char *delim, bool quoted,
									int write_fd, t_data *data);
int								read_heredoc_lines(char *delim, bool quoted,
									int write_fd, t_data *data);

/* Funções utilitárias */
char							*extract_delimiter(char *delim, bool *quoted);
int								create_heredoc_pipe(int *pipefd);
int								process_expanded_line(char *line, int fd,
									t_data *data);
void							process_quoted_line(char *line, int fd);
void							cleanup_heredoc_resources(int *pipefd,
									char *delim);

int								count_cmds(t_pipeline *pl);
void							close_fds(int *fds, int count);
int								prepare_pipeline_heredocs(t_pipeline *pipeline,
									t_data *data);
void							handle_single_command(t_pipeline *pipeline,
									t_data *data);
void							execute_multi_pipeline(t_pipeline *pipeline,
									t_data *data, int n);
void							execute_pipeline_main(t_pipeline *pipeline,
									t_data *data);

void							execute_pipeline(t_pipeline *pipeline,
									t_data *data);
void							execute_multi_pipeline(t_pipeline *pipeline,
									t_data *data, int n);
void							close_fds(int *fds, int len);
void							child_exec_cmd(t_cmd *cmd, t_data *data,
									t_exec_ctx *ctx);

void							execute_child_builtin(t_cmd *cmd, t_data *data);
void							execute_child_external(t_cmd *cmd,
									t_data *data);
void							handle_exec_error(char *cmd_name);

int								create_pipes(int **pipefds, int n);
int								wait_all(pid_t *pids, int n);
#endif
