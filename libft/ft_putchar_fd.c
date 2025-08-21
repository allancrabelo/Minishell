/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:41:36 by aaugusto          #+#    #+#             */
/*   Updated: 2025/02/28 17:17:30 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/* int	 main(void)
{
	ft_putchar_fd('4', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('2', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('S', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('c', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('h', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('o', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('o', 1);
	ft_putchar_fd('\n', 1);
	ft_putchar_fd('l', 1);
	ft_putchar_fd('\n', 1);

	return (0);
} */