/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_unnecessary_fds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:21:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/27 20:55:16 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_unnecessary_fds(t_global *g)
{
	t_command	*cmd;

	cmd = g->cmd;
	while (cmd)
	{
		if (!(cmd->command && *cmd->command))
			ft_chandle_parent_io(cmd); // close all the fds because they
			// won't be used with any command as it is missing
			// we need to delete those nodes as well?
			// I am not sure but we shouldn't in order to
			// achieve this "broken pipe" errors
		cmd = cmd->next;
	}
}
