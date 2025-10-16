/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:28:21 by aaugusto          #+#    #+#             */
/*   Updated: 2025/10/16 10:07:46 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*tmp;

	len = ft_strlen(s);
	tmp = malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	len = 0;
	while (s[len])
	{
		tmp[len] = s[len];
		len++;
	}
	tmp[len] = '\0';
	return (tmp);
}

/* 
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf ("Input Error\n");
		return (1);
	}
	printf ("Original strdup: %s\n", strdup(argv[1]));
	printf ("Ft_strdup: %s\n", ft_strdup(argv[1]));

	return (0);
} */