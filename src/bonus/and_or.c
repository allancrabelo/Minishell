#include "minishell.h"

int	execute_and_node(t_mini *mini, t_ast *node)
{
	/* Process heredocs for left side and execute */
	process_heredocs(mini, node->left);
	if (mini->heredoc_signal)
	{
		mini->exit_status = 130;
		mini->heredoc_signal = 0;
		return (mini->exit_status);
	}
	execute_ast_node(mini, node->left);

	/* Only execute right side if left succeeded */
	if (mini->exit_status == 0)
	{
		/* Clean up left side heredocs and process right side */
		if (mini->heredoc)
			heredoc_cleaner(&mini->heredoc);
		process_heredocs(mini, node->right);
		if (mini->heredoc_signal)
		{
			mini->exit_status = 130;
			mini->heredoc_signal = 0;
			return (mini->exit_status);
		}
		execute_ast_node(mini, node->right);
	}
	return (mini->exit_status);
}

int	execute_or_node(t_mini *mini, t_ast *node)
{
	/* Process heredocs for left side and execute */
	process_heredocs(mini, node->left);
	if (mini->heredoc_signal)
	{
		mini->exit_status = 130;
		mini->heredoc_signal = 0;
		return (mini->exit_status);
	}
	execute_ast_node(mini, node->left);

	/* Only execute right side if left failed */
	if (mini->exit_status != 0)
	{
		/* Clean up left side heredocs and process right side */
		if (mini->heredoc)
			heredoc_cleaner(&mini->heredoc);
		process_heredocs(mini, node->right);
		if (mini->heredoc_signal)
		{
			mini->exit_status = 130;
			mini->heredoc_signal = 0;
			return (mini->exit_status);
		}
		execute_ast_node(mini, node->right);
	}
	return (mini->exit_status);
}
