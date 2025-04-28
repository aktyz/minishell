/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:13:09 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:31:00 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_status(t_token **token_node, char c)
{
	if (c == '\'' && (*token_node)->status == DEFAULT)
		(*token_node)->status = SQUOTE;
	else if (c == '\"' && (*token_node)->status == DEFAULT)
		(*token_node)->status = DQUOTE;
	else if (c == '\'' && (*token_node)->status == SQUOTE)
		(*token_node)->status = DEFAULT;
	else if (c == '\"' && (*token_node)->status == DQUOTE)
		(*token_node)->status = DEFAULT;
}

// NOTE: why both global is passed and token_lst?
// cannot simply take global->token and have this as one argument function?
// TODO add test cases for this
int	var_expander(t_global *global, t_token **token_lst)
{
	t_token	*temp;
	int		i;

	temp = *token_lst;
	while (temp)
	{
		if (temp->type == VAR)
		{
			i = 0;
			while (temp->str[i])
			{
				update_status(&temp, temp->str[i]);
				if (temp->str[i] == '$'
					&& !is_next_char_a_sep(temp->str[i + 1])
					&& !var_between_quotes(temp->str, i)
					&& (temp->status == DEFAULT || temp->status == DQUOTE))
					replace_var(&temp,
						recover_val(temp, temp->str + i, global), i);
				else
					i++;
			}
		}
		temp = temp->next;
	}
	return (0);
}

/* var_expander_heredoc:
*	Heredoc variant of var_expander. Replaces a string containing an
*	environment variable, like $USER with its corresponding value.
*	Returns the replaced string. May return NULL on error.
*/
char	*var_expander_heredoc(t_global *global, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$'
			&& !is_next_char_a_sep(str[i + 1])
			&& !var_between_quotes(str, i))
			str = replace_str_heredoc(str,
					recover_val(NULL, str + i, global), i);
		else
			i++;
	}
	return (str);
}
