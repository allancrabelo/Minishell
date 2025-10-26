#include "minishell.h"

/**
 * @brief Checks if token type is a logical or pipe operator
 * 
 * Determines whether the given token type represents a control
 * operator (OR, AND, PIPE) used for command sequencing.
 * 
 * @param type Token type to check
 * @return int 1 if operator, 0 otherwise
 */
int	is_operator(t_token_type type)
{
	return (type == TOKEN_OR || type == TOKEN_AND || type == TOKEN_PIPE);
}

/**
 * @brief Checks if token type is a redirection operator
 * 
 * Determines whether the given token type represents a file
 * redirection operator (in, out or append).
 * 
 * @param type Token type to check
 * @return int 1 if redirection operator, 0 otherwise
 */
int	is_redirect(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND);
}

/**
 * @brief Prints syntax error message and sets exit status
 * 
 * Formats and displays syntax error information to stderr
 * and updates the shell's exit status to indicate syntax error.
 * 
 * @param mini Pointer to main shell structure
 * @param error_msg Primary error description
 * @param token Offending token data (optional)
 * @return int Always returns 0 to indicate error condition
 */
int	print_syntax_error(t_mini *mini, char *error_msg, char *token)
{
	ft_putstr_fd("minishell: syntax error ", 2);
	if (error_msg)
		ft_putstr_fd(error_msg, 2);
	if (token)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'", 2);
	}
	ft_putstr_fd("\n", 2);
	mini->exit_status = 2;
	return (0);
}

/**
 * @brief Concatenates two strings and frees the first string
 * 
 * Allocates new memory for the concatenated result, copies both strings,
 * and frees the first input string. Useful for building strings incrementally.
 * 
 * @param s1 First string (will be freed)
 * @param s2 Second string to append
 * @return char* New concatenated string, NULL on allocation failure
 */
char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	size_t	s1_len;
	size_t	s2_len;
	char	*new;

	i = 0;
	j = 0;
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	new = malloc(s1_len + s2_len + 1);
	if (!new)
	{
		free(s1);
		return (NULL);
	}
	while (i < s1_len)
		new[i++] = s1[j++];
	j = 0;
	while (j < s2_len && s2[j])
		new[i++] = s2[j++];
	new[i] = '\0';
	free(s1);
	return (new);
}

/**
 * @brief Sets PWD environment variable from current working directory
 * 
 * Retrieves current working directory and creates PWD environment variable
 * string. Handles getcwd failures and memory allocation.
 * 
 * @param cwd Buffer for current working directory
 * @param total_size
 * @param new_envp Environment array to populate
 * @param i Current index in environment array
 * @return int Updated index on success, EXIT_FAILURE on error
 */
int	set_getcwd(char *cwd, int total_size, char **new_envp, int i)
{
	i = 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		total_size = ft_strlen("PWD=") + ft_strlen(cwd) + 1;
		new_envp[i] = malloc(total_size);
		if (new_envp[i])
		{
			ft_strlcpy(new_envp[i], "PWD=", total_size);
			ft_strlcat(new_envp[i], cwd, total_size);
			i++;
		}
	}
	else
		return (perror("getcwd"), EXIT_FAILURE);
	return (i);
}
