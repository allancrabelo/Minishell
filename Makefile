NAME		:= minishell
CC			:= cc
RM			:= rm -f

CFLAGS		:= -Wall -Wextra -Werror
DEBUGFLAGS	:= -g
VALFLAGS	:=	--leak-check=full \
				--show-leak-kinds=all \
				--track-origins=yes \
				--suppressions=readline.supp

HEADDIR		:= ./includes/
HEADLIST	:= minishell.h
HEADERS		:= $(addprefix ${HEADDIR}, ${HEADLIST})

VPATH		:=	src src/utils \
				src/executor \
				src/cmd \
				src/executor/echo \
				src/executor/exit \
				src/executor/pwd \
				src/executor/env \
				src/executor/redirections \
				src/parse \
				src/expander \
				src/pipes 


SRCSLIST	:=	main \
				signals \
				handle_commands \
				ft_echo \
				ft_exit \
				ft_pwd \
				ft_env \
				redirections \
				redirections_executor \
				tokenizer \
				tokenizer_utils \
				parse \
				variable_expanser \
				expander_utils \
				environment_utils \
				pipes_utils \
				pipes_executor \
				external_commands

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
	${RM} -r ${OBJSDIR}
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

val: $(NAME)
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
	@valgrind $(VALFLAGS) ./$(NAME)

debug: CFLAGS += -g -fsanitize=address -fno-omit-frame-pointer
debug: re

release: CFLAGS += -O3
release: re

.PHONY: all clean fclean re run debug release
