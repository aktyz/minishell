/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/03/12 13:45:12 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Function that will eventually call our executable.
 * It contains three parts:
 * - setting up the STDOUT for the executable
 * - setting up the STDIN for the executable
 * - executing using execve() function
 *
 */
void	ft_process(t_global *global)
{
	t_command	*command;

	global->cmd = ft_calloc(1, sizeof(t_command *));
	command = global->cmd;
	one_command(&command);
	// I don't need to close anything because I don't serve pipes yet
	/**if (command->pipe_send)
	{
		close(command->pipe_parent[0]);
		dup2(command->pipe_parent[1], STDOUT_FILENO);
		close(command->pipe_parent[1]);
	}
	else
	{
		close(command->pipe_parent[1]);
		command->out_file_fd = open(command->executable->file_name, O_RDWR | O_CREAT, 0644);
		if (command->out_file_fd == -1)
			ft_error(&proc, NULL);
		if (!access(command->executable->file_name, F_OK))
		{
			dup2(command->out_file_fd, STDOUT_FILENO);
			close(command->out_file_fd);
		}
		else
			ft_error(&proc, NULL);
	}
	if (command->pipe_receive)
	{
		dup2(command->pipe_parent[0], STDIN_FILENO);
		close(command->pipe_parent[0]);
	}
	else if (!access(command->executable->file_name, F_OK & R_OK))
	{
		command->in_file_fd = open(command->executable->file_name, O_RDONLY);
		if (command->in_file_fd == -1)
			ft_error(&proc, NULL);
		dup2(command->in_file_fd, STDIN_FILENO);
		close(command->in_file_fd);
	}
	else
		ft_error(&proc, NULL);*/
	execve(command->path, command->args, NULL);
	//ft_error(&proc, NULL);
}
