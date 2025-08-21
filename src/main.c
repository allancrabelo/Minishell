/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:08:05 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 18:21:05 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/colors.h"

void	do_commands(char *input)
{
	add_history(input);
	handle_commands(input);
}

static void	main_loop(void)
{
	char	*input;

	while (1)
	{
		input = readline(SYELLOW "minishell> " SRESET);
		if (!input)
			return ;
		else
			do_commands(input);
		free (input);
	}
}

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	if (argc != 1 || argv[1])
		return (printf ( "[ERROR] Usage: ./minishell\n" SRESET), 127);
	signal_init();
	main_loop();
	exit(EXIT_SUCCESS);
}