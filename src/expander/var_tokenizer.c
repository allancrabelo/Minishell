#include "../../includes/minishell.h"

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
