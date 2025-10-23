#include "minishell.h"

static void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

int ft_atoll_safe(const char *str, long long *result)
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

static int get_num_256(char *str, long *res)
{
	long long	val;

	if (!str || !str[0])
		return (0);
	int i = 0;
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
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(node->args[arg_index], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			free_tokens(mini);
			free_ast(mini->ast);
			free_export_list(mini->export_list);
			close_extra_fds();
			exit(2);
		}
		if (node->arg_count > arg_index + 1)
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		mini->exit_status = res;
	}
	close_extra_fds();
	ft_free_all(mini, mini->exit_status, 1);
	return (EXIT_SUCCESS);
}
