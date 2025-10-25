#include "minishell.h"

static char	*expand_tilde(t_mini *mini)
{
	char	*home;

	home = get_env_var(mini, "HOME");
	if (home)
		return (ft_strdup(home));
	return (ft_strdup("~"));
}

static void	copy_exp(char *dst, size_t *j, char *exp)
{
	size_t	i;

	if (!exp)
		return ;
	i = 0;
	while (exp[i])
		dst[(*j)++] = exp[i++];
	free(exp);
}

void	do_tilde(t_mini *mini, char *dst, size_t *i, size_t *j)
{
	(*i)++;
	copy_exp(dst, j, expand_tilde(mini));
}

void	do_var(t_mini *mini, char *dst, size_t *i, size_t *j)
{
	char	*var_name;

	(*i)++;
	var_name = extract_var_name(mini, i);
	copy_exp(dst, j, expand_variable(mini, var_name));
	free(var_name);
}
