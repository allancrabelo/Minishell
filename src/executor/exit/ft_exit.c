/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:26 by mqueiros          #+#    #+#             */
/*   Updated: 2025/11/06 08:14:01 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (node->arg_count > arg_index)
	{
		if (!get_num_256(node->args[arg_index], &res))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(node->args[arg_index], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			close_extra_fds();
			ft_free_all(mini, 2, 1);
		}
		if (node->arg_count > arg_index + 1)
			return (print_command_error("exit", "too many arguments"),
				mini->exit_status + (mini->exit_status == 0));
		mini->exit_status = res;
	}
	close_extra_fds();
	ft_free_all(mini, mini->exit_status, 1);
	return (EXIT_SUCCESS);
}
