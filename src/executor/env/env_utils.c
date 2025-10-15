#include "minishell.h"

t_env	*findenv(t_mini *mini, char *key)
{
	t_env	*find;

	find = mini->env_list;
	while (find != NULL)
	{
		if (ft_strcmp(key, find->key) == 0)
			return (find);
		find = find->next;
	}
	return (NULL);
}
