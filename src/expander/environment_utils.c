#include "minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_env_var(t_mini *mini, char *var_name)
{
	t_env	*current;

	if (!var_name || !mini->env_list)
		return (NULL);
	current = mini->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

// Adicionar em environment_utils.c
char	*get_exp_var(t_mini *mini, char *var_name)
{
	t_export	*current;

	if (!var_name || !mini->export_list)
		return (NULL);
	current = mini->export_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			if (current->value)
				return (current->value);
			else
				return (""); // Variável exportada sem valor
		}
		current = current->next;
	}
	return (NULL);
}
