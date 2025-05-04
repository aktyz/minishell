/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:14:57 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 20:50:26 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_sort_export_list(t_list **list);
static void	ft_swap_nodes(t_list **current, t_list **next, bool *swapped);

/**
 * Simple bubble sort function for our env list, used when printing
 * export with no arguments. No issues with the sorting mechanizm.
 *
 */
void	ft_sort_export_list(t_list **list)
{
	t_list			*current;
	t_list			*next;
	bool			swapped;

	if (!list || !*list)
		return ;
	swapped = true;
	while (swapped)
	{
		swapped = false;
		current = *list;
		while (current->next)
		{
			next = current->next;
			if (ft_strcmp(((t_minishell_env *)current->content)->name_value[0],
					((t_minishell_env *)next->content)->name_value[0]) > 0)
				ft_swap_nodes(&current, &next, &swapped);
			current = current->next;
		}
	}
}

static void	ft_swap_nodes(t_list **current, t_list **next, bool *swapped)
{
	t_minishell_env	*temp_content;

	temp_content = (*current)->content;
	(*current)->content = (*next)->content;
	(*next)->content = temp_content;
	*swapped = true;
}
