#include "minishell.h"

int	is_operator(t_token_type type)
{
	return (type == TOKEN_OR || type == TOKEN_AND || type == TOKEN_PIPE);
}

int	is_redirect(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC
		|| type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND);
}

int	print_syntax_error(t_mini *mini, char *error_msg)
{
	printf("minishell: syntax error %s\n", error_msg);
	mini->exit_status = 2;
	return (0);
}
