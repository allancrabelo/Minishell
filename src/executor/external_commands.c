#include "../../includes/minishell.h"

char	*find_command_path(t_mini *mini, char *cmd)
{
	char	*path_env;
	char	**paths;
	int		i;
	char	*path;
	char	*full_path;

	(void)mini;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free (path);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free (full_path);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

void	print_command_error(char *cmd, char *error)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(":", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

int	execute_external(t_mini *mini, char **argv)
{
	char *full_path;

	full_path = find_command_path(mini, argv[0]);
	if (!full_path)
	{
		print_command_error(argv[0], "command not found");
		return (127);
	}
	execve(full_path, argv, mini->envp);
	print_command_error(argv[0], strerror(errno));
	free(full_path);
	return (126);
}