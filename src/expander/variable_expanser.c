#include "../includes/minishell.h"
#include "../includes/colors.h"

char	*expand_variable(t_mini *mini, char *var_name)
{
	char	*value;

	if (!var_name || var_name[0] == '\0')
		return (ft_strdup(""));
	if (ft_strlen(var_name) == 1 && is_special_var(var_name[0]))
		return (expand_special_var(mini, var_name[0])); //Exibir Exit status
	value  = get_env_var(mini, var_name);
	if (value)
		return (value);
	return (ft_strdup(""));
}

char	*extract_var_name_special(t_mini *mini, size_t *i)
{
	char	*var_name;

	var_name = malloc (sizeof(char) * 2);
	if (!var_name)
		return (NULL);
	if (var_name)
	{
		var_name[0] = mini->input[*i];
		var_name[1] = '\0';
		(*i)++;
	}
	return (var_name);
}

char	*extract_var_name_normal(t_mini *mini, size_t *i)
{
	char	*var_name;
	size_t	var_name_len;
	size_t	name_start;

	name_start = *i;
	var_name_len = 0;
	while (mini->input[*i] && (ft_isalnum(mini->input[*i])
		|| (mini->input[*i] == '_')))
	{
		(*i)++;
		var_name_len++;
	}
	if (var_name_len == 0)
		return (NULL);
	var_name = malloc(sizeof(char) * (var_name_len + 1));
	if (!var_name)
		return (NULL);
	if (var_name)
	{
		ft_strlcpy(var_name, mini->input + name_start, var_name_len);
		var_name[var_name_len] = '\0';
	}
	return (var_name);
}

size_t	expand_var_in_tokenizer(t_mini *mini, size_t *i)
{
	size_t	value_len;
	size_t	start;
	char	*var_name;
	char	*var_value;

	start = *i;
	(*i)++;
	if (mini->input[*i] == '?')
		var_name = extract_var_name_special(mini, i);
	else
		var_name = extract_var_name_normal(mini, i);
	if (!var_name)
		return(EXIT_FAILURE);
	var_value = expand_variable(mini, var_name);
	if (!var_value)
		return (*i - start);
	value_len = ft_strlen(var_value);
	free(var_name);
	free(var_value);
	return (value_len);
}
