/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/03 19:16:47 by zslowian         ###   ########.fr       */
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
void	ft_process(t_process **proc)
{
	t_process	*process;	

	process = *proc;
	if (process->pipe_send)
	{
		close(process->pipe_parent[0]);
		dup2(process->pipe_parent[1], STDOUT_FILENO);
		close(process->pipe_parent[1]);
	}
	else
	{
		close(process->pipe_parent[1]);
		process->out_file_fd = open(process->executable->file_name, O_RDWR | O_CREAT, 0644);
		if (process->out_file_fd == -1)
			ft_error(&proc, NULL);
		if (!access(process->executable->file_name, F_OK))
		{
			dup2(process->out_file_fd, STDOUT_FILENO);
			close(process->out_file_fd);
		}
		else
			ft_error(&proc, NULL);
	}
	if (process->pipe_receive)
	{
		dup2(process->pipe_parent[0], STDIN_FILENO);
		close(process->pipe_parent[0]);
	}
	else if (!access(process->executable->file_name, F_OK & R_OK))
	{
		process->in_file_fd = open(process->executable->file_name, O_RDONLY);
		if (process->in_file_fd == -1)
			ft_error(&proc, NULL);
		dup2(process->in_file_fd, STDIN_FILENO);
		close(process->in_file_fd);
	}
	else
		ft_error(&proc, NULL);
	execve(process->executable->path, process->executable->execve_argv, NULL);
	ft_error(&proc, NULL);
}
