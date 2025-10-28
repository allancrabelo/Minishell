#include "minishell.h"

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
		if (access(cmd, F_OK) == 0)
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
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

char	**env_list_to_array(t_mini *mini)
{
	t_env	*current;
	char	**env_array;
	int		count;
	int		i;
	char	*temp;

	count = 0;
	current = mini->env_list;
	while (current)
	{
		if (current->value != NULL)
			count++;
		current = current->next;
	}
	env_array = malloc((count + 1) * sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	current = mini->env_list;
	while (current && i < count)
	{
		if (current->value != NULL)
		{
			temp = ft_strjoin(current->key, "=");
			env_array[i] = ft_strjoin(temp, current->value);
			free(temp);
			if (!env_array[i])
			{
				ft_free_split(env_array);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

int	execute_external(t_mini *mini, t_ast *node)
{
	char		*full_path;
	char		**env_array;
	struct stat	path_stat;
	char		**argv;

	argv = node->args;
	full_path = find_command_path(mini, argv[0]);
	if (!full_path)
	{
		if (ft_strchr(argv[0], '/'))
			print_command_error(argv[0], "No such file or directory");
		else
			print_command_error(argv[0], "command not found");
		return (127);
	}
	if (stat(full_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		print_command_error(argv[0], "Is a directory");
		free(full_path);
		return (126);
	}
	if (access(full_path, X_OK) != 0)
	{
		print_command_error(argv[0], "Permission denied");
		free(full_path);
		return (126);
	}
	env_array = env_list_to_array(mini);
	if (!env_array)
	{
		free(full_path);
		return (126);
	}
	execve(full_path, argv, env_array);
	ft_free_split(env_array);
	free(full_path);
	print_command_error(argv[0], strerror(errno));
	return (126);
}
