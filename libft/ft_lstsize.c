/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 10:18:02 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:29:09 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
/* int	main(void)
{
	t_list *list = NULL;

	t_list *node1 = malloc(sizeof(t_list));
	node1->content = "First node";
	node1->next = NULL;
	ft_lstadd_front(&list, node1);
	
	t_list *node2 = malloc(sizeof(t_list));
	node2->content = "Second node";
	node2->next = NULL;
	ft_lstadd_front(&list, node2);
	
	t_list *node3 = malloc(sizeof(t_list));
	node3->content = "Third node";
	node3->next = NULL;
	ft_lstadd_front(&list, node3);
	
	int size = ft_lstsize(list);
	printf("Total Nodes: %d\n", size);
	
	return (0);
} */