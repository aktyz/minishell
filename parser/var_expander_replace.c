/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_replace.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:13:09 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 18:25:24 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Changed return type from int to char * to adapt the function
// to work for heredoc variable expansion. Heredoc has no tokens
// so token_node becomes optional.
// Heredoc variant replace_str_heredoc calls this function with
// token_node == NULL!
static char	*erase_and_replace(t_token **token_node, char *str,
			char *var_value, int index)
{
	char	*newstr;
	int		newstr_size;

	newstr_size = (ft_strlen(str) - var_length(str + index)
			+ ft_strlen(var_value));
	newstr = get_new_token_string(str, var_value, newstr_size, index);
	if (token_node && *token_node)
	{
		free_ptr((void **)&(*token_node)->str);
		(*token_node)->str = NULL;
		(*token_node)->str = newstr;
	}
	return (newstr);
}

int	replace_var(t_token **token_node, char *var_value, int index)
{
	if (var_value == NULL)
	{
		var_value = malloc(1);
		var_value[0] = '\0';
	}
	if (erase_and_replace(token_node, (*token_node)->str, \
	var_value, index) == NULL)
	{
		free_ptr((void **)&var_value);
		return (var_value = NULL, 1);
	}
	free_ptr((void **)&var_value);
	return (var_value = NULL, 0);
}

/* replace_str_heredoc:
*	Heredoc variant of replace_var, replaces an environment variable
*	by its value. Ex. $USER -> username.
*	Returns the replaced string.
*/
char	*replace_str_heredoc(char *str, char *var_value, int index)
{
	char	*tmp;

	tmp = NULL;
	if (var_value == NULL)
		*str = '\0';
	else
	{
		tmp = str;
		str = erase_and_replace(NULL, str, var_value, index);
		free_ptr((void **)&tmp);
	}
	free_ptr((void **)&var_value);
	return (var_value = NULL, str);
}
