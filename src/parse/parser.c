#include "minishell.h"

/*
 * Operator precedence (lowest to highest):
 * 1. OR (||)
 * 2. AND (&&)
 * 3. PIPE (|)
 * 4. Commands and parentheses
 */

t_ast	*build_operator_node(t_ast *left, t_ast *right, t_token_type type)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	if (type == TOKEN_PIPE)
		node->type = NODE_PIPE;
	if (type == TOKEN_AND)
		node->type = NODE_AND;
	if (type == TOKEN_OR)
		node->type = NODE_OR;
	node->left = left;
	node->right = right;
	node->args = NULL;
	node->arg_count = 0;
	node->redir = NULL;
	return (node);
}

t_ast	*build_cmd_node(char **args, int arg_count, t_redir *redir)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (ft_free_split(args), free_redir(redir), NULL);
	node->type = NODE_CMD;
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

// Parse primary expressions (commands and parentheses)
static t_ast	*parse_primary(t_mini *mini, t_token **tokens)
{
	t_ast	*node;

	if (!*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		node = parse_or(mini, tokens);
		if (!node)
			return (NULL);
		if (!*tokens || (*tokens)->type != TOKEN_RPAREN)
		{
			printf("minishell: syntax error: expected ')'\n");
			mini->exit_status = 2;
			free_ast(node);
			return (NULL);
		}
		*tokens = (*tokens)->next;
		return (node);
	}
	return (parse_command(mini, tokens));
}

// Parse PIPE expressions
static t_ast	*parse_pipe(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_primary(mini, tokens);
	if (!left)
		return (NULL);

	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_primary(mini, tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_operator_node(left, right, TOKEN_PIPE);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

// Parse AND expressions
static t_ast	*parse_and(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_pipe(mini, tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_AND)
	{
		*tokens = (*tokens)->next;
		right = parse_pipe(mini, tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_operator_node(left, right, TOKEN_AND);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

// Parse OR expressions (lowest precedence)
t_ast	*parse_or(t_mini *mini, t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_and(mini, tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_OR)
	{
		*tokens = (*tokens)->next;
		right = parse_and(mini, tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = build_operator_node(left, right, TOKEN_OR);
		if (!left)
		{
			free_ast(right);
			return (NULL);
		}
	}
	return (left);
}

/* Updated verify_tokens function to handle parentheses */
static int	verify_tokens(t_mini *mini, t_token *token)
{
	while (token)
	{
		if (token->type == TOKEN_PIPE || token->type == TOKEN_AND
			|| token->type == TOKEN_OR)
		{
			if (!token->next || token->next->type == TOKEN_PIPE
				|| token->next->type == TOKEN_AND || token->next->type == TOKEN_OR
				|| token->next->type == TOKEN_RPAREN)
			{
				printf("minishell: syntax error near unexpected token\n");
				mini->exit_status = 2;
				return (0);
			}
		}
		else if (token->type >= TOKEN_REDIRECT_IN && token->type <= TOKEN_HEREDOC)
		{
			if (!token->next || token->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				mini->exit_status = 2;
				return (0);
			}
		}
		token = token->next;
	}
	return (1);
}

int	build_ast(t_mini *mini)
{
	t_token	*cur;

	if (mini->token->type == TOKEN_PIPE || mini->token->type == TOKEN_AND
		|| mini->token->type == TOKEN_OR)
	{
		printf("minishell: syntax error near unexpected token\n");
		mini->exit_status = 2;
		return (1);
	}
	cur = mini->token;
	if (!verify_tokens(mini, cur))
		return (1);
	mini->ast = parse_or(mini, &cur);
	if (!mini->ast)
		return (1);
	return (0);
}
