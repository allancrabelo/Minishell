#include "../includes/minishell.h"

void	skip_whitespace(const char **input)
{
	while (**input && ft_isspace(**input))
		(*input)++;
}

size_t	get_word_length(const char *input)
{
	size_t		total_len;
	size_t		part_len;
	const char	*ptr;

	total_len = 0;
	ptr = input;
	while (*ptr && !ft_isspace(*ptr) && !is_operator_char(*ptr))
	{
		if (*ptr == '\'' || *ptr == '"')
			part_len = handle_quoted_section(&ptr, *ptr);
		else
			part_len = handle_unquoted_section(&ptr);
		total_len += part_len;
	}
	return (total_len);
}

void	advance_through_word(const char **input)
{
	while (**input && !ft_isspace(**input) && !is_operator_char(**input))
	{
		if (**input == '\'' || **input == '"')
		{
			handle_quoted_section(input, **input);
			continue ;
		}
		handle_unquoted_section(input);
	}
}

char	*ft_strndup(const char *str, size_t len)
{
	char	*dst;
	size_t	i;

	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len && str[i])
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	check_quotes_validity(const char *input)
{
	int		single_quote;
	int		double_quote;
	size_t	i;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (input[i] == '"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	if (single_quote || double_quote)
		return (0);
	return (1);
}
