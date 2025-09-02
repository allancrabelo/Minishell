#include "../includes/minishell.h"

t_token	*new_token(const char *str, size_t len, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->data = ft_strndup(str, len);
	if (!token->data)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_mini *mini, const char *str
	, size_t len, t_token_type type)
{
	t_token	*new;
	t_token	*cur;

	if (!str || len == 0)
		return ;
	new = new_token(str, len, type);
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

int	ft_tokenizer(t_mini *mini, char *input)
{
	const char	*ptr;
	size_t		word_len;
	size_t		op_advance;

	if (!check_quotes_validity(input))
		return (0);
	ptr = input;
	while (*ptr)
	{
		skip_whitespace(&ptr);
		if (!*ptr)
			break ;
		if (is_operator_char(*ptr))
		{
			add_token(mini, ptr, op_advance, get_op_type(ptr, &op_advance));
			ptr += op_advance;
		}
		else
		{
			word_len = get_word_length(ptr);
			add_token(mini, ptr, word_len, TOKEN_WORD);
			advance_through_word(&ptr);
		}
	}
	return (1);
}
