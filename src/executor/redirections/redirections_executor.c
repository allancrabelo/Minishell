#include "../../../includes/minishell.h"

int	apply_redirections(t_redir *redirections)
{
	t_redir	*cur;
	int		result;

	if (!redirections)
		return (0);
	cur = redirections;
	while (cur)
	{
		if (cur->type == TOKEN_REDIRECT_IN)
			result = redirect_in(cur);
		else if (cur->type == TOKEN_REDIRECT_OUT)
			result = redirect_out(cur);
		else if (cur->type == TOKEN_REDIRECT_APPEND)
			result = redirect_append(cur);
		else
			result = -1;
		if (result == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}

int	backup_fd(int *stdin, int *stdout)
{
	*stdin = dup(STDIN_FILENO);
	*stdout = dup(STDOUT_FILENO);
	if (*stdin == -1 || *stdout == -1)
	{
		// restore_fd(*stdin, *stdout);
		return (-1);
	}
	return (0);
}
