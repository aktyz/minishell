/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirects_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:19:15 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 15:28:33 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
