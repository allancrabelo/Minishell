#include "../includes/minishell.h"

int	count_args(t_token *tokens)
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

int	create_redirect_ast(t_token **tokens, t_redir **redir)
{
	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		printf("Error: expected filename after redirection\n");
		return (0);
	}
	*redir = malloc(sizeof(t_redir));
	if (!*redir)
		return (0);
	(*redir)->type = (*tokens)->type;
	(*redir)->file = ft_strdup((*tokens)->next->data);
	if (!(*redir)->file)
	{
		free(*redir);
		return (0);
	}
	(*redir)->next = NULL;
	(*tokens) = (*tokens)->next->next;
	return (1);
}

t_ast	*create_command(char **args, int arg_count, t_redir *redir)
{
	t_ast	*node = malloc(sizeof(t_ast));

	if (!node)
		return (NULL);
	node->type = 1;
	node->left = NULL;
	node->right = NULL;
	node->args = args;
	node->arg_count = arg_count;
	node->redir = redir;
	return (node);
}

static void	free_args(char **args, int count)
{
	int i = 0;

	if (!args)
		return;
	while (i < count)
		free(args[i++]);
	free(args);
}

t_ast	*parse_command(t_token **tokens)
{
	int		count;
	char	**args;
	t_redir	*redir;

	count = count_args(*tokens);
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	count = 0;
	redir = NULL;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			args[count] = ft_strdup((*tokens)->data);
			if (!args[count])
			{
				free_args(args, count);
				if (redir)
					free_redir(redir);
				return (NULL);
			}
			count++;
			*tokens = (*tokens)->next;
		}
		else if ((*tokens)->type >= TOKEN_REDIRECT_IN)
		{
			if (!create_redirect_ast(tokens, &redir))
			{
				free_args(args, count);
				if (redir)
					free_redir(redir);
				return (NULL);
			}
		}
		else
			break ;
	}
	args[count] = NULL;
	return (create_command(args, count, redir));
}

t_ast	*create_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;
	
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = 0;
	node->left = left;
	node->right = right;
	node->args	= NULL;
	node->arg_count = 0;
	node->redir = NULL;
	return (node);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(tokens);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		return (create_pipe(left, right));
	}
	return (left);
}

int	build_ast(t_mini *mini)
{
	t_token	*cur;

	cur = mini->token;
	mini->ast = parse_pipeline(&cur);
	if (!mini->ast)
		return (free_ast(mini->ast), 1);
	// free_tokens(mini); //TODO free this when ast is working
	return (0);
}

void	free_redir(t_redir *redir)
{
	t_redir *tmp;
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
	int i;
	if (!node)
		return;
	free_ast(node->left);
	free_ast(node->right);
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->redir)
		free_redir(node->redir);
	free(node);
}
