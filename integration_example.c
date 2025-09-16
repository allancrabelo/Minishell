/* Integration example showing how to use t_cmd with handle_commands.c */

#include "../../includes/minishell.h"

// Updated execute function that uses t_cmd structure
int execute_cmd_structure(t_mini *mini, t_cmd *cmd)
{
    int result;
    
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (1);
    
    // Handle redirections first
    if (cmd->redirects && !setup_redirections(cmd->redirects))
        return (1);
    
    // Check if it's a builtin command
    if (is_builtin(cmd->argv[0]))
        result = execute_builtin_cmd(mini, cmd);
    else
        result = execute_external_cmd(mini, cmd);
    
    // Clean up redirections
    if (cmd->redirects)
        restore_redirections();
    
    return (result);
}

// Helper function to check if command is builtin
int is_builtin(char *cmd)
{
    return (ft_strcmp(cmd, "echo") == 0 ||
            ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "pwd") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0 ||
            ft_strcmp(cmd, "env") == 0 ||
            ft_strcmp(cmd, "exit") == 0);
}

// Execute builtin using t_cmd structure
int execute_builtin_cmd(t_mini *mini, t_cmd *cmd)
{
    if (ft_strcmp(cmd->argv[0], "echo") == 0)
        return (ft_echo_argv(mini, cmd->argv)); // Modified version that uses argv
    if (ft_strcmp(cmd->argv[0], "pwd") == 0)
        return (ft_pwd());
    if (ft_strcmp(cmd->argv[0], "exit") == 0)
        return (ft_exit_argv(mini, cmd->argv));
    // Add other builtins...
    return (0);
}

// Your main processing function would now look like this:
int process_command_line(t_mini *mini, char *input)
{
    t_cmd *cmd;
    int result;
    
    // Tokenize the input
    if (ft_tokenizer(mini, input) != 0)
        return (1);
    
    // Parse tokens into command structure
    cmd = parse_command(mini->token);
    if (!cmd)
    {
        free_tokens(mini);
        return (1);
    }
    
    // Execute the command
    result = execute_cmd_structure(mini, cmd);
    
    // Clean up
    free_cmd(cmd);
    free_tokens(mini);
    
    return (result);
}
