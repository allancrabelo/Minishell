/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:29 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	execute_and_node(t_mini *mini, t_ast *node)
{
	execute_ast_node(mini, node->left);
	if (mini->exit_status == 0)
		execute_ast_node(mini, node->right);
	return (mini->exit_status);
}

int	execute_or_node(t_mini *mini, t_ast *node)
{
	execute_ast_node(mini, node->left);
	if (mini->exit_status != 0)
		execute_ast_node(mini, node->right);
	return (mini->exit_status);
}
