/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:11:32 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/09 16:49:09 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while ((s[i] != (char)c) && s[i])
		i++;
	if (s[i] != (char)c)
		return (NULL);
	else
		return ((char *)&s[i]);
}
/* 
int	main(void)
{
	const char *str = "42School";
	char chr1 = 'S';
	char chr2 = 'o';
	char chr3 = 'z';
	char chr4 = '\0';

	printf("Searching '%c' in \"%s\": %s\n", chr1, str, ft_strchr(str, chr1));
	printf("Searching '%c' in \"%s\": %s\n", chr2, str, ft_strchr(str, chr2));
	printf("Searching '%c' in \"%s\": %s\n", chr3, str, ft_strchr(str, chr3));
	printf("Searching '\\0' in \"%s\": %p\n", str, ft_strchr(str, chr4));

	return (0);
} */