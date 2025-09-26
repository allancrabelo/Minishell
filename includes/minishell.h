#ifndef MINISHELL_H
# define MINISHELL_H

// Includes
# include "../libft/libft.h"
# include <stdio.h>				// printf, perror
# include <stdlib.h>			// malloc, free, exit
# include <unistd.h>			// write, access, read, close, fork, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot, getcwd
# include <fcntl.h>				// open
# include <sys/stat.h>			// stat, lstat, fstat
# include <sys/wait.h>			// wait, waitpid, wait3, wait4
# include <signal.h>			// signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>			// opendir, readdir, closedir
# include <string.h>			// strerror
# include <errno.h>				// errno
# include <termios.h>			// tcsetattr, tcgetattr
# include <sys/ioctl.h>			// ioctl
# include <curses.h>			// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (ou <termcap.h>)
# include <readline/readline.h>	// readline
# include <readline/history.h>	// add_history, rl_clear_history, etc.

# define LINE SYELLOW "minishell> " SRESET
# define COMMAND_NOT_FOUND 127
# define COMMAND_DENIED_OR_FAILED 126

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

// Structs
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND
}	t_token_type;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_redir
{
	t_token_type		type;
	char				*file;
	int					fd;
	struct s_redir		*next;
}	t_redir;

typedef struct s_ast
{
	int				type;
	struct s_ast	*left;
	struct s_ast	*right;
	char			**args;
	int				arg_count;
	t_redir			*redir;
}	t_ast;

typedef struct s_mini
{
	t_ast			*ast;
	t_token_type	type;
	t_token			*token;
	char			*input;
	char			**envp;
	int				exit_status;
	int				pipe_count;
	int				**pipes;
	pid_t			*child_pids;
	int				child_count;
}	t_mini;

// Commands
void	do_commands(t_mini *mini, char *input);
void	handle_commands(t_mini *mini, char *input);
int		execute_builtin(t_mini *mini, t_ast *node, t_redir *redir);
void	execute_ast_node(t_mini *mini, t_ast *node);
int		execute_external(t_mini *mini, char **argv);
int		execute_command(t_mini *mini, t_ast *node);
int		execute_external_command(t_mini *mini, t_ast *node, t_redir *redirects);

// [BUILTINS]:
//int		ft_echo(char **commands);
int		ft_echo(t_ast *node);
int		ft_exit(t_mini *mini);
int		ft_pwd(void);

//Env:
int		ft_env(t_mini *mini);

// Redirects:
int		redirect_in(t_redir *redirect);
int		redirect_out(t_redir *redirect);
int		redirect_append(t_redir *redirect);
int		apply_redirections(t_redir *redirections);
int		backup_fd(int *stdin_backup, int *stdout_backup);
void	restore_fd(int stdin_backup, int stdout_backup);

// Signals
void	sighandler(int signal);
void	signal_init(void);

// Tokenizer
//void	ft_tokenizer(t_mini *mini, char *input);
int		ft_tokenizer(t_mini *mini, char *input);
void	free_tokens(t_mini *mini);
int		check_validity(char *input);
size_t	get_word_len(t_mini *mini, size_t len, size_t i);
int		is_op(const char *input, size_t i);
int		build_ast(t_mini *mini);
int		is_builtin_command(char *cmd);

//Expansion
char	*extract_var_name(t_mini *mini, size_t *i);
char	*expand_variable(t_mini *mini, char *var_name);
int		is_special_var(char c);
char	*get_env_var(t_mini *mini, char *var_name);
size_t	expand_var_in_tokenizer(t_mini *mini, size_t *i);

//Expansion Utils
int		is_special_var(char c);
char	*expand_special_var(t_mini *mini, char var_char);
void	ft_itoa_alternative(int n, char *dst);

//Environment Utils
char	*get_env_var(t_mini *mini, char *var_name);

//Command parsing
int		build_ast(t_mini *mini);
void	free_ast(t_ast *node);

//[Pipes]
// Pipes Executor
int		execute_pipe_node(t_mini *mini, t_ast *node);

//External Commands
int		execute_external(t_mini *mini, char **argv);
char	*find_command_path(t_mini *mini, char *cmd);
void	print_command_error(char *cmd, char *error);

#endif