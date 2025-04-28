/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 19:58:55 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prep_no_arg_commands(t_global *global)
{
	t_command	*cmd;

	if (!global || !global->cmd)
		return ;
	cmd = global->cmd;
	while (cmd && cmd->command)
	{
		if (!cmd->args)
		{
			cmd->args = malloc(sizeof * cmd->args * 2);
			cmd->args[0] = ft_strdup(cmd->command);
			cmd->args[1] = NULL;
			cmd->args_size = 2;
		}
		cmd = cmd->next;
	}
	cmd = lst_last_cmd(global->cmd);
}

static char	*get_delim(char *delim, bool *quotes)
{
	int	len;

	len = ft_strlen(delim) - 1;
	if ((delim[0] == '\"' && delim[len] == '\"')
		|| (delim[0] == '\'' && delim[len] == '\''))
	{
		*quotes = true;
		return (ft_strtrim(delim, "\'\""));
	}
	return (ft_strdup(delim));
}

static char	*get_heredoc_name(void)
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

void	parse_heredoc(t_global *global, t_command **last_cmd,
			t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;
	t_io_fds	*io;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	io = cmd->io_fds;
	if (!remove_old_file_ref(io, true))
		return ;
	io->infile = get_heredoc_name();
	io->heredoc_delimiter = get_delim(temp->next->str, &(io->heredoc_quotes));
	if (get_heredoc(global, io))
		io->fd_in = open(io->infile, O_RDONLY);
	else
		io->fd_in = -1;
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

void	create_commands(t_global *global, t_token *token)
{
	t_token	*temp;

	temp = token;
	if (temp->type == END)
		return ;
	while (temp->next != NULL)
	{
		if (temp == global->token)
			lst_add_back_cmd(&global->cmd, lst_new_cmd(false));
		if (temp->type == WORD || temp->type == VAR)
			parse_word(&global->cmd, &temp, global);
		else if (temp->type == INPUT)
			parse_input(global, &global->cmd, &temp);
		else if (temp->type == TRUNC)
			parse_trunc(&global->cmd, &temp);
		else if (temp->type == HEREDOC)
			parse_heredoc(global, &global->cmd, &temp);
		else if (temp->type == APPEND)
			parse_append(&global->cmd, &temp);
		else if (temp->type == PIPE)
			parse_pipe(&global->cmd, &temp);
		else if (temp->type == END)
			break ;
	}
	prep_no_arg_commands(global);
}
