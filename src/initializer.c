#include "minishell.h"

/**
 * @brief Initializes a t_mini structure with default values
 * 
 * This function initializes all members of the t_mini structure to their
 * default starting state. Pointers are set to NULL, numeric values to 0,
 * and the environment pointer is assigned.
 * 
 * @param mini Pointer to the t_mini structure to initialize
 * @param envp Array of environment variables
 * @return void
 */
void	mini_init(t_mini *mini, char **envp)
{
	mini->token = NULL;
	mini->ast = NULL;
	mini->input = NULL;
	mini->exit_status = 0;
	mini->export_list = NULL;
	mini->env_list = NULL;
	mini->heredoc = NULL;
	mini->heredoc_fd = 0;
	mini->heredoc_signal = 0;
	mini->envp = envp;
}
