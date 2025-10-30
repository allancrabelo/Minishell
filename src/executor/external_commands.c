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

int	count_env_vars(t_env *list)
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

char	*join_env_pair(t_env *env)
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
