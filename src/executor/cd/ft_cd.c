#include "minishell.h"

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
}

static char	*build_manual_pwd(char *old_pwd, char *target_dir)
{
	char	*tmp;
	char	*new_pwd;

	if (!old_pwd)
		return (ft_strdup(target_dir));
	tmp = ft_strjoin(old_pwd, "/");
	if (!tmp)
		return (NULL);
	new_pwd = ft_strjoin(tmp, target_dir);
	free(tmp);
	return (new_pwd);
}

static int	set_pwd_oldpwd(t_mini *mini, char *target_dir)
{
	char	pwd[4096];
	char	*cwd_result;
	int		getcwd_failed;

	getcwd_failed = 0;
	if (findenv(mini, "OLDPWD") || mini->old_pwd == NULL)
		ft_setenv("OLDPWD", mini->pwd, mini);
	if (mini->old_pwd)
		free(mini->old_pwd);
	mini->old_pwd = mini->pwd;
	cwd_result = getcwd(pwd, sizeof(pwd));
	if (cwd_result == NULL)
	{
		print_command_error("cd", "error retrieving current directory: getcwd: "
			"cannot access parent directories: No such file or directory");
		mini->pwd = build_manual_pwd(mini->old_pwd, target_dir);
		getcwd_failed = 1;
	}
	else
		mini->pwd = ft_strdup(cwd_result);
	if (findenv(mini, "PWD"))
		ft_setenv("PWD", mini->pwd, mini);
	return (getcwd_failed);
}

static int	cd_dash(t_mini *mini)
{
	char	*target_dir;

	target_dir = ft_getenv("OLDPWD", mini);
	if (!target_dir)
		return (print_command_error("cd", "OLDPWD not set"), 1);
	if (chdir(target_dir) != 0)
		return (cd_error(target_dir), 1);
	set_pwd_oldpwd(mini, target_dir);
	printf("%s\n", mini->pwd);
	return (0);
}

int	ft_cd(t_mini *mini, t_ast *node)
{
	char	*target_dir;

	target_dir = ft_getenv("HOME", mini);
	if (node->arg_count == 3)
		return (print_command_error("cd", "too many arguments"), 1);
	if (node->args[1] == NULL)
	{
		if (!target_dir)
			return (print_command_error("cd", "HOME not set"), 1);
	}
	else
		target_dir = node->args[1];
	if (ft_strcmp("-", target_dir) == 0)
		return (cd_dash(mini));
	if (chdir(target_dir) != 0)
		return (cd_error(target_dir), 1);
	return (set_pwd_oldpwd(mini, target_dir));
}
