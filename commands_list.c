/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 17:54:48 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*lst_new_cmd(bool value)
{
	t_command	*new_node;

	new_node = ft_calloc(sizeof(t_command), 1);
	if (new_node == NULL)
		ft_exit(NULL, "lst_new_cmd", 1);
	new_node->pipe_output = value;
	new_node->pipe_fd[0] = -1;
	new_node->pipe_fd[1] = -1;
	new_node->cmd_pid = -1;
	return (new_node);
}

void	lst_add_back_cmd(t_command **alst, t_command *new_node)
{
	t_command	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

t_command	*lst_last_cmd(t_command *cmd)
{
	while (cmd->next != NULL)
		cmd = cmd->next;
	return (cmd);
}
