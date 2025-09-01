#include "../includes/minishell.h"
#include "../includes/colors.h"

char	*ft_strndup(const char *str, size_t len)
{
	char	*dst;
	size_t	i;

	dst = malloc(len + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < len && str[i])
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

t_token *new_token(const char *value, size_t len, t_token_type type)
{
	t_token *t;

	t = malloc(sizeof(t_token));
	t->data = ft_strndup(value, len);
	t->type = type;
	t->next = NULL;
	return (t);
}

void add_token(t_token **head, const char *value, size_t len, t_token_type type)
{
	t_token *new;
	t_token *cur;

	if (!value || !*value || len == 0)
		return ;
	new = new_token(value, len, type);
	if (!*head)
		*head = new;
	else
	{
		cur = *head;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}

void	handle_quote(t_mini *mini, const char c)
{
	if (mini->quote == (int)c)
		mini->quote = 0;
	else if (mini->quote == 0)
		mini->quote = (int)c;
	return ;
}

void	last_token(t_mini *mini, t_token **tokens, char *start, size_t len)
{
	char	*input;

	if (!len)
		return ;
	while (mini->quote)
	{
		input = readline(SYELLOW "> " SRESET);
		if (!input)
			break ;
		do_commands(mini, input);
	}
	add_token(tokens, start, len, WORD);
}

void	ft_tokenizer(t_mini *mini, char *input, t_token **tokens)
{
	size_t	len;
	char	*start;

	len = 0;
	start = input;
	while (*input)
	{
		if (!mini->quote && ft_isspace(*input))
		{
			add_token(tokens, start, len, WORD);
			input ++;
			start = input;
			len = 0;
			continue ;
		}
		if (*input == '\'' || *input == '"')
			handle_quote(mini, *input);
		len++;
		input++;
	}
	last_token(mini, tokens, start, len);
}
