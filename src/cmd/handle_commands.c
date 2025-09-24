
#include "../../includes/minishell.h"
#include "../../includes/colors.h"

// Function to execute builtin commands
int	execute_builtin(t_mini *mini, t_token *cmd_token, char **argv)
{
	(void) argv;
	if (ft_strcmp(cmd_token->data, "echo") == 0)
		return (ft_echo(mini, cmd_token)); // You might want to pass argv here too
	else if (ft_strcmp(cmd_token->data, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd_token->data, "exit") == 0)
		return (ft_exit(mini));
	// TODO: Add other builtin implementations
	// if (ft_strcmp(cmd_token->data, "cd") == 0)
	//     return (ft_cd(mini, argv));
	// if (ft_strcmp(cmd_token->data, "export") == 0)
	//     return (ft_export(mini, argv));
	// if (ft_strcmp(cmd_token->data, "unset") == 0)
	//     return (ft_unset(mini, argv));
	else if (ft_strcmp(cmd_token->data, "env") == 0)
		return (ft_env(mini));
	return (0);
}

// Function to execute external commands using execve
int	execute_external_command(t_mini *mini, t_token *cmd_token, char **argv)
{
	pid_t	pid;
	int		exit_code;
	int		status;

	(void)cmd_token;
	pid = fork();
	if (pid == 0)
	{
		exit_code = execute_external(mini, argv);
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
int	execute_command(t_mini *mini, t_token *tokens)
{
	t_token	*cmd_token;
	char	**argv;
	int		result;

	cmd_token = find_first_command(tokens);
	if (!cmd_token)
	{
		printf("No command found\n");
		return (1);
	}

	// Build argument array from tokens
	argv = build_argv(cmd_token);
	if (!argv)
	{
		printf("Failed to build argument array\n");
		return (1);
	}

	// Check if it's a builtin command
	if (is_builtin_command(cmd_token->data))
	{
		result = execute_builtin(mini, cmd_token, argv);
	}
	else
	{
		// Execute as external command with execve
		result = execute_external_command(mini, cmd_token, argv);
	}

	// Free the argument array
	free_argv(argv);
	return (result);
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
	t_token	*cur;

	if (input[0] == '#') //TODO usar para teste
	{
		printf(BOLD SCYAN "%s\n" SRESET, input);
		return ;
	}
	ft_tokenizer(mini, input);
	if (mini->token == NULL)
		return ;
	// Identidificar o PIPE
	if (has_pipe(input))
	{
		if (!create_pipes(mini))
		{
			ft_putstr_fd("minishell: Pipe creation failed\n", 2);
			free_tokens(mini);
			return ;
		}
		execute_pipeline(mini);
	}
	else
	{
		cur = mini->token;
		execute_command(mini, cur);
	}
	free_tokens(mini);
}
