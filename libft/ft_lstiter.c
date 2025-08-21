/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:17:59 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:12:00 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		(*f)(lst->content);
		lst = lst->next;
	}
}
/* 
void	ft_print(void *content)
{
	printf("%s\n", (char *)content);
}
int	main(void)
{
	t_list *lst1 = malloc(sizeof(t_list));
	t_list *lst2 = malloc(sizeof(t_list));
	t_list *lst3 = malloc(sizeof(t_list));

	if (!lst1 || !lst2 || !lst3)
		return (1);
	
	lst1->content = strdup("42");
	lst1->next = lst2;
	
	lst2->content = strdup("School");
	lst2->next = lst3;
	
	lst3->content = strdup("Porto");
	lst3->next = NULL;

	ft_lstiter(lst1, ft_print);
	
	free(lst1->content);
	free(lst1);
	free(lst2->content);
	free(lst2);
	free(lst3->content);
	free(lst3);
	
	return (0);
} */