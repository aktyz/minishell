/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_append_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 16:14:59 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_pipe(t_global *g, t_command **curr_cmd, t_token **curr_token)
{
	(*curr_cmd)->pipe_output = true;
	ft_lstadd_back(&g->cmd, ft_lstnew(lst_new_cmd()));
	*curr_cmd = lst_last_cmd(g->cmd);
	*curr_token = (*curr_token)->next;
}
