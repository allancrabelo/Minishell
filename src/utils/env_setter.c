#include "minishell.h"

/**
 * @brief Sets PWD environment variable from current working directory
 * 
 * Retrieves current working directory and creates PWD environment variable
 * string. Handles getcwd failures and memory allocation.
 * 
 * @param cwd Buffer for current working directory
 * @param total_size
 * @param new_envp Environment array to populate
 * @param i Current index in environment array
 * @return int Updated index on success, EXIT_FAILURE on error
 */
int	set_getcwd(char *cwd, int total_size, char **new_envp, int i)
{
	i = 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		total_size = ft_strlen("PWD=") + ft_strlen(cwd) + 1;
		new_envp[i] = malloc(total_size);
		if (new_envp[i])
		{
			ft_strlcpy(new_envp[i], "PWD=", total_size);
			ft_strlcat(new_envp[i], cwd, total_size);
			i++;
		}
	}
	else
		return (perror("getcwd"), EXIT_FAILURE);
	return (i);
}
