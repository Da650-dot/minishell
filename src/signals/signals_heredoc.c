#include "../includes/minishell.h"

static void	handle_sigint_heredoc(int signum)
{
	(void)signum;
	g_signal = SIGINT;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handle_sigint_heredoc;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

bool	was_interrupted(void)
{
	return (g_signal == SIGINT);
}
