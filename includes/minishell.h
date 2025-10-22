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
# include <limits.h>

# define LINE "]\001" SBYELLOW "\002 minishell$\001" SRESET "\002 "
# define COMMAND_NOT_FOUND 127
# define COMMAND_DENIED_OR_FAILED 126

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

extern volatile sig_atomic_t	g_signal;
// Structs
typedef enum e_node_type
{
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_CMD,
}	t_node_type;

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

typedef struct s_export
{
	char				*key;
	char				*value;
	struct s_export		*next;
}	t_export;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_heredoc
{
	char				*heredoc_delimeter;
	int					pipe_fd;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	char			*heredoc_delimeter;
	int				fd;
	struct s_redir	*next;
}	t_redir;

typedef struct s_ast
{
	t_node_type		type;
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
	t_export		*export_list;
	int				env_size;
	long				exit_status;
	int				pipe_count;
	int				**pipes;
	pid_t			*child_pids;
	int				child_count;
	t_env			*env_list;
	char			*pwd;
	t_heredoc		*heredoc;
	int				heredoc_fd;
	int				heredoc_signal;
}	t_mini;

// Commands
void		do_commands(t_mini *mini, char *input);
void		handle_commands(t_mini *mini, char *input);
int			execute_builtin(t_mini *mini, t_ast *node, t_redir *redir);
void		execute_ast_node(t_mini *mini, t_ast *node);
int			execute_external(t_mini *mini, char **argv);
int			execute_command(t_mini *mini, t_ast *node);
int			execute_external_command(t_mini *mini, t_ast *node,
	t_redir *redirects);
	
// Frees
int			ft_free_all(t_mini *mini, int ret, int exit_prog);
void		free_tokens(t_mini *mini);
void		free_ast(t_ast *node);
void		free_redir(t_redir *redir);
void		free_env_list(t_env *lst);
void		free_export_list(t_export *lst);

// Utils
int			is_operator(t_token_type type);
int			is_redirect(t_token_type type);
char		*ft_strcpy(char *dest, char *src);
int			print_syntax_error(t_mini *mini, char *error_msg, char *token);

// [BUILTINS]:
// Echo:
int			ft_echo(t_ast *node);

// Export:
int			ft_export(t_mini *mini, t_ast *node);
int			is_valid_identifier(char *str);

// Export list
t_export	*ft_newexp(char *key, char *value);
void		ft_addexp(t_export **lst, t_export *new);
t_export	*findexp(t_mini *mini, char *key);
int			ft_setexp(char *key, char *newvalue, t_mini *mini);
char		*ft_getexp(char *key, t_mini *mini);

// Exit:
int			ft_exit(t_mini *mini, t_ast *node);

// Pwd:
int			ft_pwd(t_mini *mini);

//	Env:
int			ft_env(t_mini *mini);
void		init_env_list(t_mini *mini, char **envp);
char		*ft_getenv(char *key, t_mini *mini);
int			ft_setenv(char *key, char *newvalue, t_mini *mini);
void		ft_addenv(t_env **lst, t_env *new);
t_env		*ft_newenv(char *key, char *value);
void		increment_shlvl(t_mini *mini);

// Unset :
int			ft_unset(t_mini *mini, t_ast *node);

// Cd:
int			ft_cd(t_mini *mini, t_ast *node);

// Redirects:
int			redirect_in(t_redir *redirect, t_mini *mini);
int			redirect_out(t_redir *redirect, t_mini *mini);
int			redirect_append(t_redir *redirect, t_mini *mini);
int			apply_redirections(t_redir *redirections, t_mini *mini);
int			backup_fd(int *stdin_backup, int *stdout_backup);
void		restore_fd(int stdin_backup, int stdout_backup);

// Heredoc:
int			redirect_heredoc(t_redir *redirect, t_mini *mini);
void		create_heredoc_pipe(t_mini *mini, char *delimiter, t_heredoc **heredoc);
void		heredoc_cleaner(t_heredoc **heredoc);
void		process_heredocs(t_mini *mini, t_ast *node);

// Signals
void		sighandler(int signal);
void		signal_init(void);
void		heredoc_sighandler(int signal);
void		setup_heredoc_signals(void);
void		wait_update_main(int pid, int *status);
void		setup_exec_signals(void);

// Tokenizer
//void	ft_tokenizer(t_mini *mini, char *input);
int			ft_tokenizer(t_mini *mini, char *input);
int			check_validity(t_mini *mini, char *input);
size_t		get_word_len(t_mini *mini, size_t len, size_t *i);
int			is_op(const char *input, size_t i);
char		*fill_token_data(t_mini *mini, size_t *i, size_t len);
int			build_ast(t_mini *mini);
int			is_builtin_command(char *cmd);
t_ast		*parse_command(t_mini *mini, t_token **tokens);
t_ast		*build_cmd_node(char **args, int arg_count, t_redir *redir);
t_ast		*build_operator_node(t_ast *left, t_ast *right, t_token_type type);

//Expansion
char		*extract_var_name(t_mini *mini, size_t *i);
char		*expand_variable(t_mini *mini, char *var_name);
int			is_special_var(char c);
char		*get_env_var(t_mini *mini, char *var_name);
size_t		expand_var_in_tokenizer(t_mini *mini, size_t *i);
int			heredoc_expand_safe(t_mini *mini);

//Expansion Utils
int			is_special_var(char c);
char		*expand_special_var(t_mini *mini, char var_char);
void		ft_itoa_alternative(int n, char *dst);

//Environment Utils
char		*get_env_var(t_mini *mini, char *var_name);
char		*get_exp_var(t_mini *mini, char *var_name);

//Command parsing
int			build_ast(t_mini *mini);
t_ast		*parse_or(t_mini *mini, t_token **tokens);

//[Pipes]
// Pipes Executor
int			execute_pipe_node(t_mini *mini, t_ast *node);

//External Commands
int			execute_external(t_mini *mini, char **argv);
char		*find_command_path(t_mini *mini, char *cmd);
void		print_command_error(char *cmd, char *error);

// BONUS
int			execute_and_node(t_mini *mini, t_ast *node);
int			execute_or_node(t_mini *mini, t_ast *node);

#endif
