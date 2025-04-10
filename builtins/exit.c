/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:53:18 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/10 18:05:13 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_global *data)
{
	if(data)
	{
		// TODO uncomment below once the commands are ready
		// if (data->cmd && data->cmd->io_fds)
			// close_fds(data->cmd, true);
		// TODO make sure everything is cleaned up and no memory leaks
		free_global(data, true);
	}
	exit(EXIT_SUCCESS);
}
