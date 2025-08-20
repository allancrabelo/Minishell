/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:08:05 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/20 18:31:49 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/colors.h"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	char	*input;

	while (1)
	{
		input = readline(SYELLOW "minishell$" SRESET);
		if (!input)
			return (1);
		else
			add_history(input);
		free (input);
	}
	return (0);
}