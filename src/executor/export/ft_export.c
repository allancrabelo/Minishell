#include "minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if ((str[i] == '+' && str[i + 1] == '=') || (str[i] == '+'
				&& str[i + 1] == '=' && str[i + 3] == ':'))
		{
			if ((str[i] == '+' && str[i + 1] == '=' && str[i + 3] == ':'))
				i += 3;
			else
				i += 2;
			break ;
		}
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_list(t_mini *mini)
{
	t_export	*lst;

	lst = mini->export_list;
	while (lst)
	{
		if (lst->value == NULL)
			printf("declare -x %s\n", lst->key);
		else
			printf("declare -x %s=\"%s\"\n", lst->key, lst->value);
		lst = lst->next;
	}
	mini->exit_status = 0;
}

static void	extract_key_value(char *arg, char **key, char **value)
{
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		*key = malloc(equal_pos - arg + 1);
		if (!*key)
			return ;
		ft_strlcpy(*key, arg, equal_pos - arg + 1);
		*value = ft_strdup(equal_pos + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
}

static int	process_export_arg(t_mini *mini, char *arg)
{
	char	*key;
	char	*value;
	int		result;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		mini->exit_status = 1;
		return (1);
	}
	key = NULL;
	value = NULL;
	extract_key_value(arg, &key, &value);
	result = ft_setexp(key, value, mini);
	if (result == 0 && value != NULL)
		result = ft_setenv(key, value, mini);
	free(key);
	if (value)
		free(value);
	return (result);
}

int	ft_export(t_mini *mini, t_ast *node)
{
	int	i;
	int	ret;

	if (!node->args[1])
	{
		print_export_list(mini);
		return (0);
	}
	ret = 0;
	i = 1;
	while (node->args[i])
	{
		if (process_export_arg(mini, node->args[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
