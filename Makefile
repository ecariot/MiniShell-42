NAME	= minishell

LIB_DIR = ./libft

LIBFT	= ./libft/libft.a

SRCS	= main.c utils.c signal.c \
		lexer.c env/parse_env.c builtin/pwd.c \
		builtin/cd.c parsing/parser_plan.c \
		builtin/echo.c exe.c builtin/env.c \
		env/init_env.c builtin/export.c builtin/unset.c \
		parsing/pipe.c parsing/redir.c \
		parsing/append.c parsing/trim_quotes.c \
		parsing/parse_quotes.c env/expand_env.c \
		builtin/exit.c


OBJS	= ${SRCS:.c=.o}

RM	= rm -f

CC	= gcc

FLAGS	= -Wall -Wextra -Werror -g3

all:		${NAME}

.c.o:
		@${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${LIBFT}:
			@echo "\033[35m----Building libft----"
			@make -sC ${LIB_DIR}
			@echo "OK"

${NAME}:	${OBJS} ${LIBFT}
			@echo "\033[34m----Compiling----"
			@${CC} ${FLAGS} ${OBJS} -lreadline -o ${NAME} ${LIBFT}
			@echo "OK"

clean:
			@echo "\033[31m----Cleaning libft----"
			@make clean -sC ${LIB_DIR}
			@echo "OK"
			@echo "\033[31m----Cleaning objects----"
			@${RM} ${OBJS}
			@echo "OK"

fclean:		clean
			@echo "\033[33m----Cleaning all----"
			@${RM} ${NAME}
			@${RM} ${LIBFT}
			@echo "OK"

re:			fclean all

.PHONY:		all clean fclean re
