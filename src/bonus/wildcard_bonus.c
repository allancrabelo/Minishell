/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:05 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:44:06 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Matches a string against a wildcard pattern
 *
 * Recursively checks if a string matches a pattern containing '*'.
 * The '*' character matches zero or more of any character.
 *
 * @param pattern The pattern with wildcards
 * @param str The string to match
 * @return 1 if matches, 0 otherwise
 */
static int	match_pattern(const char *pattern, const char *str)
{
	if (*pattern == '\0' && *str == '\0')
		return (1);
	if (*pattern == '*')
	{
		if (match_pattern(pattern + 1, str))
			return (1);
		if (*str != '\0' && match_pattern(pattern, str + 1))
			return (1);
		return (0);
	}
	if (*pattern == *str && *str != '\0')
		return (match_pattern(pattern + 1, str + 1));
	return (0);
}

/**
 * @brief Counts files matching the wildcard pattern
 *
 * Reads current directory and counts files that match the pattern.
 * Hidden files (starting with '.') are skipped.
 *
 * @param pattern The wildcard pattern to match
 * @return Number of matching files
 */
static int	count_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

/**
 * @brief Collects matching filenames into an array
 *
 * Allocates and fills an array with names of files matching the pattern.
 *
 * @param pattern The wildcard pattern
 * @param count Number of expected matches
 * @return Array of matched filenames, NULL-terminated
 */
static char	**collect_matches(const char *pattern, int count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				i;

	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (free(matches), NULL);
	i = 0;
	entry = readdir(dir);
	while (entry && i < count)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
			matches[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	matches[i] = NULL;
	closedir(dir);
	return (matches);
}

/**
 * @brief Sorts an array of strings alphabetically
 *
 * Uses bubble sort to arrange strings in lexicographic order.
 *
 * @param matches Array of strings to sort
 * @param count Number of strings in the array
 * @return void
 */
static void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(matches[j], matches[j + 1]) > 0)
			{
				temp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

/**
 * @brief Expands a wildcard pattern to matching filenames
 *
 * Finds all files in current directory matching the pattern and
 * returns them in a sorted array. If no matches found, returns NULL.
 *
 * @param pattern Pattern with * wildcards (e.g., "*.c")
 * @return Array of matching filenames (sorted), or NULL if no matches
 */
char	**expand_wildcard(const char *pattern)
{
	char	**matches;
	int		count;

	if (!ft_strchr(pattern, '*'))
		return (NULL);
	count = count_matches(pattern);
	if (count == 0)
		return (NULL);
	matches = collect_matches(pattern, count);
	if (!matches)
		return (NULL);
	sort_matches(matches, count);
	return (matches);
}
