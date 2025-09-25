
#include "../../includes/minishell.h"
#include "../../includes/colors.h"

// Function to execute builtin commands
int	execute_builtin(t_mini *mini, t_token *cmd_token, char **argv, t_redir *redirects)
{
	int	result;
	int	stdin_backup;
	int	stdout_backup;

	(void) argv;
	if (backup_fd(&stdin_backup, &stdout_backup) == -1)
		return (1);
	if (apply_redirections(redirects) == -1)
	{
		restore_fd(stdin_backup, stdout_backup);
		return(1);
	}
	if (ft_strcmp(cmd_token->data, "echo") == 0)
		result = ft_echo(mini, cmd_token); // You might want to pass argv here too
	else if (ft_strcmp(cmd_token->data, "pwd") == 0)
		result = ft_pwd();
	else if (ft_strcmp(cmd_token->data, "exit") == 0)
		result = ft_exit(mini);
	// TODO: Add other builtin implementations
	// if (ft_strcmp(cmd_token->data, "cd") == 0)
	//     result = ft_cd(mini, argv));
	// if (ft_strcmp(cmd_token->data, "export") == 0)
	//     result = ft_export(mini, argv));
	// if (ft_strcmp(cmd_token->data, "unset") == 0)
	//     result = ft_unset(mini, argv));
	else if (ft_strcmp(cmd_token->data, "env") == 0)
		result = ft_env(mini);
	else
		result = 0;
	restore_fd(stdin_backup, stdout_backup);
	return (result);
}

// Function to execute external commands using execve
int	execute_external_command(t_mini *mini, t_token *cmd_token, char **argv, t_redir *redirects)
{
	pid_t	pid;
	int		exit_code;
	int		status;

	(void)cmd_token;
	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(redirects) == -1)
			exit(1);
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
	t_cmd	*cmd;
	int		result;
	int		stdin;
	int		stdout;

	cmd = parse_command(tokens);
	cmd_token = find_first_command(tokens);
	if (!cmd)
		return (printf("minishell: syntax error\n"), 1);
	if (!cmd_token || !cmd->argv || !cmd->argv[0])
		return (free_cmd(cmd), 0);
	if (backup_fd(&stdin, &stdout) == -1)
		return (free_cmd(cmd), 1);
	if (apply_redirections(cmd->redirects) == -1)
	{
		restore_fd(stdin, stdout);
		free_cmd(cmd);
		return (-1);
	}
	if (is_builtin_command(cmd_token->data))
		result = execute_builtin(mini, cmd_token, cmd->argv, cmd->redirects);
	else
		result = execute_external_command(mini, cmd_token, cmd->argv, cmd->redirects);
	restore_fd(stdin, stdout);
	free_cmd(cmd);
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
