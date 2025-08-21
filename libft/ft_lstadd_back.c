/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 16:26:31 by aaugusto          #+#    #+#             */
/*   Updated: 2025/04/09 16:20:19 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp -> next;
	temp -> next = new;
}
/* 
int	main(void)
{
	t_list *lst;
	t_list *new;

	lst = ft_lstnew("42");
	new = ft_lstnew("School");
	ft_lstadd_back(&lst, new);
	printf("%s\n", (char *)lst->next->content);
	return (0);
} */