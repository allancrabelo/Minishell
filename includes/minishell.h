/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:05:28 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/15 16:06:34 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_mini
{
	t_token			*token;
	t_token_type	type;
	char			*input;
	char			**envp;
	int				exit_status;
}	t_mini;

// Commands
void	do_commands(t_mini *mini, char *input);
void	handle_commands(t_mini *mini, char *input);
//int		ft_echo(char **commands);
int		ft_echo(t_mini *mini, t_token *echo);

// Signals
void	sighandler(int signal);
void	signal_init(void);

//void	ft_tokenizer(t_mini *mini, char *input);
int		ft_tokenizer(t_mini *mini, char *input);
void	free_tokens(t_token **token);
int		check_validity(char *input);
size_t	get_word_len(t_mini *mini, size_t len, size_t i);
int		is_op(const char *input, size_t i);

//Expansion
size_t	expand_var_in_tokenizer(t_mini *mini, size_t *i);
char	*extract_var_name_special(t_mini *mini, size_t *i);
char	*expand_variable(t_mini *mini, char *var_name);

//Expansion Utils
int		is_special_var(char c);
char	*expand_special_var(t_mini *mini, char var_char);
void	ft_itoa_alternative(int n, char *dst);

//Environment Utils
char	*get_env_var(t_mini *mini, char *var_name);

#endif