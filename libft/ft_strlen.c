/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:07:27 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:43:04 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
/* 
int	main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Input Error\n");
		return (1);
	}
	printf("Original strlen: %zu\n", strlen(argv[1]));
	printf("Ft_strlen: %d\n", ft_strlen(argv[1]));
	return (0);
} */