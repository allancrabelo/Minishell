#include "minishell.h"

static void	execute_left_pipe(t_mini *mini, t_ast *left, int pipefd[2])
{
	setup_exec_signals();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	
	/* Process heredocs for this side of the pipe */
	process_heredocs(mini, left);
	if (mini->heredoc_signal)
		ft_free_all(mini, 130, 1);
	
	/* Don't call apply_redirections here - it will be called by execute_command */
	execute_ast_node(mini, left);
	ft_free_all(mini, mini->exit_status, 1);
}

static void	execute_right_pipe(t_mini *mini, t_ast *right, int pipefd[2])
{
	setup_exec_signals();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	
	/* Process heredocs for this side of the pipe */
	process_heredocs(mini, right);
	if (mini->heredoc_signal)
		ft_free_all(mini, 130, 1);
	
	/* Don't call apply_redirections here - it will be called by execute_command */
	execute_ast_node(mini, right);
	ft_free_all(mini, mini->exit_status, 1);
}

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

int	execute_pipe_node(t_mini *mini, t_ast *node)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	signal(SIGINT, handle_ctrl_c_on_pipe);
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
	if (WIFEXITED(status_right))
		mini->exit_status = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
	{
		if (WTERMSIG(status_right) == SIGINT)
			write(1, "\n", 1);
		mini->exit_status = 128 + WTERMSIG(status_right);
	}
	return (mini->exit_status);
}
