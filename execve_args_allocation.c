/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_args_allocation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:53:28 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/03 19:12:47 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_allocate_execve_argv(t_executable **exe, char *cmd);
static void	ft_allocate_execve_arg(t_list **argv, char *cmd, int start_i,
			int nb_chars);

/**
 * Function takes structure storing necessary parts to call execve()
 * function and a string containing bash command string with it's arguments.
 * 
 * Command string is split and store in exe structure as an array and
 * number of arguments of this array is stored in argc element of the
 * structure so that later we can clean the array nicely.
 *
 */
void	ft_allocate_execve_argv(t_executable **exe, char *cmd)
{
	int			i;
	int			start;
	int			len;
	t_list		*argv;

	i = 0;
	start = 0;
	(*exe)->execve_argc = 0;
	argv = NULL;
	len = ft_strlen(cmd);
	while (i <= len)
	{
		if (cmd[i] == ' ')
		{
			ft_allocate_execve_arg(&argv, cmd, start,
				i - start);
			start = start + (i - start) + 1;
			((*exe)->execve_argc)++;
		}
		i++;
	}
	ft_allocate_execve_arg(&argv, cmd, start, i - start);
	((*exe)->execve_argc)++;
	(*exe)->execve_argv = ft_lst_to_arr(argv);
	ft_delete_lst(&argv, (*exe)->execve_argc);
}

/**
 * Function allocates memory for each of the execve arguments t_list
 * and copies the correct part of the program second argument
 * there.
 *
 */
static void	ft_allocate_execve_arg(t_list **argv, char *cmd, int start_i,
			int nb_chars)
{
	t_list		*new;

	new = malloc(sizeof(t_list));
	new->content = malloc(sizeof(char) * (nb_chars + 1));
	ft_strlcpy(new->content, &cmd[start_i], nb_chars);
	new->next = 0;
	new->lst_size = 1;
	ft_lstadd_back(argv, new);
}
