/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:26 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:45:27 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (EXIT_SUCCESS);
	}
	else
	{
		newenv = ft_newenv(key, newvalue);
		if (!newenv)
			return (-1);
		ft_addenv(&mini->env_list, newenv);
		return (EXIT_SUCCESS);
	}
}

void	increment_shlvl(t_mini *mini)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = ft_getenv("SHLVL", mini);
	if (!shlvl_str)
		ft_setenv("SHLVL", "1", mini);
	else
	{
		shlvl = ft_atoi(shlvl_str);
		if (shlvl < 0)
			shlvl = 0;
		else
			shlvl++;
		new_shlvl = ft_itoa(shlvl);
		if (new_shlvl)
		{
			ft_setenv("SHLVL", new_shlvl, mini);
			free(new_shlvl);
		}
	}
}

void	init_env_list(t_mini *mini, char **envp)
{
	int		i;
	char	*equal_pos;
	char	key[256];
	char	*value;

	mini->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			ft_strlcpy(key, envp[i], equal_pos - envp[i] + 1);
			value = equal_pos + 1;
			ft_setenv(key, value, mini);
		}
		else
			ft_setenv(envp[i], NULL, mini);
		i++;
	}
}

int	ft_env(t_mini *mini)
{
	t_env	*current;

	current = mini->env_list;
	while (current)
	{
		if (current->value != NULL)
		{
			ft_putstr_fd(current->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\n", 1);
		}
		current = current->next;
	}
	mini->exit_status = 0;
	return (0);
}
