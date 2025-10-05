#include "minishell.h"

int	ft_pwd(t_mini *mini)
{
	char	pwd[4096];

	if (mini->ast && mini->ast->args && mini->ast->args[1])
	{
		if (mini->ast->args[1][0] == '-')
		{
			ft_putstr_fd("minishell: pwd: ", 2);
			ft_putstr_fd(mini->ast->args[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			ft_putstr_fd("pwd: usage: pwd with no options\n", 2);
			return (2);
		}
	}
	if (getcwd(pwd, sizeof(pwd)) == NULL)
	{
		printf("%s\n", mini->pwd);
		return (0);
	}
	printf("%s\n", pwd);
	return (0);
}
