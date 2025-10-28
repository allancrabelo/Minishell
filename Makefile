NAME		:= minishell
CC			:= cc
RM			:= rm -f

CFLAGS		:= -Wall -Wextra -Werror -fPIE
DEBUGFLAGS	:= -g
VALFLAGS	:=	--leak-check=full \
				--show-leak-kinds=all \
				--track-origins=yes \
				--suppressions=readline.supp \
				--track-fds=yes

HEADDIR		:= ./includes/
HEADLIST	:= minishell.h
HEADERS		:= $(addprefix ${HEADDIR}, ${HEADLIST})

VPATH		:=	src src/utils \
				src/executor \
				src/cmd \
				src/executor/echo \
				src/executor/exit \
				src/executor/export \
				src/executor/pwd \
				src/executor/env \
				src/executor/cd \
				src/executor/unset \
				src/executor/redirections \
				src/parse \
				src/expander \
				src/pipes \
				src/bonus \


SRCSLIST	:=	main \
				initializer \
				signals \
				especial_signals \
				handle_commands \
				utils \
				atoll \
				ft_echo \
				ft_exit \
				ft_pwd \
				ft_env \
				env_list \
				ft_cd \
				ft_unset \
				ft_export \
				export_list \
				heredoc \
				redirections \
				redirections_executor \
				tokenizer \
				tokenizer_utils \
				fill_token_data \
				fil_token_data_utils \
				verify_tokens \
				parser \
				parse_args \
				parse_command \
				parse_ordered \
				operator_checker \
				variable_expander \
				expander_utils \
				environment_utils \
				pipes_executor \
				signals_pipe_update \
				external_commands \
				free_all \
				free_all_redirections \
				and_or_bonus

SRCS		:= $(addsuffix .c, ${SRCSLIST})

OBJSDIR		:= ./obj/
OBJS		:= ${SRCS:%.c=${OBJSDIR}%.o}

LIBFTDIR	:= ./libft/
LIBFT		:= ${LIBFTDIR}libft.a

CYAN		:= \033[96m
RED			:= \033[91m
RESET		:= \033[0m

# ============================
# Default build
# ============================

all: ${NAME}

${NAME}: ${OBJS} ${LIBFT}
	@echo ""
	@echo "${CYAN}Linking ${NAME} ...${RESET}"
	${CC} ${CFLAGS} ${DEBUGFLAGS} ${OBJS} ${LIBFT} -o ${NAME} -lreadline
	@echo ""
	@echo "${CYAN}${NAME} Ready to use${RESET}"

${OBJSDIR}%.o: %.c ${HEADERS}
	@echo "${CYAN}Compiling $< ...${RESET}"
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -I${HEADDIR} ${DEBUGFLAGS} -c $< -o $@

${LIBFT}:
	@echo "${CYAN}Compiling libft ...${RESET}"
	@make -C ${LIBFTDIR} --no-print-directory

# ============================
# Cleaning
# ============================

clean:
	@echo ""
	@echo "${RED}Deleting ${NAME} Objects ...${RESET}"
	@${RM} -r ${OBJSDIR}
	@make -C ${LIBFTDIR} clean --no-print-directory --quiet

fclean:
	@echo ""
	@echo "${RED}Deleting ${NAME} Executable ...${RESET}"
	@${RM} ${NAME}
	@${RM} -r ${OBJSDIR}
	@make -C ${LIBFTDIR} fclean --no-print-directory --quiet

re: fclean all

# ============================
# Extra targets
# ============================

run: ${NAME}
	./${NAME}

val: re
	@echo "{" > readline.supp
	@echo "	leak readline" >> readline.supp
	@echo "	Memcheck:Leak" >> readline.supp
	@echo "..." >> readline.supp
	@echo "	fun:readline" >> readline.supp
	@echo "}" >> readline.supp
	@echo "{" >> readline.supp
	@echo "	leak add_history" >> readline.supp
	@echo "	Memcheck:Leak" >> readline.supp
	@echo "..." >> readline.supp
	@echo "	fun:add_history" >> readline.supp
	@echo "}" >> readline.supp
	@valgrind ${VALFLAGS} ./${NAME}

debug: CFLAGS += -g -fsanitize=address -fno-omit-frame-pointer
debug: re

release: CFLAGS += -O3
release: re

.PHONY: all clean fclean re run debug release
