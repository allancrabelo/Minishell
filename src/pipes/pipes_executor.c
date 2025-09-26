#include "../../includes/minishell.h"

static void	execute_left_pipe(t_mini *mini, t_ast *left, int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	if (apply_redirections(left->redir) == -1)
	{
		mini->exit_status = 1;
		exit(1);
	}
	execute_ast_node(mini, left);
	exit(mini->exit_status);
}

static void	execute_right_pipe(t_mini *mini, t_ast *right, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	if (apply_redirections(right->redir) == -1)
	{
		mini->exit_status = 1;
		exit(1);
	}
	execute_ast_node(mini, right);
	exit(mini->exit_status);
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
	pid_left = fork();
	if (pid_left == 0)
		execute_left_pipe(mini, node->left, pipefd);
	else if (pid_left < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (perror("fork left"), 1);
	}
	pid_right = fork();
	if (pid_right == 0)
		execute_right_pipe(mini, node->right, pipefd);
	else if (pid_right < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		kill(pid_left, SIGTERM);
		return (perror("fork right"), 1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	if (WIFEXITED(status_right))
		mini->exit_status = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
		mini->exit_status = 128 + WTERMSIG(status_right);
	return (mini->exit_status);
}
