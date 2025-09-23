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

t_ast	*parse_command(t_token **tokens)
{
	int	count;
	char	**args;

	count = count_args(tokens);
	*args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	count = 0;
	while(*tokens)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			args[count++] = ft_strdup((*tokens)->data);
			*tokens = (*tokens)->next;
		}
		else if ((*tokens)->type >= TOKEN_REDIRECT_IN)
			if (!create_redirect(&tokens))
				return (NULL);
		else
			break ;
	}
}

t_ast	*parse_pipline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_token	*cur;

	left = parse_command(&tokens);
	if (cur && cur->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_pipline(tokens);
		return (create_pipe_node(left, right));
	}
	return (left);
}

int	build_ast(t_mini *mini)
{
	t_token	*cur;

	cur = mini;
	mini->ast = parse_pipeline(&cur);
}