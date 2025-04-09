/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:53:18 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/09 16:44:44 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_global *data)
{
	if(data)
	{
		// TODO uncomment below once the commands are ready
		// if (global->cmd && global->cmd->io_fds)
			// close_fds(global->cmd, true);
		free_global(data, true);
	}
	exit(EXIT_SUCCESS);
}
