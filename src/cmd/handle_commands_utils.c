/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:02 by mqueiros          #+#    #+#             */
/*   Updated: 2025/11/05 09:04:54 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_ast_node(t_mini *mini, t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		execute_pipe_node(mini, node);
		return ;
	}
	if (node->type == NODE_AND)
	{
		execute_and_node(mini, node);
		return ;
	}
	if (node->type == NODE_OR)
	{
		execute_or_node(mini, node);
		return ;
	}
	if (node->type == NODE_CMD)
	{
		mini->exit_status = execute_command(mini, node);
		return ;
	}
	execute_ast_node(mini, node->left);
	execute_ast_node(mini, node->right);
}

int	builtin(t_mini *mini, t_ast *node)
{
	int	result;

	if (ft_strcmp(node->args[0], "echo") == 0)
		result = ft_echo(node);
	else if (ft_strcmp(node->args[0], "env") == 0)
		result = ft_env(mini);
	else if (ft_strcmp(node->args[0], "exit") == 0)
		result = ft_exit(mini, node);
	else if (ft_strcmp(node->args[0], "cd") == 0)
		result = ft_cd(mini, node);
	else if (ft_strcmp(node->args[0], "pwd") == 0)
		result = ft_pwd(mini);
	else if (ft_strcmp(node->args[0], "export") == 0)
		result = ft_export(mini, node);
	else if (ft_strcmp(node->args[0], "unset") == 0)
		result = ft_unset(mini, node);
	else
		result = 0;
	return (result);
}

int	execute_builtin(t_mini *mini, t_ast *node, t_redir *redir)
{
	int	result;
	int	stdin_backup;
	int	stdout_backup;

	if (backup_fd(&stdin_backup, &stdout_backup) == -1)
		return (1);
	if (apply_redirections(redir, mini) == -1)
	{
		restore_fd(&stdin_backup, &stdout_backup);
		return (1);
	}
	result = builtin(mini, node);
	restore_fd(&stdin_backup, &stdout_backup);
	return (result);
}

int	signal_execute_setup(t_mini *mini, pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal_init();
	if (WIFEXITED(status))
	{
		mini->exit_status = WEXITSTATUS(status);
		return (WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		mini->exit_status = 128 + WTERMSIG(status);
		return (mini->exit_status);
	}
	return (g_signal);
}

int	tokenizer_init(t_mini *mini, char *input)
{
	if (ft_tokenizer(mini, input) != 0)
	{
		if (mini->input)
		{
			free(mini->input);
			mini->input = NULL;
		}
		return (EXIT_FAILURE);
	}
	if (mini->token == NULL)
	{
		if (mini->input)
		{
			free(mini->input);
			mini->input = NULL;
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
