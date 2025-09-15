/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:08:05 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/15 19:06:20 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/colors.h"

void	do_commands(t_mini *mini, char *input)
{
	add_history(input);
	handle_commands(mini, input);
}

static void	main_loop(t_mini *mini)
{
	char	*input;

	while (1)
	{
		input = readline(LINE);
		if (!input)
		{
			write(1, "exit\n", 5); // Verify output
			return ;
		}
		else
			do_commands(mini, input);
		free (input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	if (argc != 1 || argv[1])
		return (printf("[ERROR] Usage: ./minishell\n" SRESET), 127);
	signal_init();
	mini.token = NULL;
	mini.envp = envp;
	mini.exit_status = 0;
	main_loop(&mini);
	rl_clear_history();
	exit(EXIT_SUCCESS);
}
