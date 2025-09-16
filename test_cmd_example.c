/* Example usage of the t_cmd structure */

#include "../../includes/minishell.h"

// Test function for t_cmd structure (add this to your test files)
void test_cmd_parsing(void)
{
    t_mini mini;
    t_cmd *cmd;
    
    // Initialize mini structure
    mini.token = NULL;
    mini.exit_status = 0;
    
    // Test 1: Simple command
    printf("=== Test 1: Simple command ===\n");
    ft_tokenizer(&mini, "echo hello world");
    cmd = parse_command(mini.token);
    print_cmd(cmd);
    free_cmd(cmd);
    free_tokens(&mini);
    
    // Test 2: Command with redirection
    printf("\n=== Test 2: Command with redirection ===\n");
    ft_tokenizer(&mini, "echo hello > output.txt");
    cmd = parse_command(mini.token);
    print_cmd(cmd);
    free_cmd(cmd);
    free_tokens(&mini);
    
    // Test 3: Command with multiple redirections
    printf("\n=== Test 3: Multiple redirections ===\n");
    ft_tokenizer(&mini, "cat < input.txt > output.txt");
    cmd = parse_command(mini.token);
    print_cmd(cmd);
    free_cmd(cmd);
    free_tokens(&mini);
}
