/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:45:17 by mqueiros          #+#    #+#             */
/*   Updated: 2025/11/01 13:20:45 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	cd_error(char *target_dir)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(target_dir, 2);
	ft_putstr_fd(": ", 2);
	if (errno == ENOENT)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (errno == EACCES)
		ft_putstr_fd("Permission denied\n", 2);
	else
		ft_putstr_fd("Not a directory\n", 2);
	return ;
}

static int	cd_dash(t_mini *mini)
{
	char	*target_dir;

	target_dir = ft_getenv("OLDPWD", mini);
	if (!target_dir)
		return (print_command_error("cd", "OLDPWD not set"), 1);
	if (chdir(target_dir) != 0)
		return (cd_error(target_dir), 1);
	ft_setenv("OLDPWD", ft_getenv("PWD", mini), mini);
	ft_setenv("PWD", target_dir, mini);
	return (0);
}

int	ft_cd(t_mini *mini, t_ast *node)
{
	char	*target_dir;
	char	*old_pdw_var;
	char	pwd[4096];

	target_dir = ft_getenv("HOME", mini);
	if (node->arg_count == 3)
		return (print_command_error("cd", "too many arguments"), 1);
	if (node->args[1] == NULL)
	{
		if (target_dir)
			return (chdir(target_dir), 0);
		else
			return (print_command_error("cd", "HOME not set"), 1);
	}
	target_dir = node->args[1];
	if (ft_strcmp("-", target_dir) == 0)
		return (cd_dash(mini));
	if (!target_dir)
		return (print_command_error("cd", "OLDPWD not set"), 1);
	old_pdw_var = ft_getenv("PWD", mini);
	ft_setenv("OLDPWD", old_pdw_var, mini);
	if (chdir(target_dir) != 0)
		return (cd_error(target_dir), 1);
	ft_setenv("PWD", getcwd(pwd, sizeof(pwd)), mini);
	return (0);
}
