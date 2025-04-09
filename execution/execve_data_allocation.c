/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_data_allocation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 05:58:59 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/06 16:21:54 by zslowian         ###   ########.fr       */
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
	(void) file_name;
	ft_allocate_execve_argv(command, str);
	//cmd->path = resolve_command_path(extract_env_var(ENV_PATH, global->env));
}
