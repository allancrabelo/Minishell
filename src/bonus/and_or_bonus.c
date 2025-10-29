#include "minishell.h"

int	execute_and_node(t_mini *mini, t_ast *node)
{
	execute_ast_node(mini, node->left);
	if (mini->exit_status == 0)
		execute_ast_node(mini, node->right);
	return (mini->exit_status);
}

int	execute_or_node(t_mini *mini, t_ast *node)
{
	execute_ast_node(mini, node->left);
	if (mini->exit_status != 0)
		execute_ast_node(mini, node->right);
	return (mini->exit_status);
}
