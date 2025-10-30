#include "minishell.h"

static char	*join_path_cmd(char *path, char *cmd)
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

static char	*search_in_paths(char **paths, char *cmd)
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



void	print_command_error(char *cmd, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}


static int	count_env_vars(t_env *list)
{
	int	count;

	count = 0;
	while (list)
	{
		if (list->value)
			count++;
		list = list->next;
	}
	return (count);
}

static char	*join_env_pair(t_env *env)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(env->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, env->value);
	free(temp);
	if (!result)
		return (NULL);
	return (result);
}

char	**env_list_to_array(t_mini *mini)
{
	t_env	*cur;
	char	**env;
	int		i;
	int		count;

	count = count_env_vars(mini->env_list);
	env = malloc((count + 1) * sizeof(char *));
	if (!env)
		return (NULL);
	cur = mini->env_list;
	i = 0;
	while (cur)
	{
		if (cur->value)
		{
			env[i] = join_env_pair(cur);
			if (!env[i])
				return (ft_free_split(env), NULL);
			i++;
		}
		cur = cur->next;
	}
	env[i] = NULL;
	return (env);
}


static int	validate_command(char *cmd, char *full_path)
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

static int	handle_path_not_found(char *cmd)
{
	if (ft_strchr(cmd, '/'))
		print_command_error(cmd, "No such file or directory");
	else
		print_command_error(cmd, "command not found");
	return (127);
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
