/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:56:12 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 17:58:03 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	lst_delone_cmd(t_command *lst, void (*del)(void *))
{
	if (lst->command)
		(*del)(lst->command);
	if (lst->args)
		free_str_tab(lst->args);
	if (lst->io_fds)
		free_io(lst->io_fds);
	(*del)(lst);
}

void	lst_clear_cmd(t_command **lst, void (*del)(void *))
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

/* exit_shell:
*	Cleanly exits the minishell program by closing all opened
*	fds and freeing all allocated memory.
*/
void	exit_shell(t_global *global, int exno)
{
	if (global)
	{
		free_global(global, true);
		if (global && global->env)
			ft_lstclear(&global->env, ft_clean_minishell_env);
	}
	exit(exno);
}

/**
 * Function freeing env_var nodes on the env list
 *
 */
void	ft_clean_minishell_env(void *env_content_node)
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
