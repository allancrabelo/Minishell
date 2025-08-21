/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:43:00 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:41:50 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;
	size_t	j;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	i = 0;
	j = dst_len;
	if (size == 0)
		return (ft_strlen(src));
	if (dst_len >= size)
		return (size + src_len);
	while (src[i] && i < size - dst_len - 1)
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (dst_len + src_len);
}
/* int	main(void)
{
	char a[20] = "42_";
	char b[] = "School";
	size_t	result;
	
	size_t c = 10;
	result = ft_strlcat(a, b, c);
	
	printf("Ft_strlcat return: %zu\n", result);
	printf("Destination Concatened value: %s\n", a);
	return (0);
} */