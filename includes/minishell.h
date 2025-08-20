/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaugusto <aaugusto@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:05:28 by aaugusto          #+#    #+#             */
/*   Updated: 2025/08/21 00:49:19 by aaugusto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Includes
#include <stdio.h>				// printf, perror
#include <stdlib.h>				// malloc, free, exit
#include <unistd.h>				// write, access, read, close, fork, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot, getcwd
#include <fcntl.h>				// open
#include <sys/stat.h>			// stat, lstat, fstat
#include <sys/wait.h>			// wait, waitpid, wait3, wait4
#include <signal.h>				// signal, sigaction, sigemptyset, sigaddset, kill
#include <dirent.h>				// opendir, readdir, closedir
#include <string.h>				// strerror
#include <errno.h>				// errno
#include <termios.h>			// tcsetattr, tcgetattr
#include <sys/ioctl.h>			// ioctl
#include <curses.h>				// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (ou <termcap.h>)
#include <readline/readline.h>	// readline
#include <readline/history.h>	// add_history, rl_clear_history, etc.


// Commands
void	do_commands(char *input);
void	handle_commands(char *input);
int		ft_echo(char **commands);

// Utils
int		ft_strcmp(char *s1, char *s2);
char	**ft_split(char const *s, char c);
int		count_words(char *str);

#endif