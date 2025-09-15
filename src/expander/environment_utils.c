/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment _utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:25:45 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/15 16:32:52 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	char	*name_with_equals;
	size_t	name_len;

	if (!var_name || !mini->envp)
		return (NULL);
	name_len = ft_strlen(var_name);
	name_with_equals = malloc(sizeof(char) * (name_len + 2));
	if (!name_with_equals)
		return (NULL);
	ft_strcpy(name_with_equals, var_name);
	return (free(name_with_equals), NULL);
}