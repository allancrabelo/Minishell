#include "../../includes/minishell.h"

char	*extract_var_name(t_mini *mini, size_t *i)
{
	char	*var_name;
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	if (mini->input[*i] == '?')
	{
		var_name = ft_strdup("?");
		(*i)++;
		return (var_name);
	}
	while (mini->input[*i] && (ft_isalnum(mini->input[*i]) || mini->input[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	if (len == 0)
		return (ft_strdup(""));
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, &mini->input[start], len + 1);
	return (var_name);
}

char	*expand_variable(t_mini *mini, char *var_name)
{
	char	*value;

	if (!var_name || var_name[0] == '\0')
		return (ft_strdup(""));
	if (ft_strlen(var_name) == 1 && is_special_var(var_name[0]))
		return (expand_special_var(mini, var_name[0]));
	value = get_env_var(mini, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

size_t	expand_var_in_tokenizer(t_mini *mini, size_t *i)
{
	size_t	value_len;
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = extract_var_name(mini, i);
	if (!var_name)
		return (1);
	var_value = expand_variable(mini, var_name);
	free(var_name);
	if (!var_value)
		return (1);
	value_len = ft_strlen(var_value);
	free(var_value);
	return (value_len);
}