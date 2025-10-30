#include "minishell.h"

/**
 * @brief Signal handler for interactive mode
 *
 * Handles SIGINT (Ctrl-C) in the main shell loop. Sets global signal flag,
 * displays new prompt, and preserves current input line.
 *
 * @param signal The signal number received
 * @return void
 */
void	sighandler(int signal)
{
	g_signal = 128 + signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * @brief Initializes signal handlers for the shell
 *
 * Sets up SIGINT (Ctrl-C) handling for interactive mode and
 * ignores SIGQUIT (Ctrl-\) in the main shell process.
 *
 * @return void
 */
void	signal_init(void)
{
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Signal handler for child processes during execution
 *
 * Handles SIGINT (Ctrl-C) in child processes. Sets appropriate
 * exit status and displays newline without affecting parent shell.
 *
 * @param signal The signal number received
 * @return void
 */
void	handle_exec_ctrl_c(int signal)
{
	(void)signal;
	g_signal = 130;
}

/**
 * @brief Signal handler for SIGQUIT in child processes
 *
 * Handles Ctrl-\ (SIGQUIT) during command execution. Sets the
 * appropriate exit status and displays quit message.
 *
 * @param signal The signal number received
 * @return void
 */
static void	handle_exec_ctrl_bslash(int signal)
{
	(void)signal;
	g_signal = 131;
	printf("Quit (core dumped)\n");
}

/**
 * @brief Configures signal handling for child processes
 *
 * Sets up signal handlers for SIGINT and SIGQUIT specifically
 * for commands being executed (child processes).
 * Ignores SIGPIPE to allow normal cleanup on broken pipes.
 *
 * @return void
 */
void	setup_exec_signals(void)
{
	signal(SIGINT, handle_exec_ctrl_c);
	signal(SIGQUIT, handle_exec_ctrl_bslash);
	signal(SIGPIPE, SIG_IGN);
}
