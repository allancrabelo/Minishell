#include "minishell.h"

t_token	*new_token(t_mini *mini, size_t *i, size_t len, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->data = fill_token_data(mini, i, len);
	if (!token->data)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_mini *mini, size_t *i, size_t len, t_token_type type)
{
	t_token	*new;
	t_token	*cur;

	if (!mini->input)
		return ;
	new = new_token(mini, i, len, type);
	if (!new)
		return ;
	if (mini->token == NULL)
		mini->token = new;
	else
	{
		cur = mini->token;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

int	tokenize_op(t_mini *mini, size_t *i)
{
	t_token_type	type;
	int				op_type;
	size_t			op_len;

	op_type = is_op(mini->input, *i);
	if (!op_type)
		return (0);
	type = op_type;
	op_len = 1;
	if (type == TOKEN_OR || type == TOKEN_AND
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_HEREDOC)
		op_len = 2;
	add_token(mini, i, op_len, type);
	return (1);
}

int	ft_tokenizer(t_mini *mini, char *input)
{
	size_t	i;
	size_t	len;
	size_t	word_len;

	mini->input = ft_strdup(input);
	i = 0;
	len = ft_strlen(mini->input);
	if (!check_validity(mini, mini->input))
	{
		free(mini->input);
		mini->input = NULL;
		return (EXIT_FAILURE);
	}
	while (i < len)
	{
		size_t	start_word;
		int		was_quoted;
		
		while (i < len && ft_isspace(mini->input[i]))
			i++;
		if (tokenize_op(mini, &i))
			continue ;
		start_word = i;
		was_quoted = (mini->input[i] == '"' || mini->input[i] == '\'');
		word_len = get_word_len(mini, len, &i);
		if (word_len > 0 || (word_len == 0 && was_quoted))
		{
			add_token(mini, &start_word, word_len, TOKEN_WORD);
		}
	}
	return (EXIT_SUCCESS);
}
