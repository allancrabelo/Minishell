#include "minishell.h"

/**
 * @brief Expands tilde character to home directory path
 *
 * Replaces '~' with the value of HOME environment variable.
 * Returns original tilde if HOME is not set.
 *
 * @param mini Pointer to main shell structure
 * @return char* Expanded home directory path or "~"
 */
static char	*expand_tilde(t_mini *mini)
{
	char	*home;

	home = get_env_var(mini, "HOME");
	if (home)
		return (ft_strdup(home));
	return (ft_strdup("~"));
}

/**
 * @brief Copies expanded string into destination buffer
 *
 * Appends the contents of the expanded string to the destination
 * buffer at the specified index, updating the index accordingly.
 * Frees the expanded string after copying.
 *
 * @param dst Destination buffer to copy into
 * @param j Pointer to current index in destination buffer
 * @param exp Expanded string to copy
 */
static void	copy_exp(char *dst, size_t *j, char *exp)
{
	size_t	i;

	if (!exp)
		return ;
	i = 0;
	while (exp[i])
		dst[(*j)++] = exp[i++];
	free(exp);
}

/**
 * @brief Performs tilde expansion in token data
 *
 * Replaces '~' character with home directory path and copies
 * the result to the destination buffer. Advances input index.
 *
 * @param mini Pointer to main shell structure
 * @param dst Destination buffer for expanded data
 * @param i Pointer to current index in input string
 * @param j Pointer to current position in destination buffer
 * @return void
 */
void	do_tilde(t_mini *mini, char *dst, size_t *i, size_t *j)
{
	size_t	new;

	new = (*i) + 1;
	if (ft_isspace(mini->input[new]) || mini->input[new] == '\0'
		|| mini->input[new] == '/')
	{
		(*i)++;
		copy_exp(dst, j, expand_tilde(mini));
	}
	else
	{
		dst[(*j)++] = mini->input[*i];
		(*i)++;
	}
}

/**
 * @brief Performs variable expansion in token data
 *
 * Extracts variable name from input, expands it to its value,
 * and copies the result to the destination buffer. Advances input index.
 *
 * @param mini Pointer to main shell structure
 * @param dst Destination buffer for expanded data
 * @param i Pointer to current index in input string
 * @param j Pointer to current position in destination buffer
 * @return void
 */
void	do_var(t_mini *mini, char *dst, size_t *i, size_t *j)
{
	char	*var_name;

	(*i)++;
	var_name = extract_var_name(mini, i);
	copy_exp(dst, j, expand_variable(mini, var_name));
	free(var_name);
}
