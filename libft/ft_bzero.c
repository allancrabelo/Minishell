/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 12:55:07 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:19:16 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t len)
{
	unsigned char	*ptr;

	ptr = s;
	while (len--)
		*ptr++ = 0;
}
/* int	main(void)
{
	char str[] = "42_School_Porto!";

	printf("Before: %s\n", str);
	ft_bzero(str + 9, 1);
	printf("After: %s\n", str);
	return (0);
}
 */