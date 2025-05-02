/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 14:12:40 by zslowian         ###   ########.fr       */
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

char	*get_delim(char *delim, bool *quotes)
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

void	parse_heredoc(t_global *g, t_command **cmd,
			t_token **token_lst)
{
	t_token		*temp;
	t_command	*lst_cmd;

	temp = *token_lst;
	lst_cmd = lst_last_cmd(*cmd);
	add_io_infile_data(g, lst_cmd, temp->next->str, true);
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
			parse_output(&global->cmd, &temp, global, true);
		else if (temp->type == HEREDOC)
			parse_heredoc(global, &global->cmd, &temp);
		else if (temp->type == APPEND)
			parse_output(&global->cmd, &temp, global, false);
		else if (temp->type == PIPE)
			parse_pipe(&global->cmd, &temp);
		else if (temp->type == END)
			break ;
	}
	prep_no_arg_commands(global);
}
