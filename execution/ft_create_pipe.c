/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:56:26 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/29 16:22:22 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_handle_redirections(t_command *cmd);
static void	ft_chandle_child_pipe(t_command *cmd);
static void	ft_chandle_child_io(t_command *cmd);
void		ft_chandle_parent_io(t_command *cmd);
static void	ft_handle_minishell_cats(t_command *cmd);

void	ft_handle_redirections(t_command *cmd)
{
	if (cmd->cmd_pid == 0)
	{
		ft_chandle_child_pipe(cmd);
		ft_chandle_child_io(cmd);
	}
	else if (cmd->cmd_pid > 0 || cmd->cmd_pid == -1)
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
	ft_handle_minishell_cats(cmd);
}

void	ft_chandle_parent_io(t_command *cmd)
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
	if (ft_strcmp(cmd->command, CAT_CAT) == 0
		|| ft_strcmp(cmd->command, CAT_SORT) == 0
		|| ft_strcmp(cmd->command, CAT_UNIQ) == 0
		|| ft_strcmp(cmd->command, CAT_TEE) == 0
		|| ft_strcmp(cmd->command, CAT_TAIL) == 0
		|| ft_strcmp(cmd->command, CAT_HEAD) == 0)
	{
		if (isatty(STDIN_FILENO) != 1
			&& ((cmd->prev && !cmd->prev->pipe_output)
				|| (cmd->io_fds && !cmd->io_fds->infile)))
			ft_attach_tty(cmd);
	}
}
