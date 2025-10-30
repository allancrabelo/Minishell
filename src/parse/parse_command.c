/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:43:50 by mqueiros          #+#    #+#             */
/*   Updated: 2025/10/30 23:43:51 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Appends redirection node to redirection list
 *
 * Adds a new redirection node to the end of the linked list.
 * Handles both empty list and existing list cases.
 *
 * @param redir Double pointer to head of redirection list
 * @param new_redir New redirection node to append
 * @return int Always returns 1 (success)
 */
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

/**
 * @brief Expands filename for redirection with wildcard support
 *
 * Handles filename expansion for redirection targets. Processes
 * wildcards unless quoted, validates single match for ambiguity.
 *
 * @param mini Pointer to main shell structure
 * @param pattern Filename pattern to expand
 * @param quoted Flag indicating if pattern was quoted
 * @return char* Expanded filename, NULL on error or ambiguity
 */
static char	*expand_redir_file(t_mini *mini, t_token *token,
				char *pattern, int quoted)
{
	char	**expanded;
	char	*result;

	if (quoted || !has_wildcard(pattern) || token->type == TOKEN_HEREDOC)
		return (ft_strdup(pattern));
	expanded = expand_wildcard(pattern);
	if (!expanded)
	{
		print_command_error(pattern, "No such file or directory");
		mini->exit_status = 1;
		return (NULL);
	}
	if (expanded[0] && expanded[1])
	{
		print_command_error(pattern, "ambiguous redirect");
		ft_free_split(expanded);
		mini->exit_status = 1;
		return (NULL);
	}
	result = ft_strdup(expanded[0]);
	ft_free_split(expanded);
	return (result);
}

/**
 * @brief Parses redirection operator and expands target file
 *
 * Processes redirection tokens, expands filename with wildcard support,
 * and creates redirection node. Handles heredoc delimiters specially.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @param redir Double pointer to redirection list head
 * @return int 1 on success, 0 on syntax error or expansion failure
 */
int	parse_redir(t_mini *mini, t_token **tokens, t_redir **redir)
{
	t_redir	*new_redir;
	char	*file;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
		return (print_syntax_error(mini, "near unexpected token ", "newline"));
	file = expand_redir_file(mini, (*tokens), (*tokens)->next->data,
			(*tokens)->next->quoted);
	if (!file)
	{
		*tokens = (*tokens)->next;
		if (*tokens)
			*tokens = (*tokens)->next;
		return (0);
	}
	new_redir = init_redir(((*tokens)->type), &file);
	if (!new_redir)
		return (0);
	if (new_redir->type == TOKEN_HEREDOC)
		new_redir->heredoc_delimeter = ft_strdup((*tokens)->next->data);
	new_redir->next = NULL;
	add_redir_to_list(redir, new_redir);
	*tokens = (*tokens)->next;
	if (*tokens)
		*tokens = (*tokens)->next;
	return (1);
}

/**
 * @brief Initializes command structure from tokens
 *
 * Processes tokens to build command arguments array and redirection list.
 * Alternates between word tokens and redirection operators.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @param redir Double pointer to redirection list head
 * @param args Pre-allocated arguments array to populate
 * @return int Number of arguments processed, -1 on error
 */
int	init_command(t_mini *mini, t_token **tokens, t_redir **redir, char **args)
{
	int	count;

	count = 0;
	while (*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (!handle_word_token(tokens, args, &count))
				return (-1);
		}
		else if ((*tokens)->type >= TOKEN_REDIRECT_IN)
		{
			if (!parse_redir(mini, tokens, redir))
				return (-1);
		}
		else
			break ;
	}
	return (count);
}

/**
 * @brief Parses command with arguments and redirections
 *
 * Builds command AST node by processing tokens into arguments array
 * and redirection list. Handles special case of heredoc-only commands.
 *
 * @param mini Pointer to main shell structure
 * @param tokens Double pointer to current token in list
 * @return t_ast* Command AST node, NULL on parsing error
 */
t_ast	*parse_command(t_mini *mini, t_token **tokens)
{
	int		count;
	char	**args;
	t_redir	*redir;

	redir = NULL;
	count = count_cmd_args(*tokens);
	args = init_args_array(count);
	if (!args)
		return (NULL);
	count = init_command(mini, tokens, &redir, args);
	if (count == -1)
	{
		ft_free_split(args);
		free_redir(redir);
		return (NULL);
	}
	if (count == 0 && redir != NULL && redir->type == TOKEN_HEREDOC)
	{
		ft_free_split(args);
		args = init_args_array(1);
		if (!args)
			return (free_redir(redir), NULL);
		count = 0;
	}
	return (build_cmd_node(args, count, redir));
}
