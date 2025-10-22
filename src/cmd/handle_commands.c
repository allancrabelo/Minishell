#include "minishell.h"
#include "colors.h"

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
		restore_fd(stdin_backup, stdout_backup);
		return (1);
	}
	result = builtin(mini, node);
	restore_fd(stdin_backup, stdout_backup);
	return (result);
}

int	execute_external_command(t_mini *mini, t_ast *node, t_redir *redirects)
{
	pid_t	pid;
	int		status;

	pid = fork();
	setup_exec_signals();
	if (pid == 0)
	{
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
		waitpid(pid, &status, 0);
		mini->exit_status = status;
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (g_signal);
	}
}

int	execute_command(t_mini *mini, t_ast *node)
{
	if (!node || !node->args || (!node->args[0]
			&& node->redir->type != TOKEN_HEREDOC))
		return (0);
	if (node->args[0] && node->args[0][0] == '\0')
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
		free_tokens(mini);
		return ;
	}
	if (mini->input)
	{
		free(mini->input);
		mini->input = NULL;
	}
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
	free_tokens(mini);
	if (mini->ast)
	{
		free_ast(mini->ast);
		mini->ast = NULL;
	}
	if (mini->heredoc)
		heredoc_cleaner(&mini->heredoc);
}
