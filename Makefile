# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/04 15:20:00 by zslowian          #+#    #+#              #
#    Updated: 2024/12/16 19:18:26 by zslowian         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
LIBFT = libft
LIBFT_F = ./libft
INCLUDES = -I $(LIBFT_F)/headers
CC = cc
FLAGS = -Wall -Wextra -Werror
LIB = -L $(LIBFT_F) -lft
RM = rm -f

SRC = pipex.c \
	pipex_arg_split.c \
	pipex_ft_get_execve_data.c \
	pipex_ft_get_input_data.c \
	pipex_ft_lst_to_arr.c \
	pipex_ft_process.c

OBJ = $(SRC:.c=.o)

%.o : %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g -pthread

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@git submodule update --init --recursive
	@$(MAKE) -C $(LIBFT_F)

fclean: clean
	@$(RM) $(NAME) $(OBJ)
	@$(RM) ./understanding_pipex/a.out

clean:
	@$(RM) $(OBJ)
	@$(MAKE) -C $(LIBFT_F) fclean

re: fclean all

debug: $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -g -pthread

.PHONY: all clean fclean libft re