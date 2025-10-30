/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:51 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:43:52 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Identifies operator type at current input position
 *
 * Scans input string for shell operators including parentheses,
 * logical operators, pipes, and redirections. Handles multi-character
 * operators (||, &&, >>, <<).
 *
 * @param input Input string to scan
 * @param i Current index in input string
 * @return t_token_type Operator token type, 0 if no operator found
 */
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
