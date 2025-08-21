/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 00:04:50 by aaugusto          #+#    #+#             */
/*   Updated: 2025/07/10 00:21:16 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_conversions(char c, va_list arglist)
{
	if (c == 'c')
		return (ft_putchar(va_arg(arglist, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(arglist, char *)));
	else if (c == 'p')
		return (ft_putpointer(va_arg(arglist, void *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnumber(va_arg(arglist, int)));
	else if (c == 'u')
		return (ft_putunsigned(va_arg(arglist, unsigned int)));
	else if (c == 'x')
		return (ft_puthex(va_arg(arglist, unsigned int), 'a'));
	else if (c == 'X')
		return (ft_puthex(va_arg(arglist, unsigned int), 'A'));
	else if (c == '%')
		return (ft_putchar('%'));
	else
		return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	arglist;
	size_t	i;
	int		len;

	i = -1;
	len = 0;
	va_start(arglist, str);
	while (str[++i])
	{
		if (str[i] == '%')
			len += ft_conversions(str[++i], arglist);
		else
		{
			ft_putchar(str[i]);
			len++;
		}
	}
	va_end(arglist);
	return (len);
}
