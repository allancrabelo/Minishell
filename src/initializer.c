/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:44:05 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:44:06 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes a t_mini structure with default values
 *
 * This function initializes all members of the t_mini structure to their
 * default starting state. Pointers are set to NULL, numeric values to 0,
 * and the environment pointer is assigned.
 *
 * @param mini Pointer to the t_mini structure to initialize
 * @param envp Array of environment variables
 * @return void
 */
void	mini_init(t_mini *mini, char **envp)
{
	mini->ast = NULL;
	mini->token = NULL;
	mini->input = NULL;
	mini->envp = envp;
	mini->export_list = NULL;
	mini->exit_status = 0;
	mini->env_list = NULL;
	mini->pwd = NULL;
	mini->heredoc = NULL;
	mini->heredoc_fd = 0;
	mini->heredoc_signal = 0;
	mini->child = 0;
}
