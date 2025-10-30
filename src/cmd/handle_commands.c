/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:03 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:44:04 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external_command(t_mini *mini, t_ast *node, t_redir *redirects)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_exec_signals();
		if (apply_redirections(redirects, mini) == -1)
			ft_free_all(mini, mini->exit_status, 1);
		if (mini->heredoc)
			heredoc_cleaner(&mini->heredoc);
		return (ft_free_all(mini, execute_external(mini, node), 1));
	}
	else if (pid < 0)
		return (perror("fork"), 1);
	return (signal_execute_setup(mini, pid));
}

int	redir_result_handler(t_mini *mini, t_ast *node, int result)
{
	int	stdin_backup;
	int	stdout_backup;

	if (backup_fd(&stdin_backup, &stdout_backup) == -1)
		return (1);
	result = apply_redirections(node->redir, mini);
	restore_fd(&stdin_backup, &stdout_backup);
	return (result);
}

int	execute_command(t_mini *mini, t_ast *node)
{
	int	result;

	result = 0;
	if (!node)
		return (0);
	if (!node->args || !node->args[0])
	{
		if (node->redir)
			return (redir_result_handler(mini, node, result));
		return (0);
	}
	if (node->args[0][0] == '\0')
	{
		print_command_error("", "command not found");
		return (127);
	}
	if (is_builtin_command(node->args[0]))
		return (execute_builtin(mini, node, node->redir));
	else
		return (execute_external_command(mini, node, node->redir));
}

int	handle_commands_initializer(t_mini *mini, char *input)
{
	if (tokenizer_init(mini, input))
		return (EXIT_FAILURE);
	if (build_ast(mini) != 0)
	{
		free(mini->input);
		free_tokens(mini);
		return (EXIT_FAILURE);
	}
	if (mini->input)
	{
		free(mini->input);
		mini->input = NULL;
	}
	process_heredocs(mini, mini->ast);
	return (EXIT_SUCCESS);
}

void	handle_commands(t_mini *mini, char *input)
{
	if (handle_commands_initializer(mini, input))
		return ;
	if (mini->heredoc_signal)
	{
		mini->exit_status = 130;
		mini->heredoc_signal = 0;
		heredoc_cleaner(&mini->heredoc);
	}
	else
	{
		execute_ast_node(mini, mini->ast);
		if (mini->heredoc)
			heredoc_cleaner(&mini->heredoc);
	}
	free_tokens(mini);
	if (mini->ast)
	{
		free_ast(mini->ast);
		mini->ast = NULL;
	}
	if (mini->heredoc)
		heredoc_cleaner(&mini->heredoc);
}
