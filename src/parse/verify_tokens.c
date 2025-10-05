#include "minishell.h"

static int	check_start_end_tokens(t_token *first)
{
	t_token	*last;

	if (!first)
		return (0);
	last = first;
	while (last->next)
		last = last->next;
	if (is_operator(first->type) || is_redirect(first->type)
		|| first->type == TOKEN_RPAREN)
		return (0);
	if (is_operator(last->type) || is_redirect(last->type)
		|| last->type == TOKEN_LPAREN)
		return (0);
	return (1);
}

static int	check_parentheses_balance(t_token *token)
{
	int	parens;

	parens = 0;
	while (token)
	{
		if (token->type == TOKEN_LPAREN)
			parens++;
		else if (token->type == TOKEN_RPAREN)
		{
			parens--;
			if (parens < 0)
				return (0);
		}
		token = token->next;
	}
	return (parens == 0);
}

static int	check_empty_parentheses(t_token *token)
{
	while (token && token->next)
	{
		if (token->type == TOKEN_LPAREN && token->next->type == TOKEN_RPAREN)
			return (0);
		token = token->next;
	}
	return (1);
}

static int	check_syntax(t_token *cur)
{
	if (!cur)
		return (1);
	if (cur->type >= TOKEN_OR && cur->type <= TOKEN_PIPE)
	{
		if (!cur->next || (cur->next->type >= TOKEN_OR && cur->next->type <= TOKEN_PIPE)
			|| cur->next->type == TOKEN_RPAREN)
			return (0);
		return (1);
	}
	if (cur->type >= TOKEN_REDIRECT_IN && cur->type <= TOKEN_REDIRECT_APPEND)
	{
		if (!cur->next || cur->next->type != TOKEN_WORD)
			return (0);
		return (1);
	}
	return (1);
}

int	verify_tokens(t_mini *mini, t_token *token)
{
	t_token	*cur;

	if (!check_start_end_tokens(token))
		return (print_syntax_error(mini, "near unexpected token"));
	if (!check_parentheses_balance(token))
		return (print_syntax_error(mini, "unmatched parentheses"));
	if (!check_empty_parentheses(token))
		return (print_syntax_error(mini, "near unexpected token ')'"));
	cur = token;
	while (cur)
	{
		if (!check_syntax(cur))
			return (print_syntax_error(mini, "near unexpected token"));
		cur = cur->next;
	}
	return (1);
}
