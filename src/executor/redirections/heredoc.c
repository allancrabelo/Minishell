#include "../../../includes/minishell.h"

static char	*read_heredoc_input(char *delimeter, int *interruped)
{
	char	*content;
	char	*line;
	char	*temp;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (!*interruped)
			{
				ft_putstr_fd("bash: warning: here-document delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(delimeter, 2);
				ft_putstr_fd("')\n", 2);
				break;
			}
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free (line);
			break ;
		}
		temp = content;
		content = ft_strjoin(temp, line);
		free(temp);
		free(line);
		if (!content)
			break;
		temp = content;
		content = ft_strjoin(temp, "\n");
		free(temp);
		if (!content)
			break;
	}
	return (content);
}

static int	create_heredoc_file(char *delimiter)
{
	int		fd;
	char	*content;
	int		interrupted;

	interrupted = 0;
	fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	
	content = read_heredoc_input(delimiter, &interrupted);
	if (interrupted)
	{
		close(fd);
		unlink("/tmp/minishell_heredoc");
		return (-1);
	}
	
	if (content)
	{
		write(fd, content, ft_strlen(content));
		free(content);
	}
	close(fd);
	
	fd = open("/tmp/minishell_heredoc", O_RDONLY);
	unlink("/tmp/minishell_heredoc");
	
	return (fd);
}

int	redirect_heredoc(t_redir *redirect)
{
	int	fd;

	fd = create_heredoc_file(redirect->file);
	if (fd == -1)
		return (-1);
	
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

