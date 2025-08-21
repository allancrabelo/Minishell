/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 00:18:57 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:20:10 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
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
	printf("Original isalnum: %d\n", isalnum(argv[1][0]));
	printf("Ft_isalnum: %d\n", ft_isalnum(argv[1][0]));
	return (0);
} */