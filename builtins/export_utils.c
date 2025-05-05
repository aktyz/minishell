/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:15:55 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 17:37:08 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_var_name(char *var_name);
void	ft_split_env_variable(char *name_value, char **var_name,
			char **var_value);

bool	is_valid_var_name(char *var_name)
{
	int	i;

	if (!var_name || !var_name[0] || var_name[0] == '='
		|| ft_isdigit(var_name[0]))
		return (false);
	i = 0;
	while (var_name[i])
	{
		if (!(ft_isalpha(var_name[i]) || ft_isdigit(var_name[i])
				|| var_name[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

void	ft_split_env_variable(char *name_value, char **var_name,
		char **var_value)
{
	char	*split_char;
	int		split_index;
	int		total_length;

	split_char = NULL;
	split_char = ft_strchr(name_value, '=');
	if (split_char)
	{
		split_index = split_char - name_value;
		total_length = ft_strlen(name_value);
		*var_name = ft_substr(name_value, 0, split_index);
		*var_value = ft_substr(name_value, split_index + 1, total_length
				- split_index - 1);
	}
	else
		*var_name = ft_strdup(name_value);
}
