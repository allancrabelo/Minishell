#include "minishell.h"

static int	handle_quote_start(char c, int *quote)
{
	if ((c == '\'' || c == '"') && *quote == 0)
	{
		*quote = c;
		return (1);
	}
	return (0);
}

static int	handle_quote_end(char c, int *quote)
{
	if (c == *quote && *quote != 0)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}

char	*fill_token_data(t_mini *mini, size_t *i, size_t len)
{
	char	*dst;
	size_t	j;
	int		quote;
	char	*var_name;
	char	*expanded_value;

	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	quote = 0;
	j = 0;
	while (j < len && mini->input[*i])
	{
		if (handle_quote_start(mini->input[*i], &quote))
			(*i)++;
		else if (handle_quote_end(mini->input[*i], &quote))
			(*i)++;
		else if (mini->input[*i] == '$' && quote != '\'')
		{
			(*i)++;
			var_name = extract_var_name(mini, i);
			expanded_value = expand_variable(mini, var_name);
			free(var_name);
			if (expanded_value)
			{
				ft_strlcpy(&dst[j], expanded_value, len - j + 1);
				j += ft_strlen(expanded_value);
				free(expanded_value);
			}
		}
		else
			dst[j++] = mini->input[(*i)++];
	}
	dst[j] = '\0';
	return (dst);
}
