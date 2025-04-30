/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/30 12:04:03 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	remove_old_file_ref(t_io_fds *io, bool infile, t_global *g)
// Those are checks I need to transfer to ft_create_piping
// but the function itself is not needed here
{
	if (infile == true && io->infile)
	{
		if (io->outfile && io->fd_out == -1)
			return (false);
		if (io->heredoc_delimiter != NULL)
		{
			free_ptr((void **)&io->heredoc_delimiter);
			io->heredoc_delimiter = NULL;
			unlink(io->infile);
		}
		if (access(io->infile, F_OK) == -1)
		{
			ft_minishell_perror(NULL, io->infile, ENOENT);
			ft_exit(g, NULL, 1);
		}
		free_ptr((void **)&io->infile);
	}
	else if (infile == false && io->outfile)
	{
		if (io->fd_out == -1)
			return (false);
		free_ptr((void **)&io->outfile);
		close(io->fd_out);
	}
	return (true);
}

/**
 * Function creates new io_fds_struct and adds it to the cmd->io list
 */
static void	add_io_infile_data(t_global *global, t_command *cmd, char *f_name)
{
	t_io_fds	*new;

	new = ft_calloc(sizeof(t_io_fds), 1);
	new->fd_in = -1;
	new->fd_out = -1;
	new->infile = ft_strdup(f_name);
	ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
	if (new->infile && new->infile[0] == '\0') // debug - in parent or in child?
	{
		errmsg_cmd(new->infile, NULL, strerror(errno), false);
		ft_exit(global, NULL, EXIT_FAILURE);
	}
}

/**
 * Function adds new entry on io_fds list,
 */
void	parse_input(t_global *g, t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	add_io_infile_data(g, cmd, temp->next->str);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

static void	open_outfile_trunc(t_io_fds *io, char *file, char *var_filename,
				t_global *g)
{
	if (!remove_old_file_ref(io, false, g))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0')
	{
		errmsg_cmd(var_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (io->fd_out == -1)
		errmsg_cmd(io->outfile, NULL, strerror(errno), false);
}

void	parse_trunc(t_command **last_cmd, t_token **token_lst, t_global *g)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_outfile_trunc(cmd->io_fds, temp->next->str, temp->next->str_backup, g);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}
