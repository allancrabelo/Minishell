#include "../../../includes/minishell.h"

int	ft_env(t_mini *mini)
{
	int		i;
	int		j;
	char	**envp;

	i = 0;
	envp = mini->envp;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			write (1, &envp[i][j], 1);
			j++;
		}
		write (1, "\n", 1);
		i++;
	}
	mini->exit_status = 0; // nao e preciso, neste ponto ainda deve ser 0.
	return (0);
}
