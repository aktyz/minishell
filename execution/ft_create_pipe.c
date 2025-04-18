/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:56:26 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/11 21:20:49 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_redirections(t_command *cmd)
{
	if(cmd->cmd_pid == 0) // currently serving a child
	{
		// PIPING
		if (cmd->pipe_output)
		{
			close(cmd->pipe_fd[0]);
			dup2(cmd->pipe_fd[1], STDOUT_FILENO);
			close(cmd->pipe_fd[1]);
		}
		if (cmd->prev && cmd->prev->pipe_output)
		{
			close(cmd->prev->pipe_fd[1]);
			dup2(cmd->prev->pipe_fd[0], STDIN_FILENO);
			close(cmd->prev->pipe_fd[0]);
		}
		// IO FILES
		if (cmd->io_fds && cmd->io_fds->outfile)
		{
			dup2(cmd->io_fds->fd_out, STDOUT_FILENO);
			close(cmd->io_fds->fd_out);
		}
		if (cmd->io_fds && cmd->io_fds->infile) // follow up- will we handle heredoc here
		{
			dup2(cmd->io_fds->fd_in, STDIN_FILENO);
			close(cmd->io_fds->fd_in);
		}
	}
	else if (cmd->cmd_pid > 0) // currently serving parent
	{
		// PIPING
		if (cmd->prev && cmd->prev->pipe_output)
		{
			close(cmd->prev->pipe_fd[0]);
			close(cmd->prev->pipe_fd[1]);
		}
		// IO FILES
		if (cmd->io_fds && cmd->io_fds->outfile)
			close(cmd->io_fds->fd_out);
		if (cmd->io_fds && cmd->io_fds->infile) // follow up- will we handle heredoc here
			close(cmd->io_fds->fd_in);
	}
}
