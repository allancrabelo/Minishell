#include "minishell.h"

/**
 * @brief Parses primary expressions including parentheses
 *
 * Handles parenthesized expressions and command sequences.
 * Recursively parses content within parentheses and ensures
 * proper closing parenthesis.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @return t_ast* AST node for primary expression, NULL on error
 */
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

/**
 * @brief Parses pipe expressions
 *
 * Handles sequences of commands connected by pipe operators.
 * Recursively builds AST nodes for each command in the pipe chain.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @return t_ast* AST node for pipe expression, NULL on error
 */
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

/**
 * @brief Parses AND expressions
 *
 * Handles sequences of commands connected by AND operators.
 * Recursively builds AST nodes for each command in the AND chain.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @return t_ast* AST node for AND expression, NULL on error
 */
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

/**
 * @brief Parses OR expressions
 *
 * Handles sequences of commands connected by OR operators.
 * Recursively builds AST nodes for each command in the OR chain.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @return t_ast* AST node for OR expression, NULL on error
 */
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
