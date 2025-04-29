/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:13:09 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:31:00 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_variable_character(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/**
 * returns number of characters in the first encountered variable that
 * starts with $
 */
int	var_length(char *str)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (str[i] != '$')
		i++;
	i++;
	if ((str[i] >= '0' && str[i] <= '9') || str[i] == '?')
		return (count + 1);
	while (str[i])
	{
		if (!is_variable_character(str[i]))
			break ;
		count++;
		i++;
	}
	return (count);
}

void	copy_var_value(char *new_str, char *var_value, int *j)
{
	int	k;

	k = 0;
	while (var_value[k])
	{
		new_str[*j] = var_value[k];
		k++;
		(*j)++;
	}
}

bool	is_next_char_a_sep(char c)
{
	return (c == '$' || c == ' ' || c == '=' || c == '\0');
}

bool	var_between_quotes(char *str, int i)
{
	return (i > 0 && str[i - 1] == '\"' && str[i + 1] == '\"');
}
