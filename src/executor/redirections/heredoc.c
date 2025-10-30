/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:22 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:45:23 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	heredoc_expand_safe(t_mini *mini)
{
	t_token	*cur;

	cur = NULL;
	if (mini->token)
		cur = mini->token;
	while (cur && cur->next)
		cur = cur->next;
	if (cur && cur->type == TOKEN_HEREDOC)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	heredoc_cleaner(t_heredoc **heredoc)
{
	t_heredoc	*cur;
	t_heredoc	*tmp;

	cur = *heredoc;
	while (cur)
	{
		tmp = cur->next;
		if (cur->heredoc_delimeter)
			free(cur->heredoc_delimeter);
		if (cur->pipe_fd > 0)
			close(cur->pipe_fd);
		free(cur);
		cur = tmp;
	}
	*heredoc = NULL;
}

static void	wait_heredoc(t_mini *mini, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal = 130;
		mini->heredoc_signal = 1;
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_signal = 130;
		mini->heredoc_signal = 1;
	}
}

static void	heredoc_fork(t_mini *mini, t_redir **redir)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		ft_free_all(mini, errno, 1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		setup_heredoc_signals();
		write_heredoc_to_pipe(pipe_fd[1], (*redir)->heredoc_delimeter, mini);
		ft_free_all(mini, g_signal, 1);
	}
	close(pipe_fd[1]);
	wait_heredoc(mini, pid);
	signal_init();
	if (mini->heredoc_signal)
	{
		close(pipe_fd[0]);
		return ;
	}
	(*redir)->fd = pipe_fd[0];
}

void	process_heredocs(t_mini *mini, t_ast *node)
{
	t_redir	*redir;

	if (!node)
		return ;
	if (node->type == NODE_CMD && node->redir)
	{
		redir = node->redir;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
				heredoc_fork(mini, &redir);
			redir = redir->next;
		}
	}
	process_heredocs(mini, node->left);
	if (mini->heredoc_signal)
		return ;
	process_heredocs(mini, node->right);
}
