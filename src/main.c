#include "../includes/minishell.h"
#include "../includes/colors.h"

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
		status = g_signal;
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
	return (buf); // static -> safe to return
}

static void	main_loop(t_mini *mini)
{
	char	*input;

	while (1)
	{
		input = readline(make_prompt(mini));
		if (g_signal)
		{
			mini->exit_status = g_signal;
			g_signal = 0;
		}
		if (!input)
		{
			write(1, "exit\n", 5); // Check output
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
	main_loop(&mini);
	rl_clear_history();
	exit(mini.exit_status);
}
