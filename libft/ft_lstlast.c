/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 15:47:31 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:25:51 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*last;

	if (!lst)
		return (NULL);
	last = lst;
	while (last->next)
		last = last->next;
	return (last);
}
/* 
int main(void)
{
	t_list *lst;
	t_list *last;

	lst = ft_lstnew("42");
	lst->next = ft_lstnew("School");
	lst->next->next = ft_lstnew("Porto");
	last = ft_lstlast(lst);
	printf("%s\n", (char *)last->content);
	return (0);
} */