#include "minishell.h"

static char	**init_args_array(int count)
{
	char	**args;

	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	ft_memset(args, 0, (count + 1) * sizeof(char *));
	return (args);
}

int	count_cmd_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			tokens = tokens->next;
			count++;
		}
		else if (tokens->type >= TOKEN_REDIRECT_IN)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
				tokens = tokens->next;
		}
		else
			break ;
	}
	return (count);
}

static int	handle_word_token(t_token **tokens, char **args, int *count)
{
	args[*count] = ft_strdup((*tokens)->data);
	if (!args[*count])
		return (0);
	(*count)++;
	*tokens = (*tokens)->next;
	return (1);
}

static int	add_redir_to_list(t_redir **redir, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redir)
		*redir = new_redir;
	else
	{
		current = *redir;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (1);
}

int	parse_redir(t_mini *mini, t_token **tokens, t_redir **redir)
{
	t_redir	*new_redir;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		mini->exit_status = 2;
		return (printf("Error: expected filename after redirection\n"), 0);
	}
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (0);
	new_redir->type = (*tokens)->type;
	new_redir->file = ft_strdup((*tokens)->next->data);
	new_redir->heredoc_delimeter = NULL;
	if (new_redir->type == TOKEN_HEREDOC)
		new_redir->heredoc_delimeter = ft_strdup((*tokens)->next->data);
	if (!new_redir->file)
		return (free_redir(new_redir), 0);
	new_redir->next = NULL;
	add_redir_to_list(redir, new_redir);
	*tokens = (*tokens)->next;
	if (*tokens)
		*tokens = (*tokens)->next;
	return (1);
}

t_ast	*parse_command(t_mini *mini, t_token **tokens)
{
	int		count;
	char	**args;
	t_redir	*redir;

	count = count_cmd_args(*tokens);
	args = init_args_array(count);
	count = 0;
	redir = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (!handle_word_token(tokens, args, &count))
				return (ft_free_split(args), free_redir(redir), NULL);
		}
		else if ((*tokens)->type >= TOKEN_REDIRECT_IN)
		{
			if (!parse_redir(mini, tokens, &redir))
				return (ft_free_split(args), free_redir(redir), NULL);
		}
		else
			break ;
	}
	if (count == 0 && redir != NULL && redir->type == TOKEN_HEREDOC)
	{
		args = init_args_array(1);
		if (!args)
			return (free_redir(redir), NULL);
		count = 0;
	}
	return (build_cmd_node(args, count, redir));
}
