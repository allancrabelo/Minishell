#include "../../includes/minishell.h"
#include "../../includes/colors.h"

void	execute_ast_node(t_mini *mini, t_ast *node)
{
	if (!node)
		return ;

	if (node->type == 0)
	{
		execute_pipe_node(mini, node);
		return;
	}
	if (node->type == 1)
	{
		execute_command(mini, node);
		return ;
	}
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

// Function to execute builtin commands
int	execute_builtin(t_mini *mini, t_ast *node, t_redir *redir)
{
	int	result;
	int	stdin_backup;
	int	stdout_backup;

	(void)mini;
	if (backup_fd(&stdin_backup, &stdout_backup) == -1)
		return (1);
	if (apply_redirections(redir) == -1)
	{
		mini->exit_status = 1;
		restore_fd(stdin_backup, stdout_backup);
		return (1);
	}
	if (ft_strcmp(node->args[0], "echo") == 0)
		result = ft_echo(node);
	else if (ft_strcmp(node->args[0], "env") == 0)
		result = ft_env(mini);
	else
		result = 0;
/* 	else if (ft_strcmp(mini->ast, "pwd") == 0)
		result = ft_pwd();
	else if (ft_strcmp(mini->ast, "exit") == 0)
		result = ft_exit(mini);
	// TODO: Add other builtin implementations
	// if (ft_strcmp(mini->ast, "cd") == 0)
	//     result = ft_cd(mini, argv));
	// if (ft_strcmp(mini->ast, "export") == 0)
	//     result = ft_export(mini, argv));
	// if (ft_strcmp(mini->ast, "unset") == 0)
	//     result = ft_unset(mini, argv));

	else
		result = 0; */
	restore_fd(stdin_backup, stdout_backup);
	return (result);
}

// Function to execute external commands using execve
int	execute_external_command(t_mini *mini, t_ast *node, t_redir *redirects)
{
	pid_t	pid;
	int		exit_code;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(redirects) == -1)
			exit(1);
		exit_code = execute_external(mini, node->args);
		exit(exit_code);
	}
	else if (pid < 0)
		return (perror("fork"), 1);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
}

// Main command execution function
int execute_command(t_mini *mini, t_ast *node)
{
	if (!node || !node->args || !node->args[0])
		return (0);
	if (is_builtin_command(node->args[0]))
		return (execute_builtin(mini, node, node->redir));
	else
		return (execute_external_command(mini, node, node->redir));
}

void	free_tokens(t_mini *mini)
{
	t_token	*cur;
	t_token	*tmp;

	cur = mini->token;
	mini->token = NULL;
	while (cur)
	{
		tmp = cur->next;
		free(cur->data);
		free(cur);
		cur = tmp;
	}
	if (mini->input)
		free(mini->input);
}

void	handle_commands(t_mini *mini, char *input)
{
	if (input[0] == '#') //TODO usar para teste
	{
		printf(BOLD SCYAN "%s\n" SRESET, input);
		return ;
	}
	ft_tokenizer(mini, input);
	if (mini->token == NULL)
		return ;
	execute_ast_node(mini, mini->ast);
	free_tokens(mini);
	if (mini->ast)
	{
		free_ast(mini->ast);
		mini->ast = NULL;
	}
}
