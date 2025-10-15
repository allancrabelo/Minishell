#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

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

void	do_commands(t_mini *mini, char *input)
{
	add_history(input);
	mini->pwd = ft_getenv("PWD", mini);
	handle_commands(mini, input);
}

static void	main_loop(t_mini *mini)
{
	char	*input;

	while (1)
	{
		signal_init();
		input = readline(SYELLOW "minishell$ " SRESET);
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

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	if (argc != 1 || argv[1])
		return (printf("[ERROR] Usage: ./minishell\n" SRESET), 127);
	signal_init();
	init_mini(&mini, envp);
	if (envp[0] == NULL || envp == NULL)
		mini.envp = envp_initializer();
	init_export_list(&mini, mini.envp);
	init_env_list(&mini, mini.envp);
	increment_shlvl(&mini);
	g_signal = 0;
	main_loop(&mini);
	if (mini.export_list)
		free_export_list(mini.export_list);
	if (mini.export_list)
		free_env_list(mini.env_list);
	rl_clear_history();
	exit(mini.exit_status);
}
