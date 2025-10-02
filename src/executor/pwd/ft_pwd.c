#include "minishell.h"

int ft_pwd(void)
{
	char	pwd[4046];

	if(getcwd(pwd, sizeof(pwd)) == NULL)
	{
		perror ("minishell: pwd");
		return (1);
	}
	printf("%s\n", pwd);
	return (0);
}
