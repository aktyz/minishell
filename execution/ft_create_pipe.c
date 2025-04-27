/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:56:26 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/27 23:08:34 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_handle_redirections(t_command *cmd);
static void	ft_chandle_child_pipe(t_command *cmd);
static void	ft_chandle_child_io(t_command *cmd);
static void	ft_chandle_parent_io(t_command *cmd);
static void	ft_handle_minishell_cats(t_command *cmd);

void	ft_handle_redirections(t_command *cmd)
{
	if (cmd->cmd_pid == 0)
	{
		ft_handle_minishell_cats(cmd);
		ft_chandle_child_pipe(cmd);
		ft_chandle_child_io(cmd);
	}
	else if (cmd->cmd_pid > 0)
		ft_chandle_parent_io(cmd);
}

static void	ft_chandle_child_pipe(t_command *cmd)
{
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
}

static void	ft_chandle_child_io(t_command *cmd)
{
	if (cmd->io_fds && cmd->io_fds->outfile)
	{
		dup2(cmd->io_fds->fd_out, STDOUT_FILENO);
		close(cmd->io_fds->fd_out);
	}
	if (cmd->io_fds && cmd->io_fds->infile)
	{
		dup2(cmd->io_fds->fd_in, STDIN_FILENO);
		close(cmd->io_fds->fd_in);
	}
}

static void	ft_chandle_parent_io(t_command *cmd)
{
	if (cmd->prev && cmd->prev->pipe_output)
	{
		close(cmd->prev->pipe_fd[0]);
		close(cmd->prev->pipe_fd[1]);
	}
	if (cmd->io_fds && cmd->io_fds->outfile)
		close(cmd->io_fds->fd_out);
	if (cmd->io_fds && cmd->io_fds->infile)
		close(cmd->io_fds->fd_in);
}

static void	ft_handle_minishell_cats(t_command *cmd)
{
	int	tty_fd;

	if (ft_strcmp(cmd->command, CAT_CAT) == 0
		|| ft_strcmp(cmd->command, CAT_SORT) == 0
		|| ft_strcmp(cmd->command, CAT_UNIQ) == 0
		|| ft_strcmp(cmd->command, CAT_TEE) == 0
		|| ft_strcmp(cmd->command, CAT_TAIL) == 0
		|| ft_strcmp(cmd->command, CAT_HEAD) == 0)
	{
		if (cmd->prev) // It's one after a pipe - it will receive from a pipe
			return ; // therefore do nothing
		else // if there's no prev - it's frist in the pipeline
		{
			tty_fd = open("/dev/tty", O_RDONLY);
			if (tty_fd == -1)
			{
				perror("open"); // Print error if open fails
				return;
			}
			if (dup2(tty_fd, STDIN_FILENO) == -1)
			{
				perror("dup2"); // Print error if dup2 fails
				close(tty_fd);
				return;
			}
			close(tty_fd);
		}
	}
}
