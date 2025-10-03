#include "minishell.h"

int	execute_and_node(t_mini *mini, t_ast *node)
{
	mini->exit_status = execute_command(mini, node->left);
	if (mini->exit_status == 0)
		mini->exit_status = execute_command(mini, node->right);
	return (mini->exit_status);
}

int	execute_or_node(t_mini *mini, t_ast *node)
{
	mini->exit_status = execute_command(mini, node->left);
	if (mini->exit_status != 0)
		mini->exit_status = execute_command(mini, node->right);
	return (mini->exit_status);
}
