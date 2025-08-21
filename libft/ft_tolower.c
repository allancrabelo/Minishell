/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:03:14 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:47:33 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
/* 
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Input Error\n");
		return (1);
	}
	printf("Original tolower: %d\n", tolower(argv[1][0]));
	printf("Ft_tolower: %d\n", ft_tolower(argv[1][0]));
	return (0);
} */