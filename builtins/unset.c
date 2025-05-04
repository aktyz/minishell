/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:36:57 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 18:25:50 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_unset(t_command *cmd, t_global *global);
static void	ft_delete_env_node(t_list *node, t_global *global);
static void	ft_remove_env_head(t_global *global, bool *is_removed);
static void	ft_remove_node(t_list *node, t_global *global, bool *is_removed);

void	ft_unset(t_command *cmd, t_global *global)
{
	t_list	*lst;
	t_list	*node_to_delete;
	int		i;

	if (!cmd->args[1])
		return ;
	lst = global->env;
	i = 0;
	while (cmd->args[++i])
	{
		node_to_delete = ft_return_env_list_node_ptr(lst, cmd->args[i]);
		ft_delete_env_node(node_to_delete, global);
	}
}

static void	ft_delete_env_node(t_list *node, t_global *global)
{
	bool	is_removed;
	t_list	*current;

	is_removed = false;
	if (!node || !global || !global->env)
		return ;
	if (global->env == node)
		ft_remove_env_head(global, &is_removed);
	if (!is_removed)
		ft_remove_node(node, global, &is_removed);
	if (is_removed)
	{
		current = global->env;
		while (current)
		{
			current->lst_size--;
			current = current->next;
		}
	}
}

static void	ft_remove_env_head(t_global *global, bool *is_removed)
{
	t_list	*head;

	head = global->env;
	global->env = head->next;
	free(((t_minishell_env *)head->content)->name_value[0]);
	free(((t_minishell_env *)head->content)->name_value[1]);
	free(head->content);
	free(head);
	*is_removed = true;
}

static void	ft_remove_node(t_list *node, t_global *global, bool *is_removed)
{
	t_list	*prev;
	t_list	*current;

	prev = NULL;
	current = global->env;
	while (current && current != node)
	{
		prev = current;
		current = current->next;
	}
	if (current == node)
	{
		prev->next = current->next;
		free(((t_minishell_env *)current->content)->name_value[0]);
		free(((t_minishell_env *)current->content)->name_value[1]);
		free(current->content);
		free(current);
	}
	*is_removed = true;
}
