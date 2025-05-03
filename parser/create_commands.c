/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 16:54:30 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	populate_args_if_null(t_global *global);
char		*get_delim(char *delim, bool *quotes);
void		parse_heredoc(t_global *g, t_command **curr_cmd,
				t_token **curr_token);
void		create_commands(t_global *global);
static void	ft_switch_parsing_ft(t_global *g, t_command **curr_cmd,
				t_token **curr_token);

static void	populate_args_if_null(t_global *global)
{
	t_list		*lst;
	t_command	*curr_cmd;

	if (!global || !global->cmd)
		return ;
	lst = global->cmd;
	while (lst && lst->content)
	{
		curr_cmd = (t_command *) lst->content;
		if (!curr_cmd->args)
		{
			curr_cmd->args = malloc(sizeof * curr_cmd->args * 2);
			curr_cmd->args[0] = ft_strdup(curr_cmd->command);
			curr_cmd->args[1] = NULL;
			curr_cmd->args_size = 2;
		}
		lst = lst->next;
	}
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

void	parse_heredoc(t_global *g, t_command **curr_cmd,
			t_token **curr_token)
{
	t_token		*temp;

	temp = *curr_token;
	add_io_heredoc_data(g, *curr_cmd, temp->next->str);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*curr_token = temp;
}

void	create_commands(t_global *global)
{
	t_token		*curr_token;
	t_command	*curr_cmd;

	curr_token = global->token;
	curr_cmd = NULL;
	if (curr_token->type == END)
		return ;
	while (curr_token->next != NULL)
	{
		if (curr_token == global->token)
		{
			global->cmd = ft_lstnew(lst_new_cmd());
			curr_cmd = (t_command *) global->cmd->content;
		}
		ft_switch_parsing_ft(global, &curr_cmd, &curr_token);
		if (curr_token->type == END)
			break ;
	}
	populate_args_if_null(global);
}

static void	ft_switch_parsing_ft(t_global *g, t_command **curr_cmd,
	t_token **curr_token)
{
	if ((*curr_token)->type == WORD || (*curr_token)->type == VAR)
		parse_word(curr_cmd, curr_token);
	else if ((*curr_token)->type == INPUT)
		parse_input(g, curr_cmd, curr_token);
	else if ((*curr_token)->type == TRUNC)
		parse_output(g, curr_cmd, curr_token, true);
	else if ((*curr_token)->type == HEREDOC)
		parse_heredoc(g, curr_cmd, curr_token);
	else if ((*curr_token)->type == APPEND)
		parse_output(g, curr_cmd, curr_token, false);
	else if ((*curr_token)->type == PIPE)
		parse_pipe(g, curr_cmd, curr_token);
}
