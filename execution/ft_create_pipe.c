/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:56:26 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 17:31:48 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_handle_redirections(t_command *cmd, t_global *g,
				t_command *prev_cmd);
static void	ft_chandle_child_pipe(t_command *cmd, t_command *prev_cmd);
static void	ft_chandle_child_io(t_command *cmd, t_global *g,
				t_command *prev_cmd);
void		ft_chandle_parent_io(t_command *cmd, t_global *g,
				t_command *prev_cmd);
static void	ft_handle_minishell_cats(t_command *cmd, t_command *prev_cmd);

void	ft_handle_redirections(t_command *cmd, t_global *g,
			t_command *prev_cmd)
{
	if (cmd->cmd_pid == 0)
	{
		ft_chandle_child_pipe(cmd, prev_cmd);
		ft_chandle_child_io(cmd, g, prev_cmd);
	}
	else if (cmd->cmd_pid > 0 || cmd->cmd_pid == -1)
		ft_chandle_parent_io(cmd, g, prev_cmd);
}

static void	ft_chandle_child_pipe(t_command *cmd, t_command *prev_cmd)
{
	if (cmd->pipe_output)
	{
		close(cmd->pipe_fd[0]);
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
		close(cmd->pipe_fd[1]);
	}
	if (prev_cmd && prev_cmd->pipe_output)
	{
		close(prev_cmd->pipe_fd[1]);
		dup2(prev_cmd->pipe_fd[0], STDIN_FILENO);
		close(prev_cmd->pipe_fd[0]);
	}
}

/**
 * This function is a wrapper around an iterator of io_fds list:
 * It looks for infiles, outfiles and heredocs on the list,
 * translating them into the final_io for the cmd, which
 * is then opened for the execution.
 *
 */
static void	ft_chandle_child_io(t_command *cmd, t_global *g,
				t_command *prev_cmd)
{
	t_list		*head;
	t_io_fds	*node;

	if (!cmd->io_fds)
		return ;
	head = cmd->io_fds;
	while (head)
	{
		node = (t_io_fds *) head->content;
		ft_handle_child_io_lst_node(g, cmd, node);
		head = head->next;
	}
	if (cmd->final_io)
	{
		if (cmd->final_io->infile)
			ft_open_final_infile(g, &cmd->final_io);
		if (cmd->final_io->outfile)
			ft_open_final_outfile(g, &cmd->final_io);
	}
	ft_handle_minishell_cats(cmd, prev_cmd); // DEBUG IN THE END
}

/**
 * Parent process will enter into this in two different cases:
 * 1. cmd_pid > 0 - where it doesn't need to do anything - fd closing
 * 	and opening are happening insite of the forked child process
 * 2. cmd_pid == -1 - where it needs to handle io - it is running
 * 	one of it's built-in processes, which:
 * 		-> should ignore any inputs
 * 		-> outputs should be set up with STDOUT backup to be
 * 			restored once the parent process executed cmd built-in
 */
void	ft_chandle_parent_io(t_command *cmd, t_global *g,
			t_command *prev_cmd)
{
	t_io_fds	*node;
	t_list		*lst;

	node = NULL;
	lst = NULL;
	if (cmd->io_fds)
		lst = cmd->io_fds;
	if (cmd->cmd_pid == -1)
	{
		while (lst)
		{
			node = (t_io_fds *) lst->content;
			ft_handle_parent_io_lst_node(g, cmd, node);
			lst = lst->next;
		}
	}
	else if (prev_cmd && prev_cmd->pipe_output && prev_cmd->cmd_pid != -1)
	{
		close(prev_cmd->pipe_fd[0]);
		close(prev_cmd->pipe_fd[1]);
	}
	if (cmd->final_io && cmd->final_io->outfile)
	{
		cmd->stdout_backup = create_stdout_backup();
		ft_open_final_outfile(g, &cmd->final_io);
	}
}

static void	ft_handle_minishell_cats(t_command *cmd, t_command *prev_cmd)
{
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
				|| (prev_cmd && !prev_cmd->pipe_output))
				ft_attach_tty();
		}
	}
}
