#include "../../../includes/minishell.h"

int	redirect_in(t_redir *redirect)
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

int	redirect_out(t_redir *redirect)
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
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		ft_putstr_fd("minishell: redirection failed\n", 2);
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirect_append(t_redir *redirect)
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
