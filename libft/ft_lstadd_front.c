/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 18:11:51 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:24:26 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new || !lst)
		return ;
	new->next = *lst;
	*lst = new;
}
/* int	main(void)
{
	t_list **list = malloc(sizeof(t_list) * 3);
	if (list == NULL) {
		return 1;
	}
	
	t_list *newNode = malloc(sizeof(t_list));
	if (newNode == NULL) {
		return 1;
	}
	newNode->content = "First node";
	newNode->next = NULL;

	ft_lstadd_front(list, newNode);

	t_list *test = malloc(sizeof(t_list));
	if (test == NULL) {
		return 1;
	}
	test->content = "Second node";
	test->next = NULL;
	ft_lstadd_front(list, test);


	t_list *test1 = malloc(sizeof(t_list));
	if (test1 == NULL) {
		return 1;
	}
	test1->content = "Last node";
	test1->next = NULL;
	ft_lstadd_front(list, test1);
	
	t_list *current = *list;
	while (current != NULL) {
		ft_putendl_fd(current->content, 1);
		current = current->next;
	}
	free(list);
	return (0);
} */