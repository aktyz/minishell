/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:46:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 10:34:53 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_copy_input_to_final_io(t_io_fds *input, t_command *cmd);
void	ft_copy_output_to_final_io(t_io_fds *output, t_command *cmd,
			t_global *g);

/**
 * Function either creates cmd's final_io, either overwrites the infile
 * file name with the new, latter one.
 *
 */
void	ft_copy_input_to_final_io(t_io_fds *input, t_command *cmd)
{
	if (cmd)
	{
		if (cmd->final_io == NULL)
			cmd->final_io = ft_calloc(sizeof(t_io_fds), 1);// TODO: Create a save function to initialize t_io_fds
		cmd->final_io->fd_in = -1;
		cmd->final_io->fd_out = -1;
		if (cmd->final_io->infile)
			free_ptr((void **) &cmd->final_io->infile);
		cmd->final_io->infile = ft_strdup(input->infile);
	}
}

/**
 * Function either crates cmd's final_io, either overwrites the outfile
 * file name with the new, latter one. Before this replacement it stil
 * creates the file that name's will be overwritten.
 *
 */
void	ft_copy_output_to_final_io(t_io_fds *output, t_command *cmd, t_global *g)
{
	int	create_fd;
	if (cmd)
	{
		if (cmd->final_io == NULL)
		{
			cmd->final_io = ft_calloc(sizeof(t_io_fds), 1);// TODO: Create a save function to initialize t_io_fds
			cmd->final_io->fd_in = -1;
			cmd->final_io->fd_out = -1;
		}
		else if (cmd->final_io->outfile)
		{
			create_fd = open(cmd->final_io->outfile, O_WRONLY | O_CREAT, 0644);
			if (create_fd == -1)
			{
				ft_minishell_perror(g, cmd->final_io->outfile, errno);
				ft_exit(g, cmd->final_io->outfile, 1);
			}
			else
				close(create_fd);
			free_ptr((void **) &cmd->final_io->outfile);
		}
		cmd->final_io->outfile = ft_strdup(output->outfile);
		cmd->final_io->trunc = output->trunc;
	}
	if (cmd->final_io->outfile && cmd->final_io->outfile[0] == '\0')
	{
		errmsg_cmd(cmd->final_io->outfile, NULL, "ambiguous redirect", false);
		return ;
	}
}
