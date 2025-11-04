/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_executor_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:19 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	step_through_redir(t_mini *mini, t_redir *cur, t_redir *last_heredoc)
{
	int	result;

	while (cur)
	{
		if (cur->type == TOKEN_REDIRECT_IN)
			result = redirect_in(cur, mini);
		else if (cur->type == TOKEN_REDIRECT_OUT)
			result = redirect_out(cur, mini);
		else if (cur->type == TOKEN_REDIRECT_APPEND)
			result = redirect_append(cur, mini);
		else if (cur->type == TOKEN_HEREDOC)
		{
			if (cur == last_heredoc)
				result = redirect_heredoc(cur, mini);
			else
				result = 0;
		}
		else
			result = -1;
		if (result == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}

int	apply_redirections(t_redir *redirections, t_mini *mini)
{
	t_redir	*cur;
	t_redir	*last_heredoc;

	if (!redirections)
		return (0);
	last_heredoc = NULL;
	cur = redirections;
	while (cur)
	{
		if (cur->type == TOKEN_HEREDOC)
			last_heredoc = cur;
		cur = cur->next;
	}
	cur = redirections;
	return (step_through_redir(mini, cur, last_heredoc));
}

void	restore_fd(int *stdin_backup, int *stdout_backup)
{
	if (*stdin_backup != -1)
	{
		dup2(*stdin_backup, STDIN_FILENO);
		close(*stdin_backup);
		*stdin_backup = -1;
	}
	if (*stdout_backup != -1)
	{
		dup2(*stdout_backup, STDOUT_FILENO);
		close(*stdout_backup);
		*stdout_backup = -1;
	}
}

int	backup_fd(int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		restore_fd(stdin_backup, stdout_backup);
		return (-1);
	}
	return (0);
}
