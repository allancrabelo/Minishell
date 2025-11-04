/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:04 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * @brief Counts array elements
 *
 * @param arr NULL-terminated array
 * @return Number of elements
 */
static int	count_array_elements(char **arr)
{
	int	count;

	count = 0;
	while (arr && arr[count])
		count++;
	return (count);
}

/**
 * @brief Expands a single argument if it contains wildcards
 *
 * If the argument has wildcards, attempts to expand them.
 * If no matches found, returns the original argument as-is.
 *
 * @param arg Argument to expand
 * @param expanded_count Output: number of expanded arguments
 * @return Array of expanded arguments, or single original argument
 */
static char	**expand_arg(char *arg, int *expanded_count)
{
	char	**result;

	if (!has_wildcard(arg))
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(arg);
		result[1] = NULL;
		*expanded_count = 1;
		return (result);
	}
	result = expand_wildcard(arg);
	if (!result)
	{
		result = malloc(sizeof(char *) * 2);
		if (!result)
			return (NULL);
		result[0] = ft_strdup(arg);
		result[1] = NULL;
		*expanded_count = 1;
		return (result);
	}
	*expanded_count = count_array_elements(result);
	return (result);
}

/**
 * @brief Calculates total arguments after wildcard expansion
 *
 * @param args Original argument array
 * @param current_count Current number of arguments
 * @return Total number of arguments after expansion
 */
static int	count_total_expanded(char **args, int current_count)
{
	int		i;
	int		total;
	char	**expanded;
	int		exp_count;

	total = 0;
	i = 0;
	while (i < current_count)
	{
		expanded = expand_arg(args[i], &exp_count);
		if (expanded)
		{
			total += exp_count;
			ft_free_split(expanded);
		}
		else
			total++;
		i++;
	}
	return (total);
}

static void	copy_expanded_args(char **expanded, char **new_args,
			int *j, int exp_count)
{
	int	k;

	k = 0;
	while (k < exp_count)
		new_args[(*j)++] = expanded[k++];
}

/**
 * @brief Expands wildcards in all command arguments
 *
 * Takes an array of arguments and expands any wildcard patterns.
 * Returns a new array with expanded arguments.
 *
 * @param args Original argument array
 * @param arg_count Number of arguments
 * @return New array with wildcards expanded
 */
char	**expand_wildcards_in_args(char **args, int arg_count)
{
	char	**new_args;
	char	**expanded;
	int		i;
	int		j;
	int		exp_count;

	if (!args)
		return (NULL);
	new_args = malloc(sizeof(char *) * (
				count_total_expanded(args, arg_count) + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	j = 0;
	while (i < arg_count)
	{
		expanded = expand_arg(args[i++], &exp_count);
		if (expanded)
		{
			copy_expanded_args(expanded, new_args, &j, exp_count);
			free(expanded);
		}
	}
	new_args[j] = NULL;
	return (new_args);
}
