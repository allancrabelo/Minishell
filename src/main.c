#include "minishell.h"
#include "../includes/colors.h"

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
	handle_commands(mini, input);
}

static char *make_prompt(t_mini *mini)
{
	static char	buf[64];
	char		*num;
	int			status;

	status = mini->exit_status;
	if (g_signal)
	{
		status = g_signal;
		mini->exit_status = g_signal;
		g_signal = 0;
	}
	num = ft_itoa(status);
	buf[0] = '\0';
	if (status == 0)
	{
		ft_strlcpy(buf, "\001" SBBLUE "\002[", sizeof(buf));
		ft_strlcat(buf, num, sizeof(buf));
		ft_strlcat(buf, "]\001" SBYELLOW "\002 minishell$\001" SRESET "\002 ", sizeof(buf));
	}
	else
	{
		ft_strlcpy(buf, "\001" SBRED "\002[", sizeof(buf));
		ft_strlcat(buf, num, sizeof(buf));
		ft_strlcat(buf, "]\001" SBYELLOW "\002 minishell$\001" SRESET "\002 ", sizeof(buf));
	}
	free (num);
	return (buf);
}

static void	main_loop(t_mini *mini)
{
	char	*input;

	while (1)
	{
		signal_init();
		input = readline(make_prompt(mini));
		if (g_signal)
		{
			mini->exit_status = g_signal;
			g_signal = 0;
		}
		if (!input)
		{
			write(1, "exit\n", 5);
			break;
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
	mini.token = NULL;
	mini.ast = NULL;
	mini.envp = envp;
	mini.exit_status = 0;
	mini.export_list = NULL;
	init_export_list(&mini, envp);
	g_signal = 0;
	main_loop(&mini);
	if (mini.export_list)
		free_export_list(mini.export_list);
	rl_clear_history();
	exit(mini.exit_status);
}
