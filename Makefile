NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iincludes
READLINE_FLAGS = -lreadline

SRC_DIR = src
MAIN_DIR = $(SRC_DIR)/main
LEXER_DIR = $(SRC_DIR)/lexer
PARSER_DIR = $(SRC_DIR)/parser
SIGNALS_DIR = $(SRC_DIR)/signals
UTILS_DIR = $(SRC_DIR)/utils
BUILTINS_DIR = $(SRC_DIR)/builtins
EXECUTOR_DIR = $(SRC_DIR)/executor
PERMISSIONS_DIR = $(SRC_DIR)/permissions

SRCS = $(MAIN_DIR)/main.c \
	   $(MAIN_DIR)/main_cleanup.c \
	   $(MAIN_DIR)/main_utils.c \
	   $(LEXER_DIR)/lexer.c \
	   $(LEXER_DIR)/lexer_words.c \
	   $(LEXER_DIR)/lexer_redirect.c \
	   $(LEXER_DIR)/lexer_utils.c \
	   $(PARSER_DIR)/parser.c \
	   $(PARSER_DIR)/parser_utils.c \
	   $(PARSER_DIR)/parser_args.c \
	   $(PARSER_DIR)/parser_redirect.c \
	   $(PARSER_DIR)/parser_free.c \
	   $(PARSER_DIR)/parser_debug.c \
	   $(SIGNALS_DIR)/signals.c \
	   $(SIGNALS_DIR)/signals_heredoc.c \
	   $(UTILS_DIR)/env_utils.c \
	   $(UTILS_DIR)/prompt.c \
	   $(UTILS_DIR)/token_utils.c \
	   $(UTILS_DIR)/token_utils_2.c \
	   $(BUILTINS_DIR)/builtin_cd.c \
	   $(BUILTINS_DIR)/builtin_echo.c \
	   $(BUILTINS_DIR)/builtin_env.c \
	   $(BUILTINS_DIR)/builtin_unset.c \
	   $(BUILTINS_DIR)/builtin_export.c \
	   $(BUILTINS_DIR)/builtin_exit.c \
	   $(BUILTINS_DIR)/builtin_pwd.c \
	   $(BUILTINS_DIR)/env_manipulation.c \
	   $(BUILTINS_DIR)/builtin_utils.c \
	   $(EXECUTOR_DIR)/executor_minimal.c \
 	$(EXECUTOR_DIR)/exec_helpers.c \
 	$(EXECUTOR_DIR)/pipeline.c \
	   $(EXECUTOR_DIR)/expansion.c \
	   $(EXECUTOR_DIR)/expansion_utils.c \
	   $(PERMISSIONS_DIR)/permissions.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@make -C libft
	$(CC) $(CFLAGS) $(OBJS) -L./libft -lft -o $(NAME) $(READLINE_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -C libft clean
	@rm -f $(OBJS)

fclean: clean
	@make -C libft fclean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re