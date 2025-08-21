/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 00:04:52 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/11 19:10:31 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putpointer_aux(unsigned long long n)
{
	char	*base;
	int		len;

	base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
		len += ft_putpointer_aux(n / 16);
	len += ft_putchar(base[n % 16]);
	return (len);
}

int	ft_putpointer(void *pointer)
{
	if (!pointer)
		return (ft_putstr("(nil)"));
	ft_putstr("0x");
	return (ft_putpointer_aux((unsigned long long)pointer) + 2);
}

static size_t	ft_len_puthex(unsigned int n)
{
	size_t	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 16;
		digits++;
	}
	return (digits);
}

static void	ft_print_puthex(unsigned int n, char up_or_low)
{
	char	*base;

	if (up_or_low == 'a')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (n >= 16)
	{
		ft_print_puthex(n / 16, up_or_low);
		ft_print_puthex(n % 16, up_or_low);
	}
	else
	{
		write(1, &base[n], 1);
	}
}

int	ft_puthex(unsigned int n, char up_or_low)
{
	if (n == 0)
		return (write(1, "0", 1));
	ft_print_puthex(n, up_or_low);
	return (ft_len_puthex(n));
}
