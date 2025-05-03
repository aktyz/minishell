/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:46:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 16:39:59 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_copy_input_to_final_io(t_io_fds *node, t_command *cmd,
			t_global *g, bool is_heredoc);
void	ft_copy_output_to_final_io(t_io_fds *node, t_command *cmd,
			t_global *g);

/**
 * Function either creates cmd's final_io, either overwrites the infile
 * name and heredoc information name with the new, latter one.
 *
 */
void	ft_copy_input_to_final_io(t_io_fds *node, t_command *cmd,
			t_global *g, bool is_heredoc)
{
	t_io_fds	*io;

	if (cmd)
	{
		if (cmd->final_io == NULL)
			ft_calloc_io_node(&cmd->final_io, g);
		io = cmd->final_io;
		io->infile = ft_strdup(node->infile);
		if (is_heredoc)
		{
			io->heredoc_delimiter = ft_strdup(node->heredoc_delimiter);
			io->heredoc_quotes = node->heredoc_quotes;
		}
		if (io->heredoc_delimiter == NULL
			&& access(node->infile, F_OK) == -1)
		{
			if (io && io->outfile)
				ft_create_file(io->outfile, g);
			ft_minishell_perror(node->infile, ENOENT);
			ft_exit(g, NULL, 1);
		}
	}
}

/**
 * Function either crates cmd's final_io, either overwrites the outfile
 * file name with the new, latter one. Before this replacement it stil
 * creates the file that's name will be overwritten.
 *
 */
void	ft_copy_output_to_final_io(t_io_fds *node, t_command *cmd, t_global *g)
{
	if (cmd)
	{
		if (cmd->final_io == NULL)
			ft_calloc_io_node(&cmd->final_io, g);
		else if (cmd->final_io->outfile)
		{
			ft_create_file(cmd->final_io->outfile, g);
			free_ptr((void **) &cmd->final_io->outfile);
		}
		cmd->final_io->outfile = ft_strdup(node->outfile);
		cmd->final_io->trunc = node->trunc;
	}
	if (cmd->final_io->outfile && cmd->final_io->outfile[0] == '\0')
	{
		errmsg_cmd(cmd->final_io->outfile, NULL, "ambiguous redirect", false);
		return ;
	}
}
