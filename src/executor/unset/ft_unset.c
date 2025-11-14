#include "minishell.h"

static void	remove_from_env_list(t_mini *mini, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = mini->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				mini->env_list = current->next;
			free(current->key);
			if (current->value)
			{
				mini->env_mod = 1;
				free(current->value);
			}
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static void	remove_from_export_list(t_mini *mini, char *key)
{
	t_export	*current;
	t_export	*prev;

	current = mini->export_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				mini->export_list = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_mini *mini, t_ast *node)
{
	int	i;
	int	ret;

	if (!node->args[1])
		return (0);
	ret = 0;
	i = 1;
	while (node->args[i])
	{
		remove_from_env_list(mini, node->args[i]);
		remove_from_export_list(mini, node->args[i]);
		i++;
	}
	return (ret);
}
