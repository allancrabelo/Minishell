/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_pipe_update_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:44 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * @brief Updates exit status after pipe execution
 *
 * Processes exit status from both pipe commands, prioritizing
 * the right command's status. Handles signal termination cases.
 *
 * @param status_right Exit status of right pipe command
 * @param status_left Exit status of left pipe command
 * @param mini Pointer to main shell structure
 * @return int Final exit status for the pipe sequence
 */
int	signals_pipe_update(int status_right, int status_left, t_mini *mini)
{
	if (WIFSIGNALED(status_left) && WTERMSIG(status_left) == SIGINT)
		write(1, "\n", 1);
	if (WIFEXITED(status_right))
		mini->exit_status = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
	{
		if (WTERMSIG(status_right) == SIGINT)
			write(1, "\n", 1);
		mini->exit_status = 128 + WTERMSIG(status_right);
	}
	return (mini->exit_status);
}
