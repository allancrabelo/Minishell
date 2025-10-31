/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:48 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/31 15:26:29 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates AST node for logical or pipe operators
 *
 * Allocates and initializes operator node (PIPE, AND, OR) with
 * left and right subtrees. Sets node type and initializes other fields.
 *
 * @param left Left subtree AST node
 * @param right Right subtree AST node
 * @param type Token type of the operator
 * @return t_ast* New operator node, NULL on allocation failure
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

/**
 * @brief Creates AST node for command with arguments and redirections
 *
 * Allocates and initializes command node with provided arguments,
 * argument count, and redirection list. Sets node type and initializes
 * child pointers to NULL.
 *
 * @param args Array of command arguments
 * @param arg_count Number of arguments
 * @param redir Linked list of redirections
 * @return t_ast* New command node, NULL on allocation failure
 */
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

/**
 * @brief Builds the abstract syntax tree (AST) from tokens
 *
 * parsing into an AST. Starts parsing from the highest precedence
 * Validates initial token and overall token sequence before
 * level (OR) and constructs the full AST.
 *
 * @param mini Pointer to main shell structure
 * @return int 0 on success, 1 on failure
 */
int	build_ast(t_mini *mini)
{
	t_token	*cur;

	if (mini->token->type == TOKEN_PIPE || mini->token->type == TOKEN_AND
		|| mini->token->type == TOKEN_OR)
		return (print_syntax_error(mini, "near unexpected token ",
				mini->token->data), 1);
	cur = mini->token;
	if (!verify_tokens(mini, cur))
		return (1);
	mini->ast = parse_or(mini, &cur);
	if (!mini->ast)
		return (1);
	return (0);
}
