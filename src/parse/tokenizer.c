#include "../includes/minishell.h"

char	*fill_token_data(t_mini *mini, size_t *i, size_t len)
{
	char	*dst;
	size_t	j;
	int		quote;

	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	quote = 0;
	j = 0;
	while (j < len && mini->input[*i])
	{
		if ((mini->input[*i] == '\'' || mini->input[*i] == '"') && quote == 0)
			quote = mini->input[(*i)++];
		else if (mini->input[*i] == quote && quote)
		{
			quote = 0;
			(*i)++;
		}
		else
			dst[j++] = mini->input[(*i)++];
	}
	dst[j] = '\0';
	return (dst);
}

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

	if (!mini->input || len == 0)
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
	size_t	wd_len;

	mini->input = ft_strdup(input);
	i = 0;
	len = ft_strlen(mini->input);
	if (!check_validity(mini->input))
		return (write(2, "Syntax error\n", 13), 0);
	while (i < len)
	{
		while (i < len && ft_isspace(mini->input[i]))
			i++;
		if (tokenize_op(mini, &i))
			continue ;
		wd_len = get_word_len(mini, len, i);
		if (wd_len > 0)
			add_token(mini, &i, wd_len, TOKEN_WORD);
		else
			i++;
		i++;
	}
	return (1);
}
