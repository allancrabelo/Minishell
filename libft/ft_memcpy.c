/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:07:50 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:34:09 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*destination;
	const unsigned char	*source;
	size_t				i;

	if (!dest && !src)
		return (dest);
	destination = dest;
	source = src;
	i = 0;
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dest);
}
/* 
int	main(void)
{
	char str[] = "Original String";
	char dest[16];
	char *result;

	result = ft_memcpy(dest, str, 16);

	printf("Ft_memcpy: %s\n", result);
	return (0);
} */