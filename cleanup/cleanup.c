/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:56:12 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 14:12:38 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	lst_delone_cmd(t_command *lst, void (*del)(void **));
void	lst_clear_cmd(t_command **lst, void (*del)(void **));
void	ft_clear_minishell_env(void *env_content_node);
void	ft_clear_token(t_token	**list);

void	lst_delone_cmd(t_command *lst, void (*del)(void **))
{
	if (lst->command)
		(*del)((void **)&lst->command);
	if (lst->path)
		(*del)((void **)&lst->path);
	if (lst->args)
		free_str_tab(lst->args);
	if (lst->io_fds)
		ft_lstclear(&lst->io_fds, free_io);
	if (lst->final_io)
		free_io((void *) lst->final_io);
	(*del)((void **)&lst);
}

void	lst_clear_cmd(t_command **lst, void (*del)(void **))
{
	t_command	*temp;

	temp = NULL;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		lst_delone_cmd(*lst, del);
		*lst = temp;
	}
}

/**
 * Function freeing env_var nodes on the env list
 *
 */
void	ft_clear_minishell_env(void *env_content_node)
{
	t_minishell_env	*content;

	content = (t_minishell_env *) env_content_node;
	if (!content)
		return ;
	if (content->name_value[0])
	{
		free(content->name_value[0]);
		content->name_value[0] = NULL;
	}
	if (content->name_value[1])
	{
		free(content->name_value[1]);
		content->name_value[1] = NULL;
	}
	if (content->name_value)
	{
		free(content->name_value);
		content->name_value = NULL;
	}
	if (content)
		free(content);
	env_content_node = NULL;
}

void	ft_clear_token(t_token	**list)
{
	t_token	*head;
	t_token	*tmp;

	if (list && *list)
	{
		head = *list;
		while (head)
		{
			tmp = head->next;
			if (head->str)
				free_ptr((void **)&head->str);
			if (head->str_backup)
				free_ptr((void **)&head->str_backup);
			if (head)
				free_ptr((void **)&head);
			head = tmp;
		}
		*list = NULL;
	}
}
