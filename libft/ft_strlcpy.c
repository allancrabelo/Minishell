/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:17:47 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:42:54 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return ((size_t)ft_strlen(src));
	while (i < (size - 1) && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return ((size_t)ft_strlen(src));
}
/* 
int	main(void)
{
	char dest[50];
	char src[] = "Function String Copy";
	
	printf("Function Return: %zu\n", ft_strlcpy(dest, src, 21));
	printf("Dest Value: %s\n", dest);
	return (0);
} */