/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/03/18 16:05:13 by zslowian         ###   ########.fr       */
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
	global->cmd = ft_calloc(1, sizeof(t_command *)); // 
	//- strucutres will be populated from parser
	one_command(&(global->cmd)); // [test execution of "ls -la"] TODO: remove when merging with Marlena's work
	
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
	execve(global->cmd->path, global->cmd->args, NULL);
	//ft_error(&proc, NULL);
}
