#include "minishell.h"

static char	*expand_tilde(t_mini *mini)
{
	char	*home;

	home = get_env_var(mini, "HOME");
	if (home)
		return (ft_strdup(home));
	return (ft_strdup("~"));
}

void	handle_tilde_expansion(t_mini *mini, char *dst, size_t *i, size_t *j)
{
	char	*expanded_value;
	size_t	len;

	(*i)++;
	expanded_value = expand_tilde(mini);
	if (expanded_value)
	{
		len = ft_strlen(dst);
		ft_strlcpy(&dst[*j], expanded_value, len - *j + 1);
		*j += ft_strlen(expanded_value);
		free(expanded_value);
	}
}

void	handle_var_expansion(t_mini *mini, char *dst, size_t *i, size_t *j)
{
	char	*var_name;
	char	*expanded_value;
	size_t	len;

	(*i)++;
	var_name = extract_var_name(mini, i);
	expanded_value = expand_variable(mini, var_name);
	if (expanded_value)
	{
		len = ft_strlen(dst);
		ft_strlcpy(&dst[*j], expanded_value, len - *j + 1);
		*j += ft_strlen(expanded_value);
		free(expanded_value);
	}
	free(var_name);
}
