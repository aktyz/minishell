/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:10:06 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 18:18:38 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	change_status_to_quote(t_token **token_node, int *i)
{
	if ((*token_node)->str[*i] == '\'')
		(*token_node)->status = SQUOTE;
	if ((*token_node)->str[*i] == '\"')
		(*token_node)->status = DQUOTE;
	(*i)++;
}

static bool	if_quotes_and_default(t_token **token_node, int i)
{
	if (((*token_node)->str[i] == '\'' || (*token_node)->str[i] == '\"')
		&& (*token_node)->status == DEFAULT)
		return (true);
	else
		return (false);
}

static bool	change_back_to_default(t_token **token_node, int *i)
{
	if (((*token_node)->str[*i] == '\'' && (*token_node)->status == SQUOTE)
		|| ((*token_node)->str[*i] == '\"' && (*token_node)->status == DQUOTE))
	{
		(*token_node)->status = DEFAULT;
		(*i)++;
		return (true);
	}
	else
		return (false);
}

int	remove_quotes(t_token **token_node)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = malloc(sizeof(char) * count_len((*token_node)->str, i, i));
	if (!new_line)
		return (1);
	while ((*token_node)->str[i])
	{
		if (if_quotes_and_default(token_node, i))
		{
			change_status_to_quote(token_node, &i);
			continue ;
		}
		else if (change_back_to_default(token_node, &i))
			continue ;
		new_line[j++] = (*token_node)->str[i++];
	}
	new_line[j] = '\0';
	free_ptr((void **)&(*token_node)->str);
	(*token_node)->str = new_line;
	(*token_node)->join = true;
	return (0);
}

int	handle_quotes(t_global *global)
{
	t_token	*temp;

	temp = global->token;
	while (temp)
	{
		if (quotes_in_string(temp->str)
			&& (!temp->prev || (temp->prev && temp->prev->type != HEREDOC)))
			remove_quotes(&temp);
		temp = temp->next;
	}
	return (0);
}
