/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 10:23:27 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:11:46 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putonres(int n, int dig, int i, char *res)
{
	while (i < dig)
	{
		res[dig - 1] = n % 10 + '0';
		n /= 10;
		dig--;
	}
}

static int	ft_countdig(int n)
{
	int	res;

	res = 0;
	if (n <= 0)
		res++;
	while (n != 0)
	{
		n = n / 10;
		res++;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		dig;
	int		i;

	dig = ft_countdig(n);
	res = (char *)malloc(sizeof(char) * (dig + 1));
	if (!res)
		return (NULL);
	i = 0;
	if (n == -2147483648)
	{
		res[i++] = '-';
		res[i++] = '2';
		n = 147483648;
	}
	if (n < 0)
	{
		res[i++] = '-';
		n *= -1;
	}
	ft_putonres(n, dig, i, res);
	res[dig] = '\0';
	return (res);
}
/* 
int	main(void)
{
	int x = 123456;
	int y = -123456;
	int min = -2147483648;
	int max = 2147483647;

	printf("%s\n", ft_itoa(x));
	printf("%s\n", ft_itoa(y));
	printf("%s\n", ft_itoa(min));	
	printf("%s\n", ft_itoa(max));
	return (0);
} */