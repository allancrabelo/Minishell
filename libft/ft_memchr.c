/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:05:37 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:30:40 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	ch;

	str = (unsigned char *)s;
	ch = (unsigned char)c;
	while (n > 0)
	{
		if (*str == ch)
			return ((void *)str);
		str++;
		n--;
	}
	return (NULL);
}
/* int	main(void)
{
	char data[] = "42 School";
	
	char *result = (char *)ft_memchr(data, 'S', 9);
	printf("Test 1: %s\n", result ? result : "Not found");

	
	result = (char *)ft_memchr(data, 'X', 9);
	printf("Test 2: %s\n", result ? result : "Not found");

	
	result = (char *)ft_memchr(data, '\0', 10);
	printf("Test 3: %p\n", result);
	
	unsigned char binData[] = {0xAA, 0xBB, 0xCC, 0xDD};
	unsigned char *binResult = (unsigned char *)ft_memchr(binData, 0xCC, 4);
	printf("Test 4: %p\n", binResult);
} */