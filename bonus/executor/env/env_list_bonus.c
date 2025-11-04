/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:27 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
