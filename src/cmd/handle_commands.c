/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:20:45 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/01 19:19:11 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_tokens(t_token **token)
{
	t_token *cur;
	t_token *tmp;
	
	cur = *token;
	*token = NULL;
	while (cur)
	{
		tmp = cur->next;
		free(cur->data);
		free(cur);
		cur = tmp;
	}
}


void	handle_commands(t_mini *mini, char *input)
{
	t_token	*cur;
	int		i = 0;

	ft_tokenizer(mini, input);
	if (mini->token == NULL)
		return ;
	cur = mini->token;
	while (cur)
	{
		printf("[%d] %s\n", i, cur->data);
		i++;
		cur = cur->next;
	}
	printf("\n");
	free_tokens(&mini->token);
}
