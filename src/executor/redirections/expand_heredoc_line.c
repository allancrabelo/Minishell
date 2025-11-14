#include "minishell.h"

static char	*expand_var(t_mini *mini, char *line,
			const int len, const int start)
{
	char	*var_name;
	char	*value;

	if (len == 0)
		return (ft_strdup("$"));
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, line + start, len + 1);
	value = get_env_var(mini, var_name);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static char	*extract_and_expand_var(t_mini *mini, char *line, int *i)
{
	int		start;
	int		len;

	(*i)++;
	start = *i;
	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(mini->exit_status));
	}
	len = 0;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
	{
		(*i)++;
		len++;
	}
	return (expand_var(mini, line, len, start));
}

static void	fill_heredoc_var(char **result, char *tmp, int *j, const int len)
{
	int	tmp_len;

	tmp_len = ft_strlen(tmp);
	if (*j + tmp_len < len)
	{
		ft_strlcpy(*result + *j, tmp, len - *j);
		*j += tmp_len;
	}
	free(tmp);
}

static char	*fill_heredoc_line(t_mini *mini, char *line,
			char **result, const int len)
{
	int		i;
	int		j;
	char	*tmp;

	j = 0;
	i = 0;
	while (line[i] && j < len - 1)
	{
		if (line[i] == '$' && line[i + 1])
		{
			tmp = extract_and_expand_var(mini, line, &i);
			if (tmp)
				fill_heredoc_var(result, tmp, &j, len);
		}
		else
			(*result)[j++] = line[i++];
	}
	(*result)[j] = '\0';
	return (*result);
}

char	*expand_heredoc_line(t_mini *mini, char *line)
{
	char	*result;
	int		len;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	len = 4096;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	fill_heredoc_line(mini, line, &result, len);
	return (result);
}
