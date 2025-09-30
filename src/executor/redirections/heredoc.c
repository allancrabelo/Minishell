#include "../../../includes/minishell.h"

static char	*read_heredoc_input(char *delimeter, int *interrupted)
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
			if (!*interrupted)
			{
				ft_putstr_fd("bash: warning: here-document delimited by end-of-file (wanted `", 2);
				ft_putstr_fd(delimeter, 2);
				ft_putstr_fd("')\n", 2);
				break;
			}
		}
		if (ft_strcmp(line, delimeter) == 0)
		{
			free(line);
			break;
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

static int	create_heredoc_file(char *delimiter, t_mini *mini)
{
	int		fd;
	char	*content;
	int		interrupted;
	pid_t	pid;
	int		status;

	interrupted = 0;
	pid = fork();
	if (pid == 0)
	{
		// Child process for heredoc
		fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			exit(1);
		
		content = read_heredoc_input(delimiter, &interrupted);
		if (content)
		{
			write(fd, content, ft_strlen(content));
			free(content);
		}
		close(fd);
		exit(0);
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			// Heredoc was interrupted by Ctrl+C
			mini->exit_status = 128 + g_signal; // Set exit status directly
			signal_init(); // Restore original signals
			return (-1);
		}
		else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			mini->exit_status = 128 + g_signal; // Set exit status directly
			signal_init(); // Restore original signals
			return (-1);
		}
		
		fd = open("/tmp/minishell_heredoc", O_RDONLY);
		unlink("/tmp/minishell_heredoc");
		signal_init(); // Restore original signals
		return (fd);
	}
	return (-1);
}

int	redirect_heredoc(t_redir *redirect, t_mini *mini)
{
	int	fd;

	fd = create_heredoc_file(redirect->file, mini);
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