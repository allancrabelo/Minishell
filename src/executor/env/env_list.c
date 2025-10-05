#include "minishell.h"

t_env	*ft_newenv(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	ft_addenv(t_env **lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

// Encontrar variável na lista de ambiente
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

int	ft_setenv(char *key, char *newvalue, t_mini *mini)
{
	t_env	*newenv;
	t_env	*find;

	find = findenv(mini, key);
	if (find)
	{
		if (find->value != NULL)
			free(find->value);
		if (newvalue)
			find->value = ft_strdup(newvalue);
		else
			find->value = NULL;
		return (0);
	}
	else
	{
		newenv = ft_newenv(key, newvalue);
		if (!newenv)
			return (-1);
		ft_addenv(&mini->env_list, newenv);
		return (0);
	}
}

char	*ft_getenv(char *key, t_mini *mini)
{
	t_env	*find;

	find = mini->env_list;
	while (find != NULL)
	{
		if (ft_strcmp(key, find->key) == 0)
			return (find->value);
		find = find->next;
	}
	return (NULL);
}

void	free_env_list(t_env *lst)
{
	t_env	*tmp;

	if (!lst)
		return ;
	while (lst)
	{
		tmp = lst->next;
		free(lst->key);
		if (lst->value)
			free(lst->value);
		free(lst);
		lst = tmp;
	}
}
