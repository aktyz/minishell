/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_unnecessary_fds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:21:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 17:26:56 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	process_env_variable(char *env_var, t_list **list);
void	add_io_heredoc_data(t_global *g, t_command *cmd, char *delimiter);

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

void	add_io_heredoc_data(t_global *g, t_command *cmd, char *delimiter)
{
	t_io_fds	*new;
	t_list		*lst;
	t_io_fds	*content;

	if (cmd->io_fds)
	{
		lst = cmd->io_fds;
		while (lst && lst->content)
		{
			content = (t_io_fds *) lst->content;
			if (content->use_heredoc)
			{
				unlink(content->infile);
				//free_ptr((void **) &content->heredoc_delimiter);
				//free_ptr((void **) &content->infile);
				content->use_heredoc = false;
			}
			lst = lst->next;
		}
	}
	ft_calloc_io_node(&new, g);
	new->infile = get_heredoc_name();
	new->heredoc_delimiter = get_delim(delimiter, &(new->heredoc_quotes));
	new->use_heredoc = true;
	if (!get_heredoc(g, new))
	{
		ft_minishell_perror(g, new->infile, errno);
		ft_exit(g, new->infile, 1);
	}
	if (cmd->io_fds)
		ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
	else
		cmd->io_fds = ft_lstnew(new);
}
