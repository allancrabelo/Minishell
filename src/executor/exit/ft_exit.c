#include "minishell.h"

int	ft_atoll_safe(const char *str, long long *result)
{
	int					sign;
	unsigned int		digit;
	unsigned long long	acum;
	unsigned long long	lim;

	sign = 1;
	acum = 0;
	lim = (unsigned long long)LLONG_MAX;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	if (sign < 0)
		lim = (unsigned long long)LLONG_MAX + 1ULL;
	while (*str && ft_isdigit((unsigned char)*str))
	{
		digit = (unsigned)(*str - '0');
		if (acum > (lim - digit) / 10ULL)
			return (0);
		acum = acum * 10ULL + digit;
		str++;
	}
	if (*str != '\0')
		return (0);
	if (sign < 0)
		*result = (long long)(-acum);
	else
		*result = (long long)acum;
	return (1);
}

static int	get_num_256(char *str, long *res)
{
	long long	val;
	int			i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (!ft_atoll_safe(str, &val))
		return (0);
	*res = (val % 256 + 256) % 256;
	return (1);
}

int	ft_exit(t_mini *mini, t_ast *node)
{
	long	res;
	int		arg_index;

	write(1, "exit\n", 5);
	res = 0;
	arg_index = 1;
	if (node->arg_count >= 2 && ft_strcmp(node->args[arg_index], "--") == 0)
		arg_index++;
	if (node->arg_count > arg_index)
	{
		if (!get_num_256(node->args[arg_index], &res))
		{
			print_command_error("exit", "numeric argument required");
			ft_free_all(mini, 2, 1);
		}
		if (node->arg_count > arg_index + 1)			
			return (print_command_error("exit", "too many arguments"), 1);
		mini->exit_status = res;
	}
	free_tokens(mini);
	free_ast(mini->ast);
	free_export_list(mini->export_list);
	exit(mini->exit_status);
}
