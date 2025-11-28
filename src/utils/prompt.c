/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define COLOR_RESET		"\001\033[0m\002"
#define COLOR_GREEN		"\001\033[32m\002"
#define COLOR_CYAN		"\001\033[36m\002"
#define SYMBOL_PROMPT	"➜ "
#define SYMBOL_ERROR	"✗ "

static char	*get_current_dir(char **envp)
{
	char	*cwd;
	char	*home;
	char	*simplified;
	int		home_len;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("?"));
	home = get_env("HOME", envp);
	if (!home)
		return (cwd);
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) == 0)
	{
		if (cwd[home_len] == '\0')
			simplified = ft_strdup("~");
		else
			simplified = ft_strjoin("~", &cwd[home_len]);
		free(cwd);
		return (simplified);
	}
	return (cwd);
}

static char	*append_to_prompt(char *prompt, char *to_append)
{
	char	*temp;

	temp = ft_strjoin(prompt, to_append);
	free(prompt);
	return (temp);
}

char	*build_prompt(t_data *data)
{
	char	*prompt;
	char	*cwd;

	cwd = get_current_dir(data->envp);
	prompt = ft_strdup("");
	if (data->exit_status == 0)
		prompt = append_to_prompt(prompt, COLOR_GREEN);
	else
		prompt = append_to_prompt(prompt, "\001\033[31m\002");
	prompt = append_to_prompt(prompt, cwd);
	free(cwd);
	prompt = append_to_prompt(prompt, COLOR_RESET);
	prompt = append_to_prompt(prompt, " ");
	if (data->exit_status == 0)
		prompt = append_to_prompt(prompt, COLOR_CYAN SYMBOL_PROMPT COLOR_RESET);
	else
		prompt = append_to_prompt(prompt, "\001\033[31m\002" SYMBOL_ERROR COLOR_RESET);
	return (prompt);
}

char	*build_simple_prompt(void)
{
	return (ft_strdup("minishell> "));
}

void	print_welcome_banner(void)
{
	printf("\n");
	printf("╔══════════════════════════════════════════════════╗\n");
	printf("║             🐚  MINISHELL  🐚                    ║\n");
	printf("║          Bem-vindo ao seu shell!                ║\n");
	printf("╚══════════════════════════════════════════════════╝\n");
	printf("\n");
	printf("  Digite 'exit' ou pressione Ctrl-D para sair\n");
	printf("\n");
}

void	print_simple_banner(void)
{
	printf("\n");
	printf("==================================================\n");
	printf("               MINISHELL v1.0                     \n");
	printf("==================================================\n");
	printf("\n");
}

void	clear_screen(void)
{
	printf("\033[H\033[J");
}

void	print_exit_message(void)
{
	printf("\n");
	printf("👋 Até logo! Obrigado por usar o minishell.\n");
	printf("\n");
}

