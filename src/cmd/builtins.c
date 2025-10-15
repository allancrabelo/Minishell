#include "minishell.h"

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
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
