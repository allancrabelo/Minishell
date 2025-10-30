#include "minishell.h"

/**
 * @brief Copies a string from source to destination.
 * 
 * Copies all characters from `src` into `dest`, including the null terminator.
 * 
 * @param dest Destination buffer
 * @param src  Source string
 * @return char* Pointer to `dest`
 */
char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * @brief Retrieves the value of an environment variable.
 * 
 * Searches the shell's environment list for `var_name`
 * and returns its corresponding value if found.
 * 
 * @param mini     Pointer to the main shell structure
 * @param var_name Name of the environment variable
 * @return char*   Pointer to the variable's value, or NULL if not found
 */
char	*get_env_var(t_mini *mini, char *var_name)
{
	t_env	*current;

	if (!var_name || !mini->env_list)
		return (NULL);
	current = mini->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Retrieves the value of an export variable.
 * 
 * Searches the shell's export list for `var_name`
 * and returns its corresponding value if found.
 * 
 * @param mini     Pointer to the main shell structure
 * @param var_name Name of the export variable
 * @return char*   Pointer to the variable's value, or NULL if not found
 */
char	*get_exp_var(t_mini *mini, char *var_name)
{
	t_export	*current;

	if (!var_name || !mini->export_list)
		return (NULL);
	current = mini->export_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
		{
			if (current->value)
				return (current->value);
			else
				return ("");
		}
		current = current->next;
	}
	return (NULL);
}
