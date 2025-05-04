/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 18:31:29 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*lst_new_cmd(void)
{
	t_command	*new_cmd;

	new_cmd = ft_calloc(sizeof(t_command), 1);
	if (new_cmd == NULL)
		ft_exit(NULL, "lst_new_cmd", 1);
	new_cmd->pipe_fd[0] = -1;
	new_cmd->pipe_fd[1] = -1;
	new_cmd->cmd_pid = -1;
	return (new_cmd);
}

t_command	*lst_last_cmd(t_list *lst)
{
	while (lst->next != NULL)
		lst = lst->next;
	return ((t_command *) lst->content);
}
