#include "../../../includes/minishell.h"

int	ft_pwd(void)
{
	char	cur_path[FD_MAX];

	if (getcwd(cur_path, FD_MAX) != NULL)
		printf("%s\n", cur_path);
	else
		return (1); // Handle else
	return (0);
}
