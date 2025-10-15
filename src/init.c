#include "minishell.h"

static char	**alloc_envp(void)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (10 + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i <= 10)
		new_envp[i++] = NULL;
	return (new_envp);
}

static void	set_pwd_env(char **new_envp, int *i)
{
	char	cwd[4096];
	int		total_size;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		total_size = ft_strlen("PWD=") + ft_strlen(cwd) + 1;
		new_envp[*i] = malloc(total_size);
		if (new_envp[*i])
		{
			ft_strlcpy(new_envp[*i], "PWD=", total_size);
			ft_strlcat(new_envp[*i], cwd, total_size);
			(*i)++;
		}
	}
}

char	**envp_initializer(void)
{
	char	**new_envp;
	int		i;

	new_envp = alloc_envp();
	if (!new_envp)
		return (NULL);
	i = 0;
	set_pwd_env(new_envp, &i);
	new_envp[i] = ft_strdup("SHLVL=0");
	if (new_envp[i])
		i++;
	new_envp[i] = ft_strdup("_=./minishell");
	if (new_envp[i])
		i++;
	return (new_envp);
}

void	init_export_list(t_mini *mini, char **envp)
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

void	init_mini(t_mini *mini, char **envp)
{
	mini->token = NULL;
	mini->ast = NULL;
	mini->exit_status = 0;
	mini->export_list = NULL;
	mini->env_list = NULL;
	mini->envp = envp;
}