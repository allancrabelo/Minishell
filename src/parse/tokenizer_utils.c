#include "../../includes/minishell.h"

int	is_op(const char *input, size_t i)
{
	if (input[i] == '\0')
		return (0);
	if (input[i] == '(')
		return (TOKEN_LPAREN);
	if (input[i] == ')')
		return (TOKEN_RPAREN);
	if (input[i] == '|' && input[i + 1] && input[i + 1] == '|')
		return (TOKEN_OR);
	if (input[i] == '&' && input[i + 1] && input[i + 1] == '&')
		return (TOKEN_AND);
	if (input[i] == '|')
		return (TOKEN_PIPE);
	if (input[i] == '>' && input[i + 1] && input[i + 1] == '>')
		return (TOKEN_REDIRECT_APPEND);
	if (input[i] == '>')
		return (TOKEN_REDIRECT_OUT);
	if (input[i] == '<' && input[i + 1] && input[i + 1] == '<')
		return (TOKEN_HEREDOC);
	if (input[i] == '<')
		return (TOKEN_REDIRECT_IN);
	return (0);
}

size_t	expand_var(t_mini *mini, size_t *i) //	Implement expand variables
{
	size_t	start;

	start = *i;
	while (ft_isalnum(mini->input[*i]) || mini->input[*i] == '_')
		(*i)++;
	return (*i - start);
}

size_t	handle_quotes(t_mini *mini, size_t *i, char quote)
{
	size_t	len;

	(*i)++;
	len = 0;
	while (mini->input[*i] && mini->input[*i] != quote)
	{
		if (quote == '"' && mini->input[*i] == '$')
			len += expand_var_in_tokenizer(mini, i);//expand_var(mini, i);
		else
		{
			len++;
			(*i)++;
		}
	}
	if (mini->input[*i] == quote)
		(*i)++;
	return (len);
}

size_t	get_word_len(t_mini *mini, size_t len, size_t i) //	Implement exp var
{
	size_t	word_len;

	word_len = 0;
	while (i < len && !ft_isspace(mini->input[i]) && !is_op(mini->input, i))
	{
		if (mini->input[i] == '\'' || mini->input[i] == '"')
			word_len += handle_quotes(mini, &i, mini->input[i]);
		else if (mini->input[i] == '$')
			word_len += expand_var_in_tokenizer(mini, &i);//expand_var(mini, &i);
		else
		{
			word_len++;
			i++;
		}
	}
	return (word_len);
}

int	check_validity(char *input)
{
	size_t	i;
	int		single_quote;
	int		double_quote;
	int		balance;

	i = 0;
	balance = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (input[i] == '"' && !single_quote)
			double_quote = !double_quote;
		if (input[i] == '(')
			balance ++;
		else if (input[i] == ')')
			balance --;
		if (balance < 0)
			return (0);
		i++;
	}
	return (!single_quote && !double_quote && balance == 0);
}
