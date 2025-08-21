/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:22:43 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 23:23:19 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_ws(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	count_words(const char *s)
{
	int	i;
	int	cnt;
	int	in;

	i = 0;
	cnt = 0;
	in = 0;
	while (s[i])
	{
		if (is_ws(s[i]))
			in = 0;
		else if (!in)
		{
			in = 1;
			cnt++;
		}
		i++;
	}
	return (cnt);
}

int	word_len(const char *s)
{
	int	i;

	i = 0;
	while (s[i] && !is_ws(s[i]))
		i++;
	return (i);
}

char	*dup_word(const char *s, int len)
{
	char	*p;
	int		i;

	p = (char *)malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (i < len)
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
