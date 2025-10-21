#include "minishell.h"

int	is_operator(t_token_type type)
{
	return (type == TOKEN_OR || type == TOKEN_AND || type == TOKEN_PIPE);
}

int	is_redirect(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN 
		|| type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND);
}

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