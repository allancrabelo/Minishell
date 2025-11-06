/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export_list_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 08:56:23 by mqueiros          #+#    #+#             */
/*   Updated: 2025/11/06 08:56:28 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static t_export	*merge_sorted_exports(t_export *a, t_export *b)
{
	t_export	*result;

	result = NULL;
	if (!a)
		return (b);
	if (!b)
		return (a);
	if (ft_strcmp(a->key, b->key) <= 0)
	{
		result = a;
		result->next = merge_sorted_exports(a->next, b);
	}
	else
	{
		result = b;
		result->next = merge_sorted_exports(a, b->next);
	}
	return (result);
}

static void	front_back_split(t_export *source,
		t_export **front, t_export **back)
{
	t_export	*fast;
	t_export	*slow;

	if (!source || !source->next)
	{
		*front = source;
		*back = NULL;
		return ;
	}
	slow = source;
	fast = source->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*front = source;
	*back = slow->next;
	slow->next = NULL;
}

static t_export	*merge_sort_exports(t_export *head)
{
	t_export	*front;
	t_export	*back;

	if (!head || !head->next)
		return (head);
	front = NULL;
	back = NULL;
	front_back_split(head, &front, &back);
	front = merge_sort_exports(front);
	back = merge_sort_exports(back);
	return (merge_sorted_exports(front, back));
}

void	print_export_list(t_mini *mini)
{
	t_export	*cur;

	if (!mini->export_list)
	{
		mini->exit_status = 0;
		return ;
	}
	mini->export_list = merge_sort_exports(mini->export_list);
	cur = mini->export_list;
	while (cur)
	{
		if (cur->value == NULL)
			printf("declare -x %s\n", cur->key);
		else
			printf("declare -x %s=\"%s\"\n", cur->key, cur->value);
		cur = cur->next;
	}
	mini->exit_status = 0;
}
