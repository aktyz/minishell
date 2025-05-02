/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:46:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 15:30:06 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_copy_input_to_final_io(t_io_fds *input, t_command *cmd,
			t_global *g, bool is_heredoc);
void	ft_copy_output_to_final_io(t_io_fds *output, t_command *cmd,
			t_global *g);

/**
 * Function either creates cmd's final_io, either overwrites the infile
 * file name with the new, latter one.
 *
 */
void	ft_copy_input_to_final_io(t_io_fds *input, t_command *cmd,
			t_global *g, bool is_heredoc)
{
	if (cmd)
	{
		if (cmd->final_io == NULL)
			ft_calloc_io_node(&cmd->final_io, g);
		else if (cmd->final_io->infile)
		{
			if (cmd->final_io->heredoc_delimiter == NULL && access(input->infile, F_OK) == -1)
			{
				if (cmd->final_io && cmd->final_io->outfile)
					ft_create_file(cmd->final_io->outfile, g);
				ft_minishell_perror(g, input->infile, ENOENT);
				ft_exit(g, NULL, 1);
			}
			free_ptr((void **) &cmd->final_io->infile);
		}
		cmd->final_io->infile = ft_strdup(input->infile);
		if (is_heredoc)
		{
			cmd->final_io->heredoc_delimiter = ft_strdup(input->heredoc_delimiter);
			cmd->final_io->heredoc_quotes = input->heredoc_quotes;
			cmd->final_io->fd_in = input->fd_in;
		}
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
			ft_calloc_io_node(&cmd->final_io, g);
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
