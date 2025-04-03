# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/04 15:20:00 by zslowian          #+#    #+#              #
#    Updated: 2025/04/09 16:24:16 by zslowian         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
NAME_TEST = tests
LIBFT = libft
LIBFT_F = ./libft
INCLUDES = -I ./includes -I $(LIBFT_F)/headers
CC = cc
FLAGS = -Wall -Wextra -Werror -g
LIB = -L $(LIBFT_F) -lft 
RM = rm -f

SRC = main.c \
	create_commands.c \
	execve_args_allocation.c \
	execve_data_allocation.c \
	extract_env_var.c \
	utils_for_tokens.c \
	parser.c \
	var_expander.c \
	quotes.c \
	resolve_command_path.c \
	ft_process.c \
	initialization.c \
	cleanup.c \
	debug.c 

SRC_TEST = execve_args_allocation.c \
	execve_data_allocation.c \
	utils_for_tokens.c \
	parser.c \
	var_expander.c \
	quotes.c \
	ft_process.c \
	libft_functions.c \
	initialization.c \
	cleanup.c \
	debug.c \
	tests/test_exe_functions.c \
	tests/test_ft_echo.c \
	tests/test_master.c \
	tests/test_single_cmd.c \
	builtins/echo.c \
	signals.c \
	tests.c

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
