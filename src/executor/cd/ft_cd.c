#include "minishell.h"

int	ft_cd(t_mini *mini, t_ast *node)
{
	char	*target_dir;
	char	*old_pdw_var;
	char	pwd[4096];

	(void)mini;
	target_dir = ft_getenv("HOME", mini);
	if (node->args[1] == NULL)
	{
		if (target_dir)
			return (chdir(target_dir), 0);
		else
			return (print_command_error("cd", "HOME not set"), 1);
	}
	target_dir = node->args[1];
	old_pdw_var = ft_getenv("PWD", mini);
	ft_setenv("OLDPWD", old_pdw_var, mini);
	if (chdir(target_dir) != 0)
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
		return (1);
	}
	ft_setenv("PWD", getcwd(pwd, sizeof(pwd)), mini);
	return (0);
}
