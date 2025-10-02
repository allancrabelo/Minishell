#include "minishell.h"

int	ft_exit(t_mini *mini)
{
	write(1, "exit\n", 5);
	free_tokens(mini);
	//TODO: insert args handler
	//TODO: temos de adicionar com handler para por exemplo "exit a"
	exit(mini->exit_status);
}
