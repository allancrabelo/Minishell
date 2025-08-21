/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:06:13 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/09 10:15:18 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	return (c >= 32 && c <= 126);
}
/* 
int	main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Input Error\n");
        return (1);
    }
    printf("Original isprint: %d\n", isprint(argv[1][0]));
    printf("Ft_isprint: %d\n", ft_isprint(argv[1][0]));
	return (0);
} */