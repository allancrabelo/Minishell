#include "minishell.h"

// Parse primary expressions (commands and parentheses)
static t_ast	*parse_primary(t_mini *mini, t_token **tokens)
{
	t_ast	*node;

	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = parse_or(mini, tokens);
		if (!node)
			return (NULL);
		if (!*tokens || (*tokens)->type != TOKEN_RPAREN)
		{
			print_syntax_error(mini, "expected", ")");
			free_ast(node);
			return (NULL);
		}
		*tokens = (*tokens)->next;
		return (node);
	}
	return (parse_command(mini, tokens));
}

// Parse PIPE expressions
static t_ast	*parse_pipe(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_primary(mini, tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_primary(mini, tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_operator_node(left, right, TOKEN_PIPE);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

// Parse AND expressions
static t_ast	*parse_and(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_pipe(mini, tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_AND)
	{
		*tokens = (*tokens)->next;
		right = parse_pipe(mini, tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_operator_node(left, right, TOKEN_AND);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

// Parse OR expressions (lowest precedence)
t_ast	*parse_or(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_and(mini, tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_OR)
	{
		*tokens = (*tokens)->next;
		right = parse_and(mini, tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_operator_node(left, right, TOKEN_OR);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}
