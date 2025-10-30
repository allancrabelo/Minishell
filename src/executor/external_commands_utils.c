#include "minishell.h"

void	print_command_error(char *cmd, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

char	*find_command_path(t_mini *mini, char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*result;

	(void)mini;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	ft_free_split(paths);
	return (result);
}

int	validate_command(char *cmd, char *full_path)
{
	struct stat	path_stat;

	if (stat(full_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_command_error(cmd, "Is a directory");
		free(full_path);
		return (0);
	}
	if (access(full_path, X_OK) != 0)
	{
		print_command_error(cmd, "Permission denied");
		free(full_path);
		return (0);
	}
	return (1);
}

int	execute_external(t_mini *mini, t_ast *node)
{
	char	**env;
	char	**argv;
	char	*path;

	argv = node->args;
	path = find_command_path(mini, argv[0]);
	if (!path)
		return (handle_path_not_found(argv[0]));
	if (!validate_command(argv[0], path))
		return (126);
	env = env_list_to_array(mini);
	if (!env)
	{
		free(path);
		return (126);
	}
	execve(path, argv, env);
	ft_free_split(env);
	free(path);
	print_command_error(argv[0], strerror(errno));
	return (126);
}

int	handle_path_not_found(char *cmd)
{
	if (ft_strchr(cmd, '/'))
		print_command_error(cmd, "No such file or directory");
	else
		print_command_error(cmd, "command not found");
	return (127);
}
