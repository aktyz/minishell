/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_env_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 18:07:34 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/05 19:16:42 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_env_var(char *var_name, char **env)
{
	char	*res;
	char	**i;
	int		len;

	res = NULL;
	i = env;
	len = ft_strlen(var_name);
	while (*i)
	{
		if (ft_strncmp(*i, var_name, len) == 0)
		{
			res = ft_substr(*i, len + 1, ft_strlen(*i) - len - 1);
			break ;
		}
		i++;
	}
	return (res);
}
