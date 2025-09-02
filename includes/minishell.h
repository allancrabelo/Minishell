/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:05:28 by aaugusto          #+#    #+#             */
/*   Updated: 2025/09/02 08:49:27 by mqueiros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Includes
# include "../libft/libft.h"
# include <stdio.h>				// printf, perror
# include <stdlib.h>				// malloc, free, exit
# include <unistd.h>				// write, access, read, close, fork, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot, getcwd
# include <fcntl.h>				// open
# include <sys/stat.h>			// stat, lstat, fstat
# include <sys/wait.h>			// wait, waitpid, wait3, wait4
# include <signal.h>				// signal, sigaction, sigemptyset, sigaddset, kill
# include <dirent.h>				// opendir, readdir, closedir
# include <string.h>				// strerror
# include <errno.h>				// errno
# include <termios.h>			// tcsetattr, tcgetattr
# include <sys/ioctl.h>			// ioctl
# include <curses.h>				// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (ou <termcap.h>)
# include <readline/readline.h>	// readline
# include <readline/history.h>	// add_history, rl_clear_history, etc.

# define LINE SYELLOW "minishell> " SRESET

// Structs
typedef enum e_token_type
{
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_WORD
}	t_token_type;

typedef struct s_token
{
	char			*data;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_mini
{
	t_token	*token;
	int		quote;
}				t_mini;

// Commands
void	do_commands(t_mini *mini, char *input);
void	handle_commands(t_mini *mini, char *input);
int		ft_echo(char **commands);

// Signals
void	sighandler(int signal);
void	signal_init(void);

// [Utils]:
// Echo:
char	**ft_split_ws(const char *s);
int		is_ws(char c);
int		count_words(const char *s);
int		word_len(const char *s);
char	*dup_word(const char *s, int len);

//void	ft_tokenizer(t_mini *mini, char *input);
int				ft_tokenizer(t_mini *mini, char *input);
int				check_quotes_validity(const char *input);
char			*ft_strndup(const char *str, size_t len);
void			advance_through_word(const char **input);
size_t			get_word_length(const char *input);
void			skip_whitespace(const char **input);
int				is_operator_char(char c);
t_token_type	get_op_type(const char *input, size_t *advance);
size_t			handle_quoted_section(const char **input, char quote);
size_t			handle_unquoted_section(const char **input);
void			add_token(t_mini *mini, const char *value, size_t len, t_token_type type);

#endif