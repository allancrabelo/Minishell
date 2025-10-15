#include "minishell.h"

int	apply_redirections(t_redir *redirections, t_mini *mini)
{
	t_redir	*cur;
	int		result;

	if (!redirections)
		return (0);
	cur = redirections;
	while (cur)
	{
		if (cur->type == TOKEN_REDIRECT_IN)
			result = redirect_in(cur, mini);
		else if (cur->type == TOKEN_REDIRECT_OUT)
			result = redirect_out(cur, mini);
		else if (cur->type == TOKEN_REDIRECT_APPEND)
			result = redirect_append(cur, mini);
		else
			result = -1;
		if (result == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}

void	restore_fd(int stdin_backup, int stdout_backup)
{
	if (stdin_backup != -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
	}
	if (stdout_backup != -1)
	{
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdout_backup);
	}
}

int	backup_fd(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		restore_fd(*stdin_backup, *stdout_backup);
		return (-1);
	}
	return (0);
}
