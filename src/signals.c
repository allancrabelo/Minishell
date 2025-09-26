#include "../includes/minishell.h"

void	sighandler(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_init(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sighandler);
}

void	heredoc_sighandler(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	exit(130);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sighandler);
	signal(SIGQUIT, SIG_IGN);
}