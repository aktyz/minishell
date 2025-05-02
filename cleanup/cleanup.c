/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:56:12 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 19:03:12 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear_minishell_cmd(void *cmd_content_node);
void	ft_clear_minishell_env(void *env_content_node);
void	ft_clear_token(t_token	**list);

void	ft_clear_minishell_cmd(void *cmd_content_node)
{
	t_command *content;

	content = (t_command *) cmd_content_node;
	if (!content)
		return ;
	if (content->command)
		free_ptr((void **) content->command);
	if (content->path)
		free_ptr((void **) content->path);
	if (content->args)
		ft_clear_char_array(&content->args, content->args_size);
	if (content->io_fds)
		ft_lstclear(&content->io_fds, free_io);
	if (content->final_io)
		free_io(content->final_io);
	if (content)
		free(content);
	cmd_content_node = NULL;
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
