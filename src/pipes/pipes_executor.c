/* #include "../../includes/minishell.h"

int	create_pipes(t_mini *mini)
{
	int	i;

	mini->pipe_count = count_pipes(mini->token);
	if (mini->pipe_count == 0)
		return (1);
	mini->pipes = malloc(sizeof(int *) * mini->pipe_count);
	if (!mini->pipes)
		return (0);
	i = 0;
	while (i < mini->pipe_count)
	{
		mini->pipes[i] = malloc(sizeof(int) * 2);
		if (!mini->pipes[i] || pipe(mini->pipes[i]) == -1)
		{
			while (--i >= 0)
			{
				close(mini->pipes[i][0]);
				close(mini->pipes[i][1]);
				free(mini->pipes[i]);
			}
			free (mini->pipes);
			return (0);
		}
		i++;
	}
	return (1);
}

int	execute_pipeline(t_mini *mini)
{
	int		cmd_count;
	int		i;
	t_token	*current;
	t_token	*cmd_start;
	int		status;

	i = 0;
	current = mini->token;
	cmd_start = current;
	cmd_count = mini->pipe_count + 1;
	mini->child_pids = malloc(sizeof(pid_t) * cmd_count);
	if (!mini->child_pids)
		return (1);
	while (i < cmd_count)
	{
		mini->child_pids[i] = fork();
		if (mini->child_pids[i] == 0)
			single_command(mini, cmd_start, i, cmd_count);
		else if (mini->child_pids[i] < 0)
			return (perror("fork"), 1);
		while (current && current->type != TOKEN_PIPE)
			current = current->next;
		if (current && current->type == TOKEN_PIPE)
		{
			current = current->next;
			cmd_start = current;
		}
		i++;
	}
	close_pipes(mini);
	i = 0;
	while (i < cmd_count)
	{
		waitpid(mini->child_pids[i], &status, 0);
		if (WIFEXITED(status))
			mini->exit_status = WEXITSTATUS(status);
		i++;
	}
	free (mini->child_pids);
	return (0);
}
 */