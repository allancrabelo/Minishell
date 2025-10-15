#include "../../includes/minishell.h"

size_t	expand_var(t_mini *mini, size_t *i)
{
	size_t	start;

	start = *i;
	while (ft_isalnum(mini->input[*i]) || mini->input[*i] == '_')
		(*i)++;
	return (*i - start);
}

size_t	handle_quotes(t_mini *mini, size_t *i, char quote)
{
	size_t	len;

	(*i)++;
	len = 0;
	while (mini->input[*i] && mini->input[*i] != quote)
	{
		if (quote == '"' && mini->input[*i] == '$')
			len += expand_var_in_tokenizer(mini, i);
		else
		{
			len++;
			(*i)++;
		}
	}
	if (mini->input[*i] == quote)
		(*i)++;
	return (len);
}
