/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_token_data_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:53 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * @brief Handles the start of a quoted section
 *
 * Detects and sets the opening quote character when not already in quotes.
 *
 * @param c Current character being processed
 * @param quote Pointer to current quote state (0, ', or ")
 * @return int 1 if quote started, 0 otherwise
 */
static int	handle_quote_start(char c, int *quote)
{
	if ((c == '\'' || c == '"') && *quote == 0)
	{
		*quote = c;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles the end of a quoted section
 *
 * Detects and clears the quote state when matching closing quote is found.
 *
 * @param c Current character being processed
 * @param quote Pointer to current quote state (0, ', or ")
 * @return int 1 if quote ended, 0 otherwise
 */
static int	handle_quote_end(char c, int *quote)
{
	if (c == *quote && *quote != 0)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}

/**
 * @brief Fills token data with expanded content
 *
 * Processes input string to create token data, handling quote management,
 * tilde expansion, and variable substitution. Skips quote characters
 * in final output.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @param len Length of data to extract
 * @return char* Allocated string with processed token data
 */
char	*fill_token_data(t_mini *mini, size_t *i, size_t len)
{
	char	*dst;
	size_t	j;
	int		quote;

	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	quote = 0;
	j = 0;
	while (j < len && mini->input[*i])
	{
		if (handle_quote_start(mini->input[*i], &quote)
			|| handle_quote_end(mini->input[*i], &quote))
			(*i)++;
		else if (mini->input[*i] == '~' && quote == 0
			&& (j == 0 || dst[j - 1] == '='))
			do_tilde(mini, dst, i, &j);
		else if (mini->input[*i] == '$' && quote != '\''
			&& !heredoc_expand_safe(mini))
			do_var(mini, dst, i, &j);
		else
			dst[j++] = mini->input[(*i)++];
	}
	dst[j] = '\0';
	return (dst);
}
