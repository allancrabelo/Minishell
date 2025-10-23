#include "minishell.h"

void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		if (redir->heredoc_delimeter)
			free(redir->heredoc_delimeter);
		free(redir);
		redir = tmp;
	}
}

void	free_export_list(t_export *lst)
{
	t_export	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst->next;
		free(lst->key);
		if (lst->value)
			free(lst->value);
		free(lst);
		lst = tmp;
	}
}

void	free_tokens(t_mini *mini)
{
	t_token	*cur;
	t_token	*tmp;

	cur = mini->token;
	mini->token = NULL;
	if (!cur)
		return ;
	while (cur)
	{
		tmp = cur->next;
		if (cur->data)
			free(cur->data);
		free(cur);
		cur = tmp;
	}
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
		ft_free_split(node->args);
	if (node->redir)
		free_redir(node->redir);
	free(node);
}

static void free_heredocs(t_heredoc **head)
{
	t_heredoc	*cur;
	t_heredoc	*tmp;

	if (!head || !*head)
		return;
	cur = *head;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		if (tmp->heredoc_delimeter)
		{
			free(tmp->heredoc_delimeter);
			tmp->heredoc_delimeter = NULL;
		}
		if (tmp->pipe_fd != -1)
		{
			close(tmp->pipe_fd);
			tmp->pipe_fd = -1;
		}
		free(tmp);
	}
	*head = NULL;
}

void	ft_free_extra(t_mini *mini, int ret, int exit_prog)
{
	if (mini->heredoc)
	{
		free_heredocs(&mini->heredoc);
		//heredoc_cleaner(&mini->heredoc); Causa free(): invalid pointer
	}
	if (mini->token)
		free_tokens(mini);
	if (mini->ast)
	{
		free_ast(mini->ast);
		mini->ast = NULL;
	}
	mini->exit_status = ret;
	if (exit_prog)
		exit(mini->exit_status);
	//ENOMEM
	//errno
}

int	ft_free_all(t_mini *mini, int ret, int exit_prog)
{
	ft_free_extra(mini, ret, 0);
	if (mini->export_list)
	{
		free_export_list(mini->export_list);
		mini->export_list = NULL;
	}
	if (mini->env_list)
		free_env_list(mini->env_list);
	if (mini->heredoc)
	{
		free_heredocs(&mini->heredoc);
		heredoc_cleaner(&mini->heredoc);
	}
	mini->exit_status = ret;
	if (exit_prog)
		exit(mini->exit_status);
	//ENOMEM
	//errno
	return (mini->exit_status);
}

