/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 00:04:55 by aaugusto          #+#    #+#             */
/*   Updated: 2025/07/10 00:34:08 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (ft_putstr("(null)"));
	while (str[i])
	{
		ft_putchar_fd(str[i], 1);
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char			*a;
	int				digits;
	unsigned int	num;

	num = n;
	digits = 1;
	while (num / 10 != 0)
	{
		digits++;
		num /= 10;
	}
	a = (char *)malloc((digits + 1) * sizeof(char));
	if (a == NULL)
		return (NULL);
	a[digits] = '\0';
	while (digits > 0)
	{
		a[digits - 1] = n % 10 + '0';
		digits--;
		n /= 10;
	}
	return (a);
}

int	ft_putunsigned(unsigned int n)
{
	int		i;
	char	*a;

	i = 0;
	a = ft_itoa(n);
	i += ft_putstr(a);
	free (a);
	return (i);
}

int	ft_putnumber(int n)
{
	int		i;
	char	*a;

	i = 0;
	a = ft_itoa(n);
	i += ft_putstr(a);
	free(a);
	return (i);
}

int	ft_putchar(char c)
{
	ft_putchar_fd(c, 1);
	return (1);
}
