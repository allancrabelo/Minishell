#include "../../../includes/minishell.h"

static char	*extract_and_expand_var(t_mini *mini, char *line, int *i)
{
	char	*var_name;
	char	*value;
	int		start;
	int		len;

	(*i)++;
	start = *i;
	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(mini->exit_status));
	}
	len = 0;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	if (len == 0)
		return (ft_strdup("$"));
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, line + start, len + 1);
	value = get_env_var(mini, var_name);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static char	*expand_heredoc_line(t_mini *mini, char *line)
{
	char	*result;
	char	*temp;
	int		i;
	int		j;
	int		len;
	int temp_len;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	len = 4096;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i] && j < len - 1)
	{
		if (line[i] == '$' && line[i + 1])
		{
			temp = extract_and_expand_var(mini, line, &i);
			if (temp)
			{
				temp_len = ft_strlen(temp);
				if (j + temp_len < len)
				{
					ft_strlcpy(result + j, temp, len - j);
					j += temp_len;
				}
				free(temp);
			}
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (result);
}

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

static char	*get_filename(int *heredoc_fd)
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

static void	write_heredoc(int fd, char *delimiter, int interrupted, t_mini *mini)
{
	char	*line;
	char	*expanded_line;

	(void)interrupted;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document \
delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (is_delimiter(line, delimiter))
			break ;
		expanded_line = expand_heredoc_line(mini, line);
		if (expanded_line)
		{
			ft_putendl_fd(expanded_line, fd);
			free(expanded_line);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
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
	pid_t	pid;
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
	{
		close(fd);
		ft_free_all(mini, errno, 1);
	}
	else if (pid == 0)
	{
		setup_heredoc_signals();
		write_heredoc(fd, delimiter, interrupted, mini);
		close(fd);
		exit(g_signal);
	}
	close(fd);
	wait_heredoc(mini, pid);
	signal_init();
}

void	create_heredoc_file(t_mini *mini, char *delimiter, t_heredoc **heredoc)
{
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		ft_free_all(mini, ENOMEM, 1);
	new->next = NULL;
	new->heredoc_delimeter = ft_strdup(delimiter);
	if (!new->heredoc_delimeter)
	{
		free(new);
		ft_free_all(mini, ENOMEM, 1);
	}
	new->filename = get_filename(&mini->heredoc_fd);
	if (!new->filename)
	{
		free(new->heredoc_delimeter);
		free(new);
		ft_free_all(mini, ENOMEM, 1);
	}
	heredoc_append(heredoc, new);
	read_heredoc(mini, new->filename, new->heredoc_delimeter);
}

int	redirect_heredoc(t_redir *redirect, t_mini *mini)
{
	int	fd;

	(void)redirect;
	if (!mini->heredoc || !mini->heredoc->filename)
	{
		ft_putstr_fd("minishell: heredoc: file not found\n", 2);
		return (-1);
	}
	fd = open(mini->heredoc->filename, O_RDONLY);
	if (fd == -1)
	{
		print_command_error(mini->heredoc->filename, strerror(errno));
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

void	process_heredocs(t_mini *mini, t_ast *node)
{
	t_redir	*redir;

	if (!node)
		return ;
	if (node->type == NODE_CMD && node->redir)
	{
		redir = node->redir;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				create_heredoc_file(mini, redir->heredoc_delimeter,
					&mini->heredoc);
				if (mini->heredoc_signal)
					return ;
			}
			redir = redir->next;
		}
	}
	process_heredocs(mini, node->left);
	process_heredocs(mini, node->right);
}

