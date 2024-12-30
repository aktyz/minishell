/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_ft_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2024/12/16 21:48:10 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_process(t_process **pipex)
{
	t_process	*process;	

	process = *pipex;
	// Closing pipes on ends we won't need and setting up STDOUT
	if (process->pipe_send)
	{
		close(process->pipe_parent[0]);
		dup2(process->pipe_parent[1], STDOUT_FILENO);
		close(process->pipe_parent[1]);
	}
	else
	{
		close(process->pipe_parent[1]);
		process->out_file_fd = open(process->executable->infile_name, O_RDWR | O_CREAT, 0644);
		if (process->out_file_fd == -1)
			ft_error(&pipex, NULL);
		if (!access(process->executable->infile_name, F_OK))
		{
			dup2(process->out_file_fd, STDOUT_FILENO);
			close(process->out_file_fd);
		}
		else
			ft_error(&pipex, NULL);
	}
	// Setting up the STDIN
	if (process->pipe_receive)
	{
		dup2(process->pipe_parent[0], STDIN_FILENO);
		close(process->pipe_parent[0]);
	}
	else if (!access(process->executable->infile_name, F_OK & R_OK))
	{
		process->in_file_fd = open(process->executable->infile_name, O_RDONLY);
		if (process->in_file_fd == -1)
			ft_error(&pipex, NULL);
		dup2(process->in_file_fd, STDIN_FILENO);
		close(process->in_file_fd);
	}
	else
		ft_error(&pipex, NULL);
	// Execution
	execve(process->executable->path, process->executable->execve_argv, NULL);
	// Error log in case of failure
	ft_error(&pipex, NULL);
}
