/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:54:03 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:11:35 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
/* 
int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Input Error\n");
		return (1);
	}
	printf("Original isalpha: %d\n", isalpha(argv[1][0]));
	printf("Ft_isalpha: %d\n", ft_isalpha(argv[1][0]));
	return (0);
} */