/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:46:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/01 12:34:30 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool		ft_check_infile_sources(t_global *g, t_command *cmd);
static void	ft_copy_input_to_final_io(t_io_fds *input, t_command *cmd);
bool		ft_check_outfile_sources(t_global *g, t_command *cmd);
static void	ft_copy_output_to_final_io(t_io_fds *output, t_command *cmd,
				t_global *g);


bool	ft_check_infile_sources(t_global *g, t_command *cmd)
{
	t_list		*lst;
	t_io_fds	*content;
	bool		is_input;

	is_input = false;
	if (cmd && cmd->io_fds)
		lst = cmd->io_fds;
	while (lst && lst->content)
	{
		content = (t_io_fds *) lst->content;
		if (content->infile)
		{
			if (content->heredoc_delimiter != NULL) // ??
			{
				free_ptr((void **)&content->heredoc_delimiter);
				content->heredoc_delimiter = NULL;
				unlink(content->infile);
			}
			if (access(content->infile, F_OK) == -1)
			{
				ft_minishell_perror(g, content->infile, ENOENT);
				ft_exit(g, NULL, 1);
			}
			else
			{
				is_input = true;
				ft_copy_input_to_final_io(content, cmd);
			}
		}
		lst = lst->next;
	}
	return (is_input);
}

bool		ft_check_outfile_sources(t_global *g, t_command *cmd)
{
	t_list		*lst;
	t_io_fds	*content;
	bool		is_output;

	is_output = false;
	if (cmd && cmd->io_fds)
		lst = cmd->io_fds;
	while (lst && lst->content)
	{
		content = (t_io_fds *) lst->content;
		if (content->outfile)
		{
			is_output = true;
			ft_copy_output_to_final_io(content, cmd, g);
		}
		lst = lst->next;
	}
	return (is_output);
}

/**
 * Function either creates cmd's final_io, either overwrites the infile
 * file name with the new, latter one.
 *
 */
static void	ft_copy_input_to_final_io(t_io_fds *input, t_command *cmd)
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

static void	ft_copy_output_to_final_io(t_io_fds *output, t_command *cmd, t_global *g)
{
	if (cmd)
	{
		if (cmd->final_io == NULL)
		{
			cmd->final_io = ft_calloc(sizeof(t_io_fds), 1);// TODO: Create a save function to initialize t_io_fds
			cmd->final_io->fd_in = -1;
			cmd->final_io->fd_out = -1;
		}
		else
		{
			if (cmd->final_io->trunc)
			{
				cmd->final_io->fd_out = open(cmd->final_io->outfile,
					O_WRONLY | O_CREAT | O_TRUNC, 0664);
				if (cmd->final_io->fd_out == -1)
				{
					ft_minishell_perror(g, cmd->final_io->outfile, errno);
					ft_exit(g, cmd->final_io->outfile, 1);
				}
				close(cmd->final_io->fd_out);
			}
			else
			{     // if no path->mkdir :(
				cmd->final_io->fd_out = open(cmd->final_io->outfile,
					O_WRONLY | O_CREAT | O_APPEND, 0664);
				if (cmd->final_io->fd_out == -1)
				{
					ft_minishell_perror(g, cmd->final_io->outfile, errno);
					ft_exit(g, cmd->final_io->outfile, 1);
				}
				close(cmd->final_io->fd_out);
			}
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
