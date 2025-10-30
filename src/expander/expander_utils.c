#include "minishell.h"

/**
 * @brief Converts an integer to a string (no allocation).
 * 
 * Writes the decimal representation of `n` into `dst`.
 * Handles only non-negative integers and ensures null termination.
 * 
 * @param n   Integer to convert
 * @param dst Destination buffer for the resulting string
 */
void	ft_itoa_alternative(int n, char *dst)
{
	int		len;
	int		temp;
	int		i;

	if (n == 0)
	{
		dst[0] = '0';
		dst[1] = '\0';
		return ;
	}
	temp = n;
	len = 0;
	while (temp)
	{
		temp /= 10;
		len++;
	}
	dst[len] = '\0';
	i = len - 1;
	while (n)
	{
		dst[i--] = (n % 10) + '0';
		n /= 10;
	}
}

/**
 * @brief Expands the special variable `$?` with the shell's exit status.
 * 
 * Converts `mini->exit_status` into a string using `ft_itoa_alternative()`
 * and returns it. The returned string is dynamically allocated.
 * 
 * @param mini Pointer to the main shell structure
 * @return char* Newly allocated string with the exit status, or NULL on failure
 */
char	*expand_exit_status(t_mini *mini)
{
	char	*result;

	result = malloc (sizeof(char) * 12);
	if (!result)
		return (NULL);
	ft_itoa_alternative(mini->exit_status, result);
	return (result);
}

/**
 * @brief Expands special shell variables.
 * 
 * Handles expansion for special variables like `$?`.
 * Returns the expanded value as a newly allocated string.
 * 
 * @param mini Pointer to the main shell structure
 * @param var_char Character representing the special variable
 * @return char* Expanded variable value, or NULL if not a special variable
 */
char	*expand_special_var(t_mini *mini, char var_char)
{
	if (var_char == '?')
		return (expand_exit_status(mini));
	return (NULL);
}

/**
 * @brief Checks if a character is a special shell variable.
 * 
 * Identifies characters that represent special shell variables
 * such as `$?`, `$$`, `$!`, positional parameters, and `$*`.
 * 
 * @param c Character to check
 * @return int 1 if `c` is a special variable character, 0 otherwise
 */
int	is_special_var(char c)
{
	return (c == '?' || c == '$' || c == '!'
		|| (c >= '0' && c <= '9') || c == '*');
}
