#include "../includes/minishell.h"


void	sighandler(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_init(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sighandler);
}

void	wait_update_main(int pid, int *status)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, status, 0);
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

