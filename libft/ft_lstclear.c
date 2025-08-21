/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:05:59 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/07 15:25:07 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
}
/* void	ft_del(void *content)
{
	free(content);
}

int main(void)
{
	t_list *first = malloc(sizeof(t_list));
	t_list *second = malloc(sizeof(t_list));
	t_list *third = malloc(sizeof(t_list));

	if (!first || !second || !third)
		return (1);

	first->content = ft_strdup("Node 1");
	first->next = second;

	second->content = ft_strdup("Node 2");
	second->next = third;

	third->content = ft_strdup("Node 3");
	third->next = NULL;

	printf("List Before:\n");
	t_list *temp = first;
	while (temp)
	{
		printf("%s\n", (char *)temp->content);
		temp = temp->next;
	}

	ft_lstclear(&first, ft_del);

	if (!first)
		printf("\nclear!\n");
	else
		printf("\nError: List still here!\n");

	return 0;
} */