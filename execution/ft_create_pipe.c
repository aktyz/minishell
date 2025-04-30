/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:56:26 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/30 19:35:44 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_handle_redirections(t_command *cmd, t_global *g);
static void	ft_chandle_child_pipe(t_command *cmd);
static void	ft_chandle_child_io(t_command *cmd, t_global *g);
void		ft_chandle_parent_io(t_command *cmd);
static void	ft_handle_minishell_cats(t_command *cmd);

void	ft_handle_redirections(t_command *cmd, t_global *g)
{
	if (cmd->cmd_pid == 0)
	{
		ft_chandle_child_pipe(cmd);
		ft_chandle_child_io(cmd, g);
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

static void	ft_chandle_child_io(t_command *cmd, t_global *g)
{
	t_io_fds	*io;
	// here we need to add a lot of stuff iterating through
	// the new list of io structs
	if (!cmd->io_fds)
		return ;
	if (cmd->final_io && cmd->final_io->outfile) // ??
	{
		dup2(io->fd_out, STDOUT_FILENO);
		close(io->fd_out);
		// Moved from remove_old_file_ref
		//if (content->outfile && content->fd_out == -1)
			//return (false);
	}
	if (ft_check_infile_sources(g, cmd))
	{
		cmd->final_io->fd_in = open(cmd->final_io->infile, O_RDONLY);
		if (cmd->final_io->fd_in == -1)
		{
			ft_minishell_perror(g, cmd->final_io->infile, ENOENT);
			ft_exit(g, NULL, EXIT_FAILURE);
		}
		dup2(cmd->final_io->fd_in, STDIN_FILENO);
		close(cmd->final_io->fd_in);
	}
	ft_handle_minishell_cats(cmd); // DEBUG IN THE END
}

void	ft_chandle_parent_io(t_command *cmd)
{
	t_io_fds	*io;
	t_list		*lst;

	// we also need to iterate through the list?
	// no because the infile outfile will be opened by
	// childs -> parent doesn't need to close
	// unless our builtin with redirection
	//if (cmd->io_fds && cmd->io_fds->content)
	//{ // leaving for now, however parent will have nothing
		// to close here, actually only to open in case
		// of parent_builtin
		//lst = cmd->io_fds;
		//io = (t_io_fds *) lst->content;
		//if (io && io->outfile)
		//	close(io->fd_out);
		//if (io && io->infile)
		//	close(io->fd_in);
	//}
	if (cmd->prev && cmd->prev->pipe_output)
	{
		close(cmd->prev->pipe_fd[0]);
		close(cmd->prev->pipe_fd[1]);
	}
}

static void	ft_handle_minishell_cats(t_command *cmd)
{
	t_io_fds	*io;

	if (ft_strcmp(cmd->command, CAT_CAT) == 0
		|| ft_strcmp(cmd->command, CAT_SORT) == 0
		|| ft_strcmp(cmd->command, CAT_UNIQ) == 0
		|| ft_strcmp(cmd->command, CAT_TEE) == 0
		|| ft_strcmp(cmd->command, CAT_TAIL) == 0
		|| ft_strcmp(cmd->command, CAT_HEAD) == 0)
	{
		if (isatty(STDIN_FILENO) != 1)
		{
			if ((cmd && cmd->final_io && !cmd->final_io->infile)
				|| (cmd->prev && !cmd->prev->pipe_output))
				ft_attach_tty(cmd);
		}
	}
}
