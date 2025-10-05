#include "minishell.h"

static int	get_num_256(char *str, int *res)
{
	int	val;
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	val = ft_atoi(str);
	*res = ((int)(val % 256) + 256) % 256;
	return (1);
}

int	ft_exit(t_mini *mini, t_ast *node)
{
	int	res;

	write(1, "exit\n", 5);
	res = 0;
	if (node->arg_count >= 2 && get_num_256(node->args[1], &res))
	{
		if (node->arg_count == 2)
			mini->exit_status = res;
		else
			return (ft_putstr_fd("minishell: exit: too many arguments\n", 2)
				, 1);
	}
	else if (node->arg_count >= 2)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		mini->exit_status = 2;
	}
	free_tokens(mini);
	free_ast(mini->ast);
	free_export_list(mini->export_list);
	exit(mini->exit_status);
}
