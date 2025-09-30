#include "../../../includes/minishell.h"

int	ft_cd(t_mini *mini, t_ast *node)
{
	(void)mini;//TODO:
	if (!node->args[1])
		return(chdir("$HOME"));
	return (chdir(node->args[1]));
}