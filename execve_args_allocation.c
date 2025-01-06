/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_args_allocation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:53:28 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/06 19:22:57 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_allocate_execve_argv(t_executable **exe, char *cmd);
static char	**ft_arr_to_argv_array(char **argv);

/**
 * Function takes structure storing necessary parts to call execve()
 * function and a string containing bash command string with it's arguments.
 * 
 * Command string is split and store in exe structure as a list and
 * number of arguments of this list is stored in argc element of the
 * structure so that later we can clean the array nicely.
 *
 */
void	ft_allocate_execve_argv(t_executable **exe, char *cmd)
{
	int			len;
	char		**args;
	char		**tmp;

	(*exe)->execve_argc = 0;
	args = ft_split((const char *) cmd, ' ');
	(*exe)->execve_argv = ft_arr_to_argv_array(args);
	tmp = (*exe)->execve_argv;
	while (*tmp)
	{
		((*exe)->execve_argc)++;
		tmp++;
	}
}

static char	**ft_arr_to_argv_array(char **argv)
{
	int		size;
	int		i;
	char	**arr;
	char	**tmp;
	char	**t;

	size = 0;
	tmp = argv;
	while (*tmp)
	{
		size++;
		tmp++;
	}
	i = size + 1; //For null entry in the array
	arr = malloc(sizeof(char *) * i);
	if (!arr)
		return (NULL);
	tmp = arr;
	t = argv;
	while (i > 1)
	{
		*tmp = ft_calloc(sizeof(char), ft_strlen(*t) + 1);
		ft_strlcpy(*tmp, *t, ft_strlen(*t));
		t++;
		tmp++;
		i--;
	}
	*tmp = NULL;
	ft_clear_char_array(&argv, size - 1);
	return (arr);
}
