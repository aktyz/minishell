/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_ft_get_execve_data.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 05:58:59 by zslowian          #+#    #+#             */
/*   Updated: 2024/12/16 21:39:35 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * Function parses second argument of the program into separate execve
 * arguments put in an array.
 * 
 * The same array have two additional fields: input data & NULL.
 * Input data is a try.
 * NULL is required in order to execve run smoothly.
 * 
 * This function also tries executable paths to find the right binary.
 *  
 */
void	ft_get_executable_data(t_executable **executable, char *cmd, char *in_file)
{
	t_executable	*exe;

	exe = *executable;
	ft_allocate_execve_argv(executable, cmd);
	if (*in_file)
	{
		exe->infile_name = ft_calloc(sizeof(char), ft_strlen(in_file) + 1);
		ft_strlcpy(exe->infile_name, in_file, ft_strlen(in_file));
	}
	exe->path = ft_strjoin(PATH_1,
		exe->execve_argv[0]);
	if (access(exe->path, X_OK) == -1)
	{
		free(exe);
		exe->path = ft_strjoin(PATH_2,
				exe->execve_argv[0]);
		if (access(exe->path, X_OK) == -1)
		{
			free(exe);
		}
	}
}
