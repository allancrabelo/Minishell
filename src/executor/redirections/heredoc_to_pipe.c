/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_to_pipe.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:21 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:45:22 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delimiter(char *line, char *delimiter)
{
	if (ft_strlen(line) == ft_strlen(delimiter)
		&& !ft_strncmp(line, delimiter, ft_strlen(line)))
	{
		free(line);
		return (1);
	}
	return (0);
}

static void	close_heredoc_err(char *delimiter)
{
	if (g_signal == 130)
	{
		rl_clear_history();
		return ;
	}
	ft_putstr_fd("minishell: warning: here-document \
delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	return ;
}

void	write_heredoc_to_pipe(int write_fd, char *delimiter, t_mini *mini)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close_heredoc_err(delimiter);
			break ;
		}
		if (is_delimiter(line, delimiter))
			break ;
		expanded_line = expand_heredoc_line(mini, line);
		if (expanded_line)
		{
			ft_putendl_fd(expanded_line, write_fd);
			free(expanded_line);
		}
		else
			ft_putendl_fd(line, write_fd);
		free(line);
	}
	close(write_fd);
}
