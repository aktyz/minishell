/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirects_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:19:15 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 15:32:05 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_create_file(char *f_name, t_global *g);
void	ft_calloc_io_node(t_io_fds **ptr, t_global *g);
void	ft_open_final_outfile(t_global *g, t_io_fds **io);
void	ft_open_final_infile(t_global *g, t_io_fds **io);
void	ft_handle_child_io_lst_node(t_global *g, t_command *cmd,
			t_io_fds *node);
void	ft_handle_parent_io_lst_node(t_global *g, t_command *cmd,
			t_io_fds *node);

void	ft_create_file(char *f_name, t_global *g)
{
	int	fd;

	fd = open(f_name, O_WRONLY | O_CREAT, 0664);
	if (fd == -1)
	{
		ft_minishell_perror(g, f_name, errno);
		ft_exit(g, f_name, 1);
	}
	else
		close(fd);
}

void	ft_calloc_io_node(t_io_fds **ptr, t_global *g)
{
	*ptr = ft_calloc(sizeof(t_io_fds), 1);
	if (!(*ptr))
	{
		ft_minishell_perror(g, "ft_calloc_io_node", errno);
		ft_exit(g, "ft_calloc_io_node", 1);
	}
	(*ptr)->fd_in = -1;
	(*ptr)->fd_out = -1;
}

void	ft_open_final_outfile(t_global *g, t_io_fds **io)
{
	int	open_flag;

	if ((*io)->trunc)
		open_flag = O_TRUNC;
	else
		open_flag = O_APPEND;
	(*io)->fd_out = open((*io)->outfile,
		O_WRONLY | O_CREAT | open_flag, 0664);
	if ((*io)->fd_out == -1)
	{
		ft_minishell_perror(g, (*io)->outfile, errno);
		ft_exit(g, (*io)->outfile, 1);
	}
	dup2((*io)->fd_out, STDOUT_FILENO);
	close((*io)->fd_out);
}

void	ft_open_final_infile(t_global *g, t_io_fds **io)
{
	(*io)->fd_in = open((*io)->infile, O_RDONLY);
	if ((*io)->fd_in == -1)
	{
		//if ((*io)->outfile) // FOR SOME REASON THIS BELW TESTER UP
		//	ft_craete_file((*io)->outfile, g);
		ft_minishell_perror(g, (*io)->infile, ENOENT);
		ft_exit(g, NULL, EXIT_FAILURE);
	}
	dup2((*io)->fd_in, STDIN_FILENO);
	close((*io)->fd_in);
}

void	ft_handle_child_io_lst_node(t_global *g, t_command *cmd, t_io_fds *node)
{
	if (node->infile)
	{
		if (node->heredoc_delimiter != NULL && node->use_heredoc)
			ft_copy_input_to_final_io(node, cmd, g, true);
		else if (node->heredoc_delimiter == NULL)
			ft_copy_input_to_final_io(node, cmd, g, false);
	}
	if (node->outfile)
		ft_copy_output_to_final_io(node, cmd, g);
}

void	ft_handle_parent_io_lst_node(t_global *g, t_command *cmd, t_io_fds *node)
{
	if (node->infile)
	{
		if (node->heredoc_delimiter != NULL && node->use_heredoc)
			ft_copy_input_to_final_io(node, cmd, g, true);
		else if (node->heredoc_delimiter == NULL)
			ft_copy_input_to_final_io(node, cmd, g, false);
	}
	if (node->outfile)
		ft_copy_output_to_final_io(node, cmd, g);
}
