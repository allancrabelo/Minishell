/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:27:39 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 14:59:44 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_echo(char **commands)
{
	int	i;

	i = 1;
	while (commands[i])
	{
		printf("%s", commands[i]);
		if (commands[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}