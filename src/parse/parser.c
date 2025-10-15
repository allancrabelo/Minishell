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

/* Updated verify_tokens function to handle parentheses */
static int	verify_tokens(t_mini *mini, t_token *token)
{
	while (token)
	{
		if (token->type == TOKEN_PIPE || token->type == TOKEN_AND
			|| token->type == TOKEN_OR)
		{
			if (!token->next || is_operator(token->next->type)
				|| token->next->type == TOKEN_RPAREN)
				return (print_syntax_error(mini, "near unexpected token"));
		}
		else if (is_redirect(token->type))
		{
			if (!token->next || token->next->type != TOKEN_WORD)
				return (print_syntax_error(mini,
						"near unexpected token `newline'"));
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
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
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
