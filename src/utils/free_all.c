#include "minishell.h"

/**
 * @brief Frees the entire export list memory
 *
 * Iterates through the export linked list and frees all allocated
 * memory for keys, values, and node structures.
 *
 * @param lst Pointer to the head of the export list
 * @return void
 */
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

/**
 * @brief Frees all tokens from the shell structure
 *
 * Iterates through the token linked list and frees all allocated
 * memory for token data and structures. Resets the token
 * pointer to NULL.
 *
 * @param mini Pointer to the main shell structure
 * @return void
 */
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

/**
 * @brief Recursively frees the abstract syntax tree (AST)
 *
 * Performs post-order traversal to free all AST nodes, including
 * command arguments and redirection lists.
 *
 * @param node Root node of the AST to free
 * @return void
 */
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

/**
 * @brief Frees temporary shell resources and optionally exits
 *
 * Cleans up heredocs, tokens, and AST structures. Updates exit status
 * and can terminate the program if requested.
 *
 * @param mini Pointer to the main shell structure
 * @param ret Exit status to set
 * @param exit_prog Flag to terminate program (1) or continue (0)
 * @return void
 */
void	ft_free_extra(t_mini *mini, int ret, int exit_prog)
{
	if (mini->heredoc)
		free_heredocs(&mini->heredoc);
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
}

/**
 * @brief Comprehensive cleanup of all shell resources
 *
 * Frees all allocated memory including temporary structures,
 * environment lists, export lists, and heredocs. Optionally
 * terminates the program with specified exit status.
 *
 * @param mini Pointer to the main shell structure
 * @param ret Exit status to set
 * @param exit_prog Flag to terminate program (1) or return status (0)
 * @return int Final exit status
 */
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
	if (mini->envp)
		ft_free_split(mini->envp);
	if (mini->pwd)
		free(mini->pwd);
	if (mini->old_pwd)
		free(mini->old_pwd);
	mini->exit_status = ret;
	if (exit_prog)
		exit(mini->exit_status);
	return (mini->exit_status);
}
