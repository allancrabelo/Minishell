#include "minishell.h"

void	handle_exec_ctrl_c(int signal)
{
	(void)signal;
	g_signal = 130;
	write(1, "\n", 1);
}

static void	handle_exec_ctrl_bslash(int signal)
{
	(void)signal;
	g_signal = 131;
	printf("Quit (core dumped)\n");
}

void	setup_exec_signals(void)
{
	signal(SIGINT, handle_exec_ctrl_c);
	signal(SIGQUIT, handle_exec_ctrl_bslash);
}
