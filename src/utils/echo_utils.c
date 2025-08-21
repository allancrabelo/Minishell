/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:13:58 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 23:21:24 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	*free_all(char **split, int k)
{
	while (k >= 0)
	{
		if (split[k])
			free(split[k]);
		k--;
	}
	free(split);
	return (NULL);
}

char		**ft_split_ws(const char *s)
{
	char	**split;
	int		i;
	int		k;
	int		len;

	if (!s)
		return (NULL);
	split = (char **)malloc((count_words(s) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	k = 0;
	while (s[i])
	{
		while (s[i] && is_ws(s[i]))
			i++;
		if (!s[i])
			break ;
		len = word_len(s + i);
		split[k] = dup_word(s + i, len);
		if (!split[k++])
			return (free_all(split, k - 2));
		i += len;
	}
	split[k] = NULL;
	return (split);
}