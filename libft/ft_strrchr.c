/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:04:27 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/09 16:39:23 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int		i;

	i = ft_strlen(str);
	while (i >= 0)
	{
		if (str[i] == (char)c)
			return ((char *)&(str[i]));
		i--;
	}
	return (NULL);
}
/* int     main(void)
{
	const char *str = "42School";
	char chr1 = 'S';
	char chr2 = 'o';
	char chr3 = 'z';
	char chr4 = '\0';

	printf("Searching '%c' in \"%s\": %s\n", chr1, str, ft_strrchr(str, chr1));
	printf("Searching '%c' in \"%s\": %s\n", chr2, str, ft_strrchr(str, chr2));
	printf("Searching '%c' in \"%s\": %s\n", chr3, str, ft_strrchr(str, chr3));
	printf("Searching '\\0' in \"%s\": %p\n", str, ft_strrchr(str, chr4));

	return (0);
} */