#include "../../../includes/minishell.h"

static char	*read_heredoc_input(char *delimeter)
{
	char	*content;
	char	*line;
	char	*temp;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			perror("minishell:");
			break ;
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break ;
		}
		temp = ft_strjoin(content, line);
		free(content);
		free(line);
		content = ft_strjoin(temp, "\n");
		free(temp);
		if (!content)
			return (NULL);
	}
	return (content);
}

static int	create_heredoc_file(char *delimeter)
{
	int		fd;
	char	*filename;
	char	*content;

	filename = "/tmp/minishell_heredoc";
	setup_heredoc_signals();
	content = read_heredoc_input(delimeter);
	signal_init();
	if (!content)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(content), -1);
	write(fd, content, ft_strlen(content));
	close(fd);
	free(content);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	redirect_heredoc(t_redir *redirect)
{
	int	fd;

	fd = create_heredoc_file(redirect->file);
	if (fd == -1)
		return (ft_putstr_fd("minishell: heredoc failed\n", 2), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}