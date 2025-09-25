#include "../../../includes/minishell.h"

char	flag_verification(char	**args, int *i)
{
	char	ret;
	size_t	j;

	ret = 0;
	while (args[*i])
	{
		j = 0;
		if (args[*i][j] != '-')
			break ;
		j++;
		if (args[*i][j] == '\0')
			break ;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] != '\0')
			break ;
		ret = 'n';
		(*i)++;
	}
	return (ret);
}

int	ft_echo(t_ast *node)
{
	int		flag;
	char	**args;
	int		i;
	int		fd;
	t_redir	*redir;

	i = 1;
	args = node->args;
	flag = flag_verification(args, &i); // TODO: implement flag_verification if needed

	// Find last output redirection, if any
	fd = STDOUT_FILENO;
	redir = node->redir;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_REDIRECT_APPEND)
		{
			if (fd != STDOUT_FILENO)
				close(fd);
			fd = open(redir->file,
				redir->type == TOKEN_REDIRECT_OUT
					? O_WRONLY | O_CREAT | O_TRUNC
					: O_WRONLY | O_CREAT | O_APPEND,
				0644);
			if (fd < 0)
				return (perror(redir->file), 1);
		}
		redir = redir->next;
	}
	while (i < node->arg_count)
	{
		write(fd, args[i], ft_strlen(args[i]));
		if (i != node->arg_count - 1)
			write(fd, " ", 1);
		i++;
	}
	if (!flag)
		write(fd, "\n", 1);
	if (fd != STDOUT_FILENO)
		close(fd);
	return (0);
}
