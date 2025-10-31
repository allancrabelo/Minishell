/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_executor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:45 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 16:09:05 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes left command in pipe sequence
 *
 * Configures stdout to write to pipe, sets execution signals,
 * and processes left AST node. Terminates process after execution.
 *
 * @param mini Pointer to main shell structure
 * @param left Left AST node to execute
 * @param pipefd Pipe file descriptors [read, write]
 * @return void
 */
static void	execute_left_pipe(t_mini *mini, t_ast *left, int pipefd[2])
{
	setup_exec_signals();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	execute_ast_node(mini, left);
	ft_free_all(mini, mini->exit_status, 1);
}

/**
 * @brief Executes right command in pipe sequence
 *
 * Configures stdin to read from pipe, sets execution signals,
 * and processes right AST node. Terminates process after execution.
 *
 * @param mini Pointer to main shell structure
 * @param right Right AST node to execute
 * @param pipefd Pipe file descriptors [read, write]
 * @return void
 */
static void	execute_right_pipe(t_mini *mini, t_ast *right, int pipefd[2])
{
	setup_exec_signals();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	execute_ast_node(mini, right);
	ft_free_all(mini, mini->exit_status, 1);
}

/**
 * @brief Forks and executes left side of pipe operation
 *
 * Creates child process for left command in pipe sequence.
 * Handles fork failures by cleaning up pipe file descriptors.
 *
 * @param mini Pointer to main shell structure
 * @param node Left AST node of pipe operation
 * @param pipefd Pipe file descriptors [read, write]
 * @return pid_t Left child PID, -1 on fork failure
 */
static int	handle_left_fork(t_mini *mini, t_ast *node, int pipefd[2])
{
	pid_t	pid_left;

	pid_left = fork();
	if (pid_left == 0)
		execute_left_pipe(mini, node->left, pipefd);
	else if (pid_left < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (perror("fork left"), -1);
	}
	return (pid_left);
}

/**
 * @brief Forks and executes right side of pipe operation
 *
 * Creates child process for right command in pipe sequence.
 * Handles fork failures by cleaning up and terminating left process.
 *
 * @param mini Pointer to main shell structure
 * @param node Right AST node of pipe operation
 * @param pipefd Pipe file descriptors [read, write]
 * @param pid_left Process ID of left side for cleanup on failure
 * @return pid_t Right child PID, -1 on fork failure
 */
static int	handle_right_fork(t_mini *mini, t_ast *node, int pipefd[2],
						pid_t pid_left)
{
	pid_t	pid_right;

	pid_right = fork();
	if (pid_right == 0)
		execute_right_pipe(mini, node->right, pipefd);
	else if (pid_right < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		kill(pid_left, SIGTERM);
		return (perror("fork right"), -1);
	}
	return (pid_right);
}

/**
 * @brief Executes pipe node by forking left and right processes
 *
 * Creates pipe and forks two child processes for left and right sides
 * of the pipe. Manages file descriptors and waits for both processes
 * to complete before updating exit status.
 *
 * @param mini Pointer to main shell structure
 * @param node AST node representing pipe operation
 * @return int Exit status of the rightmost command in pipe
 */
int	execute_pipe_node(t_mini *mini, t_ast *node)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid_left = handle_left_fork(mini, node, pipefd);
	if (pid_left == -1)
		return (1);
	pid_right = handle_right_fork(mini, node, pipefd, pid_left);
	if (pid_right == -1)
		return (1);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	signal_init();
	return (signals_pipe_update(status_right, status_left, mini));
}
