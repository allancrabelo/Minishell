#include "minishell.h"

t_export	*ft_newexp(char *key, char *value)
{
	t_export	*new;

	new = malloc(sizeof(t_export));
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

void	ft_addexp(t_export **lst, t_export *new)
{
	t_export	*temp;

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

t_export	*findexp(t_mini *mini, char *key)
{
	t_export	*find;

	find = mini->export_list;
	while (find != NULL)
	{
		if (ft_strcmp(key, find->key) == 0)
			return (find);
		find = find->next;
	}
	return (NULL);
}

int	ft_setexp(char *key, char *newvalue, t_mini *mini)
{
	t_export	*newexp;
	t_export	*find;

	find = findexp(mini, key);
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
		newexp = ft_newexp(key, newvalue);
		if (!newexp)
			return (-1);
		ft_addexp(&mini->export_list, newexp);
		return (0);
	}
}

char	*ft_getexp(char *key, t_mini *mini)
{
	t_export	*find;

	find = mini->export_list;
	while (find != NULL)
	{
		if (ft_strcmp(key, find->key) == 0)
			return (find->value);
		find = find->next;
	}
	return (NULL);
}

void	free_export_list(t_export *lst)
{
	t_export	*tmp;

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