/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/30 16:51:43 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	remove_old_file_ref(t_io_fds *io, bool infile, t_global *g)
// Those are checks I need to transfer to ft_create_piping
// but the function itself is not needed here
{
	if (infile == true && io->infile)
	{
		
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
 * because of the existing token with f_name as an input
 *
 */
static void	add_io_infile_data(t_global *global, t_command *cmd, char *f_name)
{
	t_io_fds	*new;

	new = ft_calloc(sizeof(t_io_fds), 1);
	new->fd_in = -1;
	new->fd_out = -1;
	new->infile = ft_strdup(f_name);
	if (!cmd->io_fds)
		cmd->io_fds = ft_lstnew(new);
	else
		ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
	if (new->infile && new->infile[0] == '\0') // debug - in parent or in child?
	{
		errmsg_cmd(new->infile, NULL, strerror(errno), false);
		ft_exit(global, NULL, EXIT_FAILURE);
	}
}

/**
 * Function adds new entry on io_fds list, with infile name
 *
 */
void	parse_input(t_global *g, t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	add_io_infile_data(g, cmd, temp->next->str);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

/**
 * Function creates new io_fds_struct and adds it to the cmd->io list
 * because of the existing token with f_name as an output
 *
 */
static void	add_io_outfile_data(t_global *g, t_command *cmd, char *f_name)
{
	t_io_fds	*new;
	
	new = ft_calloc(sizeof(t_io_fds), 1);
	new->fd_in = -1;
	new->fd_out = -1;
	new->outfile = ft_strdup(f_name);
	ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
	if (new->outfile && new->outfile[0] == '\0')  // debug - in parent or in child?
	{
		errmsg_cmd(f_name, NULL, "ambiguous redirect", false);
		return ;
	}
	new->fd_out = open(new->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664); // move to child 100%
	if (new->fd_out == -1)
		errmsg_cmd(new->outfile, NULL, strerror(errno), false);
}

/**
 * Function adds new entry on the io_fds list, with outfile name
 * (what about heredoc?)
 *
 */
void	parse_trunc(t_command **last_cmd, t_token **token_lst, t_global *g)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	add_io_outfile_data(g, *last_cmd, temp->next->str);	
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}
