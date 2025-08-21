/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 17:08:44 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:11:27 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
/* int	main(void)
{
	size_t num_elements;
	size_t element_size;
	int	*arr;
	
	num_elements = 5;
	element_size = sizeof(int);
	arr = (int *)ft_calloc(num_elements, element_size);
	if(!arr)
		return(1);
	
	int	all_zero = 1;
	size_t i = 0;
	while(i < num_elements)
	{
		if (arr[i] != 0)
		{
			printf("Error: arr[%zu] isn't zero! Value: %d\n.", i, arr[i]);
			all_zero = 0;
		}
		i++;
	}
	if (all_zero)
		printf("Memory inicialized with 0\n");
	
	free(arr);
	return (0);
} */