/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:26:58 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/12/05 16:49:51 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


volatile sig_atomic_t	g_heredoc_interrupted = 0;
static int				g_stdin_backup = -1;

static void	handle_sigint_heredoc(int signum)
{
	(void)signum;
	g_heredoc_interrupted = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_done = 1;
	/* Close stdin to force readline to return with EOF */
	if (on_heredoc(-1))
		close(STDIN_FILENO);
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	/* Save stdin before entering heredoc (will be closed on SIGINT) */
	g_stdin_backup = dup(STDIN_FILENO);
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_handler = handle_sigint_heredoc;
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

bool	was_interrupted(void)
{
	if (g_heredoc_interrupted)
	{
		g_heredoc_interrupted = 0;
		return (true);
	}
	return (false);
}

void	reset_heredoc_signal(void)
{
	g_heredoc_interrupted = 0;
	/* Restore stdin if it was saved */
	if (g_stdin_backup >= 0)
	{
		dup2(g_stdin_backup, STDIN_FILENO);
		close(g_stdin_backup);
		g_stdin_backup = -1;
	}
}

