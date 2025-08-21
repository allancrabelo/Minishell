/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:28:21 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/09 10:28:43 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	size_t	len;

	len = ft_strlen(src) + 1;
	dest = (char *)malloc((len) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	ft_memcpy(dest, src, len);
	return (dest);
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