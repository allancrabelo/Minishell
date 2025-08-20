/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:20:45 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 00:46:44 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_commands(char *input)
{
	char **commands;

	commands = ft_split(input, ' ');
	if (!commands || !commands[0])
		return ;
	if (ft_strcmp(commands[0], "echo") == 0)
		ft_echo(commands);
}