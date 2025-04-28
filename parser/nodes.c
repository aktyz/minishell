/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:12:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:12:49 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_node(char *str, int type, int status)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->str_backup = ft_strdup(str);
	new_node->var_exists = false;
	new_node->type = type;
	new_node->status = status;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	add_node(t_token **list, t_token *new_node)
{
	t_token	*start;

	start = *list;
	if (start == NULL)
	{
		*list = new_node;
		return ;
	}
	if (list && *list && new_node)
	{
		while (start->next != NULL)
		{
			start = start->next;
		}
		start->next = new_node;
		new_node->prev = start;
	}
}

void	delete_node(t_token *node, void (*del)(void *))
{
	if (del && node && node->str)
	{
		(*del)(node->str);
		node->str = NULL;
	}
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
}
