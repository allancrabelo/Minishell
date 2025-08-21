/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:17:33 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:29:57 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}
/* int	main(void)
{
	char	*str;
	t_list	*lst;

	str = (char *)malloc(sizeof(char) * 16);
	str = "42 School Porto";
	lst = ft_lstnew((void *)str);
	ft_putendl_fd(lst -> content, 1);
	free(lst);
	return (0);
}*/