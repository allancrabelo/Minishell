#include "../includes/minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_env_var(t_mini *mini, char *var_name)
{
	int		i;
	size_t	name_len;

	if (!var_name || !mini->envp)
		return (NULL);
	name_len = ft_strlen(var_name);
	i = 0;
	while (mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], var_name, name_len) == 0
			&& mini->envp[i][name_len] == '=')
		{
			return (&mini->envp[i][name_len + 1]);
		}
		i++;
	}
	return (NULL);
}
