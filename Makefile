# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/04 15:20:00 by zslowian          #+#    #+#              #
#    Updated: 2025/04/05 18:57:11 by zslowian         ###   ########.fr        #
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
	execve_args_allocation.c \
	execve_data_allocation.c \
	extract_env_var.c \
	utils_for_tokens.c \
	parser.c \
	var_expander.c \
	quotes.c \
	create_commands.c \
	signals.c \
	ft_process.c \
	initialization.c \
	cleanup.c \
	debug.c \
	tests/test_ft_echo.c \
	tests/test_single_cmd.c \
	builtins/echo.c \
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
