/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:20 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 17:50:50 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_redir	*init_redir(t_token_type type, char **file)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (free(*file), NULL);
	new_redir->type = type;
	new_redir->file = *file;
	new_redir->heredoc_delimeter = NULL;
	new_redir->fd = -1;
	return (new_redir);
}

int	redirect_in(t_redir *redirect, t_mini *mini)
{
	int	fd;

	fd = open(redirect->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirect->file, 2);
		if (errno == ENOENT)
			ft_putstr_fd(": No such file or directory\n", 2);
		else if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else
			ft_putstr_fd(": Failed to open file\n", 2);
		mini->exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_heredoc(t_redir *redirect, t_mini *mini)
{
	(void)mini;
	if (!redirect || redirect->fd < 0)
	{
		print_command_error("heredoc", "pipe not available");
		return (-1);
	}
	if (dup2(redirect->fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (-1);
	}
	close(redirect->fd);
	redirect->fd = -1;
	return (0);
}

int	redirect_out(t_redir *redirect, t_mini *mini)
{
	int	fd;

	fd = open (redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirect->file, 2);
		if (errno == ENOENT)
			ft_putstr_fd(": No such file or directory\n", 2);
		else if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else if (errno == EISDIR)
			ft_putstr_fd(": Is a directory\n", 2);
		else
			ft_putstr_fd(": Cannot open file\n", 2);
		mini->exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (ft_putstr_fd("minishell: redirection failed\n", 2), -1);
	}
	close(fd);
	return (0);
}

int	redirect_append(t_redir *redirect, t_mini *mini)
{
	int	fd;

	fd = open (redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirect->file, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else if (errno == EISDIR)
			ft_putstr_fd(": Is a directory\n", 2);
		else
			ft_putstr_fd(": Cannot open file\n", 2);
		mini->exit_status = 1;
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		ft_putstr_fd("minishell: append redirection failed\n", 2);
		return (-1);
	}
	close(fd);
	return (0);
}
