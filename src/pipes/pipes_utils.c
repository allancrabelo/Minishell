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
	if (cmds == 1)
		return ;
	if (i > 0)
	{
		dup2(mini->pipes[i - 1][0], STDIN_FILENO);
		close(mini->pipes[i - 1][0]);
	}
	if (i > cmds - 1)
	{
		dup2(mini->pipes[i][1], STDOUT_FILENO);
		close(mini->pipes[i][1]);
	}
}

void	single_command(t_mini *mini,t_token *cmd_start,int i, int cmds)
{
	t_cmd	*cmd;
	char	**argv;
	int		exit_code;

	cmd = parse_command(cmd_start);
	if (!cmd)
		exit(EXIT_FAILURE);
	setup_redirections(mini, i, cmds);
	close_pipes(mini);
	argv = cmd->argv;
	if (!argv || !argv[0])
		exit (EXIT_SUCCESS);
	if (is_builtin_command(argv[0]))
	{
		exit_code = execute_builtin(mini, cmd_start, argv);
		free_cmd(cmd);
		exit(exit_code);
	}
	else
	{
		//tenho de tratar comandos externos???????????????
		free_cmd(cmd);
		exit(0);
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