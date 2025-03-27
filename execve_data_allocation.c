/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_data_allocation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 05:58:59 by zslowian          #+#    #+#             */
/*   Updated: 2025/03/19 13:31:43 by zslowian         ###   ########.fr       */
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
void	ft_get_path_and_args(t_command **command, char *str,
	char *file_name)
{
	t_command	*cmd;

	cmd = *command;
	(void) file_name; // t_command will have file FD in case of file servicing
	ft_allocate_execve_argv(command, str);
	cmd->path = ft_strjoin(PATH_1,
		cmd->args[0]);
	if (access(cmd->path, X_OK) == -1)
	{
		free(cmd);
		cmd->path = ft_strjoin(PATH_2,
				cmd->args[0]);
		if (access(cmd->path, X_OK) == -1)
		{
			free(cmd);
		}
	}
}
