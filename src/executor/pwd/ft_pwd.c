#include "../../../includes/minishell.h"

int ft_pwd(void)
{
	char *cwd;
	char *pwd_env;

	pwd_env = getenv("PWD");
	if (pwd_env && *pwd_env)
	{
		printf("%s\n", pwd_env);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}
