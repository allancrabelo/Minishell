#include "minishell.h"

int	ft_cd(t_mini *mini, t_ast *node)
{
	char	*target_dir;

	(void)mini;
	if (!node->args[1])
		return (1) ; // TODO: O que vamos tratar com o cd sem argumento????
	target_dir = node->args[1];
	if (chdir(target_dir) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(target_dir, 2);
		ft_putstr_fd(": ", 2);
		if (errno == ENOENT)
			ft_putstr_fd("No such file or directory\n", 2);
		else if (errno == EACCES)
			ft_putstr_fd("Permission denied\n", 2);
		else
			ft_putstr_fd("Not a directory\n", 2);
		return (1);
	}
	return (0);
}