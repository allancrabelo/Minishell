#include "minishell.h"

int	is_op(const char *input, size_t i)
{
	if (input[i] == '\0')
		return (0);
	if (input[i] == '(')
		return (TOKEN_LPAREN);
	if (input[i] == ')')
		return (TOKEN_RPAREN);
	if (input[i] == '|' && input[i + 1] && input[i + 1] == '|')
		return (TOKEN_OR);
	if (input[i] == '&' && input[i + 1] && input[i + 1] == '&')
		return (TOKEN_AND);
	if (input[i] == '|')
		return (TOKEN_PIPE);
	if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
		return (TOKEN_REDIRECT_APPEND);
	if (input[i] == '>')
		return (TOKEN_REDIRECT_OUT);
	if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
		return (TOKEN_HEREDOC);
	if (input[i] == '<')
		return (TOKEN_REDIRECT_IN);
	return (0);
}
