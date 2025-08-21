/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 00:11:37 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:22:23 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
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
	printf("Original isdigit: %d\n", isdigit(argv[1][0]));
	printf("Original isdigit: %d\n", ft_isdigit(argv[1][0]));

	return (0);
} */