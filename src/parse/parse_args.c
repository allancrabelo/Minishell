#include "minishell.h"

char	**init_args_array(int count)
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
	int		count;
	char	**expanded;
	int		i;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
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

int	handle_word_token(t_token **tokens, char **args, int *count)
{
	char	**expanded;
	int		i;

	if (!(*tokens)->quoted && has_wildcard((*tokens)->data))
	{
		expanded = expand_wildcard((*tokens)->data);
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
			*tokens = (*tokens)->next;
			return (1);
		}
	}
	args[*count] = ft_strdup((*tokens)->data);
	if (!args[*count])
		return (0);
	(*count)++;
	*tokens = (*tokens)->next;
	return (1);
}
