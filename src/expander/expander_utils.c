#include "minishell.h"

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

char	*expand_exit_status(t_mini *mini)
{
	char	*result;

	result = malloc (sizeof(char) * 12);
	if (!result)
		return (NULL);
	ft_itoa_alternative(mini->exit_status, result);
	return (result);
}

char	*expand_special_var(t_mini *mini, char var_char)
{
	if (var_char == '?')
		return (expand_exit_status(mini));
	return (NULL);
}

int	is_special_var(char c)
{
	return (c == '?' || c == '$' || c == '!'
		|| (c >= '0' && c <= '9') || c == '*');
}
