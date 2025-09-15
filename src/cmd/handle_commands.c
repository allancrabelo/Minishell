/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 00:20:45 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/15 17:45:49 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/colors.h"

void	free_tokens(t_mini *mini)
{
	t_token	*cur;
	t_token	*tmp;

	cur = mini->token;
	mini->token = NULL;
	while (cur)
	{
		tmp = cur->next;
		free(cur->data);
		free(cur);
		cur = tmp;
	}
	free(mini->input);
}

void	handle_commands(t_mini *mini, char *input)
{
	t_token	*cur;

	if (input[0] == '#') //TODO usar para teste
	{
		printf(BOLD SCYAN "%s\n" SRESET, input);
		return ;
	}
	ft_tokenizer(mini, input);
	if (mini->token == NULL)
		return ;
	cur = mini->token;
	while (cur)
	{
		if (ft_strcmp("echo", cur->data) == 0)
		{
			mini->exit_status = ft_echo(mini, cur);
		}
		cur = cur->next;
	}
	free_tokens(mini);
}
