/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 22:36:00 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:40:00 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*string;
	unsigned char	new_c;

	new_c = (unsigned char)c;
	string = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		string[i] = new_c;
		i++;
	}
	return (s);
}
/* int	main(void)
{
	char array_a[] = "encryption";
	
	memset(array_a, '*', 9);
	
	printf("Orginal memset: %s\n", array_a);

	char array_b[] = "encryption";

	ft_memset(array_b, '*', 9);
	
	printf("Ft_memset: %s\n", array_b);
	return (0);
} */