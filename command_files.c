/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 20:55:25 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Function creates new io_fds_struct and adds it to the cmd->io list
 * because of the existing token with f_name as an input
 *
 */
void	add_io_infile_data(t_global *g, t_command *cmd, char *f_name)
{
	t_io_fds	*new;

	if (!new)
		ft_calloc_io_node(&new, g);
	new->infile = ft_strdup(f_name);
	if (!cmd->io_fds)
		cmd->io_fds = ft_lstnew(new);
	else
		ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
	if (new->infile && new->infile[0] == '\0') // debug - in parent or in child?
	{
		errmsg_cmd(new->infile, NULL, strerror(errno), false);
		ft_exit(g, NULL, EXIT_FAILURE);
	}
}

/**
 * Function adds new entry on io_fds list, with infile name
 *
 */
void	parse_input(t_global *g, t_command **curr_cmd, t_token **curr_token)
{
	t_token		*temp;

	temp = *curr_token;
	add_io_infile_data(g, *curr_cmd, temp->next->str);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*curr_token = temp;
}

/**
 * Function creates new io_fds_struct and adds it to the cmd->io list
 * because of the existing token with f_name as an output
 *
 */
static void	add_io_outfile_data(t_global *g, t_command *cmd, char *f_name,
	bool is_trunc)
{
	t_io_fds	*new;

	ft_calloc_io_node(&new, g);
	new->outfile = ft_strdup(f_name);
	new->trunc = is_trunc;
	if (!cmd->io_fds)
		cmd->io_fds = ft_lstnew(new);
	else
		ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
}

/**
 * Function adds new entry on the io_fds list, with outfile name
 * The content of the file will be overwritten
 *
 */
void	parse_output(t_global *g, t_command **curr_cmd, t_token **curr_token,
			bool is_trunc)
{
	t_token		*temp;

	temp = *curr_token;
	add_io_outfile_data(g, *curr_cmd, temp->next->str, is_trunc);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*curr_token = temp;
}
