#include "../includes/minishell.h"

int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (1);
	if (c == '(' || c == ')')
		return (1);
	return (0);
}

t_token_type	get_pipe_or_operator(const char *input, size_t *advance)
{
	if (*input == '|')
	{
		if (*(input + 1) == '|')
		{
			*advance = 2;
			return (TOKEN_OR);
		}
		*advance = 1;
		return (TOKEN_PIPE);
	}
	*advance = 1;
	return (TOKEN_WORD);
}

t_token_type	get_redirect_operator(const char *input, size_t *advance)
{
	if (*input == '<')
	{
		if (*(input + 1) == '<')
		{
			*advance = 2;
			return (TOKEN_HEREDOC);
		}
		*advance = 1;
		return (TOKEN_REDIRECT_IN);
	}
	if (*input == '>')
	{
		if (*(input + 1) == '>')
		{
			*advance = 2;
			return (TOKEN_REDIRECT_APPEND);
		}
		*advance = 1;
		return (TOKEN_REDIRECT_OUT);
	}
	*advance = 1;
	return (TOKEN_WORD);
}

t_token_type	get_op_type(const char *input, size_t *advance)
{
	*advance = 1;
	if (*input == '|')
		return (get_pipe_or_operator(input, advance));
	else if (*input == '<' || *input == '>')
		return (get_redirect_operator(input, advance));
	else if (*input == '&' && *(input + 1) == '&')
	{
		*advance = 2;
		return (TOKEN_AND);
	}
	else if (*input == '(')
		return (TOKEN_LPAREN);
	else if (*input == ')')
		return (TOKEN_RPAREN);
	return (TOKEN_WORD);
}

size_t	handle_quoted_section(const char **input, char quote)
{
	size_t	len;

	len = 1;
	(*input)++;
	while (**input && **input != quote)
	{
		len++;
		(*input)++;
	}
	if (**input == quote)
	{
		len++;
		(*input)++;
	}
	return (len);
}

size_t	handle_unquoted_section(const char **input)
{
	size_t	len;

	len = 0;
	while (**input && !ft_isspace(**input) && !is_operator_char(**input))
	{
		if (**input == '\'' || **input == '"')
			break ;
		len++;
		(*input)++;
	}
	return (len);
}