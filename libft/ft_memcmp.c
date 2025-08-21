/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 15:26:56 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:33:23 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}
/* int	main(void)
{
	char str1[] = "Hello";
	char str2[] = "Hellx";
	char str3[] = "Hello";

	printf("Comparing str1 and str2: %d\n", ft_memcmp(str1, str2, 5));
	printf("Comparing str1 and str3: %d\n", ft_memcmp(str1, str3, 5));
	printf("Comparing str2 and str1: %d\n", ft_memcmp(str2, str1, 5));
	
	return (0);
} */