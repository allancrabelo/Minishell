/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:46 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:43:47 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Validates start and end tokens of the token stream
 *
 * Checks that the token sequence begins and ends with valid tokens:
 * - First token cannot be operator or right parenthesis
 * - Last token cannot be operator
 * - Last token cannot be left parenthesis (expecting continuation)
 *
 * @param first First token in the linked list
 * @return char* Error token data if invalid, NULL if valid
 */
static char	*check_start_end_tokens(t_token *first)
{
	t_token	*last;

	if (!first)
		return (NULL);
	last = first;
	while (last->next)
		last = last->next;
	if (is_operator(first->type) || first->type == TOKEN_RPAREN)
		return (first->data);
	if (is_operator(last->type))
		return (last->data);
	if (last->type == TOKEN_LPAREN)
		return ("newline");
	return (NULL);
}

/**
 * @brief Validates parentheses balance in token stream
 *
 * Counts left and right parentheses to ensure proper nesting
 * and balance. Detects unmatched or misordered parentheses.
 *
 * @param token Linked list of tokens to check
 * @return int 1 if parentheses are balanced, 0 otherwise
 */
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

/**
 * @brief Checks for empty parentheses in token stream
 *
 * Scans token list for adjacent left and right parentheses
 * without any content between them (e.g., "()")
 *
 * @param token Linked list of tokens to check
 * @return int 0 if empty parentheses found, 1 otherwise
 */
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

/**
 * @brief Validates syntax rules for individual tokens
 *
 * Checks operator and redirect placement rules:
 * - Operators (OR, AND, PIPE) cannot be followed by other operators
 * or right parentheses
 * - Redirects must be followed by valid filename tokens
 * - Detects missing tokens at end of input
 *
 * @param cur Current token to validate
 * @return char* Error token data if invalid, NULL if valid
 */
static char	*check_syntax(t_token *cur)
{
	if (!cur)
		return (NULL);
	if (cur->type >= TOKEN_OR && cur->type <= TOKEN_PIPE)
	{
		if (!cur->next)
			return ("newline");
		if ((cur->next->type >= TOKEN_OR && cur->next->type <= TOKEN_PIPE)
			|| cur->next->type == TOKEN_RPAREN)
			return (cur->next->data);
		return (NULL);
	}
	if (cur->type >= TOKEN_REDIRECT_IN && cur->type <= TOKEN_REDIRECT_APPEND)
	{
		if (!cur->next)
			return ("newline");
		if (cur->next->type != TOKEN_WORD)
			return (cur->next->data);
		return (NULL);
	}
	return (NULL);
}

/**
 * @brief Validates token sequence syntax
 *
 * Performs comprehensive syntax validation on tokenized input including:
 * - Start/end token verification
 * - Parentheses balancing
 * - Empty parentheses detection
 * - Operator and redirect syntax rules
 *
 * @param mini Pointer to main shell structure
 * @param token Linked list of tokens to validate
 * @return int 1 if syntax valid, 0 if error detected
 */
int	verify_tokens(t_mini *mini, t_token *token)
{
	t_token	*cur;
	char	*data;

	data = check_start_end_tokens(token);
	if (data)
		return (print_syntax_error(mini, "near unexpected token ", data));
	if (!check_parentheses_balance(token))
		return (print_syntax_error(mini, "unmatched parentheses", NULL));
	if (!check_empty_parentheses(token))
		return (print_syntax_error(mini, "near unexpected token ", "`)'"));
	cur = token;
	while (cur)
	{
		data = check_syntax(cur);
		if (data)
			return (print_syntax_error(mini, "near unexpected token ", data));
		cur = cur->next;
	}
	return (1);
}
