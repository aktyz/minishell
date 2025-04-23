# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/04 15:20:00 by zslowian          #+#    #+#              #
#    Updated: 2025/04/11 09:59:40 by zslowian         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
NAME_TEST = tester
LIBFT = libft
LIBFT_F = ./libft
INCLUDES = -I ./includes -I $(LIBFT_F)/headers
CC = cc
FLAGS = -Wall -Wextra -Werror -g
LIB = -L $(LIBFT_F) -lft
RM = rm -f

SRC = main.c \
	create_commands.c \
	execution/resolve_command_path.c \
	execution/ft_process.c \
	execution/ft_run_builtin.c \
	execution/ft_create_pipe.c \
	utils_for_tokens.c \
	parser.c \
	characters.c \
	errors.c \
	var_expander.c \
	quotes.c \
	tests/test_ft_echo.c \
	tests/test_master.c \
	initialization.c \
	signals.c \
	cleanup.c \
	builtins/echo.c \
	builtins/exit.c \
	builtins/cd.c \
	builtins/pwd.c \
	builtins/export.c \
	builtins/unset.c \
	builtins/env.c \
	debug.c

SRC_TEST = create_commands.c \
	execution/resolve_command_path.c \
	execution/ft_process.c \
	execution/ft_create_pipe.c \
	execution/ft_run_builtin.c \
	utils_for_tokens.c \
	parser.c \
	errors.c \
	characters.c \
	var_expander.c \
	quotes.c \
	tests/test_ft_echo.c \
	tests/test_master.c \
	initialization.c \
	signals.c \
	cleanup.c \
	builtins/exit.c \
	builtins/echo.c \
	builtins/cd.c \
	builtins/pwd.c \
	builtins/export.c \
	builtins/unset.c \
	builtins/env.c \
	debug.c \
	tests.c

# libft_functions.c \
# tests/test_ft_echo.c \

OBJ = $(SRC:.c=.o)
OBJ_TEST = $(SRC_TEST:.c=.o)

%.o : %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g -pthread

all: $(LIBFT) $(NAME) $(NAME_TEST)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -l readline

$(NAME_TEST): $(OBJ_TEST) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_TEST) $(LIB) -o $(NAME_TEST)  -l readline -lcriterion

$(LIBFT):
	@git submodule update --init --recursive
	$(MAKE) -C $(LIBFT_F)

fclean: clean
	@$(RM) $(NAME) $(NAME_TEST) $(OBJ) $(OBJ_TEST)

clean:
	@$(RM) $(OBJ) $(OBJ_TEST)
	@$(MAKE) -C $(LIBFT_F) fclean

re: fclean all

rebug: fclean debug clean

debug: $(OBJ) $(OBJ_TEST) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -l readline -g -pthread
	@$(CC) $(CFLAGS) $(OBJ_TEST) $(LIB) -o $(NAME_TEST) -lcriterion -lreadline -g -pthread

.PHONY: all clean fclean libft re
