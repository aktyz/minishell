/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_args_allocation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 03:53:28 by zslowian          #+#    #+#             */
/*   Updated: 2025/03/19 17:15:15 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_allocate_execve_argv(t_command **cmd, char *str);
static char	**ft_arr_to_argv_array(char **argv);

/**
 * Function takes structure storing necessary parts to call execve()
 * function and a string containing bash command string with it's arguments.
 * 
 * Command string is split and store in cmd structure as a array and
 * number of arguments of this array is stored in argc element of the
 * structure so that later we can clean the array nicely.
 *
 */
void	ft_allocate_execve_argv(t_command **cmd, char *str)
{
	int			len;
	char		**args;
	char		**tmp;

	(*cmd)->args_size = 0;
	args = ft_split((const char *) str, ' ');
	(*cmd)->args = ft_arr_to_argv_array(args);
	tmp = (*cmd)->args;
	while (*tmp)
	{
		((*cmd)->args_size)++;
		tmp++;
	}
	(*cmd)->command = (*cmd)->args[0];
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
	i = size + 1;
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
