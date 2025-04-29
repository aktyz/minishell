/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:56:12 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 20:06:34 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	restore_io(t_io_fds *io);
void	lst_delone_cmd(t_command *lst, void (*del)(void **));
void	lst_clear_cmd(t_command **lst, void (*del)(void **));
void	ft_clear_minishell_env(void *env_content_node);
void	ft_clear_token(t_token	**list);

/* restore_io:
*	Restores the original standard input and standard output
*	to their original fds of 0 and 1. Used to clear the input/output
*	fds after execution, in preparation for the next set of user commands.
*	Returns 1 if the duplication was successful, 0 if not.
*/
bool	restore_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	if (io->stdin_backup != -1)
	{
		if (dup2(io->stdin_backup, STDIN_FILENO) == -1)
			ret = false;
		close(io->stdin_backup);
		io->stdin_backup = -1;
	}
	if (io->stdout_backup != -1)
	{
		if (dup2(io->stdout_backup, STDOUT_FILENO) == -1)
			ret = false;
		close(io->stdout_backup);
		io->stdout_backup = -1;
	}
	return (ret);
}

void	lst_delone_cmd(t_command *lst, void (*del)(void **))
{
	if (lst->command)
		(*del)((void **)&lst->command);
	if (lst->path)
		(*del)((void **)&lst->path);
	if (lst->args)
		free_str_tab(lst->args);
	if (lst->io_fds)
		free_io(lst->io_fds);
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
