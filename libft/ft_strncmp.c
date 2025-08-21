/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 21:54:53 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 18:20:55 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if ((unsigned char)s1[i] > (unsigned char)s2[i])
			return (1);
		if ((unsigned char)s1[i] < (unsigned char)s2[i])
			return (-1);
		i++;
	}
	return (0);
}
/* 
int	main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Input Error");
		return (1);
	}

	size_t n;
	int result;
	
	n = atoi(argv[3]);
	result = strncmp(argv[1], argv[2], n);
	
	printf("Original strncmp: %d\n", result);

	result = ft_strncmp(argv[1], argv[2], n);

	printf("Ft_strncmp: %d\n", result);
	
	return (0);
} */