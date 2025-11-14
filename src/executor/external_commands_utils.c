#include "minishell.h"

void	print_command_error(char *cmd, char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(error, 2);
	ft_putstr_fd("\n", 2);
}

static int	count_env(t_env *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

static char	*build_env_entry(t_env *env)
{
	size_t	vlen;
	size_t	len;
	char	*entry;

	vlen = 0;
	if (env->value)
		vlen = ft_strlen(env->value);
	len = ft_strlen(env->key) + vlen + 2;
	entry = ft_calloc(len, sizeof(char));
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, env->key, len);
	ft_strlcat(entry, "=", len);
	if (env->value)
		ft_strlcat(entry, env->value, len);
	return (entry);
}

static int	fill_envp_array(char **envp, t_env *lst)
{
	int		i;
	char	*entry;

	i = 0;
	while (lst)
	{
		entry = build_env_entry(lst);
		if (!entry)
		{
			ft_free_split(envp);
			return (EXIT_FAILURE);
		}
		envp[i++] = entry;
		lst = lst->next;
	}
	envp[i] = NULL;
	return (EXIT_SUCCESS);
}

int	update_envp(t_mini *mini)
{
	int		count;
	char	**envp;

	count = count_env(mini->env_list);
	envp = ft_calloc((count + 1), sizeof(char *));
	if (!envp)
		return (EXIT_FAILURE);
	if (fill_envp_array(envp, mini->env_list) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (mini->envp)
		ft_free_split(mini->envp);
	mini->envp = envp;
	mini->env_mod = 0;
	return (EXIT_SUCCESS);
}
