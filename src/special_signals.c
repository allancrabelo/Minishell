#include "minishell.h"

/**
 * @brief Waits for child process and maintains signal handling
 *
 * Temporarily ignores SIGINT during waitpid to prevent interruption,
 * then restores the main shell signal handler.
 *
 * @param pid Process ID to wait for
 * @param status Pointer to store exit status
 * @return void
 */
void	wait_update_main(int pid, int *status)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, status, 0);
	signal(SIGINT, sighandler);
}

/**
 * @brief Signal handler for heredoc operations
 *
 * Handles SIGINT during heredoc input, closes STDIN to exit
 * the readline loop and sets appropriate signal flag.
 *
 * @param signal The signal number received
 * @return void
 */
void	heredoc_sighandler(int signal)
{
	(void)signal;
	g_signal = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

/**
 * @brief Configures signal handling for heredoc operations
 *
 * Sets up SIGINT handler specifically for heredoc input collection.
 *
 * @return void
 */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sighandler);
}

/**
 * @brief Signal handler for pipe operations
 *
 * Handles SIGINT during pipe sequence execution, writes newline
 * without affecting the shell's main signal handling.
 *
 * @param signal The signal number received
 * @return void
 */
void	handle_ctrl_c_on_pipe(int signal)
{
	(void)signal;
	write(1, "\n", 1);
}
