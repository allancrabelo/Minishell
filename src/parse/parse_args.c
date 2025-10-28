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

int	handle_word_token(t_token **tokens, char **args, int *count)
{
	args[*count] = ft_strdup((*tokens)->data);
	if (!args[*count])
		return (0);
	(*count)++;
	*tokens = (*tokens)->next;
	return (1);
}
