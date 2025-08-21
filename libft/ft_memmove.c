/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:32:48 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/09 10:33:50 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dest && !src)
		return (dest);
	if (dest < src)
		return (ft_memcpy(dest, src, n));
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (n--)
		d[n] = s[n];
	return (dest);
}
/* 
int	main(void)
{
	char str[] = "Original String";
	char dest[16];
	char *result;

	result = ft_memmove(dest, str, 16);

	printf("Ft_memmove: %s\n", result);
	return (0);
} */
