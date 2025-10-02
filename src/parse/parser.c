#include "minishell.h"

static void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
		free(args[i++]);
	free(args);
}

static void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
		free_args(node->args);
	if (node->redir)
		free_redir(node->redir);
	free(node);
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
		return (free(new_redir), 0);
	new_redir->next = NULL;
	add_redir_to_list(redir, new_redir);
	*tokens = (*tokens)->next;
	if (*tokens)
		*tokens = (*tokens)->next;
	return (1);
}

t_ast	*build_cmd_node(char **args, int arg_count, t_redir *redir)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (free_args(args), free_redir(redir), NULL);
	node->type = 1;
	node->left = NULL;
	node->right = NULL;
	node->args = args;
	node->arg_count = arg_count;
	node->redir = redir;
	return (node);
}

static char	**init_args_array(int count)
{
	char	**args;

	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	ft_memset(args, 0, (count + 1) * sizeof(char *));
	return (args);
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
				return (free_args(args), free_redir(redir), NULL);
		}
		else if ((*tokens)->type >= TOKEN_REDIRECT_IN)
		{
			if (!parse_redir(mini, tokens, &redir))
				return (free_args(args), free_redir(redir), NULL);
		}
		else
			break ;
	}
	if (count == 0 && redir != NULL && redir->type == TOKEN_HEREDOC)
	{
		free_args(args);
		args = init_args_array(1);
		if (!args)
			return (free_redir(redir), NULL);
		count = 0;
	}
	return (build_cmd_node(args, count, redir));
}

t_ast	*build_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = 0;
	node->left = left;
	node->right = right;
	node->args = NULL;
	node->arg_count = 0;
	node->redir = NULL;
	return (node);
}

t_ast	*parse_pipeline(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(mini, tokens);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		if ((!(*tokens)->next) || ((*tokens)->next && (*tokens)->next->type != TOKEN_WORD && (*tokens)->next->type <= TOKEN_PIPE))
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			mini->exit_status = 2;
			return (NULL);
		}
		*tokens = (*tokens)->next;
		right = parse_pipeline(mini, tokens);
		return (build_pipe_node(left, right));
	}
	return (left);
}

int	build_ast(t_mini *mini)
{
	t_token	*cur;

	cur = mini->token;
	mini->ast = parse_pipeline(mini, &cur);
	if (!mini->ast)
		return (1);
	return (0);
}
