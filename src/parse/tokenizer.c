#include "minishell.h"

/**
 * @brief Fills token data from input string
 *
 * Extracts a substring from the input starting at index `*i`
 * with length `len`, updates index `*i` to point after the token.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to start index of token in input string
 * @param len Length of token data to extract
 * @return char* Newly allocated token data string
 */
t_token	*new_token(t_mini *mini, size_t *i, size_t len, t_token_type type)
{
	t_token	*token;
	int		quoted;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	quoted = (mini->input[*i] == '"' || mini->input[*i] == '\'');
	token->data = fill_token_data(mini, i, len);
	if (!token->data)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}

/**
 * @brief Adds a new token to the end of the token list
 *
 * Creates a token node and appends it to the linked list.
 * Handles both empty list and existing list cases.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to start index of token in input string
 * @param len Length of token data to extract
 * @param type Type of token to create
 * @return void
 */
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

/**
 * @brief Identifies and tokenizes operators in input string
 *
 * Detects shell operators (||, &&, |, >>, <<, >, <, etc.) at current
 * position and adds corresponding token to the token list.
 *
 * @param mini Pointer to main shell structure
 * @param i Pointer to current index in input string
 * @return int 1 if operator was tokenized, 0 if no operator found
 */
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

/**
 * @brief Tokenizes input string into lexical tokens
 *
 * Processes input string by skipping whitespace, identifying operators,
 * and extracting words (including quoted strings). Builds token linked list.
 *
 * @param mini Pointer to main shell structure
 * @param input Input string to tokenize
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on syntax error
 */
int	ft_tokenizer(t_mini *mini, char *input)
{
	size_t	i;
	size_t	len;
	size_t	word_len;
	size_t	start_word;
	int		was_quoted;

	mini->input = ft_strdup(input);
	i = 0;
	len = ft_strlen(mini->input);
	if (!check_validity(mini, mini->input))
		return (EXIT_FAILURE);
	while (i < len)
	{
		while (i < len && ft_isspace(mini->input[i]))
			i++;
		if (tokenize_op(mini, &i))
			continue ;
		start_word = i;
		was_quoted = (mini->input[i] == '"' || mini->input[i] == '\'');
		word_len = get_word_len(mini, len, &i);
		if (word_len > 0 || (word_len == 0 && was_quoted))
			add_token(mini, &start_word, word_len, TOKEN_WORD);
	}
	return (EXIT_SUCCESS);
}
