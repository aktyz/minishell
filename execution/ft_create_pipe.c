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
	if (cmd->cmd_pid == 0) // child processes
	{
		if (cmd->pipe_output) // it will be sending
		{
			close(cmd->pipe_fd[0]); // close the reading end
			dup2(cmd->pipe_fd[1], STDOUT_FILENO); // dup stdout to the writing end of the pipe
			close(cmd->pipe_fd[1]);
		}
		if (cmd->prev && cmd->prev->pipe_output) // it will be receiving
		{
			close(cmd->prev->pipe_fd[1]); // close the writing end
			dup2(cmd->prev->pipe_fd[0], STDIN_FILENO); // dup stdin to the reading end of the pipe
			close(cmd->prev->pipe_fd[0]);
		}
	}
	else // parent and built-ins
	{
		if (cmd->cmd_pid == -1 || (cmd->prev && cmd->prev->cmd_pid == -1)) // built-in given two built-ins cannot follow one another as cmds
		{
			if (cmd->prev->cmd_pid == -1 && cmd->prev->pipe_output) // it will be sending
			{
				close(cmd->prev->pipe_fd[0]); // close the reading end
				dup2(cmd->prev->pipe_fd[1], STDOUT_FILENO); // dup stdout to the writing end of the pipe
				close(cmd->prev->pipe_fd[1]);
			}
			if (cmd->prev && cmd->prev->pipe_output) // it will be receiving
			{
				close(cmd->prev->pipe_fd[1]); // close the writing end
				dup2(cmd->prev->pipe_fd[0], STDIN_FILENO); // dup stdin to the reading end of the pipe
				close(cmd->prev->pipe_fd[0]);
			}
		}
		else // parent
		{
			if (cmd->prev->pipe_output)
			{
				close(cmd->prev->pipe_fd[0]);
				close(cmd->prev->pipe_fd[1]);
			}
		}
	}

}
