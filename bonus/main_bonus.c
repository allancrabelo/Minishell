/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:08 by mqueiros          #+#    #+#             */
/*   Updated: 2025/11/04 15:29:15 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

volatile sig_atomic_t	g_signal = 0;

/**
 * @brief Initializes the export list from environment variables
 *
 * Parses the environment variables array and populates the shell's
 * export list. Handles both key=value pairs and standalone keys.
 *
 * @param mini Pointer to the main shell structure
 * @param envp Array of environment variables strings
 * @return void
 */
static void	init_export_list(t_mini *mini, char **envp)
{
	int		i;
	char	*equal_pos;
	char	key[256];
	char	*value;

	mini->export_list = NULL;
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			ft_strlcpy(key, envp[i], equal_pos - envp[i] + 1);
			value = equal_pos + 1;
			ft_setexp(key, value, mini);
		}
		else
			ft_setexp(envp[i], NULL, mini);
		i++;
	}
}

/**
 * @brief Processes user input commands
 *
 * Adds input to command history, updates current working directory,
 * and delegates to command handler for execution.
 *
 * @param mini Pointer to the main shell structure
 * @param input User input string to process
 * @return void
 */
void	do_commands(t_mini *mini, char *input)
{
	add_history(input);
	if (findenv(mini, "PWD"))
		ft_setenv("PWD", mini->pwd, mini);
	if (findenv(mini, "OLDPWD"))
		ft_setenv("OLDPWD", mini->old_pwd, mini);
	handle_commands(mini, input);
}

/**
 * @brief Main interactive loop for the minishell
 *
 * Handles the REPL (Read-Eval-Print Loop) cycle for user interaction.
 * Reads input, processes commands, and manages exit conditions.
 *
 * @param mini Pointer to the main shell structure
 * @return void
 */
static void	main_loop(t_mini *mini)
{
	char	*input;

	while (1)
	{
		signal_init();
		input = readline(LINE);
		if (g_signal)
		{
			mini->exit_status = g_signal;
			g_signal = 0;
		}
		if (!input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		do_commands(mini, input);
		free (input);
	}
}

/**
 * @brief Initializes a basic environment when no environment is provided
 *
 * Creates a minimal environment array with essential variables including
 * PWD, OLDPWD, SHLVL, and underscore. Used when the shell starts without
 * an inherited environment.
 *
 * @return char** Newly allocated environment array, NULL-terminated
 */
static char	**envp_initializer(void)
{
	char	**new_envp;
	char	cwd[4096];
	int		total_size;
	int		i;

	total_size = sizeof(cwd);
	new_envp = malloc(sizeof(char *) * (10 + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i <= 10)
		new_envp[i++] = NULL;
	i = 0;
	i = set_getcwd(cwd, total_size, new_envp, i);
	new_envp[i] = ft_strdup("SHLVL=0");
	if (new_envp[i])
		i++;
	new_envp[i] = ft_strdup("_=./minishell");
	if (new_envp[i])
		i++;
	return (new_envp);
}

/**
 * @brief Main entry point for the minishell program
 *
 * Initializes the shell environment, processes startup configuration,
 * and enters the main command loop. Handles program cleanup on exit.
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @param envp Environment variables array
 * @return int Exit status code
 */
int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	if (argc != 1 || argv[1])
		return (ft_putstr_fd("[ERROR] Usage: ./minishell\n", 2), 127);
	signal_init();
	mini_init(&mini, envp);
	if (envp == NULL || envp[0] == NULL)
		mini.envp = envp_initializer();
	init_export_list(&mini, mini.envp);
	init_env_list(&mini, mini.envp);
	increment_shlvl(&mini);
	mini.pwd = ft_strdup(ft_getenv("PWD", &mini));
	if (findenv(&mini, "OLDPWD"))
		mini.old_pwd = ft_strdup(ft_getenv("OLDPWD", &mini));
	g_signal = 0;
	main_loop(&mini);
	ft_free_all(&mini, mini.exit_status, 0);
	rl_clear_history();
	exit(mini.exit_status);
}
