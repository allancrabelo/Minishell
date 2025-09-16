#include "../../includes/minishell.h"

// Helper function to check if token is a redirection
static int	is_redirect_token(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (1);
	if (type == TOKEN_REDIRECT_OUT)
		return (1);
	if (type == TOKEN_REDIRECT_APPEND)
		return (1);
	if (type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

// Function to create a new redirect structure
t_redir	*create_redirect(t_token_type type, char *file)
{
	t_redir	*redirect;

	redirect = malloc(sizeof(t_redir));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->file = ft_strdup(file);
	if (!redirect->file)
	{
		free(redirect);
		return (NULL);
	}
	redirect->fd = -1;
	redirect->next = NULL;
	return (redirect);
}

// Function to add a redirect to a command
void	add_redirect_to_cmd(t_cmd *cmd, t_redir *redirect)
{
	t_redir	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = redirect;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = redirect;
}

// Function to free redirects
void	free_redirects(t_redir *redirects)
{
	t_redir	*current;
	t_redir	*tmp;

	current = redirects;
	while (current)
	{
		tmp = current->next;
		free(current->file);
		if (current->fd > 2)
			close(current->fd);
		free(current);
		current = tmp;
	}
}

// Function to free a command structure
void	free_cmd(t_cmd *cmd)
{
	t_cmd	*current;
	t_cmd	*tmp;
	int		i;

	current = cmd;
	while (current)
	{
		tmp = current->next;
		if (current->argv)
		{
			i = 0;
			while (current->argv[i])
			{
				free(current->argv[i]);
				i++;
			}
			free(current->argv);
		}
		free_redirects(current->redirects);
		free(current);
		current = tmp;
	}
}

// Function to find the first command token (not part of redirects)
t_token	*find_first_command(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if (is_redirect_token(cur->type))
		{
			cur = cur->next;
			if (cur)
				cur = cur->next;
			continue ;
		}
		if (cur->type == TOKEN_WORD)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

// Function to check if a command is a builtin
int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

// Function to count command arguments (excluding redirects)
int	count_command_args(t_token *cmd_token)
{
	t_token	*cur;
	int		count;

	count = 0;
	cur = cmd_token;
	while (cur && cur->type != TOKEN_PIPE)
	{
		if (cur->type == TOKEN_WORD)
			count++;
		else if (is_redirect_token(cur->type))
		{
			cur = cur->next;
			if (cur)
				cur = cur->next;
			continue ;
		}
		cur = cur->next;
	}
	return (count);
}

// Function to handle redirections during parsing
int	handle_redirect(t_cmd *cmd, t_token **cur)
{
	t_token_type	redir_type;
	t_redir			*redirect;

	if (!*cur)
		return (0);
	redir_type = (*cur)->type;
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != TOKEN_WORD)
	{
		write(2, "minishell: syntax error near redirection\n", 42);
		return (0);
	}
	redirect = create_redirect(redir_type, (*cur)->data);
	if (!redirect)
		return (0);
	add_redirect_to_cmd(cmd, redirect);
	*cur = (*cur)->next;
	return (1);
}

// Helper function to allocate argv array
static int	allocate_cmd_argv(t_cmd *cmd, t_token *tokens)
{
	cmd->argc = count_command_args(tokens);
	if (cmd->argc == 0)
		return (1);
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	if (!cmd->argv)
		return (0);
	return (1);
}

// Helper function to process word token
static int	process_word_token(t_cmd *cmd, t_token **cur, int *i)
{
	cmd->argv[*i] = ft_strdup((*cur)->data);
	if (!cmd->argv[*i])
		return (ft_free_split(cmd->argv), 0);
	(*i)++;
	*cur = (*cur)->next;
	return (1);
}

// Helper function to process redirection token
static int	process_redirect_token(t_cmd *cmd, t_token **cur)
{
	if (!handle_redirect(cmd, cur))
		return (0);
	return (1);
}

// Function to build argv array for a command
int	build_cmd_argv(t_cmd *cmd, t_token *tokens)
{
	t_token	*cur;
	int		i;

	if (!allocate_cmd_argv(cmd, tokens))
		return (0);
	if (cmd->argc == 0)
		return (1);
	cur = tokens;
	i = 0;
	while (cur && cur->type != TOKEN_PIPE && i < cmd->argc)
	{
		if (cur->type == TOKEN_WORD)
		{
			if (!process_word_token(cmd, &cur, &i))
				return (0);
		}
		else if (is_redirect_token(cur->type))
		{
			if (!process_redirect_token(cmd, &cur))
				return (ft_free_split(cmd->argv), 0);
		}
		else
			cur = cur->next;
	}
	cmd->argv[i] = NULL;
	return (1);
}

// Function to create and initialize a new command
t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->argc = 0;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

// Main function to parse a command from tokens
t_cmd	*parse_command(t_token *tokens)
{
	t_cmd	*cmd;

	if (!tokens)
		return (NULL);
	cmd = create_cmd();
	if (!cmd)
		return (NULL);
	if (!build_cmd_argv(cmd, tokens))
	{
		free_cmd(cmd);
		return (NULL);
	}
	return (cmd);
}

// Compatibility function - builds argv array from tokens (legacy)
char	**build_argv(t_token *cmd_token)
{
	t_cmd	*cmd;
	char	**result;

	cmd = parse_command(cmd_token);
	if (!cmd)
		return (NULL);
	result = cmd->argv;
	cmd->argv = NULL;
	free_cmd(cmd);
	return (result);
}

// Function to free argv array
void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

// Helper function to print command name
static void	print_cmd_name(t_cmd *cmd)
{
	printf("Command: ");
	if (cmd->argv && cmd->argv[0])
		printf("%s", cmd->argv[0]);
	else
		printf("(no command)");
	printf("\n");
}

// Helper function to print arguments
static void	print_cmd_args(t_cmd *cmd)
{
	int	i;

	printf("Arguments: ");
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			printf("[%s] ", cmd->argv[i]);
			i++;
		}
	}
	printf("\n");
}

// Helper function to print redirections
static void	print_cmd_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	printf("Redirections:\n");
	redir = cmd->redirects;
	while (redir)
	{
		printf("  Type: %d, File: %s\n", redir->type, redir->file);
		redir = redir->next;
	}
}

// Example usage function for testing
void	print_cmd(t_cmd *cmd)
{
	if (!cmd)
	{
		printf("Command: NULL\n");
		return ;
	}
	print_cmd_name(cmd);
	print_cmd_args(cmd);
	print_cmd_redirections(cmd);
	printf("---\n");
}
