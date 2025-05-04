# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/04 15:20:00 by zslowian          #+#    #+#              #
#    Updated: 2025/05/03 15:51:51 by zslowian         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
LIBFT = libft
LIBFT_F = ./libft
INCLUDES = -I ./includes -I $(LIBFT_F)/headers
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIB = -L $(LIBFT_F) -lft
RM = rm -f

SRC = main.c \
	builtins/builtins_helpers.c \
	builtins/cd.c \
	builtins/echo.c \
	builtins/env.c \
	builtins/exit.c \
	builtins/export_utils.c \
	builtins/export.c \
	builtins/pwd.c \
	builtins/unset.c \
	cleanup/cleanup.c \
	cleanup/free.c \
	command_files.c \
	command_utils.c \
	command_vars.c \
	commands_append_pipe.c \
	commands_heredoc.c \
	commands_list.c \
	command_args.c \
	command_args_echo.c \
	debug/debug.c \
	errors/errors.c \
	execution/ft_create_pipe.c \
	execution/ft_exec_utils.c \
	execution/ft_input.c \
	execution/ft_process.c \
	execution/ft_redirections.c \
	execution/ft_redirects_utils.c \
	execution/ft_run_builtin.c \
	parser/characters.c \
	parser/clean_unnecessary_fds.c \
	parser/create_commands.c \
	parser/initialization.c \
	parser/nodes.c \
	parser/parser.c \
	parser/quotes.c \
	parser/signals.c \
	parser/utils_for_tokens.c \
	parser/var_expander.c \
	parser/var_expander_env.c \
	parser/var_expander_utils.c \
	parser/var_expander_replace.c \
	tokens_list.c \
	ft_utils.c \

OBJ = $(SRC:.c=.o)

%.o : %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g -pthread

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -l readline

$(LIBFT):
	@git submodule update --init --recursive
	$(MAKE) -C $(LIBFT_F)

fclean: clean
	@$(RM) $(NAME) $(OBJ)
	@$(MAKE) -C $(LIBFT_F) fclean

clean:
	@$(RM) $(OBJ)
#	@$(MAKE) -C $(LIBFT_F) clean

re: fclean all

rebug: fclean debug clean

debug: $(OBJ) #$(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -l readline -g -pthread

.PHONY: all clean fclean libft re
