#include "minishell.h"

char	*join_path_cmd(char *path, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	return (full_path);
}

char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
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
	path_env = ft_getenv("PATH", mini);
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
	char	**argv;
	char	*path;

	argv = node->args;
	path = find_command_path(mini, argv[0]);
	if (!path)
	{
		if (ft_strchr(argv[0], '/'))
			print_command_error(argv[0], "No such file or directory");
		else
			print_command_error(argv[0], "command not found");
		return (127);
	}
	if (!validate_command(argv[0], path))
		return (126);
	if (mini->env_mod)
		if (update_envp(mini) == EXIT_FAILURE)
			return (free(path), 1);
	execve(path, argv, mini->envp);
	free(path);
	print_command_error(argv[0], strerror(errno));
	return (126);
}
