/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:46:54 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:21:35 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
/* 
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Input Error\n");
		return (1);
	}
	printf("Original isascii: %d\n", isascii(argv[1][0]));
	printf("Ft_isascii: %d\n", ft_isascii(argv[1][0]));

	return (0);
} */