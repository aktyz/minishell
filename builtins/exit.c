/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:53:18 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 20:04:20 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(t_global *global, int status)
{
	if (global)
	{
		// TODO uncomment below once the commands are ready
		// if (global->cmd && global->cmd->io_fds)
		// close_fds(global->cmd, true);
		// TODO make sure everything is cleaned up and no memory leaks
		free_global(global, true);
	}
	if (global->env)
		ft_lstclear(&global->env, ft_clean_minishell_env);
	global->last_exit_code = status;
	exit(status);
}
