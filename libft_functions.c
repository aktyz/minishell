/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:03:10 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/06 18:50:50 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_delete_lst_node(t_list *node);
void	ft_delete_lst(t_list **node, int size);

void	ft_delete_lst_node(t_list *node)
{
	if (node->content)
		free(node->content);
	if (node)
		free(node);
}

void	ft_delete_lst(t_list **node, int size)
{
	t_list		*lst;

	while (size > 0)
	{
		lst = (*node)->next;
		ft_delete_lst_node(*node);
		*node = lst;
		size--;
	}
}
