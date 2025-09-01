/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:20:45 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/01 16:57:58 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
void	handle_commands(t_mini *mini, char *input)
{
	t_token	*tokens;
	t_token	*cur;
	int		i = 0;

	tokens = NULL;
	ft_tokenizer(mini, input, &tokens);
	if (!tokens)
		return ;
	cur = tokens;
	while (cur)
	{
		printf("[%d] %s\n", i, cur->data);
		i++;
		cur = cur->next;
	}
	printf("\n");
}
