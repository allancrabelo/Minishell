#include "../../../includes/minishell.h"

char	flag_verification(char	**args, int *i)
{
	char	ret;
	size_t	j;

	ret = 0;
	while (args[*i])
	{
		j = 0;
		if (args[*i][j] != '-')
			break ;
		j++;
		if (args[*i][j] == '\0')
			break ;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] != '\0')
			break ;
		ret = 'n';
		(*i)++;
	}
	return (ret);
}

int	ft_echo(t_ast *node)
{
	int		i;
	char	flag;
	char	**args;

	i = 1;
	args = node->args;
	flag = flag_verification(args, &i);
	while (i < node->arg_count)
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (i != node->arg_count - 1)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
