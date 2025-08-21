/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:27:39 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 18:40:03 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	flag_verification( char *commands)
{
	char	ret;
	int		i;

	i = 0;
	ret = 0;
	if (commands[i] == '-')
		i++;
	else
		return (0);
	while (commands[i] == 'n' || (commands[i] >= 9 && commands[i] <= 13))
		i++;
	if (commands[i] == '\0')
		ret = 'n';
	return (ret);
}

int	ft_echo(char **commands) //TODO: Lidar com as aspas
{
	int	i;
	int	flag;

	i = 1;
	flag = flag_verification(commands[1]);
	if (flag)
		while (commands[i] && flag_verification(commands[i]))
			i++;
	while (commands[i])
	{
		printf("%s", commands[i]);
		if (commands[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 'n')
		return (0);
	printf("\n");
	return (0);
}
