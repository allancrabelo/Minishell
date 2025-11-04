/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:46 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * @brief Expands variable name by advancing through valid characters
 *
 * Moves index pointer through alphanumeric characters and underscores
 * that form a valid shell variable name.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @return size_t Length of the variable name
 */
size_t	expand_var(t_mini *mini, size_t *i)
{
	size_t	start;

	start = *i;
	while (ft_isalnum(mini->input[*i]) || mini->input[*i] == '_')
		(*i)++;
	return (*i - start);
}

/**
 * @brief Processes quoted string and calculates expanded length
 *
 * Handles both single and double quotes, expanding variables
 * within double quotes. Advances index to end of quoted section.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @param quote Type of quote (' or ")
 * @return size_t Length of expanded quoted content
 */
size_t	handle_quotes(t_mini *mini, size_t *i, char quote)
{
	size_t	len;

	(*i)++;
	len = 0;
	while (mini->input[*i] && mini->input[*i] != quote)
	{
		if (quote == '"' && mini->input[*i] == '$')
			len += expand_var_in_tokenizer(mini, i);
		else
		{
			len++;
			(*i)++;
		}
	}
	if (mini->input[*i] == quote)
		(*i)++;
	return (len);
}

/**
 * @brief Expands tilde to home directory length
 *
 * Retrieves HOME environment variable and returns its length.
 * Advances index past the tilde character.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @return size_t Length of the expanded home directory path
 */
static size_t	expand_tilde_len(t_mini *mini, size_t *i)
{
	char	*home;
	size_t	len;

	home = get_env_var(mini, "HOME");
	(*i)++;
	if (home)
	{
		len = ft_strlen(home);
		return (len);
	}
	return (1);
}

/**
 * @brief Calculates length of next word in input string
 *
 * Processes input starting from index, handling quotes,
 * variable expansions, and tilde expansions to compute
 * the total length of the next word token.
 *
 * @param mini Pointer to main shell structure
 * @param len Total length of input string
 * @param i Pointer to current index in input string
 * @return size_t Length of the next word token
 */
size_t	get_word_len(t_mini *mini, size_t len, size_t *i)
{
	size_t	word_len;

	word_len = 0;
	while (*i < len && !ft_isspace(mini->input[*i]) && !is_op(mini->input, *i))
	{
		if (mini->input[*i] == '\'' || mini->input[*i] == '"')
			word_len += handle_quotes(mini, i, mini->input[*i]);
		else if (mini->input[*i] == '~' && word_len == 0)
			word_len += expand_tilde_len(mini, i);
		else if (mini->input[*i] == '$' && !heredoc_expand_safe(mini))
			word_len += expand_var_in_tokenizer(mini, i);
		else
		{
			word_len++;
			(*i)++;
		}
	}
	return (word_len);
}

/**
 * @brief Checks for balanced quotes in input string
 *
 * Scans the input for single and double quotes, ensuring
 * that all opened quotes are properly closed.
 *
 * @param mini Pointer to main shell structure
 * @param input Input string to validate
 * @return int 1 if quotes are balanced, 0 otherwise
 */
int	check_validity(t_mini *mini, char *input)
{
	size_t	i;
	int		single_quote;
	int		double_quote;
	int		ret;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (input[i] == '"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	ret = !single_quote && !double_quote;
	if (!ret)
	{
		mini->exit_status = 2;
		ft_putstr_fd("minishell: unexpected EOF while \
looking for matching quotes\n", 2);
	}
	return (ret);
}
