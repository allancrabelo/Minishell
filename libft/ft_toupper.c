/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:52:57 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:47:49 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
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
	printf("Original toupper: %d\n", toupper(argv[1][0]));
	printf("Ft_toupper: %d\n", ft_toupper(argv[1][0]));
	return (0);
} */