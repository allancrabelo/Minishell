#include "minishell.h"

/**
 * @brief Allocates and initializes command arguments array
 * 
 * Creates a NULL-terminated array of strings with specified capacity.
 * Memory is zero-initialized to ensure proper termination.
 * 
 * @param count Number of argument slots to allocate
 * @return char** Allocated arguments array, NULL on allocation failure
 */
char	**init_args_array(int count)
{
	char	**args;

	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	ft_memset(args, 0, (count + 1) * sizeof(char *));
	return (args);
}

/**
 * @brief Counts expanded arguments after wildcard processing
 * 
 * Calculates the number of arguments after wildcard expansion.
 * If token contains unquoted wildcards, counts all matching files,
 * otherwise counts as single argument.
 * 
 * @param tokens Token to check for wildcard expansion
 * @return int Number of arguments after expansion
 */
int	count_expanded_args(t_token *tokens)
{
	int		i;
	int		count;
	char	**expanded;

	if (!tokens || !tokens->data)
		return (0);
	count = 0;
	if (!tokens->quoted && has_wildcard(tokens->data))
	{
		expanded = expand_wildcard(tokens->data);
		if (expanded)
		{
			i = 0;
			while (expanded[i])
				i++;
			count += i;
			ft_free_split(expanded);
		}
		else
			count++;
	}
	else
		count++;
	return (count);
}

/**
 * @brief Counts command arguments with wildcard expansion
 * 
 * Iterates through tokens to count command arguments, expanding
 * wildcards in unquoted word tokens. Skips redirection operators
 * and their associated files.
 * 
 * @param tokens Linked list of tokens to count
 * @return int Total number of command arguments after expansion
 */
int	count_cmd_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			count += count_expanded_args(tokens);
			tokens = tokens->next;
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

int	handle_wildcard_expansion(t_token *token, char **args, int *count)
{
	char	**expanded;
	int		i;

	if (!token->quoted && has_wildcard(token->data))
	{
		expanded = expand_wildcard(token->data);
		if (expanded)
		{
			i = 0;
			while (expanded[i])
			{
				args[*count] = expanded[i];
				(*count)++;
				i++;
			}
			free(expanded);
			return (1);
		}
	}
	return (0);
}

/**
 * @brief Expands wildcards in token and adds to arguments array
 * 
 * Processes token with wildcard characters, expands matching files,
 * and adds them to the arguments array. Handles memory management.
 * 
 * @param token Token containing potential wildcard pattern
 * @param args Arguments array to populate
 * @param count Pointer to current argument count
 * @return int 1 if wildcard expansion occurred, 0 otherwise
 */
int	handle_word_token(t_token **tokens, char **args, int *count)
{
	if (!tokens || !*tokens)
		return (0);
	if (handle_wildcard_expansion(*tokens, args, count))
	{
		*tokens = (*tokens)->next;
		return (1);
	}
	args[*count] = ft_strdup((*tokens)->data);
	if (!args[*count])
		return (0);
	(*count)++;
	*tokens = (*tokens)->next;
	return (1);
}
