NAME		:= minishell
CC			:= cc
RM			:= rm -f
FLAGS		:= -Wall -Wextra -Werror
DEBUG		:= -g

HEADDIR		:= ./includes/
HEADLIST	:= minishell.h
HEADERS		:= $(addprefix ${HEADDIR}, ${HEADLIST})

SRCSDIR		:= ./src/
SRCSLIST	:= main.c
SRCS		:= $(addprefix ${SRCSDIR}, ${SRCSLIST})

OBJSDIR		:= ./obj/
OBJSLIST	:= ${SRCSLIST:.c=.o}
OBJS		:= $(addprefix ${OBJSDIR}, ${OBJSLIST})

CYAN		:= \033[96m
RED			:= \033[91m
RESET		:= \033[0m

all: ${NAME}

${NAME}: ${OBJS}
	@echo ""
	@echo "${CYAN}Linking ${NAME} ...${RESET}"
	${CC} ${FLAGS} ${DEBUG} ${OBJS} -o ${NAME}
	@echo ""
	@echo "${CYAN}${NAME} Ready to use${RESET}"


${OBJSDIR}%.o: ${SRCSDIR}%.c ${HEADERS} | ${OBJSDIR}
	@echo "${CYAN}Compiling $< ...${RESET}"
	${CC} ${FLAGS} ${DEBUG} -c $< -o $@


${OBJSDIR}:
	mkdir -p ${OBJSDIR}

clean:
	@echo ""
	@echo "${RED}Deleting ${NAME} Objects ...${RESET}"
	${RM} -r ${OBJSDIR}

fclean: clean
	@echo ""
	@echo "${RED}Deleting ${NAME} Executable ...${RESET}"
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re