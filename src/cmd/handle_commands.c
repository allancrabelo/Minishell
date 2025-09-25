#include "../../includes/minishell.h"
#include "../../includes/colors.h"

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

int	execute_builtin(t_mini *mini, char **argv, t_redir *redir)
{
	int	result;

	(void) argv;
	(void) redir;
	if (ft_strcmp(mini->ast->args[0], "echo") == 0)
		result = ft_echo(mini->ast);
	return (0);
}

int	execute_external_command(t_mini *mini, char **argv, t_redir *redirects)
{
	pid_t	pid;
	int		exit_code;
	int		status;

	(void)redirects;
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


int	execute_command(t_mini *mini)
{
	t_ast	*cmd;
	int		result;

	cmd = mini->ast;
	if (is_builtin_command(cmd->args[0]))
		result = execute_builtin(mini, cmd->args, cmd->redir);
	else
		result = execute_external_command(mini, cmd->args, mini->ast->redir);
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
	if (input[0] == '#') //TODO usar para teste
	{
		printf(BOLD SCYAN "%s\n" SRESET, input);
		return ;
	}
	ft_tokenizer(mini, input);
	if (mini->token == NULL)
		return ;
	execute_command(mini);
	free_tokens(mini);
}
