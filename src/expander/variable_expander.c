/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:18 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:44:19 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles special variable expansion cases
 *
 * Processes special characters following '$' including:
 * - '?' for exit status
 * - '*' for wildcard
 * - '$' for literal dollar
 * - Digits and '!' for special parameters
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @return char* Special variable name, NULL for regular variables
 */
static char	*handle_special_cases(t_mini *mini, size_t *i)
{
	char	*var_name;

	if (mini->input[*i] == '?' || mini->input[*i] == '*')
	{
		var_name = ft_strdup((char []){mini->input[*i], '\0'});
		(*i)++;
		return (var_name);
	}
	if (!mini->input[*i] || mini->input[*i] == ' '
		|| mini->input[*i] == '\"' || mini->input[*i] == '\''
		|| mini->input[*i] == '$')
		return (ft_strdup("$"));
	if ((mini->input[*i] >= '0' && mini->input[*i] <= '9')
		|| mini->input[*i] == '!')
	{
		var_name = malloc(2);
		if (!var_name)
			return (NULL);
		var_name[0] = mini->input[*i];
		var_name[1] = '\0';
		(*i)++;
		return (var_name);
	}
	return (NULL);
}

/**
 * @brief Extracts variable name from input string
 *
 * Parses alphanumeric characters and underscores following '$'
 * to form a valid variable name.
 *
 * @param mini Pointer to main shell structure
 * @param start Starting index of variable name
 * @param i Pointer to current index in input string
 * @return char* Extracted variable name, or "$" if none found
 */
static char	*extract_variable_name(t_mini *mini, size_t start, size_t *i)
{
	char	*var_name;
	size_t	len;

	len = 0;
	while (mini->input[*i] && (ft_isalnum(mini->input[*i])
			|| mini->input[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	if (len == 0)
		return (ft_strdup("$"));
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, &mini->input[start], len + 1);
	return (var_name);
}

/**
 * @brief Extracts variable name considering special cases
 *
 * Combines special case handling and regular variable name extraction.
 * Advances index appropriately based on variable type.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @return char* Extracted variable name
 */
char	*extract_var_name(t_mini *mini, size_t *i)
{
	char	*var_name;
	size_t	start;

	start = *i;
	var_name = handle_special_cases(mini, i);
	if (var_name)
		return (var_name);
	return (extract_variable_name(mini, start, i));
}

/**
 * @brief Expands variable name to its value
 *
 * Looks up the variable name in environment and
 * expansion variables, handling special cases.
 *
 * @param mini Pointer to main shell structure
 * @param var_name Variable name to expand
 * @return char* Expanded variable value
 */
char	*expand_variable(t_mini *mini, char *var_name)
{
	char	*value;

	if (!var_name || var_name[0] == '\0')
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "$") == 0)
		return (ft_strdup("$"));
	if (var_name[0] >= '0' && var_name[0] <= '9' && var_name[1] == '\0')
		return (ft_strdup(""));
	if (ft_strlen(var_name) == 1 && is_special_var(var_name[0]))
		return (expand_special_var(mini, var_name[0]));
	if (ft_strcmp(var_name, "*") == 0)
		return (ft_strdup(""));
	value = get_env_var(mini, var_name);
	if (value)
		return (ft_strdup(value));
	value = get_exp_var(mini, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

/**
 * @brief Expands variable in tokenizer and returns value length
 *
 * Extracts variable name from input, expands it to its value,
 * and returns the length of the expanded value. Advances index.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @return size_t Length of expanded variable value
 */
size_t	expand_var_in_tokenizer(t_mini *mini, size_t *i)
{
	size_t	value_len;
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = extract_var_name(mini, i);
	if (!var_name)
		return (1);
	var_value = expand_variable(mini, var_name);
	free(var_name);
	if (!var_value)
		return (1);
	value_len = ft_strlen(var_value);
	free(var_value);
	return (value_len);
}
