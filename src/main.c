#include "minishell.h"
#include "colors.h"

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

char	**envp_initializer(void) 
{
	char	**new_envp;
	char	cwd[4096];
	int		total_size;
	int		i;

	new_envp = malloc(sizeof(char *) * (10 + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i <= 10)
		new_envp[i++] = NULL;
	i = 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		total_size = ft_strlen("PWD=") + ft_strlen(cwd) + 1;
		new_envp[i] = malloc(total_size);
		if (new_envp[i])
		{
			ft_strlcpy(new_envp[i], "PWD=", total_size);
			ft_strlcat(new_envp[i], cwd, total_size);
			i++;
		}
	}
	else
		return(perror("getcwd"), NULL); //TODO: Make sense
	new_envp[i] = ft_strdup("SHLVL=0");
	if (new_envp[i]) i++;
	new_envp[i] = ft_strdup("_=./minishell"); 
	if (new_envp[i]) i++;
	return (new_envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	if (argc != 1 || argv[1])
		return (ft_putstr_fd("[ERROR] Usage: ./minishell\n" SRESET, 2), 127);
	signal_init();
	mini.token = NULL;
	mini.ast = NULL;
	mini.input = NULL;
	mini.exit_status = 0;
	mini.export_list = NULL;
	mini.env_list = NULL;
	mini.heredoc = NULL;
	mini.heredoc_fd = 0;
	mini.heredoc_signal = 0;
	mini.envp = envp;
	if (envp[0] == NULL || envp == NULL)
		mini.envp = envp_initializer();
	init_export_list(&mini, mini.envp);
	init_env_list(&mini, mini.envp);
	increment_shlvl(&mini);
	g_signal = 0;
	main_loop(&mini);
	ft_free_all(&mini, mini.exit_status, 0);
	rl_clear_history();
	exit(mini.exit_status);
}
