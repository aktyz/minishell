/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_unnecessary_fds.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:21:20 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 20:45:11 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		add_io_heredoc_data(t_global *g, t_command *cmd, char *delimiter);
static void	ft_check_preceedent_heredocs(t_list *cmd_io);
char		*get_heredoc_name(void);

void	add_io_heredoc_data(t_global *g, t_command *cmd, char *delimiter)
{
	t_io_fds	*new;

	if (cmd->io_fds)
		ft_check_preceedent_heredocs(cmd->io_fds);
	ft_calloc_io_node(&new, g);
	new->infile = get_heredoc_name();
	new->heredoc_delimiter = get_delim(delimiter, &(new->heredoc_quotes));
	new->use_heredoc = true;
	if (!get_heredoc(g, new))
	{
		ft_minishell_perror(new->infile, errno);
		ft_exit(g, new->infile, 1);
	}
	if (cmd->io_fds)
		ft_lstadd_back(&cmd->io_fds, ft_lstnew(new));
	else
		cmd->io_fds = ft_lstnew(new);
}

static void	ft_check_preceedent_heredocs(t_list *cmd_io)
{
	t_list		*lst;
	t_io_fds	*content;

	lst = cmd_io;
	while (lst && lst->content)
	{
		if (lst->content)
			content = (t_io_fds *) lst->content;
		else
			return ;
		if (content->use_heredoc)
		{
			unlink(content->infile);
			content->use_heredoc = false;
		}
		lst = lst->next;
	}
}

char	*get_heredoc_name(void)
{
	static int	i;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin(HEREDOC_NAME, number);
	free(number);
	i++;
	return (name);
}
