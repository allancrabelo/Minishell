#include "../../../includes/minishell.h"

/* static char	*read_heredoc_input(char *delimeter, int *interrupted)
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
			if (!*interrupted)
			{
				ft_putstr_fd("minishell: warning: here-document \
delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(delimeter, 2);
				ft_putstr_fd("')\n", 2);
				break ;
			}
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break ;
		}
		temp = content;
		content = ft_strjoin(temp, line);
		free(temp);
		free(line);
		if (!content)
			break ;
		temp = content;
		content = ft_strjoin(temp, "\n");
		free(temp);
		if (!content)
			break ;
	}
	return (content);
} */

static void	heredoc_append(t_heredoc **heredoc, t_heredoc *new)
{
	t_heredoc	*cur;

	if (!(*heredoc))
		*heredoc = new;
	else
	{
		cur = *heredoc;
		while (cur && cur->next)
			cur = cur->next;
		cur->next = new;
	}

}

static char *get_filename(int *heredoc_fd)
{
	char	*filename;
	char	*heredoc_string;

	heredoc_string = ft_itoa(*heredoc_fd);
	if (!heredoc_string)
		return (NULL);
	filename = ft_strjoin(".heredoc_", heredoc_string);
	free(heredoc_string);
	(*heredoc_fd)++;
	return (filename);
}

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

static void	write_heredoc(int fd, char *delimiter, int interrupted)
{
	char	*line;
	int		input_dup;

	input_dup = dup(STDIN_FILENO);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (!interrupted)
			{
				ft_putstr_fd("minishell: warning: here-document \
delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(delimiter, 2);
				ft_putstr_fd("')\n", 2);
				break ;
			}
		}
		if (is_delimiter(line, delimiter))
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	dup2(input_dup, STDIN_FILENO);
	close(input_dup);
}

void	heredoc_cleaner(t_heredoc **heredoc, int unlinker)
{
	t_heredoc	*cur;
	t_heredoc	*tmp;

	cur = *heredoc;
	while (cur)
	{
		tmp = cur->next;
		if (cur->heredoc_delimeter)
			free(cur->heredoc_delimeter);
		if (cur->filename)
		{
			if (unlinker)
				unlink(cur->filename);
			free(cur->filename);
		}
		free(cur);
		cur = tmp;
	}
	*heredoc = NULL;
}

static void	wait_heredoc(t_mini *mini, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_signal = 130;
		mini->heredoc_signal = 1;
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_signal = 130;
		mini->heredoc_signal = 1;
	}
}

static void	read_heredoc(t_mini *mini, char *filename, char *delimiter)
{
	int	fd;
	int	pid;
	int	interrupted;

	interrupted = 0;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_command_error(filename, strerror(errno));
		return ;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		ft_free_all(mini, errno, 1);
	else if (pid == 0)
	{
		setup_heredoc_signals();
		write_heredoc(fd, delimiter, interrupted);
		close(fd);
		heredoc_cleaner(&mini->heredoc, 0);
		ft_free_all(mini, g_signal, 1);
	}
	close(fd);
	wait_heredoc(mini, pid);
	signal_init();
}

void	create_heredoc_file(t_mini *mini, char *delimiter, t_heredoc **heredoc)
{
	t_heredoc	*new;

	new = malloc (sizeof(t_heredoc));
	if (!new)
		ft_free_all(mini, ENOMEM, 1);
	heredoc_append(heredoc, new);
	new->next = NULL;
	new->heredoc_delimeter = ft_strdup(delimiter);
	if (!new->heredoc_delimeter)
		ft_free_all(mini, ENOMEM, 1);
	new->filename = get_filename(&mini->heredoc_fd);
	if (!new->filename)
		ft_free_all(mini, ENOMEM, 1);
	read_heredoc(mini, new->filename, new->heredoc_delimeter);
	if (new)
		free(new);
}

