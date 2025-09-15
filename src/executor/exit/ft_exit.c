#include "../../../includes/minishell.h"

int	ft_exit(t_mini *mini)
{
	write(1, "exit\n", 5); // Check outpu
	free_tokens(mini);
	//TODO insert args handler
	exit(0);
}