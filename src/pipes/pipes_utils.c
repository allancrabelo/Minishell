#include "../../includes/minishell.h"

int	has_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|' && input[i + 1] != '|')
			return (1);
		i++;
	}
	return (0);
}

int	count_pipes(t_token	*token)
{
	t_token	*cur;
	int		count;

	count = 0;
	cur = token;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE)
			count++;
		cur = cur->next;
	}
	return (count);
}

void	setup_redirections(t_mini *mini,int i, int cmds)
{
	int	pipe_index;

	if (cmds == 1)
		return ;
	pipe_index = 0;
	while (pipe_index < mini->pipe_count)
	{
		if (pipe_index != i - 1)
			close(mini->pipes[pipe_index][0]);
		if (pipe_index != i)
			close(mini->pipes[pipe_index][1]);
		pipe_index++;
	}
	if (i > 0)
	{
		dup2(mini->pipes[i - 1][0], STDIN_FILENO);
		close(mini->pipes[i - 1][0]);
	}
	if (i < cmds - 1)
	{
		dup2(mini->pipes[i][1], STDOUT_FILENO);
		close(mini->pipes[i][1]);
	}
}

void	single_command(t_mini *mini,t_token *cmd_start,int i, int cmds)
{
	t_cmd	*cmd;
	int		exit_code;

	cmd = parse_command(cmd_start);
	if (!cmd)
		exit(EXIT_FAILURE);
	setup_redirections(mini, i, cmds);
	if (apply_redirections(cmd->redirects) == -1)
	{
		free_cmd(cmd);
		exit(1);
	}
	close_pipes(mini);
	if (!cmd->argv || !cmd->argv[0])
	{
		free_cmd(cmd);
		exit (EXIT_SUCCESS);
	}
	if (is_builtin_command(cmd->argv[0]))
	{
		exit_code = execute_builtin(mini, cmd_start, cmd->argv, cmd->redirects);
		free_cmd(cmd);
		exit(exit_code);
	}
	else
	{
		exit_code = execute_external(mini, cmd->argv);
		free_cmd(cmd);
		exit(exit_code);
	}
}

void	close_pipes(t_mini *mini)
{
	int	i;

	i = 0;
	if (!mini->pipes)
		return ;
	while (i < mini->pipe_count)
	{
		close(mini->pipes[i][0]);
		close(mini->pipes[i][1]);
		free(mini->pipes[i]);
		i++;
	}
	free(mini->pipes);
	mini->pipes = NULL;
}