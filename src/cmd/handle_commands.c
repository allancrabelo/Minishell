#include "minishell.h"

void	execute_ast_node(t_mini *mini, t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		execute_pipe_node(mini, node);
		return ;
	}
	if (node->type == NODE_AND)
	{
		execute_and_node(mini, node);
		return ;
	}
	if (node->type == NODE_OR)
	{
		execute_or_node(mini, node);
		return ;
	}
	if (node->type == NODE_CMD)
	{
		mini->exit_status = execute_command(mini, node);
		return ;
	}
	execute_ast_node(mini, node->left);
	execute_ast_node(mini, node->right);
}

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	builtin(t_mini *mini, t_ast *node)
{
	int	result;

	if (ft_strcmp(node->args[0], "echo") == 0)
		result = ft_echo(node);
	else if (ft_strcmp(node->args[0], "env") == 0)
		result = ft_env(mini);
	else if (ft_strcmp(node->args[0], "exit") == 0)
		result = ft_exit(mini, node);
	else if (ft_strcmp(node->args[0], "cd") == 0)
		result = ft_cd(mini, node);
	else if (ft_strcmp(node->args[0], "pwd") == 0)
		result = ft_pwd(mini);
	else if (ft_strcmp(node->args[0], "export") == 0)
		result = ft_export(mini, node);
	else if (ft_strcmp(node->args[0], "unset") == 0)
		result = ft_unset(mini, node);
	else
		result = 0;
	return (result);
}

// Function to execute builtin commands
int	execute_builtin(t_mini *mini, t_ast *node, t_redir *redir)
{
	int	result;
	int	stdin_backup;
	int	stdout_backup;

	(void)mini;
	if (backup_fd(&stdin_backup, &stdout_backup) == -1)
		return (1);
	if (apply_redirections(redir, mini) == -1)
	{
		restore_fd(&stdin_backup, &stdout_backup);
		return (1);
	}
	result = builtin(mini, node);
	restore_fd(&stdin_backup, &stdout_backup);
	return (result);
}

int	execute_external_command(t_mini *mini, t_ast *node, t_redir *redirects)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		setup_exec_signals();
		if (apply_redirections(redirects, mini) == -1)
			ft_free_all(mini, mini->exit_status, 1);
		if (mini->heredoc)
			heredoc_cleaner(&mini->heredoc);
		return (ft_free_all(mini, execute_external(mini, node->args), 1));
	}
	else if (pid < 0)
		return (perror("fork"), 1);
	else
	{
		setup_exec_signals();
		waitpid(pid, &status, 0);
		signal_init();
		if (WIFEXITED(status))
		{
			mini->exit_status = WEXITSTATUS(status);
			return (WEXITSTATUS(status));
		}
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			mini->exit_status = 128 + WTERMSIG(status);
			return (mini->exit_status);
		}
		return (g_signal);
	}
}

int	execute_command(t_mini *mini, t_ast *node)
{
	if (!node)
		return (0);

	/* Handle standalone heredoc (no command, only redirection) */
	if (!node->args || !node->args[0])
	{
		if (node->redir && node->redir->type == TOKEN_HEREDOC)
		{
			/* Standalone heredoc - just apply the redirection */
			int	stdin_backup;
			int	stdout_backup;
			int	result;

			if (backup_fd(&stdin_backup, &stdout_backup) == -1)
				return (1);
			result = apply_redirections(node->redir, mini);
			restore_fd(&stdin_backup, &stdout_backup);
			return (result == -1 ? 1 : 0);
		}
		return (0);
	}

	if (node->args[0][0] == '\0')
	{
		print_command_error("", "command not found");
		return (127);
	}
	if (is_builtin_command(node->args[0]))
		return (execute_builtin(mini, node, node->redir));
	else
		return (execute_external_command(mini, node, node->redir));
}

static int tokenizer_init(t_mini *mini, char *input)
{
	if (ft_tokenizer(mini, input) != 0)
	{
		if (mini->input)
		{
			free(mini->input);
			mini->input = NULL;
		}
		return (EXIT_FAILURE);
	}
	if (mini->token == NULL)
	{
		if (mini->input)
		{
			free(mini->input);
			mini->input = NULL;
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	handle_commands(t_mini *mini, char *input)
{
	if (tokenizer_init(mini, input))
		return ;
	if (build_ast(mini) != 0)
	{
		free(mini->input);
		free_tokens(mini);
		return ;
	}
	if (mini->input)
	{
		free(mini->input);
		mini->input = NULL;
	}
	/* Only process heredocs for immediate commands, not PIPE/AND/OR */
	if (mini->ast->type != NODE_AND && mini->ast->type != NODE_OR && mini->ast->type != NODE_PIPE)
	{
		process_heredocs(mini, mini->ast);
		if (mini->heredoc_signal)
		{
			mini->exit_status = 130;
			mini->heredoc_signal = 0;
			heredoc_cleaner(&mini->heredoc);
		}
		else
		{
			execute_ast_node(mini, mini->ast);
			/* Clean up heredocs after successful execution */
			if (mini->heredoc)
				heredoc_cleaner(&mini->heredoc);
		}
	}
	else
	{
		/* For PIPE/AND/OR nodes, heredocs are processed within their execution */
		execute_ast_node(mini, mini->ast);
	}
	free_tokens(mini);
	if (mini->ast)
	{
		free_ast(mini->ast);
		mini->ast = NULL;
	}
	if (mini->heredoc)
		heredoc_cleaner(&mini->heredoc);
}
