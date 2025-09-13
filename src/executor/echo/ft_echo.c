/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:27:39 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/13 19:45:38 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	flag_verification(t_token **cur)
{
	char	ret;
	size_t	i;

	ret = 0;
	while (*cur)
	{
		i = 0;
		if ((*cur)->data[i] != '-')
			break ;
		i++;
		if ((*cur)->data[i] == '\0')
			break ;
		while ((*cur)->data[i] == 'n')
			i++;
		if ((*cur)->data[i] != '\0')
			break ;
		ret = 'n';
		*cur = (*cur)->next;
	}
	return (ret);
}

int	ft_echo(t_mini *mini, t_token *echo) //	CUIDADO BONUS
{
	int		flag;
	t_token	*cur;

	(void) mini;
	cur = echo->next;
	flag = flag_verification(&cur);
	while (cur)
	{
		if (cur->type > TOKEN_PIPE) //		TODO Handle Operator
		{
			cur = cur->next->next;
			continue ;
		}
		printf("%s", cur->data);
		if (cur->next)
			printf(" ");
		cur = cur->next;
	}
	if (!flag)
		printf("\n");
	return (0);
}
