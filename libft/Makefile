# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mqueiros <mqueiros@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/23 00:58:27 by aaugusto          #+#    #+#              #
#    Updated: 2025/09/16 09:45:53 by mqueiros         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= libft.a
CC		:= cc
FLAGS	:= -Wall -Wextra -Werror -fPIC -fPIE
DEBUG	:= -g
SRCS	:=	ft_atoi.c \
				ft_bzero.c \
				ft_calloc.c \
				ft_isalnum.c \
				ft_isalpha.c \
				ft_isascii.c \
				ft_isdigit.c \
				ft_isprint.c \
				ft_itoa.c \
				ft_memchr.c \
				ft_memcmp.c \
				ft_memcpy.c \
				ft_memmove.c \
				ft_memset.c \
				ft_putchar_fd.c \
				ft_putendl_fd.c \
				ft_putnbr_fd.c \
				ft_putstr_fd.c \
				ft_split.c \
				ft_free_split.c \
				ft_strchr.c \
				ft_strdup.c \
				ft_striteri.c \
				ft_strjoin.c \
				ft_strlcat.c \
				ft_strlcpy.c \
				ft_strlen.c \
				ft_strmapi.c \
				ft_strncmp.c \
				ft_strcmp.c \
				ft_strnstr.c \
				ft_strrchr.c \
				ft_strtrim.c \
				ft_substr.c \
				ft_tolower.c \
				ft_toupper.c \
				get_next_line.c \
				ft_printf.c \
				ft_printf_hex.c \
				ft_printf_utils.c \
				ft_isspace.c 

OBJS	:= $(SRCS:.c=.o)
RM		:= rm -f

%.o:	%.c
			$(CC) $(FLAGS) $(DEBUG) -c $< -o $@

$(NAME):	$(OBJS)
			ar rcs $(NAME) $(OBJS)

all:	$(NAME)

clean:
			$(RM) *.o

fclean:	clean
			$(RM) $(NAME)

re:
			fclean all

so:
			$(CC) -nostartfiles -fPIC $(CFLAGS) $(SRCS)
			gcc -nostartfiles -shared -o libft.so $(OBJS)