/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_arg_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:53:28 by zslowian          #+#    #+#             */
/*   Updated: 2024/12/16 21:37:36 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_allocate_execve_argv(t_executable **exe, char *cmd);
void	ft_allocate_execve_arg(t_list **argv, char *cmd, int start_i,
			int nb_chars);

/**
 * Function is a parser of the second argument of the program. It split this
 * argument by space character and allocates command members in a list
 * in pipex structure
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
void	ft_allocate_execve_arg(t_list **argv, char *cmd, int start_i,
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
