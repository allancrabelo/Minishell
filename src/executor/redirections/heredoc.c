#include "../../../includes/minishell.h"

static int	is_delimiter(char *line, char *delimiter);
static char	*expand_heredoc_line(t_mini *mini, char *line);

static void	write_heredoc_to_pipe(int write_fd, char *delimiter, t_mini *mini)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("> ");
		if (!line && g_signal != 130)
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
			ft_putendl_fd(expanded_line, write_fd);
			free(expanded_line);
		}
		else
			ft_putendl_fd(line, write_fd);
		free(line);
	}
	close(write_fd);
}

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

int	heredoc_expand_safe(t_mini *mini)
{
	t_token	*cur;

	cur = NULL;
	if (mini->token)
		cur = mini->token;
	while (cur && cur->next)
		cur = cur->next;
	if (cur && cur->type == TOKEN_HEREDOC)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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

/* static void	heredoc_append(t_heredoc **heredoc, t_heredoc *new)
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
} */

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

// This function is now integrated into write_heredoc_to_pipe

void	heredoc_cleaner(t_heredoc **heredoc)
{
	t_heredoc	*cur;
	t_heredoc	*tmp;

	cur = *heredoc;
	while (cur)
	{
		tmp = cur->next;
		if (cur->heredoc_delimeter)
			free(cur->heredoc_delimeter);
		if (cur->pipe_fd > 0)
			close(cur->pipe_fd);
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

static int	read_heredoc_with_pipe(t_mini *mini, char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		ft_free_all(mini, errno, 1);
	}
	else if (pid == 0)
	{
		close(pipe_fd[0]);
		setup_heredoc_signals();
		write_heredoc_to_pipe(pipe_fd[1], delimiter, mini);
		ft_free_all(mini, g_signal, 1);
	}
	close(pipe_fd[1]);
	wait_heredoc(mini, pid);
	signal_init();
	return (pipe_fd[0]);
}

void	create_heredoc_pipe(t_mini *mini, char *delimiter, t_heredoc **heredoc)
{
	int	pipe_fd;

	*heredoc = malloc(sizeof(t_heredoc));
	if (!*heredoc)
		ft_free_all(mini, ENOMEM, 1);
	(*heredoc)->next = NULL;
	(*heredoc)->heredoc_delimeter = ft_strdup(delimiter);
	(*heredoc)->pipe_fd = -1;
	if (!(*heredoc)->heredoc_delimeter)
	{
		free(*heredoc);
		*heredoc = NULL;
		ft_free_all(mini, ENOMEM, 1);
	}
	pipe_fd = read_heredoc_with_pipe(mini, delimiter);
	if (pipe_fd == -1)
	{
		free((*heredoc)->heredoc_delimeter);
		free(*heredoc);
		*heredoc = NULL;
		ft_free_all(mini, ENOMEM, 1);
	}
	(*heredoc)->pipe_fd = pipe_fd;
}

int	redirect_heredoc(t_redir *redirect, t_mini *mini)
{
	(void)redirect;
	if (!mini->heredoc || mini->heredoc->pipe_fd < 0)
	{
		print_command_error("heredoc", "pipe not available");
		return (-1);
	}
	if (dup2(mini->heredoc->pipe_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (-1);
	}
	/* Close the original pipe fd since it's now duplicated to stdin */
	close(mini->heredoc->pipe_fd);
	mini->heredoc->pipe_fd = -1;
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
				create_heredoc_pipe(mini, redir->heredoc_delimeter,
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

