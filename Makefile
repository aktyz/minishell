# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/04 15:20:00 by zslowian          #+#    #+#              #
#    Updated: 2025/04/09 18:03:54 by zslowian         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
LIBFT = libft
LIBFT_F = ./libft
INCLUDES = -I ./includes -I $(LIBFT_F)/headers
CC = cc
FLAGS = -Wall -Wextra -Werror -g
LIB = -L $(LIBFT_F) -lft -lcriterion
RM = rm -f

SRC = main.c \
	create_commands.c \
	execution/execve_args_allocation.c \
	execution/execve_data_allocation.c \
	execution/extract_env_var.c \
	utils_for_tokens.c \
	parser.c \
	var_expander.c \
	quotes.c \
	execution/resolve_command_path.c \
	execution/ft_process.c \
	execution/ft_run_builtin.c \
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

%.o : %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g -pthread

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)  -l readline

all: $(LIBFT) $(NAME)

$(LIBFT):
	@git submodule update --init --recursive
	@$(MAKE) -C $(LIBFT_F)

fclean: clean
	@$(RM) $(NAME) $(OBJ)

clean:
	@$(RM) $(OBJ)
	@$(MAKE) -C $(LIBFT_F) fclean

re: fclean all

debug: $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -g -pthread -l readline

rebug: fclean debug clean

.PHONY: all clean fclean libft re
