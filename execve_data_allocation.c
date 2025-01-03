/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_data_allocation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 05:58:59 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/03 19:17:46 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Function takes structure storing necessary parts to call execve()
 * function and a string containing bash command string with it's arguments.
 * 
 * The last argument is a file name - to be used either as STDIN or STDOUT
 * later down the line.
 *
 */
void	ft_get_executable_data(t_executable **executable, char *cmd,
	char *file_name)
{
	t_executable	*exe;

	exe = *executable;
	ft_allocate_execve_argv(executable, cmd);
	if (*file_name)
	{
		exe->file_name = ft_calloc(sizeof(char), ft_strlen(file_name) + 1);
		ft_strlcpy(exe->file_name, file_name, ft_strlen(file_name));
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
