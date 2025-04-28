/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_unnecessary_fds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:21:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 20:58:30 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_unnecessary_fds(t_global *g);
bool	process_env_variable(char *env_var, t_list **list);

void	clean_unnecessary_fds(t_global *g)
{
	t_command	*cmd;

	cmd = g->cmd;
	while (cmd)
	{
		if (!(cmd->command && *cmd->command))
			ft_chandle_parent_io(cmd);
		cmd = cmd->next;
	}
}

bool	process_env_variable(char *env_var, t_list **list)
{
	t_minishell_env	*content;

	content = ft_calloc(sizeof(t_minishell_env), 1);
	if (!content)
	{
		ft_lstclear(list, ft_clear_minishell_env);
		return (false);
	}
	content->name_value = ft_calloc(sizeof(char *), 2);
	if (!content->name_value)
	{
		ft_lstclear(list, ft_clear_minishell_env);
		return (false);
	}
	ft_split_env_variable(env_var, &content->name_value[0],
		&content->name_value[1]);
	if (!content->name_value[0] || !content->name_value[1])
	{
		free(content);
		ft_lstclear(list, ft_clear_minishell_env);
		return (false);
	}
	if (ft_strcmp(content->name_value[0], "_"))
		content->export = true;
	return (ft_lstadd_back(list, ft_lstnew(content)), true);
}
