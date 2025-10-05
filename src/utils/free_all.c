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
	if (mini->input)
		free(mini->input);
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

int	ft_free_all(t_mini *mini, int ret, int exit_prog)
{
	if (mini->token)
		free_tokens(mini);
	if (mini->ast)
		free_ast(mini->ast);
	if (mini->export_list)
		free_export_list(mini->export_list);
	mini->exit_status = ret;
	if (exit_prog)
		exit(mini->exit_status);
	return (mini->exit_status);
}