/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_files.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 15:32:38 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Function creates new io_fds_struct and adds it to the cmd->io list
 * because of the existing token with f_name as an input
 *
 */
void	add_io_infile_data(t_global *g, t_command *cmd, char *f_name,
			bool is_heredoc)
{
	t_io_fds	*new;
	t_io_fds	*heredoc_node;
	t_list		*lst;

	heredoc_node = NULL; // super important to be here
	new = NULL;
	if (cmd->io_fds) // we are checking where to write new input data
	{
		if (is_heredoc)
		{
			lst = cmd->io_fds;
			while (lst && lst->content)
			{
				heredoc_node = (t_io_fds *) lst->content;
				if (heredoc_node->heredoc_delimiter)
					break ;
				lst = lst->next;
			}
			if (heredoc_node->heredoc_delimiter)
			{
				unlink(heredoc_node->infile);
				free_ptr((void **) &heredoc_node->heredoc_delimiter);
				free_ptr((void **) &heredoc_node->infile);
				new = heredoc_node;
			}
			else
				heredoc_node = NULL;
		}
	}
	if (!new)
		ft_calloc_io_node(&new, g);
	if (is_heredoc) // we are writing new input data
	{
		new->infile = get_heredoc_name();
		new->heredoc_delimiter = get_delim(f_name, &(new->heredoc_quotes));
		if (!get_heredoc(g, new))
		{
			ft_minishell_perror(g, new->infile, errno);
			ft_exit(g, new->infile, 1);
		}
	}
	else
		new->infile = ft_strdup(f_name);
	if (!cmd->io_fds)
		cmd->io_fds = ft_lstnew(new);
	else if (!heredoc_node)
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
void	parse_input(t_global *g, t_command **cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*lst_cmd;

	temp = *token_lst;
	lst_cmd = lst_last_cmd(*cmd);
	add_io_infile_data(g, lst_cmd, temp->next->str, false);
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
void	parse_output(t_command **cmd, t_token **token_lst, t_global *g,
			bool is_trunc)
{
	t_token		*temp;
	t_command	*lst_cmd;

	temp = *token_lst;
	lst_cmd = lst_last_cmd(*cmd);
	add_io_outfile_data(g, lst_cmd, temp->next->str, is_trunc);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}
